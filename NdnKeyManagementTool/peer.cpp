#include "peer.h"

Peer::Peer(QString peerName, QObject *parent) :
    QObject(parent), name(peerName)
{
}

QString Peer::getName() const
{
    return name;
}

bool Peer::operator ==(const Peer &peer) const
{
    return (name == peer.getName());
}
