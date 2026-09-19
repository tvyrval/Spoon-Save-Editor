#include "EditorTabs.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

StatsTab::StatsTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QVBoxLayout(this);

    auto* statsBox = new QGroupBox(tr("Battle Statistics"), this);
    auto* form = new QFormLayout(statsBox);

    m_winSum = new QLabel(this);
    m_winSum->setTextInteractionFlags(Qt::TextSelectableByMouse);

    m_loseSum = new QLabel(this);
    m_loseSum->setTextInteractionFlags(Qt::TextSelectableByMouse);

    m_totalMatches = new QLabel(this);
    m_totalMatches->setTextInteractionFlags(Qt::TextSelectableByMouse);

    form->addRow(tr("Victories:"), m_winSum);
    form->addRow(tr("Defeats:"), m_loseSum);
    form->addRow(tr("Total Matches:"), m_totalMatches);

    root->addWidget(statsBox);
    root->addStretch(1);
}

void StatsTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    uint32_t wins = (uint32_t)data->winSum;
    uint32_t losses = (uint32_t)data->loseSum;
    m_winSum->setText(QString::number(wins));
    m_loseSum->setText(QString::number(losses));
    m_totalMatches->setText(QString::number(wins + losses));
}

void StatsTab::saveData(SaveData* data) {
    // Read-only info tab; do not modify save data
}
