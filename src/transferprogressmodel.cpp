#include <QFileInfo>
#include <QDebug>

#include "transferprogressmodel.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
TransferProgressModelItem::TransferProgressModelItem(const QString& aOp, const QString& aSource, const QString& aTarget)
    : op(aOp)
    , source(aSource)
    , target(aTarget)
    , fileIsDir(false)
    , fileIsLink(false)
    , fileIsArchive(false)
    , status(ETPIdle)
{
    // Init File Info
    QFileInfo fileInfo(aSource);

    // Set File Is Dir
    fileIsDir = fileInfo.isDir();
    // Set File Is Link
    fileIsLink = fileInfo.isSymLink();
    // Set File Is Archive
    fileIsArchive = isFileArchiveByExt(fileInfo.fileName());
}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressModelItem::~TransferProgressModelItem()
{
}







//==============================================================================
// Constructor
//==============================================================================
TransferProgressModel::TransferProgressModel(QObject* aParent)
    : QAbstractListModel(aParent)
{
    //qDebug() << "TransferProgressModel::TransferProgressModel";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void TransferProgressModel::init()
{
    //qDebug() << "TransferProgressModel::init";

}

//==============================================================================
// Add Item
//==============================================================================
void TransferProgressModel::addItem(const QString& aOp, const QString& aSource, const QString& aTarget)
{
    //qDebug() << "TransferProgressModel::addItem - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Create New Item
    TransferProgressModelItem* newItem = new TransferProgressModelItem(aOp, aSource, aTarget);

    // Begin Append Rows
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Append Item
    items << newItem;

    // End Insert Rows
    endInsertRows();
}

//==============================================================================
// Insert Item
//==============================================================================
void TransferProgressModel::insertItem(const int& aIndex, const QString& aOp, const QString& aSource, const QString& aTarget)
{
    // Check If Source File Exists
    if (findIndex(aSource) >= 0) {
        qDebug() << "TransferProgressModel::insertItem - aSource: " << aSource << " - SOURCE ALREADY IN QUEUE!";

        return;
    }

    //qDebug() << "TransferProgressModel::insertItem - aIndex: " << aIndex << " - aOp: " << aOp << " - aSource: " << aSource << " - aTarget: " << aTarget;

    // Create New Item
    TransferProgressModelItem* newItem = new TransferProgressModelItem(aOp, aSource, aTarget);

    // Init Insertion Index
    int insertIndex = qBound(0, aIndex, rowCount()-1);

    // Begin Append Rows
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);

    // Insert Item
    items.insert(insertIndex, newItem);

    // End Insert Rows
    endInsertRows();

}

//==============================================================================
// Remove Item
//==============================================================================
void TransferProgressModel::removeItem(const int& aIndex)
{
    // Check Index
    if (aIndex >=0 && aIndex < rowCount()) {
        //qDebug() << "TransferProgressModel::removeItem - aIndex: " << aIndex;
        // Begin REmove Rows
        beginRemoveRows(QModelIndex(), aIndex, aIndex);
        // Delete Item
        delete items.takeAt(aIndex);
        // End remove Rows
        endRemoveRows();
    }
}

//==============================================================================
// Set Progress Status
//==============================================================================
void TransferProgressModel::setProgressStatus(const int& aIndex, const TransferProgressStatus& aStatus)
{
    // Check Index
    if (aIndex >=0 && aIndex < rowCount()) {
        //qDebug() << "TransferProgressModel::setProgressStatus - aIndex: " << aIndex << " - aStatus: " << aStatus;
        // Set Data
        setData(createIndex(aIndex, ERIDStatus - Qt::UserRole - 1), aStatus, ERIDStatus);
    }
}

//==============================================================================
// Get Operation
//==============================================================================
QString TransferProgressModel::getOperation(const int& aIndex)
{
    return data(createIndex(aIndex, ERIDOp - Qt::UserRole - 1)).toString();
}

//==============================================================================
// Get Source File Name
//==============================================================================
QString TransferProgressModel::getSourceFileName(const int& aIndex)
{
    return data(createIndex(aIndex, ERIDSource - Qt::UserRole - 1)).toString();
}

//==============================================================================
// Get Target File Name
//==============================================================================
QString TransferProgressModel::getTargetFileName(const int& aIndex)
{
    return data(createIndex(aIndex, ERIDTarget - Qt::UserRole - 1)).toString();
}

//==============================================================================
// Get Progress Status
//==============================================================================
TransferProgressStatus TransferProgressModel::getProgressStatus(const int& aIndex)
{
    return (TransferProgressStatus)(data(createIndex(aIndex, ERIDStatus - Qt::UserRole - 1)).toInt());
}

//==============================================================================
// Get Source
//==============================================================================
QString TransferProgressModel::getSource(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        return items[aIndex]->source;
    }

    return "";
}

//==============================================================================
// Get Target
//==============================================================================
QString TransferProgressModel::getTarget(const int& aIndex)
{
    // Check Index
    if (aIndex >= 0 && aIndex < rowCount()) {
        return items[aIndex]->target;
    }

    return "";
}

//==============================================================================
// Find Index
//==============================================================================
int TransferProgressModel::findIndex(const QString& aSourceFileName)
{
    // Get Items Count
    int iCount = items.count();

    // Go Thru Items
    for (int i=0; i<iCount; ++i) {
        // Get Item
        TransferProgressModelItem* item = items[i];
        // Check Source File Name
        if (item && item->source == aSourceFileName) {
            return i;
        }
    }

    return -1;

}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> TransferProgressModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;

    // File Operation
    roles[ERIDOp]           = "fileOp";
    // File Source
    roles[ERIDSource]       = "fileSource";
    // File Target
    roles[ERIDTarget]       = "fileTarget";
    // Operation File Is Hidden
    roles[ERIDIsHidden]     = "fileIsHidden";
    // Operation File Is Dir
    roles[ERIDIsDir]        = "fileIsDir";
    // Operation File Is Link
    roles[ERIDIsLink]       = "fileIsLink";
    // Operation File Is Archive
    roles[ERIDIsArchive]    = "fileIsArchive";
    // Operation Status
    roles[ERIDStatus]       = "fileOpStatus";

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int TransferProgressModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return items.count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int TransferProgressModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 4;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant TransferProgressModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        TransferProgressModelItem* item = items[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case Qt::DisplayRole:
                // Switch Column
                switch (aIndex.column()) {
                    default:
                    case 0: return item->op;
                    case 1: return item->source;
                    case 2: return item->target;
                    case 3: return item->status;
                }
            break;

            case ERIDOp:        return item->op;
            case ERIDSource:    return item->source;
            case ERIDTarget:    return item->target;

            case ERIDIsHidden:  return false;
            case ERIDIsLink:    return item->fileIsLink;
            case ERIDIsDir:     return item->fileIsDir;
            case ERIDIsArchive: return item->fileIsArchive;

            case ERIDStatus:    return item->status;

            default:
            break;
        }
    }

    return QVariant();
}

//==============================================================================
// Set Data
//==============================================================================
bool TransferProgressModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    // Check Index
    if (aIndex.row() >=0 && aIndex.row() < rowCount()) {
        // Get Item
        TransferProgressModelItem* item = items[aIndex.row()];

        // Switch Role
        switch (aRole) {
            case ERIDOp:
                // Set Operation
                item->op = aValue.toString();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            case ERIDSource:
                // Set Source
                item->source = aValue.toString();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            case ERIDTarget:
                // Set Target
                item->target = aValue.toString();
                // Emit Data Changed Signal
                emit dataChanged(aIndex, aIndex);
            return true;

            case ERIDStatus:
                // Check Status
                if (item->status != (TransferProgressStatus)aValue.toInt()) {
                    // Set Status
                    item->status = (TransferProgressStatus)aValue.toInt();
                    // Emit Data Changed Signal
                    emit dataChanged(aIndex, aIndex);
                }
            return true;

            default:
            break;
        }
    }

    return false;
}

//==============================================================================
// Header Data
//==============================================================================
QVariant TransferProgressModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    // Check Orientation & Display Role
    if (aOrientation == Qt::Horizontal && aRole == Qt::DisplayRole) {
        // Switch Section
        switch (aSection) {
            case 0: return tr(DEFAULT_HEADER_TITLE_OPERATION);
            case 1: return tr(DEFAULT_HEADER_TITLE_SOURCE);
            case 2: return tr(DEFAULT_HEADER_TITLE_TARGET);
            case 3: return tr(DEFAULT_HEADER_TITLE_DONE);

            default:
            break;
        }

        return "";
    }

    return QAbstractItemModel::headerData(aSection, aOrientation, aRole);
}

//==============================================================================
// Flags
//==============================================================================
Qt::ItemFlags TransferProgressModel::flags(const QModelIndex& aIndex) const
{
    Q_UNUSED(aIndex);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


//==============================================================================
// Clear
//==============================================================================
void TransferProgressModel::clear()
{
    //qDebug() << "TransferProgressModel::clear";

    // Begin Reset Model
    beginResetModel();

    // Go Thru Items
    while (items.count() > 0) {
        // Take & Delete Last Item
        delete items.takeLast();
    }
    // Clear Items
    //items.clear();
    // End Reset Model
    endResetModel();
}

//==============================================================================
// Destructor
//==============================================================================
TransferProgressModel::~TransferProgressModel()
{
    // Clear
    clear();

    //qDebug() << "TransferProgressModel::~TransferProgressModel";
}

