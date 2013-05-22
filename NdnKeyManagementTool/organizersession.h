#ifndef ORGANIZERSESSION_H
#define ORGANIZERSESSION_H

#include <QHash>
#include <QObject>

#include "sharedkey.h"
#include "peer.h"


class OrganizerSession : public QObject
{
    Q_OBJECT

public:
    OrganizerSession(const QString &sessionName, const QString &applicationName,
                     const QString &selfName, QObject *parent);

    int acceptJoin(const QString &peerName);
    int rejectJoin(const QString &peerName);

    int crateSharedKey();
    int renewSharedKey(const int currentVersion);

    int recvCreateSharedKeyLocal();
    int recvRenewSharedKeyLocal(const int currentVersion);
    int recvAcceptJoinLocal(const QString &peerName);
    int recvRejectJoinLocal(const QString &peerName);
    int recvFetchSharedKeyLocal(const int version, const int chunkNum);

    int recvJoinRemote(const QString &peerName);
    int recvFecthSharedKeyRemote(const QString &peerName, const int version, const int chunkNum);


private:
    int sendRenewSharedKeyRemote();
    int sendAcceptJoinRemote(const QString &peerName);
    int sendRejectJoinRemote(const QString &peerName);

    QString name;
    QString appName;
    SharedKey *sharedKey;
    Peer *self;

    QHash<QString, Peer *> candidates;
    QHash<QString, Peer *> participants;
    QHash<QString, Peer *> blacklist;

};

#endif // ORGANIZERSESSION_H
