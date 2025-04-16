#ifndef CARINSURANCEDIALOG_H
#define CARINSURANCEDIALOG_H

#include <QDialog>
#include "../models/insurance_application.h"

class QLineEdit;
class QPushButton;
class QCheckBox;
class QRadioButton;
class QButtonGroup;

class CarInsuranceDialog : public QDialog {
    Q_OBJECT

public:
    explicit CarInsuranceDialog(const QString& name, QWidget *parent = nullptr);
    CarInsuranceApplication getResult() const { return result; }

private:
    CarInsuranceApplication result;

    // 기존
    QLineEdit *vehicleTypeEdit;
    QLineEdit *licenseNumberEdit;
    QPushButton *submitButton;

    // 새로 추가
    QLineEdit *ssnEdit;
    QLineEdit *phoneEdit;
    QLineEdit *manufacturerEdit;
    QLineEdit *modelEdit;
    QLineEdit *registerYearEdit;
    QLineEdit *ccEdit;

    QCheckBox *discountSafeBox;
    QCheckBox *discountChildBox;
    QCheckBox *discountConnectBox;
    QCheckBox *discountLaneBox;
    QCheckBox *discountFrontBox;

    QRadioButton *onlyMyselfRadio;
    QRadioButton *everyoneRadio;
    QButtonGroup *driverGroup;
};

#endif // CARINSURANCEDIALOG_H
