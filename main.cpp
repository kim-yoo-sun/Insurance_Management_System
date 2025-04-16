#include "./widgets/loginwindow.h"
#include "./database/userdatabase.h"

#include <QApplication>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!UserDatabase::initialize()) {
        return -1;
    }

    LoginWindow login;
    login.show();
    qDebug() << "Working directory:" << QDir::currentPath();
    return a.exec();
}
