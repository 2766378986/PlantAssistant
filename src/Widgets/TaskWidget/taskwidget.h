#pragma once

#include <QWidget>

namespace Ui {
class TaskWidget;
}

// TODO: 根据是否完成和是否逾期，设置不同的样式
// TODO: 双击打开植物详细信息对话框


struct TaskContent {
    QString plantID;
    QString plantName;
    QString operation;
    QString targetTime;
    QString completedStatus;
};

class TaskWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool completed READ isCompleted WRITE setCompleted);
    Q_PROPERTY(bool ending READ isEnding WRITE setEnding);
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected);

public:
    explicit TaskWidget(QWidget *parent = nullptr, class Manager * manager = nullptr, TaskContent taskContent = TaskContent());
    ~TaskWidget();

    bool isCompleted() const;
    bool isEnding() const;

    void setCompleted(bool completed);
    void setEnding(bool ending);

    QString getPlantID() const;

    QString getPlantName() const;

    QString getOperation() const;

    void setIndex(int index);

    int getIndex() const { return index; }

    bool isSelected() const;

    void setSelected(bool selected);

signals:
    void selectedChanged(int index);

private:
    void initUI();
    void addStyle();
    void initConnect();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    virtual void mousePressEvent(QMouseEvent * event) override;

public slots:
    void onComplete();
    void onDelete();

private:
    Ui::TaskWidget *ui;

    class QLabel * labelPlantID;
    class QLabel * labelPlantName;
    class QLabel * labelOperation;
    class QLabel * labelTargetTime;
    class QLabel * labelCompletedStatus;
    class QPushButton * buttonComplete;
    class QPushButton * buttonDelete;

    TaskContent taskContent;
    Manager * manager;

    class TaskWidgetManager * taskWidgetManager;

    bool completed = false;
    bool ending = false;
    bool selected = false;

    int index;
};
