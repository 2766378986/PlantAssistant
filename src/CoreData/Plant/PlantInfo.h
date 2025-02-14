#ifndef PLANTASSISTANT_PLANTINFO_H
#define PLANTASSISTANT_PLANTINFO_H

#include <QString>
#include <QList>
class Plant {
public:

    enum PlantInfoType {
        PLANT_ID,
        PLANT_NAME,
        PLANT_SPECIES,
        PLANT_PHOTO,
        PLANT_WATERING_RATE,
        PLANT_FERTILIZING_RATE,
        PLANT_PRUNE_RATE,
        PLANT_LIGHT_REQUIREMENT,
        PLANT_TEMPERATURE_REQUIREMENT,
        PLANT_HUMIDITY_REQUIREMENT,
        PLANT_SOIL_TYPE,
        DATE_ADDED,
        LAST_WATERED,
        LAST_FERTILIZED,
        LAST_PRUNED,
        USER_NAME,
    };

    QList<QString> values;

    void setValues(const QList<QString> & values);
};

#endif // PLANTASSISTANT_PLANTINFO_H
