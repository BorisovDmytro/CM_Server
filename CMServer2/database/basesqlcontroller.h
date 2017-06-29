#ifndef BASESQLCONTROLLER_H
#define BASESQLCONTROLLER_H

#include <QSqlQuery>

class BaseSQLController
{
public:
  BaseSQLController();
  ~BaseSQLController();

  void printError(QSqlQuery &query);

  virtual void createTable() = 0;
};

#endif // BASESQLCONTROLLER_H
