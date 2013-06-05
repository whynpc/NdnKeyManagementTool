#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QString>
#include <QHash>
#include <string>
#include <QStringList>

#include "organizersession.h"
#include "participantsession.h"

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(const QString &appName, QObject *parent);

    int createOrganizerSession(const QString &sessionName, const QString &selfName);
    int destroyOrganizerSession(const QString &sessionName);

    int createParticipantSession(const QString &sessionName, const QString &selfName,
                                 const QString &organizerName);
    int destroyParticipantSession(const QString &sessionName);

    int recvCreateOrganizerSession(const std::string &sessionName, const std::string &selfName);
    int recvDestroyOrganizerSession(const std::string &sessionName);

    int recvCreateParticipantSession(const std::string &sessionName, const std::string &selfName,
                                     const std::string &organizerName);
    int recvDestroyParticipantSession(const std::string &sessionName);

    OrganizerSession *getOrganizerSession(const QString &sessionName) const;
    ParticipantSession *getParticipantSession(const QString &sessionName) const;
    void getOrganizerSessionNames(QStringList &outputList) const;
    void getParticipantSessionNames(QstringList &outputList) const;
    
private:
    QHash<QString, OrganizerSession *> organizerSessions;
    QHash<QString, ParticipantSession *> participantSessions;

    QString name;

};

#endif // APPLICATION_H
