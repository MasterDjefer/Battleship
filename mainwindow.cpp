#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    mStackedLayout = new QStackedLayout;
    this->setLayout(mStackedLayout);
    initMainMenu();
    initGameModeMenu();
    initStates();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMainMenu()
{
    mMainMenu = new QVBoxLayout;

    mButtonNewGame = new MenuButton;
    mButtonNewGame->setText(mButtonTitles[NewGame]);
    MenuButton* mButtonSettings = new MenuButton;
    mButtonSettings->setText(mButtonTitles[Settings]);
    MenuButton* mButtonAbout = new MenuButton;
    mButtonAbout->setText(mButtonTitles[About]);
    MenuButton* mButtonExit = new MenuButton;
    mButtonExit->setText(mButtonTitles[Exit]);

    mMainMenu->addWidget(mButtonNewGame);
    mMainMenu->addWidget(mButtonSettings);
    mMainMenu->addWidget(mButtonAbout);
    mMainMenu->addWidget(mButtonExit);

    mMainMenu->setAlignment(Qt::AlignHCenter);

    QWidget *w = new QWidget;
    w->setLayout(mMainMenu);
    mStackedLayout->addWidget(w);
}

void MainWindow::initGameModeMenu()
{
    mGameModeMenu = new QVBoxLayout;

    MenuButton* mButtonCreateGame = new MenuButton;
    mButtonCreateGame->setText(mButtonTitles[CreateGame]);
    MenuButton* mButtonConnectToGame = new MenuButton;
    mButtonConnectToGame->setText(mButtonTitles[ConnectToGame]);

    mGameModeMenu->addWidget(mButtonCreateGame);
    mGameModeMenu->addWidget(mButtonConnectToGame);

    mGameModeMenu->setAlignment(Qt::AlignHCenter);

    QWidget *w = new QWidget;
    w->setLayout(mGameModeMenu);
    mStackedLayout->addWidget(w);
}

void MainWindow::initStates()
{
    mStateMachine = new QStateMachine(this);

    QState *state1 = new QState(mStateMachine);
    QState *state2 = new QState(mStateMachine);

    state1->assignProperty(mStackedLayout, "currentIndex", 0);
    state1->addTransition(mButtonNewGame, &MenuButton::clicked, state2);

    state2->assignProperty(mStackedLayout, "currentIndex", 1);

    mStateMachine->setInitialState(state1);
    mStateMachine->start();
}

const char* MainWindow::mButtonTitles[] = {"New game", "Settings", "About", "Exit", "Create game", "Connect to game"};
