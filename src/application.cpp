#include <iostream>
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

int Application::recvCreateOrganizerSession(const std::string &sessionName, const std::string &selfName)
{
    QString qSessionName(sessionName.c_str());
    QString qSelfName(selfName.c_str());
    this->createOrganizerSession(qSessionName, qSelfName);
    return 0;
}

int Application::recvDestroyOrganizerSession(const std::string &sessionName)
{
    QString qSessionName(sessionName.c_str());    
    this->destroyOrganizerSession(qSessionName);
    return 0;
}

int Application::recvCreateParticipantSession(const std::string &sessionName, const std::string &selfName,
                                              const std::string &organizerName)
{
    QString qSessionName(sessionName.c_str());
    QString qSelfName(selfName.c_str());
    QString qOrganizerName(organizerName.c_str());
    this->createParticipantSession(qSessionName, qSelfName, qOrganizerName);
    return 0;
}

int Application::recvDestroyParticipantSession(const std::string &sessionName)
{
    QString qSessionName(sessionName.c_str());
    this->destroyParticipantSession(qSessionName);
    return 0;
}

OrganizerSession *Application::getOrganizerSession(const QString &sessionName) const
{
    std::clog<<sessionName.toUtf8().constData()<<std::endl;
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

void Application::getOrganizerSessionNames(QStringList &outputList) const
{
    foreach (QString name, organizerSessions.keys()) {
        outputList.append(name);
    }
}


void Application::getParticipantSessionNames(QStringList &outputList) const
{
    foreach (QString name, participantSessions.keys()) {
        outputList.append(name);
    }
}


#if WAF
#include "application.moc"
#include "application.cpp.moc"
#endif
