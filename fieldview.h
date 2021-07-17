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
    enum EFieldMode
    {
        EditMode = 0,
        SelfMode,
        EnemyMode
    };

public:
    FieldView();
    void changeMode(EFieldMode fieldMode);
    void resetShips();
    bool isFieldReady();
    void copyState(const FieldView* const fieldView);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void createScene();
    void initField();
    void initShips();
    void setShipDefaultPos(int index);
    static QPoint cellPosition(const QPoint& point);
    static bool isShipsCrossed(QGraphicsRectItem* targetShip, QGraphicsRectItem* ship);
    bool isShipPrimaryPos(QGraphicsRectItem* ship);
    void rotateShip(QGraphicsRectItem* ship);
    bool isShipOut(QGraphicsRectItem* ship);
    void setBattleState();

private:
    QGraphicsScene* mScene;
    FieldModel* mFieldModel;
    QGraphicsPixmapItem* mCells[FIELD_SIZE][FIELD_SIZE];

    QVector<QGraphicsRectItem*> mShips;
    QVector<QGraphicsRectItem*> mShipsPosition;
    QGraphicsRectItem* mCurrentItem;

    EFieldMode mFieldMode;

public slots:
    void onButtonRotateClicked();
    void onShoot(const QPoint& coordinate);

signals:
    void shoot(const QPoint& coordinated);
};

#endif // FIELDVIEW_H
