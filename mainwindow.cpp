#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), mServer(nullptr), mClient(nullptr)
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
    mButtonConnectToGame = new MenuButton;
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

    QState *stateInit = new QState(mStateMachine);
    QState *stateGameMenu = new QState(mStateMachine);
    QState *stateFieldServer = new QState(mStateMachine);
    QState *stateFieldClient = new QState(mStateMachine);
    QState *stateWaitForClient = new QState(mStateMachine);
    QState *stateConnectToServer = new QState(mStateMachine);

    stateInit->assignProperty(mStackedLayout, "currentIndex", 0);
    stateInit->addTransition(mButtonNewGame, &MenuButton::clicked, stateGameMenu);

    stateGameMenu->assignProperty(mStackedLayout, "currentIndex", 1);
    stateGameMenu->addTransition(mButtonBack1, &MenuButton::clicked, stateInit);
    stateGameMenu->addTransition(mButtonCreateGame, &MenuButton::clicked, stateFieldServer);
    stateGameMenu->addTransition(mButtonConnectToGame, &MenuButton::clicked, stateFieldClient);

    stateFieldServer->assignProperty(mStackedLayout, "currentIndex", 2);
    stateFieldServer->assignProperty(mFieldWidget, "enabledCreateServerButton", true);
    stateFieldServer->assignProperty(mFieldWidget, "enabledConnectToServerButton", false);
    stateFieldServer->addTransition(mFieldWidget, &FieldWidget::buttonBackClicked, stateGameMenu);
    stateFieldServer->addTransition(mFieldWidget, &FieldWidget::buttonCreateServerClicked, stateWaitForClient);

    stateWaitForClient->assignProperty(mStackedLayout, "currentIndex", 3);
    stateWaitForClient->addTransition(mWaitWidget, &WaitWidget::buttonBackClicked, stateFieldServer);
    QObject::connect(stateWaitForClient, &QState::entered, this, &MainWindow::onCreateServerButtonClicked);
    QObject::connect(stateFieldServer, &QState::entered, this, &MainWindow::onBackToMapButtonClicked);

    stateFieldClient->assignProperty(mStackedLayout, "currentIndex", 2);
    stateFieldClient->assignProperty(mFieldWidget, "enabledConnectToServerButton", true);
    stateFieldClient->assignProperty(mFieldWidget, "enabledCreateServerButton", false);
    stateFieldClient->addTransition(mFieldWidget, &FieldWidget::buttonBackClicked, stateGameMenu);
//    stateFieldClient->addTransition(mFieldWidget, &FieldWidget::buttonConnectToServerClicked, stateConnectToServer);
//    QObject::connect(stateConnectToServer, &QState::entered, this, &MainWindow::onConnectToServerButtonClicked);
    QObject::connect(mFieldWidget, &FieldWidget::buttonConnectToServerClicked, this, &MainWindow::onConnectToServerButtonClicked);

    mStateMachine->setInitialState(stateInit);
    mStateMachine->start();
}

void MainWindow::showConnectErrorMsg()
{
    QMessageBox msgBox;
    msgBox.setText("Connect error");
    msgBox.exec();
}

void MainWindow::onCreateServerButtonClicked()
{
    mServer = new Server;
    QObject::connect(mServer, &Server::coordinatesReceived, this, &MainWindow::onCoordinatesReceived);
    mServer->startServer();

    eSide = ServerSide;

    //connect fieldview::shoot to server::sendMsg
    //connect server::coordinatesReceived to fieldview::onShoot
}

void MainWindow::onBackToMapButtonClicked()
{
    if (mServer)
    {
        delete mServer;
        mServer = nullptr;

        eSide = NoSide;
    }
}

void MainWindow::onConnectToServerButtonClicked()
{
    mClient = new Client;
    if (!mClient->connectToServer())
    {
        showConnectErrorMsg();
        delete mClient;
    }else
    {
        //connect fieldview::shoot to client::sendMsg
        //connect client::coordinatesReceived to fieldview::onShoot
        eSide = ClientSide;
    }
}

void MainWindow::onCoordinatesReceived(const QPoint &coordinates)
{
    qDebug() << "HERE: x=" << coordinates.x() << ", y=" << coordinates.y();
}

const char* MainWindow::mButtonTitles[] = {"New game", "Settings", "About", "Exit", "Create game", "Connect to game", "Back"};
