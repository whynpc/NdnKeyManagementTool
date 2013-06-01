#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QHash>
#include <QString>
#include <string>
#include "application.h"

class OrganizerSession;
class ParticipantSession;

class Context : public QObject
{
    Q_OBJECT

public:
    inline Context() {}
    static Context *instance();

    Application *getApplication(const QString &appName) const;
    int addApplication(const QString &appName);
    int removeApplication(const QString &appName);

    OrganizerSession *getOrganizerSession(const QString &appName,
                                          const QString &sessionName) const;
    ParticipantSession *getParticipantSession(const QString &appName,
                                              const QString &sessionName) const;
    
    void retrieveSession(const std::string &appName, const std::string &sessionName, 
			 OrganizerSession **oSession, ParticipantSession **pSession) const;

    void retrieveApplication(const std::string &appName, Application **application) const;

private:
    QHash<QString, Application *> applications;
};

#endif // CONTEXT_H
