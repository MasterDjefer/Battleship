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
    Q_PROPERTY(bool enabledRotateButton READ enabledRotateButton WRITE setEnabledRotateButton);
    void setEnabledRotateButton(bool enabled);
    bool enabledRotateButton();
    Q_PROPERTY(bool enabledConnectToServerButton READ enabledConnectToServerButton WRITE setEnabledConnectToServerButton);
    void setEnabledConnectToServerButton(bool enabled);
    bool enabledConnectToServerButton();
    Q_PROPERTY(bool enabledDisconnectButton READ enabledDisconnectButton WRITE setEnabledDisconnectButton);
    void setEnabledDisconnectButton(bool enabled);
    bool enabledDisconnectButton();

public:
    explicit FieldWidget(QWidget *parent = nullptr);
    void copyState(const FieldWidget* const fieldWidget);

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
    QPushButton* mButtonDisconnect;

    FieldView* mView;
    FieldView* mEnemyView;

private slots:
    void onBackButtonClicked();
    void onButtonCreateServerClicked();
    void onButtonConnectToServerClicked();
    void onButtonDisconnectClicked();
    void onShootFromSignal(const QPoint& coordinates);

public slots:
    void onShoot(const QPoint& coordinates);
    void changePlayModeView();

signals:
    void buttonBackClicked();
    void buttonCreateServerClicked();
    void buttonConnectToServerClicked();
    void buttonDisconnectClicked();
    void shoot(const QPoint& coordinates);
};

#endif // FIELDWIDGET_H
