#ifndef TRAVELINSURANCEDIALOG_H
#define TRAVELINSURANCEDIALOG_H

#include <QDialog>
#include "../models/insurance_application.h" // TravelInsuranceApplication 구조체 선언부

class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;

class TravelInsuranceDialog : public QDialog {
    Q_OBJECT

public:
    explicit TravelInsuranceDialog(QWidget *parent = nullptr);
    TravelInsuranceApplication getResult() const { return result; }

private:
    TravelInsuranceApplication result;

    // 기본 여행 정보
    QLineEdit *destinationEdit;
    QLineEdit *startDateEdit;
    QLineEdit *endDateEdit;

    // 내 물건, 항공편 문제
    QComboBox *baggageLossCombo;            // 50 / 100 / 200
    QComboBox *flightDelayCombo;            // 20 / 50 / 80

    // 여행 중 아프거나 다쳤을 때
    QComboBox *overseasInjuryCombo;         // 1000 / 3000 / 5000
    QComboBox *overseasDiseaseCombo;        // 1000 / 3000 / 5000

    // 큰 사고
    QCheckBox *includeInjuryDeathCheck;     // 상해사망, 후유장해
    QCheckBox *includeDiseaseDeathCheck;    // 질병사망, 고도후유장해

    // 귀국해서 치료
    QComboBox *domesticInjuryCombo;         // 1000 / 2000 / 3000
    QComboBox *domesticDiseaseCombo;        // 1000 / 2000 / 3000

    // 확인 버튼
    QPushButton *submitButton;
};

#endif // TRAVELINSURANCEDIALOG_H
