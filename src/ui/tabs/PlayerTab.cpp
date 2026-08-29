#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "ItemData.h"
#include "SaveManager.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <cmath>
#include <cstring>

PlayerTab::PlayerTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QVBoxLayout(this);
    auto* tabs = new QTabWidget(this);
    root->addWidget(tabs);

    auto* stats = new QWidget;
    auto* sBox = new QGroupBox(tr("Player Statistics"), stats);
    auto* sg = new QGridLayout(sBox);
    m_level = new QLineEdit; m_level->setMaxLength(4);
    m_levelPts = new QLineEdit; m_levelPts->setMaxLength(4);
    m_rank = new QComboBox;
    m_rankPts = new QLineEdit; m_rankPts->setMaxLength(4);
    m_money = new QLineEdit; m_money->setMaxLength(7);
    m_snails = new QLineEdit; m_snails->setMaxLength(3);
    m_newsVersion = new QLineEdit; m_newsVersion->setMaxLength(3);
    sg->addWidget(new QLabel(tr("Level:")), 0, 0); sg->addWidget(m_level, 0, 1); sg->addWidget(new QLabel(tr("Level Points:")), 0, 2); sg->addWidget(m_levelPts, 0, 3);
    sg->addWidget(new QLabel(tr("Rank:")), 1, 0); sg->addWidget(m_rank, 1, 1); sg->addWidget(new QLabel(tr("Rank Points:")), 1, 2); sg->addWidget(m_rankPts, 1, 3);
    sg->addWidget(new QLabel(tr("Money:")), 2, 0); sg->addWidget(m_money, 2, 1); sg->addWidget(new QLabel(tr("Sea Snails:")), 2, 2); sg->addWidget(m_snails, 2, 3);
    sg->addWidget(new QLabel(tr("Update Version:")), 3, 0); sg->addWidget(m_newsVersion, 3, 1);
    auto* sLay = new QVBoxLayout(stats); sLay->addWidget(sBox); sLay->addStretch(1);
    tabs->addTab(stats, tr("Stats"));

    auto* appear = new QWidget;
    auto* aBox = new QGroupBox(tr("Player Appearance"), appear);
    auto* af = new QFormLayout(aBox);
    m_gender = new QComboBox;
    m_skin = new QComboBox;
    m_eye = new QComboBox;
    af->addRow(tr("Gender:"), m_gender);
    af->addRow(tr("Skin Color:"), m_skin);
    af->addRow(tr("Eye Color:"), m_eye);
    auto* aLay = new QVBoxLayout(appear); aLay->addWidget(aBox); aLay->addStretch(1);
    tabs->addTab(appear, tr("Appearance"));

    auto* sfest = new QWidget;
    auto* sfBox = new QGroupBox(tr("Splatfest Data"), sfest);
    auto* sfg = new QGridLayout(sfBox);
    m_sfId = new QLineEdit; m_sfId->setMaxLength(4);
    m_sfTeam = new QComboBox;
    m_sfPower = new QLineEdit;
    m_sfXp = new QLineEdit;
    m_sfRank = new QComboBox;
    sfg->addWidget(new QLabel(tr("Splatfest ID:")), 0, 0); sfg->addWidget(m_sfId, 0, 1); sfg->addWidget(new QLabel(tr("Splatfest Team:")), 0, 2); sfg->addWidget(m_sfTeam, 0, 3);
    sfg->addWidget(new QLabel(tr("Splatfest Title:")), 1, 0); sfg->addWidget(m_sfRank, 1, 1); sfg->addWidget(new QLabel(tr("Splatfest XP:")), 1, 2); sfg->addWidget(m_sfXp, 1, 3);
    sfg->addWidget(new QLabel(tr("Splatfest Power:")), 2, 0); sfg->addWidget(m_sfPower, 2, 1);
    auto* sfLay = new QVBoxLayout(sfest); sfLay->addWidget(sfBox); sfLay->addStretch(1);
    tabs->addTab(sfest, tr("Splatfest"));

    auto* ctrl = new QWidget;
    auto* cBox = new QGroupBox(tr("Control Options"), ctrl);
    auto* cl = new QVBoxLayout(cBox);
    m_colorLock = new QCheckBox(tr("Color Lock"));
    m_revY = new QCheckBox(tr("Reverse Y-Axis"));
    m_revX = new QCheckBox(tr("Reverse X-Axis"));
    m_motion = new QCheckBox(tr("Motion Controls"));
    cl->addWidget(m_colorLock);
    cl->addWidget(m_revY);
    cl->addWidget(m_revX);
    cl->addWidget(m_motion);
    auto* miivRow = new QFormLayout;
    m_miiverse = new QComboBox;
    miivRow->addRow(tr("Miiverse Display:"), m_miiverse);
    cl->addLayout(miivRow);
    auto* camRow = new QFormLayout;
    m_camSens = new QLineEdit;
    camRow->addRow(tr("Camera Sensitivity (-5 to 5):"), m_camSens);
    cl->addLayout(camRow);
    cl->addStretch(1);
    auto* cLay = new QVBoxLayout(ctrl); cLay->addWidget(cBox);
    tabs->addTab(ctrl, tr("Control Settings"));
}

void PlayerTab::refreshDisplay() {
    etab::fillItemCombo(m_rank, const_cast<const wchar_t**>(g_RankNames), g_RankCount, false, m_sortAlpha, m_showIDs);
    etab::fillItemCombo(m_gender, const_cast<const wchar_t**>(g_GenderNames), g_GenderCount, false, m_sortAlpha, m_showIDs);
    etab::fillItemCombo(m_skin, const_cast<const wchar_t**>(g_SkinColorNames), 8, false, false, m_showIDs);
    etab::fillItemCombo(m_eye, const_cast<const wchar_t**>(g_EyeColorNames), g_EyeColorCount, false, m_sortAlpha, m_showIDs);
    etab::fillItemCombo(m_sfTeam, const_cast<const wchar_t**>(g_SplatfestTeamNames), g_SplatfestTeamCount, false, m_sortAlpha, m_showIDs);
    etab::fillItemCombo(m_sfRank, const_cast<const wchar_t**>(g_SplatfestRankNames), g_SplatfestRankCount, false, m_sortAlpha, m_showIDs);
    etab::fillItemCombo(m_miiverse, const_cast<const wchar_t**>(g_MiiverseNames), g_MiiverseCount, false, m_sortAlpha, m_showIDs);
    if (m_data) loadData(m_data);
}

void PlayerTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;

    if ((int32_t)(uint32_t)data->level == -1)
        m_level->setText(QStringLiteral("-1"));
    else
        m_level->setText(QString::number((int32_t)(uint32_t)data->level + 1));
    m_levelPts->setText(QString::number((int32_t)(uint32_t)data->levelPoints));
    etab::setComboById(m_rank, (int32_t)(uint32_t)data->rank);
    m_rankPts->setText(QString::number((int32_t)(uint32_t)data->rankPoints));
    m_money->setText(QString::number((int32_t)(uint32_t)data->money));
    m_snails->setText(QString::number((int32_t)(uint32_t)data->seaSnails));
    m_newsVersion->setText(QString::number((int32_t)(uint32_t)data->newsVersionSeen));

    etab::setComboById(m_gender, (int32_t)(uint32_t)data->genderID);
    etab::setComboById(m_skin, (int32_t)(uint32_t)data->skinColor);
    etab::setComboById(m_eye, (int32_t)(uint32_t)data->eyeColor);

    m_sfId->setText(QString::number((int32_t)(uint32_t)data->splatfestID));
    etab::setComboById(m_sfTeam, (int32_t)(uint32_t)data->splatfestTeamID);
    m_sfPower->setText(QString::number((int32_t)(uint32_t)data->splatfestPower));
    m_sfXp->setText(QString::number((int32_t)(uint32_t)data->splatfestXP));
    etab::setComboById(m_sfRank, (int32_t)(uint32_t)data->splatfestRank);

    etab::setComboById(m_miiverse, (int32_t)(uint32_t)data->miiverseDisplay);
    m_colorLock->setChecked(((uint32_t)data->miiverseDisplay & 0xFFu) == 1);
    m_revY->setChecked(data->reverseY == 1);
    m_revX->setChecked(data->reverseX == 1);
    m_motion->setChecked(data->motionControls == 1);

    uint32_t rawValue = (uint32_t)data->cameraSensRaw;
    float camFloat = 0.0f;
    std::memcpy(&camFloat, &rawValue, sizeof(float));
    int camDisplay = (int)std::round(camFloat * 5.0f);
    m_camSens->setText(QString::number(camDisplay));
}

void PlayerTab::saveData(SaveData* data) {
    if (!data) return;

    int32_t v = m_level->text().toInt();
    if (v == -1) {
        data->level = 0xFFFFFFFFu;
    } else {
        if (v < -128) v = -128;
        if (v > 127) v = 127;
        data->level = (uint32_t)(v - 1);
    }

    int32_t lp = m_levelPts->text().toInt();
    if (lp != -1) { if (lp > 999) lp = 999; if (lp < 0) lp = 0; }
    data->levelPoints = (uint32_t)lp;

    data->rank = (uint32_t)etab::comboId(m_rank);

    int32_t rp = m_rankPts->text().toInt();
    if (rp != -1) { if (rp > 999) rp = 999; if (rp < 0) rp = 0; }
    data->rankPoints = (uint32_t)rp;

    int32_t money = m_money->text().toInt();
    if (money > 9999999) money = 9999999; if (money < 0) money = 0;
    data->money = (uint32_t)money;

    int32_t snails = m_snails->text().toInt();
    if (snails > 999) snails = 999; if (snails < 0) snails = 0;
    data->seaSnails = (uint32_t)snails;

    int32_t nv = m_newsVersion->text().toInt();
    if (nv > 15) nv = 15; if (nv < 0) nv = 0;
    data->newsVersionSeen = (uint32_t)nv;

    data->genderID = (uint32_t)etab::comboId(m_gender);
    data->skinColor = (uint32_t)etab::comboId(m_skin);
    data->eyeColor = (uint32_t)etab::comboId(m_eye);

    int32_t sfId = m_sfId->text().toInt();
    if (sfId > 9999) sfId = 9999; if (sfId < 0) sfId = 0;
    data->splatfestID = (uint32_t)sfId;
    data->splatfestTeamID = (uint32_t)etab::comboId(m_sfTeam);
    data->splatfestPower = (uint32_t)m_sfPower->text().toInt();
    data->splatfestXP = (uint32_t)m_sfXp->text().toInt();
    data->splatfestRank = (uint32_t)etab::comboId(m_sfRank);

    uint32_t miiv = (uint32_t)etab::comboId(m_miiverse);
    uint8_t colorLock = m_colorLock->isChecked() ? 1u : 0u;
    miiv = (miiv & 0xFFFFFF00u) | colorLock;
    data->miiverseDisplay = miiv;

    data->reverseY = m_revY->isChecked() ? 1 : 0;
    data->reverseX = m_revX->isChecked() ? 1 : 0;
    data->motionControls = m_motion->isChecked() ? 1 : 0;

    float camDisplay = m_camSens->text().toFloat();
    if (camDisplay < -5.0f) camDisplay = -5.0f;
    if (camDisplay > 5.0f) camDisplay = 5.0f;
    float camFloat = camDisplay / 5.0f;
    uint32_t rawVal = 0;
    std::memcpy(&rawVal, &camFloat, sizeof(float));
    data->cameraSensRaw = rawVal;
}
