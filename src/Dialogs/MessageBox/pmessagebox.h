#pragma once

#include <QDialog>

namespace Ui {
class PMessagebox;
}


class PMessagebox : public QDialog
{
    Q_OBJECT

public:
    enum ButtonType {
        OK,
        OK_CANCEL
    };

    enum Result {
        RESULT_OK,
        RESULT_CANCEL
    };

    enum MessageType {
        INFORMATION,
        CRITICAL
    };

public:
    explicit PMessagebox(QWidget *parent = nullptr, const QString & title = "", const QString & message = "", ButtonType buttonType = OK, MessageType messageType = INFORMATION);
    ~PMessagebox();

    static Result information(QWidget * parent, const QString & title, const QString & message, ButtonType buttonType = OK);

    static Result critical(QWidget * parent, const QString & title, const QString & message, ButtonType buttonType = OK);


private:
    void initUIOk();
    void initUI_Ok_Cancel();

    void addButtonAnimation(class QPushButton * button);

private:
    Ui::PMessagebox *ui;

    class QLabel * labelIcon;
    class QLabel * labelMessage;
    class QPushButton * buttonOk;
    class QPushButton * buttonCancel;

    QIcon iconTip;
};
