#include "User.h"

User::User(QString name, QString createdTime, QString imageUrl)
    : name(name), createdTime(createdTime), imageUrl(imageUrl) {}

User::~User() {

}

QString User::getName() const {
    return name;
}

QString User::getCreatedTime() const {
    return createdTime;
}

QString User::getImageUrl() const {
    return imageUrl;
}



