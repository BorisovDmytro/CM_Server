#include "cmserver.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <QDebug>
#include "entity/messageinformation.h"

#include "log/icmloger.h"
#include "log/consoledebugloger.h"


CMServer::CMServer(ICMLoger *loger, QObject *parent)
  : QObject(parent)
{
  mLoger  = (loger != NULL) ? loger : new ConsoleDebugLoger();
  mServer = NULL;
  m_nNextBlockSize = 0;
}

CMServer::~CMServer()
{
  qDeleteAll(mClients);
  delete mLoger;
}

void CMServer::start(QString host, int port)
{
  if (mServer != NULL)
    delete mServer;

  mServer = new QTcpServer(this);
  if (! mServer->listen(QHostAddress(host), port)) {
      mLoger->error("Cant run server on " + host + ":" + QString::number(port));
      return;
    }

  connect(mServer, SIGNAL(newConnection()),
          this,    SLOT(newConnection()));

  mLoger->info("Serever start on " + host + ":" + QString::number(port));
}

void CMServer::start(const CMServerSetting &setting, const DbConnectedSetting &dbSetting)
{
  mDbConnection.init(dbSetting);
  if (!mDbConnection.connect()) {
      qDebug() << "Error connect to database";
    }

  mDbAccountCtrl.init(&mDbConnection);
  updateAccountCache();
  start(setting.host(), setting.port());
}

void CMServer::newConnection()
{
  QTcpSocket     *socket = mServer->nextPendingConnection();
  ClientInstence *client = new ClientInstence(socket);

  mConnections.insert(socket, client);

  connect(socket, SIGNAL(disconnected()),
          this,   SLOT(disconnect()));

  connect(socket, SIGNAL(readyRead()),
          this,   SLOT(readyRead()));
}

void CMServer::disconnect()
{
  try {
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    if (socket == NULL)
      return;

    ClientInstence *client = mConnections.value(socket, NULL);

    if (client) {
        if ( client->getAccount() != NULL) {
            mClients.remove(client->getAccount()->name());
          }
        mConnections.remove(socket);
        client->disconect();
        delete client;
      }
    socket->deleteLater();
  } catch(...) {
    qDebug() << "Error disconect";
  }
}
// TODO CREATE COMMAND FACTORY
void CMServer::readyRead()
{
  QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

  if (socket == NULL)
    return;

  ClientInstence *client = mConnections.value(socket, NULL);

  if (client == NULL)
    return;

  QDataStream stream(socket);
  int type = MessageType::Undefined;
  try {
    for (;;) {
        if (!m_nNextBlockSize) {
            if (socket->bytesAvailable() < sizeof(quint16)) {
                break;
              }
            stream >> m_nNextBlockSize;
          }

        if (socket->bytesAvailable() < m_nNextBlockSize) {
            break;
          }
        m_nNextBlockSize = 0;

        stream >> type;
        switch (type) {
          case MessageType::Auth: {
              QString name;
              QString pass;

              stream >> name;
              stream >> pass;
              bool isValid = false;

              QByteArray arr;
              QDataStream out(&arr, QIODevice::WriteOnly);
              out << quint16(0);
              out << (int) MessageType::Auth;

              if (mClients.value(name, NULL) == NULL) {
                  Account* acc = mAccounts.value(name, NULL);
                  if (acc != NULL) {
                      if (pass == acc->password()) {
                          client->setAccount(acc);
                          mClients.insert(name, client);
                          isValid = true;
                        }
                    }
                }

              out << isValid;
              out.device()->seek(0);
              out << quint16(arr.size() - sizeof(quint16));
              client->sendData(arr);
              if (!isValid)
                socket->close();
            } break;

          case MessageType::CallFrame: {
              CallEntry* call = client->getCall();
              if (call) {
                  call->sendCallDataToEntry(client, stream);
                }
            } break;
          case MessageType::TextMessage: {
              MessageInformation msg(stream);
              ClientInstence *rec = mClients.value(msg.getRecipient(), NULL);
              if (rec) {
                  QByteArray arr;
                  QDataStream out(&arr, QIODevice::WriteOnly);
                  out.setVersion(QDataStream::Qt_5_9);
                  out << quint16(0);

                  out << (int) MessageType::TextMessage;
                  msg.saveToStream(out);
                  out.device()->seek(0);
                  out << quint16(arr.size() - sizeof(quint16));

                  rec->sendData(arr);
                } else {
                  // TODO SAVE HISTORY
                }
            } break;
          case MessageType::GetUserList: {
              QList<QString> list = mAccounts.keys();
              QByteArray arr;
              QDataStream out(&arr, QIODevice::WriteOnly);
              out.setVersion(QDataStream::Qt_5_9);
              out << quint16(0);

              out << (int) MessageType::GetUserList;
              out << list.size();

              foreach (QString buff, list) {
                  out << buff;
                }

              out.device()->seek(0);
              out << quint16(arr.size() - sizeof(quint16));

              socket->write(arr);
            } break;
          case MessageType::StartCall: {
              QString recipient;
              stream >> recipient;

              QByteArray arr;
              QDataStream out(&arr, QIODevice::WriteOnly);
              out.setVersion(QDataStream::Qt_5_9);
              out << quint16(0);

              out << (int) MessageType::StartCall;
              out << client->getAccount()->name();

              ClientInstence* rec = mClients.value(recipient, NULL);
              if (rec != NULL) {
                  out.device()->seek(0);
                  out << quint16(arr.size() - sizeof(quint16));

                  rec->sendData(arr);

                  CallEntry *entry = new CallEntry(this);

                  entry->add(client);
                  entry->add(rec);

                  mCalls.append(entry);
                } else {
                  // TODO SEND ERROR
                }
            } break;
          case MessageType::SuccessCall: {
              CallEntry *entry = client->getCall();
              if (entry != NULL) {
                  QByteArray arr;
                  QDataStream out(&arr, QIODevice::WriteOnly);
                  out.setVersion(QDataStream::Qt_5_9);
                  out << quint16(0);

                  out << (int) MessageType::SuccessCall;
                  out << client->getAccount()->name();

                  out.device()->seek(0);
                  out << quint16(arr.size() - sizeof(quint16));

                  entry->getUser(0)->sendData(arr);
                }
            } break;

          case MessageType::CanselCall: {
              CallEntry *entry = client->getCall();
              if (entry != NULL) {
                  QByteArray arr;
                  QDataStream out(&arr, QIODevice::WriteOnly);
                  out.setVersion(QDataStream::Qt_5_9);
                  out << quint16(0);

                  out << (int) MessageType::CanselCall;
                  out << client->getAccount()->name();

                  out.device()->seek(0);
                  out << quint16(arr.size() - sizeof(quint16));
                  entry->getUser(0)->sendData(arr);

                  mCalls.removeOne(entry);

                  entry->destroy();
                  delete entry;
                }
            } break;

          case MessageType::EndCall: {
              CallEntry *entry = client->getCall();
              if (entry != NULL) {
                  QByteArray arr;
                  QDataStream out(&arr, QIODevice::WriteOnly);
                  out.setVersion(QDataStream::Qt_5_9);

                  out << quint16(0);
                  out << (int) MessageType::EndCall;
                  out.device()->seek(0);
                  out << quint16(arr.size() - sizeof(quint16));

                  entry->getUser(0)->sendData(arr);
                  entry->getUser(1)->sendData(arr);

                  mCalls.removeOne(entry);

                  entry->destroy();
                  delete entry;
                } else {

                }
            } break;
          default:
            // TODO SEND MESSAGE
            break;
          }
      }
  } catch(...) {
    qDebug () << "!!! Error read package";
  }
}

void CMServer::updateAccountCache()
{
  mAccounts.clear();
  QList<Account*> accaunts = mDbAccountCtrl.getAll();
  foreach (Account* acc, accaunts) {
      mAccounts.insert(acc->name(), acc);
    }
}
