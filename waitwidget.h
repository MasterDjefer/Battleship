#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QFont>
#include <QProgressDialog>
#include <QPushButton>

class WaitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaitWidget(QWidget *parent = nullptr);

private:
    QWidget* mMainWidget;
    QProgressDialog* mProgressDialog;
    QLabel* mLabelWaitingMsg;
    QVBoxLayout* mMainLayout;
    QPushButton* mButtonBack;
    QVBoxLayout* mWaitingLayout;

private:
    void initProgresDialog();
    void initLabel();
    void initButtonBack();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onButtonBackClicked();

signals:
    void buttonBackClicked();
};

#endif // WAITWIDGET_H
