#include "plantinformationmanage.h"
#include "ui_plantinformationmanage.h"
#include "../../Manager/Manager.h"
#include "../../Widgets/PlantCard/plantcard.h"
#include "../../Dialogs/MessageBox/pmessagebox.h"
#include "../../CoreData/Plant/PlantInfo.h"
#include "../../CoreData/User/User.h"
#include "../../Dialogs/AddPlantCard/addplantcarddialog.h"
#include "../../Dialogs/PopUpDialog/popupdialog.h"
#include "../CareRemind/careremind.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QLabel>
#include <QDateTime>
#include <qstyle.h>

PlantInformationManage::PlantInformationManage(QWidget *parent, class Manager * manager)
    : QWidget(parent)
    , ui(new Ui::PlantInformationManage)
    , manager(manager)
    , previousOperation(PreviousOperation::NONE)
    , cardsOfPerPage(3)
    , selectionMode(false)
    , batchOperationType(BatchOperationType::BATCH_NONE)
    , plantInfos(manager->getPlants())
    , selectedPlantIndex(manager->getSelectedPlantIndex())
{
    ui->setupUi(this);

    setObjectName("plantInformationManage");
    setAttribute(Qt::WA_StyledBackground, true);

    databaseColumns = {
                    "PlantID",
                    "PlantName", 
                    "Species", 
                    "PhotoURL", 
                    "WateringFrequency", 
                    "FertilizingFrequency", 
                    "PruningFrequency", 
                    "LightRequirement", 
                    "TemperatureRequirement", 
                    "HumidityRequirement", 
                    "SoilType", 
                    "DateAdded", 
                    "LastWatered", 
                    "LastFertilized", 
                    "LastPruned",
                    "UserName"
                    };

    columnDataTypes = {
        ColumnDataType::TEXT,
        ColumnDataType::TEXT,
        ColumnDataType::TEXT,
        ColumnDataType::TEXT,
        ColumnDataType::INT,
        ColumnDataType::INT,
        ColumnDataType::INT,
        ColumnDataType::TEXT,
        ColumnDataType::TEXT,
        ColumnDataType::TEXT,
        ColumnDataType::TEXT,
        ColumnDataType::DATETIME,
        ColumnDataType::DATETIME,
        ColumnDataType::DATETIME,
        ColumnDataType::DATETIME,
        ColumnDataType::TEXT
    };


    filterOptions = { "全部", "植物名称", "物种" };

    initUI();

    initConnect();

}

void PlantInformationManage::initUI() {
    initTopSection();
    initMiddleSection();
    initBottomSection();

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(middleLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    
    addStyle();
}

void PlantInformationManage::initConnect() {
    connect(addButton, &QPushButton::clicked, this, &PlantInformationManage::addPlant);
    connect(refreshButton, &QPushButton::clicked, this, &PlantInformationManage::refreshPlant);
    connect(leftButton, &QPushButton::clicked, this, &PlantInformationManage::previousPage);
    connect(rightButton, &QPushButton::clicked, this, &PlantInformationManage::nextPage);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &PlantInformationManage::searchPlant);
    connect(filterComboBox, &QComboBox::currentIndexChanged, this, &PlantInformationManage::filterPlant);
    connect(batchWaterButton, &QPushButton::clicked, this, &PlantInformationManage::batchWaterButtonClicked);
    connect(batchFertilizeButton, &QPushButton::clicked, this, &PlantInformationManage::batchFertilizeButtonClicked);
    connect(batchPruneButton, &QPushButton::clicked, this, &PlantInformationManage::batchPruneButtonClicked);
    connect(confirmButton, &QPushButton::clicked, this, &PlantInformationManage::batchOperationConfirm);
}

void PlantInformationManage::addStyle() {
    QFile file("./res/QSS/informationManage.qss");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        setStyleSheet(stream.readAll());
    }
}

void PlantInformationManage::initTopSection() {
    topLayout = new QHBoxLayout;

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setObjectName("searchLineEdit");
    QIcon searchIcon("./res/icons/search.png");
    searchLineEdit->addAction(searchIcon, QLineEdit::LeadingPosition);
    searchLineEdit->setPlaceholderText("请输入植物信息");

    // 用QComboBox来实现过滤功能，filterOptions是过滤选项, currentFilterOption是当前过滤选项
    filterComboBox = new QComboBox(this);
    filterComboBox->setObjectName("filterComboBox");
    
    for (const auto & option : filterOptions) {
        filterComboBox->addItem(option);
    }

    filterComboBox->setCurrentIndex(0);
    currentFilterOption = FilterOption::ALL;

    filterComboBox->setFocusPolicy(Qt::NoFocus);

    addButton = new QPushButton("添加", this);
    addButton->setObjectName("addButton");

    refreshButton = new QPushButton("刷新", this);
    refreshButton->setObjectName("refreshButton");

    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(filterComboBox);
    topLayout->addWidget(addButton);
    topLayout->addWidget(refreshButton);
}

void PlantInformationManage::initMiddleSection() {
    middleLayout = new QHBoxLayout;

    leftButton = new QPushButton(this);
    rightButton = new QPushButton(this);
    plantCardContainer = new QWidget(this);

    leftButton->setObjectName("leftButton");
    rightButton->setObjectName("rightButton");
    leftButton->setIcon(QIcon("./res/icons/previous-page.png"));
    rightButton->setIcon(QIcon("./res/icons/next-page.png"));

    leftButton->setFixedSize(32, 32);
    rightButton->setFixedSize(32, 32);

    plantCardContainer->setObjectName("plantCardContainer");
    plantCardContainer->setFixedSize(1000, 600);
    plantCardContainer->setAttribute(Qt::WA_StyledBackground, true);
    plantCardContainer->setStyleSheet("background-color: transparent;");
    plantCardContainer->setContentsMargins(0, 0, 0, 0);

    middleLayout->addWidget(leftButton);


    QSqlQuery query;
    QString sql = "SELECT";

    // databaseColumns 中存储的是需要的列名 QList<QString>
    for (auto it = databaseColumns.begin(); it != databaseColumns.end(); it++) {
        sql += " " + *it;
        if (it != databaseColumns.end() - 1) {
            sql += ", ";
        }
    }
    sql += " FROM Plants WHERE UserName = :username";

    query.prepare(sql);
    query.bindValue(":username", manager->getUser()->getName());
    query.exec();
    
    // 从数据库中获取所有植物信息，先存储到一个列表中
    while (query.next()) {
        Plant plant;
        QList<QString> columnValues;
        for (int i = 0; i < databaseColumns.size(); i++) {
            if (columnDataTypes[i] == ColumnDataType::DATETIME) {
                columnValues.append(query.value(databaseColumns[i]).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            }
            else {
                columnValues.append(query.value(databaseColumns[i]).toString());
            }
        }
        plant.setValues(columnValues);
        plantInfos.append(plant);
    }


    // 初始化所有植物卡片都不隐藏
    hide = QList<bool>(plantInfos.size(), false);

    // 初始化所有卡片在plantInfos中的索引

    // 卡片布局
    int loadCount = 0;
    QHBoxLayout * plantCardLayout = new QHBoxLayout;

    // 初始化建立min(3, plantInfos.size())个卡片
    for (int i = 0; i < plantInfos.size(); ++i) {
        PlantCard * plantCard = new PlantCard(this, manager);
        plantCards.append(plantCard);
        usedPlantIndex.insert(i);
       
        plantCard->setPlant(plantInfos[i], i);
        currentCard = i;
        loadCount++;

        plantCardLayout->addWidget(plantCard);
        if (loadCount == cardsOfPerPage) break;
    }
    previousOperation = PreviousOperation::NEXT_PAGE;

    

    currentNumberOfCards = loadCount;

    plantCardContainer->setLayout(plantCardLayout);

    middleLayout->addWidget(plantCardContainer);

    middleLayout->addWidget(rightButton);

}

void PlantInformationManage::initBottomSection() {
    bottomLayout = new QHBoxLayout;

    statisticsLabel = new QLabel("植物总数: " + QString::number(plantInfos.size()), this);
    statisticsLabel->setObjectName("statisticsLabel");
    bottomLayout->addWidget(statisticsLabel);

    bottomLayout->addStretch();

    batchWaterButton = new QPushButton("批量浇水", this);
    batchWaterButton->setObjectName("batchWaterButton");
    batchWaterButton->setFixedSize(200, 30);
    bottomLayout->addWidget(batchWaterButton);
    

    batchFertilizeButton = new QPushButton("批量施肥", this);
    batchFertilizeButton->setObjectName("batchFertilizeButton");
    batchFertilizeButton->setFixedSize(200, 30);
    bottomLayout->addWidget(batchFertilizeButton);

    batchPruneButton = new QPushButton("批量修剪", this);
    batchPruneButton->setObjectName("batchPruneButton");
    batchPruneButton->setFixedSize(200, 30);
    bottomLayout->addWidget(batchPruneButton);

    confirmButton = new QPushButton("确认", this);
    confirmButton->setObjectName("confirmButton");
    confirmButton->setFixedSize(200, 30);
    confirmButton->setHidden(true);
    
    bottomLayout->addWidget(confirmButton);


    bottomLayout->addStretch();
}

void PlantInformationManage::nextPage() {
    if (currentCard == plantInfos.size() - 1) {
        PMessagebox::information(this, "提示", "没有更多植物了");
        return;
    }

    int newPlantIndex;


    newPlantIndex = currentCard;

    int loadCount = 0;
    for (int i = newPlantIndex; i < plantInfos.size(); ++i) {
        // 没有隐藏并且不在当前页面中
        if (!hide[i] && (usedPlantIndex.find(i) == usedPlantIndex.end())) {
            plantCards[loadCount]->setPlant(plantInfos[i], i);
            loadCount++;
            currentCard = i;

        }

        if (loadCount == cardsOfPerPage) break;
    }


    // 没有加载就不要修改任何信息
    if (loadCount == 0) {
        PMessagebox::information(this, "提示", "没有更多植物了");
        return;
    }

    currentNumberOfCards = loadCount;

    // 先让所有的卡片都隐藏起来
    for (int i = 0; i < plantCards.size(); i++) {
        plantCards[i]->setHide(true);

        // 现将所有卡片都取消选中，后面根据已经选中的植物来改变状态
        plantCards[i]->setSelected(false);
    }

    usedPlantIndex.clear();


    for (int i = 0; i < loadCount; i++) {
        plantCards[i]->setHide(false);
        usedPlantIndex.insert(plantCards[i]->getIndexOfPlantArray());

        // 如果这些植物之前已经选中了，那么就改变状态
        if (selectedPlantIndex.contains(plantCards[i]->getIndexOfPlantArray())) {
            plantCards[i]->setSelected(true);
        }
    }



    previousOperation = PreviousOperation::NEXT_PAGE;

}

void PlantInformationManage::previousPage() {
    if (currentCard < cardsOfPerPage) {
        PMessagebox::information(this, "提示", "没有更多植物了");
        return;
    }

    int newPlantIndex = currentCard;

    
    // 从后往前遍历每找到一个可以添加的植物就添加到卡片中，直到找到三个，currentLoad表明卡片控件的索引
    int currentLoad = 2;
    int loadCount = 0;
    for (int i = newPlantIndex; i >= 0; i--) {
        // 没有隐藏并且不在当前的页面中
        if (!hide[i] && usedPlantIndex.find(i) == usedPlantIndex.end()) {
            plantCards[currentLoad]->setPlant(plantInfos[i], i);
            currentLoad--;
            currentCard = i;
            loadCount++;
        }

        if (currentLoad == -1) break;
    }

    // 如果没有加载植物就不要改变卡片的显示情况
    if (loadCount == 0) {
        PMessagebox::information(this, "提示", "没有更多植物了");
        return;
    }

    currentNumberOfCards = loadCount;

    // 先让所有的卡片都隐藏起来
    for (int i = 0; i < plantCards.size(); i++) {
        plantCards[i]->setHide(true);

        // 现将所有卡片都取消选中，后面根据已经选中的植物来改变状态
        plantCards[i]->setSelected(false);
    }

    usedPlantIndex.clear();

    for (int i = currentLoad + 1; i < plantCards.size(); i++) {
        plantCards[i]->setHide(false);
        usedPlantIndex.insert(plantCards[i]->getIndexOfPlantArray());

        // 如果这些植物之前已经选中了，那么就不要改变状态
        if (selectedPlantIndex.contains(plantCards[i]->getIndexOfPlantArray())) {
            plantCards[i]->setSelected(true);
        }

    }

    previousOperation = PreviousOperation::PREVIOUS_PAGE;

}

void PlantInformationManage::searchPlant(const QString & text) {
    QString searchText = text;
    QString filterCondition;

    if (searchText.isEmpty()) {
        for (int i = 0; i < plantInfos.size(); ++i) {
            hide[i] = false;
        }
        return;
    }

    for (int i = 0; i < plantInfos.size(); ++i) {
        if (currentFilterOption == FilterOption::ALL) {
            hide[i] = !(plantInfos[i].values[Plant::PLANT_NAME].contains(searchText) || plantInfos[i].values[Plant::PLANT_SPECIES].contains(searchText));
        }
        else if (currentFilterOption == FilterOption::PLANT_NAME) {
            hide[i] = !plantInfos[i].values[Plant::PLANT_NAME].contains(searchText);
        }
        else if (currentFilterOption == FilterOption::PLANT_SPECIES) {
            hide[i] = !plantInfos[i].values[Plant::PLANT_SPECIES].contains(searchText);
        }
    }

    // 不知道要不要在这更新植物卡片
    
}

void PlantInformationManage::filterPlant(int index) {
    switch (index) {
        case 0:
            currentFilterOption = FilterOption::ALL;
            break;
        case 1:
            currentFilterOption = FilterOption::PLANT_NAME;
            break;
        case 2:
            currentFilterOption = FilterOption::PLANT_SPECIES;
            break;
    }
    searchPlant(searchLineEdit->text());
}


void PlantInformationManage::addPlant() {
    AddPlantCardDialog addPlantCardDialog(this, manager);
    if (addPlantCardDialog.exec() == QDialog::Accepted) {
        const auto & plantData = addPlantCardDialog.getPlantData();
        QSqlQuery query;
        QString sql = "INSERT INTO plants (";
       
       // 加入列名
        for (auto it = databaseColumns.begin() + 1; it != databaseColumns.end(); it++) {
            sql += *it;
            if (it != databaseColumns.end() - 1) {
                sql += ", ";
            }
        }
        sql += ") VALUES (";
        
        // 加入值
        for (auto it = plantData.begin(); it != plantData.end(); it++) {
            sql += "\"" + *it + "\"";
            if (it != plantData.end() - 1) {
                sql += ", ";
            }
        }

        sql += ")";
        query.prepare(sql);


        if (!query.exec()) {
            PMessagebox::critical(this, "提示", "添加植物失败");
            PMessagebox::critical(this, "提示", query.lastError().text());
            return;
        }


        // 获取当前插入的植物的ID，添加到plantInfos中，且刷新
        QString lastInsertId = query.lastInsertId().toString();

        Plant plant;
        QList<QString> values;
        values.append(lastInsertId);

        for (const auto & value : plantData) {
            values.append(value);
        }

        plant.setValues(values);
        plantInfos.append(plant);
        hide.append(false);

        // 插入养护统计数据
        sql = "INSERT INTO CareStatistics (PlantID, Month, WateringCount, FertilizingCount, PruningCount, UserName) \
         VALUES (:plantID, :month, :wateringCount, :fertilizingCount, :pruningCount, :userName)";
         
        query.prepare(sql);
        query.bindValue(":plantID", lastInsertId);
        query.bindValue(":month", QDate::currentDate().toString("yyyy-MM-dd"));
        query.bindValue(":wateringCount", 0);
        query.bindValue(":fertilizingCount", 0);
        query.bindValue(":pruningCount", 0);
        query.bindValue(":userName", manager->getUser()->getName());
        query.exec();

        refreshPlant();
    }
}

void PlantInformationManage::refreshPlant() {
    int loadCount = 0;


    for (int i = 0; i < plantInfos.size(); ++i) {
        if (!hide[i]) {
            // 如果当前卡片的植物已经使用过了，那么就是设置为未使用
            plantCards[loadCount]->setPlant(plantInfos[i], i);
            loadCount++;
            currentCard = i;
        }

        if (loadCount == plantCards.size()) break;
    }

    if (loadCount == 0) {
        PMessagebox::information(this, "提示", "没有符合条件的植物");
        for (int i = 0; i < plantCards.size(); i++) {
            plantCards[i]->setHide(true);
        }

        return;
    }

    // 先让所有的卡片都隐藏起来
    for (int i = 0; i < plantCards.size(); i++) {
        plantCards[i]->setHide(true);
    }

    usedPlantIndex.clear();

    for (int i = 0; i < loadCount; i++) {
        plantCards[i]->setHide(false);
        usedPlantIndex.insert(plantCards[i]->getIndexOfPlantArray());
    }

    currentNumberOfCards = loadCount;

    previousOperation = PreviousOperation::NEXT_PAGE;
}


void PlantInformationManage::editPlant(const Plant & plant, int index) {
    QString ID = plant.values[Plant::PLANT_ID];

    AddPlantCardDialog addPlantCardDialog(this, manager, AddPlantCardDialog::DialogType::EDIT, plant);


    if (addPlantCardDialog.exec() == QDialog::Accepted) {
        const auto & plantData = addPlantCardDialog.getPlantData();

        QSqlQuery query;
        QString sql = "UPDATE Plants SET";

        // plantData从plantName开始，而plantInfos[index].value是从id开始的，所以那i和i+1比较
        bool firstInequality = true;
        for (int i = 0; i < plantData.size(); i++) {

            if (plantData[i] != plantInfos[index].values[i + 1]) {
                if (firstInequality) {
                    sql += " " + databaseColumns[i + 1] + " = \"" + plantData[i] + "\"";
                    firstInequality = false;
                }
                else {
                    sql += ", " + databaseColumns[i + 1] + " = \"" + plantData[i] + "\"";
                }


                plantInfos[index].values[i + 1] = plantData[i];
            }
        }

        // 如果所有值都相等，那么就不需要更新
        if (firstInequality) return;


        sql += " WHERE PlantID = " + ID;

        query.prepare(sql);
        
        if (!query.exec()) {
            PMessagebox::critical(this, "提示", "更新植物失败");
            PMessagebox::critical(this, "提示", query.lastError().text());
            return;
        }
        
        refreshPlant();
    }
}

void PlantInformationManage::deletePlant(const Plant & plant, int index) {
    auto result = PMessagebox::information(this, "提示", "确定要删除该植物吗？", PMessagebox::ButtonType::OK_CANCEL);
    
    if (result == PMessagebox::Result::RESULT_CANCEL) return;

    QString ID = plant.values[Plant::PLANT_ID];

    
    plantInfos.removeAt(index);
    hide.removeAt(index);

    QSqlQuery query;


    // 删除过后还要删除养护记录，删除养护提醒
    QString sql = "DELETE FROM CareStatistics WHERE PlantID = " + ID;
    query.prepare(sql);
    query.exec();

    // 删除养护提醒
    manager->getCareRemind()->deleteTask(ID);


    sql = "DELETE FROM Plants WHERE PlantID = " + ID;
    query.prepare(sql);
    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "删除植物失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        
        QFile file("./sql.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(sql.toUtf8());
        file.close();
        return;
    }

    PopUpDialog::information(this, "删除成功");

    refreshPlant();
}



PlantInformationManage::~PlantInformationManage() {
    delete ui;
}

void PlantInformationManage::setManager(class Manager * manager) {
    this->manager = manager;
}

void PlantInformationManage::enterSelectionMode() {
    // 取消所有选中状态
    for (int i = 0; i < plantCards.size(); i++) {
        plantCards[i]->setSelected(false);
    }


    manager->setSelectionMode(true);
    selectedPlantIndex.clear();

    addButton->setEnabled(false);
    refreshButton->setEnabled(false);
    searchLineEdit->setEnabled(false);
    batchWaterButton->setEnabled(false);
    batchFertilizeButton->setEnabled(false);
    batchPruneButton->setEnabled(false);

    confirmButton->setHidden(false);
    
}

void PlantInformationManage::waterPlant(int index) {
    QSqlQuery query;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString sql = "UPDATE CareStatistics SET WateringCount = WateringCount + 1, Month = :date WHERE PlantID = :plantID";
    query.prepare(sql);
    
    query.bindValue(":date", dateTime.date());
    query.bindValue(":plantID", plantInfos[index].values[Plant::PLANT_ID]);

    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "浇水失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        return;
    }

    // 记录上次浇水日期
    plantInfos[index].values[Plant::LAST_WATERED] = dateTime.toString("yyyy-MM-dd HH:mm:ss");

    sql = "UPDATE Plants SET LastWatered = :date WHERE PlantID = :plantID";
    query.prepare(sql);
    query.bindValue(":date", plantInfos[index].values[Plant::LAST_WATERED]);
    query.bindValue(":plantID", plantInfos[index].values[Plant::PLANT_ID]);

    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "更新浇水日期失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        return;
    }
}

void PlantInformationManage::fertilizePlant(int index) {
    QSqlQuery query;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString sql = "UPDATE CareStatistics SET FertilizingCount = FertilizingCount + 1, Month = :date WHERE PlantID = :plantID";
    query.prepare(sql);
    
    query.bindValue(":date", dateTime.date());
    query.bindValue(":plantID", plantInfos[index].values[Plant::PLANT_ID]);

    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "施肥失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        return;
    }

    // 记录上次施肥日期
    plantInfos[index].values[Plant::LAST_FERTILIZED] = dateTime.toString("yyyy-MM-dd HH:mm:ss");

    sql = "UPDATE Plants SET LastFertilized = :date WHERE PlantID = :plantID";
    query.prepare(sql);
    query.bindValue(":date", plantInfos[index].values[Plant::LAST_FERTILIZED]);
    query.bindValue(":plantID", plantInfos[index].values[Plant::PLANT_ID]);

    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "更新施肥日期失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        return;
    }
}

void PlantInformationManage::prunePlant(int index) {
    QSqlQuery query;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString sql = "UPDATE CareStatistics SET PruningCount = PruningCount + 1, Month = :date WHERE PlantID = :plantID";
    query.prepare(sql);
    
    query.bindValue(":date", dateTime.date());
    query.bindValue(":plantID", plantInfos[index].values[Plant::PLANT_ID]);

    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "修剪失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        return;
    }

    // 记录上次修剪日期
    plantInfos[index].values[Plant::LAST_PRUNED] = dateTime.toString("yyyy-MM-dd HH:mm:ss");

    sql = "UPDATE Plants SET LastPruned = :date WHERE PlantID = :plantID";
    query.prepare(sql);
    query.bindValue(":date", plantInfos[index].values[Plant::LAST_PRUNED]);
    query.bindValue(":plantID", plantInfos[index].values[Plant::PLANT_ID]);

    if (!query.exec()) {
        PMessagebox::critical(this, "提示", "更新修剪日期失败");
        PMessagebox::critical(this, "提示", query.lastError().text());
        return;
    }

}

void PlantInformationManage::batchWaterButtonClicked() {
    enterSelectionMode();
    batchOperationType = BatchOperationType::BATCH_WATER;
}

void PlantInformationManage::batchFertilizeButtonClicked() {
    enterSelectionMode();
    batchOperationType = BatchOperationType::BATCH_FERTILIZE;
}

void PlantInformationManage::batchPruneButtonClicked() {
    enterSelectionMode();
    batchOperationType = BatchOperationType::BATCH_PRUNE;
}


void PlantInformationManage::exitSelectionMode() {
    manager->setSelectionMode(false);

    addButton->setEnabled(true);
    refreshButton->setEnabled(true);
    searchLineEdit->setEnabled(true);
    batchWaterButton->setEnabled(true);
    batchFertilizeButton->setEnabled(true);
    batchPruneButton->setEnabled(true);

    confirmButton->setHidden(true);

    // 取消所有选中状态
    for (int i = 0; i < plantCards.size(); i++) {
        plantCards[i]->setSelected(false);
    }

}

void PlantInformationManage::batchOperationConfirm() {
    exitSelectionMode();

    if (selectedPlantIndex.isEmpty()) return;

    for (const auto & index : selectedPlantIndex) {
        switch (batchOperationType) {
            case BatchOperationType::BATCH_WATER:
                waterPlant(index);
                break;
            case BatchOperationType::BATCH_FERTILIZE:
                fertilizePlant(index);
                break;
            case BatchOperationType::BATCH_PRUNE:
                prunePlant(index);
                break;
            default:
                break;
        }
    }


    switch(batchOperationType) {
        case BatchOperationType::BATCH_WATER:
            PopUpDialog::information(this, "批量浇水完成");
            break;
        case BatchOperationType::BATCH_FERTILIZE:
            PopUpDialog::information(this, "批量施肥完成");
            break;
        case BatchOperationType::BATCH_PRUNE:
            PopUpDialog::information(this, "批量修剪完成");
            break;
        default:
            break;
    }

    selectedPlantIndex.clear();
}

