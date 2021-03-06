#include <QDir>
#include <QDateTime>
#include <QDebug>

#include <mcwinterface.h>

#include "filelistmodel.h"
#include "remotefileutilclient.h"
#include "utility.h"
#include "defaultsettings.h"
#include "constants.h"



//==============================================================================
// Constructor
//==============================================================================
FileListModelItem::FileListModelItem(const QString& aPath, const QString& aFileName)
    : fileInfo(aPath + "/" + aFileName)
    , dirSize(0)
    , selected(false)
    , searchResult(false)
{
    // ...
}

//==============================================================================
// Constructor
//==============================================================================
FileListModelItem::FileListModelItem(const QString& aPath, const qint64& aSize, const QDateTime& aDate, const QString& aAttrs, const bool& aIsDir, const bool& aIsLink)
    : dirSize(0)
    , selected(false)
    , searchResult(false)
{
    // Set Up Archive File Info
    archiveFileInfo.filePath    = aPath;
    archiveFileInfo.fileName    = getFileNameFromFullName(aPath);
    archiveFileInfo.fileSize    = aSize;
    archiveFileInfo.fileDate    = aDate;
    archiveFileInfo.fileAttribs = aAttrs;
    archiveFileInfo.fileIsDir   = aIsDir;
    archiveFileInfo.fileIsLink  = aIsLink;
}







//==============================================================================
// Constructor
//==============================================================================
FileListModel::FileListModel(QObject* aParent)
    : QAbstractListModel(aParent)
    , currentDir("")
    , prevCurrentDir("")
    , fileUtil(NULL)
    , sorting(0)
    , reverseOrder(false)
    , showHiddenFiles(false)
    , showDirsFirst(true)
    , caseSensitiveSort(false)
    , selectedCount(0)
    , fetchOnConnection(false)
    , archiveMode(false)
    , archivePath("")
{
    //qDebug() << "FileListModel::FileListModel";

    // Init
    init();

    // ...
}

//==============================================================================
// Init
//==============================================================================
void FileListModel::init()
{
    qDebug() << "FileListModel::init";

    // Create File Utin
    fileUtil = new RemoteFileUtilClient();

    // Connect Signals
    connect(fileUtil, SIGNAL(clientConnectionChanged(uint,bool)), this, SLOT(clientConnectionChanged(uint,bool)));
    connect(fileUtil, SIGNAL(clientStatusChanged(uint, int)), this, SLOT(clientStatusChanged(uint, int)));
    connect(fileUtil, SIGNAL(dirListItemFound(uint,QString,QString)), this, SLOT(dirListItemFound(uint,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpQueueItemFound(uint,QString,QString,QString,QString)), this, SLOT(fileOpQueueItemFound(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)), this, SLOT(fileOpNeedConfirm(uint,QString,int,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpSkipped(uint,QString,QString,QString,QString)), this, SLOT(fileOpSkipped(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpFinished(uint,QString,QString,QString,QString)), this, SLOT(fileOpFinished(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpAborted(uint,QString,QString,QString,QString)), this, SLOT(fileOpAborted(uint,QString,QString,QString,QString)));
    connect(fileUtil, SIGNAL(fileOpError(uint,QString,QString,QString,QString,int)), this, SLOT(fileOpError(uint,QString,QString,QString,QString,int)));
    connect(fileUtil, SIGNAL(archiveListItemFound(uint,QString,QString,qint64,QDateTime,QString,bool,bool)), this, SLOT(archiveListItemFound(uint,QString,QString,qint64,QDateTime,QString,bool,bool)));

    // Connect To File Server
    fileUtil->connectToFileServer();

    // ...
}

//==============================================================================
// Get Current Dir
//==============================================================================
QString FileListModel::getCurrentDir()
{
    return currentDir;
}

//==============================================================================
// Set Current Dir
//==============================================================================
void FileListModel::setCurrentDir(const QString& aDirPath)
{
    // Check Current Dir
    if (currentDir != aDirPath) {
        qDebug() << "FileListModel::setCurrentDir - aDirPath: " << aDirPath;

        // Set Current Dir
        currentDir = aDirPath;
        // Reset Prev Current Dir
        prevCurrentDir = "";

        // Reset Archive Mode
        setArchiveMode(false);

        // Reload
        reload();

        // Emit Current dir Changed Signal
        emit currentDirChanged(currentDir);
    }
}

//==============================================================================
// Set Archive Current Dir
//==============================================================================
void FileListModel::setArchiveCurrentDir(const QString& aFilePath, const QString& aDirPath)
{
    // Check Archiove Path
    if (archivePath != aFilePath || currentDir != aDirPath) {

        // Set Archive Path
        archivePath = aFilePath;

        // Check Archive Mode
        if (!archiveMode) {
            // Save Current Dir
            prevCurrentDir = currentDir;
        }

        // Set Current Dir
        currentDir = aDirPath;

        // Set Archive Mode
        setArchiveMode(true);

        // Reload
        reload();

        // Emit Current dir Changed Signal
        emit currentDirChanged(QString(DEFAULT_ARCHIVE_FILE_DIR_PATTERN).arg(getBaseNameFromFullName(archivePath)).arg(currentDir));
    }
}

//==============================================================================
// Set Sorting Order
//==============================================================================
void FileListModel::setSorting(const int& aSorting)
{
    // Check Sorting
    if (sorting != aSorting) {
        // Set Sorting
        sorting = aSorting;

        // ...
    }
}

//==============================================================================
// Set Reverse Mode
//==============================================================================
void FileListModel::setReverse(const bool& aReverse)
{
    // Check Reverse
    if (reverseOrder != aReverse) {
        // Ser Reverse Order
        reverseOrder = aReverse;

        // ...
    }
}

//==============================================================================
// Set Show hidden Files
//==============================================================================
void FileListModel::setShowHiddenFiles(const bool& aShow)
{
    // Check Show Hidden Files
    if (showHiddenFiles != aShow) {
        // Set Show Hidden Files
        showHiddenFiles = aShow;

        // ...
    }
}

//==============================================================================
// Set Show Dirs First
//==============================================================================
void FileListModel::setShowDirsFirst(const bool& aShowDirsFirst)
{
    // Check Show Dirs First
    if (showDirsFirst != aShowDirsFirst) {
        // Set Show Dirs First
        showDirsFirst = aShowDirsFirst;

        // ...
    }
}

//==============================================================================
// Set Case Sensitive Sorting
//==============================================================================
void FileListModel::setCaseSensitiveSorting(const bool& aCaseSensitiveSort)
{
    // Check Case Sensitive Sorting
    if (caseSensitiveSort != aCaseSensitiveSort) {
        // Set Case Sensitive Sorting
        caseSensitiveSort = aCaseSensitiveSort;

        // ...
    }
}

//==============================================================================
// Get File Count
//==============================================================================
int FileListModel::getFileCount()
{
    // Check If Empty
    if (itemList.isEmpty()) {
        return 0;
    }

    // Check First Item
    if (itemList[0]->fileInfo.fileName() == QString(".."))
        return itemList.count() - 1;

    return itemList.count();
}

//==============================================================================
// Create Dir
//==============================================================================
void FileListModel::createDir(const QString& aDirPath)
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "FileListModel::createDir - aDirPath: " << aDirPath;
        // Create Dir
        fileUtil->createDir(aDirPath);
    }
}

//==============================================================================
// Create Link
//==============================================================================
void FileListModel::createLink(const QString& aLinkPath, const QString& aLinkTarget)
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "FileListModel::createLink - aLinkPath: " << aLinkPath << " - aLinkTarget: " << aLinkTarget;
        // Create Link
        fileUtil->createLink(aLinkPath, aLinkTarget);
    }
}

//==============================================================================
// Rename File
//==============================================================================
void FileListModel::renameFile(const QString& aSource, const QString& aTarget)
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "FileListModel::renameFile - aSource: " << aSource << " - aTarget: " << aTarget;
        // Rename/Move File
        fileUtil->moveFile(aSource, aTarget);
    }
}

//==============================================================================
// Delete File
//==============================================================================
void FileListModel::deleteFile(const QString& aFilePath)
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "FileListModel::deleteFile - aFilePath: " << aFilePath;
        // Delete File
        fileUtil->deleteFile(aFilePath);
    }
}

//==============================================================================
// Set Selected Files Count
//==============================================================================
void FileListModel::setSelectedCount(const int& aSelectedCount)
{
    // Check Selected Count
    if (selectedCount != aSelectedCount) {
        // Set Selected Count
        selectedCount = aSelectedCount;
        // Emit Signal
        emit selectedCountChanged(selectedCount);
    }
}

//==============================================================================
// Get Selected Files Count
//==============================================================================
int FileListModel::getSelectedCount()
{
    return selectedCount;
}

//==============================================================================
// Has Selection
//==============================================================================
bool FileListModel::hasSelection()
{
    return selectedCount > 0;
}

//==============================================================================
// Get Last Operation
//==============================================================================
QString FileListModel::lastOperation()
{
    // Check File Util
    if (fileUtil) {
        // Get Last Operation
        return fileUtil->lastOperation();
    }

    return "";
}

//==============================================================================
// Send User Response/Confirmation Code
//==============================================================================
void FileListModel::sendUserResponse(const int& aConfirm, const QString& aNewPath)
{
    // Check File Util
    if (fileUtil) {
        qDebug() << "FileListModel::sendUserResponse - aConfirm: " << aConfirm << " - aNewPath: " << aNewPath;
        // Send Confirm/Response
        fileUtil->sendUserResponse(aConfirm, aNewPath);
    }
}

//==============================================================================
// Calculate Item Index Based On Sorting Criterias
//==============================================================================
int FileListModel::calculateIndex(const QFileInfo& aItem)
{
    // Check Item
    if (aItem.exists()) {
        // Init Compare Method

        // Switch Sorting Mode
        switch (sorting) {
            default:

            case DEFAULT_SORT_NAME:
            case DEFAULT_SORT_EXT:
            case DEFAULT_SORT_TYPE:
            case DEFAULT_SORT_SIZE:
            case DEFAULT_SORT_DATE:
            case DEFAULT_SORT_OWNER:
            case DEFAULT_SORT_PERMS:
            case DEFAULT_SORT_ATTRS:

            break;
        }

        // Iterate While Item Is Greater

        // Return Index

    }

    return 0;
}

//==============================================================================
// Append Item Manually
//==============================================================================
void FileListModel::appendItem(const QString& aFilePath, const bool& aSearchResult)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(fileInfo.absolutePath(), fileInfo.fileName());
    // Set Search Result
    newItem->searchResult = aSearchResult;

    // Begin Insert Row
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Add Item To Item List
    itemList << newItem;

    // End Insert Row
    endInsertRows();

    // Add File Name To File Name List
    fileNameList << fileInfo.fileName();

    // Emit Count Changed Signal
    emit countChanged(itemList.count());
}

//==============================================================================
// Insert Item By File Path
//==============================================================================
void FileListModel::insertItem(const int& aIndex, const QString& aFilePath, const bool& aSearchResult)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(fileInfo.absolutePath(), fileInfo.fileName());
    // Set Search Result
    newItem->searchResult = aSearchResult;

    // Begin Insert Row
    beginInsertRows(QModelIndex(), aIndex, aIndex);

    // Add Item To Item List
    itemList.insert(aIndex, newItem);

    // End Insert Row
    endInsertRows();

    // Add File Name To File Name List
    fileNameList.insert(aIndex, fileInfo.fileName());

    // Emit Count Changed Signal
    emit countChanged(itemList.count());
}

//==============================================================================
// Add Item Name-Sorted Manually
//==============================================================================
void FileListModel::addItem(const QString& aFilePath, const bool& aSearchResult)
{
    // Init New File Info
    QFileInfo newFileInfo(aFilePath);

    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(newFileInfo.absolutePath(), newFileInfo.fileName());
    // Set Search Result
    newItem->searchResult = aSearchResult;

    // Get Model Count
    int ilCount = itemList.count();

    // Init Insert Index
    int insertIndex = 0;

    // Find Insert Index
    while (insertIndex < ilCount && compareFileNames(itemList[insertIndex]->fileInfo, newFileInfo) > 0 ) {
        // Increase Insert Index
        insertIndex++;
    }

    // Begin Insert Row
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);

    // Insert New Item
    itemList.insert(insertIndex, newItem);

    // End Insert Row
    endInsertRows();

    // Add File Name To File Name List
    fileNameList << newFileInfo.fileName();

    // Emit Count Changed Signal
    emit countChanged(itemList.count());
}

//==============================================================================
// Remove Item
//==============================================================================
void FileListModel::removeItem(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];

        // Begin Remove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);

        itemList.removeAt(aIndex);

        // End REmove Rows
        endRemoveRows();
        // Get Name Index
        int nameIndex = fileNameList.indexOf(item->fileInfo.fileName());
        // Check Index
        if (nameIndex >= 0) {
            // Remove From File NAme List
            fileNameList.removeAt(nameIndex);
        }

        // Delete Item
        delete item;
        item = NULL;

        // Emit Count Changed Signal
        emit countChanged(itemList.count());
    }
}

//==============================================================================
// Clear
//==============================================================================
void FileListModel::clear()
{
    // Check Item List
    if (itemList.count() <= 0) {
        return;
    }

    //qDebug() << "FileListModel::clear";

    // Begin Reset Model
    beginResetModel();

    // Go Thru File Info List
    while (itemList.count() > 0) {
        // Get File Info List Item
        FileListModelItem* item = itemList.takeLast();
        // Check Item
        if (item) {
            // Delete Item
            delete item;
            item = NULL;
        }
    }

    // End Reset Model
    endResetModel();

    // Emit Count Changed Signal
    emit countChanged(0);

    // Clear File Name List
    fileNameList.clear();

    // Reset Selected Count
    setSelectedCount(0);
}

//==============================================================================
// Reload
//==============================================================================
void FileListModel::reload()
{
    //qDebug() << "FileListModel::reload";

    // Clear
    clear();

    // Check Archive Mode
    if (archiveMode) {
        // Fetch Archive Dir Items
        fetchArchiveDirItems();
    } else {
        // Fetch Dir Items
        fetchDirItems();
    }
}

//==============================================================================
// Get Selected
//==============================================================================
bool FileListModel::getSelected(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < itemList.count()) {
        return itemList[aIndex]->selected;
    }

    return false;
}

//==============================================================================
// Set Selected
//==============================================================================
bool FileListModel::setSelected(const int& aIndex, const bool& aSelected)
{
    // Check Index
    if (aIndex >= 0 && aIndex < itemList.count()) {
        // Check File Name
        if (itemList[aIndex]->fileInfo.fileName() == "." || itemList[aIndex]->fileInfo.fileName() == "..") {
            // Skip
            return false;
        }

        // Check If Item Selected
        if (itemList[aIndex]->selected != aSelected) {
            //qDebug() << "FileListModel::setSelected - aIndex: " << aIndex << " - aSelected: " << aSelected;
            // Set Item Selected
            itemList[aIndex]->selected = aSelected;
            // Create Model Index
            QModelIndex index = createIndex(aIndex, 0);
            // Emit Data Changed Signal
            emit dataChanged(index, index);
            // Check Selected
            if (itemList[aIndex]->selected) {
                // Inc Selected Count
                selectedCount++;
            } else {
                // Dec Selected Count
                selectedCount--;
            }

            // Emit File Selection Changed Signal
            emit fileSelectionChanged(aIndex, itemList[aIndex]->selected);

            // Emit Selected Count Changed Signal
            emit selectedCountChanged(selectedCount);

            return true;
        }
    }

    return false;
}

//==============================================================================
// Select All
//==============================================================================
void FileListModel::selectAll()
{
    //qDebug() << "FileListModel::selectAll";

    // Get Item List Count
    int ilCount = itemList.count();
    // Reset Selected Count
    selectedCount = 0;
    // Go Thru Item List
    for (int i = 0; i < ilCount; ++i) {
        // Check File Name
        if (itemList[i]->fileInfo.fileName() != QString("..") && itemList[i]->fileInfo.fileName() != QString(".")) {
            // Check Selected
            if (!itemList[i]->selected) {
                // Set Item Selected
                itemList[i]->selected = true;
                // Create Model Index
                QModelIndex index = createIndex(i, 0);
                // Emit Data Changed Signal
                emit dataChanged(index, index);
                // Inc Selected Count
                selectedCount++;
            }
        }
    }

    // Emit Selected Count Changed Signal
    emit selectedCountChanged(selectedCount);
}

//==============================================================================
// Deselect All
//==============================================================================
void FileListModel::deselectAll()
{
    //qDebug() << "FileListModel::deselectAll";

    // Get Item List Count
    int ilCount = itemList.count();
    // Go Thru Item List
    for (int i = 0; i < ilCount; ++i) {
        // Check Selected
        if (itemList[i]->selected) {
            // Set Item Selected
            itemList[i]->selected = false;
            // Create Model Index
            QModelIndex index = createIndex(i, 0);
            // Emit Data Changed Signal
            emit dataChanged(index, index);
        }
    }

    // Reset Selected Count
    setSelectedCount(0);
}

//==============================================================================
// Toggle All Selection
//==============================================================================
void FileListModel::toggleAllSelection()
{
    //qDebug() << "FileListModel::toggleAllSelection";

    // Get Item List Count
    int ilCount = itemList.count();
    // Reset Selected Count
    selectedCount = 0;
    // Go Thru Item List
    for (int i = 0; i < ilCount; ++i) {
        // Check File Name
        if (itemList[i]->fileInfo.fileName() != QString("..") && itemList[i]->fileInfo.fileName() != QString(".")) {
            // Set Item Selected
            itemList[i]->selected = !itemList[i]->selected;
            // Check If Selected
            if (itemList[i]->selected) {
                // Inc Selected Count
                selectedCount++;
            }
            // Create Model Index
            QModelIndex index = createIndex(i, 0);
            // Emit Data Changed Signal
            emit dataChanged(index, index);
        }
    }

    // Emit Selected Count Changed Signal
    emit selectedCountChanged(selectedCount);
}

//==============================================================================
// Select Files
//==============================================================================
void FileListModel::selectFiles(const QString& aPattern)
{
    // Check Pattern
    if (aPattern == "*.*") {
        // Select All
        selectAll();

        return;
    }

    //qDebug() << "FileListModel::selectFiles - aPattern: " << aPattern;

    // Get Item List Count
    int ilCount = itemList.count();
    // Reset Selected Count
    selectedCount = 0;
    // Init Dir
    QDir dir(QDir::homePath());
    // Go Thru Item List
    for (int i = 0; i < ilCount; ++i) {
        // Get Item
        FileListModelItem* item = itemList[i];
        // Check File Name
        if (item->fileInfo.fileName() != QString("..") && item->fileInfo.fileName() != QString(".")) {
            // Check If Pattern Match
            if (dir.match(aPattern, item->fileInfo.fileName())) {
                // Set Item Selected
                item->selected = true;
                // Create Model Index
                QModelIndex index = createIndex(i, 0);
                // Emit Data Changed Signal
                emit dataChanged(index, index);
                // Inc Selected Count
                selectedCount++;
            }
        }
    }

    // Emit Selected Count Changed Signal
    emit selectedCountChanged(selectedCount);
}

//==============================================================================
// Deselect Files
//==============================================================================
void FileListModel::deselectFiles(const QString& aPattern)
{
    // Check Pattern
    if (aPattern == "*.*") {
        // Deselect All
        deselectAll();

        return;
    }

    //qDebug() << "FileListModel::deselectFiles - aPattern: " << aPattern;

    // Get Item List Count
    int ilCount = itemList.count();
    // Reset Selected Count
    selectedCount = 0;
    // Init Dir
    QDir dir(QDir::homePath());
    // Go Thru Item List
    for (int i = 0; i < ilCount; ++i) {
        // Get Item
        FileListModelItem* item = itemList[i];
        // Check File Name
        if (item->fileInfo.fileName() != QString("..") && item->fileInfo.fileName() != QString(".")) {
            // Check If Pattern Match
            if (dir.match(aPattern, item->fileInfo.fileName())) {
                // Set Item Selected
                item->selected = false;
                // Create Model Index
                QModelIndex index = createIndex(i, 0);
                // Emit Data Changed Signal
                emit dataChanged(index, index);
            }

            // Check If Item Selected
            if (item->selected) {
                // Inc Selected Count
                selectedCount++;
            }
        }
    }

    // Emit Selected Count Changed Signal
    emit selectedCountChanged(selectedCount);
}

//==============================================================================
// Get All Selected Files
//==============================================================================
QStringList FileListModel::getAllSelected()
{
    // Init Result
    QStringList result;

    // Get Item List Count
    int ilCount = itemList.count();
    // Go Thru Item List
    for (int i = 0; i < ilCount; ++i) {
        // Check Selected
        if (itemList[i]->selected && itemList[i]->fileInfo.fileName() != "." && itemList[i]->fileInfo.fileName() != "..") {
            // Add File Name To Result
            //result << itemList[i]->fileInfo.fileName();
            result << itemList[i]->fileInfo.absoluteFilePath();
        }
    }

    return result;
}

//==============================================================================
// Find Next Selected Index
//==============================================================================
int FileListModel::findNextSelected(const int& aIndex)
{
    // Chekc If Has Selection
    if (!hasSelection()) {
        return -1;
    }

    // Get Item List Count
    int ilCount = itemList.count();

    // Iterate Thru Items
    for (int i = aIndex; i< ilCount; ++i) {
        // Check Selected
        if (itemList[i]->selected) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Get File Name
//==============================================================================
QString FileListModel::getFileName(const int& aIndex)
{
    // Get Item List Count
    int ilCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < ilCount) {
        return itemList[aIndex]->fileInfo.fileName();
    }

    return "";
}

//==============================================================================
// Get Full File Path
//==============================================================================
QString FileListModel::getFullPath(const int& aIndex)
{
    // Get Item List Count
    int ilCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < ilCount) {
        // Get Full Path
        QString fullPath = itemList[aIndex]->fileInfo.absoluteFilePath();

        return fullPath;
    }

    return "";
}

//==============================================================================
// Get Dir Size
//==============================================================================
quint64 FileListModel::getDirSize(const int& aIndex)
{
    // Get Item List Count
    int ilCount = itemList.count();

    // Check Index
    if (aIndex >= 0 && aIndex < ilCount) {
        return itemList[aIndex]->dirSize;
    }

    return 0;
}

//==============================================================================
// Check If Path Is External Drive/Volume
//==============================================================================
bool FileListModel::isVolume(const QString& aFilePath)
{
    return isVolumePath(aFilePath);
}

//==============================================================================
// Eject Volume
//==============================================================================
void FileListModel::ejectVolume(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

    // Check File Path
    if (fileInfo.isSymLink()) {
        // Check Target
        if (fileInfo.symLinkTarget() == "/") {
            return;
        }
    }

    // Init Command Line
    QString cmdLine = QString(DEFAULT_EXTERNAL_DRIVE_EJECT_COMMAND_MAC).arg(aFilePath);

    // Execute System Command
    system(cmdLine.toLocal8Bit().data());
}

//==============================================================================
// Fetch Dir
//==============================================================================
void FileListModel::fetchDirItems()
{
    // Check File Util
    if (!fileUtil) {
        qWarning() << "FileListModel::fetchDirItems - NO FILE UTIL!!";
        return;
    }

    // Check If File Util Connected
    if (!fileUtil->isConnected()) {
        // Set Fetch On Connection
        fetchOnConnection = true;
        return;
    }

    //qDebug() << "FileListModel::fetchDirItems - currentDir: " << currentDir;

    // Init Filters
    int filters = showHiddenFiles ? DEFAULT_FILTER_SHOW_HIDDEN : 0;
    // Init Sort Flags
    int sortFlags = showDirsFirst ? DEFAULT_SORT_DIRFIRST : 0;

    // Adding Sorting Method
    sortFlags |= sorting;

    // Check Reverse Order
    if (reverseOrder) {
        // Adjust Sort Flags
        sortFlags |= DEFAULT_SORT_REVERSE;
    }

    // Check Case Sensitve Sorting
    if (caseSensitiveSort) {
        // Adjust Sort Flags
        sortFlags |= DEFAULT_SORT_CASE;
    }

    // ...

    // Fetch Dir Items
    fileUtil->getDirList(currentDir, filters, sortFlags);
}

//==============================================================================
// Fetch Archive Dir Items
//==============================================================================
void FileListModel::fetchArchiveDirItems()
{
    // Check File Util
    if (!fileUtil) {
        qWarning() << "FileListModel::fetchArchiveDirItems - NO FILE UTIL!!";
        return;
    }

    // Init Filters
    int filters = showHiddenFiles ? DEFAULT_FILTER_SHOW_HIDDEN : 0;
    // Init Sort Flags
    int sortFlags = showDirsFirst ? DEFAULT_SORT_DIRFIRST : 0;

    // Adding Sorting Method
    sortFlags |= sorting;

    // Check Reverse Order
    if (reverseOrder) {
        // Adjust Sort Flags
        sortFlags |= DEFAULT_SORT_REVERSE;
    }

    // Check Case Sensitve Sorting
    if (caseSensitiveSort) {
        // Adjust Sort Flags
        sortFlags |= DEFAULT_SORT_CASE;
    }

    // ...

    // Fetch Archive Dir Items
    fileUtil->listArchive(archivePath, currentDir, filters, sortFlags);
}

//==============================================================================
// Set Archive Mode
//==============================================================================
void FileListModel::setArchiveMode(const bool& aArchiveMode)
{
    // Check Archive Mode
    if (archiveMode != aArchiveMode) {
        // Set ARchive Mode
        archiveMode = aArchiveMode;
        // Check ARchive Mode
        if (!archiveMode) {
            // Reset ARchive Path
            archivePath = "";
        }

        // Emit Archive Mode Changed Signal
        emit archiveModeChanged(archiveMode);
    }
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void FileListModel::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "FileListModel::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected && fetchOnConnection) {
        // Reset Fetch On Connection
        fetchOnConnection = false;

        // Fetch Dir Items
        fetchDirItems();
    }
}

//==============================================================================
// Client Status Changed Slot
//==============================================================================
void FileListModel::clientStatusChanged(const unsigned int& aID, const int& aStatus)
{
    Q_UNUSED(aID);

    //qDebug() << "FileListModel::clientStatusChanged - aID: " << aID << " - aStatus: " << RemoteFileUtilClient::statusToString(aStatus);

    // Check Status
    if (aStatus == ECSTBusy || aStatus == ECSTAborting || aStatus == ECSTWaiting) {
        // Emit Busy Changed Signal
        emit busyChanged(true);
    } else {
        // Emit Busy Changed Signal
        emit busyChanged(false);
    }
}

//==============================================================================
// File Operation Finished Slot
//==============================================================================
void FileListModel::fileOpFinished(const unsigned int& aID,
                                   const QString& aOp,
                                   const QString& aPath,
                                   const QString& aSource,
                                   const QString& aTarget)
{
    //Q_UNUSED(aSource);
    //Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

    // Check Operation
    if (aOp == DEFAULT_OPERATION_LIST_DIR) {
        // Emit Dir Fetch Finished Signal
        emit dirFetchFinished();

    } else if (aOp == DEFAULT_OPERATION_LIST_ARCHIVE) {
        // Emit Dir Fetch Finished Signal
        emit dirFetchFinished();

    } else if (aOp == DEFAULT_OPERATION_MAKE_DIR) {
        // Emit Dir Created Signal
        emit dirCreated(aPath);

    } else if (aOp == DEFAULT_OPERATION_MAKE_LINK) {
        // Emit Link Created Signal
        emit linkCreated(aSource, aTarget);

    } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Init Source Info
        QFileInfo sourceInfo(aSource);
        // Check Path
        if (sourceInfo.absolutePath() == currentDir) {
            // Find Index
            int fileIndex = findIndex(sourceInfo.fileName());
            // Check File Index
            if (fileIndex >= 0) {
                // Remove item
                removeItem(fileIndex);
                // Init Target File Info
                QFileInfo targetInfo(aTarget);
                // Calculate New Index For Target
                int newIndex = calculateIndex(targetInfo);
                // Insert Item
                insertItem(newIndex, aTarget);
                // Emit File Renamed
                emit fileRenamed(aSource, aTarget);
            }
        }
    } else {
        qDebug() << "FileListModel::fileOpFinished - aOp: " << aOp << " - WTF?!?";
    }
}

//==============================================================================
// File Operation Skipped Slot
//==============================================================================
void FileListModel::fileOpSkipped(const unsigned int& aID,
                                  const QString& aOp,
                                  const QString& aPath,
                                  const QString& aSource,
                                  const QString& aTarget)
{
    Q_UNUSED(aPath);

    qDebug() << "FileListModel::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// File Operation Aborted Slot
//==============================================================================
void FileListModel::fileOpAborted(const unsigned int& aID,
                                  const QString& aOp,
                                  const QString& aPath,
                                  const QString& aSource,
                                  const QString& aTarget)
{
    //Q_UNUSED(aSource);
    //Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...

}

//==============================================================================
// File Operation Error Slot
//==============================================================================
void FileListModel::fileOpError(const unsigned int& aID,
                                const QString& aOp,
                                const QString& aPath,
                                const QString& aSource,
                                const QString& aTarget,
                                const int& aError)
{
    //Q_UNUSED(aSource);
    //Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aSource: " << aSource << " - aTarget: " << aTarget << " - aError: " << aError;

    // Emit Error Signal
    emit error(aPath, aSource, aTarget, aError);

    // ...
}

//==============================================================================
// Dir List Item Found Slot
//==============================================================================
void FileListModel::dirListItemFound(const unsigned int& aID,
                                     const QString& aPath,
                                     const QString& aFileName)
{
    Q_UNUSED(aID);

    //qDebug() << "FileListModel::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - aFileName: " << aFileName;

    // Check Current Dir vs Path
    if (currentDir != aPath) {
        qWarning() << "FileListModel::dirListItemFound - aID: " << aID << " - aPath: " << aPath << " - INVALID PATH!!";

        return;
    }

    // Check File Name
    if (fileNameList.indexOf(aFileName) >= 0) {
        qWarning() << "FileListModel::dirListItemFound - aID: " << aID << " - aFileName: " << aFileName << " - DUPLICATE ITEM!!";
        return;
    }

    // ...

    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(aPath, aFileName);

    // Begin Insert Row
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Add Item To Item List
    itemList << newItem;

    // End Insert Row
    endInsertRows();

    // Add File Name To File Name List
    fileNameList << aFileName;

    // Emit Count Changed Signal
    emit countChanged(itemList.count());
}

//==============================================================================
// Need Confirmation Slot
//==============================================================================
void FileListModel::fileOpNeedConfirm(const unsigned int& aID,
                                      const QString& aOp,
                                      const int& aCode,
                                      const QString& aPath,
                                      const QString& aSource,
                                      const QString& aTarget)
{

    qDebug() << "FileListModel::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Emit Need Confirm Signal
    emit needConfirm(aCode, aPath, aSource, aTarget);
}

//==============================================================================
// File Operation Queue Item Found Slot
//==============================================================================
void FileListModel::fileOpQueueItemFound(const unsigned int& aID,
                                         const QString& aOp,
                                         const QString& aPath,
                                         const QString& aSource,
                                         const QString& aTarget)
{
    //Q_UNUSED(aID);
    Q_UNUSED(aPath);

    qDebug() << "FileListModel::fileOpQueueItemFound - aID: " << aID << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // ...
}

//==============================================================================
// Archive File List Item Found Slot
//==============================================================================
void FileListModel::archiveListItemFound(const unsigned int& aID,
                                         const QString& aArchive,
                                         const QString& aFilePath,
                                         const qint64& aSize,
                                         const QDateTime& aDate,
                                         const QString& aAttribs,
                                         const bool& aIsDir,
                                         const bool& aIsLink)
{
    // Check Archive
    if (archivePath != aArchive) {
        qDebug() << "FileListModel::archiveListItemFound - aID: " << aID << " - aArchive: " << aArchive << " - INVALID ARCHIVE!!";

        return;
    }

    //qDebug() << "FileListModel::archiveListItemFound - aID: " << aID << " - aArchive: " << aArchive << " - aFilePath: " << aFilePath << " - aSize: " << aSize;


    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(aFilePath, aSize, aDate, aAttribs, aIsDir, aIsLink);

    // Begin Insert Row
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Add Item To Item List
    itemList << newItem;

    // End Insert Row
    endInsertRows();

    // Add File Name To File Name List
    fileNameList << newItem->archiveFileInfo.fileName;

    // Emit Count Changed Signal
    emit countChanged(itemList.count());

}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> FileListModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File Name
    roles[FileName]         = "fileName";
    // File Extension/Suffix
    roles[FileExtension]    = "fileExt";
    // File Type
    roles[FileType]         = "fileType";
    // File Name
    roles[FileAttributes]   = "fileAttrib";
    // File Name
    roles[FileSize]         = "fileSize";
    // File Name
    roles[FileDateTime]     = "fileDate";
    // File Name
    roles[FileOwner]        = "fileOwner";
    // File Permissions
    roles[FilePerms]        = "filePerms";
    // File Selected
    roles[FileSelected]     = "fileIsSelected";
    // File Search Result
    roles[FileSearchResult] = "fileIsSearchResult";
    // File Is Hidden
    roles[FileIsHidden]     = "fileIsHidden";
    // File Is Link
    roles[FileIsLink]       = "fileIsLink";
    // File Is Dir
    roles[FileIsDir ]       = "fileIsDir";
    // File Is Archive
    roles[FileIsArchive]    = "fileIsArchive";
    // Full File Name
    roles[FileFullName]     = "fileFullName";
    // Dir Size
    roles[FileDirSize]      = "dirSize";

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int FileListModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return itemList.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int FileListModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return FileRolesCount - Qt::UserRole;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant FileListModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {

        // Get Model Item
        FileListModelItem* item = itemList[aIndex.row()];

        // Check Archive Mode
        if (archiveMode) {

            // Switch Role
            switch (aRole) {
                case FileName: {
                // Check If Is Dir
                    if (item->archiveFileInfo.fileIsDir) {
                        return item->archiveFileInfo.fileName;
                    }

                    return getBaseNameFromFullName(item->archiveFileInfo.fileName);

                } break;

                case FileExtension: {
                    // Check If Is Dir
                    if (item->archiveFileInfo.fileIsDir) {
                        return QString("");
                    }

                    return getExtensionFromFullName(item->archiveFileInfo.fileName);

                } break;

                case FileSize: {
                    // Check File Info
                    if (item->archiveFileInfo.fileIsDir) {
                        return QString(DEFAULT_FILE_LIST_SIZE_DIR);
                    }

                    return formattedSize(item->archiveFileInfo.fileSize);

                } break;

                case FileDateTime:      return formatDateTime(item->archiveFileInfo.fileDate);
                case FileAttributes:
                case FilePerms:         return item->archiveFileInfo.fileAttribs;
                case FileSelected:      return item->selected;
                case FileFullName:      return item->archiveFileInfo.fileName;
                case FileIsHidden:      return false;
                case FileIsLink:        return item->archiveFileInfo.fileIsLink;
                case FileIsDir:         return item->archiveFileInfo.fileIsDir;
                case FileDirSize:       return formattedSize(item->dirSize);

                default:
                break;
            }

        } else {
            // Switch Role
            switch (aRole) {
                case FileName: {
                    // Check If Search Result
                    if (item->searchResult) {
                        return item->fileInfo.absoluteFilePath();
                    }

                    if (item->fileInfo.isSymLink()) {
                        return item->fileInfo.fileName() + " -> " + item->fileInfo.symLinkTarget();
                    }

                    // Check File Info
                    if (item->fileInfo.isBundle()) {
                        if (!item->fileInfo.bundleName().isEmpty())
                            return item->fileInfo.bundleName();

                        return item->fileInfo.fileName();
                    }

                    // Check Base Name
                    if (/*item->fileInfo.baseName().isEmpty() || */
                        item->fileInfo.isDir()) {
                        return item->fileInfo.fileName();
                    }

                    return getBaseNameFromFullName(item->fileInfo.fileName());

                } break;

                case FileExtension: {
                    // Check Base Name
                    if (/*item->fileInfo.baseName().isEmpty() ||*/
                        item->fileInfo.isDir()              ||
                        item->fileInfo.isBundle()) {
                        return QString("");
                    }

                    return getExtensionFromFullName(item->fileInfo.fileName());

                } break;

                case FileType: {
                    return QString("");
                } break;

                case FileAttributes: {
                    return QString("");
                } break;

                case FileSize: {
                    // Check File Info
                    if (item->fileInfo.isBundle()) {
                        return QString(DEFAULT_FILE_LIST_SIZE_BUNDLE);
                    }

                    // Check File Info
                    if (item->fileInfo.isDir()) {
                        return QString(DEFAULT_FILE_LIST_SIZE_DIR);
                    }

                    //return item->fileInfo.size();
                    return formattedSize(item->fileInfo.size());

                } break;

                case FileDateTime:      return formatDateTime(item->fileInfo.lastModified());
                case FileOwner:         return item->fileInfo.owner();
                case FilePerms:         return getPermsText(item->fileInfo);
                case FileSelected:      return item->selected;
                case FileSearchResult:  return item->searchResult;
                case FileFullName:      return item->fileInfo.fileName();
                case FileIsHidden:      return (item->fileInfo.fileName() == QString("..") ? false : item->fileInfo.isHidden());
                case FileIsLink:        return item->fileInfo.isSymLink();
                case FileIsDir:         return item->fileInfo.isDir();
                case FileIsArchive:     return isFileArchiveByExt(item->fileInfo.fileName());
                case FileDirSize:       return formattedSize(item->dirSize);

                default:
                break;
            }
        }
    }

    return QString("");
}

//==============================================================================
// Set Data
//==============================================================================
bool FileListModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {
        //qDebug() << "FileListModel::setData - row: " << aIndex.row() << " - aValue: " << aValue << " - aRole: " << aRole;

        // Get Item
        FileListModelItem* item = itemList[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case FileName: {

            } break;

            case FileExtension: {

            } break;

            case FileType: {

            } break;

            case FileAttributes: {

            } break;

            case FileSize: {

            } break;

            case FileDateTime: {

            } break;

            case FileOwner: {

            } break;

            case FilePerms: {

            } break;

            case FileSelected: {
                // Check Item
                if (item && item->fileInfo.fileName() != QString("..") && item->fileInfo.fileName() != QString(".")) {
                    // Set Selected
                    item->selected = aValue.toBool();
                    // Emit Data Changed Signal
                    emit dataChanged(aIndex, aIndex);

                    return true;
                }
            } break;

            case FileIsHidden: {

            } break;

            case FileIsLink: {

            } break;

            case FileDirSize: {
                // Check Item
                if (item && item->fileInfo.fileName() != QString("..") && item->fileInfo.fileName() != QString(".")) {
                    // Set Dir Size
                    item->dirSize = aValue.toULongLong();
                    // Emit Data Changed Signal
                    emit dataChanged(aIndex, aIndex);

                    return true;
                }
            } break;

            default: {

            } break;
        }
    }

    return false;
}

//==============================================================================
// Find Index
//==============================================================================
int FileListModel::findIndex(const QString& aFileName)
{
    // Get Count
    int flmCount = itemList.count();

    // Go Thru List
    for (int i = 0; i < flmCount; ++i) {
        // Get Item
        FileListModelItem* item = itemList[i];

        // Check Archive Mode
        if (archiveMode && item->archiveFileInfo.fileName == aFileName) {
            return i;
        }

        // Check Item
        if (item->fileInfo.fileName() == aFileName) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Insert Item by File Name - For Newly Create Directory
//==============================================================================
void FileListModel::insertDirItem(const QString& aFileName)
{
    // Check File Name
    if (fileNameList.indexOf(aFileName) >= 0) {
        qWarning() << "FileListModel::insertItem - aFileName: " << aFileName << " - DUPLICATE ITEM!!";
        return;
    }

    // ...

    // Create New File List Item
    FileListModelItem* newItem = new FileListModelItem(currentDir, aFileName);

    // Get Count
    int count = rowCount();
    // Loop Index
    int i = 0;

    // Check Reverse Order
    if (reverseOrder) {
        // Go Thru List
        while (i < count && itemList[i]->fileInfo.isDir() && itemList[i]->fileInfo.fileName() > aFileName) {
            // Inc Loop Index
            i++;
        }
    } else {
        // Go Thru List
        while (i < count && itemList[i]->fileInfo.isDir() && itemList[i]->fileInfo.fileName() < aFileName) {
            // Inc Loop Index
            i++;
        }
    }

    // Insert Item

    // Begin Insert Row
    beginInsertRows(QModelIndex(), i, i);

    // Add Item To Item List
    itemList.insert(i, newItem);

    // End Insert Row
    endInsertRows();

    // Add File Name To File Name List
    fileNameList.insert(i, aFileName);

    // Emit Count Changed Signal
    emit countChanged(itemList.count());
}

//==============================================================================
// Get File Info
//==============================================================================
QFileInfo FileListModel::getFileInfo(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];
        // Check if file Still Exists
        if (item && item->fileInfo.exists()) {
            // Return File Info
            return item->fileInfo;
        }
    }

    return QFileInfo("");
}

//==============================================================================
// Get Archive File Info
//==============================================================================
ArchiveFileInfo FileListModel::getArchiveFileInfo(const int& aIndex)
{
    // Check Index
    if (archiveMode && aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];

        return item->archiveFileInfo;
    }

    // Init Null File Info
    ArchiveFileInfo nullInfo;

    nullInfo.fileName   = "";
    nullInfo.filePath   = "";
    nullInfo.fileIsDir  = false;
    nullInfo.fileIsLink = false;
    nullInfo.fileSize   = 0;

    return nullInfo;
}

//==============================================================================
// Update Item
//==============================================================================
void FileListModel::updateItem(const int& aIndex, const QFileInfo& aFileInfo)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];

        // Set File Info
        item->fileInfo = aFileInfo;
        // Get Update Index
        QModelIndex updatedIndex = createIndex(aIndex, 0);

        // Emit Data Changed
        emit dataChanged(updatedIndex, updatedIndex);
    }
}

//==============================================================================
// Update Dir Size
//==============================================================================
void FileListModel::updateDirSize(const int& aIndex, const quint64& aSize)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];

        // Set File Info
        item->dirSize = aSize;
        // Get Update Index
        QModelIndex updatedIndex = createIndex(aIndex, 0);

        // Emit Data Changed
        emit dataChanged(updatedIndex, updatedIndex);
    }
}

//==============================================================================
// Check If Is Dir
//==============================================================================
bool FileListModel::isDir(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];
        // Return File Info
        return item->fileInfo.isDir();
    }

    return false;
}

//==============================================================================
// Check If Is Bundle
//==============================================================================
bool FileListModel::isBundle(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];
        // Return File Info
        return item->fileInfo.isBundle();
    }

    return false;
}

//==============================================================================
// Check If Archive
//==============================================================================
bool FileListModel::isArchive(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        // Get Item
        FileListModelItem* item = itemList[aIndex];

        // Return File Info
        return isFileArchiveByExt(item->fileInfo.fileName());
    }

    return false;
}

//==============================================================================
// Get Busy
//==============================================================================
bool FileListModel::getBusy()
{
    return fileUtil ? (fileUtil->getStatus() == ECSTBusy) || (fileUtil->getStatus() == ECSTAborting) : false;
}

//==============================================================================
// Get Archive Mode
//==============================================================================
bool FileListModel::getArchiveMode()
{
    return archiveMode;
}

//==============================================================================
// Destructor
//==============================================================================
FileListModel::~FileListModel()
{
    // Clear
    clear();

    // Check File Util
    if (fileUtil) {
        // Close
        fileUtil->close();

        // Delete File Util
        delete fileUtil;
        fileUtil = NULL;
    }

    // ...

    //qDebug() << "FileListModel::~FileListModel";
}

