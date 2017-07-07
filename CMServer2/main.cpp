#include <QCoreApplication>

#include <QDebug>
#include <QDir>
#include "cmserver.h"
#include "log/consoledebugloger.h"
#include "database/dbconnectedsetting.h"
#include "crypto/diffhelmanprotocol.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  a.setApplicationName("CMServer");
  a.setApplicationVersion("1.00");

  ConsoleDebugLoger *pLoger = new ConsoleDebugLoger();

  QString configPath   = QDir::currentPath() + "/server.json";
  QString configDbPath = QDir::currentPath() + "/database.json";

  pLoger->info("Config file path : " + configPath);
  pLoger->info("Config database file path : " + configDbPath);

  CMServerSetting    setting;
  DbConnectedSetting dbSetting;

  try {
    setting.load(configPath);
    dbSetting.load(configDbPath);
  } catch(...) {
    pLoger->error("Error load config file, set default setting : host 127.0.0.1, port 8080 and start server");
    setting.setHost("127.0.0.1");
    setting.setPort(8080);
  }

  CMServer server(pLoger);
  server.start(setting, dbSetting);

  int extCode = 0;
  try {
    extCode = a.exec();
  } catch(...) {
    qDebug() << "!!! Error exec server\n";
  }

  return extCode;
}
