#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QUrl>
#include <QWidget>

namespace Ui {
class HelpWindow;
}

//==============================================================================
// Help Window Class
//==============================================================================
class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit HelpWindow(QWidget* aParent = NULL);
    // Destructor
    virtual ~HelpWindow();

public slots:

    // Load Content
    void loadContent(const QUrl& aURL = QUrl(""));

    // Show Window
    void showWindow();
    // Close
    void closeWindow();

protected:

    // Restore UI
    void restoreUI();
    // Save Settings
    void saveSettings();

protected: // From QWidget

    // Key Press Event
    virtual void keyPressEvent(QKeyEvent* aEvent);
    // Key Release Event
    virtual void keyReleaseEvent(QKeyEvent* aEvent);

private:
    // UI
    Ui::HelpWindow*     ui;
};

#endif // HELPWINDOW_H


