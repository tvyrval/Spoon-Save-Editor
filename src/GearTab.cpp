#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "ItemData.h"
#include "SaveManager.h"

#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <algorithm>

static int itemNameID(const wchar_t** names, int count, int index) {
    if (index < 0 || index >= count) return -1;
    return etab::parseItemID(etab::w2q(names[index]));
}

static QString itemNameByID(const wchar_t** names, int count, int id, bool showIDs) {
    for (int i = 0; i < count; ++i)
        if (itemNameID(names, count, i) == id) return etab::w2q(names[i]);
    return QStringLiteral("Unknown Gear");
}

GearSubTab::GearSubTab(QWidget* parent) : QWidget(parent) {
    auto* root = new QVBoxLayout(this);

    auto* equipBox = new QGroupBox(tr("Equipped Gear"), this);
    auto* eb = new QHBoxLayout(equipBox);
    eb->addWidget(new QLabel(tr("Equipped:")));
    m_equipped = new QComboBox;
    eb->addWidget(m_equipped, 1);
    root->addWidget(equipBox);

    auto* mid = new QHBoxLayout;

    auto* invBox = new QGroupBox(tr("Gear Inventory"), this);
    auto* ib = new QVBoxLayout(invBox);
    ib->addWidget(new QLabel(tr("Inventory (Select to Edit):")));
    m_list = new QListWidget;
    m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ib->addWidget(m_list);
    mid->addWidget(invBox);

    auto* editBox = new QGroupBox(tr("Edit Selected Attributes"), this);
    auto* ed = new QVBoxLayout(editBox);
    auto* a1 = new QHBoxLayout; a1->addWidget(new QLabel(tr("Ability 1:"))); m_sub1 = new QComboBox; a1->addWidget(m_sub1); ed->addLayout(a1);
    auto* a2 = new QHBoxLayout; a2->addWidget(new QLabel(tr("Ability 2:"))); m_sub2 = new QComboBox; a2->addWidget(m_sub2); ed->addLayout(a2);
    auto* a3 = new QHBoxLayout; a3->addWidget(new QLabel(tr("Ability 3:"))); m_sub3 = new QComboBox; a3->addWidget(m_sub3); ed->addLayout(a3);
    auto* st = new QHBoxLayout;
    st->addWidget(new QLabel(tr("Level:"))); m_level = new QLineEdit; m_level->setMaxLength(1); st->addWidget(m_level);
    st->addWidget(new QLabel(tr("Slots:"))); m_slots = new QLineEdit; m_slots->setMaxLength(1); st->addWidget(m_slots);
    st->addWidget(new QLabel(tr("Exp:"))); m_exp = new QLineEdit; st->addWidget(m_exp);
    ed->addLayout(st);
    m_newFlag = new QCheckBox(tr("Mark as 'New'")); ed->addWidget(m_newFlag);
    m_apply = new QPushButton(tr("Apply Stats")); ed->addWidget(m_apply);
    mid->addWidget(editBox);
    root->addLayout(mid, 1);

    auto* addBox = new QGroupBox(tr("Add New Gear"), this);
    auto* ab = new QHBoxLayout(addBox);
    ab->addWidget(new QLabel(tr("Gear:")));
    m_addPicker = new QComboBox;
    ab->addWidget(m_addPicker, 1);
    m_add = new QPushButton(tr("Add"));
    m_del = new QPushButton(tr("Delete"));
    ab->addWidget(m_add);
    ab->addWidget(m_del);
    root->addWidget(addBox);

    etab::fillAbilityCombo(m_sub1, false, false);
    etab::fillAbilityCombo(m_sub2, false, false);
    etab::fillAbilityCombo(m_sub3, false, false);

    connect(m_list, &QListWidget::itemSelectionChanged, this, &GearSubTab::onListSelectionChanged);
    connect(m_apply, &QPushButton::clicked, this, &GearSubTab::onApply);
    connect(m_add, &QPushButton::clicked, this, &GearSubTab::onAdd);
    connect(m_del, &QPushButton::clicked, this, &GearSubTab::onDelete);
}

void GearSubTab::bind(SaveData* data, OwnedGearS1* array, const wchar_t** names,
                      int count, be32_t* equippedField, bool showIDs, bool sortAlpha) {
    m_data = data;
    m_array = array;
    m_names = names;
    m_count = count;
    m_equippedField = equippedField;
    refreshDisplay(showIDs, sortAlpha);
}

void GearSubTab::refreshDisplay(bool showIDs, bool sortAlpha) {
    m_showIDs = showIDs;
    m_sortAlpha = sortAlpha;
    etab::fillAbilityCombo(m_sub1, m_sortAlpha, m_showIDs);
    etab::fillAbilityCombo(m_sub2, m_sortAlpha, m_showIDs);
    etab::fillAbilityCombo(m_sub3, m_sortAlpha, m_showIDs);
    refreshList();
    refreshAddPicker();
    refreshEquippedPicker();
}

void GearSubTab::refreshEquippedPicker() {
    m_equipped->clear();
    if (!m_data || !m_array) return;
    struct Row { QString display; int id; };
    QVector<Row> rows;
    for (int i = 0; i < 256; ++i) {
        uint32_t id = m_array[i].id;
        if (id == 0xFFFFFFFF) continue;
        Row r;
        r.id = (int)id;
        r.display = etab::itemDisplay(itemNameByID(m_names, m_count, id, m_showIDs), m_showIDs);
        rows.append(r);
    }
    if (m_sortAlpha) {
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
            return a.display.compare(b.display, Qt::CaseInsensitive) < 0;
        });
    } else {
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) { return a.id < b.id; });
    }
    for (const auto& r : rows)
        m_equipped->addItem(r.display, QVariant(r.id));
    if (m_equippedField)
        etab::setComboById(m_equipped, (int32_t)(uint32_t)*m_equippedField);
}

void GearSubTab::refreshList() {
    m_list->clear();
    if (!m_data || !m_array) return;

    struct Row { QString display; uint32_t id; int idx; };
    QVector<Row> rows;
    QVector<uint32_t> seen;
    for (int i = 0; i < 256; ++i) {
        uint32_t id = m_array[i].id;
        if (id == 0xFFFFFFFF) continue;
        if (seen.contains(id)) continue;
        seen.append(id);
        Row r;
        r.idx = i;
        r.id = id;
        r.display = etab::itemDisplay(itemNameByID(m_names, m_count, id, m_showIDs), m_showIDs);
        rows.append(r);
    }
    if (m_sortAlpha) {
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
            return a.display.compare(b.display, Qt::CaseInsensitive) < 0;
        });
    } else {
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) { return a.id < b.id; });
    }
    for (const auto& r : rows) {
        auto* item = new QListWidgetItem(r.display);
        item->setData(Qt::UserRole, r.idx);
        m_list->addItem(item);
    }
}

void GearSubTab::refreshAddPicker() {
    if (!m_data || !m_array) { m_addPicker->clear(); return; }
    QVector<const wchar_t*> unowned;
    for (int i = 0; i < m_count; ++i) {
        QString raw = etab::w2q(m_names[i]);
        if (raw.contains(QLatin1String("(Test_Clt_")) || raw.contains(QLatin1String("(HAP")))
            continue;
        int gearID = etab::parseItemID(raw);
        if (gearID < 0) continue;
        bool owned = false;
        for (int j = 0; j < 256; ++j)
            if (m_array[j].id != 0xFFFFFFFF && m_array[j].id == (uint32_t)gearID) { owned = true; break; }
        if (!owned) unowned.append(m_names[i]);
    }
    etab::fillItemCombo(m_addPicker, const_cast<const wchar_t**>(unowned.constData()), unowned.size(), false, m_sortAlpha, m_showIDs);
}

void GearSubTab::saveEquipped() {
    if (m_equippedField)
        *m_equippedField = (uint32_t)etab::comboId(m_equipped);
}

void GearSubTab::onListSelectionChanged() {
    auto sel = m_list->selectedItems();
    if (sel.isEmpty()) return;
    int arrayIdx = sel.first()->data(Qt::UserRole).toInt();
    OwnedGearS1& gear = m_array[arrayIdx];
    etab::setComboById(m_sub1, (int32_t)(uint32_t)gear.sub1);
    etab::setComboById(m_sub2, (int32_t)(uint32_t)gear.sub2);
    etab::setComboById(m_sub3, (int32_t)(uint32_t)gear.sub3);
    m_level->setText(QString::number((int32_t)(uint32_t)gear.level));
    m_slots->setText(QString::number((int32_t)(uint32_t)gear.unlockedSlots));
    m_exp->setText(QString::number((int32_t)(uint32_t)gear.exp));
    m_newFlag->setChecked(gear.newFlag == 1);
}

void GearSubTab::onApply() {
    auto sel = m_list->selectedItems();
    if (sel.isEmpty()) return;
    for (auto* it : sel) {
        int arrayIdx = it->data(Qt::UserRole).toInt();
        OwnedGearS1& gear = m_array[arrayIdx];
        gear.sub1 = (uint32_t)etab::comboId(m_sub1);
        gear.sub2 = (uint32_t)etab::comboId(m_sub2);
        gear.sub3 = (uint32_t)etab::comboId(m_sub3);
        int32_t lvl = m_level->text().toInt(); if (lvl < 1) lvl = 1; if (lvl > 4) lvl = 4; gear.level = (uint32_t)lvl;
        int32_t sl = m_slots->text().toInt(); if (sl < 1) sl = 1; if (sl > 4) sl = 4; gear.unlockedSlots = (uint32_t)sl;
        int32_t exp = m_exp->text().toInt(); gear.exp = (uint32_t)exp;
        gear.newFlag = m_newFlag->isChecked() ? 1 : 0;
    }
    refreshList();
    refreshAddPicker();
    QMessageBox::information(this, tr("Success"),
        sel.size() == 1 ? tr("Gear stats applied!") : tr("Gear stats applied to multiple items!"));
}

void GearSubTab::onAdd() {
    int gearID = etab::comboId(m_addPicker);
    if (gearID < 0) return;
    int emptyIdx = -1;
    for (int i = 0; i < 256; ++i)
        if ((int32_t)(uint32_t)m_array[i].id == -1) { emptyIdx = i; break; }
    if (emptyIdx == -1) {
        QMessageBox::warning(this, tr("Error"), tr("Gear inventory is full (256 items)."));
        return;
    }
    m_array[emptyIdx].id = (uint32_t)gearID;
    m_array[emptyIdx].level = 1;
    m_array[emptyIdx].unlockedSlots = 1;
    m_array[emptyIdx].sub1 = 0;
    m_array[emptyIdx].sub2 = 0;
    m_array[emptyIdx].sub3 = 0;
    m_array[emptyIdx].exp = 0;
    m_array[emptyIdx].newFlag = 1;
    refreshList();
    refreshAddPicker();
    refreshEquippedPicker();
}

void GearSubTab::onDelete() {
    auto sel = m_list->selectedItems();
    if (sel.isEmpty()) return;
    if (QMessageBox::question(this, tr("Confirm"), tr("Delete selected gear item(s)?")) != QMessageBox::Yes)
        return;
    for (auto* it : sel) {
        int arrayIdx = it->data(Qt::UserRole).toInt();
        m_array[arrayIdx].id = 0xFFFFFFFF;
    }
    refreshList();
    refreshAddPicker();
    refreshEquippedPicker();
}

GearTab::GearTab(QWidget* parent) : EditorTabBase(parent) {
    m_subTabs = new QTabWidget(this);
    auto* root = new QVBoxLayout(this);
    root->addWidget(m_subTabs);
    m_head = new GearSubTab(this);
    m_clothes = new GearSubTab(this);
    m_shoes = new GearSubTab(this);
    m_subTabs->addTab(m_head, tr("Head"));
    m_subTabs->addTab(m_clothes, tr("Clothes"));
    m_subTabs->addTab(m_shoes, tr("Shoes"));
}

void GearTab::loadData(SaveData* data) {
    m_data = data;
    if (m_data) {
        m_head->bind(m_data, m_data->head, const_cast<const wchar_t**>(g_HeadNames), g_HeadCount, &m_data->currentHeadGear, m_showIDs, m_sortAlpha);
        m_clothes->bind(m_data, m_data->clothes, const_cast<const wchar_t**>(g_ClothNames), g_ClothCount, &m_data->currentClothGear, m_showIDs, m_sortAlpha);
        m_shoes->bind(m_data, m_data->shoes, const_cast<const wchar_t**>(g_ShoeNames), g_ShoeCount, &m_data->currentShoeGear, m_showIDs, m_sortAlpha);
    }
}

void GearTab::saveData(SaveData* data) {
    if (!data) return;
    m_head->saveEquipped();
    m_clothes->saveEquipped();
    m_shoes->saveEquipped();
}

void GearTab::refreshDisplay() {
    if (m_data) {
        m_head->refreshDisplay(m_showIDs, m_sortAlpha);
        m_clothes->refreshDisplay(m_showIDs, m_sortAlpha);
        m_shoes->refreshDisplay(m_showIDs, m_sortAlpha);
    }
}
