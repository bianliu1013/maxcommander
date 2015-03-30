#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QFileSystemWatcher>

namespace Ui {
class FilePanel;
}

class FileListModel;
class MainWindow;
class FileListImageProvider;

//==============================================================================
// File Panel Class
//==============================================================================
class FilePanel : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString panelName READ getPanelName)
    Q_PROPERTY(bool panelHasFocus READ getPanelFocus WRITE setPanelFocus NOTIFY panelFocusChanged)
    Q_PROPERTY(int visualItemsCount READ getvisualItemsCount WRITE setVisualItemsCount NOTIFY visualItemsCountChanged)

    Q_PROPERTY(bool showHidden READ getShowHidden WRITE setShowHidden NOTIFY showHiddenChanged)

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

public:
    // Constructor
    explicit FilePanel(QWidget* aParent = NULL);

    // Get Current Dir
    QString getCurrentDir();
    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);

    // Get Panel Name
    QString getPanelName();
    // Set Panel Name
    void setPanelName(const QString& aPanelName);

    // Get Panel Focus
    bool getPanelFocus();
    // Get Current Index
    int getCurrentIndex();
    // Get Visual Items Count
    int getvisualItemsCount();

    // Get Show Hidden
    bool getShowHidden();
    // Set Show Hidden
    void setShowHidden(const bool& aHidden);

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

    // Destructor
    virtual ~FilePanel();

public slots:

    // Go To Home Directory
    void gotoHome();
    // Go To Root
    void gotoRoot();
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

    // Handle Item Select
    void handleItemSelect();
    // Rename
    void renameCurrent();

    // Set Panel Focus
    void setPanelFocus(const bool& aFocus);
    // Set Current Index
    void setCurrentIndex(const int& aCurrentIndex);
    // Set Visual Items Count
    void setVisualItemsCount(const int& aVisualCount);

    // Get Supported Image Formats
    QStringList getSupportedImageFormats();

    // Reload
    void reload(const int& aIndex);

    // Select All Files
    void selectAllFiles();
    // Deselect All Files
    void deselectAllFiles();
    // Toggle Current File Selection
    void toggleCurrentFileSelection();
    // Get Selected Files
    QStringList getSelectedFiles();

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
    // Visual Items Count Changed
    void visualItemsCountChanged(const int& aVisualCount);

    // Show Hidden Changed Signal
    void showHiddenChanged(const bool& aHidden);

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

protected slots:

    // Init
    void init();
    // Clear
    void clear();
    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();
    // Update Available Space Label
    void updateAvailableSpaceLabel();
    // Update Supported Image Formates
    void updateSupportedImageFormats();

protected slots: // From File Model

    // File Model Fetch Ready
    void fileModelDirFetchFinished();
    // Start Dir Watcher
    void startDirWatcher();
    // Stop Dir Watcher
    void stopDirWatcher();

protected slots: // From QFileSystemWatcher

    // Directory Changed Slot
    void directoryChanged(const QString& aDirPath);
    // File Changed Slot
    void fileChanged(const QString& aFilePath);

    // Refresh File List Model
    void refreshFileListModel(const QString& aFilePath);

private slots:

    // Home Button Clicked Slot
    void on_homeButton_clicked();
    // Root Button Clicked Slot
    void on_rootButton_clicked();

protected: // From QWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);


private:
    friend class MainWindow;

    // UI
    Ui::FilePanel*          ui;

    // Current Dir
    QString                 currentDir;

    // Panel Name
    QString                 panelName;

    // Panel Focus
    bool                    panelHasFocus;

    // File List Model
    FileListModel*          fileListModel;

    // File List image Provider
    FileListImageProvider*  fileListImageProvider;

    // Modifier Keys
    int                     modifierKeys;

    // Current Index
    int                     currentIndex;

    // Visual Items Count
    int                     visualItemsCount;

    // Last Directory Name To Jump After CD UP
    QString                 lastDirName;
    // Last Index Before Reload
    int                     lastIndex;

    // Supported Image Formats
    QStringList             supportedImageFormats;

    // Show Hidden Files
    bool                    showHidden;

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

    // Sorting
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

};

#endif // FILEPANEL_H
