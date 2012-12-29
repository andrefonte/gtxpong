#include <QtWidgets/QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication GTxPong(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return GTxPong.exec();
}
