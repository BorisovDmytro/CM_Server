#ifndef DBACCOUNTCONTROLLER_H
#define DBACCOUNTCONTROLLER_H

#include "entity/account.h"
#include "dbconnection.h"
#include "basesqlcontroller.h"
#include <QList>

class DbAccountController : public BaseSQLController
{
private:
    DbConnection *connect;
public:
  DbAccountController();
  ~DbAccountController();

  void init(DbConnection *connect);
  QList<Account *> getAll();  
  void add(Account *acc);
  void remove(Account *acc);
  void update(Account *accNew, Account* old);
  void update(Account *acc);
  Account *getByName(const QString &name);

  void createTable() override;
};

#endif // DBACCOUNTCONTROLLER_H
