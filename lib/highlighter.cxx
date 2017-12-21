// Copyright 2017 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//	list of conditions and the following disclaimer in the documentation and/or 
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//	be used to endorse or promote products derived from this software 
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <QRegExp>
#include <QVariant>
#include <iostream>

#include "highlighter.hh"
#include "tinyxml2.h"

using namespace CppLib;
using namespace tinyxml2;

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *doc, QString File) : QSyntaxHighlighter(doc) {
    file = File;
    id = loadId();
    rules = getSingleSyntaxRules();
    mlComments = getMultilineSyntaxRules();
}

QString SyntaxHighlighter::getCurrentName() {
    return id;
}

QString SyntaxHighlighter::getCurrentFile() {
    return file;
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    for (int i = 0; i<rules->size(); i++) {
        Rule r = rules->at(i);
        QRegExp exp(r.pattern);
        int index = exp.indexIn(text);
        while (index>=0) {
            this->setFormat(index,exp.matchedLength(),r.format);
            index = exp.indexIn(text,index+exp.matchedLength());
        }
    }
    this->setCurrentBlockState(0);
    int start = 0;
    for (int i = 0; i<mlComments->size(); i++) {
        MultilineCommentRule r = mlComments->at(i);
        QRegExp startExp(r.startPattern);
        QRegExp endExp(r.endPattern);
        if (this->previousBlockState()!=1) {
            start = startExp.indexIn(text);
        }
        while (start>=0) {
            int end = endExp.indexIn(text,start);
            int length;
            if (end==-1) {
                this->setCurrentBlockState(1);
                length = text.length() - start;
            } else {
                length = end-start+endExp.matchedLength();
            }
            this->setFormat(start,length,r.format);
            start = startExp.indexIn(text,start+length);
        }
    }
}

QString SyntaxHighlighter::loadId() {
    QString ID = "";
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("syntax");
    if (root==nullptr) {
        return "Plain Text";
    }
    ID = QString(root->Attribute("name"));
    if (ID==nullptr) {
        return "Plain Text";
    }
    return ID;
}

QVector<Rule> *SyntaxHighlighter::getSingleSyntaxRules() {
    QVector<Rule> *list = new QVector<Rule>();
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("syntax");
    if (root==nullptr) {
        return list;
    }
    XMLElement *singleRoot = root->FirstChildElement("single");
    if (singleRoot==nullptr) {
        return list;
    }
    XMLElement *group = singleRoot->FirstChildElement("group");
    XMLElement *lastGroup;
    while (group!=nullptr) {
        Rule r;
        QTextCharFormat f;
        QString color = "#000000";
        int weight = 50;
        XMLElement *formatRoot = group->FirstChildElement("format");
        if (formatRoot!=nullptr) {
            color = QString(formatRoot->Attribute("color"));
            if (color==nullptr) {
                color = "#000000";
            }
            QString weightStr = QString(formatRoot->Attribute("weight"));
            if (weightStr!=nullptr) {
                weight = QVariant(weightStr).toInt();
            }
        }
        f.setFontWeight(weight);
        f.setForeground(QBrush(QColor(color)));
        r.format = f;
        QString start = "";
        QString end = "";
        XMLElement *patternRoot = group->FirstChildElement("pattern");
        if (patternRoot!=nullptr) {
            start = QString(patternRoot->FirstChildElement("start")->GetText());
            if (start==nullptr) {
                start = "";
            }
            end = QString(patternRoot->FirstChildElement("end")->GetText());
            if (end==nullptr) {
                end = "";
            }
        }
        XMLElement *item = group->FirstChildElement("item");
        XMLElement *lastItem;
        while (item!=nullptr) {
            QString data = QString(item->GetText());
            QString pattern = start+data+end;
            r.pattern = pattern;
            list->push_back(r);
            lastItem = item;
            item = lastItem->NextSiblingElement("item");
        }
        XMLElement *useClass = group->FirstChildElement("use-class");
        lastItem = nullptr;
        while (useClass!=nullptr) {
            QString name = QString(useClass->Attribute("name"));
            XMLElement *global = root->FirstChildElement("global");
            XMLElement *classElement = global->FirstChildElement("class");
            XMLElement *lastClassElement;
            while (classElement!=nullptr) {
                QString className = QString(classElement->Attribute("name"));
                if (className==name) {
                    item = classElement->FirstChildElement("item");
                    lastItem = nullptr;
                    while (item!=nullptr) {
                        QString text = QString(item->GetText());
                        QString pattern = start+text+end;
                        r.pattern = pattern;
                        list->push_back(r);
                        lastItem = item;
                        item = lastItem->NextSiblingElement("item");
                    }
                }
                lastClassElement = classElement;
                classElement = lastClassElement->NextSiblingElement("class");
            }
            lastItem = useClass;
            useClass = lastItem->NextSiblingElement("use-class");
        }
        lastGroup = group;
        group = lastGroup->NextSiblingElement("group");
    }
    return list;
}

QVector<MultilineCommentRule> *SyntaxHighlighter::getMultilineSyntaxRules() {
    QVector<MultilineCommentRule> *list = new QVector<MultilineCommentRule>();
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("syntax");
    if (root==nullptr) {
        return list;
    }
    XMLElement *multiRoot = root->FirstChildElement("multi");
    if (multiRoot==nullptr) {
        return list;
    }
    XMLElement *group = multiRoot->FirstChildElement("group");
    XMLElement *lastGroup;
    while (group!=nullptr) {
        MultilineCommentRule r;
        QTextCharFormat f;
        QString color = "#000000";
        int weight = 50;
        XMLElement *formatRoot = group->FirstChildElement("format");
        if (formatRoot!=nullptr) {
            color = QString(formatRoot->Attribute("color"));
            if (color==nullptr) {
                color = "#000000";
            }
            QString weightStr = QString(formatRoot->Attribute("weight"));
            if (weightStr!=nullptr) {
                weight = QVariant(weightStr).toInt();
            }
        }
        f.setFontWeight(weight);
        f.setForeground(QBrush(QColor(color)));
        r.format = f;
        XMLElement *item = group->FirstChildElement("item");
        XMLElement *lastItem;
        while (item!=nullptr) {
            XMLElement *firstElement = item->FirstChildElement("start");
            QString first = "";
            if (firstElement==nullptr) {
                lastItem = item;
                item = lastItem->NextSiblingElement("item");
                continue;
            } else {
                first = QString(firstElement->GetText());
            }
            XMLElement *endElement = item->FirstChildElement("end");
            QString end = "";
            if (endElement==nullptr) {
                lastItem = item;
                item = lastItem->NextSiblingElement("item");
                continue;
            } else {
                end = QString(endElement->GetText());
            }
            r.startPattern = first;
            r.endPattern = end;
            list->push_back(r);
            lastItem = item;
            item = lastItem->NextSiblingElement("item");
        }
        lastGroup = group;
        group = lastGroup->NextSiblingElement("group");
    }
    return list;
}
