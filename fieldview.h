#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QDebug>

#define FIELD_SIZE 10

class FieldView : public QGraphicsView
{
public:
    FieldView();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // FIELDVIEW_H
