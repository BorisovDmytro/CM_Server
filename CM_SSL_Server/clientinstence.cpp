#include "clientinstence.h"
#include "callentry.h"

#include <QDebug>

Account *ClientInstence::getAccount() const
{
  return mAccount;
}

void ClientInstence::setAccount(Account *account)
{
  mAccount = account;
}

qint64 ClientInstence::setKeyData(quint64 gen, quint64 mod, quint64 pnum)
{
  mDiffHelmanAuth.set(gen, mod);
  mDiffHelmanAuth.setPublic(pnum);
  quint64 pub = mDiffHelmanAuth.getPrivate();

  qint64 key = mDiffHelmanAuth.getKey();

  qDebug() << "AuthKey:" << key;

  std::string strKey = QString::number(key).toStdString();

  std::vector<unsigned char> keyVec(32);
  for (int i = 0; i < 32; i++) {
      if (strKey.size() > i) {
          keyVec.push_back(strKey.at(i));
        } else {
          keyVec.push_back(0xff);
        }
    }
  mKeyAuth = keyVec;
  return pub;
}

ClientInstence::ClientInstence(QWebSocket *socket, QObject *parent)
  : QObject(parent)
{
  mSocket    = socket;
  mCallEntry = NULL;
  mAccount   = NULL;
}

ClientInstence::~ClientInstence()
{
  if (mAccount != NULL)
    delete mAccount;
}

QWebSocket *ClientInstence::get()
{
  return mSocket;
}

CallEntry *ClientInstence::getCall()
{
  return mCallEntry;
}

void ClientInstence::setCall(CallEntry *entry)
{
  mCallEntry = entry;
}

void ClientInstence::disconect()
{
  if (mCallEntry) {
    mCallEntry->remove(this);
    mCallEntry = NULL;
  }
}

void ClientInstence::readyRead()
{

}
