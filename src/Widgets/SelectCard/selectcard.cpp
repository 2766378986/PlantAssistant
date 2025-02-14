#include "selectcard.h"
#include "ui_selectcard.h"
#include <QFile>
#include <QPixmap>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include "selectcardmanager.h"
#include <QStyle>
#include <QPropertyAnimation>
#include <QAbstractAnimation>

SelectCard::SelectCard(QWidget *parent, class SelectCardManager * selectCardManager, int index)
    : QWidget(parent)
    , ui(new Ui::SelectCard)
    , selectCardManager(selectCardManager)
    , selected(false)
{
    ui->setupUi(this);

    setObjectName("SelectCard");

    setAttribute(Qt::WA_StyledBackground, true);

    labelImage = new QLabel(this);
    labelImage->setAlignment(Qt::AlignCenter);
    labelImage->setObjectName("SelectCardImage");

    labelMessage = new QLabel(this);
    labelMessage->setAlignment(Qt::AlignCenter);
    labelMessage->setObjectName("SelectCardMessage");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(labelImage);
    layout->addWidget(labelMessage);
    setLayout(layout);


    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15); // 阴影模糊半径
    shadowEffect->setColor(QColor(0, 0, 0, 100)); // 阴影颜色和透明度
    shadowEffect->setOffset(3, 3); // 阴影偏移量
    setGraphicsEffect(shadowEffect);

    connect(this, &SelectCard::selectedChanged, selectCardManager, &SelectCardManager::onCardSelected);

    addStyle();
}

void SelectCard::addStyle() {
    QFile file("./res/QSS/SelectCard.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        setStyleSheet(styleSheet);
        file.close();
    }
}


void SelectCard::setInfo(const QString & imagePath, const QString & message) {
    QPixmap pixmap(imagePath);    
    labelImage->setPixmap(pixmap);
    labelMessage->setText(message);
}

QString SelectCard::getMessage() const {
    return labelMessage->text();
}

bool SelectCard::getSelected() const {
    return selected;
}

void SelectCard::setSelected(bool selected) {
    this->selected = selected;
    style()->polish(this);
    emit selectedChanged(this);
    
}


void SelectCard::mousePressEvent(QMouseEvent * event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }


    setSelected(!selected);
    QWidget::mousePressEvent(event);
}

int SelectCard::getIndex() const {
    return index;
}

void SelectCard::setIndex(int index) {
    this->index = index;
}  

void SelectCard::setImageSize(int width, int height) {
    QPixmap pixmap = labelImage->pixmap();
    pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    labelImage->setPixmap(pixmap);
}


void SelectCard::setSize(int width, int height) {
    setFixedSize(width, height);
}

QString SelectCard::getData() const {
    return data;
}

void SelectCard::setData(const QString & data) {
    this->data = data;
}


void SelectCard::showEvent(QShowEvent * event) {
    normalGeometry = geometry();
    QWidget::showEvent(event);
}

// 在 SelectCard 中添加悬停动画
void SelectCard::enterEvent(QEnterEvent *event) {
    QPropertyAnimation *anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(150);
    anim->setStartValue(geometry());
    anim->setEndValue(normalGeometry.adjusted(-2, -2, 2, 2));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    QWidget::enterEvent(event);
}

void SelectCard::leaveEvent(QEvent *event) {
    QPropertyAnimation *anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(150);
    anim->setStartValue(geometry());
    anim->setEndValue(normalGeometry);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    QWidget::leaveEvent(event);
}

SelectCard::~SelectCard()
{
    delete ui;
}
