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
#include <cstring>

PlazaTab::PlazaTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QHBoxLayout(this);

    auto* listBox = new QGroupBox(tr("Plaza Players"), this);
    auto* lb = new QVBoxLayout(listBox);
    m_list = new QListWidget;
    lb->addWidget(m_list);
    root->addWidget(listBox);

    auto* detailBox = new QGroupBox(tr("Player Details"), this);
    auto* df = new QFormLayout(detailBox);
    m_name = new QLineEdit;
    m_level = new QLineEdit; m_level->setMaxLength(4);
    m_rank = new QComboBox;
    m_turf = new QLineEdit; m_turf->setMaxLength(7);
    m_gender = new QComboBox;
    m_skin = new QComboBox;
    m_eye = new QComboBox;
    df->addRow(tr("Name:"), m_name);
    df->addRow(tr("Level:"), m_level);
    df->addRow(tr("Rank:"), m_rank);
    df->addRow(tr("Turf Inked:"), m_turf);
    df->addRow(tr("Gender:"), m_gender);
    df->addRow(tr("Skin Color:"), m_skin);
    df->addRow(tr("Eye Color:"), m_eye);
    root->addWidget(detailBox);

    auto* equipBox = new QGroupBox(tr("Equipped Layout"), this);
    auto* ef = new QFormLayout(equipBox);
    m_head = new QComboBox;
    m_cloth = new QComboBox;
    m_shoe = new QComboBox;
    m_weapon = new QComboBox;
    ef->addRow(tr("Hat:"), m_head);
    ef->addRow(tr("Clothes:"), m_cloth);
    ef->addRow(tr("Shoes:"), m_shoe);
    ef->addRow(tr("Weapon:"), m_weapon);
    m_fillFake = new QPushButton(tr("Fill Plaza with Fake Players"));
    ef->addRow(QString(), m_fillFake);
    root->addWidget(equipBox);

    connect(m_list, &QListWidget::currentRowChanged, this, &PlazaTab::onPlazaSelectionChanged);
    connect(m_fillFake, &QPushButton::clicked, this, &PlazaTab::onFillFake);
}

static QString inklingName(const PlazaInkling* ink) {
    QString s;
    for (int i = 0; i < 8; ++i) {
        if (ink->name[i] == 0) break;
        s.append(QChar(static_cast<ushort>(ink->name[i])));
    }
    return s;
}

void PlazaTab::loadInkling(int index) {
    if (!m_data || index < 0 || index >= 25) return;
    PlazaInkling* ink = &m_data->plazaInklings[index];

    if ((int32_t)(uint32_t)ink->status == -1) {
        m_name->setText(tr("[Fake Player]"));
        m_level->setText(QStringLiteral("0"));
        etab::setComboById(m_rank, 0);
        m_turf->setText(QStringLiteral("0"));
        etab::setComboById(m_gender, 0);
        etab::setComboById(m_skin, 0);
        etab::setComboById(m_eye, 0);
        etab::setComboById(m_head, 0);
        etab::setComboById(m_cloth, 0);
        etab::setComboById(m_shoe, 0);
        etab::setComboById(m_weapon, 1000);
        return;
    }

    m_name->setText(inklingName(ink));
    if ((int32_t)(uint32_t)ink->level == -1)
        m_level->setText(QStringLiteral("-1"));
    else
        m_level->setText(QString::number((int32_t)(uint32_t)ink->level + 1));
    etab::setComboById(m_rank, (int32_t)(uint32_t)ink->rank);
    m_turf->setText(QString::number((int32_t)(uint32_t)ink->turfInked));
    etab::setComboById(m_gender, (int32_t)(uint32_t)ink->gender);
    etab::setComboById(m_skin, (int32_t)(uint32_t)ink->skinColor);
    etab::setComboById(m_eye, (int32_t)(uint32_t)ink->eyeColor);
    etab::setComboById(m_head, (int32_t)(uint32_t)ink->headGear);
    etab::setComboById(m_cloth, (int32_t)(uint32_t)ink->clothGear);
    etab::setComboById(m_shoe, (int32_t)(uint32_t)ink->shoeGear);
    etab::setComboById(m_weapon, (int32_t)(uint32_t)ink->weapon);
}

void PlazaTab::saveInkling(int index) {
    if (!m_data || index < 0 || index >= 25) return;
    PlazaInkling* ink = &m_data->plazaInklings[index];
    if ((int32_t)(uint32_t)ink->status == -1) return;

    QString name = m_name->text();
    for (int i = 0; i < 8; ++i)
        ink->name[i] = (i < name.size()) ? static_cast<char16_t>(name[i].unicode()) : 0;

    int32_t v = m_level->text().toInt();
    if (v == -1) {
        ink->level = 0xFFFFFFFFu;
    } else {
        if (v < 1) v = 1; if (v > 999) v = 999;
        ink->level = (uint32_t)(v - 1);
    }

    ink->rank = (uint32_t)etab::comboId(m_rank);
    int32_t turf = m_turf->text().toInt();
    if (turf != -1) { if (turf > 999999) turf = 999999; if (turf < 0) turf = 0; }
    ink->turfInked = (uint32_t)turf;
    ink->gender = (uint32_t)etab::comboId(m_gender);
    ink->skinColor = (uint32_t)etab::comboId(m_skin);
    ink->eyeColor = (uint32_t)etab::comboId(m_eye);
    ink->headGear = (uint32_t)etab::comboId(m_head);
    ink->clothGear = (uint32_t)etab::comboId(m_cloth);
    ink->shoeGear = (uint32_t)etab::comboId(m_shoe);
    ink->weapon = (uint32_t)etab::comboId(m_weapon);
}

void PlazaTab::refreshDisplay() {
    etab::fillPlazaGearCombo(m_rank, const_cast<const wchar_t**>(g_PlazaRankNames), g_PlazaRankCount, m_sortAlpha, m_showIDs);
    etab::fillPlazaGearCombo(m_gender, const_cast<const wchar_t**>(g_GenderNames), g_GenderCount, m_sortAlpha, m_showIDs);
    etab::fillPlazaGearCombo(m_skin, const_cast<const wchar_t**>(g_SkinColorNames), 8, false, m_showIDs);
    etab::fillPlazaGearCombo(m_eye, const_cast<const wchar_t**>(g_EyeColorNames), g_EyeColorCount, m_sortAlpha, m_showIDs);
    etab::fillPlazaGearCombo(m_head, const_cast<const wchar_t**>(g_HeadNames), g_HeadCount, m_sortAlpha, m_showIDs);
    etab::fillPlazaGearCombo(m_cloth, const_cast<const wchar_t**>(g_ClothNames), g_ClothCount, m_sortAlpha, m_showIDs);
    etab::fillPlazaGearCombo(m_shoe, const_cast<const wchar_t**>(g_ShoeNames), g_ShoeCount, m_sortAlpha, m_showIDs);
    etab::fillPlazaGearCombo(m_weapon, const_cast<const wchar_t**>(g_WeaponNames), g_WeaponCount, m_sortAlpha, m_showIDs);
    if (m_data) loadInkling(m_current);
}

void PlazaTab::loadData(SaveData* data) {
    m_data = data;
    m_list->blockSignals(true);
    m_list->clear();
    if (!data) { m_list->blockSignals(false); return; }
    for (int i = 0; i < 25; ++i) {
        PlazaInkling* ink = &data->plazaInklings[i];
        if ((int32_t)(uint32_t)ink->status == -1) {
            m_list->addItem(tr("[Fake Player]"));
        } else {
            QString n = inklingName(ink);
            if (n.isEmpty()) n = tr("[No Name]");
            m_list->addItem(n);
        }
    }
    m_current = 0;
    m_list->setCurrentRow(0);
    m_list->blockSignals(false);
    loadInkling(m_current);
}

void PlazaTab::saveData(SaveData* data) {
    if (!data) return;
    saveInkling(m_current);
}

void PlazaTab::onPlazaSelectionChanged(int row) {
    if (!m_data || row < 0) return;
    saveInkling(m_current);
    m_current = row;
    loadInkling(m_current);
}

void PlazaTab::onFillFake() {
    if (!m_data) return;
    for (int i = 0; i < 25; ++i) {
        std::memset(&m_data->plazaInklings[i], 0, sizeof(PlazaInkling));
        m_data->plazaInklings[i].status = 0xFFFFFFFFu;
    }
    loadData(m_data);
    QMessageBox::information(this, tr("Spoon Editor"), tr("The Plaza has been filled with fake players."));
}
