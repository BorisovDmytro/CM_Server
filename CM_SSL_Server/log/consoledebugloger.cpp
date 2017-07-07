#include "consoledebugloger.h"
#include <QDebug>

ConsoleDebugLoger::ConsoleDebugLoger()
{

}

ConsoleDebugLoger::~ConsoleDebugLoger()
{

}

void ConsoleDebugLoger::info(const QString &msg)
{
  qDebug() << "Info: " << msg;
}

void ConsoleDebugLoger::error(const QString &msg)
{
  qDebug() << "Error: " << msg;
}
