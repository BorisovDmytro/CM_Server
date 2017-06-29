#include "basesqlcontroller.h"
#include <QSqlError>

#include <QDebug>

BaseSQLController::BaseSQLController()
{

}

BaseSQLController::~BaseSQLController()
{

}

void BaseSQLController::printError(QSqlQuery &query)
{
  QSqlError err = query.lastError();
  qDebug() << "Ошибка SQL" << err.text();
}
