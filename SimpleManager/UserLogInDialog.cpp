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

UserLogInDialog::UserLogInDialog(QSqlDatabase dataBase,QWidget *parent)
	: QDialog(parent), DataBase(dataBase)
{
	ui.setupUi(this);

	//id is a number so we only allow to type numbers
	ui.lineEdit_Id->setValidator(new QIntValidator(this));

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &UserLogInDialog::AttemptToLogIn);
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
						//we found the user and info is correct
						ManagerInfo::SUserInfo info;
						info.Name = query.value(record.indexOf("Name")).toString();
						info.Password = query.value(record.indexOf("Password")).toString();
						info.Id = query.value(record.indexOf("Id")).toInt();
						info.TableSetId = query.value(record.indexOf("TableSetId")).toInt();

						//for simplicity of reading and writing roles are stores as json array
						QJsonObject docObj = QJsonDocument::fromJson(query.value(record.indexOf("Roles")).toByteArray()).object();
						if (docObj["roles"].isArray())
						{
							auto tempArray = docObj["roles"].toArray();
							for (auto it = tempArray.begin(); it != tempArray.end(); ++it)
							{
								if ((*it).isDouble())
								{
									info.Roles.append((*it).toInt());
								}
							}
						}

						//Contact info is already stored as json object in sturct so we just find it and assign it
						docObj = QJsonDocument::fromJson(query.value(record.indexOf("ContactInfo")).toByteArray()).object();
						info.ContactInfo = docObj;

						emit OnLogInSuccessful(info);
						close();
						return;
					}
					else
					{
						QMessageBox::critical(this, "Error!", "Incorrect password!\n Error Info: " + query.value(record.indexOf("Password")).toString() + "|||" + password);
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
