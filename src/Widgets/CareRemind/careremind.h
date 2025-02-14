#pragma once

#include <QWidget>

namespace Ui {
class CareRemind;
}

class CareRemind : public QWidget
{
    Q_OBJECT

    enum BatchOperationType {
        BATCH_FINISHED,
        BATCH_DELETED,
    };

public:
    explicit CareRemind(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~CareRemind();

    void deleteTask(const QString & plantID);

    void deleteTask(int index);
    
    void finishTask(int index);


private:
    void initTopSection();

    void initMiddleSection();

    void initBottomSection();

    void initUI();

    void addStyle();

    void initConnect();

    void loadCareReminds();

    void enterSelectionMode();

    void exitSelectionMode();

private slots:
    void addTask();

    void batchFinished();

    void batchDelete();

    void batchOperationConfirm();

    void onSearchChanged(const QString & text);

    void onFilterChanged(const QString & filter);

    void checkTimeoutTasks();

private:
    Ui::CareRemind *ui;

    class Manager * manager;

    class QHBoxLayout * topLayout;
    class QHBoxLayout * middleLayout;
    class QHBoxLayout * bottomLayout;

    // 顶部
    class QLineEdit * searchLineEdit;
    class QComboBox * filterComboBox;
    class QPushButton * addButton;

    // 中间
    // class QVBoxLayout * taskLayout;
    class QScrollArea * scrollArea;
    class QListWidget * taskListWidget;
    class TaskWidgetManager * taskWidgetManager;


    // 底部
    class QLabel * finishedLabel;
    class QLabel * finishedCountLabel;

    class QLabel * unfinishedLabel;
    class QLabel * unfinishedCountLabel;

    class QPushButton * batchFinishedButton;
    class QPushButton * batchDeleteButton;
    class QPushButton * batchOperationConfirmButton;

    BatchOperationType batchOperationType;

    class QTimer * checkTimer; // 检测任务是否完成

    class QSoundEffect * soundEffect;

};
