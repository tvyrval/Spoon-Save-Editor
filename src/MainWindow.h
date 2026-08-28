#pragma once

#include <QMainWindow>
#include <QList>
#include <memory>

class QTabWidget;
class QAction;
class SaveManager;
class EditorTabBase;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    bool showInternalIDs() const;
    bool sortByAlphabet() const;

private slots:
    void openFile();
    void saveFile();
    void saveAsFile();
    void brickSave();
    void showAbout();
    void onShowInternalIDsToggled(bool checked);
    void onSortByAlphabetToggled(bool checked);

private:
    void createMenus();
    void createTabs();
    void loadSettings();
    void saveSettings() const;
    void refreshWindowTitle();
    void applyTabsDisplay();

    QTabWidget* m_tabs;
    QList<EditorTabBase*> m_editorTabs;
    std::unique_ptr<SaveManager> m_save;

    QAction* m_actOpen;
    QAction* m_actSave;
    QAction* m_actSaveAs;
    QAction* m_actExit;
    QAction* m_actShowIDs;
    QAction* m_actSortAlpha;
    QAction* m_actBrick;
    QAction* m_actAbout;
};
