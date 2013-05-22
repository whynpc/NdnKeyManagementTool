#ifndef PARTICIPANTSESSION_H
#define PARTICIPANTSESSION_H

#include <QObject>
#include <QString>

#include "sharedkey.h"
#include "peer.h"

class Session;
class Peer;
class Application;

class ParticipantSession : public QObject
{

public:
    enum State {INIT = 0, PENDING, ACCEPTED, REJECTED};

    ParticipantSession(const QString &sessionName, const QString &applicationName,
                       const QString &selfName, const QString &organizerName,
                       QObject *parent);

    int join();
    int fetchSharedKey();

    State getState() const;
    Peer *getOrganizer() const;

    int recvAcceptJoinRemote();
    int recvRejectJoinRemote();
    int recvSharedKeyRemote(const int version, const int chunkNum, const int chunkSize,
                            const QByteArray &chunkData);
    int recvRenewSharedKeyRemote(const int version);

    int recvJoinLocal();
    int recvFetchSharedKeyLocal(int version, int chunkNum);

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
