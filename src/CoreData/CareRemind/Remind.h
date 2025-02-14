#ifndef CARE_MIND_H
#define CARE_MIND_H

#include <QString>
#include <QList>

class Remind {
public:
    enum RemindType {
        RemindType_ID,
        RemindType_PlantID,
        RemindType_Type,
        RemindType_Time,
        RemindType_Status,
        RemindType_Username,
    };

    static const QList<QString> columnNames;

public:
    QList<QString> values;

    void setValues(const QList<QString> & values);

    QString& getValue(RemindType type);

    const QString& getValue(RemindType type) const;
};





#endif // CARE_MIND_H


