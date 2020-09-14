#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QFile>
#include <QDebug>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

#include "fieldmodel.h"
#include "fieldview.h"


class FieldWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FieldWidget(QWidget *parent = nullptr);

private:
    void initFieldView();
    void initBackButton();

private:
    QVBoxLayout* mMainLayout;
    QGridLayout* mFieldLayout;
    QPushButton* mButtonBack;

    QGraphicsScene* mScene;

    FieldModel* mModel;

private slots:
    void onBackButtonClicked();

signals:
    void buttonBackClicked();
};

#endif // FIELDWIDGET_H
