&nbsp;
&nbsp;
&nbsp;

# 💼 Insurance Management System

&nbsp;

이 프로젝트는 Qt 프레임워크와 C++을 사용하여 개발된 **보험 관리 시스템**입니다.

보험계약자(사용자)는 데스크탑 인터페이스를 통해 회원가입, 로그인, 자동차 보험 및 여행자 보험을 신청 & 수정할 수 있습니다.

또한, 보험사(관리자)는 사용자 및 보험 정보를 검색하고 심사 및 삭제 기능을 제공합니다.

이를 바탕으로 각 사용자의 보험의 가입 유무가 결정 되고, 사용자는 본인의 아이디에서 해당 내역을 확인할 수 있습니다.

본 프로젝트는 **Qt 기반 GUI**와 **SQLite 데이터베이스**의 통합을 바탕으로 구축되었습니다.

---
&nbsp;

## 📦 Key Features (주요 기능 요약)
&nbsp;

- 👤 사용자 회원가입 및 로그인 기능 (User registration & login)
- 🚗 자동차 보험 신청 기능 (Car insurance application)
- ✈️ 여행자 보험 신청 기능 (Travel insurance application)
- ✏️ 사용자 정보 수정 기능 (User data modification)
- 🗃️ SQLite 기반 보험 신청 정보 저장 및 조회 (SQL database-backed storage)
- 🔍 관리자용 사용자/보험 정보 검색 기능 (Admin search)
- ❌ 관리자용 신청서 삭제 기능 (Admin delete)
- 🧩 Qt 위젯, 다이얼로그, 모델을 활용한 모듈형 구조 (Modular Qt structure)

---
&nbsp;


## 🧱 Class Overview (클래스 기능 요약)
&nbsp;

### 👤 사용자 인증 및 계정 관리

- **Person**  
  사용자 ID, 비밀번호, 이름 정보를 저장하는 단순한 사용자 모델 클래스입니다.

- **SignUpDialog**  
  사용자 회원가입을 위한 입력 폼과 유효성 검사, DB 등록 기능을 담당합니다.

- **LoginWindow**  
  사용자 로그인 기능을 수행하며, 권한에 따라 사용자/관리자 창으로 분기합니다.

---

### 🙋 사용자 기능

- **UserMainWindow**  
  로그인한 일반 사용자가 자신의 보험 신청을 확인하고, 수정하거나 신규 신청할 수 있습니다.

- **CarInsuranceDialog**  
  자동차 보험 신청 정보를 입력받고 구조체로 반환하는 폼 다이얼로그입니다.

- **TravelInsuranceDialog**  
  여행자 보험 신청 정보를 선택 입력받아 구조체로 반환하는 폼 다이얼로그입니다.

---

### 👨‍💼 관리자 기능

- **AdminMainWindow**  
  관리자가 전체 보험 신청 내역을 조회, 검색, 상태 변경 및 삭제할 수 있는 메인 인터페이스입니다.

---

### 🗂 데이터 모델 및 DB 관리

- **InsuranceApplication**  
  자동차 및 여행자 보험 신청 데이터를 구조체 형태로 정의한 모델입니다.

- **UserDatabase**  
  사용자 인증 및 보험 신청서의 저장, 조회, 수정, 삭제를 수행하는 정적 SQLite 기반 데이터베이스 관리 클래스입니다.

---

### 🧩 공통/기본 위젯

- **Widget**  
  초기 UI용으로 생성된 기본 템플릿 위젯으로, 현재는 기능 없이 구조만 포함하고 있습니다.

---
&nbsp;
&nbsp;

## 🐞 Known Issues / Limitations (현재 한계 및 문제점)
&nbsp;

- 🌐 DB는 단일 연결 기반이지만 외부에서 동시에 접근할 경우 충돌 가능성이 있으며, 멀티스레드 환경이나 네트워크 기반 확장에 대비한 **동기화 처리가 부족**합니다.
- 🔄 사용자 입력값에 대한 유효성 검사가 일부 필드에서 누락되어 있으며, 주민등록번호나 전화번호 형식 검증이 필요합니다.
- 📤 보험 신청 내역을 파일로 내보내거나 가져오는 기능(CSV/PDF 등)이 아직 구현되어 있지 않습니다.
- 🔐 로그인 시 비밀번호는 평문으로 저장되며, 해시 처리 또는 암호화 기능이 구현되어 있지 않습니다.
- 🧷 **Qt 프레임워크에 강하게 의존하는 구조**로 인해, 타 프레임워크 이식이나 경량화된 환경에서는 재사용이 어렵습니다.

---
&nbsp;

## ✨ Planned Improvements (예정된 개선 사항)
&nbsp;

- [ ] 🌐 **동시성 및 DB 동기화 안전성 확보**
  다중 사용자 환경에서의 DB 충돌 방지를 위해 트랜잭션 처리 및 잠금 제어, 실시간 반영 전략 수립

- [ ] 🔎 **입력 필드별 유효성 검사 강화**
  주민등록번호, 전화번호, 날짜 형식 등에 대해 정규표현식을 활용한 정밀한 입력 검증 로직 추가

- [ ] 📤 **보험 신청 내역 내보내기 기능 추가**
  PDF 또는 CSV 형태로 보험 신청 내역을 내보내는 기능을 UI에 통합할 예정

- [ ] 🔐 **비밀번호 암호화 처리**
  사용자 인증 보안을 강화하기 위해 `QCryptographicHash` 또는 외부 암호화 라이브러리를 활용한 해시 기반 저장 방식 도입 예정

---
&nbsp;

## 🔧 Tools and Dependencies
&nbsp;

- **Qt Version**: Qt 6.8.3 MSVC 2022 64-bit
- **CMake Version**: CMake 3.30.5
- **Compiler**: Visual Studio Community 2022 (MSVC 17.13)
- **Build Generator**: Ninja
- **Kit**: Auto-detected in Qt Creator (`Desktop Qt 6.8.3 MSVC2022 64bit`)
- **Build Type**: Debug (Release also supported)

---
&nbsp;

## 📚 Required Qt Modules
&nbsp;

| Module        | Description                                  |
|---------------|----------------------------------------------|
| `Qt::Widgets` | GUI elements like `QWidget`, `QDialog`   |
| `Qt::Sql`     | Database access using `QSqlDatabase`         |

These modules are automatically found via:
```cmake
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
```

---
&nbsp;


## 📁 Project Structure

```

InsuranceManagementSystem/
├── Forms/
│   ├── dialogs/
│   │   └── signupdialog.ui
│   └── widgets/
│       ├── loginwindow.ui
│       ├── mainwindow.ui
│       └── widget.ui
├── Header Files/
│   ├── database/
│   │   └── userdatabase.h
│   ├── dialogs/
│   │   ├── carinsurancedialog.h
│   │   ├── signupdialog.h
│   │   └── travelinsurancedialog.h
│   ├── models/
│   │   ├── insurance_application.h
│   │   └── person.h
│   └── widgets/
│       ├── loginwindow.h
│       ├── mainwindow.h
│       └── widget.h
├── Source Files/
│   ├── database/
│   │   └── userdatabase.cpp
│   ├── dialogs/
│   │   ├── carinsurancedialog.cpp
│   │   ├── signupdialog.cpp
│   │   └── travelinsurancedialog.cpp
│   ├── models/
│   │   ├── insurance_application.cpp
│   │   └── person.cpp
│   └── widgets/
│       ├── loginwindow.cpp
│       ├── mainwindow.cpp
│       └── widget.cpp
├── Resources/
│   └── resources.qrc
│       └── /new/prefix1
├── main.cpp
└── README.md

```

&nbsp;
- `Forms/`: UI forms (.ui)
- `Header Files/`: Header files (.h)
- `Source Files/`: Source files (.cpp)
- `Resources/`: Qt resource files (.qrc)
- `main.cpp`: Entry point of the application
- `README.md`: Project documentation

&nbsp;
