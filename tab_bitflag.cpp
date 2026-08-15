#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>

static HWND hBitFlagTab;

void InitBitFlagTab(HWND hParent) {
    hBitFlagTab = hParent;

    CreateWindowW(L"BUTTON", L"Story Mode && Multiplayer Flags", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 10, 5, 630, 355, hBitFlagTab, NULL, GetModuleHandle(NULL), NULL);

    int x = 20, y = 20;

    LayoutCheckbox(hBitFlagTab, x, y, L"Finished tutorial, seen UFO news and Plaza explanation", IDC_B_FLAG_1); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Seen Splatfest explanation", IDC_B_FLAG_2); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Seen Ranked Battle available news", IDC_B_FLAG_3); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Seen Lobby explanation", IDC_B_FLAG_4); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Seen Octo Valley intro cutscene and given Hero Suit", IDC_B_FLAG_5); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Great Zapfish returned and DJ Octavio's snowglobe is shown", IDC_B_FLAG_6); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Cap'n Cuttlefish's post-game dialogue shown, credits block available", IDC_B_FLAG_7); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Ranked Battles unlocked", IDC_B_FLAG_8); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Rank shown in Plaza and Lobby", IDC_B_FLAG_9); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Super Sea Snail counter shown in Plaza and Lobby", IDC_B_FLAG_10); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Level cap raised to 50", IDC_B_FLAG_11); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Seen warning about game stats being published online", IDC_B_FLAG_12); y += 24;

    LayoutCheckbox(hBitFlagTab, x, y, L"Seen Spyke explanation", IDC_B_FLAG_13); y += 24;
    LayoutCheckbox(hBitFlagTab, x, y, L"Seen Spyke explanation about Super Sea Snails", IDC_B_FLAG_14);

    if (g_hFont) {
        EnumChildWindows(hBitFlagTab, [](HWND hWnd, LPARAM lp) -> BOOL {
            SendMessageW(hWnd, WM_SETFONT, (WPARAM)lp, TRUE);
            return TRUE;
            }, (LPARAM)g_hFont);
    }
}

void UpdateBitFlagTabUI(SaveData* data) {
    uint32_t itb = (uint32_t)data->gameStateFlags;
    uint32_t ttv = (uint32_t)data->talkedToSpyke;
    uint32_t vto = (uint32_t)data->spykeSSS;

    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_1, (itb & GAME_FLAG_TUTORIAL) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_2, (itb & GAME_FLAG_SPLATFEST) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_3, (itb & GAME_FLAG_RANKEDNEWS) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_4, (itb & GAME_FLAG_LOBBY) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_5, (itb & GAME_FLAG_HEROSUIT) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_6, (itb & GAME_FLAG_GREATZAPFISH) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_7, (itb & GAME_FLAG_CUTTLEFISH) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_8, (itb & GAME_FLAG_RANKED) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_9, (itb & GAME_FLAG_RANKSHOWN) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_10, (itb & GAME_FLAG_SNAILS) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_11, (itb & GAME_FLAG_LEVELCAP) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_12, (itb & GAME_FLAG_WARNING) ? BST_CHECKED : BST_UNCHECKED);

    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_13, (ttv > 0) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hBitFlagTab, IDC_B_FLAG_14, (vto & SPYKE_FLAG_SSS) ? BST_CHECKED : BST_UNCHECKED);
}

static void UpdateFlag(uint32_t& val, uint32_t mask, int idc) {
    if (IsDlgButtonChecked(hBitFlagTab, idc) == BST_CHECKED) val |= mask;
    else val &= ~mask;
}

void SaveBitFlagTabData(SaveData* data) {
    uint32_t itb = (uint32_t)data->gameStateFlags;
    uint32_t ttv = (uint32_t)data->talkedToSpyke;
    uint32_t vto = (uint32_t)data->spykeSSS;

    UpdateFlag(itb, GAME_FLAG_TUTORIAL, IDC_B_FLAG_1);
    UpdateFlag(itb, GAME_FLAG_SPLATFEST, IDC_B_FLAG_2);
    UpdateFlag(itb, GAME_FLAG_RANKEDNEWS, IDC_B_FLAG_3);
    UpdateFlag(itb, GAME_FLAG_LOBBY, IDC_B_FLAG_4);
    UpdateFlag(itb, GAME_FLAG_HEROSUIT, IDC_B_FLAG_5);
    UpdateFlag(itb, GAME_FLAG_GREATZAPFISH, IDC_B_FLAG_6);
    UpdateFlag(itb, GAME_FLAG_CUTTLEFISH, IDC_B_FLAG_7);
    UpdateFlag(itb, GAME_FLAG_RANKED, IDC_B_FLAG_8);
    UpdateFlag(itb, GAME_FLAG_RANKSHOWN, IDC_B_FLAG_9);
    UpdateFlag(itb, GAME_FLAG_SNAILS, IDC_B_FLAG_10);
    UpdateFlag(itb, GAME_FLAG_LEVELCAP, IDC_B_FLAG_11);
    UpdateFlag(itb, GAME_FLAG_WARNING, IDC_B_FLAG_12);

    UpdateFlag(ttv, SPYKE_FLAG_TALKED, IDC_B_FLAG_13);
    UpdateFlag(vto, SPYKE_FLAG_SSS, IDC_B_FLAG_14);

    data->gameStateFlags = itb;
    data->talkedToSpyke = ttv;
    data->spykeSSS = vto;
}
