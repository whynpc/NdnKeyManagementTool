#include "participantsession.h"

ParticipantSession::ParticipantSession(const QString &sessionName, const QString &applicationName,
                                       const QString &selfName, const QString &organizerName,
                                       QObject *parent) :
    QObject(parent), name(sessionName),appName(applicationName)
{
    self = new Peer(selfName, this);
    organizer = NULL;
    sharedKey = new SharedKey();
    sharedKeyCache = NULL;
    state = ParticipantSession::INIT;
    if (!organizerName.isEmpty()) {
        organizer = new Peer(organizerName, this);
        // publish data: /prefix/self-name/organizer-name/
    } else {
        organizer = NULL;
        // TODO: publishe data for organizer discovery
        // TODO: organizer discovery
    }

    QObject::connect(this, SIGNAL(newSharedKey(int)), this, SLOT(renewSharedKey(int)));
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

int ParticipantSession::sendJoinRemote()
{
    // TODO: send join request to organizer
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
        if (sharedKeyCache != NULL) {
            delete sharedKeyCache;
        }
        sharedKeyCache = new SharedKey();
        sharedKeyCache->setVersion(version);

        // send interest for 1st chunk; communication layer automatically request remaining chunks
        this->sendFetchSharedKeyRemote(sharedKeyCache->getVersion(), 1);
        return 0;
    } else {
        return -1;
    }
}

int ParticipantSession::sendFetchSharedKeyRemote(const int version, const int chunkNum)
{
    if (state == ParticipantSession::ACCEPTED) {
        // TODO: send interest to organizer
        return 0;
    } else {
        return -1;
    }
}

int ParticipantSession::recvSharedKeyRemote(const int version, const int chunkNum, 
                                            const int chunkSize, const std::string &chunkData)
{
    QByteArray qChunkData(chunkData.c_str());
    if (sharedKeyCache != NULL && version == sharedKeyCache->getVersion()) {
        sharedKeyCache->setSize(chunkSize);
        sharedKeyCache->saveChunk(chunkNum, qChunkData);

        if (sharedKeyCache->isComplete()) {
            emit newSharedKey(sharedKeyCache->getVersion());
        }
        return 0;
    } else {
        return -1;
    }
}

void ParticipantSession::renewSharedKey(int version)
{
    if (version > sharedKey->getVersion()) {
        delete sharedKey;
        sharedKey = sharedKeyCache;
        sharedKeyCache = NULL;

        this->sendRenewSharedKeyLocal();
    }
}

int ParticipantSession::sendRenewSharedKeyLocal()
{
    // TODO: send notification to app
    return 0;
}

int ParticipantSession::recvFetchSharedKeyLocal(int version, int chunkNum)
{
    if (version != sharedKey->getVersion()) {
        // send chunk
        return 0;
    } else {
        return -1;
    }
}

#if WAF
#include "participantsession.moc"
#include "participantsession.cpp.moc"
#endif
