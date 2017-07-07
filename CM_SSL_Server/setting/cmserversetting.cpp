#include "cmserversetting.h"

#include <QFile>
#include <QJsonObject>

QString CMServerSetting::host() const
{
  return mHost;
}

void CMServerSetting::setHost(const QString &host)
{
  mHost = host;
}

int CMServerSetting::port() const
{
  return mPort;
}

void CMServerSetting::setPort(int port)
{
  mPort = port;
}

CMServerSetting::CMServerSetting(const QString &path)
{
  load(path);
}

CMServerSetting::CMServerSetting()
{

}

void CMServerSetting::load(const QString &path)
{
  QFile file(path);

  if (! file.open(QIODevice::ReadOnly)) {
    throw "Error open file";
  }

  QByteArray data = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(data);
  QJsonObject obj = doc.object();

  mHost = obj.value("host").toString();
  mPort = obj.value("port").toInt();
}

void CMServerSetting::save(const QString &path)
{
  QFile file(path);

  if (! file.open(QIODevice::WriteOnly)) {
    throw "Error open file";
  }

  QJsonObject obj;

  obj.insert("host", mHost);
  obj.insert("port", mPort);

  QJsonDocument doc(obj);
  QByteArray data = doc.toJson();

  file.write(data);
  file.close();
}
