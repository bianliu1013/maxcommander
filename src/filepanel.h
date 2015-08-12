#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QTimerEvent>

namespace Ui {
class FilePanel;
}

class FileListModel;
class MainWindow;
class FileListImageProvider;
class ConfirmDialog;
class RemoteFileUtilClient;
class TransferProgressModelItem;
class FileRenamer;
class DirScanner;
class SettingsController;
class DirHistoryListModel;
class DirHistoryListPopup;


//==============================================================================
// File Panel Class
//==============================================================================
class FilePanel : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int pressedIndex READ getPressedIndex WRITE setPressedIndex NOTIFY pressedIndexChanged)
    Q_PROPERTY(QString panelName READ getPanelName)
    Q_PROPERTY(bool panelHasFocus READ getPanelFocus WRITE setPanelFocus NOTIFY panelFocusChanged)
    Q_PROPERTY(int visualItemsCount READ getvisualItemsCount WRITE setVisualItemsCount NOTIFY visualItemsCountChanged)

    Q_PROPERTY(bool extVisible READ getExtVisible WRITE setExtVisible NOTIFY extVisibleChanged)
    Q_PROPERTY(bool typeVisible READ getTypeVisible WRITE setTypeVisible NOTIFY typeVisibleChanged)
    Q_PROPERTY(bool sizeVisible READ getSizeVisible WRITE setSizeVisible NOTIFY sizeVisibleChanged)
    Q_PROPERTY(bool dateVisible READ getDateVisible WRITE setDateVisible NOTIFY dateVisibleChanged)
    Q_PROPERTY(bool ownerVisible READ getOwnerVisible WRITE setOwnerVisible NOTIFY ownerVisibleChanged)
    Q_PROPERTY(bool permsVisible READ getPermsVisible WRITE setPermsVisible NOTIFY permsVisibleChanged)
    Q_PROPERTY(bool attrsVisible READ getAttrsVisible WRITE setAttrsVisible NOTIFY attrsVisibleChanged)

    Q_PROPERTY(int extWidth READ getExtWidth WRITE setExtWidth NOTIFY extWidthChanged)
    Q_PROPERTY(int typeWidth READ getTypeWidth WRITE setTypeWidth NOTIFY typeWidthChanged)
    Q_PROPERTY(int sizeWidth READ getSizeWidth WRITE setSizeWidth NOTIFY sizeWidthChanged)
    Q_PROPERTY(int dateWidth READ getDateWidth WRITE setDateWidth NOTIFY dateWidthChanged)
    Q_PROPERTY(int ownerWidth READ getOwnerWidth WRITE setOwnerWidth NOTIFY ownerWidthChanged)
    Q_PROPERTY(int permsWidth READ getPermsWidth WRITE setPermsWidth NOTIFY permsWidthChanged)
    Q_PROPERTY(int attrsWidth READ getAttrsWidth WRITE setAttrsWidth NOTIFY attrsWidthChanged)

    Q_PROPERTY(int sorting READ getSorting WRITE setSorting NOTIFY sortingChanged)
    Q_PROPERTY(bool reverseOrder READ getReverseOrder WRITE setReverseOrder NOTIFY reverseOrderChanged)

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(bool loading READ getloading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(bool fileRenameActive READ getFileRenameActive WRITE setFileRenameActive NOTIFY fileRenameActiveChanged)
    Q_PROPERTY(int modifierKeys READ getModifierKeys NOTIFY modifierKeysChanged)

    Q_PROPERTY(bool searchResultsMode READ getSearchResultsMode WRITE setSearchResultsMode NOTIFY searchResultsModeChanged)

public:
    // Constructor
    explicit FilePanel(QWidget* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();

    // Get Files/Items Count
    int getCount();

    // Get Panel Name
    QString getPanelName();
    // Set Panel Name
    void setPanelName(const QString& aPanelName);

    // Get Panel Focus
    bool getPanelFocus();

    // Get Current Index
    int getCurrentIndex();
    // Set Current Index
    void setCurrentIndex(const int& aCurrentIndex);

    // Get Pressed Index
    int getPressedIndex();
    // Set Pressed Index
    void setPressedIndex(const int& aPressedIndex);

    // Get Visual Items Count
    int getvisualItemsCount();

    // Get Extension Visible
    bool getExtVisible();
    // Set Extension Visible
    void setExtVisible(const bool& aExtVisible);

    // Get Type Visible
    bool getTypeVisible();
    // Set Type Visible
    void setTypeVisible(const bool& aTypeVisible);

    // Get Size Visible
    bool getSizeVisible();
    // Set Size Visible
    void setSizeVisible(const bool& aSizeVisible);

    // Get Date Visible
    bool getDateVisible();
    // Set Date Visible
    void setDateVisible(const bool& aDateVisible);

    // Get Owner Visible
    bool getOwnerVisible();
    // Set Owner Visible
    void setOwnerVisible(const bool& aOwnerVisible);

    // Get Permissions Visible
    bool getPermsVisible();
    // Set Permissions Visible
    void setPermsVisible(const bool& aPermsVisible);

    // Get Attributes Visible
    bool getAttrsVisible();
    // Set Attributes Visible
    void setAttrsVisible(const bool& aAttrsVisible);

    // Get Extensions Column Width
    int getExtWidth();
    // Set Extensions Column Width
    void setExtWidth(const int& aWidth);

    // Get Type Column Width
    int getTypeWidth();
    // Set Type Column Width
    void setTypeWidth(const int& aWidth);

    // Get Size Column Width
    int getSizeWidth();
    // Set Size Column Width
    void setSizeWidth(const int& aWidth);

    // Get Date Column Width
    int getDateWidth();
    // Set Date Column Width
    void setDateWidth(const int& aWidth);

    // Get Owner Column Width
    int getOwnerWidth();
    // Set Owner Column Width
    void setOwnerWidth(const int& aWidth);

    // Get Permissions Column Width
    int getPermsWidth();
    // Set Permissions Column Width
    void setPermsWidth(const int& aWidth);

    // Get Attributes Column Width
    int getAttrsWidth();
    // Set Attributes Column Width
    void setAttrsWidth(const int& aWidth);

    // Get Sorting
    int getSorting();
    // Set Sorting
    void setSorting(const int& aSorting);

    // Get Reversed Order
    bool getReverseOrder();
    // Set Reversed Order
    void setReverseOrder(const bool& aReverse);

    // Get Current File Info
    QFileInfo getCurrFileInfo();

    // Busy
    bool busy();

    // Get Loading
    bool getloading();
    // Set Loading
    void setLoading(const bool& aLoading);

    // Get File Index By File Name
    int getFileIndex(const QString& aFileName);

    // Create Dir
    void createDir(const QString& aDirPath);
    // Create Link
    void createLink(const QString& aLinkName, const QString& aLinkTarget);

    // File Rename Active
    bool getFileRenameActive();
    // Set File Rename Active
    void setFileRenameActive(const bool& aFileRenameActive);

    // Get Modifier Keys
    int getModifierKeys();

    // Feed Search Result List
    void feedSearchResults(const QStringList& aSearchResults, const QString& aSearchTerm);

    // Get Search REsults Mode
    bool getSearchResultsMode();
    // Set Search REsults Mode
    void setSearchResultsMode(const bool& aSearchResultMode);

    // Destructor
    virtual ~FilePanel();

public slots:

    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir, const QString& aLastFileName = "");

    // Go To Home Directory
    void gotoHome();
    // Go To Root
    void gotoRoot();
    // Go To Volumes
    void gotoVolumes();
    // Go To Drive
    void gotoDrive(const int& aDriveIndex);
    // Go Up To Parent Dir
    void goUp();
    // Go To Next Item Of The List
    void goNext();
    // Go To Prev Item Of The List
    void goPrev();
    // Go To Page Up Of The List
    void pageUp();
    // Go To Page Down Of The List
    void pageDown();
    // Go To Beginning Of The List
    void goFirst();
    // Go To End Of The List
    void goLast();

    // Set Visual Items Count
    void setVisualItemsCount(const int& aVisualCount);

    // Get Supported Image Formats
    QStringList getSupportedImageFormats();

    // Reload
    void reload(const int& aIndex = -1);

    // Select All Files
    void selectAllFiles();
    // Deselect All Files
    void deselectAllFiles();

    // Toggle Current File Selection
    void toggleCurrentFileSelection();

    // Select Files
    void selectFiles(const QString& aPattern);
    // Deselect Files
    void deselectFiles(const QString& aPattern);

    // Get Selected Files
    QStringList getSelectedFiles();

    // Rename File
    void renameFile(const QString& aSource, const QString& aTarget);

    // Scan Dir
    void scanDir(const QString& aDirPath);

    // Launch Dir History Popup
    void launchDirHistoryPopup();

    // Drag Dropped
    void dragDropped(const QString& aDroppedItems, const int& aModifiers);

    // Drag Started Slot
    void fileListWidgetDragStarted(const int& aPosX, const int aPosY);

signals:

    // Focused Panel Changed Signal
    void focusedPanelChanged(FilePanel* aFocusedPanel);

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);
    // Panel Focus Changed Signal
    void panelFocusChanged(const bool& aPanelFocus);

    // Modifier Keys Pressed Signal
    void modifierKeysChanged(const int& aModifierKeys);

    // Exit Key Released Signal
    void exitKeyReleased();

    // Current Index Changed Signal
    void currentIndexChanged(const int& aIndex);
    // Pressed Index Changed Signal
    void pressedIndexChanged(const int& aIndex);
    // Visual Items Count Changed
    void visualItemsCountChanged(const int& aVisualCount);

    // Extension Visible Changed Signal
    void extVisibleChanged(const bool& aExtVisible);
    // Type Visible Changed Signal
    void typeVisibleChanged(const bool& aTypeVisible);
    // Size Visible Changed Signal
    void sizeVisibleChanged(const bool& aSizeVisible);
    // Date Visible Changed Signal
    void dateVisibleChanged(const bool& aDateVisible);
    // Owner Visible Changed Signal
    void ownerVisibleChanged(const bool& aOwnerVisible);
    // Perms Visible Changed Signal
    void permsVisibleChanged(const bool& aPermsVisible);
    // Attributes Visible Changed Signal
    void attrsVisibleChanged(const bool& aAttrsVisible);

    // Extension Width Changed Signal
    void extWidthChanged(const int& aExtWidth);
    // Type Width Changed Signal
    void typeWidthChanged(const int& aTypeWidth);
    // Size Width Changed Signal
    void sizeWidthChanged(const int& aSizeWidth);
    // Date Width Changed Signal
    void dateWidthChanged(const int& aDateWidth);
    // Owner Width Changed Signal
    void ownerWidthChanged(const int& aOwnerWidth);
    // Permissions Width Changed Signal
    void permsWidthChanged(const int& aPermsWidth);
    // Attributes Width Changed Signal
    void attrsWidthChanged(const int& aAttrsWidth);

    // Sorting Changed Signal
    void sortingChanged(const int& aSorting);
    // Reverse Order Changed Signal
    void reverseOrderChanged(const bool& aReverseOrder);


    // Busy Changed Signal
    void busyChanged(const bool& aBusy);

    // Loading Changed Signal
    void loadingChanged(const bool& aLoading);

    // Show Help Signal
    void showHelp();

    // Launch Terminal Signal
    void launchTerminal(const QString& aDirPath);
    // Launch Viewer Signal
    void launchViewer(const bool& aEditMode = false);

    // Launch File Copy Signal
    void launchFileCopy();
    // Launch File Move/Rename Signal
    void launchFileMove();
    // Launch File Rename
    void launchFileRename();

    // Launch Create Dir Signal
    void launchCreateDir();
    // Launch Create Link Signal
    void launchCreateLink();

    // Launch Search
    void launchSearch();

    // Launch Delete Signal
    void launchDelete();

    // Show Preferences Signal
    void showPreferences();

    // Launch File Select Signal
    void launchFileSelect();
    // Launch File Deselect Signal
    void launchFileDeselect();

    // Error Occured Signal
    void error(const QString& aPath, const int& aError);

    // File Rename Active Changed Signal
    void fileRenameActiveChanged(const bool& aActive);

    // Search Results Mode Changed Signal
    void searchResultsModeChanged(const bool& aSearchResultsMode);

    // Set List View Interactive
    void setListViewInteractive(const bool& aInteractive);

protected slots:

    // Init
    void init();
    // Clear
    void clear();
    // Restore UI
    void restoreUI(const bool& aReload = true);
    // Save Settings
    void saveSettings();
    // Update Available Space Label
    void updateAvailableSpaceLabel();
    // Update Supported Image Formates
    void updateSupportedImageFormats();

    // Start Dir Watcher Slot
    void startDirWatcher();
    // Stop Dir Watcher Slot
    void stopDirWatcher();

    // Handle Item Selection - Dir/Execution
    void handleItemSelection();
    // Handle Item Select
    void handleItemSelect();

    // Reset Modifier Keys
    void resetModifierKeys();

protected slots: // From FileListWidget

    // Set Panel Focus
    void setPanelFocus(const bool& aFocus);

protected slots: // From File Model

    // File List Model Busy Changed Slot
    void fileModelBusyChanged(const bool& aBusy);
    // File List Model Fetch Ready Slot
    void fileModelDirFetchFinished();
    // File List Model Dir Created Slot
    void fileModelDirCreated(const QString& aDirPath);
    // File List Model Link Created Slot
    void fileModelLinkCreated(const QString& aLinkPath, const QString& aLinkTarget);
    // File List Model File Renames Slot
    void fileModelFileRenamed(const QString& aSource, const QString& aTarget);
    // File Model Error Slot
    void fileModelError(const QString& aPath, const QString& aSource, const QString& aTarget, const int& aError);
    // File Model Need Confirm Found Slot
    void fileModelNeedConfirm(const int& aCode, const QString& aPath, const QString& aSource, const QString& aTarget);

protected slots: // From QFileSystemWatcher

    // Directory Changed Slot
    void directoryChanged(const QString& aDirPath);
    // File Changed Slot
    void fileChanged(const QString& aFilePath);

protected slots: // From File Renamer

    // Rename Finished Slot
    void renamerFinished(const QString& aSource, const QString& aTarget);

protected slots: // From Dir Scanner

    // Scan Size Changed Slot
    void scanSizeChanged(const QString& aDirPath, const quint64& aSize);

protected slots: // For Settings

    // Global Settings Has Changed
    void globalSettingsHasChanged();

    // Begin Global Settings Update Slot
    void globalSettingsUpdateBegin();
    // Finish Global Settings Update Slot
    void globalSettingsUpdateFinished();

    // Show Hidden Changed Slot
    void showHiddenFilesChanged(const bool& aHidden);
    // Show Directories First Changed Slot
    void showDirsFirstChanged(const bool& aDirFirst);
    // Case Sensitive Sorting Changed Slot
    void caseSensitiveSortChanged(const bool& aCaseSensitive);
    // Use Default Icons Changed Slot
    void useDefaultIconsChanged(const bool& aUseDefaultIcons);

protected slots:

    // Handle Modifier Key Press Event
    bool handleModifierKeyPressEvent(QKeyEvent* aEvent);
    // Handle Modifier Key Release Event
    bool handleModifierKeyReleaseEvent(QKeyEvent* aEvent);

protected slots: // Buttons

    // Home Button Clicked Slot
    void on_homeButton_clicked();
    // Root Button Clicked Slot
    void on_rootButton_clicked();
    // On Drives Button Clicked Slot
    void on_drivesButton_clicked();

protected slots: // Current Dir Label & Dir History List

    // Current Dir Label Right Clicked Slot
    void currDirLabelRightClicked(const QPoint& aPos);

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

private:
    friend class MainWindow;

    // UI
    Ui::FilePanel*          ui;

    // Settings
    SettingsController*     settings;

    // Global Settings Update Is Ongoing
    bool                    globalSettingsUpdateIsOn;
    // Need Reffresh
    bool                    needRefresh;

    // Current Dir
    QString                 currentDir;

    // Panel Name
    QString                 panelName;

    // Panel Focus
    bool                    panelHasFocus;

    // File List Model
    FileListModel*          fileListModel;

    // Modifier Keys
    int                     modifierKeys;

    // Current Index
    int                     currentIndex;
    // Pressed Index
    int                     pressedIndex;

    // Visual Items Count
    int                     visualItemsCount;

    // Last Directory Name To Jump After CD UP
    QString                 lastDirName;
    // Last File Name To Jump After Reload
    QString                 lastFileName;
    // Last Index Before Reload
    int                     lastIndex;

    // Supported Image Formats
    QStringList             supportedImageFormats;

    // Extensions Column Visible
    bool                    extVisible;
    // Type Column Visible
    bool                    typeVisible;
    // Size Column Visible
    bool                    sizeVisible;
    // Date Column Visible
    bool                    dateVisible;
    // Owner Column Visible
    bool                    ownerVisible;
    // Permissions Column Visible
    bool                    permsVisible;
    // Attributes Column Visible
    bool                    attrsVisible;

    // Extensions Column Width
    int                     extWidth;
    // Type Column Width
    int                     typeWidth;
    // Size Column Width
    int                     sizeWidth;
    // Date Column Width
    int                     dateWidth;
    // Owner Column Width
    int                     ownerWidth;
    // Permissions Column Width
    int                     permsWidth;
    // Attributes Column Width
    int                     attrsWidth;

    // Sorting Type
    int                     sorting;
    // Reverse Order
    bool                    reverseOrder;

    // Dir File System Watcher
    QFileSystemWatcher      dirWatcher;
    // Dir File System Watcher Timer ID
    int                     dirWatcherTimerID;
    // Dir Changed
    bool                    dwDirChanged;
    // File Changed
    bool                    dwFileChanged;

    // Own Key Press
    bool                    ownKeyPress;

    // File Rename Active
    bool                    fileRenameActive;
    // File Ranemer
    FileRenamer*            fileRenamer;
    // File Renamer Update Is Done
    bool                    fileRenamerUpdate;
    // File Transfer Update
    bool                    fileTransferUpdate;
    // File Delete Update
    bool                    fileDeleteUpdate;

    // Dir Scanner
    DirScanner*             dirScanner;

    // Loading State Indicator
    bool                    loading;

    // Search Result Mode
    bool                    searchResultsMode;
    // Search Term
    QString                 searchTerm;

    // Dir History Model
    DirHistoryListModel*    dirHistoryModel;

    // Dir History List Popup
    DirHistoryListPopup*    dirHistoryListPopup;
};

















//==============================================================================
// File Renamer Controller Class
//==============================================================================
class FileRenamer : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit FileRenamer(QWidget* aParent = NULL);

    // Rename File
    void renameFile(const QString& aSource, const QString& aTarget);

    // Destructor
    virtual ~FileRenamer();

signals:

    // Renamer Finished Signal
    void finished(const QString& aSource, const QString& aTarget);
    // Renamer Aborted Signal
    void aborted(const QString& aSource, const QString& aTarget);

protected slots:

    // Add Item
    void addItem(const QString& aSource, const QString& aTarget);
    // Insert Item
    void insertItem(const int& aIndex, const QString& aSource, const QString& aTarget);
    // Set Item State
    void setItemState(const int& aIndex, const int& aState);

    // Process Queue
    void processQueue();
    // Abort
    void abort();
    // Clear Queue
    void clearQueue();

protected slots: // For RemoteFileUtilClient

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Started Slot
    void fileOpStarted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Skipped Slot
    void fileOpSkipped(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Slot
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

    // Need Confirmation Slot
    void fileOpNeedConfirm(const unsigned int& aID,
                           const QString& aOp,
                           const int& aCode,
                           const QString& aPath,
                           const QString& aSource,
                           const QString& aTarget);

    // File Operation Queue Item Found Slot
    void fileOpQueueItemFound(const unsigned int& aID,
                              const QString& aOp,
                              const QString& aPath,
                              const QString& aSource,
                              const QString& aTarget);

protected:

    // File Util
    RemoteFileUtilClient*               fileUtil;
    // Rename Items Queue
    QList<TransferProgressModelItem*>   renameQueue;
    // Queue Index
    int                                 queueIndex;

    // ...

};














//==============================================================================
// Dir Scan Progess State Enum
//==============================================================================
enum DirScanProgressState
{
    EDSSIdle         = 0,
    EDSSRunning,
    EDSSPaused,
    EDSSFinished,
    EDSSSkipped,
    EDSSError
};




//==============================================================================
// Dir Scan Queue Item Class
//==============================================================================
class DirScannerQueueItem
{
public:

    // Constructor
    explicit DirScannerQueueItem(const QString& aDirPath);
    // Destructor
    virtual ~DirScannerQueueItem();

protected:
    friend class DirScanner;

    // Dir Path
    QString                 dirPath;
    // Size
    quint64                 dirSize;
    // Progress State
    DirScanProgressState    state;
};












//==============================================================================
// Dir Scanner Controller Class
//==============================================================================
class DirScanner : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit DirScanner(QObject* aParent = NULL);

    // Scan Dir
    void scanDir(const QString& aDirPath);

    // Abort
    void abort();

    // Clear Queue
    void clearQueue();

    // Destructor
    virtual ~DirScanner();

signals:

    // Scan Finished Signal
    void scanFinished(const QString& aDirPath);

    // Scan Size Changed
    void scanSizeChanged(const QString& aDirPath, const quint64& aSize);

protected slots:

    // Add Item
    void addItem(const QString& aDirPath);
    // Set Item State
    void setItemState(const int& aIndex, const int& aState);

    // Find Index
    int findIndex(const QString& aDirPath);

    // Process Queue
    void processQueue();

protected slots: // For RemoteFileUtilClient

    // Client Connection Changed Slot
    void clientConnectionChanged(const unsigned int& aID, const bool& aConnected);

    // Client Status Changed Slot
    void clientStatusChanged(const unsigned int& aID, const int& aStatus);

    // File Operation Started Slot
    void fileOpStarted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // Dir Size Scan Progress Signal
    void fileOpDirSizeScanProgress(const unsigned int& aID,
                                   const QString& aPath,
                                   const quint64& aNumDirs,
                                   const quint64& aNumFiles,
                                   const quint64& aScannedSize);

    // File Operation Skipped Slot
    void fileOpSkipped(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Finished Slot
    void fileOpFinished(const unsigned int& aID,
                        const QString& aOp,
                        const QString& aPath,
                        const QString& aSource,
                        const QString& aTarget);

    // File Operation Aborted Slot
    void fileOpAborted(const unsigned int& aID,
                       const QString& aOp,
                       const QString& aPath,
                       const QString& aSource,
                       const QString& aTarget);

    // File Operation Error Slot
    void fileOpError(const unsigned int& aID,
                     const QString& aOp,
                     const QString& aPath,
                     const QString& aSource,
                     const QString& aTarget,
                     const int& aError);

protected: // For RemoteFileUtilClient

    // File Util
    RemoteFileUtilClient*               fileUtil;

    // Rename Items Queue
    QList<DirScannerQueueItem*>         scanQueue;

    // Queue Index
    int                                 queueIndex;
};


#endif // FILEPANEL_H
