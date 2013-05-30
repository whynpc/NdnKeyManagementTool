#include "daemon.h"

Daemon::Daemon(QObject *parent) :
    QObject(parent)
{
}

#if WAF
#include "daemon.moc"
#include "daemon.cpp.moc"
#endif
