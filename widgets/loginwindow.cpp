#include "loginwindow.h"
#include "mainwindow.h"
#include "../dialogs/signupdialog.h"
#include "../database/userdatabase.h"
#include "../models/person.h"

#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWindow) {

    ui->setupUi(this);  // ui_loginwindow.ui에서 정의된 UI를 설정

    // 로그인 버튼 클릭 시 호출되는 함수 연결
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    connect(ui->signupButton, &QPushButton::clicked, this, [=]() {
        SignUpDialog dialog(this);
        dialog.exec();  // 모달 다이얼로그 띄우기
    });
}

void LoginWindow::onLoginClicked() {
    QString username = ui->idLineEdit->text();
    QString password = ui->pwLineEdit->text();

    const Person *user = UserDatabase::authenticate(username, password);

    if (!user) {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        return;
    }

    if (username == "admin") {
        AdminMainWindow *adminWin = new AdminMainWindow(user->getName(), user->getId());
        adminWin->show();
    } else {
        UserMainWindow *userWin = new UserMainWindow(user->getName(), user->getId());
        userWin->show();
    }

    this->close();
}
