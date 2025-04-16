#include "travelinsurancedialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>

TravelInsuranceDialog::TravelInsuranceDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("여행 보험 신청");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    //
    // (1) 기본 여행 정보
    //
    mainLayout->addWidget(new QLabel("목적지:"));
    destinationEdit = new QLineEdit(this);
    mainLayout->addWidget(destinationEdit);

    mainLayout->addWidget(new QLabel("출발일 (YYYY-MM-DD):"));
    startDateEdit = new QLineEdit(this);
    mainLayout->addWidget(startDateEdit);

    mainLayout->addWidget(new QLabel("도착일 (YYYY-MM-DD):"));
    endDateEdit = new QLineEdit(this);
    mainLayout->addWidget(endDateEdit);

    //
    // (2) 내 물건, 항공편 문제
    //
    QGroupBox *baggageGroup = new QGroupBox("내 물건/항공편 문제");
    QVBoxLayout *baggageLayout = new QVBoxLayout(baggageGroup);

    // 여행 중 휴대품 손해 (분실 제외): 50, 100, 200
    {
        QHBoxLayout *h = new QHBoxLayout;
        QLabel *lbl = new QLabel("휴대품 손해 (예: 50/100/200)");
        baggageLossCombo = new QComboBox(this);
        baggageLossCombo->addItem("50",  50);
        baggageLossCombo->addItem("100", 100);
        baggageLossCombo->addItem("200", 200);

        h->addWidget(lbl);
        h->addWidget(baggageLossCombo);
        baggageLayout->addLayout(h);
    }

    // 항공기 및 수하물 지연비용: 20, 50, 80
    {
        QHBoxLayout *h = new QHBoxLayout;
        QLabel *lbl = new QLabel("항공기/수하물 지연 (예: 20/50/80)");
        flightDelayCombo = new QComboBox(this);
        flightDelayCombo->addItem("20", 20);
        flightDelayCombo->addItem("50", 50);
        flightDelayCombo->addItem("80", 80);

        h->addWidget(lbl);
        h->addWidget(flightDelayCombo);
        baggageLayout->addLayout(h);
    }

    baggageGroup->setLayout(baggageLayout);
    mainLayout->addWidget(baggageGroup);

    //
    // (3) 여행 중 아프거나 다쳤을 때
    //
    QGroupBox *medicalGroup = new QGroupBox("여행 중 상해/질병");
    QVBoxLayout *medicalLayout = new QVBoxLayout(medicalGroup);

    // 해외상해 해외실손의료비: 1000 / 3000 / 5000
    {
        QHBoxLayout *h = new QHBoxLayout;
        QLabel *lbl = new QLabel("해외상해 실손의료비");
        overseasInjuryCombo = new QComboBox(this);
        overseasInjuryCombo->addItem("1000", 1000);
        overseasInjuryCombo->addItem("3000", 3000);
        overseasInjuryCombo->addItem("5000", 5000);
        h->addWidget(lbl);
        h->addWidget(overseasInjuryCombo);
        medicalLayout->addLayout(h);
    }

    // 해외질병 해외실손의료비: 1000 / 3000 / 5000
    {
        QHBoxLayout *h = new QHBoxLayout;
        QLabel *lbl = new QLabel("해외질병 실손의료비");
        overseasDiseaseCombo = new QComboBox(this);
        overseasDiseaseCombo->addItem("1000", 1000);
        overseasDiseaseCombo->addItem("3000", 3000);
        overseasDiseaseCombo->addItem("5000", 5000);
        h->addWidget(lbl);
        h->addWidget(overseasDiseaseCombo);
        medicalLayout->addLayout(h);
    }

    medicalGroup->setLayout(medicalLayout);
    mainLayout->addWidget(medicalGroup);

    //
    // (4) 여행 중 큰 사고(상해사망/질병사망)
    //
    QGroupBox *accidentGroup = new QGroupBox("큰 사고");
    QVBoxLayout *accidentLayout = new QVBoxLayout(accidentGroup);

    includeInjuryDeathCheck = new QCheckBox("상해사망/후유장해 포함", accidentGroup);
    accidentLayout->addWidget(includeInjuryDeathCheck);

    includeDiseaseDeathCheck = new QCheckBox("질병사망/고도후유장해 포함", accidentGroup);
    accidentLayout->addWidget(includeDiseaseDeathCheck);

    accidentGroup->setLayout(accidentLayout);
    mainLayout->addWidget(accidentGroup);

    //
    // (5) 귀국해서 치료
    //
    QGroupBox *domesticGroup = new QGroupBox("귀국 후 치료");
    QVBoxLayout *domesticLayout = new QVBoxLayout(domesticGroup);

    // 해외상해 국내급여 의료비: 1000 / 2000 / 3000
    {
        QHBoxLayout *h = new QHBoxLayout;
        QLabel *lbl = new QLabel("해외상해 국내급여 의료비");
        domesticInjuryCombo = new QComboBox(this);
        domesticInjuryCombo->addItem("1000", 1000);
        domesticInjuryCombo->addItem("2000", 2000);
        domesticInjuryCombo->addItem("3000", 3000);
        h->addWidget(lbl);
        h->addWidget(domesticInjuryCombo);
        domesticLayout->addLayout(h);
    }

    // 해외질병 국내급여 의료비: 1000 / 2000 / 3000
    {
        QHBoxLayout *h = new QHBoxLayout;
        QLabel *lbl = new QLabel("해외질병 국내급여 의료비");
        domesticDiseaseCombo = new QComboBox(this);
        domesticDiseaseCombo->addItem("1000", 1000);
        domesticDiseaseCombo->addItem("2000", 2000);
        domesticDiseaseCombo->addItem("3000", 3000);
        h->addWidget(lbl);
        h->addWidget(domesticDiseaseCombo);
        domesticLayout->addLayout(h);
    }

    domesticGroup->setLayout(domesticLayout);
    mainLayout->addWidget(domesticGroup);

    //
    // (6) 신청 버튼
    //
    submitButton = new QPushButton("신청", this);
    mainLayout->addWidget(submitButton);

    connect(submitButton, &QPushButton::clicked, this, [=]() {
        // 간단히 필수 값만 검사
        if (destinationEdit->text().isEmpty() || startDateEdit->text().isEmpty() || endDateEdit->text().isEmpty()) {
            QMessageBox::warning(this, "오류", "목적지를 입력해주세요.");
            return;
        }

        // 구조체 채우기
        result.destination = destinationEdit->text();
        result.startDate   = startDateEdit->text();
        result.endDate     = endDateEdit->text();

        result.baggageLossCoverage = baggageLossCombo->currentData().toInt();
        result.flightDelayCoverage = flightDelayCombo->currentData().toInt();

        result.overseasInjuryMedical  = overseasInjuryCombo->currentData().toInt();
        result.overseasDiseaseMedical = overseasDiseaseCombo->currentData().toInt();

        result.includeInjuryDeath  = includeInjuryDeathCheck->isChecked();
        result.includeDiseaseDeath = includeDiseaseDeathCheck->isChecked();

        result.domesticInjuryMedical  = domesticInjuryCombo->currentData().toInt();
        result.domesticDiseaseMedical = domesticDiseaseCombo->currentData().toInt();

        QMessageBox::information(this, "신청 완료",
                                 "여행보험 신청이 완료되었습니다.\n"
                                 "목적지: " + result.destination + "\n"
                                                            "출발일: " + result.startDate + " / 도착일: " + result.endDate);

        accept();
    });
}
