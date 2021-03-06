#include "src/passworddialog.h"
#include "ui_passworddialog.h"


//==============================================================================
// Constructor
//==============================================================================
PasswordDialog::PasswordDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::PasswordDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Get Pass
//==============================================================================
QString PasswordDialog::getPass()
{
    return ui->passwordEdit->text();
}

//==============================================================================
// Destructor
//==============================================================================
PasswordDialog::~PasswordDialog()
{
    // Delete UI
    delete ui;
}


