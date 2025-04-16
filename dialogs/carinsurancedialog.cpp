#include "carinsurancedialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>

CarInsuranceDialog::CarInsuranceDialog(const QString& name, QWidget *parent) : QDialog(parent) {
    setWindowTitle("자동차 보험 신청");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(new QLabel(QString("이름 : %1").arg(name)));

    mainLayout->addWidget(new QLabel("주민등록번호:"));
    ssnEdit = new QLineEdit(this);
    mainLayout->addWidget(ssnEdit);

    mainLayout->addWidget(new QLabel("휴대폰 번호:"));
    phoneEdit = new QLineEdit(this);
    mainLayout->addWidget(phoneEdit);

    // 2) 차량 정보
    mainLayout->addWidget(new QLabel("자동차 제조사:"));
    manufacturerEdit = new QLineEdit(this);
    mainLayout->addWidget(manufacturerEdit);

    mainLayout->addWidget(new QLabel("자동차 명:"));
    modelEdit = new QLineEdit(this);
    mainLayout->addWidget(modelEdit);

    mainLayout->addWidget(new QLabel("등록 연도:"));
    registerYearEdit = new QLineEdit(this);
    mainLayout->addWidget(registerYearEdit);

    mainLayout->addWidget(new QLabel("배기량(CC):"));
    ccEdit = new QLineEdit(this);
    mainLayout->addWidget(ccEdit);

    // 3) 기존 필드 (차종, 차량번호)
    mainLayout->addWidget(new QLabel("차종:"));
    vehicleTypeEdit = new QLineEdit(this);
    mainLayout->addWidget(vehicleTypeEdit);

    mainLayout->addWidget(new QLabel("차량 번호:"));
    licenseNumberEdit = new QLineEdit(this);
    mainLayout->addWidget(licenseNumberEdit);

    // 4) 할인 체크박스 (GroupBox)
    QGroupBox *discountGroup = new QGroupBox("추가 할인", this);
    QVBoxLayout *discountLayout = new QVBoxLayout(discountGroup);

    discountSafeBox = new QCheckBox("안전운전 할인(18%)", discountGroup);
    discountChildBox = new QCheckBox("자녀 할인(19%)", discountGroup);
    discountConnectBox = new QCheckBox("커넥트카 할인(3.5%)", discountGroup);
    discountLaneBox = new QCheckBox("차선이탈방지 할인(4.5%)", discountGroup);
    discountFrontBox = new QCheckBox("전방충돌방지 할인(5.5%)", discountGroup);

    discountLayout->addWidget(discountSafeBox);
    discountLayout->addWidget(discountChildBox);
    discountLayout->addWidget(discountConnectBox);
    discountLayout->addWidget(discountLaneBox);
    discountLayout->addWidget(discountFrontBox);

    discountGroup->setLayout(discountLayout);
    mainLayout->addWidget(discountGroup);

    // 5) 운전자 범위 (라디오버튼 2개)
    QGroupBox *driverGroupBox = new QGroupBox("운전자 범위", this);
    QVBoxLayout *driverLayout = new QVBoxLayout(driverGroupBox);

    onlyMyselfRadio = new QRadioButton("본인만 운전", driverGroupBox);
    everyoneRadio   = new QRadioButton("모든 사람 운전", driverGroupBox);

    // 기본 선택값 (예: 본인만 운전)
    onlyMyselfRadio->setChecked(true);

    driverLayout->addWidget(onlyMyselfRadio);
    driverLayout->addWidget(everyoneRadio);

    driverGroupBox->setLayout(driverLayout);
    mainLayout->addWidget(driverGroupBox);

    // 버튼 그룹 생성 (이 그룹 안에서는 단 하나만 선택 가능)
    driverGroup = new QButtonGroup(this);
    driverGroup->addButton(onlyMyselfRadio, 0);
    driverGroup->addButton(everyoneRadio, 1);

    // 확인 버튼
    submitButton = new QPushButton("신청", this);
    mainLayout->addWidget(submitButton);

    // 버튼 클릭 시 데이터 유효성 확인 & result 구조체에 저장
    connect(submitButton, &QPushButton::clicked, this, [=]() {
        QString type  = vehicleTypeEdit->text();
        QString plate = licenseNumberEdit->text();

        if (ssnEdit->text().isEmpty() || phoneEdit->text().isEmpty() || manufacturerEdit->text().isEmpty() || modelEdit->text().isEmpty() ||
            registerYearEdit->text().isEmpty() || ccEdit->text().isEmpty() || vehicleTypeEdit->text().isEmpty() || licenseNumberEdit->text().isEmpty())
        {
            QMessageBox::warning(this, "오류", "필수 정보를 모두 입력해주세요.");
            return;
        }

        // result에 저장
        result.vehicleType   = type;
        result.licenseNumber = plate;
        result.ssn           = ssnEdit->text();
        result.phone         = phoneEdit->text();
        result.manufacturer  = manufacturerEdit->text();
        result.model         = modelEdit->text();
        result.registerYear  = registerYearEdit->text();
        result.cc            = ccEdit->text();

        // 할인 체크박스
        result.discountSafe    = discountSafeBox->isChecked();
        result.discountChild   = discountChildBox->isChecked();
        result.discountConnect = discountConnectBox->isChecked();
        result.discountLane    = discountLaneBox->isChecked();
        result.discountFront   = discountFrontBox->isChecked();

        // 라디오 버튼에 따라 값 설정 (예: onlyMyself == true/false)
        if (onlyMyselfRadio->isChecked()) {
            // 본인만 운전
            result.onlyMyself = true;
        } else {
            // 모든 사람 운전
            result.onlyMyself = false;
        }

        // 성공 메시지
        QMessageBox::information(this, "신청 완료",
                                "자동차 보험 신청이 완료되었습니다.\n"
                                "가입자 명 : " + name + "\n"
                                "차량 종류 : "  + result.vehicleType + "\n"
                                "차량 연도 : "  + result.registerYear);

        accept();
    });
}
