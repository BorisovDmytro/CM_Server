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
