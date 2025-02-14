#include "Remind.h"

const QList<QString> Remind::columnNames = {
    "ReminderID",
    "PlantID",
    "ReminderType",
    "ReminderDate",
    "IsCompleted",
    "UserName"

};

void Remind::setValues(const QList<QString> & values) {
    this->values = values;
}

QString & Remind::getValue(RemindType type) {
    return values[type];
}

const QString & Remind::getValue(RemindType type) const {
    return values[type];
}
