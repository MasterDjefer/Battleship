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
    void initRotateButton();
    void initFinishButton();
    void initBackButton();

private:
    QVBoxLayout* mMainLayout;
    QVBoxLayout* mNavigationLayout;
    QHBoxLayout* mFieldLayout;

    QPushButton* mButtonRotate;
    QPushButton* mButtonFinish;
    QPushButton* mButtonBack;

    FieldView* mView;

private slots:
    void onBackButtonClicked();
    void onButtonFinishClicked();

signals:
    void buttonBackClicked();
    void buttonFinishClicked();
};

#endif // FIELDWIDGET_H
