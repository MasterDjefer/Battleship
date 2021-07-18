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
    initPlayModeWidget();
    initServer();
    initClient();
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
    mButtonBack = new MenuButton;
    mButtonBack->setText(mButtonTitles[Back]);

    mGameModeMenu->addWidget(mButtonCreateGame);
    mGameModeMenu->addWidget(mButtonConnectToGame);
    mGameModeMenu->addWidget(mButtonBack);

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

void MainWindow::initPlayModeWidget()
{
    mPlayModeWidget = new FieldWidget;
    mStackedLayout->addWidget(mPlayModeWidget);
}

void MainWindow::initStates()
{
    mStateMachine = new QStateMachine(this);

    QState *stateInit = new QState(mStateMachine);
    QState *stateGameMenu = new QState(mStateMachine);
    QState *stateFieldServer = new QState(mStateMachine);
    QState *stateFieldClient = new QState(mStateMachine);
    QState *stateWaitForClient = new QState(mStateMachine);
    QState *stateFieldPlayMode = new QState(mStateMachine);

    stateInit->assignProperty(mStackedLayout, "currentIndex", 0);
    stateInit->addTransition(mButtonNewGame, &MenuButton::clicked, stateGameMenu);

    stateGameMenu->assignProperty(mStackedLayout, "currentIndex", 1);
    stateGameMenu->addTransition(mButtonBack, &MenuButton::clicked, stateInit);
    stateGameMenu->addTransition(mButtonCreateGame, &MenuButton::clicked, stateFieldServer);
    stateGameMenu->addTransition(mButtonConnectToGame, &MenuButton::clicked, stateFieldClient);

    stateFieldServer->assignProperty(mStackedLayout, "currentIndex", 2);
    stateFieldServer->assignProperty(mFieldWidget, "enabledRotateButton", true);
    stateFieldServer->assignProperty(mFieldWidget, "enabledCreateServerButton", true);
    stateFieldServer->assignProperty(mFieldWidget, "enabledConnectToServerButton", false);
    stateFieldServer->assignProperty(mFieldWidget, "enabledDisconnectButton", false);
    stateFieldServer->addTransition(mFieldWidget, &FieldWidget::buttonBackClicked, stateGameMenu);
    stateFieldServer->addTransition(mFieldWidget, &FieldWidget::buttonCreateServerClicked, stateWaitForClient);

    stateWaitForClient->assignProperty(mStackedLayout, "currentIndex", 3);
    stateWaitForClient->addTransition(mWaitWidget, &WaitWidget::buttonBackClicked, stateFieldServer);
    stateWaitForClient->addTransition(mServer, &Server::connectionAccepted, stateFieldPlayMode);
    QObject::connect(mWaitWidget, &WaitWidget::buttonBackClicked, this, &MainWindow::onBackToMapButtonClicked);
    QObject::connect(stateWaitForClient, &QState::entered, this, &MainWindow::onCreateServerButtonClicked);

    stateFieldClient->assignProperty(mStackedLayout, "currentIndex", 2);
    stateFieldClient->assignProperty(mFieldWidget, "enabledRotateButton", true);
    stateFieldClient->assignProperty(mFieldWidget, "enabledConnectToServerButton", true);
    stateFieldClient->assignProperty(mFieldWidget, "enabledCreateServerButton", false);
    stateFieldClient->assignProperty(mFieldWidget, "enabledDisconnectButton", false);
    stateFieldClient->addTransition(mFieldWidget, &FieldWidget::buttonBackClicked, stateGameMenu);
    stateFieldClient->addTransition(mClient, &Client::connectedToServer, stateFieldPlayMode);
    QObject::connect(mFieldWidget, &FieldWidget::buttonConnectToServerClicked, this, &MainWindow::onConnectToServerButtonClicked);

    stateFieldPlayMode->assignProperty(mStackedLayout, "currentIndex", 4);
    stateFieldPlayMode->assignProperty(mPlayModeWidget, "enabledRotateButton", false);
    stateFieldPlayMode->assignProperty(mPlayModeWidget, "enabledConnectToServerButton", false);
    stateFieldPlayMode->assignProperty(mPlayModeWidget, "enabledCreateServerButton", false);
    stateFieldPlayMode->assignProperty(mPlayModeWidget, "enabledDisconnectButton", true);
    stateFieldPlayMode->addTransition(mServer, &Server::serverStopped, stateGameMenu);
    stateFieldPlayMode->addTransition(mClient, &Client::disconnected, stateGameMenu);
    QObject::connect(mPlayModeWidget, &FieldWidget::buttonDisconnectClicked, this, &MainWindow::onDisconnectButtonClicked);

    mStateMachine->setInitialState(stateInit);
    mStateMachine->start();
}

void MainWindow::initServer()
{
    mServer = new Server;
    QObject::connect(mServer, &Server::coordinatesReceived, mPlayModeWidget, &FieldWidget::onShoot);
}

void MainWindow::initClient()
{
    mClient = new Client;
    QObject::connect(mClient, &Client::coordinatesReceived, mPlayModeWidget, &FieldWidget::onShoot);
}

void MainWindow::showWidgetMsg(const QString &errorMsg)
{
    QMessageBox msgBox;
    msgBox.setText(errorMsg);
    msgBox.exec();
}

void MainWindow::onCreateServerButtonClicked()
{
    mServer->startServer();

    mPlayModeWidget->copyState(mFieldWidget);
    mPlayModeWidget->changePlayModeView();

    QObject::connect(mPlayModeWidget, &FieldWidget::shoot, this, &MainWindow::onShootServerSend);
}

void MainWindow::onBackToMapButtonClicked()
{
    mServer->stopServer();
}

void MainWindow::onConnectToServerButtonClicked()
{
    if (!mClient->connectToServer())
    {
        showWidgetMsg("Connection error");
    }else
    {
        mPlayModeWidget->copyState(mFieldWidget);
        mPlayModeWidget->changePlayModeView();

        QObject::connect(mPlayModeWidget, &FieldWidget::shoot, this, &MainWindow::onShootClientSend);
    }
}

void MainWindow::onDisconnectButtonClicked()
{
    if (mServer->isServerRunning())
    {
        showWidgetMsg("Server will be stopped");
        mServer->stopServer();
    }
    else
    if (mClient->isClientConnected())
    {
        showWidgetMsg("You will be disconnected from server");
        mClient->disconnect();
    }
}

void MainWindow::onShootServerSend(const QPoint &coordinates)
{
    std::string sBuffer = std::to_string(coordinates.x()) + ";" + std::to_string(coordinates.y());
    mServer->sendMsg(sBuffer.c_str());
}

void MainWindow::onShootClientSend(const QPoint &coordinates)
{
    std::string sBuffer = std::to_string(coordinates.x()) + ";" + std::to_string(coordinates.y());
    mClient->sendMsg(sBuffer.c_str());
}

const char* MainWindow::mButtonTitles[] = {"New game", "Settings", "About", "Exit", "Create game", "Connect to game", "Back"};
