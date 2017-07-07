#ifndef CLIENTINSTENCE_H
#define CLIENTINSTENCE_H

#include "entity/account.h"
#include <QWebSocket>
#include <QObject>
#include "protocol.h"

class CallEntry;

class ClientInstence : public QObject
{
  Q_OBJECT
private:
  QWebSocket *mSocket;
  CallEntry  *mCallEntry;
  Account    *mAccount;
public:
  ClientInstence(QWebSocket *socket, QObject *parent = NULL);
  ~ClientInstence();

  QWebSocket *get();

  CallEntry *getCall();
  void setCall(CallEntry *entry);

  void disconect();

  Account *getAccount() const;
  void setAccount(Account *account);

  inline void sendData(QByteArray &arr) {  mSocket->sendBinaryMessage(arr); }
private slots:
  void readyRead();
};

#endif // CLIENTINSTENCE_H
