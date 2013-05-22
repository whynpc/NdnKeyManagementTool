#include "organizersession.h"

OrganizerSession::OrganizerSession(const QString &sessionName, const QString &applicationName,
                                   const QString &selfName, QObject *parent) :
    QObject(parent), name(sessionName), appName(applicationName)
{
    self = new Peer(selfName, this);
    sharedKey = NULL;
    // TODO: publish data: /prefix/self-name/
    // TODO: publish data for organizer discovery
}

int OrganizerSession::recvJoinRemote(const QString &peerName)
{
    if (blacklist.contains(peerName)) {
        // TODO: possible to send reject later
    } else if(participants.contains(peerName) || candidates.contains(peerName)) {
        // do nothing
    } else {
        Peer *peer = new Peer(peerName, this);
        candidates.insert(peerName, peer);
    }
    return 0;
}

int OrganizerSession::acceptJoin(const QString &peerName)
{
    if (candidates.contains(peerName)) {
        Peer *peer = candidates[peerName];
        candidates.remove(peerName);
        participants.insert(peerName, peer);
        sendAcceptJoinRemote(peerName);
        return 0;
    } else {
        return -1;
    }
}

int OrganizerSession::sendAcceptJoinRemote(const QString &peerName)
{
    // TODO: send accept
    return 0;
}

int OrganizerSession::rejectJoin(const QString &peerName)
{
    if (candidates.contains(peerName)) {
        Peer *peer = candidates[peerName];
        candidates.remove(peerName);
        blacklist.insert(peerName, peer);
        sendRejectJoinRemote(peerName);
        return 0;
    } else {
        return -1;
    }
}

int OrganizerSession::sendRejectJoinRemote(const QString &peerName)
{
    // TODO: send reject
    return 0;
}

int OrganizerSession::crateSharedKey()
{
    if (sharedKey == NULL) {
        sharedKey = new SharedKey();
        sharedKey->genNewSharedKey();
    }
    return 0;
}

int OrganizerSession::renewSharedKey(const int currentVersion) {
    if (currentVersion == sharedKey->getVersion()) {
        sharedKey->genNewSharedKey();
        this->sendRenewSharedKeyRemote();
    }
    return 0;
}

int OrganizerSession::sendRenewSharedKeyRemote()
{
    foreach(Peer *peer, participants.values()) {
        // TODO: send notification to each participant
    }
    return 0;
}

int OrganizerSession::recvCreateSharedKeyLocal()
{
    this->crateSharedKey();
    // TODO: send ack
    return 0;
}

int OrganizerSession::recvRenewSharedKeyLocal(const int currentVersion)
{
    this->renewSharedKey(currentVersion);
    // TODO: send ack
    return 0;
}

int OrganizerSession::recvFecthSharedKeyRemote(const QString &peerName,
                                               const int version,
                                               const int chunkNum)
{
    // TODO: check version & chunkNum
    if (participants.contains(peerName)) {
        // TODO: send chunk
        // Use sharedKey.getChunk(chunkNum)
    } else {
        // TODO: send error code
    }
    return 0;
}

int OrganizerSession::recvAcceptJoinLocal(const QString &peerName)
{
    this->acceptJoin(peerName);
    return 0;
}

int OrganizerSession::recvRejectJoinLocal(const QString &peerName)
{
    this->rejectJoin(peerName);
    return 0;
}
