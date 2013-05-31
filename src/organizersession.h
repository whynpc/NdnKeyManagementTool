#ifndef ORGANIZERSESSION_H
#define ORGANIZERSESSION_H

#include <QHash>
#include <QObject>
#include <string>
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

    // recv Create Shared Key Interest from application
    int recvCreateSharedKeyLocal();
    // recv Renew Shared Key Interest from application
    int recvRenewSharedKeyLocal(const int currentVersion);
    // recv Accept Interest from application
    int recvAcceptJoinLocal(const std::string &peerName);
    // recv Reject Interest from application
    int recvRejectJoinLocal(const std::string &peerName);
    // recv Fetch Shared Key Interest from application
    int recvFetchSharedKeyLocal(int &version, int &chunkNum);
    // recv Join Interest from participant
    int recvJoinRemote(const std::string &peerName);
    // recv Fetch Shared Key Interest from participant
    int recvFecthSharedKeyRemote(const std::string &peerName, int &version, int &chunkNum,
                                 std::string &buffer);
    // recv Fetch Public Key from participant
    int recvFetchPublicKeyRemote(const std::string &peerName, int &version, int &chunkNum,
                                 std::string &buffer);
// recv Public Key Data from participant
    int recvPublicKeyRemote();
    


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
