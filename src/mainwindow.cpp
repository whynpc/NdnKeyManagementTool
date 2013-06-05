#include <QtGui>

 #include "mainwindow.h"
 #include  "dialog.h"
#include "context.h"
 #include <QVector>

 MainWindow::MainWindow()
 {
     createActions();
     creatMenu();
     textEdit = new QPlainTextEdit;
     createupperBox();
     addList();
     //Set mainwindow layout
     QWidget *widget = new QWidget();
     this->setCentralWidget(widget);
     QGridLayout *mainLayout = new QGridLayout;
     mainLayout->addWidget(horizontalGroupBox,0,0);
     mainLayout->addWidget(textEdit,1,0);
     centralWidget()->setLayout(mainLayout);
 }

 void MainWindow::update_app()
 {
    QStringList appSmallList;
    Context::instance()->getApplicationNames(appSmallList);
    appList->clear();
//    appList->insertItems(0,appSmallList);

 }

 void MainWindow::add_app()
 {
     bool ok;
     update_app();
     QString text = QInputDialog::getText(this, tr("add application"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
 //    Context::instance()->addApplication(text);
     update_app();
 }

 void MainWindow::delete_app()
 {
     bool ok;
 //    update_app();
     QString text = QInputDialog::getText(this, tr("delete application"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
  //   Context::instance()->removeApplication(text);
  //   update_app();
 }

 void MainWindow::create_session()
 {
     bool ok;
     QString text = QInputDialog::getText(this, tr("create session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
 }

 void MainWindow::participate_session()
 {
     bool ok;
     QString text = QInputDialog::getText(this, tr("participate session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);
 }

 void MainWindow::join_session()
 {
     bool ok;
     QString text = QInputDialog::getText(this, tr("join session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);

 }

 void MainWindow::destroy_session()
 {
     bool ok;
     QString text = QInputDialog::getText(this, tr("destroy session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);

 }

 void MainWindow::quit_session()
 {
     bool ok;
     QString text = QInputDialog::getText(this, tr("destroy session"),
                                              NULL, QLineEdit::Normal,
                                              NULL, &ok);

 }

 void MainWindow::join_membership()
 {
     Dialog dialog;
     if(dialog.exec() == QDialog::Accepted){
       // You can access everything you need in dialog object
       QString app = dialog.getAppData();
       QString session = dialog.getSessionData();

     }
 }
 void MainWindow::accept_membership()
 {
     Dialog dialog;
     if(dialog.exec() == QDialog::Accepted){
       // You can access everything you need in dialog object
       QString app = dialog.getAppData();
       QString session = dialog.getSessionData();
//       textEdit->setPlainText(app +"  "+session);
     }
 }

 void MainWindow::reject_membership()
 {
     Dialog dialog;
     if(dialog.exec() == QDialog::Accepted){
       // You can access everything you need in dialog object
       QString app = dialog.getAppData();
       QString session = dialog.getSessionData();

     }
 }

 void MainWindow::create_shared_key()
 {
     QString text = "dsafas";
     textEdit->setPlainText(text);
 }

 void MainWindow::renew_shared_key()
 {
     QString text = "dsafas";
     textEdit->setPlainText(text);
 }
 void MainWindow::fetch_shared_key()
 {
     QString text = "dsafas";
     textEdit->setPlainText(text);
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
     destroySession= new QAction(tr("&Delete Session"), this);
     quitSession = new QAction(tr("&Quit Session"), this);
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
     sessionMenu->addAction(destroySession);
     connect(destroySession, SIGNAL(triggered()), this, SLOT(destroy_session()));
     sessionMenu->addAction(quitSession);
     connect(quitSession, SIGNAL(triggered()), this, SLOT(quit_session()));
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
     userList = new QListWidget(this);
     acceptList = new QListWidget(this);
     labels[0] = new QLabel(tr("app"));
     labels[1] = new QLabel(tr("session"));
     labels[2] = new QLabel(tr("users"));
     labels[3] = new QLabel(tr("users(accepted)"));
     comboBox = new QComboBox();
     comboBox->insertItem(0,"organizer");
     comboBox->insertItem(0,"participant");
     layout->addWidget(labels[0],0,0);
     layout->addWidget(labels[1],0,1);
     layout->addWidget(labels[2],0,2);
     layout->addWidget(labels[3],0,3);
     layout->addWidget(appList,1,0);
     layout->addWidget(sessionList,1,1);
     layout->addWidget(userList,1,2);
     layout->addWidget(acceptList,1,3);
     layout->addWidget(comboBox,2,2);
     horizontalGroupBox->setLayout(layout);
 }
