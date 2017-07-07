#ifndef CMSERVERSETTING_H
#define CMSERVERSETTING_H

#include <QJsonDocument>

class CMServerSetting
{
private:
  QString mHost;
  int     mPort;
public:
  CMServerSetting(const QString &path);
  CMServerSetting();

  void load(const QString &path);
  void save(const QString &path);

  QString host() const;
  void setHost(const QString &host);

  int port() const;
  void setPort(int port);
};

#endif // CMSERVERSETTING_H
