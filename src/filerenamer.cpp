
// INCLUDES

#include <QDebug>

#include "filerenamer.h"
#include "ui_filerenamer.h"
#include "customfilelist.h"


// IMPLEMENTATION



//==============================================================================
// Constructor
//==============================================================================
FileNameEditor::FileNameEditor(QWidget* aParent)
    : QLineEdit(aParent)
{
}

//==============================================================================
// Key Release Event
//==============================================================================
void FileNameEditor::keyReleaseEvent(QKeyEvent* aEvent)
{
    QLineEdit::keyReleaseEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Switch Key
        switch (aEvent->key()) {
            case Qt::Key_Escape:
                // Emit Editor Finished Signal
                emit editFinished(false);
            break;

            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Emit Editor Finished Signal
                emit editFinished(true);
            break;
        }
    }
}










//==============================================================================
// Constructor
//==============================================================================
FileRenamer::FileRenamer(QEventLoop* aEventLoop, QWidget* aParent)
    : QFrame(aParent)
    , ui(new Ui::FileRenamer)
    , eventLoop(aEventLoop)
    , mAccepted(false)
    , renamerParent(aParent)
{
    // Set Window Modality
    setWindowModality(Qt::ApplicationModal);
    // Set Window Flags
    setWindowFlags(Qt::Popup);

    // Setup UI
    ui->setupUi(this);

    // Set Parent
    setParent(aParent);

    // Conenct Signals
    connect(ui->fileNameEdit, SIGNAL(editFinished(bool)), this, SLOT(editorFinished(bool)));
}

//==============================================================================
// Set File Name
//==============================================================================
void FileRenamer::setFileName(const QString& aFileName)
{
    // Check UI
    if (ui && ui->fileNameEdit) {
        // Set Text
        ui->fileNameEdit->setText(aFileName);
    }
}

//==============================================================================
// Get File Name
//==============================================================================
QString FileRenamer::getFileName()
{
    // Check UI
    if (ui && ui->fileNameEdit) {
        return ui->fileNameEdit->text();
    }

    return QString("");
}

//==============================================================================
// Show
//==============================================================================
void FileRenamer::showRenamer(FileListDelegate* aDelegate)
{
    // Check Delegate
    if (aDelegate && renamerParent) {
        // Set Geometry
        setGeometry(QRect(renamerParent->mapToGlobal(renamerParent->pos()) + aDelegate->pos(), QSize(aDelegate->width(), aDelegate->height())));
    }

    // Show
    show();

    // Check UI
    if (ui && ui->fileNameEdit) {
        // Set Focus
        ui->fileNameEdit->setFocus();
    }
}

//==============================================================================
// Hide
//==============================================================================
void FileRenamer::hideRenamer()
{
    // Close
    close();

    // Check Event Loop
    if (eventLoop) {
        // Exit Event Loop
        eventLoop->exit(0);
    }
}

//==============================================================================
// Get Accepted State
//==============================================================================
bool FileRenamer::getAccepted()
{
    return mAccepted;
}

//==============================================================================
// Editor Finished Slot
//==============================================================================
void FileRenamer::editorFinished(const bool& aAccept)
{
    // Set Accepted State
    mAccepted = aAccept;
    // Hide Renamer
    hideRenamer();
}

//==============================================================================
// On Close Button Clicked Slot
//==============================================================================
void FileRenamer::on_closeButton_clicked()
{
    // Hide Renamer
    hideRenamer();
}
//==============================================================================
// Destructor
//==============================================================================
FileRenamer::~FileRenamer()
{
    // Delete UI
    delete ui;
}

