
/**
 * @file /src/main.cpp
 *
 * @brief Qt based gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QtGui>

#include "loginwidget.h"

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VizlibTest w(argc, argv);
    w.show();

    return a.exec();
}
