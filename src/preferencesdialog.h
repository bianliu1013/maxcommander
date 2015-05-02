#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class PreferencesDialog;
}

//==============================================================================
// Preferences Dialog Class
//==============================================================================
class PreferencesDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(bool showFunctionKeys READ getShowFunctionKeys WRITE setShowFunctionKeys NOTIFY showFunctionKeysChanged)
    Q_PROPERTY(bool showDirHotKeys READ getShowDirHotKeys WRITE setShowDirHotKeys NOTIFY showDirHotKeysChanged)
    Q_PROPERTY(bool showDriveButtons READ getShowDriveButtons WRITE setShowDriveButtons NOTIFY showDriveButtonsChanged)
    Q_PROPERTY(bool selectDirectories READ getSelectDirectories WRITE setSelectDirectories NOTIFY selectDirectoriesChanged)

    Q_PROPERTY(bool showHiddenFiles READ getShowHiddenFiles WRITE setShowHiddenFiles NOTIFY showHiddenFilesChanged)
    Q_PROPERTY(bool showDirsFirst READ getShowDirsFirst WRITE setShowDirsFirst NOTIFY showDirsFirstChanged)
    Q_PROPERTY(bool caseSensitiveSort READ getCaseSensitiveSort WRITE setCaseSensitiveSort NOTIFY caseSensitiveSortChanged)

    Q_PROPERTY(QString textColor READ getTextColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString textBGColor READ gettextBGColor WRITE setTextBGColor NOTIFY textBGColorChanged)

    Q_PROPERTY(QString currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(QString currentBGColor READ getCurrentBGColor WRITE setCurrentBGColor NOTIFY currentBGColorChanged)

    Q_PROPERTY(QString selectedColor READ getSelectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QString selectedBGColor READ getSelectedBGColor WRITE setSelectedBGColor NOTIFY selectedBGColorChanged)

    Q_PROPERTY(QString currentSelectedColor READ getCurrentSelectedColor WRITE setCurrentSelectedColor NOTIFY currentSelectedColorChanged)
    Q_PROPERTY(QString currentSelectedBGColor READ getCurrentSelectedBGColor WRITE setCurrentSelectedBGColor NOTIFY currentSelectedBGColorChanged)

    Q_PROPERTY(QString hiddenColor READ getHiddenColor WRITE setHiddenColor NOTIFY hiddenColorChanged)
    Q_PROPERTY(QString hiddenBGColor READ getHiddenBGColor WRITE setHiddenBGColor NOTIFY hiddenBGColorChanged)

    Q_PROPERTY(QString linkColor READ getLinkColor WRITE setLinkColor NOTIFY linkColorChanged)
    Q_PROPERTY(QString linkBGColor READ getLinkBGColor WRITE setLinkBGColor NOTIFY linkBGColorChanged)

    Q_PROPERTY(QString fontName READ getFontName WRITE setFontName NOTIFY fontNameChanged)
    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool fontBold READ getFontBold WRITE setFontBold NOTIFY fontBoldChanged)
    Q_PROPERTY(bool fontItalic READ getFontItalic WRITE setFontItalic NOTIFY fontItalicChanged)

    Q_PROPERTY(int thumbWidth READ getThumbWidth WRITE setThumbWidth NOTIFY thumbWidthChanged)
    Q_PROPERTY(int thumbHeight READ getThumbHeight WRITE setThumbHeight NOTIFY thumbHeightChanged)

    // ...

public:
    // Constructor
    explicit PreferencesDialog(QWidget* aParent = NULL);

    // Get Normal Text Color
    QString getTextColor();
    // Set Normal Text Color
    void setTextColor(const QString& aColor);

    // Get Normal Text BG Color
    QString gettextBGColor();
    // Set Normal Text BG Color
    void setTextBGColor(const QString& aColor);

    // Get Current Text Color
    QString getCurrentColor();
    // Set Current Text Color
    void setCurrentColor(const QString& aColor);

    // Get Current Text BG Color
    QString getCurrentBGColor();
    // Set Current Text BG Color
    void setCurrentBGColor(const QString& aColor);

    // Get Selected Text Color
    QString getSelectedColor();
    // Set Selected Text Color
    void setSelectedColor(const QString& aColor);

    // Get Selected Text BG Color
    QString getSelectedBGColor();
    // Set Selected Text BG Color
    void setSelectedBGColor(const QString& aColor);

    // Get Current Selected Text Color
    QString getCurrentSelectedColor();
    // Set Current Selected Text Color
    void setCurrentSelectedColor(const QString& aColor);

    // Get Current Selected Text BG Color
    QString getCurrentSelectedBGColor();
    // Set Current Selected Text BG Color
    void setCurrentSelectedBGColor(const QString& aColor);

    // Get Hidden Text Color
    QString getHiddenColor();
    // Set Hidden Text Color
    void setHiddenColor(const QString& aColor);

    // Get Hidden Text BG Color
    QString getHiddenBGColor();
    // Set Hidden Text BG Color
    void setHiddenBGColor(const QString& aColor);

    // Get Link Text Color
    QString getLinkColor();
    // Set Link Text Color
    void setLinkColor(const QString& aColor);

    // Get Link Text BG Color
    QString getLinkBGColor();
    // Set Link Text BG Color
    void setLinkBGColor(const QString& aColor);

    // Get Font Name
    QString getFontName();
    // Set Font Name
    void setFontName(const QString& aFontName);

    // Get Font Size
    int getFontSize();
    // Set Font Size
    void setFontSize(const int& aSize);

    // Get Font Bold
    bool getFontBold();
    // Set Font Bold
    void setFontBold(const bool& aBold);

    // Get Font Italic
    bool getFontItalic();
    // Set Font Italic
    void setFontItalic(const bool& aItalic);

    // Get Thumb Width
    int getThumbWidth();
    // Set Thumb Width
    void setThumbWidth(const int& aWidth);

    // Get Thumb Height
    int getThumbHeight();
    // Set Thumb height
    void setThumbHeight(const int& aheight);

    // Get Show Function Keys
    bool getShowFunctionKeys();
    // Set Show Function Keys
    void setShowFunctionKeys(const bool& aShow);

    // Get Show Directory Hot Keys
    bool getShowDirHotKeys();
    // Set Show Directory Hot Keys
    void setShowDirHotKeys(const bool& aShow);

    // Get Show Drive Buttons
    bool getShowDriveButtons();
    // Set Show Drive Buttons
    void setShowDriveButtons(const bool& aShow);

    // Get Select Directories
    bool getSelectDirectories();
    // Set Select Directories
    void setSelectDirectories(const bool& aSelect);

    // Get Show Hidden Files
    bool getShowHiddenFiles();
    // Set Show Hidden Files
    void setShowHiddenFiles(const bool& aShow);

    // Get Show Directories First
    bool getShowDirsFirst();
    // Set Show Directories First
    void setShowDirsFirst(const bool& aShow);

    // Get Case Sensitive Sorting
    bool getCaseSensitiveSort();
    // Set Case Sensitive Sorting
    void setCaseSensitiveSort(const bool& aCaseSensitive);


    // Destructor
    virtual ~PreferencesDialog();

public slots:

    // Exec Dialog
    int execDialog();

signals:

    // Settings Has changed Slot
    void settingsHasChanged();

    // Text Color Changed Signal
    void textColorChanged(const QString& aColor);
    // BG Color Changed Signal
    void textBGColorChanged(const QString& aColor);
    // Current Text Color Changed Signal
    void currentColorChanged(const QString& aColor);
    // Current BG Color Changed Signal
    void currentBGColorChanged(const QString& aColor);
    // Selected Color Changed Signal
    void selectedColorChanged(const QString& aColor);
    // Selected BG Color Changed Signal
    void selectedBGColorChanged(const QString& aColor);
    // Current Selected Color Changed Signal
    void currentSelectedColorChanged(const QString& aColor);
    // Current Selected BG Color Changed Signal
    void currentSelectedBGColorChanged(const QString& aColor);
    // Hidden Color Changed Signal
    void hiddenColorChanged(const QString& aColor);
    // Hidden BG Color Changed Signal
    void hiddenBGColorChanged(const QString& aColor);
    // Link Color Changed Signal
    void linkColorChanged(const QString& aColor);
    // Link BG Color Changed Signal
    void linkBGColorChanged(const QString& aColor);
    // Font Name Changed Signal
    void fontNameChanged(const QString& aFontName);
    // Font Size Changed Signal
    void fontSizeChanged(const int& aSize);
    // Font Bold Changed Signal
    void fontBoldChanged(const bool& aBold);
    // Font italic Changed Signal
    void fontItalicChanged(const bool& aItalic);

    // Thumb Width Changed Signal
    void thumbWidthChanged(const int& aWidth);
    // Thumb Height Changed Signal
    void thumbHeightChanged(const int& aHeight);

    // Show Function Keys Changed Signal
    void showFunctionKeysChanged(const bool& aShow);
    // Show Directory Hot Keys Changed Signal
    void showDirHotKeysChanged(const bool& aShow);
    // Show Drive Buttons changed Signal
    void showDriveButtonsChanged(const bool& aShow);

    // Select Directries Changed Signal
    void selectDirectoriesChanged(const bool& aSelect);
    // Show Hidden Files Changed Signal
    void showHiddenFilesChanged(const bool& aShow);
    // Show Directories First Changed Signal
    void showDirsFirstChanged(const bool& aShow);
    // Case Sensitive Sorting Changed Signal
    void caseSensitiveSortChanged(const bool& aCaseSensitive);

protected slots:

    // Init
    void init();

    // Load Settings
    void loadSettings();
    // Save Settings
    void saveSettings();

    // Restore Defaults
    void restoreDefaults();

    // Apply Settings
    void applySettings();

    // Restore UI
    void restoreUI();

    // Set Dirty
    void setDirty(const bool& aDirty);

    // Update Preview
    void updatePreview();

    // Update Font Button Text
    void updateFontButtonText();

    // Exec
    virtual int exec();

protected slots:

    // On Show Function Keys Check Box Clicked Slot
    void on_functionKeysCheckBox_clicked();
    // On Show Drive Buttons Check Box Clicked Slot
    void on_driveButtonsCheckBox_clicked();
    // On Show Directory Hot Keys Check Box Clicked Slot
    void on_dirHotKeysCheckBox_clicked();
    // On Show Hidden Files Check Box Clicked Slot
    void on_showHiddenFilesCheckBox_clicked();
    // On Select Dirs Check Box Clicked Slot
    void on_selectDirsCheckBox_clicked();
    // On Show Dirs First Check Box Clicked Slot
    void on_showDirsFirstCheckBox_clicked();
    // On Case Sensitive Sort Checkbox Clicked Slot
    void on_caseSensitiveSortCheckBox_clicked();

    // On Text Color Button Clicked Slot
    void on_textColorButton_clicked();
    // On Text BG Color Button Clicked Slot
    void on_textBgButton_clicked();
    // On Current Color Button Clicked Slot
    void on_currentColorButton_clicked();
    // On Current BG Color Button Clicked Slot
    void on_currentBGButton_clicked();
    // On Selected Color Button Clicked Slot
    void on_selectedColorButton_clicked();
    // On Selected BG Color Button Clicked Slot
    void on_selectedBGButton_clicked();
    // On Current Selected Color Button Clicked Slot
    void on_currentSelectedColorButton_clicked();
    // On Current Selected BG Color Button Clicked Slot
    void on_currentSelectedBGButton_clicked();
    // On Hidden Color Button Clicked Slot
    void on_hiddenTextColorButton_clicked();
    // On Hidden BG Color Button Clicked Slot
    void on_hiddenTextBgButton_clicked();
    // On Link Color Button Clicked Slot
    void on_linkTextColorButton_clicked();
    // On Link BG Color Button Clicked Slot
    void on_linkTextBgButton_clicked();
    // On Font Select Button Clicked Slot
    void on_fontButton_clicked();

    // Clear Current BG Color Button Clicked Slot
    void on_clearCurrentBGButton_clicked();
    // Clear Selected BG Color Button Clicked Slot
    void on_clearSelectedBGButton_clicked();
    // Clear Current Selected BG Color Button Clicked Slot
    void on_clearCurrentSelectedBGButton_clicked();
    // Clear Hidden BG Color Button Clicked Slot
    void on_clearHiddenBGButton_clicked();
    // Clear Link BG Color Button Clicked Slot
    void on_clearLinkBGButton_clicked();

    // Icon/Thumb Size Combo Box Current Index Changed Slot
    void on_iconSizeComboBox_currentIndexChanged(const QString& arg1);

protected slots:

    // Button Box Button Clicked
    void buttonBoxButtonPressed(QAbstractButton* aButton);

private:
    // UI
    Ui::PreferencesDialog*  ui;
    // Dirty
    bool                    dirty;

    // ...

    // Show Function Keys
    bool                    showFunctionKeys;
    // Show Directory Hot Keys
    bool                    showDirHotKeys;
    // Show Drive Buttons
    bool                    showDriveButtons;

    // Select Directories
    bool                    selectDirectories;
    // Show Hidden Files
    bool                    showHiddenFiles;
    // Show Directories First
    bool                    showDirsFirst;
    // Case Sensitive Sorting
    bool                    caseSensitiveSort;

    // ...

    // Normal Text Color
    QString                 textColor;
    // Normal Text BG Color
    QString                 textBGColor;
    // Current Item Text Color
    QString                 currentColor;
    // Current Item BG Color
    QString                 currentBGColor;
    // Selected Item Text Color
    QString                 selectedColor;
    // Selected Item BG Color
    QString                 selectedBGColor;
    // Current Selected Item Text Color
    QString                 currentSelectedColor;
    // Current Selected Item BG Color
    QString                 currentSelectedBGColor;
    // Hidden Item Text Color
    QString                 hiddenColor;
    // Hidden Item BG Color
    QString                 hiddenBGColor;
    // Link Item Text Color
    QString                 linkColor;
    // Link Item BG Color
    QString                 linkBGColor;
    // Font Name
    QString                 fontName;
    // Font Size
    int                     fontSize;
    // Font Bold
    bool                    fontBold;
    // Font Italic
    bool                    fontItalic;

    // Thumb Width
    int                     thumbWidth;
    // Thumb Height
    int                     thumbHeight;

    // ...

    // Viewer App Path
    QString                 viewerPath;
    // Editor App Path
    QString                 editorPath;
    // Compare Path
    QString                 comparePath;
    // Packer Path
    QString                 packerPath;
    // Unpacker Path
    QString                 unPackerPath;

    // ...

};

#endif // PREFERENCESDIALOG_H
