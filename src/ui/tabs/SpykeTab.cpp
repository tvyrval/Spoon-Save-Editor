#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "ItemData.h"
#include "SaveManager.h"

#include <QComboBox>
#include <QDateTime>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

SpykeTab::SpykeTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QHBoxLayout(this);
    for (int i = 0; i < 3; ++i) {
        auto* box = new QGroupBox(tr("Order %1").arg(i + 1), this);
        auto* f = new QFormLayout(box);
        m_orders[i].category = new QComboBox;
        m_orders[i].gear = new QComboBox;
        m_orders[i].state = new QComboBox;
        m_orders[i].ordered = new QLineEdit;
        m_orders[i].arrival = new QLineEdit;
        m_orders[i].now = new QPushButton(tr("Set Times to Now"));
        f->addRow(tr("Category:"), m_orders[i].category);
        f->addRow(tr("Gear:"), m_orders[i].gear);
        f->addRow(tr("Status:"), m_orders[i].state);
        f->addRow(tr("Ordered:"), m_orders[i].ordered);
        f->addRow(tr("Arrival:"), m_orders[i].arrival);
        f->addRow(QString(), m_orders[i].now);
        root->addWidget(box);

        etab::fillItemCombo(m_orders[i].category, const_cast<const wchar_t**>(g_SpykeCategoryNames),
                            g_SpykeCategoryCount, false, false, m_showIDs);
        etab::fillItemCombo(m_orders[i].state, const_cast<const wchar_t**>(g_SpykeStateNames),
                            g_SpykeStateCount, false, false, m_showIDs);

        const int idx = i;
        connect(m_orders[i].category, &QComboBox::currentIndexChanged, this,
                [this, idx](int){ onCategoryChanged(idx); });
        connect(m_orders[i].now, &QPushButton::clicked, this, [this, idx]{ onSetNow(idx); });
    }
}

void SpykeTab::buildOrderBox(int, const QString&) {
}

void SpykeTab::fillGearForCategory(QComboBox* gear, int catSel, uint32_t activeID) {
    const wchar_t** names = nullptr;
    int count = 0;
    if (catSel == 1) { names = const_cast<const wchar_t**>(g_ShoeNames); count = g_ShoeCount; }
    else if (catSel == 2) { names = const_cast<const wchar_t**>(g_ClothNames); count = g_ClothCount; }
    else if (catSel == 3) { names = const_cast<const wchar_t**>(g_HeadNames); count = g_HeadCount; }
    if (names) {
        etab::fillPlazaGearCombo(gear, names, count, m_sortAlpha, m_showIDs);
        etab::setComboById(gear, (int32_t)activeID);
    } else {
        gear->clear();
    }
}

void SpykeTab::syncOrder(int idx) {
    if (!m_data) return;
    SpykeOrder& o = m_data->orders[idx];

    int32_t catRaw = (int32_t)(uint32_t)o.category;
    int catSel = 0;
    if (catRaw == 0) catSel = 1;
    else if (catRaw == 1) catSel = 2;
    else if (catRaw == 2) catSel = 3;
    m_orders[idx].category->setCurrentIndex(catSel);

    fillGearForCategory(m_orders[idx].gear, catSel, (uint32_t)o.gearID);

    int32_t stateRaw = (int32_t)(uint32_t)o.state;
    int stateSel = 0;
    if (stateRaw == 0) stateSel = 1;
    else if (stateRaw == 1) stateSel = 2;
    m_orders[idx].state->setCurrentIndex(stateSel);

    m_orders[idx].ordered->setText(etab::timeToText((uint32_t)o.orderedTime));
    m_orders[idx].arrival->setText(etab::timeToText((uint32_t)o.arrivalTime));
}

void SpykeTab::saveOrder(int idx) {
    if (!m_data) return;
    SpykeOrder& o = m_data->orders[idx];

    int catSel = m_orders[idx].category->currentIndex();
    if (catSel == 0) o.category = (uint32_t)-1;
    else o.category = (uint32_t)(catSel - 1);

    o.gearID = (uint32_t)etab::comboId(m_orders[idx].gear);

    int stateSel = m_orders[idx].state->currentIndex();
    if (stateSel == 0) o.state = (uint32_t)-1;
    else o.state = (uint32_t)(stateSel - 1);

    o.orderedTime = etab::textToTime(m_orders[idx].ordered->text());
    o.arrivalTime = etab::textToTime(m_orders[idx].arrival->text());
}

void SpykeTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    for (int i = 0; i < 3; ++i) syncOrder(i);
}

void SpykeTab::saveData(SaveData* data) {
    if (!data) return;
    for (int i = 0; i < 3; ++i) saveOrder(i);
}

void SpykeTab::onCategoryChanged(int idx) {
    int catSel = m_orders[idx].category->currentIndex();
    fillGearForCategory(m_orders[idx].gear, catSel, 0);
}

void SpykeTab::onSetNow(int idx) {
    uint32_t t = (uint32_t)QDateTime::currentDateTime().toSecsSinceEpoch();
    m_orders[idx].ordered->setText(etab::timeToText(t));
    m_orders[idx].arrival->setText(etab::timeToText(t + 86400u));
}
