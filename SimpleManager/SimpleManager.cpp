#include "SimpleManager.h"
#include "RoleSelectionWindow.h"
#include "UserManager.h"

SimpleManager::SimpleManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QSqlDatabase Database = QSqlDatabase::addDatabase("QSQLITE");
    Database.setDatabaseName("Users.db");
    if (Database.open())
    {
        UserManager* selection = new UserManager(Database, this);
        selection->show();
    }
}
