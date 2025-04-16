#include "signupdialog.h"
#include "../database/userdatabase.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);  // signupdialog.ui에서 UI를 설정
    connect(ui->signupConfirmButton, &QPushButton::clicked, this, &SignUpDialog::onSignUpClicked);
}

void SignUpDialog::onSignUpClicked() {
    QString id = ui->idLineEdit->text();            // 사용자 ID
    QString pw = ui->pwLineEdit->text();            // 비밀번호
    QString chk_pw = ui->pwConfirmLineEdit->text(); // 비밀번호 확인
    QString name = ui->nameLineEdit->text();        // 이름

    // 유효성 검사: 입력된 값이 비어 있으면 경고 메시지 표시
    if (id.isEmpty() || pw.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields.");
        return;
    }

    // 유효성 검사: 비밀번호 값과 비밀번호 확인 값이 일치하지 않을 경우 경고 메시지 표기
    if (pw != chk_pw) {
        QMessageBox::warning(this, "Error", "Please check Password & Validation");
        return;
    }

    // 이미 있는 아이디인지 확인 (DB)
    if (UserDatabase::exists(id)) {
        QMessageBox::warning(this, "Error", "User ID already exists.");
        return;
    }

    // DB에 사용자 등록 시도
    if (UserDatabase::addUser(id, pw, name)) {
        QMessageBox::information(this, "Success",
                                 "User registered:\nID: " + id + "\nName: " + name);
        accept();  // 회원가입 창 닫기
    } else {
        QMessageBox::critical(this, "Error", "Failed to register user. Please try again.");
    }
}
