#pragma once

#include <QWidget>

namespace Ui {
class TaskWidgetManager;
}

class TaskWidgetManager : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool selectionMode READ getSelectionMode WRITE setSelectionMode)

public:
    explicit TaskWidgetManager(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~TaskWidgetManager();

    void addTaskWidget(class TaskWidget * taskWidget);

    void removeTaskWidget(class TaskWidget * taskWidget);

    void clearTaskWidgets();

    void setSpacing(int spacing);

    QList<int> & getSelectedIndexes();

    QList<class TaskWidget *> & getTaskWidgets();

    class TaskWidget * getTaskWidget(int index) const;

    bool getSelectionMode() const;

    void setSelectionMode(bool selectionMode);

    QList<class TaskWidget *> getSelectedTaskWidgets();

    void setAlignment(Qt::Alignment alignment);

    int getSize() const;

    int getSelectedNum() const;

    void resetSelection();

public slots:
    void onSelectedChanged(int index);

private:
    Ui::TaskWidgetManager *ui;

    class Manager * manager;
    class QVBoxLayout * mainLayout;

    int size;
    QList<class TaskWidget *> taskWidgets;
    QList<int> selectedIndexes;
    int selectedNum;

    bool selectionMode;

};
