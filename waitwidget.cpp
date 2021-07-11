#include "waitwidget.h"

WaitWidget::WaitWidget(QWidget *parent) : QWidget(parent)
{
    mMainWidget = new QWidget;
    mMainWidget->setFixedSize(400, 300);

    initProgresDialog();
    initLabel();
    initButtonBack();

    mWaitingLayout = new QVBoxLayout;
    mWaitingLayout->addWidget(mProgressDialog);
    mWaitingLayout->addWidget(mLabelWaitingMsg, 0, Qt::AlignBottom);
    mWaitingLayout->setAlignment(Qt::AlignHCenter);

    mMainWidget->setLayout(mWaitingLayout);

    mMainLayout = new QVBoxLayout;
    mMainLayout->addWidget(mMainWidget);
    mMainLayout->addWidget(mButtonBack, 0, Qt::AlignHCenter);

    this->setLayout(mMainLayout);
}

void WaitWidget::initProgresDialog()
{
    mProgressDialog = new QProgressDialog;
    mProgressDialog->setRange(0, 0);
    mProgressDialog->setCancelButton(nullptr);
}

void WaitWidget::initLabel()
{
    mLabelWaitingMsg = new QLabel();
    QFont font = mLabelWaitingMsg->font();
    font.setPointSize(18);
    mLabelWaitingMsg->setFont(font);
    mLabelWaitingMsg->setText("Waiting for the client...");
}

void WaitWidget::initButtonBack()
{
    mButtonBack = new QPushButton;
    mButtonBack->setText("Back");
    mButtonBack->setFixedSize(50,50);
    QObject::connect(mButtonBack, &QPushButton::clicked, this, &WaitWidget::onButtonBackClicked);
}

void WaitWidget::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);
    painter.setBrush(Qt::blue);
    painter.drawRect(0, 0, mMainWidget->width(), mMainWidget->height());
}

void WaitWidget::onButtonBackClicked()
{
    emit buttonBackClicked();
}
