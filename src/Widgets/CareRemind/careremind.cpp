#include "careremind.h"
#include "ui_careremind.h"
#include "../../Manager/manager.h"
#include "../TaskWidget/taskwidget.h"
#include "../../Dialogs/PopUpDialog/popupdialog.h"
#include "../../Dialogs/AddRemindTask/addremindtask.h"
#include "../../Dialogs/MessageBox/pmessagebox.h"
#include "../../CoreData/User/User.h"
#include "../TaskWidget/taskwidgetmanager.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QScrollArea>
#include <QListWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <qpushbutton.h>
#include <QTimer>
#include <QSoundEffect>

CareRemind::CareRemind(QWidget *parent, Manager * manager)
    : QWidget(parent)
    , ui(new Ui::CareRemind)
    , manager(manager)
{
    ui->setupUi(this);
    
    initUI();

    checkTimer = new QTimer(this);
    checkTimer->setInterval(3 * 1000); // 每3秒检查一次

    connect(checkTimer, &QTimer::timeout, this, &CareRemind::checkTimeoutTasks);
    checkTimer->start();

    soundEffect = new QSoundEffect(this);
    soundEffect->setSource(QUrl::fromLocalFile("./res/sounds/alarm.wav"));

    addStyle();
}

void CareRemind::addStyle() {
    QFile file("./res/QSS/careRemind.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

void CareRemind::initUI() {
    initTopSection();
    initMiddleSection();
    initBottomSection();

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    // mainLayout->addStretch();
    mainLayout->addLayout(middleLayout);
    // mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    initConnect();

    loadCareReminds();

}


void CareRemind::initConnect() {
    connect(batchFinishedButton, &QPushButton::clicked, this, &CareRemind::batchFinished);
    connect(batchDeleteButton, &QPushButton::clicked, this, &CareRemind::batchDelete);
    connect(addButton, &QPushButton::clicked, this, &CareRemind::addTask);
    connect(batchOperationConfirmButton, &QPushButton::clicked, this, &CareRemind::batchOperationConfirm);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &CareRemind::onSearchChanged);
    connect(filterComboBox, &QComboBox::currentTextChanged, this, &CareRemind::onFilterChanged);
}

void CareRemind::initTopSection() {
    topLayout = new QHBoxLayout;

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setObjectName("searchLineEdit");
    QIcon searchIcon("./res/icons/search.png");
    searchLineEdit->addAction(searchIcon, QLineEdit::LeadingPosition);
    searchLineEdit->setPlaceholderText("请输入植物信息");

    filterComboBox = new QComboBox(this);
    filterComboBox->setObjectName("filterComboBox");
    filterComboBox->addItem("全部");
    filterComboBox->addItem("浇水");
    filterComboBox->addItem("施肥");
    filterComboBox->addItem("修剪");

    addButton = new QPushButton(this);
    addButton->setObjectName("addButton");
    addButton->setText("添加提醒");

    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(filterComboBox);
    topLayout->addWidget(addButton);
}


void CareRemind::initMiddleSection() {
    middleLayout = new QHBoxLayout();

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");

    taskWidgetManager = new TaskWidgetManager(this, manager);
    taskWidgetManager->setSpacing(5);
    taskWidgetManager->setAlignment(Qt::AlignTop);
    
    scrollArea->setWidget(taskWidgetManager);
    scrollArea->setWidgetResizable(true);

    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    middleLayout->addWidget(scrollArea);
}

void CareRemind::initBottomSection() {
    bottomLayout = new QHBoxLayout;

    finishedLabel = new QLabel("已完成", this);
    finishedLabel->setObjectName("finishedLabel");

    finishedCountLabel = new QLabel("0", this);
    finishedCountLabel->setObjectName("finishedCountLabel");


    unfinishedLabel = new QLabel("未完成", this);
    unfinishedLabel->setObjectName("unfinishedLabel");

    unfinishedCountLabel = new QLabel("0", this);
    unfinishedCountLabel->setObjectName("unfinishedCountLabel");

    batchFinishedButton = new QPushButton("批量标记完成", this);
    batchFinishedButton->setObjectName("batchFinishedButton");

    batchDeleteButton = new QPushButton("批量删除", this);
    batchDeleteButton->setObjectName("batchDeleteButton");

    batchOperationConfirmButton = new QPushButton(this);
    batchOperationConfirmButton->setObjectName("batchOperationConfirmButton");
    batchOperationConfirmButton->setIcon(QIcon("./res/icons/checkmark.png")); // 对号图标
    batchOperationConfirmButton->setIconSize(QSize(24, 24)); // 图标大小


    batchOperationConfirmButton->hide();

    bottomLayout->addWidget(finishedLabel);
    bottomLayout->addWidget(finishedCountLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(unfinishedLabel);
    bottomLayout->addWidget(unfinishedCountLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(batchFinishedButton);
    bottomLayout->addWidget(batchDeleteButton);
    bottomLayout->addWidget(batchOperationConfirmButton);
    bottomLayout->addStretch();
}


void CareRemind::loadCareReminds() {
    const auto & careReminds = manager->getCareReminds();

    QSqlQuery query;
    QString sql = "SELECT carereminders.*, plants.plantName FROM carereminders, plants WHERE carereminders.username = :username AND carereminders.plantID = plants.plantID";
    query.prepare(sql);
    query.bindValue(":username", manager->getUser()->getName());
    
    if (!query.exec()) {
        PMessagebox::critical(this, "查询失败", query.lastError().text());
        return;
    }

    while (query.next()) {
        QList<QString> values;
        QString plantName = query.value("plantName").toString();

        // 从数据中读取信息
        for (int i = 0; i < Remind::columnNames.size(); ++i) {
            if (Remind::RemindType::RemindType_Time == i) {
                values.append(query.value(Remind::columnNames[i]).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            } else {
                values.append(query.value(Remind::columnNames[i]).toString());
            }

            
        }

        // 创建一个remind
        Remind remind;
        remind.setValues(values);

        // 创建一个taskContent
        TaskContent taskContent;
        taskContent.plantID = remind.getValue(Remind::RemindType::RemindType_PlantID);
        taskContent.plantName = plantName;
        taskContent.operation = remind.getValue(Remind::RemindType::RemindType_Type);
        taskContent.targetTime = remind.getValue(Remind::RemindType::RemindType_Time);
        taskContent.completedStatus = remind.getValue(Remind::RemindType::RemindType_Status) == "1" ? "已完成" : "未完成";

        if (taskContent.operation == "Watering") {
            taskContent.operation = "浇水";
        } else if (taskContent.operation == "Fertilizing") {
            taskContent.operation = "施肥";
        } else if (taskContent.operation == "Pruning") {
            taskContent.operation = "修剪";
        }

        // 创建一个taskWidget, 并设置index和完成状态
        auto taskWidget = new TaskWidget(taskWidgetManager, manager, taskContent);
        taskWidget->setIndex(manager->getCareReminds().size());
        taskWidget->setCompleted(taskContent.completedStatus == "已完成");

        // 添加到布局中
        // taskLayout->addWidget(taskWidget);
        taskWidgetManager->addTaskWidget(taskWidget);

        // 添加到manager中
        manager->addNewCareRemind(remind);

        // 更新label
        if (taskWidget->isCompleted()) {
            finishedCountLabel->setText(QString::number(finishedCountLabel->text().toInt() + 1));
        } else {
            unfinishedCountLabel->setText(QString::number(unfinishedCountLabel->text().toInt() + 1));
        }

    }
        
}


void CareRemind::addTask() {
    AddRemindTask dialog(this, manager);
    
    if (dialog.exec() == QDialog::Accepted) {
        auto selectedPlants = dialog.getSelectedPlants();
        auto selectedOperations = dialog.getSelectedOperations();
        auto selectedDateTime = dialog.getSelectedDateTime();
        QSqlQuery query;


        for (auto plant : selectedPlants.keys()) {
            for (auto operation : selectedOperations) {
                QString sql = "INSERT INTO carereminders (plantID, ReminderType, ReminderDate, IsCompleted, UserName) VALUES (:plantID, :type, :reminderDate, :completedStatus, :username)";

                // 加入到数据库
                query.prepare(sql);
                QString plantID = plant;
                QString type = operation == "浇水" ? "Watering" : operation == "施肥" ? "Fertilizing" : "Pruning";
                QString reminderDate = selectedDateTime.toString("yyyy-MM-dd hh:mm:ss");
                QString completedStatus = "0";
                QString username = manager->getUser()->getName();

                query.bindValue(":plantID", plantID);
                query.bindValue(":type", type);
                query.bindValue(":reminderDate", reminderDate);
                query.bindValue(":completedStatus", completedStatus);
                query.bindValue(":username", username);
                
                // 执行sql
                if (!query.exec()) {
                    PMessagebox::critical(this, "添加失败", query.lastError().text());
                    return;
                }

                // 获取ID，加入到manager的careReminds中
                QString ID = query.lastInsertId().toString();

                Remind remind;
                QList<QString> values;
                values.append(ID);
                values.append(plantID);
                values.append(type);
                values.append(reminderDate);
                values.append(completedStatus);
                values.append(username);

                remind.setValues(values);
                manager->addNewCareRemind(remind);
                
                // 创建新的提醒加入到taskLayout中
                TaskContent taskContent;
                taskContent.plantID = plantID;
                taskContent.plantName = selectedPlants[plant];
                taskContent.operation = operation;
                taskContent.targetTime = reminderDate;
                taskContent.completedStatus = "未完成";

                

                auto taskWidget = new TaskWidget(taskWidgetManager, manager, taskContent);
                taskWidget->setCompleted(false);
                taskWidget->setEnding(false);
                taskWidget->setIndex(manager->getCareReminds().size() - 1);

                // taskLayout->addWidget(taskWidget);
                taskWidgetManager->addTaskWidget(taskWidget);

                int currentUnfinished = unfinishedCountLabel->text().toInt();
                unfinishedCountLabel->setText(QString::number(currentUnfinished + 1));

            }
        }

        PopUpDialog::information(this, "添加成功");
    }
}


void CareRemind::deleteTask(const QString & plantID) {
    // 删除taskLayout中的任务
    QSqlQuery query;
    QString sql = "DELETE FROM carereminders WHERE plantID = :plantID";


    QList<TaskWidget *> taskToDelete;
    
    // 从数据库中删除，并且添加到删除队列
    int size = taskWidgetManager->getSize();
    for (int i = 0; i < size; ++i) {
        TaskWidget * taskWidget = taskWidgetManager->getTaskWidget(i);
        if (taskWidget->getPlantID() == plantID) {
            taskToDelete.append(taskWidget);
            query.prepare(sql);
            query.bindValue(":plantID", plantID);
            query.exec();
        }
    }

    // 从删除队列中删除，并更完成信息
    int currentUnfinished = unfinishedCountLabel->text().toInt();
    int currentFinished = finishedCountLabel->text().toInt();


    for (auto widget : taskToDelete) {
        if (widget->isCompleted()) {
            currentFinished--;
        } else {
            currentUnfinished--;
        }

        // taskLayout->removeWidget(widget);
        taskWidgetManager->removeTaskWidget(widget);

        delete widget;
    }

    manager->deleteCareRemind(plantID);

    unfinishedCountLabel->setText(QString::number(currentUnfinished));
    finishedCountLabel->setText(QString::number(currentFinished));
}

void CareRemind::deleteTask(int index) {
    // 先找到task
    // auto taskWidget = qobject_cast<TaskWidget *>(taskLayout->itemAt(index)->widget());
    auto taskWidget = taskWidgetManager->getTaskWidget(index);

    PopUpDialog::information(this, QString::number(index));

    // 从数据库中删除
    QSqlQuery query;
    QString sql = "DELETE FROM carereminders WHERE ReminderID = :reminderID";
    query.prepare(sql);
    query.bindValue(":reminderID", manager->getCareReminds()[index].getValue(Remind::RemindType::RemindType_ID));
    if (!query.exec()) {
        PMessagebox::critical(this, "删除失败", query.lastError().text());
        return;
    }

    // 从内存中删除时不能直接的删除，因为删除后index会有变化，会导致其余的index全部出错。
    // 要修改该taskwidget后面所有的taskwidget的index
    int size = taskWidgetManager->getSize();
    for (int i = index + 1; i < size; ++i) {
        auto latter = taskWidgetManager->getTaskWidget(i);
        latter->setIndex(latter->getIndex() - 1);
    }

    
    // 更新label
    int currentUnfinished = unfinishedCountLabel->text().toInt();
    int currentFinished = finishedCountLabel->text().toInt();

    if (taskWidget->isCompleted()) {
        currentFinished--;
    } else {
        currentUnfinished--;
    }

    unfinishedCountLabel->setText(QString::number(currentUnfinished));
    finishedCountLabel->setText(QString::number(currentFinished));
    
    // 从内存中删除
    // taskLayout->removeWidget(taskWidget);
    taskWidgetManager->removeTaskWidget(taskWidget);
    manager->getCareReminds().removeAt(index);

    delete taskWidget;
}


void CareRemind::finishTask(int index) {
    // 找到对应的task

    // auto taskWidget = qobject_cast<TaskWidget *>(taskLayout->itemAt(index)->widget());
    auto taskWidget = taskWidgetManager->getTaskWidget(index);
    auto& cares = manager->getCareReminds();

    // 将内存中的状态设置为已完成
    cares[index].getValue(Remind::RemindType::RemindType_Status) = "1";
    
    // 更新数据库
    QSqlQuery query;
    QString sql = "UPDATE carereminders SET IsCompleted = :isCompleted WHERE ReminderID = :reminderID";
    query.prepare(sql);
    query.bindValue(":isCompleted", "1");
    query.bindValue(":reminderID", cares[index].getValue(Remind::RemindType::RemindType_ID));
    query.exec();

    // 更新label
    int finishCount = finishedCountLabel->text().toInt();
    int unfinishedCount = unfinishedCountLabel->text().toInt();

    finishedCountLabel->setText(QString::number(finishCount + 1));
    unfinishedCountLabel->setText(QString::number(unfinishedCount - 1));

}

void CareRemind::enterSelectionMode() {
    taskWidgetManager->setSelectionMode(true);
    taskWidgetManager->resetSelection();
    addButton->setEnabled(false);
    batchFinishedButton->setEnabled(false);
    batchDeleteButton->setEnabled(false);
    batchOperationConfirmButton->show();
}

void CareRemind::exitSelectionMode() {
    taskWidgetManager->setSelectionMode(false);
    addButton->setEnabled(true);
    batchFinishedButton->setEnabled(true);
    batchDeleteButton->setEnabled(true);
    batchOperationConfirmButton->hide();
}


void CareRemind::batchFinished() {
    batchOperationType = BatchOperationType::BATCH_FINISHED;
    enterSelectionMode();
}

void CareRemind::batchDelete() {
    batchOperationType = BatchOperationType::BATCH_DELETED;
    enterSelectionMode();
}


void CareRemind::batchOperationConfirm() {
    exitSelectionMode();
    
    int selectedNum = taskWidgetManager->getSelectedNum();

    if (selectedNum == 0) {
        PopUpDialog::information(this, "请选择要操作的提醒");
        return;
    }

    QList<TaskWidget *> selectedTaskWidgets = taskWidgetManager->getSelectedTaskWidgets();

    for (int i = 0; i < selectedNum; ++i) {
        switch (batchOperationType) {
            case BatchOperationType::BATCH_FINISHED:
                selectedTaskWidgets[i]->onComplete();
                break;
            case BatchOperationType::BATCH_DELETED:
                selectedTaskWidgets[i]->onDelete();
                break;
        }
    }


    PopUpDialog::information(this, "操作成功");
    taskWidgetManager->resetSelection();
}

void CareRemind::onSearchChanged(const QString & text) {
    QString filter = filterComboBox->currentText();
    int taskCount = taskWidgetManager->getSize();

    int currentFinished = finishedCountLabel->text().toInt();
    int currentUnfinished = unfinishedCountLabel->text().toInt();

    for (int i = 0; i < taskCount; ++i) {
        TaskWidget * taskWidget = taskWidgetManager->getTaskWidget(i);
        bool matchSearch = text.isEmpty() || taskWidget->getPlantName().contains(text, Qt::CaseInsensitive);
        bool matchFilte = (filter == "全部") || taskWidget->getOperation().contains(filter);

        if (matchSearch && matchFilte) {
            taskWidget->show();
        } else {
            taskWidget->hide();
            if (taskWidget->isCompleted()) {
                currentFinished--;
            } else {
                currentUnfinished--;
            }
        }
    }

    // 更新label
    finishedCountLabel->setText(QString::number(currentFinished));
    unfinishedCountLabel->setText(QString::number(currentUnfinished));
}


void CareRemind::onFilterChanged(const QString & filter) {
    QString text = searchLineEdit->text();
    
    onSearchChanged(text);
}

void CareRemind::checkTimeoutTasks() {
    const auto & cares = manager->getCareReminds();
    auto currentTime = QDateTime::currentDateTime();

    int index = 0;

    bool hasAlarm = false;

    for (const auto & care : cares) {
        QDateTime reminderDate = QDateTime::fromString(care.getValue(Remind::RemindType::RemindType_Time), "yyyy-MM-dd hh:mm:ss");;
    
        auto taskWidget = taskWidgetManager->getTaskWidget(index);
        if (reminderDate > currentTime || taskWidget->isCompleted() || taskWidget->isEnding()) {
            index++;
            continue;
        }

        // 如果超时,并且还没完成,则设置已超时并提醒用户
        taskWidget->setEnding(true);
        PopUpDialog::information(this, QString("任务[%1],植物[%2]已经超时，请及时处理").arg(
            index).arg(taskWidget->getPlantName()));

        // 更新数据库
        hasAlarm = true;
        index++;
    }

    if (hasAlarm) {
        soundEffect->play();
    }
}


CareRemind::~CareRemind()
{
    delete ui;
}
