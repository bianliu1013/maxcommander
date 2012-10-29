#ifndef CUSTOMFILELIST_H
#define CUSTOMFILELIST_H

// INCLUDES

#include <QFrame>
#include <QImage>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QDir>
#include <QLabel>
#include <QGridLayout>

#include "listbox.h"
#include "fileutils.h"


// FORWARD DECLARATIONS

namespace Ui {
class CustomFilelist;
}

class CustomHeader;
class Settings;
class IconScanner;
class ListPopup;
class FileUtilsClient;


// DECLARATIONS

//==============================================================================
//! @class FileItemData File List Box Model Item Data Class
//==============================================================================
class FileItemData : public ListModelItemData
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    FileItemData(QObject* aParent = NULL);

    //! @brief Constructor
    //! @param aInfo File Info
    //! @param aParent Parent
    FileItemData(const QFileInfo& aInfo, QObject* aParent = NULL);

    //! @brief Constructor
    //! @param aFilePath File Path
    //! @param aParent Parent
    FileItemData(const QString& aFilePath, QObject* aParent = NULL);

    //! @brief Set File Info
    //! @param aInfo File Info
    //! @param aUpdate Update Update
    void setFileInfo(const QFileInfo& aInfo, const bool& aUpdate = true);

    //! @brief Get File Info
    //! @param none
    //! @return File Info
    QFileInfo getFileInfo();

    //! @brief Start Calclating Directory Size
    //! @param none
    void startCalculatingDirSize();

    //! @brief Stop Calclating Directory Size
    //! @param none
    void stopCalculatingDirSize();

    //! @brief Get Dir Size
    //! @param none
    //! @return Dir Size If Item Is Dir
    qint64 getDirSize();

    //! @brief Set Selected Flag
    //! @param aSelected Selected Flag
    virtual void setSelected(const bool& aSelected);

    //! @brief Destructor
    //! @param none
    virtual ~FileItemData();

signals:

    //! @brief Size Scanning Finished Signal
    //! @param aIndex Item Data Index
    void sizeScanFinished(const int& aIndex);

protected slots:

    //! @brief Dir Size Updated Slot
    //! @param aSize Dir Size
    void dirSizeUpdated(const qint64& aSize);

    //! @brief Directory Size Scan Stopped Slot
    //! @param none
    void dirSizeScanStopped();

    //! @brief Directory Size Scan Finished Slot
    //! @param none
    void dirSizeScanfinished();

protected:

    //! @brief Delete Dir Size Scanner
    //! @param none
    void deleteDirSizeScanner();

protected: // Data
    friend class ListModel;
    friend class FileListDelegate;
    friend class CustomFilelist;

    //! File Info
    QFileInfo       info;
    //! Dir Size Scanner
    DirSizeScanner* dirSizeScanner;
    //! Calculated Dir Size
    qint64          dirSize;
};





//==============================================================================
//! @class FileListIcon File List Box Item Icon Class
//==============================================================================
class FileListIcon : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    FileListIcon(QWidget* aParent = NULL);

    //! @brief Set Icon Image
    //! @param aImage New Icon Image
    //! @param aUpdate Update
    void setIcon(const QImage& aImage, const bool& aUpdate = true);

    //! @brief Destructor
    //! @param none
    virtual ~FileListIcon();

protected: // From QFrame

    //! @brief Paint Event
    //! @param aEvent Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected: // Data

    //! UI
    QImage  icon;
};






//==============================================================================
//! @class FileListDelegate File List Box Item Delegate Class
//==============================================================================
class FileListDelegate : public ItemDelegate
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aData Item Data
    //! @param aParent Parent Widget
    FileListDelegate(ListModelItemData* aData, QWidget* aParent = NULL);

    //! @brief Create Delegate Duplicate
    //! @param aData Item Data
    //! @return New Item Delegate
    virtual ItemDelegate* duplicate(ListModelItemData* aData = NULL);

    //! @brief Set Data
    //! @param aData Item Data
    //! @param aUpdate Update
    virtual void setData(ListModelItemData* aData, const bool& aUpdate = false);

    //! @brief Get File Info
    //! @param none
    //! @return File Info
    QFileInfo getFileInfo();

    //! @brief Get Item Size
    //! @param none
    //! @return Item Size
    virtual QSize getItemSize();

    //! @brief Get Needs Icon Update State
    //! @param none
    //! @return Needs Icon Update State
    bool getNeedsIconUpdate();

    //! @brief Set Icon Size
    //! @param aIconSize Icon Size
    void setIconSize(const int& aIconSize);

    //! @brief Get Icon Size
    //! @param none
    //! @return Icon Size
    int getIconSize();

    //! @brief Set Icon
    //! @param aImage Icon Image
    //! @param aUpdate Update
    void setIcon(const QImage& aImage, const bool& aUpdate = true);

    //! @brief Destructor
    //! @param none
    virtual ~FileListDelegate();

protected:

    //! @brief Setup UI
    //! @param none
    void setupUI();

    //! @brief Create Delegate Layout
    //! @param none
    void createDelegateLayout();

    //! @brief Create Name Layout
    //! @param none
    void createNameLayout();

    //! @brief Create Icon Label
    //! @param none
    void createIconLabel();

    //! @brief Create Name Label
    //! @param none
    void createNameLabel();

    //! @brief Create Extension Label
    //! @param none
    void createExtLabel();

    //! @brief Create Size Label
    //! @param none
    void createSizeLabel();

    //! @brief Create Date Label
    //! @param none
    void createDateLabel();

    //! @brief Create Owner Label
    //! @param none
    void createOwnrLabel();

    //! @brief Create Permissions Label
    //! @param none
    void createPermsLabel();

    //! @brief Clear UI
    //! @param none
    void clearUI();

    //! @brief Reset Icon
    //! @param aUpdate Update
    void resetIcon(const bool& aUpdate = true);

    //! @brief Update UI
    //! @param none
    virtual void updateUI();

    //! @brief Validate Icon
    //! @param aFileData File Item Data
    virtual void validateIcon(FileItemData* aFileData);

    //! @brief Update Icon Size
    //! @param none
    void updateIconSize();

    //! @brief Set Delegate Header
    //! @param aHeader New Delegate Header
    void setDelegateHeader(CustomHeader* aHeader);

    //! @brief Update Delegate Items Size And Pos
    //! @param none
    void updateDelegateItemsSizeAndPos();

protected slots:

    //! @brief Header Item Size Changed Slot
    //! @param aIndex Header Item Index
    //! @param aWidth Header Item Width
    void headerSizeChanged(const int& aIndex, const int& aWidth);

protected: // From QFrame

    //! @brief Paint Event
    //! @param aEvent Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected: // Data
    friend class ListBox;
    friend class FileListBox;

    //! Icon
    QImage          icon;
    //! Previous File Name
    QString         prevFileName;
    //! Previous Icon Size
    int             prevIconSize;
    //! Icon Size
    int             iconSize;
    //! Needs Icon Update
    bool            needsIconUpdate;
    //! Settings
    Settings*       settings;
    //! Delegate Header
    CustomHeader*   header;
    //! Layout
    QGridLayout*    gridLayout;
    //! Layout
    QGridLayout*    nameLayout;
    //! Icon Label
    FileListIcon*   iconLabel;
    //! Name Label
    QLabel*         nameLabel;
    //! Extension Frame
    QLabel*         extLabel;
    //! Size Frame
    QLabel*         sizeLabel;
    //! Date Label
    QLabel*         dateLabel;
    //! Owner Label
    QLabel*         ownrLabel;
    //! Permission Label
    QLabel*         permLabel;
};










//==============================================================================
//! @class FileListBox File List Box Class
//==============================================================================
class FileListBox : public ListBox
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    FileListBox(QWidget* aParent = NULL);

    //! @brief Set Items Header
    //! @param aHeader Items Header
    void setHeader(CustomHeader* aHeader);

    //! @brief Get Items Header
    //! @param none
    //! @return Items Header
    CustomHeader* getHeader();

    //! @brief Set Panel Name
    //! @param aPanelName Panel Name
    void setPanelName(const QString& aPanelName);

    //! @brief Find/Get Item Index By Name
    //! @param aFileName File Name
    //! @return Item Index if Found, -1 Otherwise
    int findItemByName(const QString& aFileName);

    //! @brief Get Selected Files Size
    //! @param none
    //! @return Selected Files Size
    qint64 getSelectedSize();

    //! @brief Get Item Data By Index
    //! @param aIndex Item Index
    //! @return Item Data Or NULL
    FileItemData* getItemData(const int& aIndex);

    //! @brief Set Item Icon Size
    //! @param aIconSize Icon Size
    //! @param aRefresh Refresh
    void setItemIconSize(const int& aIconSize, const bool& aRefresh = true);

    //! @brief Get Item Icon Size
    //! @param none
    //! @return Icon Size
    int getItemIconSize();

    //! @brief Update Delegate Icon Size By Settings
    //! @param aRefresh Refresh
    void updateDelegateIconSize(const bool& aRefresh = true);

    //! @brief Update Icons
    //! @param none
    void updateIcons();

    //! @brief Select All Items
    //! @param none
    virtual void selectAll();

    //! @brief Get File List Box Item
    //! @param aIndex Item Index
    //! @brief File List Box Item
    const FileListDelegate* getItem(const int& aIndex) const;

    //! @brief Destructor
    //! @param none
    virtual ~FileListBox();

signals:

    //! @brief List Box Focus Changed Signal
    //! @param aName List Box Panel Name
    //! @param aFocused List Box Focused State
    void listBoxFocusChanged(const QString& aPanelName, const bool& aFocused);

    //! @brief Item Icon Size Changed Signal
    //! @param aRefresh Refresh
    void itemIconSizeChanged(const bool& aRefresh);

    //! @brief List Box Right Clicked Signal
    //! @param aPos Right Click Global Position
    void listBoxRightClicked(const QPoint& aPos);

protected:

    //! @brief Start Icon Update Timer
    //! @param none
    void startIconUpdateTimer();

    //! @brief Stop Icon Update Timer
    //! @param none
    void stopIconUpdateTimer();

protected slots:

    //! @brief Get Item Icons
    //! @param none
    void getItemIcons();

protected: // From ListBox

    //! @brief Connect Delegate Signals
    //! @param aItem Item Delegate
    virtual void connectDelegateSignals(ItemDelegate* aItem);

    //! @brief Disconnect Delegate Signals
    //! @param aItem Item Delegate
    virtual void disconnectDelegateSignals(ItemDelegate* aItem);

    //! @brief Updating Cached Items Data
    //! @param aUpdateUI Update UI
    virtual void updateCachedItemsData(const bool& aUpdateUI = true);

    //! @brief Clear Delegate Cache
    //! @param none
    virtual void clearCache();

    //! @brief Handle Key Event
    //! @param aKey Key Code
    //! @param aModifiers Key Modifiers
    virtual void handleKeyEvent(const int& aKey, const Qt::KeyboardModifiers& aModifiers);

protected: // From QFrame

    //! @brief Focus In Event
    //! @param aEvent Focus Event
    virtual void focusInEvent(QFocusEvent* aEvent);

    //! @brief Focus Out Event
    //! @param aEvent Focus Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    //! @brief Timer Event
    //! @param aEvent Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

    //! @brief Mouse Press Event
    //! @param aEvent Mouse Event
    virtual void mousePressEvent(QMouseEvent* aEvent);

    //! @brief Mouse Release Event
    //! @param aEvent Mouse Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

protected: // Data
    friend class FileListDelegate;

    //! File List Items Header
    CustomHeader*   header;
    //! Panel Name
    QString         panelName;
    //! Got Focus
    bool            gotFocus;
    //! Icon Size
    int             iconSize;
    //! Icon Scanner
    IconScanner*    iconScanner;
    //! Previous First Cached Item
    int             prevFCI;
    //! Previous Cache Count
    int             prevCC;
    //! Icon Update Timer ID
    int             iconUpdateTimerID;
};















//==============================================================================
//! @class IconScanner Icon Scanner Thread
//==============================================================================
class IconScanner : public FileUtilThreadBase
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent
    IconScanner(QObject* aParent = NULL);

    //! @brief Scan Icons
    //! @param aCache File List Box Delegate Cache
    //! @param aFVI File List Box First Visible Index
    //! @param aIconSize File List Box Icon Size
    void scanIcons(const CacheType* aCache, const int& aFVI, const int& aIconSize);

    //! @brief Destructor
    //! @param none
    virtual ~IconScanner();

protected:

    //! @brief Update Item Icon
    //! @param aIndex Item Index
    void updateItemIcon(const int& aIndex);

protected: // From FileUtilThreadBase

    //! @brief Do Operation
    //! @param none
    virtual void doOperation();

protected: // Data

    //! Icon Size
    int         iconSize;
    //! File List Box Delegate Cache
    CacheType*  delegateCache;
    //! File List Box First Visible Index
    int         firstVisibleIndex;
};












//==============================================================================
//! @class CustomFilelist Custom File List Class
//==============================================================================
class CustomFilelist : public QFrame
{
    Q_OBJECT

public:

    //! @brief Constructor
    //! @param aParent Parent Widget
    explicit CustomFilelist(QWidget* aParent = NULL);

    //! @brief Set Panel Name
    //! @param aPanelName Panel Name
    void setPanelName(const QString& aPanelName);

    //! @brief Set Current Directory
    //! @param aDirectory New Directory
    //! @param aRefresh Refresh
    //! @param aResetIndex Reset Index
    void setCurrentDir(const QString& aDirectory, const bool& aRefresh = false, const bool& aResetIndex = false);

    //! @brief Get Current Directory
    //! @param none
    //! @brief Current Directory
    QString getCurrentDir();

    //! @brief Get Item Count
    //! @param none
    //! @brief Item Count
    int getItemCount();

    //! @brief Get Current Directory's Files Count WITHOUT COUNTINT '..'
    //! @param none
    //! @brief Current Directory's Files Count
    int getFilesCount();

    //! @brief Get Current Directory's Selected Files Count
    //! @param none
    //! @brief Current Directory's Selected File Count
    int getSelectedCount();

    //! @brief Get Item Data By Index
    //! @param aIndex Item Index
    //! @return Item Data Or NULL
    FileItemData* getItemData(const int& aIndex);

    //! @brief Get Current Index
    //! @param none
    //! @return Current Index
    int getCurrentIndex();

    //! @brief Get Current Directory's Selected Files Size
    //! @param none
    //! @brief Current Directory's Selected File Size
    qint64 getSelectedSize();

    //! @brief Get File/Directory Size
    //! @param aFilePath File Path
    //! @return File/Directory Size
    qint64 getFileSize(const QString& aFilePath);

    //! @brief Set Sorting
    //! @param aSorting Sorting Order
    //! @param aReverse Sorting Reverse Order
    //! @param aRefresh Refresh
    void setSorting(const FileSortType& aSorting, const bool& aReverse, const bool& aRefresh = true);

    //! @brief Get Sorting Mode
    //! @param none
    //! @return Sorting Mode
    FileSortType getSorting();

    //! @brief Set Show Hidden Files
    //! @param aShowHidden Show Hidden Files Flag
    //! @param aRefresh Refresh
    void setShowHiddenFiles(const bool& aShowHidden, const bool& aRefresh = true);

    //! @brief Get Show Hidden Files Mode
    //! @param none
    //! @return Show Hidden Files Mode
    bool getShowHiddenFiles();

    //! @brief Set Name Filters
    //! @param aNameFilters Name Filters
    //! @param aRefresh Refresh
    void setNameFilters(const QStringList& aNameFilters, const bool& aRefresh = true);

    //! @brief Get Sorting Direction
    //! @param none
    //! @return Sorting Direction
    bool getSortDirection();

    //! @brief Set File List Item Header
    //! @param aHeader Custom File List Item Header
    void setFileListHeader(CustomHeader* aHeader);

    //! @brief Set Active
    //! @param aActive Active State
    void setActive(const bool& aActive);

    //! @brief Set Item Icon Size
    //! @param aIconSize Icon Size
    //! @param aRefresh Refresh
    void setItemIconSize(const int& aIconSize, const bool& aRefresh = true);

    //! @brief Get Item Icon Size
    //! @param none
    //! @return Icon Size
    int getItemIconSize();

    //! @brief Set List Box Item/Delegate Spacing
    //! @param aSpacing List Box Item/Delegate Spacing
    //! @param aRefresh Refresh
    void setItemSpacing(const int& aSpacing, const bool& aRefresh = true);

    //! @brief Get List Box Item/Delegate Spacing
    //! @param none
    //! @return Item/Delegate Spacing
    int getItemSpacing();

    //! @brief Update Delegate Icon Size
    //! @param aRefresh Refresh
    void updateDelegateIconSize(const bool& aRefresh = true);

    //! @brief Reload Dir
    //! @param aResetIndex Reset File Listbox Index
    void reload(const bool& aResetIndex = false);

    //! @brief Reload Dir
    //! @param aBgColor Background Color
    void setBackgroundColor(const int& aBgColor);

    //! @brief Get File List Box
    //! @param none
    //! @brief File List Box
    const FileListBox* listbox();

    //! @brief Feed Search Result To The List Box
    //! @param none
    void feedSearchResult();

    //! @brief Destructor
    //! @param none
    virtual ~CustomFilelist();

public slots:

    //! @brief Go To User Home Directory
    //! @param none
    void gotoHome();

    //! @brief Go To Root Directory
    //! @param none
    void gotoRoot();

    //! @brief Go Up to Parent Directory
    //! @param none
    void goUp();

    //! @brief Go Forward
    //! @param none
    void goForward();

    //! @brief Go Back
    //! @param none
    void goBack();

    //! @brief Go To Drive
    //! @param aIndex Drive Index
    void gotoDrive(const int& aIndex);

    //! @brief Go To Next Item
    //! @param aSelection Select Item
    void goNextItem(const bool& aSelection = false);

    //! @brief Go To Prev Item
    //! @param aSelection Select Item
    void goPrevItem(const bool& aSelection = false);

    //! @brief Go To Index
    //! @param aIndex Item Index
    //! @param aSelection Select Item
    void gotoIndex(const int& aIndex, const bool& aSelection = false);
/*
    //! @brief Make Directory
    //! @param aDirPath New Directory Name
    void makeDir(const QString& aDirPath);

    //! @brief Copy Files
    //! @param aPath Target Path
    void copyFiles(const QString& aPath);

    //! @brief Move Files
    //! @param aPath Target Path
    void moveFiles(const QString& aPath);

    //! @brief Delete Files
    //! @param none
    void deleteFiles();

    //! @brief Search Files
    //! @param aSearchTerm Search Term
    //! @param aContent Content Search Term
    //! @param aCase Case Sensitive
    //! @param aWhole Whole Word
    void searchFiles(const QString& aSearchTerm, const QString& aContent, const bool& aCase, const bool& aWhole);

    //! @brief Delete Files
    //! @param none
    void scanAllDirsSize();
*/
signals:

    //! @brief Current Directory Changed Signal
    //! @param aCurrDir Current Directory
    void currentDirChanged(const QString& aCurrDir);

    //! @brief Current Directory Changed Signal
    //! @param none
    void currentDirReadFinished();

    //! @brief File Selection Changed Signal
    //! @param aCount Files Count
    //! @param aSize Selected Files Size
    void fileSelectionChanged(const int& aCount, const int& aSize);

    //! @brief File Found Signal
    //! @param aFilePath Matching File Path
    void fileFound(const QString& aFilePath);

    //! @brief File List Key Pressed Signal
    //! @param aPanelName Panel Name
    //! @param aKey Key
    //! @param aModifiers Modifiers
    void fileListKeyPressed(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief File List Key Releases Signal
    //! @param aPanelName Panel Name
    //! @param aKey Key
    //! @param aModifiers Modifiers
    void fileListKeyReleased(const QString& aPanelName, const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief List Box Focus Changed Signal
    //! @param aName List Box Panel Name
    //! @param aFocused List Box Focused State
    void listBoxFocusChanged(const QString& aPanelName, const bool& aFocused);

    //! @brief File Selected Signal
    //! @param aFileItemData File Item Data
    void fileSelected(FileItemData* aFileItemData);

protected:

    //! @brief Clear
    //! @param none
    void clear();

    //! @brief Start File Item Size Scan
    //! @param aIndex File Index
    void startFileItemSizeScan(const int& aIndex);

    //! @brief Stop File Item Size Scan
    //! @param aIndex File Index
    void stopFileItemSizeScan(const int& aIndex);

    //! @brief Start All Items Size Scan
    //! @param none
    void startAllItemsSizeScan();

    //! @brief Stop All Items Size Scan
    //! @param none
    void stopAllItemsSizeScan();

    //! @brief Start File System Watcher Filter Timer
    //! @param none
    void startFSWFilterTimer();

    //! @brief Stop File System Watcher Filter Timer
    //! @param none
    void stopFSWFilterTimer();

protected: // From QObject

    //! @brief Timer Event
    //! @param aEvent Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

protected slots:

    //! @brief Dir Reader Entry Found Slot
    //! @param aFilePath File Path
    void dirReaderEntryFound(const QString& aFilePath);

    //! @brief Dir Reader Started Slot
    //! @param none
    void dirReaderStarted();

    //! @brief Dir Reader Stopped Slot
    //! @param none
    void dirReaderStopped();

    //! @brief Dir Reader Finished Slot
    //! @param none
    void dirReaderFinished();

    //! @brief Dir Reader Thread Finished Slot
    //! @param none
    void dirReaderThreadFinished();

    //! @brief List Box Key Pressed Slot
    //! @param aKey Key
    //! @param aModifiers Key Modifiers
    void listBoxKeyPressed(const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief List Box Key Released Slot
    //! @param aKey Key
    //! @param aModifiers Key Modifiers
    void listBoxKeyReleased(const int& aKey, const Qt::KeyboardModifiers& aModifiers);

    //! @brief List Box Item Selected Slot
    //! @param aIndex Item Index
    void listBoxItemSelected(const int& aIndex);

    //! @brief List Box Item Options Slot
    //! @param aIndex Item Index
    //! @param aPos Item Options Position
    void listBoxItemOptions(const int& aIndex, const QPoint& aPos);

    //! @brief Item Icon Size Changed Slot
    //! @param aRefresh Refresh/Reload
    void listBoxItemIconSizeChanged(const bool& aRefresh = true);

    //! @brief Item Size Scan Finished Slot
    //! @param aIndex Item Index
    void itemSizeScanFinished(const int& aIndex);

    //! @brief File System Directory Changed Slot
    //! @param aPath Directory Path
    void fsDirectoryChanged(const QString& aPath);


protected: // Data
    friend class MainWindow;
    friend class CustomPanel;

    //! UI
    Ui::CustomFilelist*     ui;
    //! Settings
    Settings*               settings;
    //! Set Panel Name
    QString                 panelName;
    //! Startup
    bool                    forceRefresh;
    //! Current Dir
    QDir                    currentDir;
    //! Current Dir Path
    QString                 currentDirPath;
    //! Previous Dir name
    QString                 prevDirName;
    //! Previous File Name
    QString                 prevFileName;
    //! Previous File Index
    int                     prevFileIndex;

    //! Directory Reader
    DirReader*              dirReader;

    //! Icon Scanner

    //! Deleter

    //! Copier

    //! Mover

    //! Search Engine

    //! File Utils
    FileUtils*              fileUtils;

    //! File Utils Client
    FileUtilsClient*        fileUtilsClient;

    //! Sort Order
    FileSortType            sortOrder;
    //! Reverse Order
    bool                    reverseOrder;
    //! Show Hidden
    bool                    showHidden;
    //! Name Filters
    QStringList             nameFilters;
    //! Need To Clear Flag
    bool                    needToClear;
    //! Dir Items Size Scan Active
    bool                    dirItemsSizeScanActive;
    //! Size Scan Current Item
    int                     sizeScanCurrent;

    //! File System Watcher
    QFileSystemWatcher*     fileSystemWatcher;

    //! File System Watcher Filter Timer ID
    int                     fswTimerID;

    //! File List Popup
    ListPopup*              popup;

    //! Operation Queue
    //QStringList             opQueue;
};

#endif // CUSTOMFILELIST_H
