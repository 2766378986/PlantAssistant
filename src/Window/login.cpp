#include "login.h"
#include "ui_login.h"
#include "../Manager/Manager.h"
#include "plantassistant.h"
#include "../Dialogs/MessageBox/pmessagebox.h"
#include "../CoreData/User/User.h"
#include "../Dialogs/PopUpDialog/popupdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QStackedWidget>
#include <QFile>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>


Login::Login(QWidget *parent, Manager * manager)
    : QMainWindow(parent)
    , ui(new Ui::Login)
    , manager(manager)
    , db(manager->getDb())
{
    ui->setupUi(this);

    setWindowTitle(" ");
    // 不显示最大化按钮
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    
    addSource();
    
    setWindowIcon(iconLogo);
    setFixedSize(QSize(1462, 743));
    initUI();

    addStyleSheet();

}

void Login::addSource() {
    iconUsername = QIcon("./res/icons/username.png");
    iconPassword = QIcon("./res/icons/password.png");
    iconVisiblePassword = QIcon("./res/icons/eye.png");
    iconInvisiblePassword = QIcon("./res/icons/eye-invisible.png");
    iconConfirmPassword = QIcon("./res/icons/confirm-password.png");
    iconGoBack = QIcon("./res/icons/go-back.png");
    iconLogo = QIcon("./res/icons/logo.png");
    

}

void Login::addStyleSheet() {
    lineEditUsername->setObjectName("lineEditUsername");
    lineEditPassword->setObjectName("lineEditPassword");
    buttonLogin->setObjectName("buttonLogin");
    buttonRegistry->setObjectName("buttonRegistry");

    lineEditRegistryUsername->setObjectName("lineEditRegistryUsername");
    lineEditRegistryPassword->setObjectName("lineEditRegistryPassword");
    lineEditRegistryConfirmPassword->setObjectName("lineEditRegistryConfirmPassword");
    buttonRegistryConfirm->setObjectName("buttonRegistryConfirm");
    buttonRegistryGoBack->setObjectName("buttonRegistryGoBack");

    stackedWidgetLoginRegistry->setObjectName("stackedWidgetLoginRegistry");


    
    QFile file("./res/QSS/sign.qss");
    file.open(QFile::ReadOnly);
    
    if (file.isOpen()) {
        QTextStream stream(&file);
        setStyleSheet(stream.readAll());
    }
}

void Login::initUI() {
    QWidget * mainWidget = new QWidget(this);
    QHBoxLayout * hMainLayout = new QHBoxLayout;

    // 左边图片
    QLabel * labelImage = new QLabel(mainWidget);
    labelImage->setPixmap(QPixmap("./res/image/Login.jpg"));
    labelImage->setAlignment(Qt::AlignTop);
    labelImage->setScaledContents(true);
    labelImage->setStyleSheet("border-radius: 15px;");
    labelImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    stackedWidgetLoginRegistry = new QStackedWidget(mainWidget);

    // 右边登录信息
    QWidget * loginInfo = CreateLoginWidget(mainWidget);

    // 添加登录界面
    stackedWidgetLoginRegistry->addWidget(loginInfo);
    
    // 添加注册界面
    QWidget * registryInfo = CreateRegistryWidget(mainWidget);
    stackedWidgetLoginRegistry->addWidget(registryInfo);

    hMainLayout->addSpacing(10);
    hMainLayout->addWidget(labelImage);
    // hMainLayout->addStretch();
    hMainLayout->addWidget(stackedWidgetLoginRegistry);
    hMainLayout->addStretch();

    mainWidget->setLayout(hMainLayout);
    setCentralWidget(mainWidget);
}


QWidget* Login::CreateLoginWidget(QWidget * parent) {
    QWidget * loginWidget = new QWidget(this);
    QVBoxLayout * vLoginLayout = new QVBoxLayout;

    labelWelcome = new QLabel("嗨！别来无恙", loginWidget);
    labelWelcome->setStyleSheet("font-size: 50px; font-weight: bold; color: black; min-width: 300px;");
    labelWelcome->setAlignment(Qt::AlignCenter);
    labelWelcome->setMaximumWidth(300);

    labelIntro = new QLabel("Welcome to the plant care assistant for the future", loginWidget);
    labelIntro->setStyleSheet("font-size: 20px; color: gray; min-width: 300px;");
    labelIntro->setAlignment(Qt::AlignCenter);
    labelIntro->setWordWrap(true);
    labelIntro->setMaximumWidth(300);

   
    lineEditUsername = new QLineEdit(loginWidget);
    lineEditUsername->setPlaceholderText("请输入用户名");
    lineEditUsername->addAction(iconUsername, QLineEdit::LeadingPosition);
    lineEditUsername->setMaximumWidth(300);



     // 创建密码输入框
    lineEditPassword = new QLineEdit(loginWidget);
    lineEditPassword->setPlaceholderText("请输入密码");
    lineEditPassword->addAction(iconPassword, QLineEdit::LeadingPosition);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditPassword->setMaximumWidth(300);
    actionShowPassword = lineEditPassword->addAction(iconInvisiblePassword, QLineEdit::TrailingPosition);
    lineEditPassword->setMaxLength(16);


    connect(actionShowPassword, &QAction::triggered, [this]() {
        if (lineEditPassword->echoMode() == QLineEdit::Password) {
            lineEditPassword->setEchoMode(QLineEdit::Normal);
            actionShowPassword->setIcon(iconVisiblePassword);
        } else {
            lineEditPassword->setEchoMode(QLineEdit::Password);
            actionShowPassword->setIcon(iconInvisiblePassword);
        }
    });



    // 登录按钮
    buttonLogin = new QPushButton("登录", loginWidget);
    addButtonAnimation(buttonLogin);
    buttonLogin->setFixedWidth(300);

    // 注册按钮
    buttonRegistry = new QPushButton("还没有账号？点击这里注册", loginWidget);
    connect(buttonRegistry, &QPushButton::clicked, [this]() {
        stackedWidgetLoginRegistry->setCurrentIndex(1);
    });
    // 设置大小依赖文本
    buttonRegistry->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    QHBoxLayout * hlabelWelcomeLayout = new QHBoxLayout;
    hlabelWelcomeLayout->addSpacing(250);
    hlabelWelcomeLayout->addWidget(labelWelcome);

    QHBoxLayout * hlabelIntroLayout = new QHBoxLayout;
    hlabelIntroLayout->addSpacing(250);
    hlabelIntroLayout->addWidget(labelIntro);

    QHBoxLayout * hlineEditUsernameLayout = new QHBoxLayout;
    hlineEditUsernameLayout->addSpacing(250);
    hlineEditUsernameLayout->addWidget(lineEditUsername);

    QHBoxLayout * hlineEditPasswordLayout = new QHBoxLayout;
    hlineEditPasswordLayout->addSpacing(250);
    hlineEditPasswordLayout->addWidget(lineEditPassword);

    QHBoxLayout * hbuttonLoginLayout = new QHBoxLayout;
    hbuttonLoginLayout->addSpacing(250);
    hbuttonLoginLayout->addWidget(buttonLogin);

    QHBoxLayout * hbuttonRegistryLayout = new QHBoxLayout;
    hbuttonRegistryLayout->addSpacing(400);
    hbuttonRegistryLayout->addWidget(buttonRegistry);

    vLoginLayout->addStretch();
    vLoginLayout->addLayout(hlabelWelcomeLayout);
    vLoginLayout->addSpacing(20);
    vLoginLayout->addLayout(hlabelIntroLayout);
    vLoginLayout->addSpacing(20);
    vLoginLayout->addLayout(hlineEditUsernameLayout);
    vLoginLayout->addSpacing(10);
    vLoginLayout->addLayout(hlineEditPasswordLayout);
    vLoginLayout->addSpacing(5);
    vLoginLayout->addLayout(hbuttonRegistryLayout);    
    vLoginLayout->addSpacing(5);
    vLoginLayout->addLayout(hbuttonLoginLayout);
    vLoginLayout->addStretch();
    loginWidget->setLayout(vLoginLayout);


    connect(buttonLogin, &QPushButton::clicked, this, &Login::buttonClickedLogin);

    return loginWidget;
}

QWidget * Login::CreateRegistryWidget(QWidget * parent) {
    QWidget * registryWidget = new QWidget(parent);
    QVBoxLayout * vRegistryLayout = new QVBoxLayout;

    labelRegistry = new QLabel("注册", registryWidget);
    labelRegistry->setStyleSheet("font-size: 50px; font-weight: bold; color: black; min-width: 300px;");
    labelRegistry->setAlignment(Qt::AlignCenter);
    labelRegistry->setMaximumWidth(300);

    labelRegistryIntro = new QLabel("Welcome to join the plant care assistant community.", registryWidget);
    labelRegistryIntro->setStyleSheet("font-size: 20px; color: gray; min-width: 300px;");
    labelRegistryIntro->setAlignment(Qt::AlignCenter);
    labelRegistryIntro->setWordWrap(true);
    labelRegistryIntro->setMaximumWidth(300);

    lineEditRegistryUsername = new QLineEdit(registryWidget);
    lineEditRegistryUsername->setPlaceholderText("请输入用户名");
    lineEditRegistryUsername->setMaximumWidth(300);
    lineEditRegistryUsername->addAction(iconUsername, QLineEdit::LeadingPosition);

    lineEditRegistryPassword = new QLineEdit(registryWidget);
    lineEditRegistryPassword->setPlaceholderText("请输入密码");
    lineEditRegistryPassword->setMaximumWidth(300);
    lineEditRegistryPassword->addAction(iconPassword, QLineEdit::LeadingPosition);
    lineEditRegistryPassword->setEchoMode(QLineEdit::Password);
    lineEditRegistryPassword->setMaxLength(16);

    lineEditRegistryConfirmPassword = new QLineEdit(registryWidget);
    lineEditRegistryConfirmPassword->setPlaceholderText("请确认密码");
    lineEditRegistryConfirmPassword->setMaximumWidth(300);
    lineEditRegistryConfirmPassword->addAction(iconPassword, QLineEdit::LeadingPosition);
    lineEditRegistryConfirmPassword->setEchoMode(QLineEdit::Password);
    lineEditRegistryConfirmPassword->setMaxLength(16);
    
    actionShowPasswordRegistry = lineEditRegistryConfirmPassword->addAction(iconInvisiblePassword, QLineEdit::TrailingPosition);
    connect(actionShowPasswordRegistry, &QAction::triggered, [this]() {
        if (lineEditRegistryConfirmPassword->echoMode() == QLineEdit::Password) {
            lineEditRegistryConfirmPassword->setEchoMode(QLineEdit::Normal);
            lineEditRegistryPassword->setEchoMode(QLineEdit::Normal);
            actionShowPasswordRegistry->setIcon(iconVisiblePassword);
        } else {
            lineEditRegistryConfirmPassword->setEchoMode(QLineEdit::Password);
            lineEditRegistryPassword->setEchoMode(QLineEdit::Password);
            actionShowPasswordRegistry->setIcon(iconInvisiblePassword);
        }
    });


    buttonRegistryConfirm = new QPushButton("注册", registryWidget);
    addButtonAnimation(buttonRegistryConfirm);
    buttonRegistryConfirm->setFixedWidth(300);
    connect(buttonRegistryConfirm, &QPushButton::clicked, this, &Login::buttonClickedRegistry);

    buttonRegistryGoBack = new QPushButton("返回登录", registryWidget);
    buttonRegistryGoBack->setIcon(iconGoBack);
    connect(buttonRegistryGoBack, &QPushButton::clicked, [this]() {
        stackedWidgetLoginRegistry->setCurrentIndex(0);
    });


    QHBoxLayout * labelRegistryLayout = new QHBoxLayout;
    labelRegistryLayout->addSpacing(250);
    labelRegistryLayout->addWidget(labelRegistry);

    QHBoxLayout * labelRegistryIntroLayout = new QHBoxLayout;
    labelRegistryIntroLayout->addSpacing(250);
    labelRegistryIntroLayout->addWidget(labelRegistryIntro);

    QHBoxLayout * lineEditRegistryUsernameLayout = new QHBoxLayout;
    lineEditRegistryUsernameLayout->addSpacing(250);
    lineEditRegistryUsernameLayout->addWidget(lineEditRegistryUsername);

    QHBoxLayout * lineEditRegistryPasswordLayout = new QHBoxLayout;
    lineEditRegistryPasswordLayout->addSpacing(250);
    lineEditRegistryPasswordLayout->addWidget(lineEditRegistryPassword);

    QHBoxLayout * lineEditRegistryConfirmPasswordLayout = new QHBoxLayout;
    lineEditRegistryConfirmPasswordLayout->addSpacing(250);
    lineEditRegistryConfirmPasswordLayout->addWidget(lineEditRegistryConfirmPassword);

    QHBoxLayout * buttonRegistryConfirmLayout = new QHBoxLayout;
    buttonRegistryConfirmLayout->addSpacing(250);
    buttonRegistryConfirmLayout->addWidget(buttonRegistryConfirm);

    
    vRegistryLayout->addWidget(buttonRegistryGoBack);
    vRegistryLayout->addStretch();
    vRegistryLayout->addLayout(labelRegistryLayout);
    vRegistryLayout->addSpacing(20);
    vRegistryLayout->addLayout(labelRegistryIntroLayout);
    vRegistryLayout->addSpacing(20);
    vRegistryLayout->addLayout(lineEditRegistryUsernameLayout);
    vRegistryLayout->addSpacing(10);
    vRegistryLayout->addLayout(lineEditRegistryPasswordLayout);
    vRegistryLayout->addSpacing(10);
    vRegistryLayout->addLayout(lineEditRegistryConfirmPasswordLayout);
    vRegistryLayout->addSpacing(10);
    vRegistryLayout->addLayout(buttonRegistryConfirmLayout);
    vRegistryLayout->addStretch();
    registryWidget->setLayout(vRegistryLayout);

    return registryWidget;
}




void Login::addButtonAnimation(class QPushButton * button) {
    // 创建缩放动画
    auto *animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(10);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    QObject::connect(button, &QPushButton::pressed, [=]() {
        animation->setStartValue(button->geometry());
        animation->setEndValue(button->geometry().adjusted(2, 2, -2, -2));
        animation->start();
    });

    QObject::connect(button, &QPushButton::released, [=]() {
        animation->setStartValue(button->geometry());
        animation->setEndValue(button->geometry().adjusted(-2, -2, 2, 2));
        animation->start();
    });
}

// 加密密码
QString Login::encryptPassword(const QString & password) {
    QByteArray byteArray = password.toUtf8();

    QByteArray encryptedArray = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);

    return encryptedArray.toHex();
}


// 登录
void Login::buttonClickedLogin() {
    if (loginSuccess) return;

    // QString username = lineEditUsername->text();
    // QString password = lineEditPassword->text();

    // if (username == "" || password == "") {
    //     PMessagebox::critical(this, "错误", "用户名或密码不能为空", PMessagebox::ButtonType::OK_CANCEL);
    //     return;
    // }

    // QSqlQuery query;

    // query.prepare("SELECT username, created_at, imageUrl FROM users WHERE username = :username AND password_hash = :password");
    // query.bindValue(":username", username);
    // query.bindValue(":password", encryptPassword(password));
    // query.exec();

    // if (!query.next()) {
    //     PMessagebox::information(this, "错误", "用户名或密码错误", PMessagebox::ButtonType::OK);
    //     lineEditPassword->setFocus();
    //     lineEditPassword->selectAll();
    //     return;
    // }

    // QString name = query.value("username").toString();
    // QString createdTime = query.value("created_at").toString();
    // QString imageUrl = query.value("imageUrl").toString();

    

    QString name = "test";
    QString createdTime = "2025-01-01";
    QString imageUrl = "./res/images/default-plant.png";

    manager->setUser(new User(name, createdTime, imageUrl));

    PlantAssistant * plantAssistant = new PlantAssistant(nullptr, manager);
    manager->setPlantAssistant(plantAssistant);
    manager->showPlantAssistant(); 

    loginSuccess = true;

    this->close();
}

// 注册
void Login::buttonClickedRegistry() {
    QString username = lineEditRegistryUsername->text();
    QString password = lineEditRegistryPassword->text();
    QString confirmPassword = lineEditRegistryConfirmPassword->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        PMessagebox::information(this, "错误", "用户名或密码不能为空", PMessagebox::ButtonType::OK);
        return;
    }

    if (password != confirmPassword) {
        PMessagebox::information(this, "错误", "两次输入的密码不一致", PMessagebox::ButtonType::OK);
        lineEditRegistryConfirmPassword->setFocus();
        lineEditRegistryConfirmPassword->selectAll();
        return;
    }

    QString encryptedPassword = encryptPassword(password);

    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        PMessagebox::information(this, "错误", "用户名已存在", PMessagebox::ButtonType::OK);
        lineEditRegistryUsername->clear();
        return;
    }

    query.prepare("INSERT INTO users (username, password_hash) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", encryptedPassword);
    if (query.exec()) {
        PopUpDialog::information(this, "注册成功");
        lineEditRegistryUsername->clear();
        lineEditRegistryPassword->clear();
        lineEditRegistryConfirmPassword->clear();
    } else {
        PMessagebox::information(this, "错误", "注册失败", PMessagebox::ButtonType::OK);
    }

    
}

void Login::closeEvent(QCloseEvent * event) {
    static bool firstClose = true; 

    if (firstClose) {
        event->ignore();

        // 创建并行动画组
        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

        // 透明度动画
        QPropertyAnimation *opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
        opacityAnimation->setDuration(300);
        opacityAnimation->setStartValue(1);
        opacityAnimation->setEndValue(0);
        group->addAnimation(opacityAnimation);

        // 位置移动动画
        if (loginSuccess) {
            QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos");
            posAnimation->setDuration(300);
            posAnimation->setStartValue(pos());
            posAnimation->setEndValue(QPoint(pos().x() - 400, pos().y())); // 向左移动400像素

            group->addAnimation(posAnimation);
        }
        
        
        connect(group, &QParallelAnimationGroup::finished, this, [this]() {
            this->close();
        });

        group->start(QAbstractAnimation::DeleteWhenStopped);
        firstClose = false;
    } else {
        QMainWindow::closeEvent(event);
    }
}



Login::~Login()
{
    delete ui;
}
