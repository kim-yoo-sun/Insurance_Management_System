cmake_minimum_required(VERSION 3.16)

project(InsuranceManagementSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(InsuranceManagementSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        # dialogs/
        dialogs/signupdialog.h
        dialogs/signupdialog.cpp
        dialogs/signupdialog.ui
        dialogs/carinsurancedialog.h
        dialogs/carinsurancedialog.cpp
        dialogs/travelinsurancedialog.h
        dialogs/travelinsurancedialog.cpp

        # widgets/
        widgets/widget.h
        widgets/widget.cpp
        widgets/widget.ui
        widgets/loginwindow.h
        widgets/loginwindow.cpp
        widgets/loginwindow.ui
        widgets/mainwindow.h
        widgets/mainwindow.cpp
        widgets/mainwindow.ui

        # models/
        models/person.h
        models/person.cpp
        models/insurance_application.h
        models/insurance_application.cpp

        # database/
        database/userdatabase.h
        database/userdatabase.cpp

        # main entry
        main.cpp
        resources.qrc
        README.md

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET InsuranceManagementSystem APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(InsuranceManagementSystem SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(InsuranceManagementSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(InsuranceManagementSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.InsuranceManagementSystem)
endif()
set_target_properties(InsuranceManagementSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS InsuranceManagementSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(InsuranceManagementSystem)
endif()
