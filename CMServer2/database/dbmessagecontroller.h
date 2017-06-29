#ifndef DBMESSAGECONTROLLER_H
#define DBMESSAGECONTROLLER_H

#include "entity/messageinformation.h"

typedef QList<MessageInformation> MessageList;


class DbMessageController
{
public:
  DbMessageController();

  MessageList getMessageByEmail(QString strEmail);

  void add(MessageInformation *msg);
  void remove(MessageInformation *msg);
  void update(MessageInformation *msg, MessageInformation *old);
};

#endif // DBMESSAGECONTROLLER_H
