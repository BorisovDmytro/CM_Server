#ifndef DIFFHELMANPROTOCOL_H
#define DIFFHELMANPROTOCOL_H

#include <qmath.h>
#include "QTime"

class DiffHelmanProtocol
{
private:
  long generator;
  long module;
  long privat;
  long key;
public:
  DiffHelmanProtocol(bool generate = false);

  long getPrivate();
  void setPublic(const long pub);

  long getKey();
  void set(long generator, long module);


  long getModule() const;
  long getGenerator() const;

private:
  long getRandomInt(const long min, const long max);
  long getGenerator(const long gen);
};

#endif // DIFFHELMANPROTOCOL_H

