#include "account.h"

Account::Account(QString name, QString password, bool isOnline)
{
    mName = name;
    mPassword = password;
    mIsOnline = isOnline;
}

Account::Account()
{

}

Account::Account(QDataStream &stream)
{
  stream >> mName;
  stream >> mPassword;
}

void Account::serialize(QDataStream &stream)
{
  stream << mName;
  stream << mPassword;
}

QString Account::password() const
{
  return mPassword;
}

void Account::setPassword(const QString &password)
{
  mPassword = password;
}

QString Account::name() const
{
  return mName;
}

void Account::setName(const QString &name)
{
  mName = name;
}

bool Account::isOnline() const
{
    return mIsOnline;
}

void Account::setIsOnline(bool isOnline)
{
    mIsOnline = isOnline;
}
