#include <QtGui>

 #include "mainwindow.h"
 #include  "dialog.h"
 #include "context.h"
 #include "application.h"
 #include "participantsession.h"
 #include "organizersession.h"
 #include <QVector>


 MainWindow::MainWindow()
 {
     userName = "Tom";
     createActions();
     creatMenu();
     textEdit = new QPlainTextEdit;
     createupperBox();
//     addList();
     //Set mainwindow layout
     QWidget *widget = new QWidget();
     this->setCentralWidget(widget);
     QGridLayout *mainLayout = new QGridLayout;
     mainLayout->addWidget(horizontalGroupBox,0,0);
     mainLayout->addWidget(textEdit,1,0);
     centralWidget()->setLayout(mainLayout);
 }

 void MainWindow::appEvent(QListWidgetItem *item)
 {
     currentApp = item->text();
     update_session();
//         textEdit->setPlainText(item->text());
 }

 void MainWindow::sessionEvent(QListWidgetItem *item)
 {
     currentSession = item->text();
     update_candidate();
     update_participate();
 //        textEdit->setPlainText(item->text());
 }

 void MainWindow::participateEvent(QListWidgetItem *item)
 {
     currentParticipate = item->text();
 }

 void MainWindow::candidateEvent(QListWidgetItem *item)
 {
     currentCandidate = item->text();
 }

 void MainWindow::comboEvent(QString item)
 {
     currentCombo = item;
     update_app();
//     textEdit->setPlainText("change combo:  "+currentCombo);
//     currentSession = item->text();
 }

 void MainWindow::update_app()
 {
    QStringList appSmallList;
    Context::instance()->getApplicationNames(appSmallList);
    appList->clear();
    appList->insertItems(0,appSmallList);
 }

 void MainWindow::update_session()
 {
    QStringList sessionSmallList;
    textEdit->setPlainText("current:  "+currentCombo);
    if (currentCombo == "organizer")
    {
        Context::instance()->getOrganizerSessionNames(currentApp, sessionSmallList);
    }
    else
    {
        if (currentCombo == "participant")
        {
            Context::instance()->getParticipantSessionNames(currentApp, sessionSmallList);
        }
    }
    sessionList->clear();
    sessionList->insertItems(0,sessionSmallList);
 }

 void MainWindow::update_candidate()
 {
     QStringList candidateSmallList;
     if (currentCombo == "organizer")
     {
         Context::instance()->getCandidateNames(currentApp,currentSession,candidateSmallList);
     }
     candidateList->clear();
     candidateList->insertItems(0,candidateSmallList);
 }

 void MainWindow::update_participate()
 {
     QStringList participateSmallList;
     if (currentCombo == "organizer")
     {
         Context::instance()->getParticipantNames(currentApp,currentSession,participateSmallList);
     }
     participateList->clear();
     participateList->insertItems(0,participateSmallList);
 }


 void MainWindow::add_app()
 {
     bool ok;
     update_app();
     QString text = QInputDialog::getText(this, tr("add application"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
     Context::instance()->addApplication(text);
     update_app();
 }

 void MainWindow::delete_app()
 {
     bool ok;
     update_app();
     QString text = QInputDialog::getText(this, tr("delete application"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
     Context::instance()->removeApplication(text);
     update_app();
 }

 void MainWindow::create_session()
 {
     update_session();
     Dialog dialog;
     if(dialog.exec() == QDialog::Accepted){
         QString session = dialog.getSessionData();
         QString org = dialog.getOrgData();
         Application *app_bk = Context::instance()->getApplication(currentApp);
         if (app_bk != NULL)
         {
              app_bk->createOrganizerSession(session, org);
              textEdit->setPlainText(session+"  "+org);
         }
         update_session();
    }
 }

 void MainWindow::participate_session()
 {
     update_session();
     Dialog dialog;
     if(dialog.exec() == QDialog::Accepted){
       // You can access everything you need in dialog object
       QString session = dialog.getSessionData();
       QString pat =dialog.getPatData();
       QString org = dialog.getOrgData();
       Application *app_bk = Context::instance()->getApplication(currentApp);
       if (app_bk != NULL)
       {
            app_bk->createParticipantSession(session,pat,org);
       }
       update_session();
     }
 }

 void MainWindow::join_session()
 {
     update_session();
     Dialog dialog;
     if (currentCombo == "participant")
     {
         if(dialog.exec() == QDialog::Accepted){
           QString session = dialog.getSessionData();
           QString app = dialog.getAppData();
           Application *app_bk = Context::instance()->getApplication(app);
           if (app_bk != NULL)
           {
               ParticipantSession *pSession_bk = app_bk->getParticipantSession(session);
               pSession_bk->join();
           }
           update_session();
         }
     }
 }

 void MainWindow::destroy_session()
 {
     bool ok;
     update_session();
     QString session = QInputDialog::getText(this, tr("destroy session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
     Application *app_bk = Context::instance()->getApplication(currentApp);
     if (app_bk != NULL)
     {
         if (currentCombo == "organizer")
         {
             app_bk->destroyOrganizerSession(session);
         }
         if (currentCombo == "participant")
         {
             app_bk->destroyParticipantSession(session);
         }     }
     update_session();
 }

 void MainWindow::quit_session()
 {
     /*bool ok;
     update_session(currentApp);
     QString session = QInputDialog::getText(this, tr("quit session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
     Application *app_bk = Context::instance()->getApplication(currentApp);
     if (app_bk != NULL)
     {
          app_bk->destroyParticipantSession(session);
     }
     update_session(currentApp);
     */
 }

 void MainWindow::join_membership()
 {
     if (currentCombo == "participant")
     {
         Application *app_bk = Context::instance()->getApplication(currentApp);
         if (app_bk != NULL)
         {
             ParticipantSession *pSession_bk = app_bk->getParticipantSession(currentSession);
             pSession_bk->join();
         }
     }
 }

 void MainWindow::accept_membership()
 {
    if (currentCombo == "organizer")
    {
        Application *app_bk = Context::instance()->getApplication(currentApp);
        if (app_bk != NULL)
        {
            OrganizerSession *oSession_bk = app_bk->getOrganizerSession(currentSession);
            oSession_bk->acceptJoin(currentCandidate);
        }
        update_participate();
    }
 }

 void MainWindow::reject_membership()
 {
     if (currentCombo == "organizer")
     {
         Application *app_bk = Context::instance()->getApplication(currentApp);
         if (app_bk != NULL)
         {
             OrganizerSession *oSession_bk = app_bk->getOrganizerSession(currentSession);
             oSession_bk->rejectJoin(currentCandidate);
         }
     }
 }

 void MainWindow::create_shared_key()
 {
     if (currentCombo == "organizer")
     {
         Application *app_bk = Context::instance()->getApplication(currentApp);
         if (app_bk != NULL)
         {
             OrganizerSession *oSession_bk = app_bk->getOrganizerSession(currentSession);
             oSession_bk->crateSharedKey();
         }
     }
 }

 void MainWindow::renew_shared_key()
 {
     if (currentCombo == "organizer")
     {
         Application *app_bk = Context::instance()->getApplication(currentApp);
         if (app_bk != NULL)
         {
             OrganizerSession *oSession_bk = app_bk->getOrganizerSession(currentSession);
             oSession_bk->renewSharedKey();//how  to get the version
         }
     }
 }

 void MainWindow::fetch_shared_key()
 {
    if (currentCombo == "participant")
     {
         Application *app_bk = Context::instance()->getApplication(currentApp);
         if (app_bk != NULL)
         {
             ParticipantSession *pSession_bk = app_bk->getParticipantSession(currentSession);
             pSession_bk->fetchSharedKey();
         }
    }
 }


void MainWindow::addList()
{
     QListWidgetItem *newItem = new QListWidgetItem;
     newItem->setText("foo");
     appList->insertItem( 0,newItem );
     QListWidgetItem *newItem2 = new QListWidgetItem;
     newItem2->setText("bar");
     appList->insertItem( 0,newItem2 );
//     appList->insertItem( 0,"Foo2" );
 }


 void MainWindow::createActions()
 {
     addApp = new QAction(tr("&Add Application"), this);
     deleteApp = new QAction(tr("&Delete Application"), this);
     createSession = new QAction(tr("&Create Session"), this);;
     participateSession = new QAction(tr("&Participate Application"), this);
     joinSession = new QAction(tr("&Join Session"), this);
     destroySession= new QAction(tr("&Destroy/Quit Session"), this);
  //   quitSession = new QAction(tr("&Quit Session"), this);
     joinMembership = new QAction(tr("&Join Membership"), this);
     acceptMembership = new QAction(tr("&Accept Membership"), this);
     rejectMembership = new QAction(tr("&Reject Membership"), this);
     createSharedKey = new QAction(tr("&Create Shared Key"), this);
     renewSharedKey = new QAction(tr("&Renew Shared Key"), this);
     fetchSharedKey = new QAction(tr("&Fetch Shared Key"), this);
 }

 void MainWindow::creatMenu()
 {
     appMenu = menuBar()->addMenu(tr("&application"));
     appMenu->addAction(addApp);
     connect(addApp, SIGNAL(triggered()), this, SLOT(add_app()));
     appMenu->addAction(deleteApp);
     connect(deleteApp, SIGNAL(triggered()), this, SLOT(delete_app()));

     sessionMenu = menuBar()->addMenu(tr("&Session"));
     sessionMenu->addAction(createSession);
     connect(createSession, SIGNAL(triggered()), this, SLOT(create_session()));
     sessionMenu->addAction(participateSession);
     connect(participateSession, SIGNAL(triggered()), this, SLOT(participate_session()));
     sessionMenu->addAction(joinSession);
     connect(joinSession, SIGNAL(triggered()), this, SLOT(join_session()));
 //    sessionMenu->addAction(quitSession);
 //    connect(quitSession, SIGNAL(triggered()), this, SLOT(quit_session()));
     sessionMenu->addAction(destroySession);
     connect(destroySession, SIGNAL(triggered()), this, SLOT(destroy_session()));

     membershipMenu = menuBar()->addMenu(tr("&Membership"));
     membershipMenu->addAction(joinMembership);
     connect(joinMembership, SIGNAL(triggered()), this, SLOT(join_membership()));
     membershipMenu->addAction(acceptMembership);
     connect(acceptMembership, SIGNAL(triggered()), this, SLOT(accept_membership()));
     membershipMenu->addAction(rejectMembership);
     connect(rejectMembership, SIGNAL(triggered()), this, SLOT(reject_membership()));
     sharedKeyMenu = menuBar()->addMenu(tr("&SharedKey"));
     sharedKeyMenu->addAction(createSharedKey);
     connect(createSharedKey, SIGNAL(triggered()), this, SLOT(create_shared_key()));
     sharedKeyMenu->addAction(renewSharedKey);
     connect(renewSharedKey, SIGNAL(triggered()), this, SLOT(renew_shared_key()));
     sharedKeyMenu->addAction(fetchSharedKey);
     connect(fetchSharedKey, SIGNAL(triggered()), this, SLOT(fetch_shared_key()));
 }

 void MainWindow::createupperBox()
 {
     horizontalGroupBox = new QGroupBox();
     QGridLayout *layout = new QGridLayout;
     appList = new QListWidget(this);
     sessionList = new QListWidget(this);
     candidateList = new QListWidget(this);
     participateList = new QListWidget(this);
     labels[0] = new QLabel(tr("app"));
     labels[1] = new QLabel(tr("session"));
     labels[2] = new QLabel(tr("candidates"));
     labels[3] = new QLabel(tr("participates"));
     buttons[0]= new QPushButton(tr("refresh"));
     buttons[1]= new QPushButton(tr("refresh"));
     buttons[2]= new QPushButton(tr("refresh"));
     comboBox = new QComboBox();
     comboBox->insertItem(0,"organizer");
     comboBox->insertItem(0,"participant");
     layout->addWidget(labels[0],0,0);
     layout->addWidget(labels[1],0,1);
     layout->addWidget(labels[2],0,2);
     layout->addWidget(labels[3],0,3);
     layout->addWidget(appList,1,0);
     layout->addWidget(sessionList,1,1);
     layout->addWidget(candidateList,1,2);
     layout->addWidget(participateList,1,3);
     layout->addWidget(comboBox,2,1);
     layout->addWidget(buttons[0],3,0);
     layout->addWidget(buttons[1],3,1);
     layout->addWidget(buttons[2],3,2);
     connect(appList, SIGNAL(itemClicked(QListWidgetItem *)),
             this, SLOT(appEvent(QListWidgetItem *)));
     connect(sessionList, SIGNAL(itemClicked(QListWidgetItem *)),
             this, SLOT(sessionEvent(QListWidgetItem *)));
     connect(participateList, SIGNAL(itemClicked(QListWidgetItem *)),
             this, SLOT(participateEvent(QListWidgetItem *)));
     connect(candidateList, SIGNAL(itemClicked(QListWidgetItem *)),
             this, SLOT(candidateEvent(QListWidgetItem *)));

     connect(buttons[0], SIGNAL(clicked()), this, SLOT(update_app()));
     connect(buttons[1], SIGNAL(clicked()), this, SLOT(update_session()));
     connect(buttons[2], SIGNAL(clicked()), this, SLOT(update_candidate()));

     connect(comboBox,SIGNAL(activated(QString)),
             this, SLOT(comboEvent(QString)));
     connect(comboBox,SIGNAL(currentIndexChanged(QString)),
             this, SLOT(comboEvent(QString)));
     horizontalGroupBox->setLayout(layout);
 }

#if WAF
#include "mainwindow.moc"
#include "mainwindow.cpp.moc"
#endif

