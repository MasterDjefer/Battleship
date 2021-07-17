#include "fieldwidget.h"

void FieldWidget::setEnabledCreateServerButton(bool enabled)
{
    mButtonCreateServer->setVisible(enabled);
}

bool FieldWidget::enabledCreateServerButton()
{
    return mButtonCreateServer->isVisible();
}

void FieldWidget::setEnabledConnectToServerButton(bool enabled)
{
    mButtonConnectToServer->setVisible(enabled);
}

bool FieldWidget::enabledConnectToServerButton()
{
    return mButtonConnectToServer->isVisible();
}

FieldWidget::FieldWidget(QWidget *parent) : QWidget(parent)
{
    mMainLayout = new QVBoxLayout;
    mNavigationLayout = new QVBoxLayout;
    mNavigationLayout->setAlignment(Qt::AlignTop);
    mFieldLayout = new QHBoxLayout;

    initFieldView();
    initRotateButton();
    initServerButtons();
    mFieldLayout->addLayout(mNavigationLayout);
    mMainLayout->addLayout(mFieldLayout);
    initBackButton();

    mMainLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(mMainLayout);

    QObject::connect(mEnemyView, &FieldView::shoot, this, &FieldWidget::onShootFromSignal);
}

void FieldWidget::copyState(const FieldWidget * const fieldWidget)
{
    mView->copyState(fieldWidget->mView);
}

void FieldWidget::changePlayModeView()
{
    mView->changeMode(FieldView::SelfMode);

    mEnemyView->changeMode(FieldView::EnemyMode);
    mEnemyView->setVisible(true);
}

void FieldWidget::initFieldView()
{
    mView = new FieldView;
    mFieldLayout->addWidget(mView);

    //added enemy view
    mEnemyView = new FieldView;
    mEnemyView->setVisible(false);
    mFieldLayout->addWidget(mEnemyView);
}

void FieldWidget::initRotateButton()
{
    mButtonRotate = new QPushButton("Rotate");
    mButtonRotate->setFixedSize(50,50);
    QObject::connect(mButtonRotate, &QPushButton::clicked, mView, &FieldView::onButtonRotateClicked);

    mNavigationLayout->addWidget(mButtonRotate);
}

void FieldWidget::initServerButtons()
{
    mButtonCreateServer = new QPushButton("Create");
    mButtonCreateServer->setFixedSize(50,50);
    QObject::connect(mButtonCreateServer, &QPushButton::clicked, this, &FieldWidget::onButtonCreateServerClicked);
    mNavigationLayout->addWidget(mButtonCreateServer);

    mButtonConnectToServer = new QPushButton("Connect");
    mButtonConnectToServer->setFixedSize(50,50);
    QObject::connect(mButtonConnectToServer, &QPushButton::clicked, this, &FieldWidget::onButtonConnectToServerClicked);
    mNavigationLayout->addWidget(mButtonConnectToServer);
}

void FieldWidget::initBackButton()
{
    mButtonBack = new QPushButton;
    mButtonBack->setObjectName("BackImageButton");
    mButtonBack->setFixedSize(50,50);
    QFile file(":/styles/CustomButton.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    mButtonBack->setStyleSheet(styleSheet);

    QObject::connect(mButtonBack, &QPushButton::clicked, this, &FieldWidget::onBackButtonClicked);

    mMainLayout->addWidget(mButtonBack);
}

void FieldWidget::onBackButtonClicked()
{
    emit buttonBackClicked();
    mView->resetShips();
}

void FieldWidget::onButtonCreateServerClicked()
{
    //if (mView->isFieldReady())
        emit buttonCreateServerClicked();
}

void FieldWidget::onButtonConnectToServerClicked()
{
    //if (mView->isFieldReady())
    emit buttonConnectToServerClicked();
}

void FieldWidget::onShoot(const QPoint &coordinates)
{
    mView->onShoot(coordinates);
}

void FieldWidget::onShootFromSignal(const QPoint &coordinates)
{
    emit shoot(coordinates);
}
