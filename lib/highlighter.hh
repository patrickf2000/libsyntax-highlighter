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
#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QString>
#include <QVector>
#include <QStringList>

#include "rules.hh"

namespace CppLib {

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
	/// Initializes a new syntax highlighter
	/// @param doc
	/// The QTextEdit's QTextDocument
	/// @param File
	/// The absolute path to the desired syntax file
    explicit SyntaxHighlighter(QTextDocument *doc, QString File);
	///
	/// Returns the current name for the current syntax highlighting file
    QString getCurrentName();
	///
	/// Returns the current syntax highlighting file being used.
    QString getCurrentFile();
protected:
    void highlightBlock(const QString &text);
private:
    QVector<Rule> *rules;
    QVector<MultilineCommentRule> *mlComments;
    QString file, id;
    QString loadId();
    QVector<Rule> *getSingleSyntaxRules();
    QVector<MultilineCommentRule> *getMultilineSyntaxRules();
};

}
