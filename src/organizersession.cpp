#define BOOST_TT_HAS_OPERATOR_HPP_INCLUDED 1
#include <iostream>
#include "remote-server.hpp"
#include "remote.hpp"
#include "organizersession.h"
#include "keydb.h"

OrganizerSession::OrganizerSession(const QString &sessionName, const QString &applicationName,
                                   const QString &selfName, QObject *parent) :
    QObject(parent), name(sessionName), appName(applicationName)
{
    self = new Peer(selfName, this);
    sharedKey = new SharedKey(this);
    sharedKey->create();
    std::string sAppName(appName.toUtf8().constData());
    std::string sSessionName(name.toUtf8().constData());
	std::string sSelfName(selfName.toUtf8().constData());
//    remoteServer *rs = new remoteServer();
 //   rs->init(sAppName, sSessionName);
//    remoteServer rs =remoteServer::instance();

    remoteServer::instance().init(sAppName, sSessionName, sSelfName);

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
    // send accept
    std::string sAppName(appName.toUtf8().constData());
    std::string sSessionName(name.toUtf8().constData());
    std::string sOrganizerName(self->getName().toUtf8().constData());
    std::string sParticipantName(peerName.toUtf8().constData());
    remote::instance().acceptMembership(sAppName, sSessionName, sOrganizerName, sParticipantName);
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
    // send reject
    std::string sAppName(appName.toUtf8().constData());
    std::string sSessionName(name.toUtf8().constData());
    std::string sOrganizerName(self->getName().toUtf8().constData());
    std::string sParticipantName(peerName.toUtf8().constData());
    remote::instance().rejectMembership(sAppName, sSessionName, sOrganizerName, sParticipantName);
    return 0;
}

int OrganizerSession::crateSharedKey()
{
    sharedKey->create();
    return 0;
}

int OrganizerSession::renewSharedKey() 
{
    return renewSharedKey(sharedKey->getVersion());
}    

int OrganizerSession::renewSharedKey(const int currentVersion) 
{
    if (currentVersion == sharedKey->getVersion()) {
        sharedKey->renew();
        this->sendRenewSharedKeyRemote();
    }
    return 0;
}

int OrganizerSession::sendRenewSharedKeyRemote()
{
    foreach(Peer *peer, participants.values()) {
        // send notification to each participant
        std::string sAppName(appName.toUtf8().constData());
        std::string sSessionName(name.toUtf8().constData());
        std::string sOrganizerName(self->getName().toUtf8().constData());
        std::string sParticipantName(peer->getName().toUtf8().constData());
        remote::instance().updateSharedKey(sAppName, sSessionName, sOrganizerName, 
                                           sParticipantName, sharedKey->getVersion());
    }
    return 0;
}

int OrganizerSession::recvCreateSharedKeyLocal()
{
    this->crateSharedKey();
    return 0;
}

int OrganizerSession::recvRenewSharedKeyLocal(const int currentVersion)
{
    this->renewSharedKey(currentVersion);
    return 0;
}

int OrganizerSession::recvFetchSharedKeyRemote(const std::string &peerName, int &version, 
                                               int &chunkNum, int &chunkSize, std::string &buffer)
{
    QString qPeerName(peerName.c_str());   
    if (participants.contains(qPeerName)) {
        if (version == 0) {
            version = sharedKey->getVersion();
        }
        if (chunkNum == 0) {
            chunkNum = 1;
        }
        if (chunkSize == 0) {
            chunkSize = sharedKey->getChunkSize();
        }
        if (version == sharedKey->getVersion()) {
            QByteArray buf;
            sharedKey->readChunk(chunkNum, buf);
            buffer.append(buf.data());
           // buffer.append("sharedKeyContent");
            return buf.length();
        }
    }
    return -1;     
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

int OrganizerSession::recvFetchSharedKeyLocal(int &version, int &chunkNum, 
                                              int &chunkSize, std::string &buffer)
{
    if (version == 0) {
        version = sharedKey->getVersion();
    }
    if (chunkNum == 0) {
        chunkNum = 1;
    }
    if (chunkSize == 0) {
        chunkSize = sharedKey->getChunkSize();
    }
    if (version == sharedKey->getVersion()) {
        QByteArray buf;
        sharedKey->readChunk(chunkNum, buf);
        buffer.append(buf.data());
        return 0;
    } else {
        return -1;
    }
}

int OrganizerSession::recvPublicKeyRemote(const std::string &peerName, const int version, 
                                          const int chunkNum, const int chunkSize, 
                                          const std::string &chunkData)
{
    return 0;
}

int OrganizerSession::recvFetchPublicKeyRemote(const std::string &peerName, int &version, int &chunkNum, 
                                               int &chunkSize, std::string &buffer)
{
    return 0;
}


void OrganizerSession::getParticipantNames(QStringList &outputList) const
{
    foreach (QString name, participants.keys()) {
        outputList.append(name);
    }
}

void OrganizerSession::getCandidateNames(QStringList &outputList) const
{
    foreach (QString name, candidates.keys()) {
        outputList.append(name);
    }
}

void OrganizerSession::getBlacklistNames(QStringList &outputList) const
{
    foreach (QString name, blacklist.keys()) {
        outputList.append(name);
    }   
}

void OrganizerSession::getDebugInfo(QString &outputBuffer) const
{
    outputBuffer.append("Self: ");
    outputBuffer.append(self->getName());
    outputBuffer.append("\nShared Key: ");
    sharedKey->getDebugInfo(outputBuffer);
    outputBuffer.append("\n");
}

int OrganizerSession::recvOrganizerDiscoveryRemote(const std::string &peerName, 
                                                    std::string &outOrganizerName) const
{
    outOrganizerName.append(self->getName().toUtf8().constData());
    return 0;
}

void OrganizerSession::getPeerInfo(const QString &peerName, QString &outputBuffer) const
{
    std::string pubKey[3];
    KeyDB::instance().getKeyFromUser(appName.toStdString(), KeyDB::global, pubKey, 
                                     peerName.toStdString());
    outputBuffer.append("PeerName: ");
    outputBuffer.append(peerName);
    outputBuffer.append("\nPublic Key:\n");
    outputBuffer.append(pubKey[0].c_str());
    outputBuffer.append("\n");
    outputBuffer.append(pubKey[1].c_str());
}


#if WAF
#include "organizersession.moc"
#include "organizersession.cpp.moc"
#endif
