#ifndef MESSAGEINFORMATION_H
#define MESSAGEINFORMATION_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QDataStream>
#include <aes/aes256.hpp>


class MessageInformation
{
protected:
  QString recipient;
  QString autor;
  ByteArray message;
  QDate date;
  QTime time;
public:
  MessageInformation();
  MessageInformation(QString recipient, QString autor, ByteArray message, QDate date, QTime time);
  MessageInformation(QDataStream &stream);

  QString getAutor() const;
  void setAutor(const QString &value);
  ByteArray getMessage() const;
  void setMessage(ByteArray &arr);
  QDate getDate() const;
  void setDate(const QDate &value);
  QTime getTime() const;
  void setTime(const QTime &value);
  QString getRecipient() const;
  void setRecipient(const QString &value);

  void saveToStream(QDataStream &stream);
};

#endif // MESSAGEINFORMATION_H
