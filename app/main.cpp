#include "visualizer3d.h"

#include <QtGui>
#include <QApplication>

/**
 * Suppresses debug messages
 */
void messageHandler(QtMsgType type, const char* msg)
{
//#if defined(WIN32) || defined(__APPLE__)
//        if (QLCArgs::dbgBox != NULL)
//            QLCArgs::dbgBox->addText(msg);
//#else
        fprintf(stderr, "%d: %s\n", type, msg);
        fflush(stderr);
//#endif
}

int main(int argc, char *argv[])
{
    /* Handle debug messages */
    qInstallMsgHandler(messageHandler);

    QApplication app(argc, argv);
    Visualizer3D vis;
    vis.show();

    return app.exec();
}
