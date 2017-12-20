#include "testwindow.hh"
#include "ui_testwindow.h"

#include <repository.hh>
#include <highlighter.hh>

TestWindow::TestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);

    Repository *repo = new Repository;
    QStringList *names = repo->allNames();
    for (int i = 0; i<names->size(); i++) {
        ui->listWidget->addItem(names->at(i));
    }
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QTextEdit *edit = ui->textEdit;
    Repository *repo = new Repository;
    QString file = repo->fileForName(item->text());
    SyntaxHighlighter *h = new SyntaxHighlighter(edit->document(),file);
}
