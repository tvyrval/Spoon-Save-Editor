#include "EditorTabs.h"
#include <QFormLayout>
#include <QLabel>

InfoTab::InfoTab(QWidget* parent) : EditorTabBase(parent) {
    auto* layout = new QFormLayout(this);
    
    m_winSum = new QLabel(this);
    m_winSum->setTextInteractionFlags(Qt::TextSelectableByMouse);
    
    m_loseSum = new QLabel(this);
    m_loseSum->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout->addRow(tr("Wins:"), m_winSum);
    layout->addRow(tr("Loses:"), m_loseSum);
}

void InfoTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    m_winSum->setText(QString::number((uint32_t)data->winSum));
    m_loseSum->setText(QString::number((uint32_t)data->loseSum));
}

void InfoTab::saveData(SaveData* data) {
    // Read-only info tab; do not modify save data
}
