#ifndef MANAGER_H
#define MANAGER_H

#include <QApplication>
#include <QSqlDatabase>
#include "../CoreData/Plant/PlantInfo.h"
#include "../CoreData/CareRemind/Remind.h"

class Manager : public QObject {
    Q_OBJECT


private:
    QApplication app;
    class User * user;
    QSqlDatabase db;
    class PlantAssistant * plantAssistant;
    class CareRemind * careRemind;
    class PlantInformationManage * plantInformationManage;

    QList<Plant> plants;
    // QList<Task> tasks;
    QList<int> selectedPlantIndex;
    QList<Remind> careReminds;

    bool selectionMode;


public:
    Manager(int argc, char * argv[], QObject * parent = nullptr);

    ~Manager();

    int run();


    QSqlDatabase & getDb() { return db;}

    void setPlantAssistant(class PlantAssistant * plantAssistant) {
        this->plantAssistant = plantAssistant;
    }

    void setCareRemind(class CareRemind * careRemind) {
        this->careRemind = careRemind;
    }


    void setPlantInformationManage(class PlantInformationManage * plantInformationManage) {
        this->plantInformationManage = plantInformationManage;
    }

    class PlantAssistant * getPlantAssistant() const { return plantAssistant; }

    class CareRemind * getCareRemind() const { return careRemind; }

    class PlantInformationManage * getPlantInformationManage() const { return plantInformationManage; }

    void showPlantAssistant();

    void setUser(class User * user);

    class User * getUser() const;

    QList<Plant> & getPlants() { return plants; }

    void addNewPlant(const Plant & plant);

    QList<int> & getSelectedPlantIndex() { return selectedPlantIndex; }

    bool getSelectionMode() const { return selectionMode; }

    void setSelectionMode(bool selectionMode) { this->selectionMode = selectionMode; }

    QList<Remind> & getCareReminds() { return careReminds; }

    void addNewCareRemind(const Remind & remind);

    void deleteCareRemind(const QString & plantID);

    Plant getPlant(const QString & plantID) const;


};



#endif