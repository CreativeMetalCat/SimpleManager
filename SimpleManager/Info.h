#pragma once
#include <QString>
#include <QVector>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

//This namespace contains structs that act more or less as copy of the DB's tables strurcts
namespace ManagerInfo
{
	static QVector<QString> ConvertJsonStringToStringArray(QString jsonString,QString arrayName) 
	{
		QVector<QString> result = QVector<QString>();
		QJsonObject obj = QJsonDocument::fromJson(jsonString.toUtf8()).object();
		if (obj[arrayName].isArray()) 
		{
			QJsonArray array = obj[arrayName].toArray();
			for (auto it = array.begin(); it != array.end(); ++it)
			{
				result.append((*it).toString());
			}
		}
		return result;
	}

	static QVector<int> ConvertJsonStringToIntArray(QString jsonString, QString arrayName)
	{
		QVector<int> result = QVector<int>();
		QJsonObject obj = QJsonDocument::fromJson(jsonString.toUtf8()).object();
		if (obj[arrayName].isArray())
		{
			QJsonArray array = obj[arrayName].toArray();
			for (auto it = array.begin(); it != array.end(); ++it)
			{
				result.append((*it).toInt());
			}
		}
		return result;
	}

	struct SRoleInfo
	{
		//Role id in table
		int Id = 0;

		int PowerLevel = 0;
		
		QString Name;

		//Id of table set that this roles belongs to
		int TableSetId = 0;

		QVector<QString> Groups = QVector<QString>();

	};

	struct SUserInfo
	{
		int Id = 0;

		QString Name;

		QVector<int> RolesId = QVector<int>();

		QVector<SRoleInfo> RolesInfo = QVector<SRoleInfo>();
		
		QString Password;

		//Id of table set that this user belongs to
		int TableSetId = 0;

		//QVector<SContactInfo> ContactInfo = QVector<SContactInfo>();

		bool IsAdmin = false;

		bool ContainsRoleFromGroup(QString groupName)
		{
			if (RolesInfo.count() > 0)
			{
				for (int i = 0; RolesInfo.count(); i++)
				{
					if (RolesInfo[i].Groups.contains(groupName)) { return true; }
				}
				return false;
			}
			else
			{
				return false;
			}
		}

		bool ContainsRoleFromGroup(QString groupName, int& groupId)
		{
			groupId = -1;
			if (RolesInfo.count() > 0)
			{
				for (int i = 0; i < RolesInfo.count(); i++)
				{
					if (RolesInfo[i].Groups.contains(groupName)) { groupId = i; return true; }
				}
				return false;
			}
			else
			{
				return false;
			}
		}

		/*SUserInfo(int id, QString name, QVector<int> roles, QString password, int tableSetId, QVector<SContactInfo> contactInfo, bool isAdmin) :
			Id(id), Name(name), Roles(roles), Password(password), TableSetId(tableSetId) ,ContactInfo(contactInfo), IsAdmin(isAdmin) {}*/
		SUserInfo() { /*ContactInfo = QVector<SContactInfo>();*/}
	};

}