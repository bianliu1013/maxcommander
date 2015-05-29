
#include <QHostAddress>
#include <QMutexLocker>
#include <QDebug>

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <mcwinterface.h>

#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
RemoteFileUtilClient::RemoteFileUtilClient(QObject* aParent)
    : QObject(aParent)
    , cID(0)
    , status(ECSTCreated)
    , client(NULL)
    , reconnectAsRoot(false)
    , needReconnect(false)
{
    qDebug() << "RemoteFileUtilClient::RemoteFileUtilClient";

    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void RemoteFileUtilClient::init()
{
    qDebug() << "RemoteFileUtilClient::init";

    // Create Client
    client = new QTcpSocket();

    // Connect Signals
    connect(client, SIGNAL(hostFound()), this, SLOT(socketHostFound()));
    connect(client, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(client, SIGNAL(aboutToClose()), this, SLOT(socketAboutToClose()));
    connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(client, SIGNAL(readChannelFinished()), this, SLOT(socketReadChannelFinished()));
    connect(client, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    // Set Buffer Size
    //client->setReadBufferSize(8192);

    // Init Frame Pattern
    framePattern.append(DEFAULT_DATA_FRAME_PATTERN_CHAR_1);
    framePattern.append(DEFAULT_DATA_FRAME_PATTERN_CHAR_2);
    framePattern.append(DEFAULT_DATA_FRAME_PATTERN_CHAR_3);
    framePattern.append(DEFAULT_DATA_FRAME_PATTERN_CHAR_4);

    // ...
}

//==============================================================================
// Connect To File Server
//==============================================================================
void RemoteFileUtilClient::connectToFileServer(const QString& aHost, const bool& asRoot, const QString& aRootPass)
{
    // Check Client
    if (!client) {
        qWarning() << "RemoteFileUtilClient::connectToFileServer - NO CLIENT SOCKET!!";

        return;
    }

    // Check If Server Runnind
    if (!checkFileServerRunning()) {
        qDebug() << "RemoteFileUtilClient::connectToFileServer - File Server Not Running! Starting...";

        // Launch REmote File Server
        startFileServer(asRoot, aRootPass);
    }

    // Check Client Status
    if (client->state() == QAbstractSocket::ConnectingState || client->state() == QAbstractSocket::ConnectedState) {
        return;
    }

    qDebug() << "RemoteFileUtilClient::connectToFileServer - host: " << (aHost.isEmpty() ? DEFAULT_WORKER_HOST_NAME : aHost) << " - port:" << (asRoot ? DEFAULT_FILE_SERVER_ROOT_HOST_PORT : DEFAULT_FILE_SERVER_HOST_PORT);

    // Connect To Host
    client->connectToHost(DEFAULT_WORKER_HOST_NAME, asRoot ? DEFAULT_FILE_SERVER_ROOT_HOST_PORT : DEFAULT_FILE_SERVER_HOST_PORT);
}

//==============================================================================
// Is Client Connected
//==============================================================================
bool RemoteFileUtilClient::isConnected()
{
    return client ? client->state() == QAbstractSocket::ConnectedState : false;
}

//==============================================================================
// Get ID
//==============================================================================
unsigned int RemoteFileUtilClient::getID()
{
    return client && client->state() == QAbstractSocket::ConnectedState ? cID : 0;
}

//==============================================================================
// Get Status
//==============================================================================
ClientStatusType RemoteFileUtilClient::getStatus()
{
    return status;
}

//==============================================================================
// Get Last Operation
//==============================================================================
QString RemoteFileUtilClient::lastOperation()
{
    return lastDataMap[DEFAULT_KEY_OPERATION].toString();
}

//==============================================================================
// Get Dir List
//==============================================================================
void RemoteFileUtilClient::getDirList(const QString& aDirPath, const int& aFilters, const int& aSortFlags)
{
    // Check If Connected
    if (!isConnected()) {
        qDebug() << "RemoteFileUtilClient::getDirList - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    // Check Status
    if (status == ECSTBusy) {

        qDebug() << "#########################################################################";
        qDebug() << "#### RemoteFileUtilClient::getDirList - cID: " << cID << " - BUSY!!";
        qDebug() << "#########################################################################";

        // ...
    }

    //qDebug() << "RemoteFileUtilClient::getDirList - aDirPath: " << aDirPath << " - aFilters: " << aFilters << " - aSortFlags: " << aSortFlags;

    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_LIST_DIR);
    newData[DEFAULT_KEY_PATH]           = aDirPath;
    newData[DEFAULT_KEY_FILTERS]        = aFilters;
    newData[DEFAULT_KEY_FLAGS]          = aSortFlags;

    // ...

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);
}

//==============================================================================
// Create Directory
//==============================================================================
void RemoteFileUtilClient::createDir(const QString& aDirPath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_MAKE_DIR);
    newData[DEFAULT_KEY_PATH]           = aDirPath;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Create Link
//==============================================================================
void RemoteFileUtilClient::createLink(const QString& aLinkPath, const QString& aLinkTarget)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_MAKE_LINK);
    newData[DEFAULT_KEY_SOURCE]         = aLinkPath;
    newData[DEFAULT_KEY_TARGET]         = aLinkTarget;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Delete File/Directory
//==============================================================================
void RemoteFileUtilClient::deleteFile(const QString& aFilePath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_DELETE_FILE);
    newData[DEFAULT_KEY_PATH]           = aFilePath;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Scan Directory Size
//==============================================================================
void RemoteFileUtilClient::scanDirSize(const QString& aDirPath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_SCAN_DIR);
    newData[DEFAULT_KEY_PATH]           = aDirPath;

    // ...

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);
}

//==============================================================================
// Scan Directory Tree
//==============================================================================
void RemoteFileUtilClient::scanDirTree(const QString& aDirPath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_TREE_DIR);
    newData[DEFAULT_KEY_PATH]           = aDirPath;

    // ...

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);
}

//==============================================================================
// Copy File
//==============================================================================
void RemoteFileUtilClient::copyFile(const QString& aSource, const QString& aTarget)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_COPY_FILE);
    newData[DEFAULT_KEY_SOURCE]         = aSource;
    newData[DEFAULT_KEY_TARGET]         = aTarget;

    // ...

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);
}

//==============================================================================
// Rename/Move File
//==============================================================================
void RemoteFileUtilClient::moveFile(const QString& aSource, const QString& aTarget)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_MOVE_FILE);
    newData[DEFAULT_KEY_SOURCE]         = aSource;
    newData[DEFAULT_KEY_TARGET]         = aTarget;

    // ...

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);
}

//==============================================================================
// Set File Attributes
//==============================================================================
void RemoteFileUtilClient::setFileAttributes(const QString& aFilePath, const int& aAttrib)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_ATTRIBUTES);
    newData[DEFAULT_KEY_PATH]           = aFilePath;
    newData[DEFAULT_KEY_ATTRIB]         = aAttrib;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Set File Owner
//==============================================================================
void RemoteFileUtilClient::setFileOwner(const QString& aFilePath, const QString& aOwner)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_OWNER);
    newData[DEFAULT_KEY_PATH]           = aFilePath;
    newData[DEFAULT_KEY_OWNER]          = aOwner;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Set File Permissions
//==============================================================================
void RemoteFileUtilClient::setFilePermissions(const QString& aFilePath, const int& aPermissions)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_PERMISSIONS);
    newData[DEFAULT_KEY_PATH]           = aFilePath;
    newData[DEFAULT_KEY_PERMISSIONS]    = aPermissions;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Set File Date Time
//==============================================================================
void RemoteFileUtilClient::setFileDateTime(const QString& aFilePath, const QDateTime& aDateTime)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_DATETIME);
    newData[DEFAULT_KEY_PATH]           = aFilePath;
    newData[DEFAULT_KEY_DATETIME]       = aDateTime;

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Search File
//==============================================================================
void RemoteFileUtilClient::searchFile(const QString& aName, const QString& aDirPath, const QString& aContent, const int& aOptions)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_SEARCH_FILE);
    newData[DEFAULT_KEY_FILENAME]       = aName;
    newData[DEFAULT_KEY_PATH]           = aDirPath;
    newData[DEFAULT_KEY_CONTENT]        = aContent;
    newData[DEFAULT_KEY_OPTIONS]        = aOptions;

    // ...

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);
}

//==============================================================================
// Clear Global File Transfer Options
//==============================================================================
void RemoteFileUtilClient::clearFileTransferOptions()
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_CLEAR);

    // ...

    // Write Data
    writeData(newData);
}

//==============================================================================
// Abort Current Operation
//==============================================================================
void RemoteFileUtilClient::abort()
{
    // Check Client & Status
    if (client && cID > 0 && (status == ECSTBusy || status == ECSTWaiting || status == ECSTSuspended)) {
        // Set Status
        setStatus(ECSTAborting);

        qDebug() << "RemoteFileUtilClient::abort - cID: " << cID;

        // Init New Data
        QVariantMap newData;

        // Set Up New Data
        newData[DEFAULT_KEY_CID]        = cID;
        newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_OPERATION_ABORT);

        // Write Data
        writeData(newData);

        // ...

    } else {
        //qDebug() << "RemoteFileUtilClient::abort - cID: " << cID << " - NOT BUSY, NOTHING TO ABORT!";
    }
}

//==============================================================================
// Suspend Current Operation
//==============================================================================
void RemoteFileUtilClient::suspend()
{
    // Check Client & Status
    if (client && cID > 0 && status == ECSTBusy) {
        // Set Status
        setStatus(ECSTSuspended);

        qDebug() << "RemoteFileUtilClient::suspend - cID: " << cID;

        // Init New Data
        QVariantMap newData;

        // Set Up New Data
        newData[DEFAULT_KEY_CID]        = cID;
        newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_OPERATION_PAUSE);

        // Write Data
        writeData(newData);

        // ...

    } else {
        //qDebug() << "RemoteFileUtilClient::suspend - cID: " << cID << " - NOT BUSY, NOTHING TO SUSPEND!";
    }
}

//==============================================================================
// Resume Current Operation
//==============================================================================
void RemoteFileUtilClient::resume()
{
    // Check Client & Status
    if (client && cID > 0 && status == ECSTSuspended) {
        // Set Status
        setStatus(ECSTBusy);

        qDebug() << "RemoteFileUtilClient::resume - cID: " << cID;

        // Init New Data
        QVariantMap newData;

        // Set Up New Data
        newData[DEFAULT_KEY_CID]        = cID;
        newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_OPERATION_RESUME);

        // Write Data
        writeData(newData);

        // ...

    } else {
        //qDebug() << "RemoteFileUtilClient::resume - cID: " << cID << " - NOT SUSPENDED, CANT TO RESUME!";
    }
}

//==============================================================================
// Close
//==============================================================================
void RemoteFileUtilClient::close()
{
    // Abort
    abort();

    // Check Client
    if (client && cID > 0) {
        qDebug() << "RemoteFileUtilClient::close - cID: " << cID;

        // Disconnect From Host
        client->disconnectFromHost();
        // Close Client
        client->close();

        // Reset ID
        cID = 0;
    }
}

//==============================================================================
// Execute Shell Command
//==============================================================================
void RemoteFileUtilClient::executeShellCommand(const QString& aCommand, const bool& asRoot, const QString& aRootPass)
{
    qDebug() << "RemoteFileUtilClient::executeShellCommand - cID: " << cID << " - cmd: " << aCommand << " - asRoot: " << asRoot;

    // Exec Shell Command
    execShellCommand(aCommand, asRoot, aRootPass);
}

//==============================================================================
// Send Test
//==============================================================================
void RemoteFileUtilClient::sendUserResponse(const int& aResponse, const QString& aNewPath)
{
    // Check If Connected
    if (!isConnected()) {
        qDebug() << "RemoteFileUtilClient::sendUserResponse - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    qDebug() << "RemoteFileUtilClient::sendUserResponse - cID: " << cID << " - aResponse: " << aResponse;

    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]            = cID;
    newData[DEFAULT_KEY_OPERATION]      = QString(DEFAULT_OPERATION_USER_RESP);
    newData[DEFAULT_KEY_PATH]           = aNewPath;
    newData[DEFAULT_KEY_RESPONSE]       = aResponse;

    // Write Data
    writeData(newData);
}

//==============================================================================
// Launch Server Test
//==============================================================================
void RemoteFileUtilClient::launchServerTest(const QString& aHost, const bool& asRoot, const QString& aRootPass)
{
    qDebug() << "RemoteFileUtilClient::launchServerTest - cID: " << cID;

    // Connect To File Server
    connectToFileServer(aHost, asRoot, aRootPass);
}

//==============================================================================
// Start Test Operation
//==============================================================================
void RemoteFileUtilClient::startTestOperation()
{
    // Check If Connected
    if (!isConnected()) {
        qDebug() << "RemoteFileUtilClient::startTestOperation - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    qDebug() << "RemoteFileUtilClient::startTestOperation - cID: " << cID;

    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_OPERATION_TEST);

    // Write Data
    writeData(newData);

    // Set Status
    setStatus(ECSTBusy);

    // ...
}

//==============================================================================
// Stop/Abort Test Operation
//==============================================================================
void RemoteFileUtilClient::stopTestOperation()
{
    // Check If Connected
    if (!isConnected()) {
        qDebug() << "RemoteFileUtilClient::stopTestOperation - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    qDebug() << "RemoteFileUtilClient::stopTestOperation - cID: " << cID;

    // Abort
    abort();
}

//==============================================================================
// Disconnect Test
//==============================================================================
void RemoteFileUtilClient::disconnectTest()
{
    // Check If Connected
    if (!isConnected()) {
        qDebug() << "RemoteFileUtilClient::disconnectTest - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    qDebug() << "RemoteFileUtilClient::disconnectTest - cID: " << cID;

    // Close
    close();
}

//==============================================================================
// Shut Down
//==============================================================================
void RemoteFileUtilClient::shutDown()
{
    // Close
    close();

    qDebug() << "RemoteFileUtilClient::shutDown - cID: " << cID;
}

//==============================================================================
// Set Status
//==============================================================================
void RemoteFileUtilClient::setStatus(const ClientStatusType& aStatus)
{
    // Check Status
    if (status != aStatus) {
        //qDebug() << "RemoteFileUtilClient::setStatus - cID: " << cID << " - aStatus: " << statusToString(aStatus);

        // Set Status
        status = aStatus;

        // ...

        // Emit Client Status Changed
        emit clientStatusChanged(cID, status);
    }
}

//==============================================================================
// Check If File Server Running
//==============================================================================
bool RemoteFileUtilClient::checkFileServerRunning()
{
    return checkRemoteFileServerRunning();
}

//==============================================================================
// Start File Server
//==============================================================================
void RemoteFileUtilClient::startFileServer(const bool& asRoot, const QString& aRootPass)
{
    qDebug() << "RemoteFileUtilClient::startFileServer - cID: " << cID << " - asRoot: " << asRoot;

    // Launch Remote File Sever
    if (launchRemoteFileServer(asRoot, aRootPass) != 0) {
        qDebug() << "RemoteFileUtilClient::startFileServer - cID: " << cID << " - ERROR LAUNCHING FILE SERVER!!";
    }
}

//==============================================================================
// Write Data
//==============================================================================
void RemoteFileUtilClient::writeData(const QByteArray& aData)
{
    // Check Client
    if (!client) {
        qWarning() << "RemoteFileUtilClient::writeData - cID: " << cID << " - NO CLIENT!!";
        return;
    }

    // Check If Client Is Connected
    if (!isConnected()) {
        qWarning() << "RemoteFileUtilClient::writeData - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    // Check Data
    if (!aData.isNull() && !aData.isEmpty()) {
        //qDebug() << "RemoteFileUtilClient::writeData - cID: " << cID << " - length: " << aData.length();
        // Write Data
        client->write(aData);
        // Flush
        client->flush();
    }
}

//==============================================================================
// Write Data
//==============================================================================
void RemoteFileUtilClient::writeData(const QVariantMap& aData)
{
    // Check Data
    if (!aData.isEmpty() && aData.count() > 0) {
        //qDebug() << "RemoteFileUtilClient::writeData - cID: " << cID << " - aData[clientid]: " << aData[DEFAULT_KEY_CID].toInt();

        // Init New Byte Array
        QByteArray newByteArray;

        // Init New Data Stream
        QDataStream newDataStream(&newByteArray, QIODevice::ReadWrite);

        // Add Variant Map To Data Stream
        newDataStream << aData;

        // Write Data
        writeData(newByteArray);
    }
}

//==============================================================================
// Socket Host Found
//==============================================================================
void RemoteFileUtilClient::socketHostFound()
{
    //qDebug() << "RemoteFileUtilClient::socketHostFound";

    // ...
}

//==============================================================================
// Socket Connected Slot
//==============================================================================
void RemoteFileUtilClient::socketConnected()
{
    //qDebug() << "RemoteFileUtilClient::socketConnected";

    // ...

    // Emit Client Connection Changed Signal
    //emit clientConnectionChanged(cID, true);
}

//==============================================================================
// Socket Disconnected Slot
//==============================================================================
void RemoteFileUtilClient::socketDisconnected()
{
    qDebug() << "RemoteFileUtilClient::socketDisconnected - cID: " << cID << " - Resetting Client ID!";

    // Emit Client Connection Changed Signal
    emit clientConnectionChanged(cID, false);

    // Reset Client ID
    cID = 0;

    // ...
}

//==============================================================================
// Socket Error Slot
//==============================================================================
void RemoteFileUtilClient::socketError(QAbstractSocket::SocketError socketError)
{
    qWarning() << " ";
    qWarning() << "#### RemoteFileUtilClient::socketError - cID: " << cID << " - socketError: " << socketError << " - error: " << client->errorString();
    qWarning() << " ";

    // ...
}

//==============================================================================
// Socket State Changed Slot
//==============================================================================
void RemoteFileUtilClient::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    Q_UNUSED(socketState);

    //qDebug() << "RemoteFileUtilClient::socketStateChanged - cID: " << cID << " - socketState: " << socketState;

    // ...
}

//==============================================================================
// Socket About To Close Slot
//==============================================================================
void RemoteFileUtilClient::socketAboutToClose()
{
    //qDebug() << "RemoteFileUtilClient::socketAboutToClose - cID: " << cID;

    // ...
}

//==============================================================================
// Socket Bytes Written Slot
//==============================================================================
void RemoteFileUtilClient::socketBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);

    //qDebug() << "RemoteFileUtilClient::socketBytesWritten - cID: " << cID << " - bytes: " << bytes;

    // ...
}

//==============================================================================
// Socket Read Channel Finished Slot
//==============================================================================
void RemoteFileUtilClient::socketReadChannelFinished()
{
    //qDebug() << "RemoteFileUtilClient::socketReadChannelFinished - cID: " << cID;

}

//==============================================================================
// Socket Ready Read Slot
//==============================================================================
void RemoteFileUtilClient::socketReadyRead()
{
    //qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - bytesAvailable: " << client->bytesAvailable();

    // Read Data
    lastBuffer = client ? client->readAll() : QByteArray();

    //qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - lastBuffer.size: " << lastBuffer.size();

    // Parse Last Buffer
    parseLastBuffer();
}

//==============================================================================
// Send Acknowledge
//==============================================================================
void RemoteFileUtilClient::sendAcknowledge()
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_OPERATION_ACKNOWLEDGE);

    // Write Data
    writeData(newData);

    // ...
}

//==============================================================================
// Parse Last Buffer
//==============================================================================
void RemoteFileUtilClient::parseLastBuffer()
{
    // Mutex Locker
    //QMutexLocker locker(&mutex);

    // Check ID
    if (cID == 0) {
        // Set ID
        cID = QString(lastBuffer).toULongLong();

        //qDebug() << "RemoteFileUtilClient::parseLastBuffer - cID: " << cID << " - Client ID is SET!!";

        // Set Status
        setStatus(ECSTIdle);

        // Emit Client Connection Changed Signal
        emit clientConnectionChanged(cID, true);

    } else {

        // Init Buffer Pos
        int bPos = 0;
        // Init Frame Pattern Pos
        int fpPos = 0;
        // Get Last Buffer Size
        int bSize = lastBuffer.size();

        //qDebug() << "RemoteFileUtilClient::parseLastBuffer - cID: " << cID << " - lastBuffer.size: " << lastBuffer.size();

        // Go Thru Last Buffer - Buffer Pos
        while (bPos < bSize) {
            // Get Frame Pattern Pos
            fpPos = lastBuffer.indexOf(framePattern, bPos);

            // Check Frame Pattern Pos
            if (fpPos >= 0) {
                // Update Buffer Pos
                bPos = fpPos + framePattern.size();
            }

            // Get Next Frame Pattern Pos
            int nextFPPos = lastBuffer.indexOf(framePattern, bPos);
            // Check Next Frame Pattern Pos
            if (nextFPPos < 0) {
                // Adjust Next Frame Pattern Post
                nextFPPos = bSize;
            }

            // Init New Data Stream
            QDataStream newDataStream(lastBuffer.mid(bPos, nextFPPos - bPos));

            // Clear Last Variant Map
            lastDataMap.clear();

            // Red Data Stream To Data Map
            newDataStream >> lastDataMap;

            //qDebug() << lastDataMap;

            // Get Next Buffer Pos
            bPos = nextFPPos + framePattern.size();

            // Parse Last Data Map
            parseLastDataMap();
        }
    }
}

//==============================================================================
// Parse Last data Map
//==============================================================================
void RemoteFileUtilClient::parseLastDataMap()
{
    //qDebug() << ">>>> RemoteFileUtilClient::parseLastDataMap";

    // Get Last Data Map Client ID
    unsigned int rcID = lastDataMap[DEFAULT_KEY_CID].toInt();

    // Check Client ID
    if (cID != rcID) {
        qWarning() << "RemoteFileUtilClient::parseLastDataMap - cID: " << cID << " - INVALID CLIENT ID: " << rcID;
        //return;
        goto finished;
    }

    // Check Error
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_ERROR)) {
        // Handle Error
        handleError();
        //return;
        goto finished;
    }

    // Check Aborted
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_ABORT)) {
        // Handle Abort
        handleAbort();
        //return;
        goto finished;
    }

    // Check If Aborting
    if (status == ECSTAborting || status == ECSTAborted) {
        qDebug() << "#### RemoteFileUtilClient::parseLastDataMap - cID: " << cID << " - ABORTING!!";
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_TEST)) {
        // Handle Test
        handleTest();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_START)) {
        // Handle Started
        handleStarted();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_DIRITEM)) {
        // Handle Dir List Item
        handleDirListItem();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_QUEUE)) {
        // Handle Queue Item
        handleQueueItem();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_CONFIRM)) {
        // Handle Confirm
        handleConfirm();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_SKIP)) {
        // Handle Skipped
        handleSkipped();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_READY)) {
        // Handle Finished
        handleFinished();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_PROGRESS)) {
        // Handle Progress
        handleProgress();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_DIRSCAN)) {
        // Handle Dir Size Scan
        handleDirSizeUpdate();
        //return;
        goto finished;
    }

    // Check Response
    if (lastDataMap[DEFAULT_KEY_RESPONSE].toString() == QString(DEFAULT_RESPONSE_SEARCH)) {
        // Handle Search Item Found
        handleSearchItemFound();

        goto finished;
    }

    qDebug() << "RemoteFileUtilClient::parseLastDataMap - WTF?!??";

finished:

    //qDebug() << "<<<< RemoteFileUtilClient::parseLastDataMap";

    return;
}

//==============================================================================
// Handle Test
//==============================================================================
void RemoteFileUtilClient::handleTest()
{
    qDebug() << "RemoteFileUtilClient::handleTest - custom data: " << lastDataMap[DEFAULT_KEY_CUSTOM].toString();

    // ...

//    std::cout << ".";
//    fflush(stdout);

    // ...
}

//==============================================================================
// Handle Started
//==============================================================================
void RemoteFileUtilClient::handleStarted()
{
    // Set Status
    setStatus(ECSTBusy);

    // Emit File Operation Started Signal
    emit fileOpStarted(cID,
                       lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                       lastDataMap[DEFAULT_KEY_PATH].toString(),
                       lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                       lastDataMap[DEFAULT_KEY_TARGET].toString());
}

//==============================================================================
// Handle Preogress
//==============================================================================
void RemoteFileUtilClient::handleProgress()
{
    // Set Status
    setStatus(ECSTBusy);

    // Emit File Operation Progress Signal
    emit fileOpProgress(cID,
                        lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                        lastDataMap[DEFAULT_KEY_PATH].toString(),
                        lastDataMap[DEFAULT_KEY_CURRPROGRESS].toULongLong(),
                        lastDataMap[DEFAULT_KEY_CURRTOTAL].toULongLong());

    // Send Acknowledge
    //sendAcknowledge();
}

//==============================================================================
// Handle Confirm
//==============================================================================
void RemoteFileUtilClient::handleConfirm()
{
    // Set Status
    setStatus(ECSTWaiting);

    // Emit File Operation Need Confirm Signal
    emit fileOpNeedConfirm(cID,
                           lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                           lastDataMap[DEFAULT_KEY_CONFIRMCODE].toInt(),
                           lastDataMap[DEFAULT_KEY_PATH].toString(),
                           lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                           lastDataMap[DEFAULT_KEY_TARGET].toString());
}

//==============================================================================
// Handle Finished
//==============================================================================
void RemoteFileUtilClient::handleFinished()
{
    // Set Status
    setStatus(ECSTIdle);

    // Emit File Operation Finished Signal
    emit fileOpFinished(cID,
                        lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                        lastDataMap[DEFAULT_KEY_PATH].toString(),
                        lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                        lastDataMap[DEFAULT_KEY_TARGET].toString());
}

//==============================================================================
// Handle Abort
//==============================================================================
void RemoteFileUtilClient::handleAbort()
{
    // Set Status
    setStatus(ECSTAborted);

    // Emit File Operation Aborted Signal
    emit fileOpAborted(cID,
                       lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                       lastDataMap[DEFAULT_KEY_PATH].toString(),
                       lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                       lastDataMap[DEFAULT_KEY_TARGET].toString());
}

//==============================================================================
// Handle Skipped
//==============================================================================
void RemoteFileUtilClient::handleSkipped()
{
    // Set Status
    setStatus(ECSTIdle);

    // Emit File Operation Skipped Signal
    emit fileOpSkipped(cID,
                       lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                       lastDataMap[DEFAULT_KEY_PATH].toString(),
                       lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                       lastDataMap[DEFAULT_KEY_TARGET].toString());
}

//==============================================================================
// Handle Error
//==============================================================================
void RemoteFileUtilClient::handleError()
{
    // Set Status
    setStatus(ECSTError);

    // Emit Error Signal
    emit fileOpError(cID,
                     lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                     lastDataMap[DEFAULT_KEY_PATH].toString(),
                     lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                     lastDataMap[DEFAULT_KEY_TARGET].toString(),
                     lastDataMap[DEFAULT_KEY_ERROR].toInt());
}

//==============================================================================
// Handle Dir List Item
//==============================================================================
void RemoteFileUtilClient::handleDirListItem()
{
    // Emit Dir List Item Found Signal
    emit dirListItemFound(cID,
                          lastDataMap[DEFAULT_KEY_PATH].toString(),
                          lastDataMap[DEFAULT_KEY_FILENAME].toString());

    // Send Acknowledge
    //sendAcknowledge();
}

//==============================================================================
// Handle Queue Item
//==============================================================================
void RemoteFileUtilClient::handleQueueItem()
{
    // Emit File Operation Queue Item Found Signal
    emit fileOpQueueItemFound(cID,
                              lastDataMap[DEFAULT_KEY_OPERATION].toString(),
                              lastDataMap[DEFAULT_KEY_PATH].toString(),
                              lastDataMap[DEFAULT_KEY_SOURCE].toString(),
                              lastDataMap[DEFAULT_KEY_TARGET].toString());

    // Send Acknowledge
    sendAcknowledge();
}

//==============================================================================
// Handle Dir Size Update
//==============================================================================
void RemoteFileUtilClient::handleDirSizeUpdate()
{
    // Emit Dir Size Scan Progress Signal
    emit dirSizeScanProgress(cID,
                             lastDataMap[DEFAULT_KEY_PATH].toString(),
                             lastDataMap[DEFAULT_KEY_NUMFILES].toULongLong(),
                             lastDataMap[DEFAULT_KEY_NUMDIRS].toULongLong(),
                             lastDataMap[DEFAULT_KEY_DIRSIZE].toULongLong());

    // Send Acknowledge
    //sendAcknowledge();
}

//==============================================================================
// Handle File Search Item Found
//==============================================================================
void RemoteFileUtilClient::handleSearchItemFound()
{
    // Emit Search REsult Item Found Signal
    emit fileSearchResultItemFound(cID,
                                   lastDataMap[DEFAULT_KEY_PATH].toString(),
                                   lastDataMap[DEFAULT_KEY_FILENAME].toString());

    // Send Acknowledge
    sendAcknowledge();
}

//==============================================================================
// Status To String
//==============================================================================
QString RemoteFileUtilClient::statusToString(const int& aStatus)
{
    switch (aStatus) {
        case ECSTCreated:   return "ECSTCreated";
        case ECSTIdle:      return "ECSTIdle";
        case ECSTBusy:      return "ECSTBusy";
        case ECSTSuspended: return "ECSTSuspended";
        case ECSTAborting:  return "ECSTAborting";
        case ECSTAborted:   return "ECSTAborted";
        case ECSTWaiting:   return "ECSTWaiting";
        case ECSTError:     return "ECSTError";

        default:
        break;
    }

    return "";
}

//==============================================================================
// Destructor
//==============================================================================
RemoteFileUtilClient::~RemoteFileUtilClient()
{
    // Shut Down
    shutDown();

    // Check Client
    if (client) {
        // Close
        client->close();

        // Delete Socket
        delete client;
        client = NULL;
    }

    qDebug() << "RemoteFileUtilClient::~RemoteFileUtilClient - cID: " << cID;
}

