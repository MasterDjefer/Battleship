#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include <QFile>
#include <QDebug>

class MenuButton : public QPushButton
{
    Q_OBJECT

public:
    MenuButton(QWidget* parent = nullptr);
};

#endif // CUSTOMBUTTON_H
