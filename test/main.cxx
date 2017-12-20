#include <iostream>
#include <QStringList>
#include <repository.hh>
#include <QApplication>

#include "testwindow.hh"

int main(int argc, char **argv) {
    Repository *repo = new Repository;
    QStringList *names = repo->allNames();
    std::cout << "Syntax Names:" << std::endl;
    for (int i = 0; i<names->size(); i++) {
        std::cout << names->at(i).toStdString() << std::endl;
    }
    std::cout << std::endl;

    QApplication app(argc,argv);

    TestWindow win;
    win.show();

    return app.exec();
}
