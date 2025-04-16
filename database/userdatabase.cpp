#include "userdatabase.h"
#include "../models/person.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// 정적 멤버 초기화
QSqlDatabase UserDatabase::db = QSqlDatabase();

bool UserDatabase::initialize() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qWarning() << "DB Open Failed:" << db.lastError();
        return false;
    }

    QSqlQuery query;
    bool userTableOk = query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id TEXT PRIMARY KEY,
            pw TEXT,
            name TEXT
        )
    )");

    QSqlQuery appQuery;
    bool appTableOk = appQuery.exec(R"(
        CREATE TABLE IF NOT EXISTS applications (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id TEXT,
            type TEXT,
            summary TEXT,
            status TEXT DEFAULT '처리 중',
            created_at TEXT DEFAULT (datetime('now'))
        )
    )");

    if (!exists("admin")) {
        addUser("admin", "admin", "admin");  // ID, PW, Name 모두 "admin"
    }

    return userTableOk && appTableOk;
}

bool UserDatabase::addUser(const QString &id, const QString &pw, const QString &name) {
    if (exists(id)) return false;

    QSqlQuery query;
    query.prepare("INSERT INTO users (id, pw, name) VALUES (?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(pw);
    query.addBindValue(name);
    return query.exec();
}

bool UserDatabase::exists(const QString &id) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    return query.next();
}

const Person* UserDatabase::authenticate(const QString &id, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT id, pw, name FROM users WHERE id = ? AND pw = ?");
    query.addBindValue(id);
    query.addBindValue(password);
    if (query.exec() && query.next()) {
        // Person은 new로 생성해서 반환하고, 사용자는 복사해서 쓸 것
        return new Person(query.value(0).toString(), query.value(1).toString(), query.value(2).toString());
    }
    return nullptr;
}

bool UserDatabase::submitCarInsurance(const QString &userId, const QString &summary)
{
    QSqlQuery query;
    query.prepare("INSERT INTO applications (user_id, type, summary) VALUES (?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue("car");
    query.addBindValue(summary);
    return query.exec();
}

bool UserDatabase::submitTravelInsurance(const QString &userId, const QString &summary)
{
    QSqlQuery query;
    query.prepare("INSERT INTO applications (user_id, type, summary) VALUES (?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue("travel");
    query.addBindValue(summary);
    return query.exec();
}

QStringList UserDatabase::loadApplicationsForUser(const QString &userId) {
    QStringList result;
    QSqlQuery query;
    query.prepare("SELECT summary, status FROM applications WHERE user_id = ?");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            QString line = query.value(0).toString() + " | 상태: " + query.value(1).toString();
            result << line;
        }
    }
    return result;
}

QList<QList<QString>> UserDatabase::loadAllApplication() {
    QList<QList<QString>> result;
    QSqlQuery query("SELECT id, user_id, summary, status FROM applications");
    while (query.next()) {
        QList<QString> row;
        row << query.value(0).toString();  // id (PK)
        row << query.value(1).toString();  // user_id
        row << query.value(2).toString();  // summary
        row << query.value(3).toString();  // status
        result << row;
    }
    return result;
}

bool UserDatabase::updateStatusById(int appId, const QString& newStatus) {
    QSqlQuery query;
    query.prepare("UPDATE applications SET status = ? WHERE id = ?");
    query.addBindValue(newStatus);
    query.addBindValue(appId);
    if (!query.exec()) {
        qWarning() << "UPDATE 실패:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserDatabase::deleteApplicationById(int appId) {
    QSqlQuery query;
    query.prepare("DELETE FROM applications WHERE id = ?");
    query.addBindValue(appId);
    return query.exec();
}


QList<QList<QString>> UserDatabase::loadApplicationsForUserDetail(const QString &userId)
{
    QList<QList<QString>> result;
    QSqlQuery query;
    query.prepare("SELECT id, type, summary, status FROM applications WHERE user_id = ?");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            QList<QString> row;
            row << query.value(0).toString(); // id
            row << query.value(1).toString(); // type (car / travel)
            row << query.value(2).toString(); // summary
            row << query.value(3).toString(); // status
            result << row;
        }
    }
    return result;
}

bool UserDatabase::updateApplicationById(int appId, const QString &newSummary)
{
    QSqlQuery query;
    query.prepare("UPDATE applications SET summary = ? WHERE id = ?");
    query.addBindValue(newSummary);
    query.addBindValue(appId);

    return query.exec();
}

QList<QList<QString>> UserDatabase::searchApplicationsByUserId(const QString &keyword) {
    QList<QList<QString>> result;
    QSqlQuery query;
    query.prepare("SELECT id, user_id, summary, status FROM applications WHERE user_id LIKE ?");
    query.addBindValue("%" + keyword + "%");

    if (query.exec()) {
        while (query.next()) {
            QList<QString> row;
            row << query.value(0).toString();  // id
            row << query.value(1).toString();  // user_id
            row << query.value(2).toString();  // summary
            row << query.value(3).toString();  // status
            result << row;
        }
    } else {
        qWarning() << "Search query failed:" << query.lastError();
    }

    return result;
}
