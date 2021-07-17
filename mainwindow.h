#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedLayout>
#include <QPropertyAnimation>
#include <QPalette>
#include <QRgb>

#include <QState>
#include <QStateMachine>

#include "fieldwidget.h"
#include "menubutton.h"
#include "waitwidget.h"

#include "server.h"
#include "client.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    enum
    {
        NewGame = 0,
        Settings,
        About,
        Exit,
        CreateGame,
        ConnectToGame,
        Back
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initMainMenu();
    void initGameModeMenu();
    void initFieldWidget();
    void initWaitWidget();
    void initPlayModeWidget();
    void initStates();
    void initServer();
    void initClient();
    void showConnectErrorMsg();

private:
    QStackedLayout *mStackedLayout;

    QVBoxLayout* mMainMenu;
    MenuButton* mButtonNewGame;
    MenuButton* mButtonExit;

    QVBoxLayout* mGameModeMenu;
    MenuButton* mButtonCreateGame;
    MenuButton* mButtonConnectToGame;
    MenuButton* mButtonBack1;

    FieldWidget* mFieldWidget;
    FieldWidget* mPlayModeWidget;
    WaitWidget* mWaitWidget;

    QStateMachine* mStateMachine;

    Server* mServer;
    Client* mClient;

    static const char* mButtonTitles[];

private slots:
    void onCreateServerButtonClicked();
    void onBackToMapButtonClicked();
    void onConnectToServerButtonClicked();
    void onShootServerSend(const QPoint& coordinates);
    void onShootClientSend(const QPoint& coordinates);
};
#endif // WIDGET_H
