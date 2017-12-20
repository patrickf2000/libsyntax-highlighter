#include <iostream>
#include <QStringList>
#include <repository.hh>

int main(int argc, char **argv) {
    Repository *repo = new Repository;
    QStringList *names = repo->allNames();
    std::cout << "Syntax Names:" << std::endl;
    for (int i = 0; i<names->size(); i++) {
        std::cout << names->at(i).toStdString() << std::endl;
    }
    std::cout << std::endl;
}
