#ifndef CONSOLEDEBUGLOGER_H
#define CONSOLEDEBUGLOGER_H

#include "icmloger.h"

class ConsoleDebugLoger : public ICMLoger
{
public:
  ConsoleDebugLoger();

  ~ConsoleDebugLoger() override;

  void info(const QString &msg) override;

  void error(const QString &msg) override;
};

#endif // CONSOLEDEBUGLOGER_H
