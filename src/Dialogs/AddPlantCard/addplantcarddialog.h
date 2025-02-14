#pragma once

#include <QDialog>
#include <QVariant>
#include "../../CoreData/Plant/PlantInfo.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QDateTimeEdit;
class QPushButton;
class QGroupBox;
// class Manager;
class Plant;

namespace Ui {
class AddPlantCardDialog;
}

class AddPlantCardDialog : public QDialog
{
    Q_OBJECT

public:
    enum DialogType {
        ADD,
        EDIT,
        SHOW
    };


public:
    explicit AddPlantCardDialog(QWidget *parent = nullptr, class Manager * manager = nullptr, DialogType dialogType = DialogType::ADD, Plant plant = Plant());
    ~AddPlantCardDialog();
    QList<QString> getPlantData() const;

    void setUniqueShow(bool uniqueShow);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setupUI();

    void addStyle();

    void setupInfo();

    void loadPicture(const QString & photoPath);

private slots:
    void onUploadPhoto();

    void fadeOut();

private:
    Ui::AddPlantCardDialog *ui;

    QPushButton *uploadButton;
    QLabel *photoLabel;
    QLineEdit *nameEdit;
    QComboBox *speciesCombo;
    QLineEdit *wateringEdit, *fertilizingEdit, *pruningEdit;
    QComboBox *lightCombo, *soilCombo;
    QLineEdit *temperatureEdit, *humidityEdit;
    QDateTimeEdit *dateAddedEdit, *lastWateredEdit, *lastFertilizedEdit, *lastPrunedEdit;
    QString photoPath_;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    Manager *manager;
    Plant plant;

    DialogType dialogType;    
};
