#include <QApplication>
#include <iostream>
#include <fstream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	std::ofstream ofs("log.txt");
	std::clog.rdbuf(ofs.rdbuf());
	 


    QApplication app(argc, argv);
    app.setOrganizationName("Trolltech");
    app.setApplicationName("Application Example");
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}

