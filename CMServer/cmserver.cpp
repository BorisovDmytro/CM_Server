#include "cmserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

#include <QDebug>
#include "messageinformation.h"

CMServer::CMServer(QObject *parent) : QObject(parent)
{
  mServer = NULL;
  m_nNextBlockSize = 0;
}

void CMServer::start(QString host, int port)
{
  if (mServer != NULL)
    delete mServer;

  mServer = new QTcpServer(this);
  if (! mServer->listen(QHostAddress(host), port)) {
      QMessageBox::critical(NULL, "Critical error", "Cant run server on " + host + ":" + QString::number(port));
      return;
    }

  connect(mServer, SIGNAL(newConnection()),
          this,    SLOT(newConnection()));

  qDebug() << "Serever start";

  entry = new CallEntry(this);
}

void CMServer::newConnection()
{
  qDebug() << "New Connection";
  QTcpSocket *socket     = mServer->nextPendingConnection();
  ClientInstence *client = new ClientInstence(socket);

  mConnections.insert(socket, client);

  connect(socket, SIGNAL(disconnected()),
          this,   SLOT(disconnect()));

  connect(socket, SIGNAL(readyRead()),
          this,    SLOT(readyRead()));

  entry->add(client);
}

void CMServer::disconnect()
{
  QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

  if (socket == NULL)
    return;

  ClientInstence *client = mConnections.value(socket, NULL);

  if (!client)
    return;

  mConnections.remove(socket);
  mClients.remove(client->getAccount()->name());

  client->disconect();
  delete client;
}
// TODO CREATE COMMAND FACTORY
void CMServer::readyRead()
{
  QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

  if (socket == NULL)
    return;

  ClientInstence *client = mConnections.value(socket, NULL);

  QDataStream stream(socket);
  int type = MessageType::Undefined;

  qDebug() << "Bytes:" << socket->bytesAvailable();
  //while(!stream.atEnd()) {
  try {

    for (;;) {
        if (!m_nNextBlockSize) {
            if (socket->bytesAvailable() < sizeof(quint16)) {
                break;
              }
            stream >> m_nNextBlockSize;
            qDebug() << "NextBlockSize:" << m_nNextBlockSize;
          }

        if (socket->bytesAvailable() < m_nNextBlockSize) {
            break;
          }
        m_nNextBlockSize = 0;

        stream >> type;
        qDebug() << "readyRead:" << type << " " << socket->bytesAvailable() ;
        switch (type) {
          case MessageType::Auth: {
              QString name;
              QString pass;

              stream >> name;
              stream >> pass;

              QByteArray arr;
              QDataStream out(&arr, QIODevice::WriteOnly);
              out << quint16(0);
              out << (int) MessageType::Auth;

              if (mClients.value(name, NULL) == NULL) {
                  out << true;
                  client->setAccount(new Account(name, pass));
                  mClients.insert(name, client);
                  qDebug() << "Auth:" << name;
                } else {
                  out << false;
                }
              out.device()->seek(0);
              out << quint16(arr.size() - sizeof(quint16));
              client->sendData(arr);
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
                  out.setVersion(QDataStream::Qt_5_8);
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
              QList<QString> list = mClients.keys();
              QByteArray arr;
              QDataStream out(&arr, QIODevice::WriteOnly);
              out.setVersion(QDataStream::Qt_5_8);
              out << quint16(0);

              out << (int) MessageType::GetUserList;
              out << list.size();

              foreach (QString buff, list) {
                  out << buff;
                }

              out.device()->seek(0);
              out << quint16(arr.size() - sizeof(quint16));

              socket->write(arr);
              qDebug() << "GetUserList " << mClients.keys() << quint16(arr.size() - sizeof(quint16));
            } break;
          case MessageType::StartCall: {
              QString recipient;
              stream >> recipient;

              QByteArray arr;
              QDataStream out(&arr, QIODevice::WriteOnly);
              out.setVersion(QDataStream::Qt_5_8);
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
                  out.setVersion(QDataStream::Qt_5_8);
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
                  out.setVersion(QDataStream::Qt_5_8);
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
                  out.setVersion(QDataStream::Qt_5_8);

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
    qDebug () << "readyRead end :" << socket->bytesAvailable();
  } catch(...) {
    qDebug () << "Error read package";
  }
  // }
}
