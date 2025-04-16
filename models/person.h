#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person {
public:
    Person(const QString &id, const QString &password, const QString &name);

    QString getId() const;
    QString getPassword() const;
    QString getName() const;

private:
    QString id;
    QString password;
    QString name;
};

#endif // PERSON_H
