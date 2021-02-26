#pragma once
#include <QString>
#include <QVector>
#include <QMap>

//This namespace contains structs that act more or less as copy of the DB's tables strurcts
namespace ManagerInfo
{
	struct SRoleInfo
	{
		//Role id in table
		int Id = 0;
		
		QString Name;

		//Id of table set that this roles belongs to
		int TableSetId = 0;

		QVector<int> ChildRoles;

	};

	struct SContactInfo
	{
		QString TypeName;

		QString Info;

		SContactInfo(QString typeName, QString info) :TypeName(typeName), Info(info) {}
	};

	struct SUserInfo
	{
		int Id = 0;

		QString Name;

		QVector<int> Roles = QVector<int>();
		
		QString Password;

		//Id of table set that this user belongs to
		int TableSetId = 0;

		//QVector<SContactInfo> ContactInfo = QVector<SContactInfo>();

		bool IsAdmin = false;

		SUserInfo(int id, QString name, QVector<int> roles, QString password, int tableSetId, QVector<SContactInfo> contactInfo, bool isAdmin) :
			Id(id), Name(name), Roles(roles), Password(password), TableSetId(tableSetId) /*,ContactInfo(contactInfo)*/, IsAdmin(isAdmin) {}
		SUserInfo() { /*ContactInfo = QVector<SContactInfo>();*/}
	};

}