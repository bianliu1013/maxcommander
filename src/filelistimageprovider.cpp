#include <QImage>
#include <QImageReader>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>

#include "filelistimageprovider.h"
#include "settingscontroller.h"
#include "utility.h"
#include "defaultsettings.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
FileListImageProvider::FileListImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
    , iconWidth(DEFAULT_ICON_WIDTH_32)
    , iconHeight(DEFAULT_ICON_HEIGHT_32)
    , settings(SettingsController::getInstance())
    , useDefaultIcons(DEAFULT_SETTINGS_USE_DEFAULT_ICONS)
    , settingsReciever(new SettingsReciever(this))
{
    // Get Supported Image Formats
    supportedFormats = QImageReader::supportedImageFormats();

    // Connect Signals
    QObject::connect(settings, SIGNAL(useDefaultIconsChanged(bool)), settingsReciever, SLOT(useDefaultIconsChanged(bool)));

    // ...
}

//==============================================================================
// Request Image
//==============================================================================
QImage FileListImageProvider::requestImage(const QString& aID, QSize* aSize, const QSize& aRequestedSize)
{
    Q_UNUSED(aID);
    Q_UNUSED(aSize);
    Q_UNUSED(aRequestedSize);

    //qDebug() << "FileListImageProvider::requestImage - aID: " << aID << " - aRequestedSize: " << aRequestedSize;

    // Init File Name
    QString fileName = aID;

#if defined(Q_OS_MACX)

    // Check ID - Mac OSX HACK!!! X ((
    if (aID == QString("//dev")) {
        // Update File Name
        fileName = "/etc";
    }

    // Check ID
    if (aID.startsWith("/dev")) {
        // Update File Name
        fileName = "/.file";
    }

#endif // Q_OS_MACX

    // Check Use Default Icons
    if (useDefaultIcons) {
        // Init File Info
        QFileInfo fileInfo(fileName);

        // Check If Is Dir
        if (fileInfo.isDir()) {
            return QImage(DEFAULT_FILE_ICON_DIR);
        }

        return QImage(DEFAULT_FILE_ICON_FILE);
    }

    // Get File Icon Image
    QImage image = getFileIconImage(fileName, iconWidth, iconHeight);

    // Check Image
    if (!image.isNull()) {
        return image;
    }

    return QImage(DEFAULT_FILE_ICON_FILE);
}

//==============================================================================
// Destructor
//==============================================================================
FileListImageProvider::~FileListImageProvider()
{
    // Check Settings Reciever
    if (settingsReciever) {
        // Delete Settings Reciever
        delete settingsReciever;
        // Reset Settings Reciever
        settingsReciever = NULL;
    }

    // Check Settings
    if (settings) {
        // Release
        settings->release();
        // Reset Settings
        settings = NULL;
    }
}











//==============================================================================
// Constructor
//==============================================================================
SettingsReciever::SettingsReciever(FileListImageProvider* aParent)
    : QObject(NULL)
    , ipParent(aParent)
{

}

//==============================================================================
// Use Default Icons Changed Slot
//==============================================================================
void SettingsReciever::useDefaultIconsChanged(const bool& aUseDefaultIcons)
{
    // Set Use Default Image
    ipParent->useDefaultIcons = aUseDefaultIcons;
}


//==============================================================================
// Destructor
//==============================================================================
SettingsReciever::~SettingsReciever()
{
}


