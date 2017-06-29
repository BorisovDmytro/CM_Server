#include "dbaccountcontroller.h"
#include <QSqlResult>
#include <QSqlError>

DbAccountController::DbAccountController()
{

}

DbAccountController::~DbAccountController()
{

}

void DbAccountController::init(DbConnection *connect)
{
  this->connect = connect;
}

QList<Account *> DbAccountController::getAll()
{
  QSqlQuery query = connect->getQuery();
  query.prepare("SELECT * FROM \"users\";");
  if (!query.exec()) {
      printError(query);
    }

  QList<Account *> list;
  while (query.next()) {
      list.append(new Account(query.value("name").toString(),
                              query.value("password").toString(),
                              query.value("online").toBool()));
    }

  return list;
}

void DbAccountController::add(Account *acc)
{
  QSqlQuery query = connect->getQuery();
  query.prepare("INSERT INTO \"users\" VALUES (:name, :password, :online)");
  query.bindValue(":name", acc->name());
  query.bindValue(":password", acc->password());
  query.bindValue(":online", acc->isOnline());
  if (!query.exec()) {
      printError(query);
    }
}

void DbAccountController::remove(Account *acc)
{
  QSqlQuery query = connect->getQuery();
  query.prepare("DELETE FROM \"users\" WHERE name=:name");
  query.bindValue(":name", acc->name());
  if (!query.exec()) {
      printError(query);
    }
}

void DbAccountController::update(Account *accNew, Account *old)
{
  QSqlQuery query = connect->getQuery();
  query.prepare("UPDATE \"users\" SET name = :name, password = :password, online= :online WHERE name = :oldName");
  query.bindValue(":name",     accNew->name());
  query.bindValue(":password", accNew->password());
  query.bindValue(":online",   accNew->isOnline());
  query.bindValue(":oldName",  old->name());
  if (!query.exec()) {
      printError(query);
    }
}

void DbAccountController::update(Account *acc)
{
  QSqlQuery query = connect->getQuery();
  query.prepare("UPDATE \"users\" SET password = :password, online= :online WHERE name = :name");
  query.bindValue(":name",     acc->name());
  query.bindValue(":password", acc->password());
  query.bindValue(":online",   acc->isOnline());
  if (!query.exec()) {
      printError(query);
    }
}

Account *DbAccountController::getByName(const QString &name)
{
  QSqlQuery query = connect->getQuery();
  query.prepare("SELECT * FROM \"users\" WHERE :name;");
  query.bindValue(":name", name);

  if (!query.exec()) {
      printError(query);
    }


  if (query.next()) {
     return new Account(query.value("name").toString(),
                        query.value("password").toString(),
                        query.value("online").toBool());
    }

  return nullptr;
}

void DbAccountController::createTable()
{
  QSqlQuery query = connect->getQuery();
  query.prepare("CREATE TABLE users"
                "("
                "  name character varying(200) NOT NULL,"
                "  password character varying(200),"
                "  online boolean,"
                "  CONSTRAINT pk_users PRIMARY KEY (name)"
                ")"
                "WITH ("
                "  OIDS=FALSE"
                ");"
                "ALTER TABLE users"
                "  OWNER TO postgres;");
  if (!query.exec()) {
      printError(query);
    }
}


