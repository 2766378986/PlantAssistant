#pragma once

#include <QDialog>

namespace Ui {
class PopUpDialog;
}

class PopUpDialog : public QDialog
{
    Q_OBJECT

private:
    static int width_;
    static int height_;

private:
    void addStyle();

public:
    explicit PopUpDialog(QWidget *parent = nullptr, const QString & message = "", int timeOut = 2000);
    ~PopUpDialog();

    static void information(QWidget * parent, const QString & message, int timeOut = 2000);

protected:
    virtual void showEvent(QShowEvent * event) override;

private slots:
    void fadeOut();

private:
    Ui::PopUpDialog *ui;

    class QLabel * labelIcon;
    class QLabel * labelMessage;

    int timeOut;


    static QList<PopUpDialog*> activeDialogs;  // 跟踪所有活动对话框
    static const int SPACING;            // 对话框间距
};
