#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    MenuButton* mButton1 = new MenuButton;
    mButton1->setText("Jager");
    MenuButton* mButton2 = new MenuButton;
    mButton2->setText("Becher");
    MenuButton* mButton3 = new MenuButton;
    mButton3->setText("Sadochok");

    QVBoxLayout* mMainLayout = new QVBoxLayout;
    mMainLayout->addWidget(mButton1);
    mMainLayout->addWidget(mButton2);
    mMainLayout->addWidget(mButton3);
    mMainLayout->setAlignment(Qt::AlignHCenter);

    this->setLayout(mMainLayout);
}

MainWindow::~MainWindow()
{
}

