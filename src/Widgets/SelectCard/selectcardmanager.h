#pragma once

#include <QWidget>
#include "selectcard.h"

namespace Ui {
class SelectCardManager;
}

class SelectCardManager : public QWidget
{
    Q_OBJECT

public:
    static const int HORIZONTAL = 0;
    static const int VERTICAL = 1;
    
public:
    explicit SelectCardManager(QWidget *parent = nullptr, class Manager * manager = nullptr, bool allowMultipleSelection = false);
    ~SelectCardManager();

    void addCard(const QString & imagePath, const QString & message);

    void addCard(SelectCard * card);

    void setCard(int index, const QString & imagePath, const QString & message);

    void removeCard(int index);

    void clearCards();

    void setMargin(int margin);

    QString getSelectedMessage();

    QList<QString> getMultipleSelectedMessages();

    void setSelectedIndex(int index);

    bool hasSelectedCard() const;

    int getSelectedIndex() const;

    void setOrientation(int orientation);

    SelectCard * getCard(int index) const;
    
    QList<int> & getSelectedIndexes();

    QString getData(int index) const;

public slots:
    void onCardSelected(SelectCard * card);

private:
    Ui::SelectCardManager *ui;

    class Manager * manager;

    int numOfCards;
    
    QList<SelectCard *> cards;

    class QHBoxLayout * layout;
    class QVBoxLayout * layoutVertical;

    int selectedIndex;

    bool allowMultipleSelection;

    QList<int> selectedIndexes;

};
