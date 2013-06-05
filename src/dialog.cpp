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

void Dialog::create()
{
//    gridGroupBox = new QGroupBox(tr("xx"));
    QGridLayout *layout = new QGridLayout;
    appName = new QLabel(tr("app name"));
    sessionName = new QLabel(tr("session name"));
    app = new QLineEdit;
    session = new QLineEdit;
    button = new QPushButton(tr("OK"));
    layout->addWidget(appName,0,0);
    layout->addWidget(sessionName,1,0);
    layout->addWidget(app,0,1);
    layout->addWidget(session,1,1);
    layout->addWidget(button,2,0);
    setLayout(layout);
}

#if WAF
#include "dialog.moc"
#include "dialog.cpp.moc"
#endif
