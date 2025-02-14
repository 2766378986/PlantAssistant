#pragma once

#include <QMainWindow>
#include <QIcon>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~Login();

    void enableMica(bool darkMode = false);

    void enableAcrylic();

private:
    void initUI();

    QWidget* CreateLoginWidget(QWidget * parent = nullptr);

    QWidget * CreateRegistryWidget(QWidget * parent = nullptr);

    void addButtonAnimation(class QPushButton * button);

    void addSource();

    void addStyleSheet();

    QString encryptPassword(const QString & password);

private slots:
    void buttonClickedLogin();

    void buttonClickedRegistry();

protected:
    virtual void closeEvent(QCloseEvent * event) override;

private:
    Ui::Login *ui;

    Manager * manager;
    class QSqlDatabase & db;
    class QLabel * labelWelcome;
    class QLabel * labelIntro;
    class QLineEdit * lineEditUsername;
    class QLineEdit * lineEditPassword;
    class QPushButton * buttonShowPassword;
    class QPushButton * buttonLogin;
    class QPushButton * buttonRegistry;

    QIcon iconUsername;
    QIcon iconPassword;
    QIcon iconVisiblePassword;
    QIcon iconInvisiblePassword;
    QIcon iconConfirmPassword;
    QIcon iconGoBack;
    QIcon iconLogo;


    QLabel * labelRegistry;
    QLabel * labelRegistryIntro;
    QLineEdit * lineEditRegistryUsername;
    QLineEdit * lineEditRegistryPassword;
    QLineEdit * lineEditRegistryConfirmPassword;
    QPushButton * buttonRegistryConfirm;
    QPushButton * buttonRegistryGoBack;

    QAction * actionShowPassword;
    QAction * actionShowPasswordRegistry;

    bool loginSuccess = false;

    class QStackedWidget * stackedWidgetLoginRegistry;
    

};
