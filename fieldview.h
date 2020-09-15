#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QDebug>

#include "fieldmodel.h"

#define FIELD_SIZE 10

class FieldView : public QGraphicsView
{
public:
    FieldView();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsScene* mScene;
    FieldModel* mFieldModel;
    QGraphicsPixmapItem* mCells[FIELD_SIZE][FIELD_SIZE];
};

#endif // FIELDVIEW_H
