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
    Q_PROPERTY(bool enabledCreateServerButton READ enabledCreateServerButton WRITE setEnabledCreateServerButton);
    void setEnabledCreateServerButton(bool enabled);
    bool enabledCreateServerButton();
    Q_PROPERTY(bool enabledConnectToServerButton READ enabledConnectToServerButton WRITE setEnabledConnectToServerButton);
    void setEnabledConnectToServerButton(bool enabled);
    bool enabledConnectToServerButton();

public:
    explicit FieldWidget(QWidget *parent = nullptr);

private:
    void initFieldView();
    void initRotateButton();
    void initServerButtons();
    void initBackButton();

private:
    QVBoxLayout* mMainLayout;
    QVBoxLayout* mNavigationLayout;
    QHBoxLayout* mFieldLayout;

    QPushButton* mButtonRotate;
    QPushButton* mButtonCreateServer;
    QPushButton* mButtonConnectToServer;
    QPushButton* mButtonBack;

    FieldView* mView;

private slots:
    void onBackButtonClicked();
    void onButtonCreateServerClicked();
    void onButtonConnectToServerClicked();
    void onShootFromSignal(const QPoint& coordinates);

public slots:
    void onShoot(const QPoint& coordinates);

signals:
    void buttonBackClicked();
    void buttonCreateServerClicked();
    void buttonConnectToServerClicked();
    void shoot(const QPoint& coordinates);
};

#endif // FIELDWIDGET_H
