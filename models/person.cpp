#include "person.h"

Person::Person(const QString &id, const QString &password, const QString &name)
    : id(id), password(password), name(name) {}

QString Person::getId() const {
    return id;
}

QString Person::getPassword() const {
    return password;
}

QString Person::getName() const {
    return name;
}
