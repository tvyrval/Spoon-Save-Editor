#include "EditorTabs.h"
#include "EditorTabUtils.h"
#include "save_data.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>

static const char* const kFlagLabels[] = {
    "Finished tutorial, seen UFO news and Plaza explanation",
    "Seen Splatfest explanation",
    "Seen Ranked Battle available news",
    "Seen Lobby explanation",
    "Seen Octo Valley intro cutscene and given Hero Suit",
    "Great Zapfish returned and DJ Octavio's snowglobe is shown",
    "Cap'n Cuttlefish's post-game dialogue shown, credits block available",
    "Ranked Battles unlocked",
    "Rank shown in Plaza and Lobby",
    "Super Sea Snail counter shown in Plaza and Lobby",
    "Level cap raised to 50",
    "Seen warning about game stats being published online",
    "Seen Spyke explanation",
    "Seen Spyke explanation about Super Sea Snails"
};

static const uint32_t kGameMasks[12] = {
    GAME_FLAG_TUTORIAL, GAME_FLAG_SPLATFEST, GAME_FLAG_RANKEDNEWS, GAME_FLAG_LOBBY,
    GAME_FLAG_HEROSUIT, GAME_FLAG_GREATZAPFISH, GAME_FLAG_CUTTLEFISH, GAME_FLAG_RANKED,
    GAME_FLAG_RANKSHOWN, GAME_FLAG_SNAILS, GAME_FLAG_LEVELCAP, GAME_FLAG_WARNING
};

BitFlagTab::BitFlagTab(QWidget* parent) : EditorTabBase(parent) {
    auto* root = new QVBoxLayout(this);
    auto* box = new QGroupBox(tr("Story Mode && Multiplayer Flags"), this);
    auto* bl = new QVBoxLayout(box);
    for (int i = 0; i < 14; ++i) {
        m_flags[i] = new QCheckBox(QString::fromLatin1(kFlagLabels[i]), box);
        bl->addWidget(m_flags[i]);
    }
    root->addWidget(box);
    root->addStretch(1);
}

void BitFlagTab::loadData(SaveData* data) {
    m_data = data;
    if (!data) return;
    uint32_t itb = (uint32_t)data->gameStateFlags;
    for (int i = 0; i < 12; ++i)
        m_flags[i]->setChecked(itb & kGameMasks[i]);
    m_flags[12]->setChecked((uint32_t)data->talkedToSpyke != 0);
    m_flags[13]->setChecked((uint32_t)data->spykeSSS & SPYKE_FLAG_SSS);
}

void BitFlagTab::saveData(SaveData* data) {
    if (!data) return;
    uint32_t itb = (uint32_t)data->gameStateFlags;
    for (int i = 0; i < 12; ++i) {
        if (m_flags[i]->isChecked()) itb |= kGameMasks[i];
        else itb &= ~kGameMasks[i];
    }
    data->gameStateFlags = itb;

    uint32_t ttv = (uint32_t)data->talkedToSpyke;
    if (m_flags[12]->isChecked()) ttv |= SPYKE_FLAG_TALKED;
    else ttv &= ~SPYKE_FLAG_TALKED;
    data->talkedToSpyke = ttv;

    uint32_t vto = (uint32_t)data->spykeSSS;
    if (m_flags[13]->isChecked()) vto |= SPYKE_FLAG_SSS;
    else vto &= ~SPYKE_FLAG_SSS;
    data->spykeSSS = vto;
}
