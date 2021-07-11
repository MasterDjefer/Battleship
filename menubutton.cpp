#include "menubutton.h"

MenuButton::MenuButton(QWidget* parent) : QPushButton(parent)
{
    QFile file(":/styles/CustomButton.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    this->setStyleSheet(styleSheet);
    this->setFixedSize(300, 75);
}
