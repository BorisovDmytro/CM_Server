#include "diffhelmanprotocol.h"

#include <QDebug>

long DiffHelmanProtocol::getModule() const
{
  return module;
}

long DiffHelmanProtocol::getGenerator() const
{
  return generator;
}

DiffHelmanProtocol::DiffHelmanProtocol(bool generate)
{
  if (generate) {
    generator = getGenerator(getRandomInt(0, 2));
    module    = getRandomInt(111111111, 999999999);
  }

  privat = getRandomInt(2, 8);
}

long DiffHelmanProtocol::getKey()
{
  return this->key;
}

void DiffHelmanProtocol::set(long generator, long module)
{
  this->generator = generator;
  this->module    = module;
}

long DiffHelmanProtocol::getPrivate()
{
  return (long)pow(generator, privat) % module;
}

void DiffHelmanProtocol::setPublic(const long pub)
{
  key = fabs(((long)pow(pub, privat) % module));
}

long DiffHelmanProtocol::getRandomInt(const long min, const long max)
{
  QTime midnight(0,0,0);
  qsrand(midnight.secsTo(QTime::currentTime()));
  return fabs(floor(qrand() * (max - min +1)) + min);
}

long DiffHelmanProtocol::getGenerator(const long gen)
{
  return 2 * gen + 2;
}

