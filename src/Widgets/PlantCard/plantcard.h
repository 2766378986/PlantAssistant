#pragma once

#include <QWidget>
#include <qpropertyanimation.h>
#include "../../CoreData/Plant/PlantInfo.h"


namespace Ui {
class PlantCard;
}

class PlantCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ getSelected WRITE setSelected)
    Q_PROPERTY(bool selectionMode READ getSelectionMode WRITE setSelectionMode)

private:
    class QLabel * plantPhoto;
    class QLabel * plantName;
    class QLabel * plantSpecies;
    class QLabel * plantWateringRate;
    class QLabel * plantLightRequirement;
    class QLabel * plantTemperatureRequirement;
    class QLabel * plantLastWatered;

    class QLabel * plantWaterIcon;
    class QLabel * plantLightIcon;
    class QLabel * plantTemperatureIcon;

    class QPushButton * plantEditButton;
    class QPushButton * plantDeleteButton;
    int indexOfPlantArray;

    class PlantInformationManage * plantInformationManage;

    bool selected;

    bool selectionMode;

    class Manager * manager;
    
private:
    Plant plant;

    void addSource();

    void initUI();

    void addPlantToUI();

    void addStyle();

private slots:
    void editPlant();
    
    void deletePlant();

public:
    explicit PlantCard(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~PlantCard();

    void setPlant(const Plant & plant, int indexOfPlantArray);

    void setHide(bool hide);

    int getIndexOfPlantArray() const;

    void setSelected(bool selected);

    bool getSelected() const;

    void setSelectionMode(bool selectionMode);

    bool getSelectionMode() const;

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;

private:
    Ui::PlantCard *ui;



};
