#pragma once
#include <QString>
#include <QVector>

//This namespace contains structs that act more or less as copy of the DB's tables strurcts
namespace ManagerInfo
{
	struct SRoleInfo
	{
		int Id = 0;
		
		QString Name;
	};

	struct SUserInfo
	{
		int Id = 0;

		QString Name;

		QVector<int> Roles;
	};

}