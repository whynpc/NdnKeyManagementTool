#include <QtGui>

 #include "dialog.h"

Dialog::Dialog()
{
    create();
    connect(button, SIGNAL(clicked()), this, SLOT(accept()));
}

/*
void Dialog::accept()
{
    data1 =app->text();
}
*/

QString Dialog::getAppData()
{
    return app->text();
}

QString Dialog::getSessionData()
{
    return session->text();
}

QString Dialog::getOrgData()
{
    return org->text();
}

QString Dialog::getPatData()
{
    return pat->text();
}

void Dialog::create()
{
//    gridGroupBox = new QGroupBox(tr("xx"));
    QGridLayout *layout = new QGridLayout;
    appName = new QLabel(tr("app name"));
    sessionName = new QLabel(tr("session name"));
    orgName = new QLabel(tr("organizer name"));
    patName = new QLabel(tr("participant name"));
    app = new QLineEdit;
    session = new QLineEdit;
    org = new QLineEdit;
    pat = new QLineEdit;
    button = new QPushButton(tr("OK"));
    layout->addWidget(appName,0,0);
    layout->addWidget(sessionName,1,0);
    layout->addWidget(orgName,2,0);
    layout->addWidget(patName,3,0);
    layout->addWidget(app,0,1);
    layout->addWidget(session,1,1);
    layout->addWidget(org,2,1);
    layout->addWidget(pat,3,1);
    layout->addWidget(button,4,0);
    setLayout(layout);
}

#if WAF
#include "dialog.moc"
#include "dialog.cpp.moc"
#endif
