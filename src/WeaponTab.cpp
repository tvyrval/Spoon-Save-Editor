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
#include <QVBoxLayout>

static int weaponIdAt(int index) {
    if (index < 0 || index >= g_WeaponCount) return -1;
    return etab::parseItemID(etab::w2q(g_WeaponNames[index]));
}

WeaponTab::WeaponTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QVBoxLayout(this);

    auto* equippedBox = new QGroupBox(tr("Equipped Weapon"), this);
    auto* ebLay = new QHBoxLayout(equippedBox);
    ebLay->addWidget(new QLabel(tr("Equipped Weapon:"), equippedBox));
    m_equipped = new QComboBox(equippedBox);
    ebLay->addWidget(m_equipped, 1);
    root->addWidget(equippedBox);

    auto* mid = new QHBoxLayout;
    auto* invBox = new QGroupBox(tr("Weapon Inventory"), this);
    auto* invLay = new QVBoxLayout(invBox);
    invLay->addWidget(new QLabel(tr("Inventory (Turf Inked):"), invBox));
    m_list = new QListWidget(invBox);
    m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    invLay->addWidget(m_list);
    mid->addWidget(invBox);

    auto* editBox = new QGroupBox(tr("Edit Selected Attributes"), this);
    auto* editLay = new QVBoxLayout(editBox);
    auto* t1 = new QHBoxLayout; t1->addWidget(new QLabel(tr("Turf Inked:")));
    m_turf = new QLineEdit; m_turf->setMaxLength(6); t1->addWidget(m_turf); editLay->addLayout(t1);
    m_newFlag = new QCheckBox(tr("Mark as 'New'")); editLay->addWidget(m_newFlag);
    m_apply = new QPushButton(tr("Apply Stats")); editLay->addWidget(m_apply);
    mid->addWidget(editBox);
    root->addLayout(mid, 1);

    auto* addBox = new QGroupBox(tr("Add New Weapon"), this);
    auto* addLay = new QHBoxLayout(addBox);
    addLay->addWidget(new QLabel(tr("Weapon:")));
    m_addPicker = new QComboBox;
    addLay->addWidget(m_addPicker, 1);
    m_add = new QPushButton(tr("Add"));
    m_del = new QPushButton(tr("Delete"));
    addLay->addWidget(m_add);
    addLay->addWidget(m_del);
    root->addWidget(addBox);

    connect(m_list, &QListWidget::itemSelectionChanged, this, &WeaponTab::onListSelectionChanged);
    connect(m_apply, &QPushButton::clicked, this, &WeaponTab::onApply);
    connect(m_add, &QPushButton::clicked, this, &WeaponTab::onAdd);
    connect(m_del, &QPushButton::clicked, this, &WeaponTab::onDelete);
}

void WeaponTab::refreshDisplay() {
    refreshAddPicker();
    refreshOwnedList();
    refreshEquippedPicker();
}

void WeaponTab::refreshEquippedPicker() {
    m_equipped->clear();
    if (!m_data) return;
    struct Row { QString display; int id; };
    QVector<Row> rows;
    for (int i = 0; i < 128; ++i) {
        uint32_t id = m_data->ownedWeapons[i].id;
        if (id == 0 || id == 0xFFFFFFFF) continue;
        QString name = tr("Unknown Weapon");
        for (int k = 0; k < g_WeaponCount; ++k)
            if (weaponIdAt(k) == (int)id) { name = etab::w2q(g_WeaponNames[k]); break; }
        Row r;
        r.id = (int)id;
        r.display = etab::itemDisplay(name, m_showIDs);
        rows.append(r);
    }
    if (m_sortAlpha) {
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
            return a.display.compare(b.display, Qt::CaseInsensitive) < 0;
        });
    }
    for (const auto& r : rows)
        m_equipped->addItem(r.display, QVariant(r.id));
    etab::setComboById(m_equipped, m_data->currentWeapon);
}

void WeaponTab::refreshOwnedList() {
    m_list->clear();
    if (!m_data) return;

    struct Row { QString display; uint32_t turf; int idx; };
    QVector<Row> rows;
    for (int i = 0; i < 128; ++i) {
        uint32_t id = m_data->ownedWeapons[i].id;
        if (id == 0 || id == 0xFFFFFFFF) continue;
        Row r;
        r.idx = i;
        r.turf = m_data->ownedWeapons[i].turfInked;
        QString name = tr("Unknown Weapon");
        for (int k = 0; k < g_WeaponCount; ++k)
            if (weaponIdAt(k) == (int)id) { name = etab::w2q(g_WeaponNames[k]); break; }
        r.display = etab::itemDisplay(name, m_showIDs);
        rows.append(r);
    }
    if (m_sortAlpha) {
        std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
            return a.display.compare(b.display, Qt::CaseInsensitive) < 0;
        });
    }
    for (const auto& r : rows) {
        auto* item = new QListWidgetItem(QStringLiteral("%1 - %2pt").arg(r.display).arg((int32_t)r.turf));
        item->setData(Qt::UserRole, r.idx);
        m_list->addItem(item);
    }
}

void WeaponTab::refreshAddPicker() {
    if (!m_data) { m_addPicker->clear(); return; }
    QVector<const wchar_t*> unowned;
    for (int i = 0; i < g_WeaponCount; ++i) {
        int id = weaponIdAt(i);
        bool owned = false;
        for (int j = 0; j < 128; ++j)
            if (m_data->ownedWeapons[j].id == (uint32_t)id) { owned = true; break; }
        if (!owned) unowned.append(g_WeaponNames[i]);
    }
    etab::fillItemCombo(m_addPicker, const_cast<const wchar_t**>(unowned.constData()), unowned.size(), false, m_sortAlpha, m_showIDs);
}

void WeaponTab::loadData(SaveData* data) {
    m_data = data;
    refreshDisplay();
    if (m_data) etab::setComboById(m_equipped, (int32_t)(uint32_t)m_data->currentWeapon);
}

void WeaponTab::saveData(SaveData* data) {
    if (!data) return;
    data->currentWeapon = (uint32_t)etab::comboId(m_equipped);
}

void WeaponTab::onListSelectionChanged() {
    auto sel = m_list->selectedItems();
    if (sel.size() == 1) {
        int arrayIdx = sel.first()->data(Qt::UserRole).toInt();
        m_turf->setText(QString::number((int32_t)(uint32_t)m_data->ownedWeapons[arrayIdx].turfInked));
        m_newFlag->setChecked(m_data->ownedWeapons[arrayIdx].flags == 1);
    } else {
        m_turf->clear();
        m_newFlag->setChecked(false);
    }
}

void WeaponTab::onApply() {
    auto sel = m_list->selectedItems();
    if (sel.size() != 1) {
        QMessageBox::warning(this, tr("Error"), tr("Please select exactly one weapon to apply stats."));
        return;
    }
    int arrayIdx = sel.first()->data(Qt::UserRole).toInt();
    int32_t val = m_turf->text().toInt();
    if (val > 999999) val = 999999;
    if (val < -999999) val = -999999;
    m_data->ownedWeapons[arrayIdx].turfInked = (uint32_t)val;
    m_data->ownedWeapons[arrayIdx].flags = m_newFlag->isChecked() ? 1 : 0;
    refreshOwnedList();
    m_list->setCurrentRow(m_list->row(sel.first()));
    QMessageBox::information(this, tr("Success"), tr("Weapon stats applied!"));
}

void WeaponTab::onAdd() {
    int weaponID = etab::comboId(m_addPicker);
    if (weaponID < 0) return;
    int emptyIdx = -1;
    for (int i = 0; i < 128; ++i)
        if (m_data->ownedWeapons[i].id == 0 || m_data->ownedWeapons[i].id == 0xFFFFFFFF) { emptyIdx = i; break; }
    if (emptyIdx == -1) {
        QMessageBox::warning(this, tr("Error"), tr("Weapon list is full."));
        return;
    }
    m_data->ownedWeapons[emptyIdx].id = weaponID;
    m_data->ownedWeapons[emptyIdx].turfInked = 0;
    m_data->ownedWeapons[emptyIdx].flags = 1;
    refreshOwnedList();
    refreshAddPicker();
    refreshEquippedPicker();
}

void WeaponTab::onDelete() {
    auto sel = m_list->selectedItems();
    if (sel.isEmpty()) return;
    if (QMessageBox::question(this, tr("Confirm"),
            tr("Delete %1 selected weapon(s)?").arg(sel.size())) != QMessageBox::Yes)
        return;
    for (auto* it : sel) {
        int arrayIdx = it->data(Qt::UserRole).toInt();
        m_data->ownedWeapons[arrayIdx].id = 0xFFFFFFFF;
    }
    refreshOwnedList();
    refreshAddPicker();
    refreshEquippedPicker();
    m_turf->clear();
    m_newFlag->setChecked(false);
}
