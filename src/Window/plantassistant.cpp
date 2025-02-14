#include "plantassistant.h"
#include "./ui_plantassistant.h"
#include "../Widgets/PlantInformationManager/plantinformationmanage.h"
#include "../Widgets/CareRemind/careremind.h"
#include "../Manager/Manager.h"
#include "../Dialogs/MessageBox/pmessagebox.h"
#include "../CoreData/User/User.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QSplitter>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>
#include <QQuickWidget>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QApplication>
#include <QRect>
#include <QParallelAnimationGroup>

PlantAssistant::PlantAssistant(QWidget *parent, Manager * manager)
    : QMainWindow(parent)
    , ui(new Ui::PlantAssistant)
    , manager(manager)
{
    ui->setupUi(this);

    setObjectName("plantAssistant");

    setFixedSize(1400, 800);

    addSource();
    
    addStyle();

    initUI();

    navigationList->setCurrentRow(1);
}


void PlantAssistant::addStyle() {
    QFile file("./res/QSS/plantAssistant.qss");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        setStyleSheet(stream.readAll());
    }
}


void PlantAssistant::addSource() {

    iconLogo.addFile("./res/icons/logo.png");

}

void PlantAssistant::initUI() {
    setWindowIcon(iconLogo);

    navigationList = new QListWidget();
    navigationList->setObjectName("navigationList");
    navigationList->setIconSize(QSize(32, 32));


    QListWidgetItem * plantInfoItem = new QListWidgetItem(QIcon("./res/icons/info-manage.png"),"植物信息");
    QListWidgetItem * careRemindItem = new QListWidgetItem(QIcon("./res/icons/care-remind.png"),"养护提醒");
    QListWidgetItem * growthLogItem = new QListWidgetItem(QIcon("./res/icons/growth-log.png"),"生长日志");
    QListWidgetItem * dataStatisticItem = new QListWidgetItem(QIcon("./res/icons/data-statistic.png"),"数据统计");

    QListWidgetItem * healthScoreItem = new QListWidgetItem(QIcon("./res/icons/health-score.png"),"健康评分");
    QListWidgetItem * personalizedPlanItem = new QListWidgetItem(QIcon("./res/icons/personalized-plan.png"),"养护计划");
    QListWidgetItem * plantGrowthReportItem = new QListWidgetItem(QIcon("./res/icons/plant-growth-report.png"),"生长报告");


    navigationList->addItem(plantInfoItem);
    navigationList->addItem(careRemindItem);
    navigationList->addItem(growthLogItem);
    navigationList->addItem(dataStatisticItem);
    navigationList->addItem(healthScoreItem);
    navigationList->addItem(personalizedPlanItem);
    navigationList->addItem(plantGrowthReportItem);


    navigationList->setMaximumWidth(200);
    navigationList->setFocusPolicy(Qt::NoFocus);
    navigationList->setCurrentRow(0);

    // 创建右侧显示区域
    contentStack = new QStackedWidget(this);
    contentStack->setObjectName("contentStack");

    // 添加多个页面（示例页面）
    plantInformationManage = new PlantInformationManage(contentStack, manager);
    contentStack->addWidget(plantInformationManage);
    manager->setPlantInformationManage(plantInformationManage);

    careRemind = new CareRemind(contentStack, manager);
    contentStack->addWidget(careRemind);
    manager->setCareRemind(careRemind);

    contentStack->addWidget(new QLabel("生长日志页面"));
    contentStack->addWidget(new QLabel("数据统计页面"));
    contentStack->addWidget(new QLabel("健康评分页面"));
    contentStack->addWidget(new QLabel("个性化养护计划页面"));
    contentStack->addWidget(new QLabel("植物生长报告页面"));

    QWidget *leftWidget = new QWidget(this);
    leftWidget->setMaximumWidth(200);

    navigationList->setParent(leftWidget);
    QWidget *userInfoWidget = CreateUserInfo(leftWidget);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(navigationList);
    vlayout->addWidget(userInfoWidget);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);

    leftWidget->setLayout(vlayout);


    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(leftWidget);
    hlayout->addWidget(contentStack);
    hlayout->setContentsMargins(0, 0, 0, 0);

    mainWidget = new QWidget(this);
    mainWidget->setLayout(hlayout);
    setCentralWidget(mainWidget);


    // 连接导航栏的点击信号和页面切换信号槽

    connect(navigationList, &QListWidget::currentRowChanged, contentStack, &QStackedWidget::setCurrentIndex);
}


QWidget * PlantAssistant::CreateUserInfo(QWidget * parent) {
    userWidget = new QWidget(parent);
    userWidget->setObjectName("userWidget");

    QVBoxLayout * vUserLayout = new QVBoxLayout;
    QHBoxLayout * hUserLayout = new QHBoxLayout;


    // 用户头像
    QString userHeadPath = manager->getUser()->getImageUrl();
    headLabel = new QLabel(userWidget);
    headLabel->setObjectName("headLabel");
    
    QPixmap pixmap(userHeadPath);

    if (pixmap.isNull()) {
        pixmap.load("./res/image/head.jpg");
    }

    pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);


    // 裁剪为圆角
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(0, 0, pixmap.width(), pixmap.height(), 25, 25);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pixmap);


    // 设置裁剪后的图像
    headLabel->setPixmap(roundedPixmap);
    headLabel->setAlignment(Qt::AlignCenter);




    QVBoxLayout * userInfoLayout = new QVBoxLayout;

    // 用户名和信息
    nameLabel = new QLabel(manager->getUser()->getName(), userWidget);
    nameLabel->setObjectName("nameLabel");


    infoLabel = new QLabel("在线", userWidget);
    infoLabel->setObjectName("infoLabel");


    userInfoLayout->addWidget(nameLabel);
    userInfoLayout->addWidget(infoLabel);


    hUserLayout->addWidget(headLabel);
    hUserLayout->addLayout(userInfoLayout);
    

    QPushButton * exit = new QPushButton("退出", userWidget);
    exit->setStyleSheet("background-color: transparent; border: none;");
    exit->setFixedSize(100, 40);

    vUserLayout->addLayout(hUserLayout);
    vUserLayout->addWidget(exit);
    vUserLayout->setAlignment(Qt::AlignLeft);

    userWidget->setLayout(vUserLayout);


    connect(exit, &QPushButton::clicked, this, &PlantAssistant::on_exit_clicked);
    
    return userWidget;
}

void PlantAssistant::setManager(Manager * manager) {
    this->manager = manager;
}


void PlantAssistant::on_exit_clicked() {
    // TODO: 保存数据或者做一些退出前的处理
    PMessagebox::information(this, "退出", "退出成功");
    close();

}

void PlantAssistant::showEvent(QShowEvent * event) {
    QMainWindow::showEvent(event);

    // 获取屏幕尺寸
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    
    // 设置初始位置（屏幕右侧外）
    int startX = x() + 400;
    move(startX, y());


    // 创建并行动画组
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    // 透明度动画
    QPropertyAnimation *opacityAnim = new QPropertyAnimation(this, "windowOpacity");
    opacityAnim->setDuration(300);
    opacityAnim->setStartValue(0);
    opacityAnim->setEndValue(1);

    // 位置动画（从右侧滑入）
    QPropertyAnimation *posAnim = new QPropertyAnimation(this, "pos");
    posAnim->setDuration(300);
    posAnim->setStartValue(QPoint(startX, y()));
    posAnim->setEndValue(QPoint(startX - 400, y())); // 向左移动400px


    group->addAnimation(opacityAnim);
    group->addAnimation(posAnim);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}


PlantAssistant::~PlantAssistant()
{
    delete ui;
}
