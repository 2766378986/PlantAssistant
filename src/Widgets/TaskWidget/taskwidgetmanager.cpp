#include "taskwidgetmanager.h"
#include "ui_taskwidgetmanager.h"
#include "../../Manager/Manager.h"
#include "taskwidget.h"
#include <QVBoxLayout>

TaskWidgetManager::TaskWidgetManager(QWidget *parent, class Manager * manager)
    : QWidget(parent)
    , ui(new Ui::TaskWidgetManager)
    , manager(manager)
{
    ui->setupUi(this);

    mainLayout = new QVBoxLayout(this);

    size = 0;

    selectedNum = 0;
    selectionMode = false;

    setObjectName("taskWidgetManager");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("#taskWidgetManager { background-color: white; }");


}

void TaskWidgetManager::addTaskWidget(class TaskWidget * taskWidget) {
    mainLayout->addWidget(taskWidget);
    taskWidgets.append(taskWidget);
    size++;
}

void TaskWidgetManager::removeTaskWidget(class TaskWidget * taskWidget) {
    mainLayout->removeWidget(taskWidget);
    taskWidgets.removeOne(taskWidget);
    size--;
}

void TaskWidgetManager::clearTaskWidgets() {
    for (int i = 0; i < size; i++) {
        removeTaskWidget(taskWidgets[i]);
    }
}

void TaskWidgetManager::setSpacing(int spacing) {
    mainLayout->setSpacing(spacing);
}

void TaskWidgetManager::setAlignment(Qt::Alignment alignment) {
    mainLayout->setAlignment(alignment);
}

QList<int> & TaskWidgetManager::getSelectedIndexes() {
    return selectedIndexes;
}

QList<class TaskWidget *> & TaskWidgetManager::getTaskWidgets() {
    return taskWidgets;
}

bool TaskWidgetManager::getSelectionMode() const {
    return selectionMode;
}

void TaskWidgetManager::setSelectionMode(bool selectionMode) {
    this->selectionMode = selectionMode;

    if (selectionMode) {
        selectedIndexes.clear();
        selectedNum = 0;
    }
}

void TaskWidgetManager::onSelectedChanged(int index) {
    if (!selectionMode) return;

    auto taskWidget = taskWidgets[index];

    if (taskWidget->isSelected()) {
        selectedIndexes.append(index);
        selectedNum++;
    } else {
        selectedIndexes.removeOne(index);
        selectedNum--;
    }

}

void TaskWidgetManager::resetSelection() {
    bool previousSelectionMode = selectionMode;
    selectionMode = false;

    for (auto tw : taskWidgets) {
        tw->setSelected(false);
    }

    selectedNum = 0;
    selectedIndexes.clear();

    selectionMode = previousSelectionMode;
}


QList<class TaskWidget *>  TaskWidgetManager::getSelectedTaskWidgets() {
    QList<TaskWidget *> selectedTaskWidgets;

    for (int i = 0; i < selectedNum; i++) {
        selectedTaskWidgets.append(taskWidgets[selectedIndexes[i]]);
    }

    return selectedTaskWidgets;
}

class TaskWidget * TaskWidgetManager::getTaskWidget(int index) const {
    return taskWidgets[index];
}

int TaskWidgetManager::getSize() const {
    return size;
}

int TaskWidgetManager::getSelectedNum() const {
    return selectedNum;
}



TaskWidgetManager::~TaskWidgetManager()
{
    delete ui;
}
