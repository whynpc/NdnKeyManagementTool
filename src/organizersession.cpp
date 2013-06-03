#include "organizersession.h"

OrganizerSession::OrganizerSession(const QString &sessionName, const QString &applicationName,
                                   const QString &selfName, QObject *parent) :
    QObject(parent), name(sessionName), appName(applicationName)
{
    self = new Peer(selfName, this);
    sharedKey = new SharedKey(this);
    // TODO: publish data: /prefix/self-name/
    // TODO: publish data for organizer discovery
}

int OrganizerSession::recvJoinRemote(const std::string &peerName)
{
    QString qPeerName(peerName.c_str());
    if (blacklist.contains(qPeerName)) {
        // TODO: possible to send reject later
    } else if(participants.contains(qPeerName) || candidates.contains(qPeerName)) {
        // do nothing
    } else {
        Peer *peer = new Peer(qPeerName, this);
        candidates.insert(qPeerName, peer);
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
    sharedKey->create();
    return 0;
}

int OrganizerSession::renewSharedKey(const int currentVersion) {
    if (currentVersion == sharedKey->getVersion()) {
        sharedKey->renew();
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

int OrganizerSession::recvFetchSharedKeyRemote(const std::string &peerName, int &version, 
                                               int &chunkNum, int &chunkSize, std::string &buffer)
{
    QString qPeerName(peerName.c_str());   
    // TODO: check version & chunkNum
    if (participants.contains(qPeerName)) {
        // TODO: send chunk
        // Use sharedKey.getChunk(chunkNum)
    } else {
        // TODO: send error code
    }
    return 0;
}

int OrganizerSession::recvAcceptJoinLocal(const std::string &peerName)
{
    QString qPeerName(peerName.c_str());
    this->acceptJoin(qPeerName);
    return 0;
}

int OrganizerSession::recvRejectJoinLocal(const std::string &peerName)
{
    QString qPeerName(peerName.c_str());
    this->rejectJoin(qPeerName);
    return 0;
}

#if WAF
#include "organizersession.moc"
#include "organizersession.cpp.moc"
#endif
