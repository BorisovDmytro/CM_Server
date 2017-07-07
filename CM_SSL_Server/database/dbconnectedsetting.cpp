#include "dbconnectedsetting.h"
#include <QFile>
#include <QDebug>
#include <QJsonObject>

QString DbConnectedSetting::getUrl() const
{
  return mUrl;
}

void DbConnectedSetting::setUrl(const QString &url)
{
  mUrl = url;
}

int DbConnectedSetting::getPort() const
{
  return mPort;
}

void DbConnectedSetting::setPort(int port)
{
  mPort = port;
}

QString DbConnectedSetting::getDbName() const
{
  return mDbName;
}

void DbConnectedSetting::setDbName(const QString &dbName)
{
  mDbName = dbName;
}

QString DbConnectedSetting::getUser() const
{
  return mUser;
}

void DbConnectedSetting::setUser(const QString &user)
{
  mUser = user;
}

QString DbConnectedSetting::getPassword() const
{
  return mPassword;
}

void DbConnectedSetting::setPassword(const QString &password)
{
  mPassword = password;
}

DbConnectedSetting::DbConnectedSetting()
{

}

DbConnectedSetting::DbConnectedSetting(const QString &path)
{
  try {
    load(path);
  } catch(...) {
    qDebug() << "error laod setting";
  }
}

void DbConnectedSetting::load(const QString &path)
{
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
      throw "Error open file " + path;
    }

  QByteArray data = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(data);

  if (!doc.isObject()) {
      throw "Error JSON format";
    }

  QJsonObject obj = doc.object();

  mUrl      = obj.value("host").toString();
  mPort     = obj.value("port").toInt(0);
  mUser     = obj.value("user").toString();
  mPassword = obj.value("password").toString();
  mDbName   = obj.value("database").toString();
}

void DbConnectedSetting::save(const QString &path)
{
  QJsonObject obj;

  obj.insert("host",     QJsonValue(mUrl));
  obj.insert("port",     QJsonValue(mPort));
  obj.insert("user",     QJsonValue(mUser));
  obj.insert("password", QJsonValue(mPassword));
  obj.insert("database", QJsonValue(mDbName));

  QJsonDocument doc(obj);

  QByteArray data = doc.toJson();

  QFile file(path);

  if (!file.open(QIODevice::WriteOnly)) {
      throw "Error open file: " + path;
    }

  file.write(data);
  file.close();
}

