#include "popupdialog.h"
#include "ui_popupdialog.h"
#include <QPainter>
#include <QPainterPath>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QPoint>
#include <QScreen>
#include <QFile>
#include <QGraphicsDropShadowEffect>

int PopUpDialog::width_ = 240;
int PopUpDialog::height_ = 140;

QList<PopUpDialog*> PopUpDialog::activeDialogs = {};
const int PopUpDialog::SPACING = 10;

PopUpDialog::PopUpDialog(QWidget *parent, const QString & message, int timeOut)
    : QDialog(parent)
    , ui(new Ui::PopUpDialog)
    , timeOut(timeOut)
{
    ui->setupUi(this);

    setMinimumHeight(height_);
    setMaximumWidth(width_);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    setObjectName("popupDialog");

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15); // 阴影模糊半径
    shadowEffect->setColor(QColor(0, 0, 0, 100)); // 阴影颜色和透明度
    shadowEffect->setOffset(3, 3); // 阴影偏移量
    setGraphicsEffect(shadowEffect);



    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    QWidget * cardWidget = new QWidget(this);
    cardWidget->setObjectName("cardWidget");

    QVBoxLayout * cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setSpacing(15);

    QPixmap pixmap("./res/icons/logo.png");
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

    labelIcon = new QLabel(cardWidget);
    labelIcon->setPixmap(roundedPixmap);
    labelIcon->setAlignment(Qt::AlignCenter);

    labelMessage = new QLabel(cardWidget);
    labelMessage->setText(message);
    labelMessage->setAlignment(Qt::AlignCenter);
    labelMessage->setWordWrap(true);
    labelMessage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    labelMessage->setMinimumWidth(width_ - 40); // 留出边距

    // 动态调整高度
    int textHeight = labelMessage->fontMetrics().boundingRect(QRect(0, 0, width_ - 40, 0), 
                        Qt::TextWordWrap, message).height();
    int newHeight = qMin(textHeight + 120, 400); // 最大高度400px
    setFixedHeight(newHeight);


    cardLayout->addWidget(labelIcon);
    cardLayout->addWidget(labelMessage);
    cardWidget->setLayout(cardLayout);

    mainLayout->addWidget(cardWidget);
    setLayout(mainLayout);

 
    QTimer::singleShot(timeOut, this, &PopUpDialog::fadeOut);

    addStyle();

}


void PopUpDialog::addStyle() {
    QFile file("./res/QSS/popupDialog.qss");
    file.open(QFile::ReadOnly);
    QTextStream textStream(&file);
    setStyleSheet(textStream.readAll());
    file.close();
}

void PopUpDialog::showEvent(QShowEvent * event) {
    QDialog::showEvent(event);

    QPoint basePos;
    int referenceHeight;
    QWidget* parentWindow = parentWidget();
    auto mainwindow = QApplication::activeWindow();

    if (parentWindow) {
        // 基于父窗口计算位置
        QRect parentRect = mainwindow->geometry();
        basePos = QPoint(parentRect.right() - width_ - SPACING, parentRect.bottom());
        referenceHeight = parentRect.height();
    } else {
        // 无父窗口时使用屏幕尺寸
        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        basePos = QPoint(screenGeometry.width() - width_ - SPACING, screenGeometry.height());
        referenceHeight = screenGeometry.height();
    }

    // 计算垂直偏移（仅考虑同父窗口的对话框）
    int verticalOffset = 0;
    for(auto dialog : activeDialogs) {
        if (dialog->parentWidget() == nullptr && parentWindow == nullptr) {
            verticalOffset += dialog->height() + SPACING;
        }
        else if (dialog->parentWidget() != nullptr && parentWindow != nullptr) {
            verticalOffset += dialog->height() + SPACING;
        }
    }

    // 计算最终位置
    QPoint endPos = QPoint(basePos.x(), basePos.y() - height_ - SPACING - verticalOffset);

    // 确保位置在可见范围内
    if(parentWindow) {
        QRect parentGeo = mainwindow->geometry();
        int minY = parentGeo.top() + SPACING;
        if(endPos.y() < minY) {
            endPos.setY(minY);
        }
    }

    // 位置动画
    QPropertyAnimation *posAnim = new QPropertyAnimation(this, "pos");
    posAnim->setDuration(300);
    posAnim->setStartValue(QPoint(basePos.x(), basePos.y())); // 从父窗口底部开始
    posAnim->setEndValue(endPos);

    // 透明度动画保持原样
    QPropertyAnimation *opacityAnim = new QPropertyAnimation(this, "windowOpacity");
    opacityAnim->setDuration(300);
    opacityAnim->setStartValue(0.0);
    opacityAnim->setEndValue(1.0);

    activeDialogs.append(this);

    posAnim->start(QAbstractAnimation::DeleteWhenStopped);
    opacityAnim->start(QAbstractAnimation::DeleteWhenStopped);
}


void PopUpDialog::fadeOut() {
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        QWidget * parentWindow = parentWidget();

        int baseY = y();

        // 找到当前对话框，并删除
        auto it = activeDialogs.begin();
        while(it != activeDialogs.end()) {
            if (*it == this) {
                it = activeDialogs.erase(it);
                break;
            }
            else {
                ++it;
            }
        }

        // 将原本在当前对话框之上的对话框统统往下移动
        for (; it != activeDialogs.end(); ++it) {
            if (parentWindow == nullptr && (*it)->parentWidget() == nullptr) {
                QPoint newPos = QPoint((*it)->x(), baseY);
                baseY = (*it)->y();
                (*it)->move(newPos);
            }
            else if (parentWindow != nullptr && (*it)->parentWidget() != nullptr) {
                QPoint newPos = QPoint((*it)->x(), baseY);
                baseY = (*it)->y();
                (*it)->move(newPos);
            }
        }
        
        
        this->close();
        this->deleteLater();
    });
    
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void PopUpDialog::information(QWidget * parent, const QString & message, int timeOut) {
    PopUpDialog * dialog = new PopUpDialog(parent, message, timeOut);

    if (parent == nullptr) {
        connect(dialog, &PopUpDialog::finished, dialog, &QObject::deleteLater);
    }

    dialog->show();
}


PopUpDialog::~PopUpDialog() {
    delete ui;
}
