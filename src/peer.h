#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QString>

class Peer : public QObject
{
    Q_OBJECT

public:       
    explicit Peer(QString peerName, QObject *parent);

    bool operator==(const Peer &peer) const;
    QString getName() const;

private:
    QString name;
    
};

#endif // PEER_H
