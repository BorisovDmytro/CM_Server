#include "callentry.h"

#include <QDebug>

CallEntry::CallEntry(QObject *parent) : QObject(parent)
{

}

void CallEntry::add(ClientInstence *user)
{
  if (user == NULL)
    return;

  mUsers.append(user);
  user->setCall(this);
}

void CallEntry::remove(ClientInstence *user)
{
  mUsers.removeOne(user);
  user->setCall(NULL);

  if (mUsers.size() < 2) {
    // TODO CALL ENTRY
  }
}

ClientInstence* CallEntry::getUser(int index)
{
  return mUsers.at(index);
}

void CallEntry::destroy()
{
  foreach (ClientInstence *user, mUsers) {
    //if (sender == user) continue;
    remove(user);
  }
}

void CallEntry::sendCallDataToEntry(ClientInstence *sender, QDataStream &stream)
{
  uint       length;
  int        size;
  quint64    frameIndex;
  char*      data;
  QByteArray arr;

  stream >> frameIndex;
  stream >> size;

  stream.readBytes(data, length);

  if (length == 0)
    return;

  qDebug () << "sendVoiceDataToEntry " << frameIndex << " " << size;
  QDataStream out(&arr, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_5_8);

  int type = MessageType::CallFrame;
  out << quint16(0);
  out << type;
  out << frameIndex;
  out << size;

  // TODO Add crypto
  out.writeBytes(data, size);
  out.device()->seek(0);
  out << quint16(arr.size() - sizeof(quint16));

  foreach (ClientInstence *user, mUsers) {
    //if (sender == user) continue;
    user->sendData(arr);
  }
}
/*
out << quint16(0);
out.device()->seek(0);
out << quint16(arr.size() - sizeof(quint16));
          */
