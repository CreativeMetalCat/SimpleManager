#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleManager.h"
#include "Info.h"

#include "RoleSelectionWindow.h"
#include "UserManager.h"
#include "UserLogInDialog.h"

class SimpleManager : public QMainWindow
{
    Q_OBJECT

public:
    SimpleManager(QWidget *parent = Q_NULLPTR);
private slots:
    void GenerateTabs(ManagerInfo::SUserInfo info);
private:
   

    QSqlDatabase Database;

    Ui::SimpleManagerClass ui;
};
