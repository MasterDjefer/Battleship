#include "fieldview.h"

FieldView::FieldView() : mCurrentItem(nullptr)
{
    mFieldModel = new FieldModel;

    mScene = new QGraphicsScene;
    mScene->setSceneRect(0, 0, 800, 500);

    this->setScene(mScene);
    this->setFixedSize(mScene->width(), mScene->height());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < FIELD_SIZE; ++i)
    {
        for (int j = 0; j < FIELD_SIZE; ++j)
        {
            QGraphicsPixmapItem *temp = new QGraphicsPixmapItem;
            QPixmap pixmap(":/images/primary.png");
            pixmap = pixmap.scaled(50,50);
            temp->setPixmap(pixmap);
            temp->setPos(50 * i, 50 * j);

            mScene->addItem(temp);
            mCells[i][j] = temp;
        }
    }

    QGraphicsRectItem* rect1 = new QGraphicsRectItem(610,160,50,50);
    rect1->setBrush(Qt::red);

    QGraphicsRectItem* rect2 = new QGraphicsRectItem(600,100,50,50);
    rect2->setBrush(Qt::red);

    QGraphicsRectItem* rect3 = new QGraphicsRectItem(550,300,200,50);
    rect3->setBrush(Qt::red);

    mShips.push_back(rect1);
    mShips.push_back(rect2);
    mShips.push_back(rect3);

    mScene->addItem(rect3);
    mScene->addItem(rect2);
    mScene->addItem(rect1);
}

void FieldView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int i = event->pos().x() / 50;
        int j = event->pos().y() / 50;

        for (int i = 0; i < mShips.size(); ++i)
        {
            mShips.at(i)->setBrush(Qt::red);
        }

        for (int i = 0; i < mShips.size(); ++i)
        {
            QGraphicsRectItem* rect = mShips.at(i);

            if ((event->x() >= rect->rect().x() && event->x() <= (rect->rect().x() + rect->rect().width())) &&
                (event->y() >= rect->rect().y() && event->y() <= (rect->rect().y() + rect->rect().height())))
            {
                mCurrentItem = rect;
                rect->setBrush(Qt::blue);
                return;
            }
        }

        mCurrentItem = nullptr;


        if (i >= FIELD_SIZE || j >= FIELD_SIZE)
            return;


        QPixmap pixmap;

        switch (mFieldModel->getCellValue(i, j))
        {
        case FieldModel::Empty:
            pixmap = QPixmap(":/images/empty.png");
            break;
        case FieldModel::Ship:
            pixmap = QPixmap(":/images/hurted.png");
            break;
        default:
            break;
        }

        pixmap = pixmap.scaled(50,50);
        mCells[i][j]->setPixmap(pixmap);
    }
}

void FieldView::mouseMoveEvent(QMouseEvent *event)
{
    if (mCurrentItem)
    {
        QPoint nPos(event->pos().x() - mCurrentItem->rect().width() / 2, event->pos().y() - mCurrentItem->rect().height() / 2);
        if (nPos.x() + mCurrentItem->rect().width() > mScene->width())
        {
            nPos.setX(mScene->width() - mCurrentItem->rect().width());
        }
        else
        if (nPos.x()  < 0)
        {
            nPos.setX(0);
        }
        if (nPos.y() + mCurrentItem->rect().height() > mScene->height())
        {
            nPos.setY(mScene->height() - mCurrentItem->rect().height());
        }
        else
        if (nPos.y()  < 0)
        {
            nPos.setY(0);
        }

        mCurrentItem->setRect(nPos.x(), nPos.y(), mCurrentItem->rect().width(), mCurrentItem->rect().height());
    }
}

void FieldView::mouseReleaseEvent(QMouseEvent *event)
{
//    pressed = false;
}
