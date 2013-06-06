#include "participantsession.h"
#include "remote.hpp"
#include "remote-server.hpp"

ParticipantSession::ParticipantSession(const QString &sessionName, const QString &applicationName,
                                       const QString &selfName, const QString &organizerName,
                                       QObject *parent) :
    QObject(parent), name(sessionName),appName(applicationName)
{
    self = new Peer(selfName, this);
    organizer = NULL;
    sharedKey = new SharedKey(this);
    state = ParticipantSession::INIT;
    if (!organizerName.isEmpty()) {
        organizer = new Peer(organizerName, this);
        // publish data: /prefix/self-name/organizer-name/
        std::string sAppName(appName.toUtf8().constData());
        std::string sSessionName(name.toUtf8().constData());
        remoteServer::instance().init(sAppName, sSessionName);
    } else {
        organizer = NULL;
        // TODO: publishe data for organizer discovery
        // TODO: organizer discovery
    }

    QObject::connect(sharedKey, SIGNAL(updateComplete(int)), this, SLOT(renewSharedKey(int)));
}

ParticipantSession::State ParticipantSession::getState() const
{
    return state;
}

Peer *ParticipantSession::getOrganizer() const
{
    return organizer;
}

int ParticipantSession::join()
{
    if (organizer != NULL) {
        if (state == ParticipantSession::INIT) {
            sendJoinRemote();
            state = ParticipantSession::PENDING;
            return 0;
        } else if (state == ParticipantSession::PENDING || state == ParticipantSession::ACCEPTED) {
            // do nothing
            return 0;
        } else /* state == ParticipantSession::REJECTED */ {
            return -1;
        }
    } else {
        return -1;
    }
}

int ParticipantSession::fetchSharedKey() 
{
        // TDOO: check the parameter setting
    sendFetchSharedKeyRemote(sharedKey->getVersion(), 1);
    return 0;
}

int ParticipantSession::sendJoinRemote()
{
    // send join request to organizer
    std::string sAppName(appName.toUtf8().constData());
    std::string sSessionName(name.toUtf8().constData());
    std::string sOrganizerName(organizer->getName().toUtf8().constData());
    std::string sParticipantName(self->getName().toUtf8().constData());
    remote::instance().joinMembership(sAppName, sSessionName, sParticipantName, sOrganizerName);
    return 0;
}

int ParticipantSession::recvAcceptJoinRemote()
{
    if (state == ParticipantSession::PENDING) {
        state = ParticipantSession::ACCEPTED;
        this->sendAcceptJoinLocal();
        return 0;
    } else {
        return -1;
    }
}

int ParticipantSession::recvRejectJoinRemote()
{
    if (state == ParticipantSession::PENDING) {
        state = ParticipantSession::REJECTED;
        this->sendRejectJoinLocal();
        return 0;
    } else {
        return -1;
    }

}

int ParticipantSession::recvJoinLocal()
{
    this->join();
    return 0;
}

int ParticipantSession::sendAcceptJoinLocal()
{
    // TODO: send accept notification to app
    return 0;
}

int ParticipantSession::sendRejectJoinLocal()
{
    // TODO: send reject notification to app
    return 0;
}

int ParticipantSession::recvRenewSharedKeyRemote(const int version)
{
    if (version > sharedKey->getVersion()) {
        // send interest for 1st chunk; communication layer automatically request remaining chunks        
        this->sendFetchSharedKeyRemote(sharedKey->getVersion(), 1);
        return 0;
    } else {
        return -1;
    }
}

int ParticipantSession::sendFetchSharedKeyRemote(const int version, const int chunkNum)
{
    if (state == ParticipantSession::ACCEPTED) {
        // send interest to organizer
        std::string sAppName(appName.toUtf8().constData());
        std::string sSessionName(name.toUtf8().constData());
        std::string sOrganizerName(organizer->getName().toUtf8().constData());
        std::string sParticipantName(self->getName().toUtf8().constData());
        remote::instance().fetchSharedKey(sAppName, sSessionName, sParticipantName, sOrganizerName);
        return 0;
    } else {
        return -1;
    }
}

int ParticipantSession::recvSharedKeyRemote(const int version, const int chunkNum, 
                                            const int chunkSize, const std::string &chunkData)
{
    QByteArray qChunkData(chunkData.c_str());
    return sharedKey->updateChunk(version, chunkNum, chunkSize, qChunkData);
}

void ParticipantSession::renewSharedKey(int version)
{
    if (version > sharedKey->getVersion()) {
        this->sendRenewSharedKeyLocal();
    }
}

int ParticipantSession::sendRenewSharedKeyLocal()
{
    // TODO: send notification to app
    return 0;
}

int ParticipantSession::recvFetchSharedKeyLocal(int &version, int &chunkNum, int &chunkSize, std::string &buffer)
{
    if (version != sharedKey->getVersion()) {
        // send chunk
        return 0;
    } else {
        return -1;
    }
}

int ParticipantSession::recvFetchPublicKeyRemote(const std::string &peerName, int &version, 
                                                 int &chunkNum, int &chunkSize, std::string &buffer)
{
    return 0;    
}

int ParticipantSession::recvPublicKeyRemote(const std::string &peerName, const int version, const int chunkNum, 
                                            const int chunkSize, const std::string &chunkData)
{
    return 0;
}

#if WAF
#include "participantsession.moc"
#include "participantsession.cpp.moc"
#endif
