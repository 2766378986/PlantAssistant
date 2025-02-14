#include "selectcardmanager.h"
#include "ui_selectcardmanager.h"
#include "../../Manager/Manager.h"
#include <QHBoxLayout>
#include "../../Dialogs/PopUpDialog/popupdialog.h"
SelectCardManager::SelectCardManager(QWidget *parent, class Manager * manager, bool allowMultipleSelection)
    : QWidget(parent)
    , ui(new Ui::SelectCardManager)
    , manager(manager)
    , selectedIndex(-1)
    , allowMultipleSelection(allowMultipleSelection)
{
    ui->setupUi(this);

    layout = new QHBoxLayout(this);

    numOfCards = 0;

    // setAttribute(Qt::WA_TranslucentBackground);
    
    layout->setSpacing(10);

    setLayout(layout);
}

void SelectCardManager::setMargin(int margin) {
    layout->setSpacing(margin);
}

void SelectCardManager::addCard(const QString & imagePath, const QString & message) {
    SelectCard * card = new SelectCard(this, this, numOfCards);
    card->setInfo(imagePath, message);
    cards.append(card);
    layout->addWidget(card);
    numOfCards++;
}

void SelectCardManager::addCard(SelectCard * card) {
    cards.append(card);
    card->setIndex(numOfCards);
    layout->addWidget(card);
    numOfCards++;
}



void SelectCardManager::setCard(int index, const QString & imagePath, const QString & message) {
    if (index < 0 || index >= numOfCards) {
        return;
    }
    cards[index]->setInfo(imagePath, message);
}

SelectCard * SelectCardManager::getCard(int index) const {
    if (index < 0 || index >= numOfCards) {
        return nullptr;
    }
    return cards[index];
}

void SelectCardManager::removeCard(int index) {
    if (index < 0 || index >= numOfCards) {
        return;
    }
    cards[index]->deleteLater();
    cards.removeAt(index);
    layout->removeWidget(cards[index]);
}

void SelectCardManager::clearCards() {
    foreach (SelectCard * card, cards) {
        card->deleteLater();
    }
    cards.clear();
    numOfCards = 0;
}

QString SelectCardManager::getSelectedMessage() {
    if (selectedIndex == -1) {
        return "";
    }
    return cards[selectedIndex]->getMessage();
}

QList<QString> SelectCardManager::getMultipleSelectedMessages() {
    QList<QString> messages;
    foreach (int index, selectedIndexes) {
        messages.append(cards[index]->getMessage());
    }
    return messages;
}


void SelectCardManager::setSelectedIndex(int index) {
    if (index < 0 || index >= numOfCards) {
        return;
    }
    
    for (int i = 0; i < numOfCards; i++) {
        cards[i]->setSelected(false);
    }
    cards[index]->setSelected(true);
    selectedIndex = index;
}


bool SelectCardManager::hasSelectedCard() const {
    return selectedIndex != -1;
}

void SelectCardManager::onCardSelected(SelectCard * card) {
    static bool isProcessing = false;
    if (isProcessing) {
        return;
    }
    isProcessing = true;

    if (allowMultipleSelection) {

        if (card->getSelected()) {
            selectedIndexes.append(card->getIndex());
        }
        else {
            selectedIndexes.removeOne(card->getIndex());
        }

        isProcessing = false;
        return;
    }

    // 不允许多选

    int previousIndex = selectedIndex;

    // 之前没有选中卡片, 就选择现在的卡片，返回
    if (selectedIndex == -1) {
        selectedIndex = card->getIndex();
        isProcessing = false;
        return;
    }

    // 之前有选中卡片，并且选择了新的卡片，那么就取消之前选中的卡片，选中新的卡片，返回
    if (card->getSelected()) {

        selectedIndex = card->getIndex();
        cards[previousIndex]->setSelected(false);
        isProcessing = false;
        return;

    }

    // 之前有选中卡片，但是没有选中新的卡片，那么就取消之前选中的卡片，返回
    selectedIndex = -1;

    isProcessing = false;
}

int SelectCardManager::getSelectedIndex() const {
    return selectedIndex;
}

QList<int> & SelectCardManager::getSelectedIndexes() {
    return selectedIndexes;
}

QString SelectCardManager::getData(int index) const {
    return cards[index]->getData();
}

void SelectCardManager::setOrientation(int orientation) {

}




SelectCardManager::~SelectCardManager() {
    delete ui;
}
