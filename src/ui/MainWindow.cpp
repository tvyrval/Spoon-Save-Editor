#include "MainWindow.h"
#include "SaveManager.h"
#include "EditorTabs.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QIcon>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>
#include <QTabWidget>
#include <cstring>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_tabs(new QTabWidget(this)),
      m_save(new SaveManager()) {
    setWindowIcon(QIcon(":/images/icon.png"));
    setMinimumSize(860, 540);
          
#ifdef Q_OS_MAC
          m_tabs->setDocumentMode(true);
#endif

    createMenus();
    createTabs();
    setCentralWidget(m_tabs);
    loadSettings();
    refreshWindowTitle();
}

MainWindow::~MainWindow() = default;

bool MainWindow::showInternalIDs() const {
    return m_actShowIDs->isChecked();
}

bool MainWindow::sortByAlphabet() const {
    return m_actSortAlpha->isChecked();
}

void MainWindow::createTabs() {
    m_editorTabs = {
        new WeaponTab(this),
        new GearTab(this),
        new PlayerTab(this),
        new HeroTab(this),
        new PlazaTab(this),
        new AmiiboTab(this),
        new MinigameTab(this),
        new SpykeTab(this),
        new BitFlagTab(this),
    };
    for (EditorTabBase* tab : m_editorTabs) {
        tab->setShowInternalIDs(m_actShowIDs ? m_actShowIDs->isChecked() : false);
    }
    m_tabs->addTab(m_editorTabs[0], tr("Weapon Editor"));
    m_tabs->addTab(m_editorTabs[1], tr("Gear Editor"));
    m_tabs->addTab(m_editorTabs[2], tr("Player Editor"));
    m_tabs->addTab(m_editorTabs[3], tr("Hero Mode"));
    m_tabs->addTab(m_editorTabs[4], tr("Plaza Editor"));
    m_tabs->addTab(m_editorTabs[5], tr("Amiibo Challenges"));
    m_tabs->addTab(m_editorTabs[6], tr("Minigames"));
    m_tabs->addTab(m_editorTabs[7], tr("Spyke Orders"));
    m_tabs->addTab(m_editorTabs[8], tr("BitFlags"));
}

void MainWindow::applyTabsDisplay() {
    const bool ids = m_actShowIDs->isChecked();
    const bool alpha = m_actSortAlpha->isChecked();
    for (EditorTabBase* tab : m_editorTabs) {
        tab->setShowInternalIDs(ids);
        tab->setSortByAlphabet(alpha);
    }
}

void MainWindow::createMenus() {
    QMenuBar* mb = menuBar();

    QMenu* fileMenu = mb->addMenu(tr("&File"));
    m_actOpen = fileMenu->addAction(tr("&Open..."));
    m_actSave = fileMenu->addAction(tr("&Save"));
    m_actSaveAs = fileMenu->addAction(tr("&Save As..."));
    fileMenu->addSeparator();
    m_actExit = fileMenu->addAction(tr("E&xit"));

    QMenu* settingsMenu = mb->addMenu(tr("&Settings"));
    m_actShowIDs = settingsMenu->addAction(tr("Show Internal IDs"));
    m_actShowIDs->setCheckable(true);
    m_actSortAlpha = settingsMenu->addAction(tr("Sort Text Alphabetically"));
    m_actSortAlpha->setCheckable(true);
    settingsMenu->addSeparator();
    m_actBrick = settingsMenu->addAction(tr("Brick Save"));

    QMenu* helpMenu = mb->addMenu(tr("&Help"));
    m_actAbout = helpMenu->addAction(tr("&About"));

    connect(m_actOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_actSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(m_actSaveAs, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(m_actExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_actShowIDs, &QAction::toggled, this, &MainWindow::onShowInternalIDsToggled);
    connect(m_actSortAlpha, &QAction::toggled, this, &MainWindow::onSortByAlphabetToggled);
    connect(m_actBrick, &QAction::triggered, this, &MainWindow::brickSave);
    connect(m_actAbout, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::openFile() {
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open Save File"), QString(),
        tr("Dat Files (*.dat);;All Files (*)"));
    if (path.isEmpty()) return;

    if (!m_save->loadFile(path.toStdString())) {
        QMessageBox::warning(this, tr("Error"),
            tr("Failed to open the save file. Expected a Splatoon 1 save "
               "(~53888 or 52292 bytes)."));
        return;
    }

    SaveData* d = m_save->data();
    if (!d) {
        QMessageBox::warning(this, tr("Error"), tr("Save data is not available."));
        return;
    }
    for (EditorTabBase* tab : m_editorTabs)
        tab->loadData(d);

    refreshWindowTitle();
    statusBar()->showMessage(tr("Loaded: %1").arg(path), 4000);
}

void MainWindow::saveFile() {
    if (!m_save->hasData()) {
        QMessageBox::warning(this, tr("Error"), tr("No file loaded."));
        return;
    }

    const QString path = QString::fromStdString(m_save->currentPath());
    if (path.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No file loaded."));
        return;
    }

    SaveData* d = m_save->data();
    if (d)
        for (EditorTabBase* tab : m_editorTabs)
            tab->saveData(d);

    if (!m_save->saveFile(path.toStdString())) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to write the save file."));
        return;
    }

    refreshWindowTitle();
    statusBar()->showMessage(tr("Saved: %1").arg(path), 4000);
}

void MainWindow::saveAsFile() {
    if (!m_save->hasData()) {
        QMessageBox::warning(this, tr("Error"), tr("No file loaded."));
        return;
    }

    const QString path = QFileDialog::getSaveFileName(
        this, tr("Save As..."), QString(),
        tr("Dat Files (*.dat);;All Files (*)"));
    if (path.isEmpty()) return;

    if (!m_save->saveFile(path.toStdString())) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to write the save file."));
        return;
    }
}

void MainWindow::brickSave() {
    if (!m_save->hasData()) {
        QMessageBox::warning(this, tr("Error"), tr("No file loaded."));
        return;
    }

    const auto choice = QMessageBox::warning(this, tr("Brick Save"),
        tr("This will zero out the loaded save data in memory, rendering it "
           "corrupt. Continue?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (choice != QMessageBox::Yes) return;

    SaveData* d = m_save->data();
    if (d) {
        std::memset(d, 0, sizeof(SaveData));
        for (EditorTabBase* tab : m_editorTabs)
            tab->loadData(d);
    }

    refreshWindowTitle();
    statusBar()->showMessage(tr("Save data bricked in memory. Use Save to write it."), 6000);
}

#ifndef APP_VERSION
#define APP_VERSION "Unknown"
#endif

void MainWindow::showAbout() {
    QString aboutText = QStringLiteral(
        "Spoon Save Editor\n"
        "Made by @tvyrval / vyrval\n"
        "UI made by @jerrysm64 / Jerry Starke\n"
        "Version: %1").arg(QStringLiteral(APP_VERSION));

    QMessageBox::about(this, tr("About"), aboutText);
}

void MainWindow::onShowInternalIDsToggled(bool /*checked*/) {
    saveSettings();
    applyTabsDisplay();
}

void MainWindow::onSortByAlphabetToggled(bool /*checked*/) {
    saveSettings();
    applyTabsDisplay();
}

void MainWindow::loadSettings() {
    QSettings s;
    m_actShowIDs->setChecked(s.value("showInternalIDs", false).toBool());
    m_actSortAlpha->setChecked(s.value("sortByAlphabet", false).toBool());
    applyTabsDisplay();
}

void MainWindow::saveSettings() const {
    QSettings s;
    s.setValue("showInternalIDs", m_actShowIDs->isChecked());
    s.setValue("sortByAlphabet", m_actSortAlpha->isChecked());
}

void MainWindow::refreshWindowTitle() {
    QString title = tr("Spoon Save Editor");
    if (m_save->hasData() && !m_save->currentPath().empty())
        title += QStringLiteral(" - ") + QString::fromStdString(m_save->currentPath());
    setWindowTitle(title);
}
