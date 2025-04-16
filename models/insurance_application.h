#ifndef INSURANCE_APPLICATION_H
#define INSURANCE_APPLICATION_H

#include <QString>

struct CarInsuranceApplication {
    QString vehicleType;    // 기존 필드
    QString licenseNumber;  // 기존 필드

    // 추가 필드
    QString ssn;            // 주민등록번호
    QString phone;          // 휴대폰 번호
    QString manufacturer;   // 자동차 제조사
    QString model;          // 자동차 명
    QString registerYear;   // 등록 연도
    QString cc;             // 배기량(CC)

    bool discountSafe;      // 안전운전 할인
    bool discountChild;     // 자녀 할인
    bool discountConnect;   // 커넥트카 할인
    bool discountLane;      // 차선이탈방지 장치 할인
    bool discountFront;     // 전방충돌방지 장치 할인

    bool onlyMyself;        // 본인만 운전
};

struct TravelInsuranceApplication {
    // 1) 기본 정보
    QString destination;     // 목적지
    QString startDate;       // 출발일
    QString endDate;         // 종료일

    // 2) 내 물건, 항공편 문제
    //   (예: 50 / 100 / 200, 20 / 50 / 80)
    int baggageLossCoverage;    // 여행 중 휴대품 손해 (분실 제외)
    int flightDelayCoverage;    // 항공기 및 수하물 지연비용

    // 3) 여행 중 아프거나 다쳤을 때
    //   (예: 1000 / 3000 / 5000)
    int overseasInjuryMedical;  // 해외상해 해외실손의료비
    int overseasDiseaseMedical; // 해외질병 해외실손의료비

    // 4) 여행 중 큰 사고가 발생했을 때
    bool includeInjuryDeath;    // 여행 중 상해사망, 후유장해
    bool includeDiseaseDeath;   // 여행 중 질병사망, 고도후유장해

    // 5) 귀국해서 치료받을 때
    //   (예: 1000 / 2000 / 3000, etc.)
    int domesticInjuryMedical;  // 해외상해 국내급여 의료비
    int domesticDiseaseMedical; // 해외질병 국내급여의료비
};

#endif // INSURANCE_APPLICATION_H
