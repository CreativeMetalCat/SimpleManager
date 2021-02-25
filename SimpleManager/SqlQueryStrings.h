#pragma once
#include <QString>
/*
This file shows a very crude but needed solution. 
Issue -> you want to have an online and ofline member managment. BUT online and offline databases can not be mixed(this app uses SQLite and MySQL which have a lot
of similarities, but also are very different). As such this app needs to have two versions -> for online DB and offline

Solution -> actually make two apps
New issue -> how to deal with difference in commands and keeping them update?
New solution -> create a set of macroses and static functinos just for that purpose and build needed on based on a switch.
This way it's abstarcted and less likely to cause issues
*/


//if you want to build version of an app that uses online component -> uncomment next like
//#define MYSQL

#ifdef MYSQL // define commands compatible with MySQL

namespace SqlHelpers
{
	
}

#else // define commands compatible with SQLite

namespace SqlHelpers
{
	
}
#endif // MYSQL

