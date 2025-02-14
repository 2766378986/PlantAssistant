#include "Manager.h"
#include "../Window/login.h"
#include "../Window/plantassistant.h"
#include "../CoreData/User/User.h"
#include "../CoreData/CareRemind/Remind.h"
#include "../Dialogs/MessageBox/pmessagebox.h"
#include <QWidget>


Manager::Manager(int argc, char * argv[], QObject * parent) : QObject(parent), app(argc, argv), plantAssistant(nullptr) {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("plantassistant");
    db.setUserName("root");
    db.setPassword("zhangshihao");
    if (!db.open()) {
        PMessagebox::critical(nullptr, "数据库连接失败", "无法连接到数据库");
    }


}

void Manager::setUser(User * user) {    
    this->user = user;
}

User * Manager::getUser() const {
    return this->user;
}

Manager::~Manager() {
    delete plantAssistant;
}

void Manager::showPlantAssistant() {
    plantAssistant->show();
    plantAssistant->activateWindow();
}

int Manager::run() {
    Login login(nullptr, this);
    login.show();


    return app.exec();
}

void Manager::addNewPlant(const Plant & plant) {
    plants.append(plant);
}

void Manager::addNewCareRemind(const Remind & remind) {
    careReminds.append(remind);
}

void Manager::deleteCareRemind(const QString & plantID) {
    QList<QList<Remind>::iterator> remindToDelete;

    for (auto it = careReminds.begin(); it != careReminds.end(); ++it) {
        if (it->getValue(Remind::RemindType::RemindType_PlantID) == plantID) {
            remindToDelete.append(it);
        }
    }
    
    for (auto it : remindToDelete) {
        careReminds.erase(it);
    }

}


Plant Manager::getPlant(const QString & plantID) const {
    for (const auto & plant : plants) {
        if (plant.values[Plant::PLANT_ID] == plantID) {
            return plant;
        }
    }
    return Plant();
}

