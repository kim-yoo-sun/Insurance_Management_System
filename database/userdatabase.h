#ifndef USERDATABASE_H
#define USERDATABASE_H

#include "../models/insurance_application.h"
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QList>

class Person;

class UserDatabase {
public:
    static const Person* authenticate(const QString &id, const QString &password);

    static bool initialize();  // DB 연결 및 테이블 생성
    static bool addUser(const QString &id, const QString &pw, const QString &name);
    static bool exists(const QString &id);

    static bool submitCarInsurance(const QString &userId, const QString &summary);
    static bool submitTravelInsurance(const QString &userId, const QString &summary);

    static bool updateStatusById(int appId, const QString& newStatus);

    static QStringList loadApplicationsForUser(const QString &userId);       // User  용
    static QStringList loadAllApplications();                                // Admin 용

    static bool deleteApplicationById(int appId);
    static bool updateApplicationById(int appId, const QString &newSummary);
    static QList<QList<QString>> loadAllApplication();
    static QList<QList<QString>> loadApplicationsForUserDetail(const QString &userId);

    static QList<QList<QString>> searchApplicationsByUserId(const QString &keyword);

private:
    static QSqlDatabase db;
};

#endif // USERDATABASE_H
