#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QDebug>

#include <mcwinterface.h>

#include "deleteprogressdialog.h"
#include "deleteprogressmodel.h"
#include "confirmdialog.h"
#include "ui_deleteprogressdialog.h"
#include "remotefileutilclient.h"
#include "constants.h"






//==============================================================================
// Constructor
//==============================================================================
DeleteProgressQueueItemDelegate::DeleteProgressQueueItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
{

}

//==============================================================================
// Paint
//==============================================================================
void DeleteProgressQueueItemDelegate::paint(QPainter* aPainter,
                                            const QStyleOptionViewItem& aOption,
                                            const QModelIndex& aIndex) const
{
    // Check Painter
    if (!aPainter) {
        return;
    }

    // Check State
    if (aOption.state & QStyle::State_Selected) {
        // Fill Selected Background
        aPainter->fillRect(aOption.rect, aOption.palette.highlight());
    }

    // Switch Column
    switch (aIndex.column()) {
        default:
        case 0:
            // Draw Text
            aPainter->drawText(aOption.rect.adjusted(DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN, 0, -DEFAULT_PROGRESS_DIALOG_TEXT_MARGIN, 0), Qt::AlignVCenter, aIndex.model()->data(aIndex).toString());
        break;

        case 1:
            // Draw Text
            aPainter->drawText(aOption.rect, Qt::AlignCenter, aIndex.model()->data(aIndex).toBool() ? "X" : "");
        break;
    }
}

//==============================================================================
// Create Editor
//==============================================================================
QWidget* DeleteProgressQueueItemDelegate::createEditor(QWidget* aParent,
                                                       const QStyleOptionViewItem& aOption,
                                                       const QModelIndex& aIndex) const
{
    Q_UNUSED(aParent);
    Q_UNUSED(aOption);
    Q_UNUSED(aIndex);

    return NULL;
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressQueueItemDelegate::~DeleteProgressQueueItemDelegate()
{

}











//==============================================================================
// Constructor
//==============================================================================
DeleteProgressDialog::DeleteProgressDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DeleteProgressDialog)
    , queueModel(NULL)
    , fileUtil(NULL)
    , closeWhenFinished(false)
    , queueIndex(-1)
    , dirPath("")
{
    qDebug() << "DeleteProgressDialog::DeleteProgressDialog";

    // Setup UI
    ui->setupUi(this);

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void DeleteProgressDialog::init()
{
    qDebug() << "DeleteProgressDialog::init";

    // Create New Queue Model
    queueModel = new DeleteProgressModel();
    // Set Model
    ui->deleteQueue->setModel(queueModel);
    // Set Item Delegate
    ui->deleteQueue->setItemDelegate(new DeleteProgressQueueItemDelegate());

    // Create File Util
    fileUtil = new RemoteFileUtilClient();

    // Check File Util
    if (fileUtil) {
        // Connect Signals
        connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
        connect(fileUtil, SIGNAL(clientStatusChanged(uint,int)), this, SLOT(clientStatusChanged(uint,int)));
        connect(fileUtil, SIGNAL(fileOpStarted(uint,QString,QString,QString,QString)), this, SLOT(fileOpStarted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpProgress(uint,QString,QString,quint64,quint64)), this, SLOT(fileOpProgress(uint,QString,QString,quint64,quint64)));
        connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
        connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
        connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
    }

    // Connect To File Server
    fileUtil->connectToFileServer();

    // Connect Signals
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBoxAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(buttonBoxRejected()));

    // Connect Signals
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    // ...
}

//==============================================================================
// Build Queue
//==============================================================================
bool DeleteProgressDialog::buildQueue(const QString& aDirPath, const QStringList& aSelectedFiles)
{
    // Get Selected Files Count
    int sfCount = aSelectedFiles.count();

    // Check Selected Files Count
    if (sfCount <= 0) {
        qDebug() << "DeleteProgressDialog::buildQueue - aDirPath: " << aDirPath << " - NO SELECTED FILES TO DELETE!";

        return false;
    }

    // Check Queue Model
    if (!queueModel) {
        qDebug() << "DeleteProgressDialog::buildQueue - aDirPath: " << aDirPath << " - NO QUEUE MODEL!";

        return false;
    }

    qDebug() << "DeleteProgressDialog::buildQueue - aDirPath: " << aDirPath << " - sfCount: " << sfCount;

    // Init Local Path
    QString localPath = aDirPath;
    // Check Local Path
    if (!localPath.endsWith("/")) {
        // Adjust Local Path
        localPath += "/";
    }

    // Go Thru Selected Files
    for (int i=0; i<sfCount; ++i) {
        // Add To Queue Model
        queueModel->addItem(localPath + aSelectedFiles[i]);
    }

    return true;
}

//==============================================================================
// Process Queue
//==============================================================================
void DeleteProgressDialog::processQueue()
{
    // Check File Util & Queue Model
    if (fileUtil && queueModel) {
        // Check Queue Index
        if (queueIndex >= 0 && queueIndex < queueModel->rowCount()) {
            qDebug() << "DeleteProgressDialog::processQueue";
            // Get File Name
            QString fileName = queueModel->getFileName(queueIndex);
            // Delete File
            fileUtil->deleteFile(fileName);
            // Configure Buttons
            configureButtons(QDialogButtonBox::Abort);
        } else {

            // Check Close When Finished
            if (closeWhenFinished) {
                // Close
                close();
            } else {
                // Configure Buttons
                configureButtons(QDialogButtonBox::Close);
            }
        }
    }
}

//==============================================================================
// Clear Queue
//==============================================================================
void DeleteProgressDialog::clearQueue()
{
    // Check Queue
    if (queueModel) {
        qDebug() << "DeleteProgressDialog::clearQueue";

        // Clear Model
        queueModel->clear();

        // Reset Queue Index
        queueIndex = -1;
    }
}

//==============================================================================
// Restore UI
//==============================================================================
void DeleteProgressDialog::restoreUI()
{
    qDebug() << "DeleteProgressDialog::restoreUI";

    // Init Settings
    QSettings settings;
    // Get Close When Finished
    closeWhenFinished = settings.value(SETTINGS_KEY_CLOSE_WHEN_FINISHED, false).toBool();
    // Set Checkbox
    ui->closeWhenFinishedCheckBox->setChecked(closeWhenFinished);

    // Set Column Width
    ui->deleteQueue->setColumnWidth(0, ui->deleteQueue->width() - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
    // Set Column Width
    ui->deleteQueue->setColumnWidth(1, DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
}

//==============================================================================
// Save Settings
//==============================================================================
void DeleteProgressDialog::saveSettings()
{
    qDebug() << "DeleteProgressDialog::saveSettings";

    // Init Settings
    QSettings settings;
    // Get Close When Finished
    closeWhenFinished = ui->closeWhenFinishedCheckBox->checkState() == Qt::Checked;
    // Save Setting
    settings.setValue(SETTINGS_KEY_CLOSE_WHEN_FINISHED, closeWhenFinished);
    // Sync
    settings.sync();
}

//==============================================================================
// Configure Buttons
//==============================================================================
void DeleteProgressDialog::configureButtons(const QDialogButtonBox::StandardButtons& aButtons)
{
    // Configure Buttons
    ui->buttonBox->setStandardButtons(aButtons);
}

//==============================================================================
// Launch Progress Dialog
//==============================================================================
void DeleteProgressDialog::launch(const QString& aDirPath, const QStringList& aSelectedFiles)
{
    qDebug() << "DeleteProgressDialog::launch - aDirPath: " << aDirPath << " - count: " << aSelectedFiles.count();

    // Set Dir Path
    dirPath = aDirPath;

    // Restore UI
    restoreUI();

    // ...

    // Show
    show();

    // Build Queue
    if (buildQueue(aDirPath, aSelectedFiles)) {
        // Set Queue Index
        queueIndex = 0;
        // Process Queue
        //QTimer::singleShot(1, this, SLOT(processQueue()));
    } else {
        // Reset Queue Index
        queueIndex = -1;
    }
}

//==============================================================================
// Get Dir Path
//==============================================================================
QString DeleteProgressDialog::getDirPath()
{
    return dirPath;
}

//==============================================================================
// Suspend
//==============================================================================
void DeleteProgressDialog::suspend()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "DeleteProgressDialog::suspend";

        // Suspend
        fileUtil->suspend();
    }
}

//==============================================================================
// Resume
//==============================================================================
void DeleteProgressDialog::resume()
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "DeleteProgressDialog::resume";

        // Resume
        fileUtil->resume();
    }
}

//==============================================================================
// Abort
//==============================================================================
void DeleteProgressDialog::abort()
{
    // Check File Util
    if (fileUtil && (fileUtil->getStatus() == ECSTBusy || fileUtil->getStatus() == ECSTSuspended || fileUtil->getStatus() == ECSTWaiting)) {
        qDebug() << "DeleteProgressDialog::abort";

        // Abort
        fileUtil->abort();
    }
}

//==============================================================================
// Set Current File Name
//==============================================================================
void DeleteProgressDialog::setCurrentFileName(const QString& aCurrentFileName)
{
    // Set Text
    ui->currentFileNameLabel->setText(aCurrentFileName);
}

//==============================================================================
// Set Current File Progress
//==============================================================================
void DeleteProgressDialog::setCurrentProgress(const quint64& aProgress, const quint64& aTotal)
{
    // Set Minimum
    ui->currentProgress->setMinimum(0);

    // Check Total
    if (aTotal > INT_MAX) {
        // Set Maximum
        ui->currentProgress->setMaximum(aTotal >> 32);
        // Set Value
        ui->currentProgress->setValue(aProgress >> 32);
    } else {
        // Set Maximum
        ui->currentProgress->setMaximum(aTotal);
        // Set Value
        ui->currentProgress->setValue(aProgress);
    }
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void DeleteProgressDialog::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "DeleteProgressDialog::clientStatusChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected) {
        // Process Queue
        QTimer::singleShot(1, this, SLOT(processQueue()));
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void DeleteProgressDialog::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    qDebug() << "DeleteProgressDialog::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // ...
}

//==============================================================================
// File Operation Started Slot
//==============================================================================
void DeleteProgressDialog::fileOpStarted(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpStarted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // Set Current File Name
    setCurrentFileName(aPath);
    // Set Current Progress
    setCurrentProgress(queueIndex, queueModel->rowCount());

    // Configure Buttons
    //configureButtons(QDialogButtonBox::Abort);

    // ...
}

//==============================================================================
// File Operation Progress Slot
//==============================================================================
void DeleteProgressDialog::fileOpProgress(const unsigned int& aID,
                                          const QString& aOp,
                                          const QString& aCurrFilePath,
                                          const quint64& aCurrProgress,
                                          const quint64& aCurrTotal)
{
    Q_UNUSED(aCurrProgress);
    Q_UNUSED(aCurrTotal);

    qDebug() << "DeleteProgressDialog::fileOpProgress - aID: " << aID << " - aOp: " << aOp;

    // Set Current File Name
    setCurrentFileName(aCurrFilePath);
    // Set Current Progress
    //setCurrentProgress(aCurrProgress, aCurrTotal);
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void DeleteProgressDialog::fileOpFinished(const unsigned int& aID,
                                          const QString& aOp,
                                          const QString& aPath,
                                          const QString& aSource,
                                          const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

    // Check Operation - Delete File
    if (aOp == DEFAULT_OPERATION_DELETE_FILE) {
        // Check Queue Model
        if (queueModel) {
            // Set Done
            queueModel->setDone(queueIndex);
        }

        // Increase Current Queue Index
        queueIndex++;

        // Set Current Progress
        setCurrentProgress(queueIndex, queueModel->rowCount());

    // Check Operation - Queue
    } else if (aOp == DEFAULT_OPERATION_QUEUE) {

        // Just Move On To Inserted Files

        // ...

    }

    // Process Queue
    processQueue();
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void DeleteProgressDialog::fileOpAborted(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

    // Check Close When Finished
    if (closeWhenFinished) {
        // Close
        close();
    }
}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void DeleteProgressDialog::fileOpError(const unsigned int& aID,
                                       const QString& aOp,
                                       const QString& aPath,
                                       const QString& aSource,
                                       const QString& aTarget,
                                       const int& aError)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

    // ...

    // Init Confirmation Dialog
    ConfirmDialog confirmDialog;

    // Set Dialog Title
    confirmDialog.setConfirmTitle(tr(DEFAULT_ERROR_TITLE_DELETE_FILE));

    // Switch Error
    switch (aError) {
        case DEFAULT_ERROR_NOTEXISTS:
            // Set Error Text
            confirmDialog.setConfirmText(tr(DEFAULT_ERROR_TEXT_FILE_DOESNT_EXIST));
        break;
    }

    // Set Path
    confirmDialog.setPath(aPath);

    // Configure Standard Buttons
    confirmDialog.configureButtons(QDialogButtonBox::Abort);

    // Add Button
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_RETRY), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_RETRY);
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIP), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIP);
    confirmDialog.addCustomButton(tr(DEFAULT_CONFIRM_BUTTON_TEXT_SKIPALL), QDialogButtonBox::AcceptRole, DEFAULT_CONFIRM_SKIPALL);

    // Exec Confirm Dialog
    confirmDialog.exec();

    // Get Action Index
    int actionIndex = confirmDialog.getActionIndex();

    // Check File Util
    if (fileUtil) {
        // Send User Response
        fileUtil->sendUserResponse(actionIndex == -1 ? DEFAULT_CONFIRM_ABORT : actionIndex, confirmDialog.getPath());
    }
}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void DeleteProgressDialog::fileOpNeedConfirm(const unsigned int& aID,
                                             const QString& aOp,
                                             const int& aCode,
                                             const QString& aPath,
                                             const QString& aSource,
                                             const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "DeleteProgressDialog::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aCode: " << aCode;

    // ...
}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void DeleteProgressDialog::fileOpQueueItemFound(const unsigned int& aID,
                                                const QString& aOp,
                                                const QString& aPath,
                                                const QString& aSource,
                                                const QString& aTarget)
{
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    // Check Queue
    if (queueModel) {
        qDebug() << "DeleteProgressDialog::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

        // Insert Item To Queue
        queueModel->insertItem(queueIndex, aPath);

        // Set Current Progress
        setCurrentProgress(queueIndex, queueModel->rowCount());
    }
}

//==============================================================================
// Button Box Accepted Slot
//==============================================================================
void DeleteProgressDialog::buttonBoxAccepted()
{
    qDebug() << "DeleteProgressDialog::buttonBoxAccepted";

    // ...

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// Button Box Rejected Slot
//==============================================================================
void DeleteProgressDialog::buttonBoxRejected()
{
    qDebug() << "DeleteProgressDialog::buttonBoxRejected";

    // ...

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// Tab Changed Slot
//==============================================================================
void DeleteProgressDialog::tabChanged(const int& aIndex)
{
    //qDebug() << "DeleteProgressDialog::tabChanged - aIndex: " << aIndex;

    // Check Tab Index
    if (aIndex == 1) {
        // Set Column Width
        ui->deleteQueue->setColumnWidth(0, ui->deleteQueue->width() - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
        // Set Column Width
        ui->deleteQueue->setColumnWidth(1, DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
    }
}

//==============================================================================
// Close Event
//==============================================================================
void DeleteProgressDialog::closeEvent(QCloseEvent* aEvent)
{
    //qDebug() << "DeleteProgressDialog::closeEvent";

    QDialog::closeEvent(aEvent);

    // Emit Dialog Closed Signal
    emit dialogClosed(this);
}

//==============================================================================
// Resize Event
//==============================================================================
void DeleteProgressDialog::resizeEvent(QResizeEvent* aEvent)
{
    // Check Event
    if (aEvent && aEvent->size().width() > 0 && aEvent->size().height() > 0) {
        //qDebug() << "DeleteProgressDialog::resizeEvent - aEvent: " << aEvent->size();

        // Set Column Width
        ui->deleteQueue->setColumnWidth(0, ui->deleteQueue->width() - DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
        // Set Column Width
        ui->deleteQueue->setColumnWidth(1, DEFAULT_PROGRESS_DIALOG_COLUMN_WIDTH_DONE);
    }
}

//==============================================================================
// Destructor
//==============================================================================
DeleteProgressDialog::~DeleteProgressDialog()
{
    // Save Settings
    saveSettings();

    // Abort
    abort();

    // Clear Queue
    clearQueue();

    // Delete UI
    delete ui;

    // Check Qeuue Model
    if (queueModel) {
        // Delete Queue
        delete queueModel;
        queueModel = NULL;
    }

    // Check File Util
    if (fileUtil) {
        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    qDebug() << "DeleteProgressDialog::~DeleteProgressDialog";
}
