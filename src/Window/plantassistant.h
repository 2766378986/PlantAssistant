#pragma once

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class PlantAssistant;
}
QT_END_NAMESPACE

class PlantAssistant : public QMainWindow
{
    Q_OBJECT

private:
    QWidget * CreateUserInfo(QWidget * parent = nullptr);

    void addSource();

    void addStyle();

    void initUI();
    

private slots:
    void on_exit_clicked();

public:
    PlantAssistant(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~PlantAssistant();

    void setManager(class Manager * manager);

    QList<class Plant> * getPlantList();
  

protected:
    virtual void showEvent(QShowEvent * event) override;
private:
    Ui::PlantAssistant *ui;
    class Manager * manager;

    class QListWidget * navigationList;
    class QWidget * userWidget;
    class QLabel * headLabel;
    class QLabel * nameLabel;
    class QLabel * infoLabel;

    class QStackedWidget * contentStack;

    class QWidget * mainWidget;
    

    QIcon iconLogo;

    // 各个页面
    class PlantInformationManage * plantInformationManage;
    class CareRemind * careRemind;

};
