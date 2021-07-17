#include "fieldview.h"

FieldView::FieldView() : mCurrentItem(nullptr), mFieldMode(EditMode)
{
    mFieldModel = new FieldModel;

    createScene();

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initField();
    initShips();
}

void FieldView::changeMode(EFieldMode fieldMode)
{
    mFieldMode = fieldMode;

    QVector<QGraphicsRectItem*> cShips;
    std::for_each(mShips.begin(), mShips.end(), [&cShips](QGraphicsRectItem* item)
    {
        QGraphicsRectItem* nItem = new QGraphicsRectItem(item->rect());
        nItem->setBrush(Qt::red);
        cShips.push_back(nItem);
    });

    switch (mFieldMode)
    {
    case SelfMode:
        delete mScene;
        createScene();
        initField();

        mShips.clear();
        std::for_each(cShips.begin(), cShips.end(), [this](QGraphicsRectItem* item)
        {
            mShips.push_back(item);
        });

        for (int i = mShips.size() - 1; i >= 0; --i)
        {
            //cant add it because 'delete mScene delete also child component'
            mScene->addItem(mShips.at(i));
        }
        break;

    case EnemyMode:
        delete mScene;
        createScene();
        initField();
        break;

    default:
        break;
    }
}

void FieldView::resetShips()
{
    mCurrentItem = nullptr;

    for (int i = 0; i < FIELD_SIZE; ++i)
    {
        for (int j = 0; j < FIELD_SIZE; ++j)
        {
            mScene->removeItem(mCells[i][j]);
            delete mCells[i][j];
        }
    }

    while (mShips.size() > 0)
    {
        mScene->removeItem(mShips.back());
        delete mShips.back();
        mShips.pop_back();
    }
    while (mShipsPosition.size() > 0)
    {
        mScene->removeItem(mShipsPosition.back());
        delete mShipsPosition.back();
        mShipsPosition.pop_back();
    }

    initField();
    initShips();
}

bool FieldView::isFieldReady()
{
    for (int i = 0; i < mShips.size(); ++i)
    {
        QGraphicsRectItem* ship = mShips.at(i);
        int width = ship->rect().width() > ship->rect().height() ? ship->rect().width() : ship->rect().height();
        QGraphicsRectItem* shipPos = mShipsPosition.at(width / CELL_SIZE - 1);

        if (shipPos->rect().x() == ship->rect().x() && shipPos->rect().y() == ship->rect().y())
        {
            return false;
        }
    }

    return true;
}

void FieldView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPoint cellPos = cellPosition(event->pos());
        int i = cellPos.x();
        int j = cellPos.y();

        if (mCurrentItem)
        {
            QPen pen;
            pen.setWidth(1);
            mCurrentItem->setPen(pen);
        }

        for (int i = 0; i < mShips.size(); ++i)
        {
            QGraphicsRectItem* rect = mShips.at(i);

            if ((event->x() >= rect->rect().x() && event->x() <= (rect->rect().x() + rect->rect().width())) &&
                (event->y() >= rect->rect().y() && event->y() <= (rect->rect().y() + rect->rect().height())))
            {
                mCurrentItem = rect;
                QPen pen;
                pen.setWidth(3);
                rect->setPen(pen);
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

        pixmap = pixmap.scaled(CELL_SIZE,CELL_SIZE);
        mCells[i][j]->setPixmap(pixmap);

        emit shoot(QPoint(i, j));
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
    QPoint cellPos = cellPosition(event->pos());

    if (mCurrentItem)
    {
        if (cellPos.x() >= FIELD_SIZE)
        {
            int width = mCurrentItem->rect().width() > mCurrentItem->rect().height() ?
                        mCurrentItem->rect().width() : mCurrentItem->rect().height();
            int index = width / CELL_SIZE - 1;
            setShipDefaultPos(index);
        }
        else
        {
            int centerPosX = (mCurrentItem->rect().width() / CELL_SIZE) / 2;
            if (!((int)(mCurrentItem->rect().width() / CELL_SIZE) % 2))
            {
                centerPosX--;
            }

            int centerPosY = (mCurrentItem->rect().height() / CELL_SIZE) / 2;
            if (!((int)(mCurrentItem->rect().height() / CELL_SIZE) % 2))
            {
                centerPosY--;
            }

            int nXPos = (cellPos.x() - centerPosX) * CELL_SIZE;
            int nYPos = (cellPos.y() - centerPosY) * CELL_SIZE;

            if (nXPos < 0)
                nXPos = 0;
            if (nYPos < 0)
                nYPos = 0;
            if (nYPos >= FIELD_SIZE * CELL_SIZE)
                nYPos = (FIELD_SIZE - 1) * CELL_SIZE;

            if (mCurrentItem->rect().x() + mCurrentItem->rect().width() >= FIELD_SIZE * CELL_SIZE)
            {
                nXPos = FIELD_SIZE * CELL_SIZE - mCurrentItem->rect().width();
            }

            if (mCurrentItem->rect().y() + mCurrentItem->rect().height() >= FIELD_SIZE * CELL_SIZE)
            {
                nYPos = FIELD_SIZE * CELL_SIZE - mCurrentItem->rect().height();
            }

            mCurrentItem->setRect(nXPos, nYPos, mCurrentItem->rect().width(), mCurrentItem->rect().height());
        }

        for (int i = 0; i < mShips.size(); ++i)
        {
            QGraphicsRectItem *ship = mShips.at(i);

            if (ship != mCurrentItem)
            {
                if (isShipsCrossed(mCurrentItem, ship))
                {
                    int width = mCurrentItem->rect().width() > mCurrentItem->rect().height() ?
                                mCurrentItem->rect().width() : mCurrentItem->rect().height();
                    int index = width / CELL_SIZE - 1;
                    setShipDefaultPos(index);

                    return;
                }
            }
        }
    }
}

void FieldView::createScene()
{
    mScene = new QGraphicsScene;
    mScene->setSceneRect(0, 0, (FIELD_SIZE + ((mFieldMode == EditMode) ? 6 : 0))* CELL_SIZE, FIELD_SIZE * CELL_SIZE);

    this->setScene(mScene);
    this->setFixedSize(mScene->width(), mScene->height());
}

void FieldView::initField()
{
    for (int i = 0; i < FIELD_SIZE; ++i)
    {
        for (int j = 0; j < FIELD_SIZE; ++j)
        {
            QGraphicsPixmapItem *temp = new QGraphicsPixmapItem;
            QPixmap pixmap(":/images/primary.png");
            pixmap = pixmap.scaled(CELL_SIZE,CELL_SIZE);
            temp->setPixmap(pixmap);
            temp->setPos(CELL_SIZE * i, CELL_SIZE * j);

            mScene->addItem(temp);
            mCells[i][j] = temp;
        }
    }
}

void FieldView::initShips()
{
    ShipsInfo shipsInfo = {{1, 4}, {2, 3}, {3, 2}, {4, 1}};
    QPoint shipPos((FIELD_SIZE + 1) * CELL_SIZE, 1 * CELL_SIZE);

    for (ShipsInfo::const_iterator it = shipsInfo.begin(); it != shipsInfo.end(); ++it)
    {
        int shipSize = it->first;
        int shipsCount = it->second;

        for (int i = 0; i < shipsCount; ++i)
        {
            QGraphicsRectItem* ship = new QGraphicsRectItem(shipPos.x(), shipPos.y(), shipSize * CELL_SIZE, CELL_SIZE);
            ship->setBrush(Qt::red);
            mShips.push_back(ship);
        }

        QGraphicsRectItem* shipPosition = new QGraphicsRectItem(shipPos.x(), shipPos.y(), shipSize * CELL_SIZE, CELL_SIZE);
        shipPosition->setPen(Qt::DashDotLine);
        mShipsPosition.push_back(shipPosition);

        shipPos = QPoint(shipPos.x(), shipPos.y() + 2 * CELL_SIZE);
    }

    for (int i = mShipsPosition.size() - 1; i >= 0; --i)
    {
        mScene->addItem(mShipsPosition.at(i));
    }

    for (int i = mShips.size() - 1; i >= 0; --i)
    {
        mScene->addItem(mShips.at(i));
    }
}

void FieldView::setShipDefaultPos(int index)
{
    mCurrentItem->setRect(mShipsPosition.at(index)->rect().x(), mShipsPosition.at(index)->rect().y(),
                          mShipsPosition.at(index)->rect().width(), mShipsPosition.at(index)->rect().height());
}

QPoint FieldView::cellPosition(const QPoint& point)
{
    return QPoint(point.x() / 50, point.y() / 50);
}

bool FieldView::isShipsCrossed(QGraphicsRectItem *targetShip, QGraphicsRectItem *ship)
{
    std::vector<QPoint> targetPoints;

    //if vertical pos
    if ((int)targetShip->rect().width() / CELL_SIZE == 1)
    {
        //3 vertical lines
        for (int i = -1; i < targetShip->rect().height() / CELL_SIZE + 1; ++i)
        {
            targetPoints.push_back(QPoint(targetShip->rect().x() - CELL_SIZE, targetShip->rect().y() + CELL_SIZE * i));
            targetPoints.push_back(QPoint(targetShip->rect().x(), targetShip->rect().y() + CELL_SIZE * i));
            targetPoints.push_back(QPoint(targetShip->rect().x() + CELL_SIZE, targetShip->rect().y() + CELL_SIZE * i));
        }
    }
    else//if horizontal pos
    {
        //3 horizontal lines
        for (int i = -1; i < targetShip->rect().width() / CELL_SIZE + 1; ++i)
        {
            targetPoints.push_back(QPoint(targetShip->rect().x() + CELL_SIZE * i, targetShip->rect().y() - CELL_SIZE));
            targetPoints.push_back(QPoint(targetShip->rect().x() + CELL_SIZE * i, targetShip->rect().y()));
            targetPoints.push_back(QPoint(targetShip->rect().x() + CELL_SIZE * i, targetShip->rect().y() + CELL_SIZE));
        }
    }


    std::vector<QPoint> shipPoints;
    //if ship is vertical pos
    if ((int)ship->rect().width() / CELL_SIZE == 1)
    {
        for (int i = 0; i < ship->rect().height() / CELL_SIZE; ++i)
        {
            shipPoints.push_back(QPoint(ship->rect().x(), ship->rect().y() + CELL_SIZE * i));
        }
    }
    else//if ship is horizontal pos
    {
        for (int i = 0; i < ship->rect().width() / CELL_SIZE; ++i)
        {
            shipPoints.push_back(QPoint(ship->rect().x() + CELL_SIZE * i, ship->rect().y()));
        }
    }


    for (std::vector<QPoint>::size_type i = 0; i < targetPoints.size(); ++i)
    {
        for (std::vector<QPoint>::size_type j = 0; j < shipPoints.size(); ++j)
        {
            if (targetPoints.at(i) == shipPoints.at(j))
            {
                return true;
            }
        }
    }

    return false;
}

bool FieldView::isShipPrimaryPos(QGraphicsRectItem *ship)
{
    if (ship)
    {
        int width = ship->rect().width() > ship->rect().height() ? ship->rect().width() : ship->rect().height();
        QGraphicsRectItem* shipPos = mShipsPosition.at(width / CELL_SIZE - 1);

        if (shipPos->rect().x() == ship->rect().x() && shipPos->rect().y() == ship->rect().y())
        {
            return true;
        }
    }

    return false;
}

void FieldView::rotateShip(QGraphicsRectItem *ship)
{
    ship->setRect(ship->rect().x(), ship->rect().y(),
                          ship->rect().height(), ship->rect().width());
}

bool FieldView::isShipOut(QGraphicsRectItem *ship)
{
    if (mCurrentItem)
    {
        std::vector<QPoint> points;

        //if vertical pos
        if ((int)ship->rect().width() / CELL_SIZE == 1)
        {
            for (int i = 0; i < ship->rect().height() / CELL_SIZE; ++i)
            {
                points.push_back(QPoint(ship->rect().x(), ship->rect().y() + CELL_SIZE * i));
            }
        }
        else//if horizontal pos
        {
            for (std::vector<QPoint>::size_type i = 0; i < ship->rect().width() / CELL_SIZE; ++i)
            {
                points.push_back(QPoint(ship->rect().x() + CELL_SIZE * i, ship->rect().y()));
            }
        }


        for (std::vector<QPoint>::size_type i = 0; i < points.size(); ++i)
        {
            if (points.at(i).x() >= FIELD_SIZE * CELL_SIZE || points.at(i).y() >= FIELD_SIZE * CELL_SIZE)
            {
                return true;
            }
        }
    }

    return false;
}

void FieldView::setBattleState()
{
    mScene = new QGraphicsScene;
    mScene->setSceneRect(0, 0, (FIELD_SIZE + 0)* CELL_SIZE, FIELD_SIZE * CELL_SIZE);

    this->setScene(mScene);
    this->setFixedSize(mScene->width(), mScene->height());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initField();
    initShips();
}

void FieldView::copyState(const FieldView * const fieldView)
{
    std::for_each(fieldView->mShips.begin(), fieldView->mShips.end(), [this](QGraphicsRectItem* item)
    {
        QGraphicsRectItem* nItem = new QGraphicsRectItem(item->rect());
        nItem->setBrush(Qt::red);
        this->mShips.push_back(nItem);
        this->mScene->addItem(nItem);
    });
}

void FieldView::onButtonRotateClicked()
{
    if (mCurrentItem)
    {
        if (!isShipPrimaryPos(mCurrentItem))
        {
            rotateShip(mCurrentItem);

            if (isShipOut(mCurrentItem))
            {
                rotateShip(mCurrentItem);
            }

            for (int i = 0; i < mShips.size(); ++i)
            {
                QGraphicsRectItem *ship = mShips.at(i);

                if (ship != mCurrentItem)
                {
                    if (isShipsCrossed(mCurrentItem, ship))
                    {
                        rotateShip(mCurrentItem);
                        return;
                    }
                }
            }
        }
    }
}

void FieldView::onShoot(const QPoint &coordinate)
{
    QPixmap pixmap;

    switch (mFieldModel->getCellValue(coordinate.x(), coordinate.y()))
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

    pixmap = pixmap.scaled(CELL_SIZE,CELL_SIZE);
    mCells[coordinate.x()][coordinate.y()]->setPixmap(pixmap);

}
