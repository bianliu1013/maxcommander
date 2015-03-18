#include <QDebug>

#include <mcwinterface.h>

#include "remotefileutilclient.h"
#include "utility.h"
#include "constants.h"


//==============================================================================
// Constructor
//==============================================================================
RemoteFileUtilClient::RemoteFileUtilClient(RemoteFileUtilClientObserver* aObserver, QObject* aParent)
    : QObject(aParent)
    , cID(0)
    , client(NULL)
    , observer(aObserver)
    , reconnectAsRoot(false)
    , needReconnect(false)
{
    // Init
    init();
}

//==============================================================================
// Init
//==============================================================================
void RemoteFileUtilClient::init()
{
    // Create Client
    client = new QLocalSocket();

    // Connect Signals
    connect(client, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(client, SIGNAL(aboutToClose()), this, SLOT(socketAboutToClose()));
    connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));
    connect(client, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(socketError(QLocalSocket::LocalSocketError)));
    connect(client, SIGNAL(readChannelFinished()), this, SLOT(socketReadChannelFinished()));
    connect(client, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(client, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(socketStateChanged(QLocalSocket::LocalSocketState)));

    // ...
}

//==============================================================================
// Connect To File Server
//==============================================================================
void RemoteFileUtilClient::connectToFileServer(const bool& asRoot, const QString& aRootPass)
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

    qDebug() << "RemoteFileUtilClient::connectToFileServer";

    // Connect To Server
    client->connectToServer(asRoot ? DEFAULT_SERVER_LISTEN_ROOT_PATH : DEFAULT_SERVER_LISTEN_PATH);
}

//==============================================================================
// Is Client Connected
//==============================================================================
bool RemoteFileUtilClient::isConnected()
{
    return client ? client->state() == QLocalSocket::ConnectedState : false;
}

//==============================================================================
// Get Dir List
//==============================================================================
void RemoteFileUtilClient::getDirList(const QString& aDirPath, const int& aFilters, const int& aSortFlags)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_LIST_DIR);
    newData[DEFAULT_KEY_PATH]       = aDirPath;
    newData[DEFAULT_KEY_FILTERS]    = aFilters;
    newData[DEFAULT_KEY_OPTIONS]    = aSortFlags;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Create Directory
//==============================================================================
void RemoteFileUtilClient::createDir(const QString& aDirPath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_MAKE_DIR);
    newData[DEFAULT_KEY_PATH]       = aDirPath;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Delete File/Directory
//==============================================================================
void RemoteFileUtilClient::deleteFile(const QString& aFilePath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_DELETE_FILE);
    newData[DEFAULT_KEY_PATH]       = aFilePath;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Scan Directory Size
//==============================================================================
void RemoteFileUtilClient::scanDirSize(const QString& aDirPath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_SCAN_DIR);
    newData[DEFAULT_KEY_PATH]       = aDirPath;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Scan Directory Tree
//==============================================================================
void RemoteFileUtilClient::scanDirTree(const QString& aDirPath)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_TREE_DIR);
    newData[DEFAULT_KEY_PATH]       = aDirPath;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Copy File
//==============================================================================
void RemoteFileUtilClient::copyFile(const QString& aSource, const QString& aTarget)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_COPY_FILE);
    newData[DEFAULT_KEY_SOURCE]     = aSource;
    newData[DEFAULT_KEY_TARGET]     = aTarget;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Rename/Move File
//==============================================================================
void RemoteFileUtilClient::moveFile(const QString& aSource, const QString& aTarget)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_MOVE_FILE);
    newData[DEFAULT_KEY_SOURCE]     = aSource;
    newData[DEFAULT_KEY_TARGET]     = aTarget;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Search File
//==============================================================================
void RemoteFileUtilClient::searchFile(const QString& aName, const QString& aDirPath, const QString& aContent, const int& aOptions)
{
    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_SEARCH_FILE);
    newData[DEFAULT_KEY_FILENAME]   = aName;
    newData[DEFAULT_KEY_PATH]       = aDirPath;
    newData[DEFAULT_KEY_CONTENT]    = aContent;
    newData[DEFAULT_KEY_OPTIONS]    = aOptions;

    // ...

    // Write Data
    wirteData(newData);
}

//==============================================================================
// Abort Current Operation
//==============================================================================
void RemoteFileUtilClient::abort()
{
    // Check Client
    if (client && cID > 0) {
        qDebug() << "RemoteFileUtilClient::abort - cID: " << cID;

        // Init New Data
        QVariantMap newData;

        // Set Up New Data
        newData[DEFAULT_KEY_CID]        = cID;
        newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_ABORT);

        // Write Data
        wirteData(newData);

        // ...

        // Abort
        //client->abort();
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

        // Disconnect From Server
        client->disconnectFromServer();
        // Close Client
        client->close();
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
// Launch Server Test
//==============================================================================
void RemoteFileUtilClient::launchServerTest(const bool& asRoot, const QString& aRootPass)
{
    qDebug() << "RemoteFileUtilClient::launchServerTest - cID: " << cID;

    // Connect To File Server
    connectToFileServer(asRoot, aRootPass);
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
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_TEST);

    // Write Data
    wirteData(newData);

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
// Send Test
//==============================================================================
void RemoteFileUtilClient::sendTestResponse(const int& aResponse)
{
    // Check If Connected
    if (!isConnected()) {
        qDebug() << "RemoteFileUtilClient::sendTestResponse - cID: " << cID << " - CLIENT NOT CONNECTED!!";
        return;
    }

    qDebug() << "RemoteFileUtilClient::sendTestResponse - cID: " << cID;

    // Init New Data
    QVariantMap newData;

    // Set Up New Data
    newData[DEFAULT_KEY_CID]        = cID;
    newData[DEFAULT_KEY_OPERATION]  = QString(DEFAULT_REQUEST_RESP);
    newData[DEFAULT_KEY_RESPONSE]   = aResponse;

    // Write Data
    wirteData(newData);

    // ...
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
        qDebug() << "RemoteFileUtilClient::writeData - cID: " << cID << " - length: " << aData.length();
        // Write Data
        client->write(aData);
    }
}

//==============================================================================
// Write Data
//==============================================================================
void RemoteFileUtilClient::wirteData(const QVariantMap& aData)
{
    // Check Data
    if (!aData.isEmpty() && aData.count() > 0) {
        qDebug() << "RemoteFileUtilClient::writeData - cID: " << cID << " - aData[clientid]: " << aData[DEFAULT_KEY_CID].toInt();

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
// Socket Connected Slot
//==============================================================================
void RemoteFileUtilClient::socketConnected()
{
    qDebug() << "RemoteFileUtilClient::socketConnected - cID: " << cID;

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
void RemoteFileUtilClient::socketError(QLocalSocket::LocalSocketError socketError)
{
    qDebug() << "RemoteFileUtilClient::socketError - cID: " << cID << " - socketError: " << socketError << " - error: " << client->errorString();

    // ...
}

//==============================================================================
// Socket State Changed Slot
//==============================================================================
void RemoteFileUtilClient::socketStateChanged(QLocalSocket::LocalSocketState socketState)
{
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
    //qDebug() << "RemoteFileUtilClient::socketBytesWritten - cID: " << cID << " - bytes: " << bytes;

    // ...
}

//==============================================================================
// Socket Read Channel Finished Slot
//==============================================================================
void RemoteFileUtilClient::socketReadChannelFinished()
{
    qDebug() << "RemoteFileUtilClient::socketReadChannelFinished - cID: " << cID;

}

//==============================================================================
// Socket Ready Read Slot
//==============================================================================
void RemoteFileUtilClient::socketReadyRead()
{
    //qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - bytesAvailable: " << client->bytesAvailable();

    // Read Data
    lastBuffer = client->readAll();

    //qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - lastBuffer: " << lastBuffer;

    // Check ID
    if (cID == 0) {
        // Init Result
        bool ok = false;
        // Set ID
        cID = QString(lastBuffer).toULongLong(&ok);

        qDebug() << "RemoteFileUtilClient::socketReadyRead - cID: " << cID << " - Client ID is SET!!";

        // Emit Client Connection Changed Signal
        emit clientConnectionChanged(cID, true);

    }

    // ...

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
        // Delete Socket
        delete client;
        client = NULL;
    }

    qDebug() << "RemoteFileUtilClient::~RemoteFileUtilClient - cID: " << cID;
}

