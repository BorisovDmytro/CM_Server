#ifndef DBCONNECTEDSETTING_H
#define DBCONNECTEDSETTING_H

#include <QJsonDocument>
#include <QSqlDatabase>

class DbConnectedSetting
{
private:
  QString mUrl;
  int     mPort;
  QString mDbName;
  QString mUser;
  QString mPassword;
public:
  DbConnectedSetting();
  DbConnectedSetting(const QString& path);

  void load(const QString& path);
  void save(const QString& path);

  QString getUrl() const;
  void setUrl(const QString &url);
  int getPort() const;
  void setPort(int port);
  QString getDbName() const;
  void setDbName(const QString &dbName);
  QString getUser() const;
  void setUser(const QString &user);
  QString getPassword() const;
  void setPassword(const QString &password);
};

#endif // DBCONNECTEDSETTING_H
