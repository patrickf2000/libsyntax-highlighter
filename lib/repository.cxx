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
#include <QFile>
#include <QDir>
#include <QMimeDatabase>
#include <iostream>
#include <QByteArray>

#include "repository.hh"
#include "tinyxml2.h"

using namespace CppLib;
using namespace tinyxml2;

#ifdef _WIN32
Repository::Repository() {
    QString path = qgetenv("ProgramData");
    if (!path.endsWith("\\")) {
        path+="\\";
    }
    path+="syntax-highlighting";
    initConstructor(path);
}
#elif HAIKU_OS
Repository::Repository() {
	QString path = QDir::homePath();
	if (!path.endsWith("/")) {
		path+="/";	
	}
    path+="config/syntax-highlighting";
	if (!QDir(path).exists()) {
		QDir().mkpath(path);	
	}
	initConstructor(path);
}
#else
Repository::Repository() {
    initConstructor("/usr/share/syntax-highlighting");
}
#endif

QString Repository::fileForName(QString name) {
    QString file = "";
    for (int i = 0; i<files->size(); i++) {
        if (getName(files->at(i))==name) {
            file = files->at(i);
            break;
        }
    }
    if (file!="") {
        file = syntaxPath+file;
    }
    return file;
}

QString Repository::getNameForFile(QString file) {
    return getName(file);
}

QString Repository::getFileByType(QString file) {
    QString mimeType = QMimeDatabase().mimeTypeForFile(file).name();
    std::cout << "MimeType: " << mimeType.toStdString() << std::endl;
    QString sf = "";
    for (int i = 0; i<files->size(); i++) {
        if (hasMime(files->at(i),mimeType)) {
            sf = syntaxPath+files->at(i);
        } else if (hasExtension(file,files->at(i))) {
            sf = syntaxPath+files->at(i);
        }
    }
    return sf;
}

QStringList *Repository::allNames() {
    QStringList *names = new QStringList;
    for (int i = 0; i<files->size(); i++) {
        QString current = files->at(i);
        QString name = getName(current);
        names->push_back(name);
    }
    return names;
}

QStringList *Repository::allFiles() {
    return files;
}

QString Repository::getName(QString file) {
    QString path = file;
    if (!file.startsWith(syntaxPath)) {
        path = syntaxPath+file;
    }
    QString ID = "";
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(path.toStdString().c_str());
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

bool Repository::hasMime(QString file, QString mime) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile((syntaxPath+file).toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("syntax");
    if (root==nullptr) {
        return false;
    }
    XMLElement *mimeType = root->FirstChildElement("mime");
    if (mimeType==nullptr) {
        return false;
    }
    XMLElement *lastMime;
    while (mimeType!=nullptr) {
        QString data = QString(mimeType->Attribute("type"));
        if (data==mime) {
            return true;
        }
        lastMime = mimeType;
        mimeType = lastMime->NextSiblingElement("mime");
    }
    return false;
}

bool Repository::hasExtension(QString file, QString syntaxFile) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile((syntaxPath+syntaxFile).toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("syntax");
    if (root==nullptr) {
        return false;
    }
    XMLElement *mimeType = root->FirstChildElement("extension");
    if (mimeType==nullptr) {
        return false;
    }
    XMLElement *lastMime;
    while (mimeType!=nullptr) {
        QString data = QString(mimeType->Attribute("name"));
        if (file.endsWith(data)) {
            return true;
        }
        lastMime = mimeType;
        mimeType = lastMime->NextSiblingElement("extension");
    }
    return false;
}

void Repository::initConstructor(QString path) {
    syntaxPath = path;
    if (!syntaxPath.endsWith("/")) {
        syntaxPath+="/";
    }
    files = new QStringList;
    load();
}

void Repository::load() {
    QDir dir(syntaxPath);
    QStringList entries = dir.entryList(QDir::Files);
    for (int i = 0; i<entries.size(); i++) {
        files->push_back(entries.at(i));
    }
}
