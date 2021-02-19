#pragma once
#include <QString>

//This namespace contains structs that act more or less as copy of the DB's tables strurcts
namespace ManagerInfo
{
	struct SRoleInfo
	{
		int Id = 0;
		
		QString Name;
	};


}