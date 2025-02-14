#pragma once

#include <QWidget>
#include "../../CoreData/Plant/PlantInfo.h"

namespace Ui {
class PlantInformationManage;
}

class PlantInformationManage : public QWidget
{
    Q_OBJECT
    friend class PlantCard;

    enum PreviousOperation {
        PREVIOUS_PAGE,
        NEXT_PAGE,
        NONE
    };

    enum ColumnDataType {
        TEXT,
        INT,
        DATETIME
    };

    enum BatchOperationType {
        BATCH_WATER,
        BATCH_FERTILIZE,
        BATCH_PRUNE,
        BATCH_NONE
    };

public:
    explicit PlantInformationManage(QWidget *parent = nullptr, class Manager * manager = nullptr);
    ~PlantInformationManage();

    void setManager(class Manager * manager);

private:
    void initUI();

    void initTopSection();

    void initMiddleSection();

    void initBottomSection();

    void addStyle();

    void initConnect();

    void enterSelectionMode();

    void exitSelectionMode();
    
    void waterPlant(int index);
    
    void fertilizePlant(int index);

    void prunePlant(int index);
    
   
private slots:
    void addPlant();

    void refreshPlant();

    void nextPage();

    void previousPage();

    void searchPlant(const QString & text);

    void filterPlant(int index);

    void batchWaterButtonClicked();

    void batchFertilizeButtonClicked();

    void batchPruneButtonClicked();

    void batchOperationConfirm();

public:
    void editPlant(const Plant & plant, int index);

    void deletePlant(const Plant & plant, int index);

    // void addPlantToDatabase();
    

private:
    Ui::PlantInformationManage *ui;
    class Manager * manager;

    // 顶部区域
    class QLineEdit * searchLineEdit;
    class QComboBox * filterComboBox;
    class QPushButton * addButton;
    class QPushButton * refreshButton;

    // 中间区域
    QPushButton * leftButton;
    QList<class PlantCard *> plantCards;
    QPushButton * rightButton;
    class QWidget * plantCardContainer;

    // 底部区域
    class QLabel * statisticsLabel;
    class QLabel * growthStatusLabel;
    class QPushButton * batchWaterButton;
    class QPushButton * batchFertilizeButton;
    class QPushButton * batchPruneButton;
    class QPushButton * confirmButton;

    // 布局
    class QHBoxLayout * topLayout;
    class QHBoxLayout * middleLayout;
    class QHBoxLayout * bottomLayout;


    // 数据
    QList<Plant>& plantInfos;
    QList<bool> hide;
    QList<QString> databaseColumns;
    // 当前卡片在plantInfos中的索引
    int currentCard;
    // 当前页面的卡片数量
    int currentNumberOfCards;

    PreviousOperation previousOperation;

    QStringList filterOptions;
    enum FilterOption {
        ALL,
        PLANT_NAME,
        PLANT_SPECIES
    };
    FilterOption currentFilterOption;

    // 所有卡片在plantInfos中的索引
    QSet<int> usedPlantIndex;

    int cardsOfPerPage;

    QList<ColumnDataType> columnDataTypes;


    bool selectionMode;

    QList<int>& selectedPlantIndex;

    BatchOperationType batchOperationType;

};
