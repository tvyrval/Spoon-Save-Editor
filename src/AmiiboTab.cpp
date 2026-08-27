#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "ItemData.h"
#include "save_data.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>

static QWidget* buildChallengePage(QVector<QComboBox*>& combos, QWidget* parent) {
    auto* page = new QWidget(parent);
    auto* box = new QGroupBox(QStringLiteral("Amiibo Challenges"), page);
    auto* grid = new QGridLayout(box);
    for (int j = 0; j < 20; ++j) {
        int col = (j < 10) ? 0 : 1;
        int row = (j < 10) ? j : j - 10;
        auto* cb = new QComboBox(box);
        grid->addWidget(new QLabel(QStringLiteral("Challenge %1:").arg(j + 1)), row * 2, col);
        grid->addWidget(cb, row * 2 + 1, col);
        combos.append(cb);
    }
    auto* l = new QVBoxLayout(page);
    l->addWidget(box);
    return page;
}

AmiiboTab::AmiiboTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QVBoxLayout(this);
    m_subTabs = new QTabWidget(this);
    root->addWidget(m_subTabs);

    m_subTabs->addTab(buildChallengePage(m_girl, this), tr("Inkling Girl"));
    m_subTabs->addTab(buildChallengePage(m_boy, this), tr("Inkling Boy"));
    m_subTabs->addTab(buildChallengePage(m_squid, this), tr("Inkling Squid"));

    m_unlockAll = new QPushButton(tr("Unlock All (Finished)"), this);
    root->addWidget(m_unlockAll);

    connect(m_unlockAll, &QPushButton::clicked, this, &AmiiboTab::onUnlockAll);

    refreshDisplay();
}

void AmiiboTab::refreshDisplay() {
    for (const auto& cb : {m_girl, m_boy, m_squid}) {
        for (QComboBox* c : cb) {
            etab::fillItemCombo(c, const_cast<const wchar_t**>(g_ChallengeStates),
                                g_ChallengeStateCount, false, false, m_showIDs);
        }
    }
    if (m_data) {
        for (int i = 0; i < 20; ++i) {
            etab::setComboById(m_girl[i], (int32_t)(uint32_t)m_data->girlChallenges[i]);
            etab::setComboById(m_boy[i], (int32_t)(uint32_t)m_data->boyChallenges[i]);
            etab::setComboById(m_squid[i], (int32_t)(uint32_t)m_data->squidChallenges[i]);
        }
    }
}

void AmiiboTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    for (int i = 0; i < 20; ++i) {
        etab::setComboById(m_girl[i], (int32_t)(uint32_t)data->girlChallenges[i]);
        etab::setComboById(m_boy[i], (int32_t)(uint32_t)data->boyChallenges[i]);
        etab::setComboById(m_squid[i], (int32_t)(uint32_t)data->squidChallenges[i]);
    }
}

void AmiiboTab::saveData(SaveData* data) {
    if (!data) return;
    for (int i = 0; i < 20; ++i) {
        data->girlChallenges[i] = (uint32_t)etab::comboId(m_girl[i]);
        data->boyChallenges[i] = (uint32_t)etab::comboId(m_boy[i]);
        data->squidChallenges[i] = (uint32_t)etab::comboId(m_squid[i]);
    }
}

void AmiiboTab::onUnlockAll() {
    if (!m_data) return;
    for (int i = 0; i < 20; ++i) {
        m_data->girlChallenges[i] = 4;
        m_data->boyChallenges[i] = 4;
        m_data->squidChallenges[i] = 4;
    }
    loadData(m_data);
    QMessageBox::information(this, tr("Success"), tr("All Amiibo Challenges set to Finished."));
}
