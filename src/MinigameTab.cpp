#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "ItemData.h"
#include "SaveManager.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>

MinigameTab::MinigameTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QHBoxLayout(this);

    auto* unlockBox = new QGroupBox(tr("Minigame Unlocks"), this);
    auto* ul = new QVBoxLayout(unlockBox);
    m_sjLock = new QCheckBox(tr("Squid Jump Unlocked"));
    m_sbLock = new QCheckBox(tr("Squid Ball Unlocked"));
    m_srLock = new QCheckBox(tr("Squid Racer Unlocked"));
    m_sbzLock = new QCheckBox(tr("Squid Beatz Unlocked"));
    ul->addWidget(m_sjLock);
    ul->addWidget(m_sbLock);
    ul->addWidget(m_srLock);
    ul->addWidget(m_sbzLock);
    root->addWidget(unlockBox);

    auto* scoreBox = new QGroupBox(tr("High Scores"), this);
    auto* sf = new QFormLayout(scoreBox);
    m_sjScore = new QLineEdit; m_sjScore->setMaxLength(4);
    m_sbScore = new QLineEdit; m_sbScore->setMaxLength(4);
    m_srScore = new QLineEdit; m_srScore->setMaxLength(4);
    sf->addRow(tr("Squid Jump High:"), m_sjScore);
    sf->addRow(tr("Squid Ball High:"), m_sbScore);
    sf->addRow(tr("Squid Racer High:"), m_srScore);
    root->addWidget(scoreBox);
}

void MinigameTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    uint32_t ptt = (uint32_t)data->minigameUnlocks;
    m_sjLock->setChecked(ptt & 0x010000u);
    m_sbLock->setChecked(ptt & 0x020000u);
    m_srLock->setChecked(ptt & 0x040000u);
    m_sbzLock->setChecked(ptt & 0x080000u);
    m_sjScore->setText(QString::number((int32_t)(uint32_t)data->squidJumpScore));
    m_sbScore->setText(QString::number((int32_t)(uint32_t)data->squidballScore));
    m_srScore->setText(QString::number((int32_t)(uint32_t)data->squidRacerScore));
}

void MinigameTab::saveData(SaveData* data) {
    if (!data) return;
    uint32_t ptt = (uint32_t)data->minigameUnlocks;
    if (m_sjLock->isChecked()) ptt |= 0x010000u; else ptt &= ~0x010000u;
    if (m_sbLock->isChecked()) ptt |= 0x020000u; else ptt &= ~0x020000u;
    if (m_srLock->isChecked()) ptt |= 0x040000u; else ptt &= ~0x040000u;
    if (m_sbzLock->isChecked()) ptt |= 0x080000u; else ptt &= ~0x080000u;
    data->minigameUnlocks = ptt;

    int32_t sj = m_sjScore->text().toInt(); if (sj > 9999) sj = 9999; if (sj < 0) sj = 0; data->squidJumpScore = (uint32_t)sj;
    int32_t sb = m_sbScore->text().toInt(); if (sb > 9999) sb = 9999; if (sb < 0) sb = 0; data->squidballScore = (uint32_t)sb;
    int32_t sr = m_srScore->text().toInt(); if (sr > 9999) sr = 9999; if (sr < 0) sr = 0; data->squidRacerScore = (uint32_t)sr;
}
