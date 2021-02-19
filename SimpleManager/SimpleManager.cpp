#include "SimpleManager.h"
#include "RoleSelectionWindow.h"

SimpleManager::SimpleManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QSqlDatabase Database = QSqlDatabase::addDatabase("QSQLITE");
    Database.setDatabaseName("Users.db");
    RoleSelectionWindow* selection = new RoleSelectionWindow(Database, this);
    selection->show();
}
