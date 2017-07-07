#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{

private:
  QString mName;
  QString mPassword;
  bool    mIsOnline;

public:
  Account();
  Account(QString name, QString password, bool isOnline = false);
  Account(QDataStream &stream);


  void serialize(QDataStream &stream);

  QString name() const;
  void setName(const QString &name);
  QString password() const;
  void setPassword(const QString &password);
  bool isOnline() const;
  void setIsOnline(bool isOnline);
};

#endif // ACCOUNT_H
