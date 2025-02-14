#pragma once

#include <QDialog>
#include <QDateTime>

class QComboBox;
class QDateTimeEdit;


namespace Ui {
class AddRemindTask;
}

class AddRemindTask : public QDialog
{
    Q_OBJECT

public:
    explicit AddRemindTask(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~AddRemindTask();

    QString getSelectedOperation() const;
    QDateTime getSelectedDateTime() const;

    QMap<QString, QString> getSelectedPlants() const;
    QList<QString> getSelectedOperations() const;

private:
    Ui::AddRemindTask *ui;

    class QScrollArea * scrollAreaPlant;
    class SelectCardManager * cardManagerPlant;
    
    class QLabel * labelOperation;
    class SelectCardManager * cardManagerOperation;    

    class QLabel * labelRemindTime;
    QDateTimeEdit * dateTimeEdit;

    class Manager * manager;
};
