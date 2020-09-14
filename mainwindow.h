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
    void initStates();

private:
    QStackedLayout *mStackedLayout;

    QVBoxLayout* mMainMenu;
    MenuButton* mButtonNewGame;
    MenuButton* mButtonExit;

    QVBoxLayout* mGameModeMenu;
    MenuButton* mButtonCreateGame;
    MenuButton* mButtonBack1;

    FieldWidget* mFieldWidget;

    QStateMachine* mStateMachine;

    static const char* mButtonTitles[];
};
#endif // WIDGET_H
