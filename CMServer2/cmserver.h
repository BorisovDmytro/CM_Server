/*
  create by Borysov Dmytro
  email borysovdmytrocnua@gmail.com
*/

#ifndef CMSERVER_H
#define CMSERVER_H

#include <QObject>
#include <QMap>

#include "clientinstence.h"
#include "callentry.h"
#include "setting/cmserversetting.h"
#include "log/icmloger.h"
#include "database/dbconnectedsetting.h"

#include "database/dbconnection.h"
#include "database/dbaccountcontroller.h"

#include <QSharedPointer>

class QTcpSocket;
class QTcpServer;

typedef QMap<QString, Account*>             CacheAccount;
typedef QMap<QTcpSocket *,ClientInstence *> ConenctionMap;
typedef QMap<QString,     ClientInstence *> ClientMap;
typedef QList<CallEntry *>                  CallEntryList;

class CMServer : public QObject
{
  Q_OBJECT
private:
  ICMLoger     *mLoger;
  QTcpServer   *mServer;
  ConenctionMap mConnections;
  ClientMap     mClients;
  CallEntryList mCalls;
  CacheAccount  mAccounts;

  DbConnection        mDbConnection;
  DbAccountController mDbAccountCtrl;
  quint16 m_nNextBlockSize;
public:
  explicit CMServer(ICMLoger *loger = 0, QObject *parent = 0);
  ~CMServer();

  void start(QString host, int port);
  void start(const CMServerSetting &setting, const DbConnectedSetting &dbSetting);
signals:

private slots:
  void newConnection();

  void disconnect();

  void readyRead();

  void updateAccountCache();
};

#endif // CMSERVER_H
