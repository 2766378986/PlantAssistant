#include "pmessagebox.h"
#include "ui_pmessagebox.h"
#include <QLabel>
#include <QPushButton>
#include <qboxlayout.h>
#include <QFile>
#include <QPropertyAnimation>

PMessagebox::PMessagebox(QWidget *parent, const QString & title, const QString & message, ButtonType buttonType, MessageType messageType)
    : QDialog(parent)
    , ui(new Ui::PMessagebox)
{
    // ui->setupUi(this);

    setWindowTitle(title);
    // setFixedSize(320, 100);

    labelMessage = new QLabel(this);
    labelMessage->setText(message);
    labelMessage->setAlignment(Qt::AlignCenter);
    labelMessage->setFixedHeight(20);
    labelMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    labelIcon = new QLabel(this);

    if (messageType == INFORMATION) {
        iconTip = QIcon("./res/icons/information.png");
    } else if (messageType == CRITICAL) {
        iconTip = QIcon("./res/icons/critical.png");
    }
    labelIcon->setPixmap(iconTip.pixmap(20, 20));
    labelIcon->setAlignment(Qt::AlignTop);

    if (buttonType == OK) {
        initUIOk();

    } else if (buttonType == OK_CANCEL) {
        initUI_Ok_Cancel();
    }

    QFile file("./res/QSS/messagebox.qss");
    file.open(QFile::ReadOnly);

    QTextStream stream(&file);
    QString styleSheet = stream.readAll();
    setStyleSheet(styleSheet);
}

void PMessagebox::initUIOk() {
    // 总布局纵向布局
    QVBoxLayout * layout = new QVBoxLayout;

    // 确定按钮
    buttonOk = new QPushButton("确定", this);

    // 添加图标和消息， 图标和消息横向布局
    QHBoxLayout * labelLayout = new QHBoxLayout;
    labelLayout->addWidget(labelIcon);
    labelLayout->addWidget(labelMessage);
    labelLayout->addStretch();

    // 将图标和消息添加总布局
    layout->addLayout(labelLayout);

    // 添加按钮， 按钮横向布局
    QHBoxLayout * buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonOk);

    // 将按钮添加总布局
    layout->addLayout(buttonLayout);
    setLayout(layout);

    // 为按钮添加点击时动画
    addButtonAnimation(buttonOk);

    // 为按钮添加点击事件
    connect(buttonOk, &QPushButton::clicked, this, &PMessagebox::accept);
}

void PMessagebox::initUI_Ok_Cancel() {
    // 总布局纵向布局
    QVBoxLayout * layout = new QVBoxLayout;

    // 确定按钮
    buttonOk = new QPushButton("确定", this);
    // 取消按钮
    buttonCancel = new QPushButton("取消", this);

    // 添加图标和消息， 图标和消息横向布局
    QHBoxLayout * labelLayout = new QHBoxLayout;
    labelLayout->addWidget(labelIcon);
    labelLayout->addWidget(labelMessage);
    labelLayout->addStretch();

    // 将图标和消息添加总布局
    layout->addLayout(labelLayout);

    // 添加按钮， 按钮横向布局
    QHBoxLayout * buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonOk);
    buttonLayout->addWidget(buttonCancel);

    // 将按钮添加总布局
    layout->addLayout(buttonLayout);
    setLayout(layout);

    // 为按钮添加点击时动画
    addButtonAnimation(buttonOk);
    addButtonAnimation(buttonCancel);

    // 为按钮添加点击事件
    connect(buttonOk, &QPushButton::clicked, this, &PMessagebox::accept);
    connect(buttonCancel, &QPushButton::clicked, this, &PMessagebox::reject);
}


PMessagebox::Result PMessagebox::information(QWidget * parent, const QString & title, const QString & message, ButtonType buttonType) {
    PMessagebox * messagebox = new PMessagebox(parent, title, message, buttonType, INFORMATION);
    Result result;
    if (messagebox->exec() == QDialog::Accepted) {
        result = RESULT_OK;
    } else {
        result = RESULT_CANCEL;
    }
    if (parent == nullptr ) delete messagebox;
    return result;
}

PMessagebox::Result PMessagebox::critical(QWidget * parent, const QString & title, const QString & message, ButtonType buttonType) {
    PMessagebox * messagebox = new PMessagebox(parent, title, message, buttonType, CRITICAL);
    Result result;
    
    if (messagebox->exec() == QDialog::Accepted) {
        result = RESULT_OK;
    } else {
        result = RESULT_CANCEL;
    }
    if (parent == nullptr ) delete messagebox;
    return result;
}

void PMessagebox::addButtonAnimation(class QPushButton * button) {
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

PMessagebox::~PMessagebox()
{
    delete ui;
}
