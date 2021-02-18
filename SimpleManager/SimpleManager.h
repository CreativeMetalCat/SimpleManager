#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleManager.h"

class SimpleManager : public QMainWindow
{
    Q_OBJECT

public:
    SimpleManager(QWidget *parent = Q_NULLPTR);

private:
    Ui::SimpleManagerClass ui;
};
