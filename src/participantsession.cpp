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
        std::string sProducer(selfName.toUtf8().constData());
        remoteServer::instance().init(sAppName, sSessionName,sProducer);
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
    if (organizer != NULL) {
        sendFetchSharedKeyRemote(sharedKey->getVersion(), 1);
        return 0;
    } else {
        return -1;
    }
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
        this->sendFetchSharedKeyRemote(version, 1);
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
    recvSharedKey = true;
    
    std::clog<<"rec shared key"<<"  "<<version<<"  "<<chunkData<<std::endl;
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

int ParticipantSession::recvFetchSharedKeyLocal(int &version, int &chunkNum, 
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

int ParticipantSession::recvFetchPublicKeyRemote(const std::string &peerName, int &version, 
                                                 int &chunkNum, int &chunkSize, std::string &buffer)
{
    return 0;    
}

int ParticipantSession::recvPublicKeyRemote(const std::string &peerName, const int version, 
                                            const int chunkNum, const int chunkSize, 
                                            const std::string &chunkData)
{
    return 0;
}

void ParticipantSession::getDebugInfo(QString &outputBuffer) const
{
    outputBuffer.append("Self: ");
    outputBuffer.append(self->getName());
    outputBuffer.append("\nOrganizer: ");
    if (organizer == NULL) {
        outputBuffer.append("NULL");
    } else {
        outputBuffer.append(organizer->getName());
    }
    outputBuffer.append("\nStatus: ");
    switch (state) {
        case ParticipantSession::INIT:
            outputBuffer.append("init");
            break;
        case ParticipantSession::PENDING:
            outputBuffer.append("PENDING");
            break;
        case ParticipantSession::ACCEPTED:
            outputBuffer.append("ACCEPTED");
            break;
        case ParticipantSession::REJECTED:
            outputBuffer.append("REJECTED");
            break;
        default:
            break;
    }
    outputBuffer.append("\nShared Key: ");
    sharedKey->getDebugInfo(outputBuffer);
    outputBuffer.append("\n");
}


#if WAF
#include "participantsession.moc"
#include "participantsession.cpp.moc"
#endif
