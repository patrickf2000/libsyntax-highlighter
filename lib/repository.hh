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

#include <QStringList>
#include <QString>

namespace CppLib {

class Repository {
public:
	/// Initialize the repository
	/// This MUST be called first, as it performs functions needed to find the files.
    explicit Repository();
	///
	/// Returns the absolute path to the syntax file with given name.
    QString fileForName(QString name);
	///
	/// Returns the name based on the path to a specific syntax file
    QString getNameForFile(QString file);
	///
	/// Returns the absolute path to a syntax file based on the MIME type of the path passed in.
    QString getFileByType(QString file);
	///
	/// Returns all available syntax names.
    QStringList *allNames();
	///
	/// Returns all available syntax files.
    QStringList *allFiles();
protected:
    QString getName(QString file);
    bool hasMime(QString file, QString mime);
    bool hasExtension(QString file, QString syntaxFile);
private:
    QStringList *files;
    QString syntaxPath;
    void initConstructor(QString path);
    void load();
};

}
