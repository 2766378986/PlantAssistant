#include "addremindtask.h"
#include "src/CoreData/Plant/PlantInfo.h"
#include "ui_addremindtask.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QFile>
#include <qnamespace.h>
#include "../../Dialogs/MessageBox/pmessagebox.h"
#include "../../Manager/manager.h"
#include "../../Widgets/SelectCard/selectcardmanager.h"

AddRemindTask::AddRemindTask(QWidget *parent, class Manager * manager)
    : QDialog(parent)
    , ui(new Ui::AddRemindTask)
    , manager(manager)
{
    ui->setupUi(this);

    setFixedSize(810, 600);
    setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout * layout = new QVBoxLayout(this);

    QHBoxLayout * topLayout = new QHBoxLayout();


    scrollAreaPlant = new QScrollArea(this);
    scrollAreaPlant->setWidgetResizable(true);
    scrollAreaPlant->setLayoutDirection(Qt::LeftToRight);
    scrollAreaPlant->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    cardManagerPlant = new SelectCardManager(this, manager, true);
    for (int i = 0; i < manager->getPlants().size(); i++) {
        auto card = new SelectCard(cardManagerPlant, cardManagerPlant, -1);
        card->setData(QString::number(i));

        card->setInfo(manager->getPlants()[i].values[Plant::PLANT_PHOTO], manager->getPlants()[i].values[Plant::PLANT_NAME]);
        card->setImageSize(200, 200);
        card->setSize(250, 250);
        cardManagerPlant->addCard(card);
    }

    scrollAreaPlant->setWidget(cardManagerPlant);

    topLayout->addWidget(scrollAreaPlant);

    // 其他控件初始化

    QHBoxLayout * middleLayout = new QHBoxLayout();



    cardManagerOperation = new SelectCardManager(this, manager, true);

    auto card = new SelectCard(cardManagerOperation, cardManagerOperation);
    card->setInfo("./res/icons/watering.png", "浇水");
    card->setImageSize(50, 50);
    card->setSize(100, 100);
    cardManagerOperation->addCard(card);

    card = new SelectCard(cardManagerOperation, cardManagerOperation);
    card->setInfo("./res/icons/fertilize.png", "施肥");
    card->setImageSize(50, 50);
    card->setSize(100, 100);
    cardManagerOperation->addCard(card);

    card = new SelectCard(cardManagerOperation, cardManagerOperation);
    card->setInfo("./res/icons/pruning.png", "修剪");
    card->setImageSize(50, 50);
    card->setSize(100, 100);
    cardManagerOperation->addCard(card);
    


    QHBoxLayout * bottomLayout = new QHBoxLayout();

    labelRemindTime = new QLabel(this);
    labelRemindTime->setText("提醒时间");

    dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(1), this);
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    dateTimeEdit->setMinimumDate(QDate::currentDate());

    bottomLayout->addWidget(labelRemindTime);
    bottomLayout->addWidget(dateTimeEdit);



    // 按钮
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, 
        this
    );
    
    // 输入验证
    connect(buttonBox, &QDialogButtonBox::accepted, [this]() {
        if (cardManagerPlant->getSelectedIndexes().size() == 0 || cardManagerOperation->getSelectedIndexes().size() == 0) {
            PMessagebox::critical(this, "错误", "必须选择植物和操作！");
            return;
        }
        else if (dateTimeEdit->dateTime() < QDateTime::currentDateTime()) {
            PMessagebox::critical(this, "错误", "提醒时间不能小于当前时间！");
            return;
        }
        accept();
    });

    
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(topLayout);

    layout->addWidget(cardManagerOperation);
    layout->addLayout(bottomLayout);
    layout->addWidget(buttonBox);

    setWindowTitle("添加新提醒");

    QFile file("./res/QSS/addremindertask.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
        file.close();
    }
}


QString AddRemindTask::getSelectedOperation() const {
    return cardManagerOperation->getSelectedMessage();
}

QDateTime AddRemindTask::getSelectedDateTime() const {
    return dateTimeEdit->dateTime();
}

QMap<QString, QString> AddRemindTask::getSelectedPlants() const {
    QMap<QString, QString> selectedPlants;
    const auto & plants = manager->getPlants();

    for (const auto & index : cardManagerPlant->getSelectedIndexes()) {
        int originalIndex = cardManagerPlant->getData(index).toInt();
        QString plantID = plants[originalIndex].values[Plant::PLANT_ID];
        QString plantName = plants[originalIndex].values[Plant::PLANT_NAME];
        selectedPlants[plantID] = plantName;
    }
    
    return selectedPlants;
}

QList<QString> AddRemindTask::getSelectedOperations() const {
    return cardManagerOperation->getMultipleSelectedMessages();
}


AddRemindTask::~AddRemindTask()
{
    delete ui;
}
