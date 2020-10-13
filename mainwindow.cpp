#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    mStackedLayout = new QStackedLayout;
    this->setLayout(mStackedLayout);
    QColor color(236, 229, 227);
    QPalette palette(color);
    setAutoFillBackground(true);
    this->setPalette(palette);

    initMainMenu();
    initGameModeMenu();
    initFieldWidget();
    initWaitWidget();
    initStates();

    QObject::connect(mFieldWidget, &FieldWidget::buttonFinishClicked, this, &MainWindow::onFinishButtonClicked);
    QObject::connect(mWaitWidget, &WaitWidget::buttonBackClicked, this, &MainWindow::onBackToMapButtonClicked);
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
    mButtonExit = new MenuButton;
    mButtonExit->setText(mButtonTitles[Exit]);
    QObject::connect(mButtonExit, &MenuButton::clicked, this, &MainWindow::close);

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

    mButtonCreateGame = new MenuButton;
    mButtonCreateGame->setText(mButtonTitles[CreateGame]);
    MenuButton* mButtonConnectToGame = new MenuButton;
    mButtonConnectToGame->setText(mButtonTitles[ConnectToGame]);
    mButtonBack1 = new MenuButton;
    mButtonBack1->setText(mButtonTitles[Back]);

    mGameModeMenu->addWidget(mButtonCreateGame);
    mGameModeMenu->addWidget(mButtonConnectToGame);
    mGameModeMenu->addWidget(mButtonBack1);

    mGameModeMenu->setAlignment(Qt::AlignHCenter);

    QWidget *w = new QWidget;
    w->setLayout(mGameModeMenu);
    mStackedLayout->addWidget(w);
}

void MainWindow::initFieldWidget()
{
    mFieldWidget = new FieldWidget;
    mStackedLayout->addWidget(mFieldWidget);
}

void MainWindow::initWaitWidget()
{
    mWaitWidget = new WaitWidget;

    QVBoxLayout* l = new QVBoxLayout;
    l->setAlignment(Qt::AlignCenter);
    l->addWidget(mWaitWidget);
    QWidget* w = new QWidget;
    w->setLayout(l);
    mStackedLayout->addWidget(w);
}

void MainWindow::initStates()
{
    mStateMachine = new QStateMachine(this);

    QState *state1 = new QState(mStateMachine);
    QState *state2 = new QState(mStateMachine);
    QState *state3 = new QState(mStateMachine);
    QState *state4 = new QState(mStateMachine);

    state1->assignProperty(mStackedLayout, "currentIndex", 0);
    state1->addTransition(mButtonNewGame, &MenuButton::clicked, state2);

    state2->assignProperty(mStackedLayout, "currentIndex", 1);
    state2->addTransition(mButtonBack1, &MenuButton::clicked, state1);
    state2->addTransition(mButtonCreateGame, &MenuButton::clicked, state3);

    state3->assignProperty(mStackedLayout, "currentIndex", 2);
    state3->addTransition(mFieldWidget, &FieldWidget::buttonBackClicked, state2);
    state3->addTransition(mFieldWidget, &FieldWidget::buttonFinishClicked, state4);

    state4->assignProperty(mStackedLayout, "currentIndex", 3);
    state4->addTransition(mWaitWidget, &WaitWidget::buttonBackClicked, state3);

    mStateMachine->setInitialState(state1);
    mStateMachine->start();
}

void MainWindow::onFinishButtonClicked()
{
    mServer = new Server;
    mServer->startServer();
}

void MainWindow::onBackToMapButtonClicked()
{
    delete mServer;
}

const char* MainWindow::mButtonTitles[] = {"New game", "Settings", "About", "Exit", "Create game", "Connect to game", "Back"};
