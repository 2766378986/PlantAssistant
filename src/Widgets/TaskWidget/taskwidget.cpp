#include "taskwidget.h"
#include "ui_taskwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFile>
#include <QGraphicsBlurEffect>
#include <QMouseEvent>
#include <QStyle>
#include "../../Dialogs/AddPlantCard/addplantcarddialog.h"
#include "../../Manager/Manager.h"
#include "../CareRemind/careremind.h"
#include "taskwidgetmanager.h"


TaskWidget::TaskWidget(QWidget *parent, class Manager * manager, TaskContent taskContent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
    , taskContent(taskContent)
    , manager(manager)
{
    ui->setupUi(this);

    setFixedHeight(60);

    setObjectName("taskWidget");
    setAttribute(Qt::WA_StyledBackground, true);

    taskWidgetManager = dynamic_cast<TaskWidgetManager *>(parent);
    selected = false;    

    initUI();
    addStyle();
    initConnect();

    if (taskWidgetManager) {
        connect(this, &TaskWidget::selectedChanged, taskWidgetManager, &TaskWidgetManager::onSelectedChanged);
    }
}

void TaskWidget::initUI() {
    labelPlantID = new QLabel(this);
    labelPlantName = new QLabel(this);
    labelOperation = new QLabel(this);
    labelTargetTime = new QLabel(this);
    labelCompletedStatus = new QLabel(this);
    buttonComplete = new QPushButton(this);
    buttonDelete = new QPushButton(this);

    labelPlantID->setText(taskContent.plantID);
    labelPlantName->setText(taskContent.plantName);
    labelOperation->setText(taskContent.operation);
    labelTargetTime->setText(taskContent.targetTime);
    labelCompletedStatus->setText(taskContent.completedStatus);

    buttonComplete->setIcon(QIcon("./res/icons/checkmark.png")); // 对号图标
    buttonComplete->setIconSize(QSize(24, 24)); // 图标大小
    buttonComplete->setObjectName("completeButton");

    buttonDelete->setIcon(QIcon("./res/icons/delete.png")); // 叉号图标
    buttonDelete->setIconSize(QSize(24, 24)); // 图标大小
    buttonDelete->setObjectName("deleteButton");




    buttonComplete->setObjectName("completeButton");
    buttonDelete->setObjectName("deleteButton");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10); // 设置控件之间的间距
    mainLayout->setContentsMargins(10, 10, 10, 10); // 设置布局的内边距

    mainLayout->addWidget(labelPlantID);
    mainLayout->addWidget(labelPlantName);
    mainLayout->addWidget(labelOperation);
    mainLayout->addWidget(labelTargetTime);
    mainLayout->addWidget(labelCompletedStatus);
    mainLayout->addStretch(); // 添加伸缩空间
    mainLayout->addWidget(buttonComplete);
    mainLayout->addWidget(buttonDelete);
    mainLayout->addSpacing(20);

    setLayout(mainLayout);
}

void TaskWidget::initConnect() {
    connect(buttonComplete, &QPushButton::clicked, this, &TaskWidget::onComplete);
    connect(buttonDelete, &QPushButton::clicked, this, &TaskWidget::onDelete);
}


void TaskWidget::addStyle() {
    QFile file("./res/QSS/taskWidget.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = file.readAll();
        setStyleSheet(styleSheet);
        file.close();
    }
}


QString TaskWidget::getPlantID() const {
    return labelPlantID->text();
}


void TaskWidget::setCompleted(bool completed) {
    if (this->completed != completed) {
        this->completed = completed;
        style()->unpolish(this);  // 强制样式刷新
        style()->polish(this);
        update();
    }
}

void TaskWidget::setEnding(bool ending) {
    if (this->ending != ending) {
        this->ending = ending;
        style()->unpolish(this);
        style()->polish(this);
        update();
    }
}  

bool TaskWidget::isCompleted() const {
    return completed;
}

bool TaskWidget::isEnding() const {
    return ending;
}


void TaskWidget::onComplete() {
    if (completed) return;

    setCompleted(true);
    labelCompletedStatus->setText("已完成");
    manager->getCareRemind()->finishTask(index);
}

void TaskWidget::onDelete() {
    manager->getCareRemind()->deleteTask(index);
}

void TaskWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    AddPlantCardDialog dialog(this, manager, AddPlantCardDialog::DialogType::SHOW, manager->getPlant(labelPlantID->text()));
    dialog.setUniqueShow(true);
    dialog.exec();
}

void TaskWidget::setIndex(int index) {
    this->index = index;
}

bool TaskWidget::isSelected() const {
    return selected;
}

void TaskWidget::setSelected(bool selected) {
    bool previousSelected = this->selected;
    this->selected = selected;
    style()->polish(this);

    if (previousSelected != selected) {
        emit selectedChanged(index);
    }
}

void TaskWidget::mousePressEvent(QMouseEvent * event) {
    if (!taskWidgetManager->getSelectionMode() || event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    setSelected(!selected);
}

QString TaskWidget::getPlantName() const {
    return labelPlantName->text();
}

QString TaskWidget::getOperation() const {
    return labelOperation->text();
}


TaskWidget::~TaskWidget()
{
    delete ui;
}
