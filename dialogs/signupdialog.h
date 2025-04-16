#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
#include "ui_signupdialog.h"

class QLineEdit;
class QPushButton;

class SignUpDialog : public QDialog {
    Q_OBJECT

public:
    explicit SignUpDialog(QWidget *parent = nullptr);
    void onSignUpClicked();

private:
    QLineEdit *newUsernameEdit;
    QLineEdit *newPasswordEdit;
    QPushButton *registerButton;
    QLineEdit *newNameEdit;
    Ui::SignUpDialog *ui;
};

#endif // SIGNUPDIALOG_H
