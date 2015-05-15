#include <QSettings>
#include <QFileInfo>
#include <QKeyEvent>
#include <QDir>
#include <QDebug>

#include <QDialogButtonBox>
#include <mcwinterface.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "preferencesdialog.h"
#include "createdirdialog.h"
#include "deletefiledialog.h"
#include "deleteprogressdialog.h"
#include "transferfiledialog.h"
#include "transferprogressdialog.h"
#include "helpwindow.h"
#include "viewerwindow.h"
#include "searchdialog.h"
#include "remotefileutilclient.h"
#include "infodialog.h"
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
    , viewMenuActionGroup(new QActionGroup(NULL))
    , leftPanel(NULL)
    , rightPanel(NULL)
    , focusedPanel(NULL)
    , modifierKeys(Qt::NoModifier)
    , aboutDialog(NULL)
    , preferencesDialog(NULL)
    , createDirDialog(NULL)
    , deleteFileDialog(NULL)
    , transferFileDialog(NULL)
    , helpWindow(NULL)
    , searchFileDialog(NULL)

{
    qDebug() << "MainWindow::MainWindow";

    // Setup UI
    ui->setupUi(this);

    // Add Actions To Action Group
    viewMenuActionGroup->addAction(ui->actionSort_by_Name);
    viewMenuActionGroup->addAction(ui->actionSort_by_Extension);
    viewMenuActionGroup->addAction(ui->actionSort_by_Type);
    viewMenuActionGroup->addAction(ui->actionSort_by_Size);
    viewMenuActionGroup->addAction(ui->actionSort_by_Date);
    viewMenuActionGroup->addAction(ui->actionSort_by_Owner);
    viewMenuActionGroup->addAction(ui->actionSort_by_Permissions);
    // Set Exclusive
    viewMenuActionGroup->setExclusive(true);

    // Set Left Panel
    leftPanel  = ui->leftPanel;
    rightPanel = ui->rightPanel;

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
    leftPanel->setPanelName(DEFAULT_PANEL_NAME_LEFT);
    // Set Panel Name
    rightPanel->setPanelName(DEFAULT_PANEL_NAME_RIGHT);

    // Connect Signals
    connect(leftPanel, SIGNAL(exitKeyReleased()), this, SLOT(quitApp()));
    connect(rightPanel, SIGNAL(exitKeyReleased()), this, SLOT(quitApp()));

    connect(leftPanel, SIGNAL(modifierKeysChanged(int)), this, SLOT(modifierKeysChanged(int)));
    connect(rightPanel, SIGNAL(modifierKeysChanged(int)), this, SLOT(modifierKeysChanged(int)));

    connect(leftPanel, SIGNAL(focusedPanelChanged(FilePanel*)), this, SLOT(focusedPanelChanged(FilePanel*)));
    connect(rightPanel, SIGNAL(focusedPanelChanged(FilePanel*)), this, SLOT(focusedPanelChanged(FilePanel*)));

    // ...

    connect(leftPanel, SIGNAL(launchTerminal(QString)), this, SLOT(launchTerminal(QString)));
    connect(rightPanel, SIGNAL(launchTerminal(QString)), this, SLOT(launchTerminal(QString)));

    connect(leftPanel, SIGNAL(launchViewer(bool)), this, SLOT(launchViewer(bool)));
    connect(rightPanel, SIGNAL(launchViewer(bool)), this, SLOT(launchViewer(bool)));

    connect(leftPanel, SIGNAL(launchFileCopy()), this, SLOT(launchFileCopy()));
    connect(rightPanel, SIGNAL(launchFileCopy()), this, SLOT(launchFileCopy()));

    connect(leftPanel, SIGNAL(launchFileMove()), this, SLOT(launchFileMove()));
    connect(rightPanel, SIGNAL(launchFileMove()), this, SLOT(launchFileMove()));

    connect(leftPanel, SIGNAL(launchCreateDir()), this, SLOT(launchCreateDir()));
    connect(rightPanel, SIGNAL(launchCreateDir()), this, SLOT(launchCreateDir()));

    connect(leftPanel, SIGNAL(launchSearch()), this, SLOT(launchSearch()));
    connect(rightPanel, SIGNAL(launchSearch()), this, SLOT(launchSearch()));

    connect(leftPanel, SIGNAL(launchDelete()), this, SLOT(launchDelete()));
    connect(rightPanel, SIGNAL(launchDelete()), this, SLOT(launchDelete()));

    connect(leftPanel, SIGNAL(showPreferences()), this, SLOT(showPreferences()));
    connect(rightPanel, SIGNAL(showPreferences()), this, SLOT(showPreferences()));

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainWindow::restoreUI(const bool& aReload, const int& aFocusedPanel)
{
    qDebug() << "MainWindow::restoreUI - aReload: " << aReload;

    // Check Left Panel
    if (leftPanel) {
        // Restore Left Panel UI
        leftPanel->restoreUI(aReload);
    }

    // Check Right Panel
    if (rightPanel) {
        // Restore Right Panel UI
        rightPanel->restoreUI(aReload);
    }

    // Check FocusedPanel
    if (aFocusedPanel == 0) {
        // Set Focus
        leftPanel->setPanelFocus(true);
    } else {
        // Set Focus
        rightPanel->setPanelFocus(true);
    }

    // Update Function Keys
    updateFunctionKeys();

    // Update Menu
    updateMenu();
}

//==============================================================================
// Load Settings
//==============================================================================
void MainWindow::loadSettings()
{
    qDebug() << "MainWindow::loadSettings";

    // ...
}

//==============================================================================
// Save Settings
//==============================================================================
void MainWindow::saveSettings()
{
    qDebug() << "MainWindow::loadSettings";

    // ...
}

//==============================================================================
// Show Window
//==============================================================================
void MainWindow::showWindow()
{
    qDebug() << "MainWindow::showWindow";

    // Load Settings
    loadSettings();

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

    // Check About Dialog
    if (!aboutDialog) {
        // Create About Dialog
        aboutDialog = new AboutDialog();
    }

    // Exec About Dialog
    aboutDialog->exec();
}

//==============================================================================
// Show Help
//==============================================================================
void MainWindow::showHelp()
{
    qDebug() << "MainWindow::showHelp";

    // Check Help Window
    if (!helpWindow) {
        // Ceate Help Window
        helpWindow = new HelpWindow();
    }

    // Load Content
    helpWindow->loadContent();

    // Show Help Window
    helpWindow->showWindow();
}

//==============================================================================
// Launch Terminal
//==============================================================================
void MainWindow::launchTerminal(const QString& aDirPath)
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    qDebug() << "MainWindow::launchTerminal - aDirPath: " << aDirPath;

    // Init Settings
    QSettings settings;

    // Get Terminal App
    QString terminalApp = settings.value(SETTINGS_KEY_APPS_TERMINAL).toString();

    // Check Terminal App
    if (!terminalApp.isEmpty()) {
        // Init Args
        QStringList args;
        // Add Current Dir To Args
        args << focusedPanel->getCurrentDir();

        // Launch App
        launchApp(terminalApp, args, focusedPanel->getCurrentDir());
    }

    // ...
}

//==============================================================================
// Launch Viewer
//==============================================================================
void MainWindow::launchViewer(const bool& aEditMode, const bool& aNewFile)
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Get File info
    QFileInfo fileInfo = focusedPanel->getCurrFileInfo();

    // Check File Type
    if (fileInfo.isDir() || fileInfo.isBundle() || fileInfo.isSymLink()) {
        qDebug() << "MainWindow::launchViewer - aEditMode: " << aEditMode << " - NO SELECTED FILE!";

        // Launch Dir Quick View Maybe...

        // ...

        return;
    }

    qDebug() << "MainWindow::launchViewer - aEditMode: " << aEditMode;


    // Check Settings For Using External Viewer


    // Create New Viewer Window
    ViewerWindow* newViewer = new ViewerWindow();

    // Set Edit Mode
    newViewer->setEditModeEnabled(aEditMode);

    // Check New File
    if (aNewFile) {
        // New File
        if (!newViewer->newFile(focusedPanel->getCurrentDir())) {
            // Show Info - Can't Save
            InfoDialog warningDialog(tr(DEFAULT_WARNING_TEXT_CANT_CREATE_NEW_FILE), EIDTWarning);
            // Exec Dialog
            warningDialog.exec();

            // Delete New Viewer
            delete newViewer;

            return;
        }
    } else {
        // Load File
        if (!newViewer->loadFile(focusedPanel->getCurrFileInfo().absoluteFilePath(), focusedPanel->getPanelName())) {

            // Show Info - Unsupported Format
            InfoDialog warningDialog(tr(DEFAULT_WARNING_TEXT_UNSUPPORTED_FILE_FORMAT), EIDTWarning);
            // Exec Dialog
            warningDialog.exec();

            // Delete New Viewer
            delete newViewer;

            return;
        }
    }

    // Connect Signal
    connect(newViewer, SIGNAL(viewerClosed(ViewerWindow*)), this, SLOT(viewerWindowClosed(ViewerWindow*)));

    // Add To Viewer List
    viewerWindows << newViewer;

    // Show Window
    newViewer->showWindow();
}

//==============================================================================
// Launch File Copy
//==============================================================================
void MainWindow::launchFileCopy()
{
    //qDebug() << "MainWindow::launchFileCopy";

    // Launch Transfer
    launchTransfer(DEFAULT_OPERATION_COPY_FILE);
}

//==============================================================================
// Launch File Move/REname
//==============================================================================
void MainWindow::launchFileMove()
{
    //qDebug() << "MainWindow::launchFileMove";

    // Launch Transfer
    launchTransfer(DEFAULT_OPERATION_MOVE_FILE);
}

//==============================================================================
// Launch Transfer
//==============================================================================
void MainWindow::launchTransfer(const QString& aOperation)
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Check Transfer File Dialog
    if (!transferFileDialog) {
        // Create Transfer File Dialog
        transferFileDialog = new TransferFileDialog();
    }

    // Check Operation
    if (aOperation == DEFAULT_OPERATION_COPY_FILE) {
        // Set Title
        transferFileDialog->setWindowTitle(tr(DEFAULT_TITLE_COPY_FILES));
    } else if (aOperation == DEFAULT_OPERATION_MOVE_FILE) {
        // Set Title
        transferFileDialog->setWindowTitle(tr(DEFAULT_TITLE_MOVE_FILES));
    }

    // Init Selected Files
    QStringList selectedFiles = focusedPanel->getSelectedFiles();

    // Init Transfer Source Dir
    QString transferSource = "";
    // Init Transfer Target Dir
    QString transferTarget = "";

    // Check Focused Panel - Left
    if (focusedPanel == leftPanel) {

        // Set Transfer Source
        transferSource = leftPanel->getCurrentDir();
        // Set Transfer Target
        transferTarget = rightPanel->getCurrentDir();

    // Check Focused Panel - Right
    } else if (focusedPanel == rightPanel) {

        // Set Transfer Source
        transferSource = rightPanel->getCurrentDir();
        // Set Transfer Target
        transferTarget = leftPanel->getCurrentDir();

    }

    // Setup Delete File Dialog - Copy

    // Check Selected Files Count
    if (selectedFiles.count() > 1) {

        // Set Transfer Dialog Source File Text
        transferFileDialog->setSourceFileText(transferSource);
        // Set Transfer Dialog Target File Text
        transferFileDialog->setTargetFileText(transferTarget);

    } else if (selectedFiles.count() == 1) {

        // Check Transfer Source
        if (!transferSource.endsWith("/")) {
            // Adjust Transfer Source
            transferSource += "/";
        }

        // Check Transfer Target
        if (!transferTarget.endsWith("/")) {
            // Adjust Transfer Target
            transferTarget += "/";
        }

        // Set Transfer Dialog Source File Text
        transferFileDialog->setSourceFileText(transferSource + selectedFiles[0], true);
        // Set Transfer Dialog Target File Text
        transferFileDialog->setTargetFileText(transferTarget + selectedFiles[0]);

    } else {

        qDebug() << "MainWindow::launchTransfer - aOperation: " << aOperation << " - NO SELECTED FILE!";

        // Nothing Selected

        // ...

        return;
    }

    qDebug() << "MainWindow::launchTransfer - aOperation: " << aOperation << " - count: " << selectedFiles.count();

    // ...

    // Launch Dialog
    if (transferFileDialog->exec()) {
        // Create Transfer Progress Dialog
        TransferProgressDialog* newTransferProgressDialog = new TransferProgressDialog(aOperation);

        // Connect Closed Signal
        connect(newTransferProgressDialog, SIGNAL(dialogClosed(TransferProgressDialog*)), this, SLOT(transferProgressClosed(TransferProgressDialog*)));

        // Add To Transfer Progress Dialog List
        transferProgressDialogs << newTransferProgressDialog;

        // Check Selected Files Count
        if (selectedFiles.count() == 1) {
            // Launch Progress Dialog
            newTransferProgressDialog->launch(transferFileDialog->getSourceFileText(), transferFileDialog->getTargetFileText());
        } else {
            // Launch Progress Dialog
            newTransferProgressDialog->launch(transferSource, transferTarget, selectedFiles);
        }

        // Clear Selected Files
        focusedPanel->deselectAllFiles();
    }
}

//==============================================================================
// Launch Create Dir
//==============================================================================
void MainWindow::launchCreateDir()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    qDebug() << "MainWindow::launchCreateDir";

    // Check CreateDirDialog
    if (!createDirDialog) {
        // Create Create Dir Dialog
        createDirDialog = new CreateDirDialog();
    }

    // Init Dir Path
    QString dirPath = focusedPanel->getCurrentDir();
    // Chekc Dir Path
    if (!dirPath.endsWith("/")) {
        // Adjust Dir Path
        dirPath += "/";
    }

    // Setup Dialog
    createDirDialog->setDirPath(dirPath);

    // ...

    // Launch Dialog
    if (createDirDialog->exec()) {
        // Get Dir Path
        dirPath = createDirDialog->getDirectory();

        // Create Directory
        focusedPanel->createDir(dirPath);
    }
}

//==============================================================================
// Launch Search Slot
//==============================================================================
void MainWindow::launchSearch()
{
    // Clear Modifier Keys
    modifierKeys = Qt::NoModifier;
    // Update Function Keys
    updateFunctionKeys();

    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Reset Modifier Keys
    focusedPanel->modifierKeys = Qt::NoModifier;

    // Check Search Dialog
    if (!searchFileDialog) {
        // Create Search File Dialog
        searchFileDialog = new SearchDialog();
    }

    // Check If Dialog Shown
    if (!searchFileDialog->isDialogShown()) {
        qDebug() << "MainWindow::launchSearch";
        // Init Dir Path
        QString dirPath = focusedPanel->getCurrentDir();
        // Show Dialog
        searchFileDialog->showDialog(dirPath);
    }
}

//==============================================================================
// Launch Create Link
//==============================================================================
void MainWindow::launchCreateLink()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    qDebug() << "MainWindow::launchCreateLink";

    // ...

}

//==============================================================================
// Launch Delete
//==============================================================================
void MainWindow::launchDelete()
{
    // Check Focused Panel
    if (!focusedPanel) {
        return;
    }

    // Get Number Of Transfer Progress Dialogs
    int numTransferProgressDialogs = transferProgressDialogs.count();
    // Get number Of Delete Progress Dialogs
    int numDeleteProgressDialogs = deleteProgressDialogs.count();

    // Check Number Of Progress Dialogs
    if ((numTransferProgressDialogs + numDeleteProgressDialogs) >= MAX_NUMBER_OF_FILE_THREADS) {
        qWarning() << "MainWindow::launchDelete - MAX NUMBER OF THREADS REACHED!!";

        return;
    }

    // Check Delete File Dialog
    if (!deleteFileDialog) {
        // Create Delete File Dialog
        deleteFileDialog = new DeleteFileDialog();
    }

    // Get Selected File List
    QStringList selectedFileList = focusedPanel->getSelectedFiles();

    // Check Selected File List
    if (selectedFileList.count() == 0) {
        qDebug() << "MainWindow::launchDelete - NO SELECTED FILE!";

        return;
    }

    qDebug() << "MainWindow::launchDelete";

    // Get Focused Panel Current Dir
    QString currDir = focusedPanel->getCurrentDir();

    // Check Current Dir
    if (!currDir.endsWith("/")) {
        // Adjust Current Dir
        currDir += "/";
    }

    // Check Selected File List
    if (selectedFileList.count() == 1) {
        // Setup Delete File Dialog
        deleteFileDialog->setFileName(currDir + selectedFileList[0]);
    } else {
        // Setup Delete File Dialog
        deleteFileDialog->setFileName(currDir);
    }

    // ...

    // Launch Dialog
    if (deleteFileDialog->exec()) {

        // Create Delete Progress Dialog
        DeleteProgressDialog* newDialog = new DeleteProgressDialog();

        // Connect Signal
        connect(newDialog, SIGNAL(dialogClosed(DeleteProgressDialog*)), this, SLOT(deleteProgressClosed(DeleteProgressDialog*)));

        // Add To Delete Progress Dialog List
        deleteProgressDialogs << newDialog;

        // Launch
        newDialog->launch(focusedPanel->getCurrentDir(), focusedPanel->getSelectedFiles());

        // Clear Selected Files
        focusedPanel->deselectAllFiles();
    }
}

//==============================================================================
// Show Preferences
//==============================================================================
void MainWindow::showPreferences()
{
    qDebug() << "MainWindow::showPreferences";

    // Check Preferences Dialog
    if (!preferencesDialog) {
        // Create Preferences Dialog
        preferencesDialog = new PreferencesDialog();

        // Connect Signals
        connect(preferencesDialog, SIGNAL(settingsHasChanged()), this, SLOT(settingsHasChanged()));
    }

    // Show Dialog
    preferencesDialog->execDialog();
}

//==============================================================================
// Settings Has Changed Slot
//==============================================================================
void MainWindow::settingsHasChanged()
{
    qDebug() << "MainWindow::settingsHasChanged";

    // Restore UI
    restoreUI(false, (focusedPanel == leftPanel) ? 0 : 1);

    // Check Left Panel
    if (leftPanel) {
        // Set Last File Name
        leftPanel->lastFileName = leftPanel->getCurrFileInfo().fileName();
        // Reload
        leftPanel->reload();
    }

    // Check Right Panel
    if (rightPanel) {
        // Set Last File Name
        rightPanel->lastFileName = rightPanel->getCurrFileInfo().fileName();
        // Reload
        rightPanel->reload();
    }
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

    // Save Settings
    saveSettings();

    // Abort & Clear Transfer Progress Dialogs
    while (transferProgressDialogs.count() > 0) {
        // Take Last Dialog
        TransferProgressDialog* lastDialog = transferProgressDialogs.takeLast();
        // Abort
        lastDialog->abort();
        // Delete Last Dialog
        delete lastDialog;
    }

    // Abort & Clear Delete Progress Dialogs
    while (deleteProgressDialogs.count() > 0) {
        // Take Last Dialog
        DeleteProgressDialog* lastDialog = deleteProgressDialogs.takeLast();
        // Abort
        lastDialog->abort();
        // Delete Last Dialog
        delete lastDialog;
    }

    // Close Viewer Windows
    while (viewerWindows.count() > 0) {
        // Take Last
        ViewerWindow* lastWindow = viewerWindows.takeLast();
        try {
            // Delete Last Window
            delete lastWindow;
        } catch (...) {
            qCritical() << "#### MainWindow::shutDown - ERROR DELETING VIEWER WINDOW!";
        }
    }
}

//==============================================================================
// Modifier Keys Changed Slot
//==============================================================================
void MainWindow::modifierKeysChanged(const int& aModifiers)
{
    //qDebug() << "MainWindow::modifierKeysChanged - aModifiers: " << aModifiers;

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
        //qDebug() << "MainWindow::updateFunctionKeys - SHIFT";

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
        //qDebug() << "MainWindow::updateFunctionKeys - CONTROL";

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
        //qDebug() << "MainWindow::updateFunctionKeys - ALT";

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
        //qDebug() << "MainWindow::updateFunctionKeys - META";

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
        //qDebug() << "MainWindow::updateFunctionKeys";

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
// Update Menu
//==============================================================================
void MainWindow::updateMenu()
{
    //qDebug() << "MainWindow::updateMenu";

    // Check Focused Panel
    if (focusedPanel) {

        // Switch Sorting
        switch (focusedPanel->getSorting()) {
            default:
            case DEFAULT_SORT_NAME:     ui->actionSort_by_Name->setChecked(true); break;
            case DEFAULT_SORT_EXT:      ui->actionSort_by_Extension->setChecked(true); break;
            case DEFAULT_SORT_TYPE:     ui->actionSort_by_Type->setChecked(true); break;
            case DEFAULT_SORT_SIZE:     ui->actionSort_by_Size->setChecked(true); break;
            case DEFAULT_SORT_DATE:     ui->actionSort_by_Date->setChecked(true); break;
            case DEFAULT_SORT_OWNER:    ui->actionSort_by_Owner->setChecked(true); break;
            case DEFAULT_SORT_PERMS:    ui->actionSort_by_Permissions->setChecked(true); break;
            case DEFAULT_SORT_ATTRS:    break;
        }

        // Set Show Hidden Checked
        ui->actionShow_Hide_Hiden->setChecked(focusedPanel->getShowHidden());
    }
}

//==============================================================================
// Toggle Hidden Files
//==============================================================================
void MainWindow::toggleHiddenFile()
{
    qDebug() << "MainWindow::toggleHiddenFile";

    // Get Show Hidden
    bool showHidden = focusedPanel ? focusedPanel->getShowHidden() : true;

    // Check Left Panel
    if (leftPanel) {
        // Set Show Hidden
        leftPanel->setShowHidden(!showHidden);
    }

    // Check Right Panel
    if (rightPanel) {
        // Set Show Hidden
        rightPanel->setShowHidden(!showHidden);
    }

}

//==============================================================================
// Viewer Window Closed Slot
//==============================================================================
void MainWindow::viewerWindowClosed(ViewerWindow* aViewer)
{
    // Check Viewer
    if (aViewer) {
        qDebug() << "MainWindow::viewerWindowClosed - aViewer: " << aViewer;

        // Get Viewer Windows Count
        int vwCount = viewerWindows.count();
        // Go Thru Viewer Windows
        for (int i=0; i<vwCount; ++i) {
            // Get Viewer Window
            ViewerWindow* window = viewerWindows[i];
            // Check Window
            if (window == aViewer) {
                // Remove From Window List
                viewerWindows.removeAt(i);

                try {
                    // Delete Viewer Window
                    delete window;
                } catch (...) {
                    qCritical() << "#### MainWindow::viewerWindowClosed - ERROR DELETING WIEVER WINDOW!!";
                }

                return;
            }
        }
    }
}

//==============================================================================
// Delete Progress Window Closed Slot
//==============================================================================
void MainWindow::deleteProgressClosed(DeleteProgressDialog* aDeleteProgressDialog)
{
    // Check Progress Dialog
    if (aDeleteProgressDialog) {
        qDebug() << "MainWindow::deleteProgressClosed - aDeleteProgressDialog: " << aDeleteProgressDialog;

        // Get Delete Progress Dialogs Count
        int dpdCount = deleteProgressDialogs.count();

        // Go Thru Dialogs
        for (int i=0; i<dpdCount; ++i) {
            // Get Dialog
            DeleteProgressDialog* dialog = deleteProgressDialogs[i];
            // Check Dialog
            if (dialog == aDeleteProgressDialog) {

                // Check Left Panel
                if (leftPanel /*&& (leftPanel->dwDirChanged || leftPanel->dwFileChanged)*/) {
                    // Check Current Dir
                    if (dialog->dirPath == leftPanel->currentDir) {
                        // Reload
                        leftPanel->reload(leftPanel->currentIndex);
                    }
                }

                // Check Right Panel
                if (rightPanel /*&& (rightPanel->dwDirChanged || rightPanel->dwFileChanged)*/) {
                    // Check Current Dir
                    if (dialog->dirPath == rightPanel->currentDir) {
                        // Reload
                        rightPanel->reload(rightPanel->currentIndex);
                    }
                }

                // Remove Dialog From Delete Progress Dialogs
                deleteProgressDialogs.removeAt(i);
                // Delete Dialog
                delete dialog;
                dialog = NULL;
            }
        }
    }
}

//==============================================================================
// Transfer Progress Window Closed Slot
//==============================================================================
void MainWindow::transferProgressClosed(TransferProgressDialog* aTransferProgressDialog)
{
    // Check Progress Dialog
    if (aTransferProgressDialog) {
        qDebug() << "MainWindow::transferProgressClosed - aTransferProgressDialog: " << aTransferProgressDialog;

        // Get Transfer Progress Dialogs Count
        int tpdCount = transferProgressDialogs.count();

        // Go Thru Dialogs
        for (int i=0; i<tpdCount; ++i) {
            // Get Dialog
            TransferProgressDialog* dialog = transferProgressDialogs[i];

            // Check Dialog
            if (dialog == aTransferProgressDialog) {
                // Check Left Panel
                if (leftPanel /*&& (leftPanel->dwDirChanged || leftPanel->dwFileChanged)*/) {
                    // Check Source Path
                    if (dialog->targetPath == leftPanel->currentDir) {
                        // Set Last File Name
                        leftPanel->lastFileName = QFileInfo(dialog->getLastTarget()).fileName();
                        // Try To Find Lat Item File Name & Reload
                        leftPanel->reload();

                    } else if (dialog->operation == DEFAULT_OPERATION_MOVE_FILE && dialog->sourcePath == leftPanel->currentDir) {
                        // Reload
                        leftPanel->reload(leftPanel->currentIndex);
                    }
                }

                // Check Right Panel
                if (rightPanel /*&& (rightPanel->dwDirChanged || rightPanel->dwFileChanged)*/) {
                    // Check Dialog Target Path
                    if (dialog->targetPath == rightPanel->currentDir) {
                        // Set Last Target File Name
                        rightPanel->lastFileName = QFileInfo(dialog->getLastTarget()).fileName();
                        // Reload
                        rightPanel->reload();

                    } else if (dialog->operation == DEFAULT_OPERATION_MOVE_FILE && dialog->sourcePath == rightPanel->currentDir) {
                        // Reload
                        rightPanel->reload(rightPanel->currentIndex);
                    }
                }

                // Remove Dialog From Transfer Progress Dialogs
                transferProgressDialogs.removeAt(i);
                // Delete Dialog
                delete dialog;
                dialog = NULL;
            }
        }
    }
}

//==============================================================================
// Focused Panel Changed Slot
//==============================================================================
void MainWindow::focusedPanelChanged(FilePanel* aFocusedPanel)
{
    //qDebug() << "MainWindow::focusedPanelChanged";

    // Set Focused Panel
    focusedPanel = aFocusedPanel;

    // Update Menu
    updateMenu();
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
        ui->actionHelp->trigger();
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
        ui->actionTerminal->trigger();
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
        // Trigger View Action
        ui->actionView->trigger();
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
        // Trigger Edit Action
        ui->actionEdit->trigger();
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
        // Trigger Copy Action
        ui->actionCopy->trigger();
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
        // Trigger Move Action
        ui->actionMove->trigger();
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
        // Trigger Make Dir Action
        ui->actionNew_Directory->trigger();
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
        // Trigger Delete Action
        ui->actionDelete_File->trigger();
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
        // Trigger Options Action
        ui->actionPreferences->trigger();
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
    showHelp();
}

//==============================================================================
// Action Options Triggered Slot
//==============================================================================
void MainWindow::on_actionOptions_triggered()
{
    // Show Preferences
    showPreferences();
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
    // Check Focused Panel
    if (focusedPanel) {
        // Select All Files In Focused Panel
        focusedPanel->selectAllFiles();
    }
}

//==============================================================================
// Action Select None Triggered Slot
//==============================================================================
void MainWindow::on_actionSelect_None_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Deselect All Files In Focused Panel
        focusedPanel->deselectAllFiles();
    }
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
    // Check Focused Panel
    if (focusedPanel) {
        // Check Sorting

        // Sort Focused Panel Items By Name
        focusedPanel->setSorting(DEFAULT_SORT_NAME);
    }
}

//==============================================================================
// Action Sort By Extension Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Extension_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
    // Sort Focused Panel Items By Extension
    }
}

//==============================================================================
// Action Sort By Type Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Type_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Type
        focusedPanel->setSorting(DEFAULT_SORT_EXT);
    }
}

//==============================================================================
// Action Sort By Size Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Size_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Size
        focusedPanel->setSorting(DEFAULT_SORT_SIZE);
    }
}

//==============================================================================
// Action Sort By Date Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Date_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Date
        focusedPanel->setSorting(DEFAULT_SORT_DATE);
    }
}

//==============================================================================
// Action Sort By Owner Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Owner_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Owner
        focusedPanel->setSorting(DEFAULT_SORT_OWNER);
    }
}

//==============================================================================
// Action Sort By Permission Triggered Slot
//==============================================================================
void MainWindow::on_actionSort_by_Permissions_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Sort Focused Panel Items By Permission
        focusedPanel->setSorting(DEFAULT_SORT_PERMS);
    }
}

//==============================================================================
// Action Toggle Hidden Files Triggered Slot
//==============================================================================
void MainWindow::on_actionShow_Hide_Hiden_triggered()
{
    // Toggle Show Hidden
    toggleHiddenFile();
}

//==============================================================================
// Action New Directory Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_Directory_triggered()
{
    // Launch Create Dir
    launchCreateDir();
}

//==============================================================================
// Action New File Triggered Slot
//==============================================================================
void MainWindow::on_actionNew_File_triggered()
{
    // Launch Editor
    launchViewer(true, true);
}

//==============================================================================
// Action View File Triggered Slot
//==============================================================================
void MainWindow::on_actionView_triggered()
{
    // Launch Viewer
    launchViewer(false);
}

//==============================================================================
// Action Edit File Triggered Slot
//==============================================================================
void MainWindow::on_actionEdit_triggered()
{
    // Launch Viewer
    launchViewer(true);
}

//==============================================================================
// Action Copy File Triggered Slot
//==============================================================================
void MainWindow::on_actionCopy_triggered()
{
    // Launch File Copy
    launchFileCopy();
}

//==============================================================================
// Action Move File Triggered Slot
//==============================================================================
void MainWindow::on_actionMove_triggered()
{
    // Launch File Move
    launchFileMove();
}

//==============================================================================
// Action Delete File Triggered Slot
//==============================================================================
void MainWindow::on_actionDelete_File_triggered()
{
    // Launch Delete Dir
    launchDelete();
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
// Actrion Reload Triggered Slot
//==============================================================================
void MainWindow::on_actionReload_triggered()
{
    // Check Focused Panel
    if (focusedPanel) {
        // Reload Focused Panel
        focusedPanel->reload(focusedPanel->currentIndex);
    }
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
    // Shut Down
    shutDown();

    // Check View Menu Action Group
    if (viewMenuActionGroup) {
        // Delete View Menu Action Group
        delete viewMenuActionGroup;
        viewMenuActionGroup = NULL;
    }

    // Delete UI
    delete ui;

    // Check About Dialog
    if (aboutDialog) {
        // Delete Dialog
        delete aboutDialog;
        aboutDialog = NULL;
    }

    // Check Preferences Dialog
    if (preferencesDialog) {
        // Delete Dialog
        delete preferencesDialog;
        preferencesDialog = NULL;
    }

    // Check Create Dir Dialog
    if (createDirDialog) {
        // Delete Dialog
        delete createDirDialog;
        createDirDialog = NULL;
    }

    // Check Delete File Dialog
    if (deleteFileDialog) {
        // Delete Dialog
        delete deleteFileDialog;
        deleteFileDialog = NULL;
    }

    // Check Transfer File Dialog
    if (transferFileDialog) {
        // Delete Dialog
        delete transferFileDialog;
        transferFileDialog = NULL;
    }

    // Check Help Window
    if (helpWindow) {
        // Delete Dialog
        delete helpWindow;
        helpWindow = NULL;
    }

    // Check Search File Dialog
    if (searchFileDialog) {
        // Delete Search File Dialog
        delete searchFileDialog;
        searchFileDialog = NULL;
    }

    qDebug() << "MainWindow::~MainWindow";
}



