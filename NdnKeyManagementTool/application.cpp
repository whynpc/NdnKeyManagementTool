#include "application.h"

Application::Application(const QString &appName, QObject *parent) :
    QObject(parent), name(appName)
{
}

int Application::createOrganizerSession(const QString &sessionName, const QString &selfName)
{
    if (organizerSessions.contains(sessionName)) {
        return -1;
    } else {
        OrganizerSession *session = new OrganizerSession(sessionName, this->name, selfName, this);
        organizerSessions.insert(sessionName, session);
        return 0;
    }
}

int Application::destroyOrganizerSession(const QString &sessionName)
{
    if (organizerSessions.contains(sessionName)) {
        OrganizerSession *session = organizerSessions[sessionName];
        organizerSessions.remove(sessionName);
        delete session;
        return 0;
    } else {
        return -1;
    }
}

int Application::createParticipantSession(const QString &sessionName, const QString &selfName,
                                          const QString &organizerName)
{
    if (participantSessions.contains(sessionName)) {
        return -1;
    } else {
        ParticipantSession *session = new ParticipantSession(sessionName, this->name,
                                                             selfName, organizerName, this);
        participantSessions.insert(sessionName, session);
        return 0;
    }
}

int Application::destroyParticipantSession(const QString &sessionName)
{
    if (participantSessions.contains(sessionName)) {
        ParticipantSession *session = participantSessions[sessionName];
        participantSessions.remove(sessionName);
        delete session;
        return 0;
    } else {
        return -1;
    }
}

int Application::recvCreateOrganizerSession(const QString &sessionName, const QString &selfName)
{
    this->createOrganizerSession(sessionName, selfName);
    return 0;
}

int Application::recvDestroyOrganizerSession(const QString &sessionName)
{
    this->destroyOrganizerSession(sessionName);
    return 0;
}

int Application::recvCreateParticipantSession(const QString &sessionName, const QString &selfName,
                                              const QString &organizerName)
{
    this->createParticipantSession(sessionName, selfName, organizerName);
    return 0;
}

int Application::recvDestroyParticipantSession(const QString &sessionName)
{
    this->destroyParticipantSession(sessionName);
    return 0;
}

OrganizerSession *Application::getOrganizerSession(const QString &sessionName) const
{
    if (organizerSessions.contains(sessionName)) {
        return organizerSessions[sessionName];
    } else {
        return NULL;
    }
}

ParticipantSession *Application::getParticipantSession(const QString &sessionName) const
{
    if (participantSessions.contains(sessionName)) {
        return participantSessions[sessionName];
    } else {
        return NULL;
    }
}
