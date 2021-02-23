#pragma once
#include <QString>
#include <QVector>
#include <QJsonObject>

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

	struct SUserInfo
	{
		int Id = 0;

		QString Name;

		QVector<int> Roles;
		
		QString Password;

		//Id of table set that this user belongs to
		int TableSetId = 0;

		QJsonObject ContactInfo;
	};

}