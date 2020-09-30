#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QMessageBox>

#include "fieldmodel.h"

#define FIELD_SIZE 10
#define CELL_SIZE 50


class FieldView : public QGraphicsView
{
    Q_OBJECT

typedef std::map<int, int> ShipsInfo;

public:
    FieldView();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void initField();
    void initShips();
    void setShipDefaultPos(int index);
    static QPoint cellPosition(const QPoint& point);
    static bool isShipsCrossed(QGraphicsRectItem* targetShip, QGraphicsRectItem* ship);
    bool isShipPrimaryPos(QGraphicsRectItem* ship);
    void rotateShip(QGraphicsRectItem* ship);
    bool isShipOut(QGraphicsRectItem* ship);

private:
    QGraphicsScene* mScene;
    FieldModel* mFieldModel;
    QGraphicsPixmapItem* mCells[FIELD_SIZE][FIELD_SIZE];

    QVector<QGraphicsRectItem*> mShips;
    QVector<QGraphicsRectItem*> mShipsPosition;
    QGraphicsRectItem* mCurrentItem;

public slots:
    void onButtonRotateClicked();
    void onButtonFinishClicked();
};

#endif // FIELDVIEW_H
