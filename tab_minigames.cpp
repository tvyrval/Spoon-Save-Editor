#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>

static HWND hMinigameTab;

void InitMinigameTab(HWND hParent) {
    hMinigameTab = hParent;

    CreateWindowW(L"BUTTON", L"Minigame Unlocks", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 10, 5, 230, 130, hMinigameTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"High Scores", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 260, 5, 340, 130, hMinigameTab, NULL, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"BUTTON", L"Squid Jump Unlocked", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP, 25, 25, 200, 20, hMinigameTab, (HMENU)IDC_M_SJ_LOCK, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Squid Ball Unlocked", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP, 25, 50, 200, 20, hMinigameTab, (HMENU)IDC_M_SB_LOCK, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Squid Racer Unlocked", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP, 25, 75, 200, 20, hMinigameTab, (HMENU)IDC_M_SR_LOCK, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Squid Beatz Unlocked", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP, 25, 100, 200, 20, hMinigameTab, (HMENU)IDC_M_SBZ_LOCK, GetModuleHandle(NULL), NULL);

    LayoutControls(hMinigameTab, 275, 25, L"Squid Jump High:", IDC_M_SJ_SCORE);
    HWND hSj = GetDlgItem(hMinigameTab, IDC_M_SJ_SCORE);
    SendMessageW(hSj, EM_SETLIMITTEXT, 4, 0);
    SetWindowLongW(hSj, GWL_STYLE, GetWindowLongW(hSj, GWL_STYLE) | ES_NUMBER);

    LayoutControls(hMinigameTab, 275, 55, L"Squid Ball High:", IDC_M_SB_SCORE);
    HWND hSb = GetDlgItem(hMinigameTab, IDC_M_SB_SCORE);
    SendMessageW(hSb, EM_SETLIMITTEXT, 4, 0);
    SetWindowLongW(hSb, GWL_STYLE, GetWindowLongW(hSb, GWL_STYLE) | ES_NUMBER);

    LayoutControls(hMinigameTab, 275, 85, L"Squid Racer High:", IDC_M_SR_SCORE);
    HWND hSr = GetDlgItem(hMinigameTab, IDC_M_SR_SCORE);
    SendMessageW(hSr, EM_SETLIMITTEXT, 4, 0);
    SetWindowLongW(hSr, GWL_STYLE, GetWindowLongW(hSr, GWL_STYLE) | ES_NUMBER);

    if (g_hFont) {
        EnumChildWindows(hMinigameTab, [](HWND hWnd, LPARAM lp) -> BOOL {
            SendMessageW(hWnd, WM_SETFONT, (WPARAM)lp, TRUE);
            return TRUE;
            }, (LPARAM)g_hFont);
    }
}

void UpdateMinigameTabUI(SaveData* data) {
    uint32_t ptt = (uint32_t)data->minigameUnlocks;
    
    CheckDlgButton(hMinigameTab, IDC_M_SJ_LOCK, (ptt & 0x010000) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hMinigameTab, IDC_M_SB_LOCK, (ptt & 0x020000) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hMinigameTab, IDC_M_SR_LOCK, (ptt & 0x040000) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hMinigameTab, IDC_M_SBZ_LOCK, (ptt & 0x080000) ? BST_CHECKED : BST_UNCHECKED);

    WriteInt32(hMinigameTab, IDC_M_SJ_SCORE, data->squidJumpScore);
    WriteInt32(hMinigameTab, IDC_M_SB_SCORE, data->squidballScore);
    WriteInt32(hMinigameTab, IDC_M_SR_SCORE, data->squidRacerScore);
}

void SaveMinigameTabData(SaveData* data) {
    uint32_t ptt = (uint32_t)data->minigameUnlocks;

    auto updateBit = [&](int id, uint32_t bit) {
        if (IsDlgButtonChecked(hMinigameTab, id) == BST_CHECKED) ptt |= bit;
        else ptt &= ~bit;
    };

    updateBit(IDC_M_SJ_LOCK, 0x010000);
    updateBit(IDC_M_SB_LOCK, 0x020000);
    updateBit(IDC_M_SR_LOCK, 0x040000);
    updateBit(IDC_M_SBZ_LOCK, 0x080000);

    data->minigameUnlocks = ptt;

    ReadInt32(hMinigameTab, IDC_M_SJ_SCORE, data->squidJumpScore);
    if ((int32_t)data->squidJumpScore > 9999) data->squidJumpScore = 9999;
    if ((int32_t)data->squidJumpScore < 0) data->squidJumpScore = 0;

    ReadInt32(hMinigameTab, IDC_M_SB_SCORE, data->squidballScore);
    if ((int32_t)data->squidballScore > 9999) data->squidballScore = 9999;
    if ((int32_t)data->squidballScore < 0) data->squidballScore = 0;

    ReadInt32(hMinigameTab, IDC_M_SR_SCORE, data->squidRacerScore);
    if ((int32_t)data->squidRacerScore > 9999) data->squidRacerScore = 9999;
    if ((int32_t)data->squidRacerScore < 0) data->squidRacerScore = 0;
}
