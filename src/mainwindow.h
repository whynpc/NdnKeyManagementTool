#ifndef MAINWINDOW_H
 #define MAINWINDOW_H

 #include <QMainWindow>

 class QAction;
 class QMenu;
 class QPlainTextEdit;
 class QPushButton;
 class  QGroupBox;
 class QLabel;
 class  QLineEdit;
 class  QTextEdit;
 class QListWidget;
 class QComboBox;
 class QListWidgetItem;

 class MainWindow : public QMainWindow
 {
     Q_OBJECT

 public:
     MainWindow();

 private slots:
     void update_app();
     void update_session(QString &appname);
     void update_candidate();
     void update_participate();

     void add_app();
     void delete_app();
     void create_session();
     void participate_session();
     void join_session();
     void destroy_session();
     void quit_session();
     void join_membership();
     void accept_membership();
     void reject_membership();
     void create_shared_key();
     void renew_shared_key();
     void fetch_shared_key();

//list_event:
     void appEvent(QListWidgetItem *item);
     void sessionEvent(QListWidgetItem *item);
     void comboEvent(QString item);
     void candidateEvent(QListWidgetItem *item);
     void participateEvent(QListWidgetItem *item);

 private:
     void createupperBox();
     void addList();
     void creatMenu();
     void createActions();
     QPlainTextEdit *textEdit;
     QString curFile;

     enum { NumGridRows = 4, NumButtons = 4 };
     QPushButton *buttons[NumButtons];
     QGroupBox *horizontalGroupBox;
     QTextEdit *smallEditor;
     QGroupBox *gridGroupBox;
     QComboBox *comboBox;
     QLabel *labels[NumGridRows];
     QLineEdit *lineEdits[NumGridRows];
     QListWidget *appList;
     QListWidget *sessionList;
     QListWidget *candidateList;
     QListWidget *participateList;
     QMenu *appMenu;
     QMenu *sessionMenu;
     QMenu *membershipMenu;
     QMenu *sharedKeyMenu;
     QMenu *publicKeyMenu;
     QAction *addApp;
     QAction *deleteApp;
     QAction *createSession;
     QAction *participateSession;
     QAction *joinSession;
     QAction *destroySession;
     QAction *quitSession;
     QAction *joinMembership;
     QAction *acceptMembership;
     QAction *rejectMembership;
     QAction *createSharedKey;
     QAction *renewSharedKey;
     QAction *fetchSharedKey;

     QString currentApp;
     QString currentSession;
     QString currentCombo;
     QString currentParticipate;
     QString currentCandidate;

     QString userName;
 };

 #endif
