#include <QSettings>
#include <QFileInfo>
#include <QKeyEvent>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"


// Singleton Instance
static MainWindow* mainWindowInstance = NULL;

//==============================================================================
// Get Main Window Instance - Static Constructor
//==============================================================================
MainWindow* MainWindow::getInstance()
{
    // Check Singleton Instance
    if (!mainWindowInstance) {
        // Create Singleton Instance
        mainWindowInstance = new MainWindow();
    }

    return mainWindowInstance;
}

//==============================================================================
// Release Instance
//==============================================================================
void MainWindow::release()
{
    // Check Main Window Singleton Instance
    if (mainWindowInstance) {
        // Delete Singleton
        delete mainWindowInstance;
        mainWindowInstance = NULL;
    }
}

//==============================================================================
// Constructor
//==============================================================================
MainWindow::MainWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainWindow)
    , testClient(NULL)
{
    qDebug() << "MainWindow::MainWindow";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();

    // Restore UI
    //restoreUI();
}

//==============================================================================
// Init
//==============================================================================
void MainWindow::init()
{
    qDebug() << "MainWindow::init";

    // Set Panel Name
    ui->leftPanel->panelName = DEFAULT_PANEL_NAME_LEFT;
    // Set Panel Name
    ui->rightPanel->panelName = DEFAULT_PANEL_NAME_RIGHT;

    // Connect Signals
    connect(ui->leftPanel, SIGNAL(exitKeyReleased()), this, SLOT(quitApp()));
    connect(ui->rightPanel, SIGNAL(exitKeyReleased()), this, SLOT(quitApp()));

    connect(ui->leftPanel, SIGNAL(modifierKeysChanged(int)), this, SLOT(modifierKeysChanged(int)));
    connect(ui->rightPanel, SIGNAL(modifierKeysChanged(int)), this, SLOT(modifierKeysChanged(int)));


    // Create Test Client
    testClient = new RemoteFileUtilClient();

    // Connect Signals
    connect(testClient, SIGNAL(fileOpProgress(uint,QString,QString,quint64,quint64,quint64,quint64,int)), this, SLOT(fileOpProgress(uint,QString,QString,quint64,quint64,quint64,quint64,int)));
    connect(testClient, SIGNAL(fileOpFinished(uint,QString,QString,QString,int)), this, SLOT(fileOpFinished(uint,QString,QString,QString,int)));
    connect(testClient, SIGNAL(fileOpError(uint,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,int)));
    connect(testClient, SIGNAL(fileOpNeedConfirm(uint,QString,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,QString,QString,QString)));
    connect(testClient, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString)));

    connect(testClient, SIGNAL(dirListItemFound(uint,QString,QString)), this, SLOT(dirListItemFound(uint,QString,QString)));
    connect(testClient, SIGNAL(dirSizeScanProgress(uint,QString,quint64,quint64,quint64)), this, SLOT(dirSizeScanProgress(uint,QString,quint64,quint64,quint64)));


    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI()
{
    qDebug() << "MainWindow::restoreUI";



    // ...


    // Check Last Focused Panel

    // Set Focus
    ui->leftPanel->setPanelFocus(true);


}

//==============================================================================
// Save Settings
//==============================================================================
void MainWindow::saveSettings()
{
    qDebug() << "MainWindow::saveSettings";

}

//==============================================================================
// Show Window
//==============================================================================
void MainWindow::showWindow()
{
    qDebug() << "MainWindow::showWindow";

    // Restore UI
    restoreUI();

    // Show
    show();
}

//==============================================================================
// Show About
//==============================================================================
void MainWindow::showAbout()
{
    qDebug() << "MainWindow::showAbout";

}

//==============================================================================
// Show Preferences
//==============================================================================
void MainWindow::showPreferences()
{
    qDebug() << "MainWindow::showPreferences";

}

//==============================================================================
// Quit Application
//==============================================================================
void MainWindow::quitApp()
{
    qDebug() << "MainWindow::quitApp";

    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Shut Down
//==============================================================================
void MainWindow::shutDown()
{
    qDebug() << "MainWindow::shutDown";

    // Check Test Client
    if (testClient) {
        // Close
        testClient->close();
        // Delete Test Client
        delete testClient;
        testClient = NULL;
    }
}

//==============================================================================
// Modifier Keys Changed Slot
//==============================================================================
void MainWindow::modifierKeysChanged(const int& aModifiers)
{
    qDebug() << "MainWindow::modifierKeysChanged - aModifiers: " << aModifiers;

    // Check Modifier Keys
    if (modifierKeys != aModifiers) {
        // Set Modifier Keys
        modifierKeys = aModifiers;

        // Update Function Keys
        updateFunctionKeys();
    }
}

//==============================================================================
// Update Function Keys
//==============================================================================
void MainWindow::updateFunctionKeys()
{
    //qDebug() << "MainWindow::updateFunctionKeys";

    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - SHIFT";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr("Rename"));
        ui->makeDirButton->setText(tr(""));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::ControlModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - CONTROL";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr("Sort by Name"));
        ui->editButton->setText(tr("Sort by Ext"));
        ui->copyButton->setText(tr("Sort by Size"));
        ui->moveButton->setText(tr("Sort by Date"));
        ui->makeDirButton->setText(tr(""));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::AltModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - ALT";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr(""));
        ui->makeDirButton->setText(tr("Search"));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else if (modifierKeys & Qt::MetaModifier) {
        qDebug() << "MainWindow::updateFunctionKeys - META";

        // Set Button Text
        ui->helpButton->setText(tr(""));
        ui->terminalButton->setText(tr(""));
        ui->viewButton->setText(tr(""));
        ui->editButton->setText(tr(""));
        ui->copyButton->setText(tr(""));
        ui->moveButton->setText(tr(""));
        ui->makeDirButton->setText(tr(""));
        ui->delButton->setText(tr(""));
        ui->optionsButton->setText(tr(""));
        ui->exitButton->setText(tr(""));

    } else {
        qDebug() << "MainWindow::updateFunctionKeys";

        // Set Button Text
        ui->helpButton->setText(tr("Help"));
        ui->terminalButton->setText(tr("Terminal"));
        ui->viewButton->setText(tr("View"));
        ui->editButton->setText(tr("Edit"));
        ui->copyButton->setText(tr("Copy"));
        ui->moveButton->setText(tr("Move"));
        ui->makeDirButton->setText(tr("MakeDir"));
        ui->delButton->setText(tr("Delete"));
        ui->optionsButton->setText(tr("Options"));
        ui->exitButton->setText(tr("Exit"));

    }
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void MainWindow::fileOpProgress(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aCurrFilePath,
                                const quint64& aCurrProgress,
                                const quint64& aCurrTotal,
                                const quint64& aOverallProgress,
                                const quint64& aOverallTotal,
                                const int& aSpeed)
{
    qDebug() << "MainWindow::fileOpProgress - aID: " << aID << " - aOp: " << aOp;

    // ...
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void MainWindow::fileOpFinished(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aSource,
                                const QString& aTarget,
                                const int& aError)
{
    qDebug() << "MainWindow::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aError: " << aError;

    // ...

}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void MainWindow::fileOpError(const unsigned int& aID,
                             const QString& aOp,
                             const QString& aSource,
                             const QString& aTarget,
                             const int& aError)
{
    qDebug() << "MainWindow::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aError: " << aError;

    // ...

}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void MainWindow::fileOpNeedConfirm(const unsigned int& aID,
                                   const QString& aOp,
                                   const QString& aCode,
                                   const QString& aSource,
                                   const QString& aTarget)
{
    qDebug() << "MainWindow::fileOpNeedConfirm - aID: " << aID << " - aOp: " << aOp << " - aCode: " << aCode;

    // ...

}

//==============================================================================
// Dir Size Scan Progress Slot
//==============================================================================
void MainWindow::dirSizeScanProgress(const unsigned int& aID,
                                     const QString& aPath,
                                     const quint64& aNumDirs,
                                     const quint64& aNumFiles,
                                     const quint64& aScannedSize)
{
    qDebug() << "MainWindow::dirSizeScanProgress - aID: " << aID << " - aPath: " << aPath << " - aNumDirs: " << aNumDirs << " - aNumFiles: " << aNumFiles << " - aScannedSize: " << aScannedSize;

    // ...

}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void MainWindow::dirListItemFound(const unsigned int& aID,
                                  const QString& aPath,
                                  const QString& aFileName)
{
    qDebug() << "MainWindow::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - aFileName: " << aFileName;

    // ...

}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void MainWindow::fileOpQueueItemFound(const unsigned int& aID,
                                      const QString& aOp,
                                      const QString& aSource,
                                      const QString& aTarget)
{
    qDebug() << "MainWindow::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp;

    // ...

}

//==============================================================================
// Key Press Event
//==============================================================================
void MainWindow::keyPressEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check If Repeat
        if (!aEvent->isAutoRepeat()) {

            // Switch

        } else {

        }

    }
}

//==============================================================================
// Key Release Event
//==============================================================================
void MainWindow::keyReleaseEvent(QKeyEvent* aEvent)
{
    // Check Event
    if (aEvent) {

    }
}

//==============================================================================
// Help Button Clicked Slot
//==============================================================================
void MainWindow::on_helpButton_clicked()
{
    //qDebug() << "MainWindow::on_helpButton_clicked";

    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Help Action
        //ui->actionHelp->trigger();

        // Check Test Client
        if (testClient) {

            qDebug() << "#### testClient - Connect";

            // Launch Server Test
            testClient->launchServerTest();
        }
    }
}

//==============================================================================
// Terminal Button Clicked Slot
//==============================================================================
void MainWindow::on_terminalButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Terminal Action
        //ui->actionTerminal->trigger();
    }
}

//==============================================================================
// View Button Clicked Slot
//==============================================================================
void MainWindow::on_viewButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Edit Button Clicked Slot
//==============================================================================
void MainWindow::on_editButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Copy Button Clicked Slot
//==============================================================================
void MainWindow::on_copyButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Move Button Clicked Slot
//==============================================================================
void MainWindow::on_moveButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Make Dir Button Clicked Slot
//==============================================================================
void MainWindow::on_makeDirButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Delete Button Clicked Slot
//==============================================================================
void MainWindow::on_delButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Options Button Clicked Slot
//==============================================================================
void MainWindow::on_optionsButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {

    }
}

//==============================================================================
// Exit Button Clicked Slot
//==============================================================================
void MainWindow::on_exitButton_clicked()
{
    // Check Modifier Keys
    if (modifierKeys & Qt::ShiftModifier) {

    } else if (modifierKeys & Qt::ControlModifier) {

    } else if (modifierKeys & Qt::AltModifier) {

    } else if (modifierKeys & Qt::MetaModifier) {

    } else {
        // Trigger Action Exit
        ui->actionExit->trigger();
    }
}

//==============================================================================
// Action About Triggered Slot
//==============================================================================
void MainWindow::on_actionAbout_triggered()
{
    // Show About
    showAbout();
}

//==============================================================================
// Action Preferences Triggered Slot
//==============================================================================
void MainWindow::on_actionPreferences_triggered()
{
    // Show Preferences
    showPreferences();
}

//==============================================================================
// Action Help Triggered Slot
//==============================================================================
void MainWindow::on_actionHelp_triggered()
{
    // Show Help
}

//==============================================================================
// Action Options Triggered Slot
//==============================================================================
void MainWindow::on_actionOptions_triggered()
{
    // Show Options
}

//==============================================================================
// Action Compare Files Triggered Slot
//==============================================================================
void MainWindow::on_actionCompare_Files_triggered()
{
    // Compare Files
}

//==============================================================================
// Action Select All Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_all_triggered()
{
    // Select All Files In Focused Panel
}

//==============================================================================
// Action Select None Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_None_triggered()
{
    // Deselect All Files In Focused Panel
}

//==============================================================================
// Action Terminal Triggered Slot
//==============================================================================
void MainWindow::on_actionTerminal_triggered()
{
    // Launch Terminal
}

//==============================================================================
// Action Sort By Name Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Name_triggered()
{
    // Sort Focused Panel Items By Name
}

//==============================================================================
// Action Sort By Extension Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Extension_triggered()
{
    // Sort Focused Panel Items By Extension

}

//==============================================================================
// Action Sort By Type Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Type_triggered()
{
    // Sort Focused Panel Items By Type

}

//==============================================================================
// Action Sort By Size Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Size_triggered()
{
    // Sort Focused Panel Items By Size

}

//==============================================================================
// Action Sort By Date Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Date_triggered()
{
    // Sort Focused Panel Items By Date

}

//==============================================================================
// Action Sort By Owner Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Owner_triggered()
{
    // Sort Focused Panel Items By Owner

}

//==============================================================================
// Action Sort By Permission Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Permissions_triggered()
{
    // Sort Focused Panel Items By Permission

}

//==============================================================================
// Action Toggle Hidden Files Triggered Slot
//==============================================================================
void MainWindow::on_actionShow_Hide_Hiden_triggered()
{

}

//==============================================================================
// Action New Directory Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_Directory_triggered()
{

}

//==============================================================================
// Action New File Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_File_triggered()
{

}

//==============================================================================
// Action Delete File Triggered Slot
//==============================================================================
void MainWindow::on_actionDelete_File_triggered()
{

}

//==============================================================================
// Action Create Link Triggered Slot
//==============================================================================
void MainWindow::on_actionCreate_Link_triggered()
{

}

//==============================================================================
// Action Quit Triggered Slot
//==============================================================================
void MainWindow::on_actionQuit_triggered()
{
    // Quit Application
    quitApp();
}

//==============================================================================
// Action Exit Triggered Slot
//==============================================================================
void MainWindow::on_actionExit_triggered()
{
    // Quit App
    quitApp();
}

//==============================================================================
// Destructor
//==============================================================================
MainWindow::~MainWindow()
{
    // Save Settings
    saveSettings();

    // Shut Down
    shutDown();

    // Delete UI
    delete ui;

    qDebug() << "MainWindow::~MainWindow";
}



