#include "SimpleManager.h"
#include "RoleManagerWindow.h"
#include "TableSettingsTab.h"

SimpleManager::SimpleManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    Database = QSqlDatabase::addDatabase("QSQLITE");
    Database.setDatabaseName("Users.db");
    if (Database.open())
    {
        UserLogInDialog* dialog = new UserLogInDialog(Database, this);
        dialog->show();

        connect(dialog, &UserLogInDialog::OnLogInSuccessful, this, [this, dialog]()
        {
            this->GenerateTabs(dialog->Info);
            dialog->close();
        });
    }
}

void SimpleManager::GenerateTabs(ManagerInfo::SUserInfo info)
{
    //once we succesfully loged in -> open the manager
    UserManager* selection = new UserManager(info, Database, this);
    ui.tabWidget->addTab(selection, "User Manager");

    //then open role managment window
    RoleManagerWindow* roles = new RoleManagerWindow(info, Database, this);
    ui.tabWidget->addTab(roles, "Role Manager");

    //if user is an admin we allow them to use table set settings menu
    if (info.IsAdmin)
    {
        TableSettingsTab* settings = new TableSettingsTab(info,Database,this);
        ui.tabWidget->addTab(settings, "Settings");

        connect(settings, &TableSettingsTab::OnTableSetDeleted, this, [this, roles, selection, settings]() 
        {
            delete roles;
            delete selection;
            delete settings;

            UserLogInDialog* dialog = new UserLogInDialog(Database, this);
            dialog->show();

            connect(dialog, &UserLogInDialog::OnLogInSuccessful, this, [this, dialog]()
            {
                this->GenerateTabs(dialog->Info);
                dialog->close();
            });
        });
    }
}
