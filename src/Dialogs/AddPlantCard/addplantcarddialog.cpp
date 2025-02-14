#include "addplantcarddialog.h"
#include "ui_addplantcarddialog.h"
#include "../../Manager/Manager.h"
#include "../../CoreData/User/User.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QPropertyAnimation>
#include <QShowEvent>
#include <QCloseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QGraphicsDropShadowEffect>

AddPlantCardDialog::AddPlantCardDialog(QWidget *parent, Manager * manager, DialogType dialogType, Plant plant) : QDialog(parent),
                        ui(new Ui::AddPlantCardDialog), 
                        plant(plant),
                        manager(manager),
                        dialogType(dialogType) {

    ui->setupUi(this);
    setupUI();
    setObjectName("AddPlantCardDialog");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);


    if (dialogType == DialogType::EDIT || dialogType == DialogType::SHOW) {
        setupInfo();
    }

    

    addStyle();
}

void AddPlantCardDialog::addStyle() {
    QFile file("./res/QSS/addPlantCard.qss");
    if (file.open(QFile::ReadOnly)) {
        QTextStream textStream(&file);
        setStyleSheet(textStream.readAll());
        file.close();
    }
}

void AddPlantCardDialog::setupUI() {
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // 卡片容器（实现圆角和阴影）
    QWidget *cardWidget = new QWidget(this);
    cardWidget->setObjectName("cardWidget");
    QVBoxLayout *cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(15);

    // --- 第一部分：名称、种类、照片 ---
    QHBoxLayout *topLayout = new QHBoxLayout;
    
    // 照片上传区域
    QVBoxLayout *photoLayout = new QVBoxLayout;
    photoLabel = new QLabel(cardWidget);
    photoLabel->setFixedSize(120, 120);
    photoLabel->setAlignment(Qt::AlignCenter);
    uploadButton = new QPushButton("上传照片", cardWidget);
    connect(uploadButton, &QPushButton::clicked, this, &AddPlantCardDialog::onUploadPhoto);
    photoLayout->addWidget(photoLabel);
    photoLayout->addWidget(uploadButton);
    topLayout->addLayout(photoLayout);

    // 名称和种类
    QFormLayout *infoForm = new QFormLayout;
    nameEdit = new QLineEdit(cardWidget);
    speciesCombo = new QComboBox(cardWidget);
    speciesCombo->addItems({"多肉植物", "观叶植物", "开花植物", "水生植物"});
    infoForm->addRow("名称:", nameEdit);
    infoForm->addRow("种类:", speciesCombo);
    topLayout->addLayout(infoForm);
    cardLayout->addLayout(topLayout);

    // --- 第二部分：养护频率 ---
    QGroupBox *frequencyGroup = new QGroupBox("养护频率", cardWidget);
    QFormLayout *frequencyLayout = new QFormLayout(frequencyGroup);
    wateringEdit = new QLineEdit(frequencyGroup);
    fertilizingEdit = new QLineEdit(frequencyGroup);
    pruningEdit = new QLineEdit(frequencyGroup);
    frequencyLayout->addRow("浇水（天）:", wateringEdit);
    frequencyLayout->addRow("施肥（天）:", fertilizingEdit);
    frequencyLayout->addRow("修剪（天）:", pruningEdit);
    cardLayout->addWidget(frequencyGroup);

    // --- 第三部分：环境需求 ---
    QGroupBox *environmentGroup = new QGroupBox("环境需求", cardWidget);
    QFormLayout *environmentLayout = new QFormLayout(environmentGroup);
    lightCombo = new QComboBox(environmentGroup);
    lightCombo->addItems({"全日照", "半阴", "阴凉"});
    temperatureEdit = new QLineEdit(environmentGroup);
    humidityEdit = new QLineEdit(environmentGroup);
    soilCombo = new QComboBox(environmentGroup);
    soilCombo->addItems({"沙质土", "腐殖土", "黏土"});
    environmentLayout->addRow("光照:", lightCombo);
    environmentLayout->addRow("温度（℃）:", temperatureEdit);
    environmentLayout->addRow("湿度（%）:", humidityEdit);
    environmentLayout->addRow("土壤类型:", soilCombo);
    cardLayout->addWidget(environmentGroup);

    // --- 第四部分：日期记录 ---
    QGroupBox *dateGroup = new QGroupBox("日期记录", cardWidget);
    QFormLayout *dateLayout = new QFormLayout(dateGroup);
    dateAddedEdit = new QDateTimeEdit(QDateTime::currentDateTime(), dateGroup);
    lastWateredEdit = new QDateTimeEdit(dateGroup);
    lastFertilizedEdit = new QDateTimeEdit(dateGroup);
    lastPrunedEdit = new QDateTimeEdit(dateGroup);
    dateLayout->addRow("添加日期:", dateAddedEdit);
    dateLayout->addRow("上次浇水:", lastWateredEdit);
    dateLayout->addRow("上次施肥:", lastFertilizedEdit);
    dateLayout->addRow("上次修剪:", lastPrunedEdit);
    cardLayout->addWidget(dateGroup);

    // --- 底部按钮 ---
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    confirmButton = new QPushButton("确认", cardWidget);
    cancelButton = new QPushButton("取消", cardWidget);
    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &AddPlantCardDialog::fadeOut);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(confirmButton);
    cardLayout->addLayout(buttonLayout);

    mainLayout->addWidget(cardWidget);
}

void AddPlantCardDialog::loadPicture(const QString & photoPath) {
    photoLabel->setPixmap(QPixmap(photoPath).scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    photoLabel->update();
}

void AddPlantCardDialog::setupInfo() {
    loadPicture(plant.values[Plant::PLANT_PHOTO]);
    nameEdit->setText(plant.values[Plant::PLANT_NAME]);
    speciesCombo->setCurrentText(plant.values[Plant::PLANT_SPECIES]);
    photoPath_ = plant.values[Plant::PLANT_PHOTO];
    wateringEdit->setText(plant.values[Plant::PLANT_WATERING_RATE]);
    fertilizingEdit->setText(plant.values[Plant::PLANT_FERTILIZING_RATE]);
    pruningEdit->setText(plant.values[Plant::PLANT_PRUNE_RATE]);
    lightCombo->setCurrentText(plant.values[Plant::PLANT_LIGHT_REQUIREMENT]);
    temperatureEdit->setText(plant.values[Plant::PLANT_TEMPERATURE_REQUIREMENT]);
    humidityEdit->setText(plant.values[Plant::PLANT_HUMIDITY_REQUIREMENT]);
    soilCombo->setCurrentText(plant.values[Plant::PLANT_SOIL_TYPE]);
    dateAddedEdit->setDateTime(QDateTime::fromString(plant.values[Plant::DATE_ADDED], "yyyy-MM-dd hh:mm:ss"));
    lastWateredEdit->setDateTime(QDateTime::fromString(plant.values[Plant::LAST_WATERED], "yyyy-MM-dd hh:mm:ss"));
    lastFertilizedEdit->setDateTime(QDateTime::fromString(plant.values[Plant::LAST_FERTILIZED], "yyyy-MM-dd hh:mm:ss"));
    lastPrunedEdit->setDateTime(QDateTime::fromString(plant.values[Plant::LAST_PRUNED], "yyyy-MM-dd hh:mm:ss"));
    
}

void AddPlantCardDialog::onUploadPhoto() {
    photoPath_ = QFileDialog::getOpenFileName(this, "选择植物照片", "", "Images (*.png *.jpg)");

    if (!photoPath_.isEmpty()) {
        loadPicture(photoPath_);
    }
}

QList<QString> AddPlantCardDialog::getPlantData() const {
    QList<QString> data;

    data.append(nameEdit->text());
    data.append(speciesCombo->currentText());
    data.append(photoPath_);
    data.append(wateringEdit->text());
    data.append(fertilizingEdit->text());
    data.append(pruningEdit->text());
    data.append(lightCombo->currentText());
    data.append(temperatureEdit->text());
    data.append(humidityEdit->text());
    data.append(soilCombo->currentText());
    data.append(dateAddedEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    data.append(lastWateredEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    data.append(lastFertilizedEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    data.append(lastPrunedEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"));    
    data.append(manager->getUser()->getName());

    return data;
}

void AddPlantCardDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);

    // 打开动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void AddPlantCardDialog::fadeOut() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, &QPropertyAnimation::finished, this, &QDialog::close);
}

void AddPlantCardDialog::setUniqueShow(bool uniqueShow) {
    if (uniqueShow) {
        nameEdit->setReadOnly(true);
        speciesCombo->setEnabled(false);
        wateringEdit->setEnabled(false);
        fertilizingEdit->setEnabled(false);
        pruningEdit->setEnabled(false);
        lightCombo->setEnabled(false);
        temperatureEdit->setEnabled(false);
        humidityEdit->setEnabled(false);
        soilCombo->setEnabled(false);
        dateAddedEdit->setEnabled(false);
        lastWateredEdit->setEnabled(false);
        lastFertilizedEdit->setEnabled(false);
        lastPrunedEdit->setEnabled(false);
        uploadButton->setHidden(true);
        confirmButton->setHidden(true);
    }
}


AddPlantCardDialog::~AddPlantCardDialog() {
    delete ui;
}