#include "fieldwidget.h"

FieldWidget::FieldWidget(QWidget *parent) : QWidget(parent)
{
    mMainLayout = new QVBoxLayout;
    mNavigationLayout = new QVBoxLayout;
    mNavigationLayout->setAlignment(Qt::AlignTop);
    mFieldLayout = new QHBoxLayout;

    initFieldView();
    initRotateButton();
    initFinishButton();
    mFieldLayout->addLayout(mNavigationLayout);
    mMainLayout->addLayout(mFieldLayout);
    initBackButton();

    mMainLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(mMainLayout);
}

void FieldWidget::initFieldView()
{
    mView = new FieldView;
    mFieldLayout->addWidget(mView);
}

void FieldWidget::initRotateButton()
{
    mButtonRotate = new QPushButton("Rotate");
    mButtonRotate->setFixedSize(50,50);
    QObject::connect(mButtonRotate, &QPushButton::clicked, mView, &FieldView::onButtonRotateClicked);

    mNavigationLayout->addWidget(mButtonRotate);
}

void FieldWidget::initFinishButton()
{
    mButtonFinish = new QPushButton("Finish");
    mButtonFinish->setFixedSize(50,50);
    QObject::connect(mButtonFinish, &QPushButton::clicked, this, &FieldWidget::onButtonFinishClicked);

    mNavigationLayout->addWidget(mButtonFinish);
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

void FieldWidget::onButtonFinishClicked()
{
    //if (mView->isFieldReady())
        emit buttonFinishClicked();
}
