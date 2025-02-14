#pragma once

#include <QWidget>


namespace Ui {
class SelectCard;
}

class SelectCard : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool selected READ getSelected WRITE setSelected)
    Q_PROPERTY(int index READ getIndex WRITE setIndex)

public:
    explicit SelectCard(QWidget *parent = nullptr, class SelectCardManager * selectCardManager = nullptr, int index = -1);
    ~SelectCard();

    void setInfo(const QString & imagePath, const QString & message);

    QString getMessage() const;

    bool getSelected() const;

    void setSelected(bool selected);

    int getIndex() const;

    void setIndex(int index);

    void setImageSize(int width, int height);

    void setSize(int width, int height);


    QString getData() const;

    void setData(const QString & data);

signals:
    void selectedChanged(SelectCard * card);

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;

    virtual void enterEvent(QEnterEvent * event) override;

    virtual void leaveEvent(QEvent * event) override;

    virtual void showEvent(QShowEvent * event) override;

private:
    void addStyle();


private:
    Ui::SelectCard *ui;
    
    class Manager * manager;

    class QLabel * labelImage;
    class QLabel * labelMessage;    

    bool selected;

    class SelectCardManager * selectCardManager;

    int index;

    QRect normalGeometry;

    QString data;
};
