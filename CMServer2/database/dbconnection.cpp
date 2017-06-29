#include "dbconnection.h"
#include <QSqlDatabase>

DbConnection::DbConnection()
{
  mDb = QSqlDatabase::addDatabase("QPSQL");
}

void DbConnection::init(const DbConnectedSetting &setting)
{
  mDb.setDatabaseName(setting.getDbName());
  mDb.setHostName(setting.getUrl());
  mDb.setPassword(setting.getPassword());
  mDb.setPort(setting.getPort());
  mDb.setUserName(setting.getUser());
}

bool DbConnection::connect()
{
  return mDb.open();
}

QSqlQuery DbConnection::getQuery()
{
  return QSqlQuery(mDb);
}

bool DbConnection::isConnected()
{
  return mDb.isOpen();
}
