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

#include <QWebSocket>
#include <QWebSocketServer>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include "aes/aes256.hpp"

typedef QMap<QString, Account*>             CacheAccount;
typedef QMap<QWebSocket *,ClientInstence *> ConenctionMap;
typedef QMap<QString,     ClientInstence *> ClientMap;
typedef QList<CallEntry *>                  CallEntryList;

class CMServer : public QObject
{
  Q_OBJECT
private:
  ICMLoger         *mLoger;
  QWebSocketServer *mServer;
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

  void readyRead(QByteArray in);

  void updateAccountCache();

  void sslErrors(QList<QSslError> errs);
};

#endif // CMSERVER_H
