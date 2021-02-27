#include "UserLogInDialog.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>
#include "TableCreationWindow.h"
#include "SimpleManager.h"

UserLogInDialog::UserLogInDialog(QSqlDatabase dataBase,QWidget *parent)
	: QDialog(parent), DataBase(dataBase)
{
	ui.setupUi(this);

	//id is a number so we only allow to type numbers
	ui.lineEdit_Id->setValidator(new QIntValidator(this));

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &UserLogInDialog::AttemptToLogIn);

	connect(ui.button_CreateNewUser, &QPushButton::clicked, this, &UserLogInDialog::OpenNewDBCreationWindow);
}

void UserLogInDialog::AttemptToLogIn()
{
	if (DataBase.isOpen()) 
	{
		QString password = QString(QCryptographicHash::hash(ui.lineEdit_Password->text().toUtf8(), QCryptographicHash::Md5).toHex());
		QSqlQuery query;
		//if user provided id search is simple -> find the user in the table and check the password
		if (ui.lineEdit_Id->text().remove(" ") != "")
		{
			if (query.exec("SELECT * FROM Users WHERE Id = " + ui.lineEdit_Id->text()))
			{
				//create record to make access easier
				QSqlRecord record = query.record();
				while (query.next())
				{
					
					if (password == query.value(record.indexOf("Password")).toString())
					{
						//we found the user and Info is correct
						Info.Name = query.value(record.indexOf("Name")).toString();
						Info.Password = query.value(record.indexOf("Password")).toString();
						Info.Id = query.value(record.indexOf("Id")).toInt();
						
						//1 is true, 0 is false - basics of programming
						//Info.IsAdmin = (query.value(record.indexOf("Admin")).toInt() == 1);

						//get id of table set that player wants to work with
						QJsonObject obj = QJsonDocument::fromJson(query.value(record.indexOf("TableSetId")).toString().toUtf8()).object();
						qWarning() << query.value(record.indexOf("TableSetId")).toString();
						if (obj["tables"].isArray())
						{
							QDialog* dbSelection = new QDialog(this);

							dbSelection->setFixedSize(200, 200);
							dbSelection->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
							QGridLayout* grid = new QGridLayout(dbSelection);
							QScrollArea* area = new QScrollArea(dbSelection);
							QVBoxLayout* layout = new QVBoxLayout(area);

							dbSelection->setLayout(grid);
							grid->addWidget(area);
							area->setLayout(layout);

							QJsonArray array = obj["tables"].toArray();
							QSqlQuery tableSearch;
							for (auto it = array.begin(); it != array.end(); ++it)
							{
								qWarning() << QString::number((*it).toInt());
								tableSearch.exec("SELECT Name FROM TableSets WHERE Id = " + QString::number((*it).toInt()));
								if (tableSearch.next())
								{
									QPushButton* button = new QPushButton(tableSearch.value(0).toString(), this);
									button->setProperty("Id", (*it).toInt());
									connect(button, &QPushButton::clicked, this, [this, button, dbSelection]()
									{
										Info.TableSetId = button->property("Id").toInt();
										dbSelection->close();
									});
									layout->addWidget(button);
								}
							}
							dbSelection->exec();
						}

						auto roleStringBytes = query.value(record.indexOf("RoleId")).toString().remove("'").toUtf8();
						//for simplicity of reading and writing roles are stores as json array
						QJsonObject docObj = QJsonDocument::fromJson(roleStringBytes).object();
						if (docObj[QString::number(Info.TableSetId)].isArray())
						{
							QSqlQuery roleSearch;
							QSqlRecord roleRecord;
							auto tempArray = docObj[QString::number(Info.TableSetId)].toArray();
							for (auto it = tempArray.begin(); it != tempArray.end(); ++it)
							{
								if ((*it).isDouble())
								{
									ManagerInfo::SRoleInfo role;
									
									//read and add role group info
									roleSearch.exec("SELECT Admin,Groups,Name,Power FROM Roles WHERE id = " + QString::number((*it).toInt()));
									roleRecord = roleSearch.record();
									if (roleSearch.next())
									{
										role.Id = (*it).toInt();
										role.Name = query.value(roleRecord.indexOf("Name")).toString();
										role.PowerLevel = query.value(roleRecord.indexOf("Id")).toInt();
										role.TableSetId = Info.TableSetId;

										role.Groups = ConvertJsonStringToStringArray(query.value(roleRecord.indexOf("Groups")).toString(),"groups");

										//add role info
										Info.RolesInfo.append(role);

										//if we still are not sure if user is an admin we do a check
										if (!Info.IsAdmin)
										{
											if (roleSearch.value(0).toInt() == 1) { Info.IsAdmin = true; }
										}
									}
									qWarning() << (*it).toInt();
								}
							}
						}
						else
						{
							qWarning() << query.value(record.indexOf("RoleId")).toString();
						}

						//Contact Info is already stored as json object in sturct so we just find it and assign it
						//docObj = QJsonDocument::fromJson(query.value(record.indexOf("ContactInfo")).toByteArray()).object();
						//Info.ContactInfo = docObj;

						emit OnLogInSuccessful();
						
						return;
					}
					else
					{
						QMessageBox::critical(this, "Error!", "Incorrect password!");
					}
				}


				QMessageBox::critical(this, "Error!", "Failed find the user!\n Check the user id!");

			}
			else
			{
				QMessageBox::critical(this, "Error!", query.lastError().text());
			}
		}
		
	}
	else
	{
		QMessageBox::critical(this, "Error!", "Failed to connect to database!\n Error Info: " + DataBase.lastError().text());
	}
}


UserLogInDialog::~UserLogInDialog()
{
}

void UserLogInDialog::OpenNewDBCreationWindow()
{
	if (auto Parent = qobject_cast<SimpleManager*>(parent()))
	{
		TableCreationWindow* tableCreator = new TableCreationWindow( DataBase,Parent);
		close();
		tableCreator->show();

		connect(tableCreator, &TableCreationWindow::OnFinished, Parent, &SimpleManager::GenerateTabs);
	}
}
