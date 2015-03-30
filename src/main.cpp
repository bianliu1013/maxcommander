
#include <QApplication>

#include <mcwinterface.h>

#include "mainwindow.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
    // Init Application
    QApplication app(argc, argv);

    // Set Application Name
    app.setApplicationName(DEFAULT_APPLICATION_NAME);
    // Set Organization Name
    app.setOrganizationName(DEFAULT_ORGANIZATION_NAME);
    // Set Organization Domain
    app.setOrganizationDomain(DEFAULT_ORGANIZATION_DOMAIN);

    // Store App Exec Path
    storeAppExecPath(argv[0]);

    // Init Main Window
    MainWindow* mainWindow = MainWindow::getInstance();

    // Show Main Window
    mainWindow->showWindow();

    // Execute App
    int result = app.exec();

    // Release Main Window Instance
    mainWindow->release();

    return result;
}


