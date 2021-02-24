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
//a simple macro-string for query for checking if table exists
#define DOES_TABLE_EXISTS_STRING(type,tableName) "SELECT * FROM information_schema.tables WHERE table_schema = 'Users'AND table_name = '" + tableType + "_" + tableName + "' LIMIT 1; "

namespace SqlHelpers
{
	static QString GetTableExistanceCheckString(QString tableType, QString tableName)
	{
		return "SELECT * FROM information_schema.tables WHERE table_schema = 'Users'AND table_name = '" + tableType + "_" + tableName + "' LIMIT 1; ";
	}
}

#else // define commands compatible with SQLite
//a simple macro-string for simplification of writing the table creation query
#define ROLE_TABLE_CREATION_STRING(type,tableName) "CREATE TABLE " + tableType+"_" + tableName + " (Id	INTEGER NOT NULL UNIQUE,Name	TEXT NOT NULL DEFAULT 'Rolename' UNIQUE,ChildRoles	TEXT NOT NULL DEFAULT '{}',PRIMARY KEY(Id AUTOINCREMENT))"


//a simple macro-string for query for checking if table exists
#define DOES_TABLE_EXISTS_STRING(type,tableName) "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableType+"_" + tableName + "';"

namespace SqlHelpers
{
	static QString GetRoleTableCreationString(QString tableType, QString tableName)
	{
		return "CREATE TABLE " + tableType+"_" + tableName + " (Id	INTEGER NOT NULL UNIQUE,Name	TEXT NOT NULL DEFAULT 'Rolename',ChildRoles	TEXT NOT NULL DEFAULT '{}',PRIMARY KEY(Id AUTOINCREMENT))";
	}

	static QString GetTableExistanceCheckString(QString tableType, QString tableName)
	{
		return "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableType + "_" + tableName + "';";
	}
}
#endif // MYSQL

