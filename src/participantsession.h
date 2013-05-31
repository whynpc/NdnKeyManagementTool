#ifndef PARTICIPANTSESSION_H
#define PARTICIPANTSESSION_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <string>
#include "sharedkey.h"
#include "peer.h"


class Peer;
class Application;

class ParticipantSession : public QObject
{
    Q_OBJECT

public:
    enum State {INIT = 0, PENDING, ACCEPTED, REJECTED};

    ParticipantSession(const QString &sessionName, const QString &applicationName,
                       const QString &selfName, const QString &organizerName,
                       QObject *parent);

    int join();
    int fetchSharedKey();

    State getState() const;
    Peer *getOrganizer() const;

// recv Accept Interest from organizer
    int recvAcceptJoinRemote();
// recv Reject Interest from organizer
    int recvRejectJoinRemote();
// recv Shared Key Data from organizer
    int recvSharedKeyRemote(const int version, const int chunkNum, const int chunkSize,
                            const std::string &chunkData);
// recv Renew Shared Key Interest from organizer
    int recvRenewSharedKeyRemote(const int version);
// recv Fetch Public Key Interest from organizer/participant
    int recvFetchPublicKeyRemote(const std::string &peerName, int &version, int &chunkNum,
                                 int &chunkSize, std::string &buffer);
// recv Public Key Data from organizer/participant
    int recvPublicKeyRemote(const int version, const int chunkNum, 
                            const int chunkSize, const std::string &chunkData);
    
// recv Join Interest from application
    int recvJoinLocal();
// recv Fetch Shared Key Interest from application
    int recvFetchSharedKeyLocal(int &version, int &chunkNum);

public slots:
    void renewSharedKey(int version);

signals:
    void newSharedKey(int version);


private:
    int sendJoinRemote();
    int sendFetchSharedKeyRemote(const int version, const int chunkNum);

    int sendAcceptJoinLocal();
    int sendRejectJoinLocal();
    int sendRenewSharedKeyLocal();

    QString name;
    QString appName;
    SharedKey *sharedKey;
    SharedKey *sharedKeyCache;
    Peer *self;
    Peer *organizer;
    State state;
};

#endif // PARTICIPANTSESSION_H
