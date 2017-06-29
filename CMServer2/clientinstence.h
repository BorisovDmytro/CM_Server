#ifndef CLIENTINSTENCE_H
#define CLIENTINSTENCE_H

#include "entity/account.h"
#include <QTcpSocket>
#include <QObject>
#include "protocol.h"

class CallEntry;

class ClientInstence : public QObject
{
  Q_OBJECT
private:
  QTcpSocket *mSocket;
  CallEntry  *mCallEntry;
  Account    *mAccount;
public:
  ClientInstence(QTcpSocket *socket, QObject *parent = NULL);
  ~ClientInstence();

  QTcpSocket *get();

  CallEntry *getCall();
  void setCall(CallEntry *entry);

  void disconect();

  Account *getAccount() const;
  void setAccount(Account *account);

  inline void sendData(QByteArray &arr) {  mSocket->write(arr); }
private slots:
  void readyRead();
};

#endif // CLIENTINSTENCE_H
