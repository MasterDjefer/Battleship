#include "fieldview.h"

FieldView::FieldView()
{
    mFieldModel = new FieldModel;

    mScene = new QGraphicsScene;
    mScene->setSceneRect(0, 0, 500, 500);

    this->setScene(mScene);
    this->setFixedSize(500, 500);
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
//            temp->setBrush(Qt::red);
            temp->setPos(50 * i, 50 * j);
            mScene->addItem(temp);
            mCells[i][j] = temp;


//            QPushButton* cell = new QPushButton;
//            FieldModel::CellState state = mModel->getCellValue(i, j);
//            QObject::connect(cell, &QPushButton::clicked, [&cell, state]()
//            {
//                switch (state)
//                {
//                case FieldModel::CellState::Empty:
//                    cell->setText("-");
//                    break;
//                case FieldModel::CellState::Hurted:
//                    cell->setText("X");
//                    break;
//                case FieldModel::CellState::Killed:
//                    cell->setText("O");
//                    break;
//                case FieldModel::CellState::Ship:
//                    cell->setText("A");
//                    break;
//                default:
//                    break;
//                }
//            });

//            cell->setFixedSize(50,50);
//            mFieldLayout->addWidget(cell, i, j);
        }
    }
}

void FieldView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int i = event->pos().x() / 50;
        int j = event->pos().y() / 50;
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
