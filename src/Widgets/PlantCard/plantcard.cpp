#include "plantcard.h"
#include "ui_plantcard.h"
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QFile>
#include <QStyle>
#include "../../Manager/Manager.h"
#include "../../Dialogs/AddPlantCard/addplantcarddialog.h"
#include "../../Widgets/PlantInformationManager/plantinformationmanage.h"

PlantCard::PlantCard(QWidget *parent, class Manager * manager)
    : QWidget(parent)
    , ui(new Ui::PlantCard)
    , manager(manager)
    , plantPhoto(new QLabel(this))
    , plantName(new QLabel(this))
    , plantSpecies(new QLabel(this))
    , plantWateringRate(new QLabel(this))
    , plantLightRequirement(new QLabel(this))
    , plantTemperatureRequirement(new QLabel(this))
    , plantLastWatered(new QLabel(this))
    , plantEditButton(new QPushButton("编辑", this))
    , plantDeleteButton(new QPushButton("删除", this))
    , plantWaterIcon(new QLabel(this))
    , plantLightIcon(new QLabel(this))
    , plantTemperatureIcon(new QLabel(this))
    , selected(false)
{
    ui->setupUi(this);

    setFixedSize(280, 380);

    
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StyledBackground, true);

    setObjectName("plantCard");


    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15); // 阴影模糊半径
    shadowEffect->setColor(QColor(0, 0, 0, 100)); // 阴影颜色和透明度
    shadowEffect->setOffset(3, 3); // 阴影偏移量
    setGraphicsEffect(shadowEffect);


    initUI();

    addStyle();


    
}

void PlantCard::addSource() {
    QPixmap pixmap(plant.values[Plant::PLANT_PHOTO]);

    if (pixmap.isNull()) {
        pixmap.load("./res/images/default-plant.png");
    }

    // 裁剪为圆角
    // 确保图像大小为 200x200
    pixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 裁剪为圆角
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(0, 0, pixmap.width(), pixmap.height(), 10, 10);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pixmap);

    // 设置裁剪后的图像
    plantPhoto->setPixmap(roundedPixmap);
    plantPhoto->setAlignment(Qt::AlignCenter);
    
}

void PlantCard::addStyle() {
    QFile file("./res/QSS/plantCard.qss");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        setStyleSheet(stream.readAll());
    }

}

void PlantCard::initUI() {

    // 设置布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10); // 设置间距
    mainLayout->setContentsMargins(15, 15, 15, 15); // 设置边距

    plantName->setAlignment(Qt::AlignCenter);
    plantSpecies->setAlignment(Qt::AlignCenter);
    plantName->setObjectName("plantName"); // 设置特殊样式

    plantPhoto->setFixedSize(200, 200);

    mainLayout->addWidget(plantPhoto, 0, Qt::AlignCenter);
    mainLayout->addWidget(plantName);
    mainLayout->addWidget(plantSpecies);


    plantWateringRate->setAlignment(Qt::AlignCenter);
    plantLightRequirement->setAlignment(Qt::AlignCenter);
    plantTemperatureRequirement->setAlignment(Qt::AlignCenter);

    plantWaterIcon->setPixmap(QPixmap("./res/icons/water.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    plantLightIcon->setPixmap(QPixmap("./res/icons/sun.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    plantTemperatureIcon->setPixmap(QPixmap("./res/icons/temperature.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    plantWaterIcon->setAlignment(Qt::AlignCenter);
    plantLightIcon->setAlignment(Qt::AlignCenter);
    plantTemperatureIcon->setAlignment(Qt::AlignCenter);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    centerLayout->addWidget(plantWaterIcon);
    centerLayout->addWidget(plantWateringRate);
    centerLayout->addStretch();
    centerLayout->addWidget(plantLightIcon);
    centerLayout->addWidget(plantLightRequirement);
    centerLayout->addStretch();
    centerLayout->addWidget(plantTemperatureIcon);
    centerLayout->addWidget(plantTemperatureRequirement);
    centerLayout->addStretch();
    mainLayout->addLayout(centerLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(plantEditButton);
    buttonLayout->addWidget(plantDeleteButton);
    mainLayout->addLayout(buttonLayout);



    connect(plantEditButton, &QPushButton::clicked, this, &PlantCard::editPlant);
    connect(plantDeleteButton, &QPushButton::clicked, this, &PlantCard::deletePlant);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(plantLastWatered);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    // 只有管理界面才可以编辑和删除
    if (dynamic_cast<class PlantInformationManage *>(parentWidget()) == nullptr) {
        plantEditButton->hide();
        plantDeleteButton->hide();
    }

}


void PlantCard::setPlant(const Plant & plant, int indexOfPlantArray) {
    this->plant = plant;
    this->indexOfPlantArray = indexOfPlantArray;

    addSource();

    addPlantToUI();
}

void PlantCard::addPlantToUI() {
    plantName->setText(plant.values[Plant::PLANT_NAME]);
    plantSpecies->setText(plant.values[Plant::PLANT_SPECIES]);
    plantWateringRate->setText("每" + plant.values[Plant::PLANT_WATERING_RATE] + "天");
    plantLightRequirement->setText(plant.values[Plant::PLANT_LIGHT_REQUIREMENT]);
    plantTemperatureRequirement->setText(plant.values[Plant::PLANT_TEMPERATURE_REQUIREMENT]);
    plantLastWatered->setText("上次浇水: " + plant.values[Plant::LAST_WATERED]);
}

void PlantCard::setHide(bool hide) {
    if (hide) {
        this->hide();
    } else {
        this->show();
    }
}

int PlantCard::getIndexOfPlantArray() const {
    return indexOfPlantArray;
}

void PlantCard::editPlant() {
    class PlantInformationManage * plantInformationManage = manager->getPlantInformationManage();
    plantInformationManage->editPlant(plant, indexOfPlantArray);

}

void PlantCard::deletePlant() {
    class PlantInformationManage * plantInformationManage = manager->getPlantInformationManage();
    plantInformationManage->deletePlant(plant, indexOfPlantArray);
}

void PlantCard::setSelected(bool selected) {
    this->selected = selected;
    style()->polish(this);
}

bool PlantCard::getSelected() const {
    return selected;
}


void PlantCard::mousePressEvent(QMouseEvent * event) {
    if (!manager->getSelectionMode() || event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    

    if (selected) {
        setSelected(false);
        manager->getSelectedPlantIndex().removeOne(indexOfPlantArray);
    } else {
        setSelected(true);
        manager->getSelectedPlantIndex().append(indexOfPlantArray);
    }

}


void PlantCard::setSelectionMode(bool selectionMode) {
    this->selectionMode = selectionMode;
}

bool PlantCard::getSelectionMode() const {
    return selectionMode;
}

PlantCard::~PlantCard()
{
    delete ui;
}
