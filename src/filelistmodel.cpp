#include <QDir>
#include <QSettings>
#include <QDateTime>
#include <QDebug>

#include <mcwinterface.h>

#include "filelistmodel.h"
#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"



//==============================================================================
// Constructor
//==============================================================================
FileListModelItem::FileListModelItem(const QString& aPath, const QString& aFileName)
    : fileInfo(aPath + "/" + aFileName)
    , dirSize(0)
    , selected(false)
{
    // ...
}








//==============================================================================
// Constructor
//==============================================================================
FileListModel::FileListModel(QObject* aParent)
    : QAbstractListModel(aParent)
    , currentDir(QDir::homePath())
    , fileUtil(NULL)
    , sorting(0)
    , reverseOrder(false)
    , selectedCount(0)
{
    qDebug() << "FileListModel::FileListModel";

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
void FileListModel::setCurrentDir(const QString& aCurrentDir)
{
    // Check Current Dir
    if (currentDir != aCurrentDir) {
        qDebug() << "FileListModel::setCurrentDir - aCurrentDir: " << aCurrentDir;

        // Set Current Dir
        currentDir = aCurrentDir;

        // Clear
        clear();

        // Fetch Current Dir Items
        fetchDirItems();

        // ...

        // Emit Current dir Changed Signal
        currentDirChanged(currentDir);
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
// Get File Count
//==============================================================================
int FileListModel::getFileCount()
{
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
// Clear
//==============================================================================
void FileListModel::clear()
{
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
    emit countChanged(itemList.count());

    // Clear File Name List
    fileNameList.clear();

    // Reset Selected Count
    selectedCount = 0;
}

//==============================================================================
// Reload
//==============================================================================
void FileListModel::reload()
{
    //qDebug() << "FileListModel::reload";

    // Clear
    clear();

    // Fetch Dir Items
    fetchDirItems();
}

//==============================================================================
// Get Selected
//==============================================================================
bool FileListModel::getSelected(const int& aIndex)
{
    // Check Index
    if (aIndex >=0 && aIndex < itemList.count()) {
        return itemList[aIndex]->selected;
    }

    return false;
}

//==============================================================================
// Set Selected
//==============================================================================
void FileListModel::setSelected(const int& aIndex, const bool& aSelected)
{
    // Check Index
    if (aIndex >=0 && aIndex < itemList.count()) {

        // Check File Name
        if (itemList[aIndex]->fileInfo.fileName() == "." || itemList[aIndex]->fileInfo.fileName() == ".."  ) {

            // Skip
            return;
        }

        // Check If Item Selected
        if (itemList[aIndex]->selected != aSelected) {
            qDebug() << "FileListModel::setSelected - aIndex: " << aIndex << " - aSelected: " << aSelected;
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
        }
    }
}

//==============================================================================
// Select All
//==============================================================================
void FileListModel::selectAll()
{
    qDebug() << "FileListModel::selectAll";
    // Get Item List Count
    int ilCount = itemList.count();
    // Reset Selected Count
    selectedCount = 0;
    // Go Thru Item List
    for (int i=0; i<ilCount; ++i) {
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
}

//==============================================================================
// Deselect All
//==============================================================================
void FileListModel::deselectAll()
{
    qDebug() << "FileListModel::deselectAll";
    // Get Item List Count
    int ilCount = itemList.count();
    // Go Thru Item List
    for (int i=0; i<ilCount; ++i) {
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
    selectedCount = 0;
}

//==============================================================================
// Toggle All Selection
//==============================================================================
void FileListModel::toggleAllSelection()
{
    qDebug() << "FileListModel::toggleAllSelection";
    // Get Item List Count
    int ilCount = itemList.count();
    // Reset Selected Count
    selectedCount = 0;
    // Go Thru Item List
    for (int i=0; i<ilCount; ++i) {

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
    for (int i=0; i<ilCount; ++i) {
        // Check Selected
        if (itemList[i]->selected && itemList[i]->fileInfo.fileName() != "." && itemList[i]->fileInfo.fileName() != "..") {
            // Add File Name To Result
            result << itemList[i]->fileInfo.fileName();
        }
    }

    return result;
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
// Fetch Dir
//==============================================================================
void FileListModel::fetchDirItems()
{
    // Check File Util
    if (!fileUtil) {
        qWarning() << "FileListModel::fetchDirItems - NO FILE UTIL!!";
        return;
    }

    qDebug() << "FileListModel::fetchDirItems - currentDir: " << currentDir;

    // Init Settings
    QSettings settings;

    // Init Filters
    int filters = settings.value(SETTINGS_KEY_SHOW_HIDDEN_FILES, true).toBool() ? DEFAULT_FILTER_SHOW_HIDDEN : 0;

    // ...

    // Init Sort Flags
    int sortFlags = settings.value(SETTINGS_KEY_DIRFIRST, true).toBool() ? DEFAULT_SORT_DIRFIRST : 0;

    // Adding Sorting Method
    sortFlags |= sorting;

    // Check Reverse Order
    if (reverseOrder) {
        sortFlags |= DEFAULT_SORT_REVERSE;
    }

    // ...

    // Fetch Dir Items
    fileUtil->getDirList(currentDir, filters, sortFlags);
}

//==============================================================================
// Delete Item
//==============================================================================
void FileListModel::deleteItem(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0  && aIndex < itemList.count()) {
        qDebug() << "FileListModel::deleteItem - aIndex: " << aIndex;

        // Get File Name
        QString fileName = itemList[aIndex]->fileInfo.fileName();

        // Delete Item
        delete itemList.takeAt(aIndex);

        // Find File Name Index
        int index = fileNameList.indexOf(fileName);
        // Check Index
        if (index >= 0 && index < fileNameList.count()) {
            // Remove File Name
            fileNameList.removeAt(index);
        }
    }
}

//==============================================================================
// Client Connection Changed Slot
//==============================================================================
void FileListModel::clientConnectionChanged(const unsigned int& aID, const bool& aConnected)
{
    qDebug() << "FileListModel::clientConnectionChanged - aID: " << aID << " - aConnected: " << aConnected;

    // Check If Connected
    if (aConnected) {
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
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpFinished - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

    // ...

    // Check Operation
    if (aOp == DEFAULT_OPERATION_LIST_DIR) {
        // Emit Dir Fetch Finished Signal
        emit dirFetchFinished();
    } else if (aOp == DEFAULT_OPERATION_MAKE_DIR) {
        // Emit Dir Create Signal
        emit dirCreated(aPath);
    } else if (aOp == DEFAULT_OPERATION_MOVE_FILE) {
        // Init Source Info
        QFileInfo sourceInfo(aSource);
        // Check Path
        if (sourceInfo.absolutePath() == currentDir) {
            // Find Index
            int fileIndex = findIndex(sourceInfo.fileName());
            // Check File Index
            if (fileIndex >= 0) {
                // Get Item
                FileListModelItem* item = itemList[fileIndex];
                // Update File Info
                item->fileInfo = QFileInfo(aTarget);
                // Create Index
                QModelIndex updateIndex = createIndex(fileIndex, 0);
                // Emit Data Changed
                emit dataChanged(updateIndex, updateIndex);
                // Emit File Renamed
                emit fileRenamed(aSource, aTarget);
            }
        }
    } else {
        qDebug() << "FileListModel::fileOpFinished - WTF?!?";
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
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpAborted - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath;

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
    Q_UNUSED(aSource);
    Q_UNUSED(aTarget);

    qDebug() << "FileListModel::fileOpError - aID: " << aID << " - aOp: " << aOp << " - aPath: " << aPath << " - aError: " << aError;

    // Emit Error Signal
    emit error(aPath, aError);

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
    roles[FilePerms]        = "fileParms";
    // File Selected
    roles[FileSelected]     = "fileIsSelected";
    // File Is Hidden
    roles[FileIsHidden]     = "fileIsHidden";
    // File Is Link
    roles[FileIsLink]       = "fileIsLink";
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

        // Switch Role
        switch (aRole) {
            case FileName: {
                // Check File Info
                if (item->fileInfo.isBundle()) {
                    if (!item->fileInfo.bundleName().isEmpty())
                        return item->fileInfo.bundleName();

                    return item->fileInfo.fileName();
                }

                // Check Base Name
                if (item->fileInfo.baseName().isEmpty() || item->fileInfo.isDir()) {
                    return item->fileInfo.fileName();
                }

                return getFileNameFromFullName(item->fileInfo.fileName());

            } break;

            case FileExtension: {
                // Check Base Name
                if (item->fileInfo.baseName().isEmpty() ||
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

            } break;

            case FileSize: {
                // Check File Info
                if (item->fileInfo.isBundle()) {
                    return QString("[BUNDLE]");
                }

                // Check File Info
                if (item->fileInfo.isDir()) {
                    return QString("[DIR]");
                }

                return item->fileInfo.size();
            } break;

            case FileDateTime:      return formatDateTime(item->fileInfo.lastModified());
            case FileOwner:         return item->fileInfo.owner();
            case FilePerms:         return (int)item->fileInfo.permissions();
            case FileSelected:      return item->selected;
            case FileFullName:      return item->fileInfo.fileName();
            case FileIsHidden:      return (item->fileInfo.fileName() == QString("..") ? false : item->fileInfo.isHidden());
            case FileIsLink:        return item->fileInfo.isSymLink();

            case FileDirSize:       return item->dirSize;

            default:
            break;
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
    for (int i=0; i<flmCount; ++i) {
        // Get Item
        FileListModelItem* item = itemList[i];

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
void FileListModel::insertItem(const QString& aFileName)
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
        while (itemList[i]->fileInfo.fileName() > aFileName && i < count) {
            // Inc Loop Index
            i++;
        }
    } else {
        // Go Thru List
        while (itemList[i]->fileInfo.fileName() < aFileName && i < count) {
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
        // Return File Info
        return item->fileInfo;
    }

    return QFileInfo("/");
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
// Get Busy
//==============================================================================
bool FileListModel::getBusy()
{
    return fileUtil ? (fileUtil->getStatus() == ECSTBusy) || (fileUtil->getStatus() == ECSTAborting) : false;
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

    qDebug() << "FileListModel::~FileListModel";
}

