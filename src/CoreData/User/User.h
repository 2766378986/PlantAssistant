#ifndef PLANTASSISTANT_USER_H
#define PLANTASSISTANT_USER_H

#include <QString>

class User {
private:
    QString name;
    QString createdTime;
    QString imageUrl;

public:
    User(QString name, QString createdTime, QString imageUrl);
    ~User();

    QString getName() const;
    QString getCreatedTime() const;
    QString getImageUrl() const;
};


#endif
