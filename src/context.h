#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QHash>
#include <QString>
#include <string>
#include <QStringList>
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

        // the following functions used by ccnx module to retrieve information
    void retrieveSession(const std::string &appName, const std::string &sessionName, 
			 OrganizerSession **oSession, ParticipantSession **pSession) const;
    void retrieveApplication(const std::string &appName, Application **application) const;

        // the following functions used by GUI to reteve information
    void getApplicationNames(QStringList &outputList) const;
    void getOrganizerSessionNames(const QString &appName, QStringList &outputList) const;
    void getParticipantSessionNames(const QString &appName, QStringList &outputList) const;
    void getParticipantNames(const QString &appName, const QString &sessionName, QStringList &outputList) const;
    void getCandidateNames(const QString &appName, const QString &sesssionName, QStringList &outputList) const;
    void getBlacklistNames(const QString &appName, const QString &sessionName, QStringList &outputList) const;

signals:
    void sigSessionListUpdate(const QString &appName);
    
    

private:
    QHash<QString, Application *> applications;
};

#endif // CONTEXT_H
