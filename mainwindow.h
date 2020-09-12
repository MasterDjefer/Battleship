#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QStackedLayout>

#include <QState>
#include <QStateMachine>

#include "menubutton.h"

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
        ConnectToGame
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initMainMenu();
    void initGameModeMenu();
    void initStates();

private:
    QStackedLayout *mStackedLayout;

    QVBoxLayout* mMainMenu;
    MenuButton* mButtonNewGame;

    QVBoxLayout* mGameModeMenu;
    QVBoxLayout* mSettingsMenu;

    QStateMachine* mStateMachine;

    static const char* mButtonTitles[];
};
#endif // WIDGET_H
