#include "fieldwidget.h"

FieldWidget::FieldWidget(QWidget *parent) : QWidget(parent)
{
    mMainLayout = new QVBoxLayout;
    mMainLayout->setAlignment(Qt::AlignCenter);

    mModel = new FieldModel;

    initFieldView();
    initBackButton();

    this->setLayout(mMainLayout);
}

void FieldWidget::initFieldView()
{
    FieldView* view = new FieldView;
    mMainLayout->addWidget(view);
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
}
