#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QGroupBox;

class Dialog : public QDialog
{
    Q_OBJECT
public:
    Dialog();
    QString getAppData();
    QString getSessionData();
//private slots:
//    void accept();
private:
    void create();
    QGroupBox *gridGroupBox;
    QLabel *appName;
    QLabel *sessionName;
    QLineEdit *app;
    QLineEdit *session;
    QPushButton *button;

};

#endif // DIALOG_H
