#include "mainwindow.h"
#include "loginwindow.h"
#include "../dialogs/carinsurancedialog.h"
#include "../dialogs/travelinsurancedialog.h"
#include "../database/userdatabase.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QInputDialog>
#include <QTextEdit>

// // 관리자용 메인 창
AdminMainWindow::AdminMainWindow(const QString& name, const QString& id, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("HanWha VEDA 손해 보험 : 관리자");
    resize(1200, 800);

    setStyleSheet(R"(
        QLabel { font-size: 15px; }
        QPushButton { min-height: 32px; padding: 6px 12px; font-size: 14px; }
        QLineEdit { padding: 6px; font-size: 14px; }
        QListWidget { border: 1px solid #ccc; font-size: 13px; }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QLabel *labelTitle = new QLabel("<b>관리자 패널</b>", this);
    labelTitle->setAlignment(Qt::AlignCenter);
    labelTitle->setStyleSheet("font-size: 20px; margin-bottom: 10px;");
    layout->addWidget(labelTitle);

    QHBoxLayout *infoLayout = new QHBoxLayout;
    infoLayout->setAlignment(Qt::AlignLeft);  // 전체를 왼쪽 정렬

    QLabel *nameLabel = new QLabel("👤 이름: " + name, this);
    QLabel *idLabel = new QLabel("🆔 ID: " + id, this);

    nameLabel->setStyleSheet("margin-right: 15px;");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(idLabel);
    layout->addLayout(infoLayout);

    QFrame *line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line1);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    QLineEdit *searchEdit = new QLineEdit(this);
    QPushButton *searchBtn = new QPushButton("검색", this);
    QPushButton *resetBtn  = new QPushButton("전체 보기", this);
    searchEdit->setPlaceholderText("ID를 입력하세요");
    searchLayout->setSpacing(10);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchBtn);
    searchLayout->addWidget(resetBtn);
    layout->addLayout(searchLayout);

    applyList = new QListWidget(this);
    applyList->setMinimumHeight(400);
    layout->addWidget(applyList);

    allApplications = UserDatabase::loadAllApplication();

    auto refreshApplyList = [&](const QList<QList<QString>> &data) {
        applyList->clear();
        for (const QList<QString>& row : data) {
            QString appId   = row[0];
            QString userId  = row[1];
            QString summary = row[2];
            QString status  = row[3];

            QWidget *rowWidget = new QWidget;
            QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(5, 2, 5, 2);

            QCheckBox *checkBox = new QCheckBox;
            QLabel *summaryLabel = new QLabel(userId + " | " + summary);
            summaryLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            summaryLabel->setWordWrap(true);
            summaryLabel->setToolTip(summaryLabel->text());

            QComboBox *statusBox = new QComboBox;
            statusBox->addItems({"처리 중", "심사 성공", "심사 거절"});
            statusBox->setCurrentText(status);
            statusBox->setMinimumWidth(80);
            statusBox->setMinimumHeight(28);
            statusBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

            summaryLabel->setProperty("appId", appId);
            rowLayout->addWidget(checkBox, 0);
            rowLayout->addWidget(summaryLabel, 8);
            rowLayout->addWidget(statusBox, 1);

            QListWidgetItem *item = new QListWidgetItem(applyList);
            item->setSizeHint(QSize(0, 100));
            applyList->addItem(item);
            applyList->setItemWidget(item, rowWidget);
        }
    };

    refreshApplyList(allApplications);

    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        QString keyword = searchEdit->text().trimmed();
        if (keyword.isEmpty()) {
            QMessageBox::information(this, "검색", "검색할 ID를 입력하세요.");
            return;
        }
        QList<QList<QString>> filtered = UserDatabase::searchApplicationsByUserId(keyword);
        if (filtered.isEmpty()) {
            QMessageBox::information(this, "결과 없음", "해당 ID로 검색된 결과가 없습니다.");
        }
        refreshApplyList(filtered);
    });

    connect(resetBtn, &QPushButton::clicked, this, [=]() {
        searchEdit->clear();
        refreshApplyList(allApplications);
    });

    QHBoxLayout *actionLayout = new QHBoxLayout;
    QPushButton *updateBtn = new QPushButton("심사 인용", this);
    QPushButton *deleteBtn = new QPushButton("선택 삭제", this);
    QPushButton *logoutBtn = new QPushButton("Log Out", this);

    actionLayout->setSpacing(10);
    actionLayout->addWidget(updateBtn);
    actionLayout->addWidget(deleteBtn);
    actionLayout->addWidget(logoutBtn);
    layout->addLayout(actionLayout);

    // (5) 심사 반영 버튼
    layout->addWidget(updateBtn);

    connect(updateBtn, &QPushButton::clicked, this, [=]() {
        int count = 0;

        for (int i = 0; i < applyList->count(); ++i) {
            QListWidgetItem *item = applyList->item(i);
            QWidget *row = applyList->itemWidget(item);
            if (!row) continue;

            QHBoxLayout *hlayout = qobject_cast<QHBoxLayout *>(row->layout());
            if (!hlayout) continue;

            QCheckBox *checkBox = qobject_cast<QCheckBox *>(hlayout->itemAt(0)->widget());
            QLabel *label       = qobject_cast<QLabel *>(hlayout->itemAt(1)->widget());
            QComboBox *combo    = qobject_cast<QComboBox *>(hlayout->itemAt(2)->widget());

            if (!checkBox || !label || !combo) continue;
            if (!checkBox->isChecked()) continue;

            bool ok = false;
            int appId = label->property("appId").toInt(&ok);
            if (!ok) continue;

            QString newStatus = combo->currentText();

            if (UserDatabase::updateStatusById(appId, newStatus)) {
                count++;
            }
        }

        QMessageBox::information(this, "처리 완료", QString::number(count) + "건 반영되었습니다.");

        // 체크박스 초기화
        for (int i = 0; i < applyList->count(); ++i) {
            QWidget *row = applyList->itemWidget(applyList->item(i));
            if (!row) continue;

            QHBoxLayout *hlayout = qobject_cast<QHBoxLayout *>(row->layout());
            if (!hlayout) continue;

            QCheckBox *checkBox = qobject_cast<QCheckBox *>(hlayout->itemAt(0)->widget());
            if (checkBox) checkBox->setChecked(false);
        }
    });

    // (6) 선택 삭제 버튼
    layout->addWidget(deleteBtn);

    connect(deleteBtn, &QPushButton::clicked, this, [=]() {
        int deleted = 0;

        // 뒤에서부터 확인
        for (int i = applyList->count() - 1; i >= 0; --i) {
            QListWidgetItem *item = applyList->item(i);
            QWidget *row = applyList->itemWidget(item);
            if (!row) continue;

            QHBoxLayout *hlayout = qobject_cast<QHBoxLayout *>(row->layout());
            if (!hlayout) continue;

            QCheckBox *checkBox = qobject_cast<QCheckBox *>(hlayout->itemAt(0)->widget());
            QLabel *label       = qobject_cast<QLabel *>(hlayout->itemAt(1)->widget());

            if (!checkBox || !label) continue;
            if (!checkBox->isChecked()) continue;

            bool ok = false;
            int appId = label->property("appId").toInt(&ok);
            if (!ok) continue;

            // DB에서 삭제 성공하면 applyList에서도 제거
            if (UserDatabase::deleteApplicationById(appId)) {
                delete applyList->takeItem(i); // UI에서 삭제
                deleted++;
            }
        }

        QMessageBox::information(this, "삭제 완료", QString::number(deleted) + "건 삭제되었습니다.");

        // 체크박스 초기화
        for (int i = 0; i < applyList->count(); ++i) {
            QWidget *row = applyList->itemWidget(applyList->item(i));
            if (!row) continue;

            QHBoxLayout *hlayout = qobject_cast<QHBoxLayout *>(row->layout());
            if (!hlayout) continue;

            QCheckBox *checkBox = qobject_cast<QCheckBox *>(hlayout->itemAt(0)->widget());
            if (checkBox) checkBox->setChecked(false);
        }
    });

    // (7) 로그아웃 버튼
    layout->addWidget(logoutBtn);

    connect(logoutBtn, &QPushButton::clicked, this, [=]() {
        this->close();
        LoginWindow *login = new LoginWindow(nullptr);
        login->show();
    });
}

UserMainWindow::UserMainWindow(const QString& name, const QString& id, QWidget *parent)
    : QWidget(parent), m_userId(id)
{
    setWindowTitle("HanWha VEDA 손해 보험");
    resize(1200, 800);

    // 전역 스타일
    setStyleSheet(R"(
        QLabel {
            font-size: 15px;
        }
        QPushButton {
            min-height: 32px;
            padding: 6px 12px;
            font-size: 14px;
        }
        QListWidget {
            border: 1px solid #ccc;
            font-size: 13px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // 제목
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":/new/prefix1/images/insurance_logo_main.jpg")
                             .scaled(400, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignRight);

    // 제목 라벨
    QLabel *title = new QLabel("<b>사용자 보험 관리</b>", this);
    title->setStyleSheet("font-size: 35px;");
    title->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    // 수평 레이아웃으로 로고 + 제목 배치
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);
    titleLayout->addSpacing(10);
    titleLayout->addWidget(logoLabel, 0, Qt::AlignRight);

    layout->addLayout(titleLayout);


    // 이름 및 ID 표시
    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->setAlignment(Qt::AlignLeft); // 전체 우측 정렬

    QLabel *nameLabel = new QLabel("👤 이름: " + name, this);
    nameLabel->setAlignment(Qt::AlignLeft); // 개별 라벨도 정렬

    QLabel *idLabel = new QLabel("🆔 ID: " + id, this);
    idLabel->setAlignment(Qt::AlignLeft);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(idLabel);

    layout->addLayout(infoLayout);

    // 보험 신청 버튼 + 이미지
    int imageWidth = 300;
    int imageHeight = 150;

    QPushButton *carBtn    = new QPushButton("자동차 보험 신청", this);
    QPushButton *travelBtn = new QPushButton("여행 보험 신청", this);
    carBtn->setFixedWidth(imageWidth);
    travelBtn->setFixedWidth(imageWidth);

    // 자동차 이미지 + 버튼 + 테두리 박스
    QLabel *carImgLabel = new QLabel(this);
    carImgLabel->setPixmap(QPixmap(":/new/prefix1/images/img_prod_car.png")
                               .scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    carImgLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *carBoxLayout = new QVBoxLayout;
    carBoxLayout->setSpacing(8);
    carBoxLayout->addWidget(carImgLabel, 0, Qt::AlignCenter);
    carBoxLayout->addWidget(carBtn, 0, Qt::AlignCenter);

    QFrame *carBox = new QFrame(this);
    carBox->setLayout(carBoxLayout);
    carBox->setFrameShape(QFrame::StyledPanel);
    carBox->setStyleSheet("QFrame { border: 1px solid #bbb; border-radius: 6px; padding: 10px; }");

    // 여행 이미지 + 버튼 + 테두리 박스
    QLabel *travelImgLabel = new QLabel(this);
    travelImgLabel->setPixmap(QPixmap(":/new/prefix1/images/img_prod_travel.png")
                                  .scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    travelImgLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *travelBoxLayout = new QVBoxLayout;
    travelBoxLayout->setSpacing(8);
    travelBoxLayout->addWidget(travelImgLabel, 0, Qt::AlignCenter);
    travelBoxLayout->addWidget(travelBtn, 0, Qt::AlignCenter);

    QFrame *travelBox = new QFrame(this);
    travelBox->setLayout(travelBoxLayout);
    travelBox->setFrameShape(QFrame::StyledPanel);
    travelBox->setStyleSheet("QFrame { border: 1px solid #bbb; border-radius: 6px; padding: 10px; }");

    // 수평 레이아웃으로 정렬
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(20);
    buttonLayout->addWidget(carBox);
    buttonLayout->addWidget(travelBox);
    layout->addLayout(buttonLayout);

    // QListWidget 생성
    applyList = new QListWidget(this);
    applyList->setMinimumHeight(350);
    layout->addWidget(applyList);

    // 수정 버튼
    QPushButton *editButton = new QPushButton("수정하기", this);
    connect(editButton, &QPushButton::clicked, this, &UserMainWindow::onEditButtonClicked);

    // 새로고침 버튼
    QPushButton *refreshButton = new QPushButton("새로고침", this);
    connect(refreshButton, &QPushButton::clicked, this, [=]() {
        loadUserApplications(id);
    });

    // 로그아웃 버튼
    QPushButton *logoutBtn = new QPushButton("Log Out", this);
    connect(logoutBtn, &QPushButton::clicked, this, [=]() {
        this->close();
        LoginWindow *login = new LoginWindow(nullptr); // 생성자 인자 주의
        login->show();
    });

    // 하단 버튼 레이아웃
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setSpacing(12);
    bottomLayout->addWidget(editButton);
    bottomLayout->addWidget(refreshButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(logoutBtn);
    layout->addLayout(bottomLayout);

    // 버튼 기능 연결
    connect(carBtn, &QPushButton::clicked, this, [=]() {
        CarInsuranceDialog dialog(name, this);
        if (dialog.exec() == QDialog::Accepted) {
            CarInsuranceApplication app = dialog.getResult();

            QStringList discounts;
            if (app.discountSafe)    discounts << "안전운전(18%)";
            if (app.discountChild)   discounts << "자녀(19%)";
            if (app.discountConnect) discounts << "커넥트카(3.5%)";
            if (app.discountLane)    discounts << "차선이탈(4.5%)";
            if (app.discountFront)   discounts << "전방충돌(5.5%)";

            QString summary = QString("\n\n🚗 자동차보험 | 이름: %1 | 주민등록번호: %2 | 휴대폰: %3 \n")
                                  .arg(name)
                                  .arg(app.ssn)
                                  .arg(app.phone);
            summary += QString("제조사: %1, 모델: %2, 연도: %3, cc: %4 \n")
                           .arg(app.manufacturer)
                           .arg(app.model)
                           .arg(app.registerYear)
                           .arg(app.cc);
            summary += QString("차종: %1 | 차량번호: %2 | ")
                           .arg(app.vehicleType)
                           .arg(app.licenseNumber);
            summary += QString("할인: %1 | ").arg(discounts.join(", "));
            summary += (app.onlyMyself ? "[본인만 운전]" : "[모든 사람 운전]\n");
            summary += "\n\n";

            UserDatabase::submitCarInsurance(id, summary);
            loadUserApplications(id);
        }
    });

    connect(travelBtn, &QPushButton::clicked, this, [=]() {
        TravelInsuranceDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            TravelInsuranceApplication app = dialog.getResult();

            QString summary = QString("\n\n✈️ 여행보험 | 목적지: %1 | 기간: %2 ~ %3 \n")
                                  .arg(app.destination)
                                  .arg(app.startDate)
                                  .arg(app.endDate);
            summary += QString("휴대품 손해: %1 / 수하물 지연: %2 | ")
                           .arg(app.baggageLossCoverage)
                           .arg(app.flightDelayCoverage);
            summary += QString("해외상해: %1 / 해외질병: %2 | ")
                           .arg(app.overseasInjuryMedical)
                           .arg(app.overseasDiseaseMedical);
            summary += QString("큰사고(상해:%1 / 질병:%2) | ")
                           .arg(app.includeInjuryDeath ? "O" : "X")
                           .arg(app.includeDiseaseDeath ? "O" : "X");
            summary += QString("귀국(상해:%1 / 질병:%2)")
                           .arg(app.domesticInjuryMedical)
                           .arg(app.domesticDiseaseMedical);
            summary += "\n\n";

            UserDatabase::submitCarInsurance(id, summary);
            loadUserApplications(id);
        }
    });

    loadUserApplications(id);
}

void UserMainWindow::loadUserApplications(const QString &userId)
{
    applyList->clear(); // 기존 목록 초기화

    QList<QList<QString>> data = UserDatabase::loadApplicationsForUserDetail(userId);
    for (const QList<QString>& row : data) {
        QString appId   = row[0];
        QString appType = row[1];
        QString summary = row[2];
        QString status  = row[3];

        // 표시될 텍스트
        QString displayText = summary + " | 상태: " + status;

        // QListWidgetItem 생성
        QListWidgetItem *item = new QListWidgetItem(displayText, applyList);

        item->setData(Qt::UserRole + 1, appId);
        item->setData(Qt::UserRole + 2, appType);
        item->setData(Qt::UserRole + 3, summary); // 수정 전 summary (원본)

        applyList->addItem(item);
    }
}

void UserMainWindow::onEditButtonClicked()
{
    // (1) 현재 선택된 QListWidgetItem 가져오기
    QListWidgetItem *curItem = applyList->currentItem();
    if (!curItem) {
        QMessageBox::warning(this, "알림", "수정할 항목을 선택하세요!");
        return;
    }

    // (2) 아이템에 저장된 데이터 꺼내오기
    int appId        = curItem->data(Qt::UserRole + 1).toInt();
    QString appType  = curItem->data(Qt::UserRole + 2).toString();
    QString oldSummary = curItem->data(Qt::UserRole + 3).toString();

    // (3) 수정 기능
    QDialog dialog(this);
    dialog.setWindowTitle("수정하기");
    dialog.setMinimumSize(600, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* label = new QLabel("신청내용(요약)", &dialog);
    layout->addWidget(label);

    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setText(oldSummary);
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(textEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString newSummary = textEdit->toPlainText();

    // (4) DB 업데이트
    if (!UserDatabase::updateApplicationById(appId, newSummary)) {
        QMessageBox::critical(this, "에러", "DB 업데이트 실패");
        return;
    }

    QMessageBox::information(this, "완료", "수정되었습니다.");

    // (5) 변경사항을 반영하기 위해 목록 다시 로드
    loadUserApplications(m_userId);
}
