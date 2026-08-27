#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "ItemData.h"
#include "save_data.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

static const int g_HeroStageIDs[] = {
    1, 2, 3, 101, 4, 5, 6, 7, 8, 9, 102, 10, 11, 12, 13, 14, 15, 103, 16, 17, 18, 19, 20, 21, 104, 22, 23, 24, 25, 26, 27, 105
};
static const char* g_HeroStageNames[] = {
    "Stage 1", "Stage 2", "Stage 3", "Boss 1",
    "Stage 4", "Stage 5", "Stage 6", "Stage 7", "Stage 8", "Stage 9", "Boss 2",
    "Stage 10", "Stage 11", "Stage 12", "Stage 13", "Stage 14", "Stage 15", "Boss 3",
    "Stage 16", "Stage 17", "Stage 18", "Stage 19", "Stage 20", "Stage 21", "Boss 4",
    "Stage 22", "Stage 23", "Stage 24", "Stage 25", "Stage 26", "Stage 27", "Boss 5"
};

HeroTab::HeroTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QVBoxLayout(this);

    auto* top = new QHBoxLayout;

    auto* genBox = new QGroupBox(tr("General & Upgrades"), this);
    auto* gf = new QFormLayout(genBox);
    m_powerEggs = new QLineEdit; m_powerEggs->setMaxLength(4);
    m_shotLvl = new QComboBox;
    m_tankLvl = new QComboBox;
    m_bombSelect = new QComboBox;
    m_splatLvl = new QComboBox;
    m_burstLvl = new QComboBox;
    m_seekerLvl = new QComboBox;
    gf->addRow(tr("Power Eggs:"), m_powerEggs);
    gf->addRow(tr("Hero Shot Lvl:"), m_shotLvl);
    gf->addRow(tr("Ink Tank Lvl:"), m_tankLvl);
    gf->addRow(tr("Equipped Bomb:"), m_bombSelect);
    gf->addRow(tr("Splat Bomb Lvl:"), m_splatLvl);
    gf->addRow(tr("Burst Bomb Lvl:"), m_burstLvl);
    gf->addRow(tr("Seeker Lvl:"), m_seekerLvl);
    m_unlockWorld = new QPushButton(tr("Unlock All World (Meta)"));
    gf->addRow(QString(), m_unlockWorld);
    top->addWidget(genBox);

    auto* stBox = new QGroupBox(tr("Stage Progress"), this);
    auto* stLay = new QVBoxLayout(stBox);
    m_stageList = new QListWidget;
    for (int i = 0; i < 32; ++i) m_stageList->addItem(QString::fromLatin1(g_HeroStageNames[i]));
    stLay->addWidget(m_stageList);
    m_zapfish = new QCheckBox(tr("Zapfish Collected"));
    m_scroll = new QCheckBox(tr("Sunken Scroll Collected"));
    m_completeAll = new QPushButton(tr("Mark All Stages Complete"));
    stLay->addWidget(m_zapfish);
    stLay->addWidget(m_scroll);
    stLay->addWidget(m_completeAll);
    top->addWidget(stBox);
    root->addLayout(top);

    auto* flagBox = new QGroupBox(tr("Story Flags"), this);
    auto* ff = new QFormLayout(flagBox);
    m_flagZapfish = new QCheckBox(tr("Great Zapfish Returned (Story Beaten)"));
    m_flagHerosuit = new QCheckBox(tr("Hero Suit Given"));
    m_flagCuttlefish = new QCheckBox(tr("Cap'n Cuttlefish Dialogue Seen"));
    m_inkrails = new QLineEdit; m_inkrails->setMaxLength(10);
    m_events = new QLineEdit; m_events->setMaxLength(10);
    ff->addRow(m_flagZapfish);
    ff->addRow(m_flagHerosuit);
    ff->addRow(m_flagCuttlefish);
    ff->addRow(tr("Hero Inkrails Unlocked:"), m_inkrails);
    ff->addRow(tr("Hero Events (Bitmask):"), m_events);
    root->addWidget(flagBox);

    connect(m_stageList, &QListWidget::currentRowChanged, this, &HeroTab::onStageChanged);
    connect(m_unlockWorld, &QPushButton::clicked, this, &HeroTab::onUnlockWorld);
    connect(m_completeAll, &QPushButton::clicked, this, &HeroTab::onCompleteAll);

    etab::fillItemCombo(m_shotLvl, const_cast<const wchar_t**>(g_HeroUpgradeNames), g_HeroUpgradeCount, false, false, false);
    etab::fillItemCombo(m_tankLvl, const_cast<const wchar_t**>(g_HeroUpgradeNames), g_HeroUpgradeCount, false, false, false);
    etab::fillItemCombo(m_splatLvl, const_cast<const wchar_t**>(g_HeroUpgradeNames), g_HeroUpgradeCount, false, false, false);
    etab::fillItemCombo(m_burstLvl, const_cast<const wchar_t**>(g_HeroUpgradeNames), g_HeroUpgradeCount, false, false, false);
    etab::fillItemCombo(m_seekerLvl, const_cast<const wchar_t**>(g_HeroUpgradeNames), g_HeroUpgradeCount, false, false, false);
    etab::fillItemCombo(m_bombSelect, const_cast<const wchar_t**>(g_BombNames), g_BombCount, false, false, false);
    m_stageList->setCurrentRow(0);
}

void HeroTab::loadStage(int index) {
    if (!m_data || index < 0 || index >= 32) return;
    int id = g_HeroStageIDs[index];
    for (int i = 0; i < 64; ++i) {
        if ((uint32_t)m_data->heroStages[i].stageID == (uint32_t)id) {
            m_zapfish->setChecked(m_data->heroStages[i].zapfish == 3);
            m_scroll->setChecked(m_data->heroStages[i].scroll == 1);
            return;
        }
    }
    m_zapfish->setChecked(false);
    m_scroll->setChecked(false);
}

void HeroTab::saveStage(int index) {
    if (!m_data || index < 0 || index >= 32) return;
    int id = g_HeroStageIDs[index];
    bool zap = m_zapfish->isChecked();
    bool scroll = m_scroll->isChecked();
    for (int i = 0; i < 64; ++i) {
        if ((uint32_t)m_data->heroStages[i].stageID == (uint32_t)id) {
            m_data->heroStages[i].zapfish = zap ? 3 : 0;
            m_data->heroStages[i].scroll = scroll ? 1 : 0;
            return;
        }
    }
    for (int i = 0; i < 64; ++i) {
        if ((uint32_t)m_data->heroStages[i].stageID == 0 || (int32_t)(uint32_t)m_data->heroStages[i].stageID == -1) {
            m_data->heroStages[i].stageID = id;
            m_data->heroStages[i].zapfish = zap ? 3 : 0;
            m_data->heroStages[i].scroll = scroll ? 1 : 0;
            return;
        }
    }
}

void HeroTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    m_powerEggs->setText(QString::number((int32_t)(uint32_t)data->powerEggs));
    etab::setComboById(m_shotLvl, (int32_t)(uint32_t)data->heroShotLevel);
    etab::setComboById(m_tankLvl, (int32_t)(uint32_t)data->heroTankLevel);
    etab::setComboById(m_splatLvl, (int32_t)(uint32_t)data->heroSplatBombLevel);
    etab::setComboById(m_burstLvl, (int32_t)(uint32_t)data->heroBurstBombLevel);
    etab::setComboById(m_seekerLvl, (int32_t)(uint32_t)data->heroSeekerLevel);
    etab::setComboById(m_bombSelect, (int32_t)(uint32_t)data->heroBombSelect);

    m_flagZapfish->setChecked((uint32_t)data->gameStateFlags & 0x00000080u);
    m_flagHerosuit->setChecked((uint32_t)data->gameStateFlags & 0x00000010u);
    m_flagCuttlefish->setChecked((uint32_t)data->gameStateFlags & 0x00000100u);

    m_inkrails->setText(QString::number((int32_t)(uint32_t)data->heroInkrails));
    m_events->setText(QString::number((int32_t)(uint32_t)data->heroEvents));

    m_currentStage = 0;
    m_stageList->setCurrentRow(0);
    loadStage(m_currentStage);
}

void HeroTab::saveData(SaveData* data) {
    if (!data) return;
    int32_t eggs = m_powerEggs->text().toInt();
    if (eggs > 9999) eggs = 9999; if (eggs < 0) eggs = 0;
    data->powerEggs = (uint32_t)eggs;
    data->heroShotLevel = (uint32_t)etab::comboId(m_shotLvl);
    data->heroTankLevel = (uint32_t)etab::comboId(m_tankLvl);
    data->heroSplatBombLevel = (uint32_t)etab::comboId(m_splatLvl);
    data->heroBurstBombLevel = (uint32_t)etab::comboId(m_burstLvl);
    data->heroSeekerLevel = (uint32_t)etab::comboId(m_seekerLvl);
    data->heroBombSelect = (uint32_t)etab::comboId(m_bombSelect);

    uint32_t flags = (uint32_t)data->gameStateFlags;
    if (m_flagZapfish->isChecked()) flags |= 0x00000080u; else flags &= ~0x00000080u;
    if (m_flagHerosuit->isChecked()) flags |= 0x00000010u; else flags &= ~0x00000010u;
    if (m_flagCuttlefish->isChecked()) flags |= 0x00000100u; else flags &= ~0x00000100u;
    data->gameStateFlags = flags;

    data->heroInkrails = (uint32_t)m_inkrails->text().toUInt();
    data->heroEvents = (uint32_t)m_events->text().toUInt();

    saveStage(m_currentStage);
}

void HeroTab::onStageChanged(int row) {
    if (!m_data || row < 0) return;
    saveStage(m_currentStage);
    m_currentStage = row;
    loadStage(m_currentStage);
}

void HeroTab::onUnlockWorld() {
    if (!m_data) return;
    m_data->heroInkrails = 0xFFFFFFFFu;
    m_data->heroEvents = 0xFFFFFFFFu;
    m_inkrails->setText(QString::number((int32_t)(uint32_t)m_data->heroInkrails));
    m_events->setText(QString::number((int32_t)(uint32_t)m_data->heroEvents));
    QMessageBox::information(this, tr("Hero Mode"), tr("All Inkrails and World Events unlocked."));
}

void HeroTab::onCompleteAll() {
    if (!m_data) return;
    for (int i = 0; i < 32; ++i) {
        int id_val = g_HeroStageIDs[i];
        bool found = false;
        for (int j = 0; j < 64; ++j) {
            if ((uint32_t)m_data->heroStages[j].stageID == (uint32_t)id_val) {
                m_data->heroStages[j].zapfish = 3;
                m_data->heroStages[j].scroll = 1;
                found = true;
                break;
            }
        }
        if (!found) {
            for (int j = 0; j < 64; ++j) {
                if ((uint32_t)m_data->heroStages[j].stageID == 0 || (int32_t)(uint32_t)m_data->heroStages[j].stageID == -1) {
                    m_data->heroStages[j].stageID = id_val;
                    m_data->heroStages[j].zapfish = 3;
                    m_data->heroStages[j].scroll = 1;
                    break;
                }
            }
        }
    }
    m_data->heroInkrails = 0x1FFFFFu;
    m_data->heroEvents = 0x3EFBEu;
    loadStage(m_currentStage);
    QMessageBox::information(this, tr("Hero Mode"),
        tr("All 32 stages marked complete.\nAll inkrails and world events unlocked."));
}
