#include "context.h"

Q_GLOBAL_STATIC(Context, context)

Context *Context::instance()
{
    return context();
}

Application *Context::getApplication(const QString &appName) const
{
    if (applications.contains(appName)) {
        return applications[appName];
    } else {
        return NULL;
    }
}

OrganizerSession *Context::getOrganizerSession(const QString &appName, const QString &sessionName) const
{
    if (applications.contains(appName)) {
        return applications[appName]->getOrganizerSession(sessionName);
    } else {
        return NULL;
    }
}

ParticipantSession *Context::getParticipantSession(const QString &appName, const QString &sessionName) const
{
    if (applications.contains(appName)) {
        return applications[appName]->getParticipantSession(sessionName);
    } else {
        return NULL;
    }
}

int Context::addApplication(const QString &appName)
{
    if (applications.contains(appName)) {
        return -1;
    } else {
        Application *app = new Application(appName, this);
        applications.insert(appName, app);
        return 0;
    }
}

int Context::removeApplication(const QString &appName)
{
    if (applications.contains(appName)) {
        return 0;
    } else {
        return -1;
    }
}

void Context::retrieveSession(const std::string &appName, const std::string &sessionName, 
		     OrganizerSession **oSession, ParticipantSession **pSession) const
{
    QString qAppName(appName.c_str());
    QString qSessionName(sessionName.c_str());
    *oSession = getOrganizerSession(qAppName, qSessionName);
    *pSession = getParticipantSession(qAppName, qSessionName);
}

void Context::retrieveApplication(const std::string &appName, Application **application) const
{
    QString qAppName(appName.c_str());
    *application = getApplication(qAppName);
}


#if WAF
#include "context.moc"
#include "context.cpp.moc"
#endif
