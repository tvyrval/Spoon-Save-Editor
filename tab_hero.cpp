#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <string>

static HWND hHeroTab;
static HWND hStageList;
static int currentStageIndex = 0;

static const int g_HeroStageIDs[] = {
    1, 2, 3, 101, 4, 5, 6, 7, 8, 9, 102, 10, 11, 12, 13, 14, 15, 103, 16, 17, 18, 19, 20, 21, 104, 22, 23, 24, 25, 26, 27, 105
};

static const wchar_t* g_HeroStageNames[] = {
    L"Stage 1", L"Stage 2", L"Stage 3", L"Boss 1",
    L"Stage 4", L"Stage 5", L"Stage 6", L"Stage 7", L"Stage 8", L"Stage 9", L"Boss 2",
    L"Stage 10", L"Stage 11", L"Stage 12", L"Stage 13", L"Stage 14", L"Stage 15", L"Boss 3",
    L"Stage 16", L"Stage 17", L"Stage 18", L"Stage 19", L"Stage 20", L"Stage 21", L"Boss 4",
    L"Stage 22", L"Stage 23", L"Stage 24", L"Stage 25", L"Stage 26", L"Stage 27", L"Boss 5"
};

const wchar_t* g_BombNames[] = { L"0 (Splat Bomb)", L"1 (Burst Bomb)", L"2 (Seeker)" };
const wchar_t* g_HeroUpgradeNames[] = { L"-1 (Locked)", L"0 (Level 1)", L"1 (Level 2)", L"2 (Level 3)", L"3 (Level MAX)" };

static void LoadStageData(int index, SaveData* data) {
    if (!data || index < 0 || index >= 32) return;
    int id = g_HeroStageIDs[index];

    for (int i = 0; i < 64; i++) {
        if ((uint32_t)data->heroStages[i].stageID == id) {
            CheckDlgButton(hHeroTab, IDC_H_STAGE_ZAPFISH, data->heroStages[i].zapfish == 3 ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hHeroTab, IDC_H_STAGE_SCROLL, data->heroStages[i].scroll == 1 ? BST_CHECKED : BST_UNCHECKED);
            return;
        }
    }
    CheckDlgButton(hHeroTab, IDC_H_STAGE_ZAPFISH, BST_UNCHECKED);
    CheckDlgButton(hHeroTab, IDC_H_STAGE_SCROLL, BST_UNCHECKED);
}

static void SaveStageToData(int index, SaveData* data) {
    if (!data || index < 0 || index >= 32) return;
    int id = g_HeroStageIDs[index];

    bool zap = IsDlgButtonChecked(hHeroTab, IDC_H_STAGE_ZAPFISH) == BST_CHECKED;
    bool scroll = IsDlgButtonChecked(hHeroTab, IDC_H_STAGE_SCROLL) == BST_CHECKED;

    for (int i = 0; i < 64; i++) {
        if ((uint32_t)data->heroStages[i].stageID == id) {
            data->heroStages[i].zapfish = zap ? 3 : 0;
            data->heroStages[i].scroll = scroll ? 1 : 0;
            return;
        }
    }
    for (int i = 0; i < 64; i++) {
        if ((uint32_t)data->heroStages[i].stageID == 0 || (int32_t)(uint32_t)data->heroStages[i].stageID == -1) {
            data->heroStages[i].stageID = id;
            data->heroStages[i].zapfish = zap ? 3 : 0;
            data->heroStages[i].scroll = scroll ? 1 : 0;
            return;
        }
    }
}



void InitHeroTab(HWND hParent) {
    hHeroTab = hParent;
    CreateWindowW(L"BUTTON", L"General && Upgrades", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 10, 5, 365, 285, hHeroTab, NULL, GetModuleHandle(NULL), NULL);
    LayoutSmallControls(hHeroTab, 20, 25, L"Power Eggs:", IDC_H_EGGS);
    HWND hEggs = GetDlgItem(hHeroTab, IDC_H_EGGS);
    SendMessageW(hEggs, EM_SETLIMITTEXT, 4, 0);
    SetWindowLongW(hEggs, GWL_STYLE, GetWindowLongW(hEggs, GWL_STYLE) | ES_NUMBER);
    LayoutSmallComboList(hHeroTab, 20, 55, L"Hero Shot Lvl:", IDC_H_SHOT_LVL, g_HeroUpgradeNames, 5);
    LayoutSmallComboList(hHeroTab, 20, 85, L"Ink Tank Lvl:", IDC_H_TANK_LVL, g_HeroUpgradeNames, 5);
    LayoutSmallComboList(hHeroTab, 20, 115, L"Equipped Bomb:", IDC_H_BOMB_SELECT, g_BombNames, 3);
    LayoutSmallComboList(hHeroTab, 20, 145, L"Splat Bomb Lvl:", IDC_H_SPLATBOM_LVL, g_HeroUpgradeNames, 5);
    LayoutSmallComboList(hHeroTab, 20, 175, L"Burst Bomb Lvl:", IDC_H_BURSTBOM_LVL, g_HeroUpgradeNames, 5);
    LayoutSmallComboList(hHeroTab, 20, 205, L"Seeker Lvl:", IDC_H_SEEKER_LVL, g_HeroUpgradeNames, 5);
    CreateWindowW(L"BUTTON", L"Unlock All World (Meta)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 20, 240, 320, 25, hHeroTab, (HMENU)IDC_H_BTN_UNLOCK_WORLD, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Stage Progress", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 385, 5, 365, 285, hHeroTab, NULL, GetModuleHandle(NULL), NULL);
    hStageList = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 395, 25, 150, 245, hHeroTab, (HMENU)IDC_H_STAGE_LIST, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Zapfish Collected", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 555, 25, 150, 20, hHeroTab, (HMENU)IDC_H_STAGE_ZAPFISH, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Sunken Scroll Collected", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 555, 55, 180, 20, hHeroTab, (HMENU)IDC_H_STAGE_SCROLL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Mark All Stages Complete", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 550, 245, 190, 25, hHeroTab, (HMENU)IDC_H_BTN_COMPLETE_ALL, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"BUTTON", L"Story Flags", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 10, 295, 740, 110, hHeroTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Great Zapfish Returned (Story Beaten)", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 20, 315, 270, 20, hHeroTab, (HMENU)IDC_H_FLAG_ZAPFISH, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Hero Suit Given", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 20, 340, 250, 20, hHeroTab, (HMENU)IDC_H_FLAG_HEROSUIT, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Cap'n Cuttlefish Dialogue Seen", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 20, 365, 280, 20, hHeroTab, (HMENU)IDC_H_FLAG_CUTTLEFISH, GetModuleHandle(NULL), NULL);

    LayoutSmallControls(hHeroTab, 320, 315, L"Hero Inkrails Unlocked:", IDC_H_INKRAILS, 150);
    HWND hInk = GetDlgItem(hHeroTab, IDC_H_INKRAILS);
    SendMessageW(hInk, EM_SETLIMITTEXT, 10, 0);
    SetWindowLongW(hInk, GWL_STYLE, GetWindowLongW(hInk, GWL_STYLE) | ES_NUMBER);

    LayoutSmallControls(hHeroTab, 320, 345, L"Hero Events (Bitmask):", IDC_H_EVENTS, 150);
    HWND hEvents = GetDlgItem(hHeroTab, IDC_H_EVENTS);
    SendMessageW(hEvents, EM_SETLIMITTEXT, 10, 0);
    SetWindowLongW(hEvents, GWL_STYLE, GetWindowLongW(hEvents, GWL_STYLE) | ES_NUMBER);

    if (g_hFont) {
        EnumChildWindows(hHeroTab, [](HWND k, LPARAM l) -> BOOL { SendMessageW(k, WM_SETFONT, (WPARAM)l, TRUE); return TRUE; }, (LPARAM)g_hFont);
    }
    for (int i = 0; i < 32; i++) SendMessageW(hStageList, LB_ADDSTRING, 0, (LPARAM)g_HeroStageNames[i]);
    SendMessageW(hStageList, LB_SETCURSEL, 0, 0);
}

void UpdateHeroTabUI(SaveData* data) {
    if (!data) return;
    WriteInt32(hHeroTab, IDC_H_EGGS, data->powerEggs);
    WriteCombo32(hHeroTab, IDC_H_SHOT_LVL, data->heroShotLevel);
    WriteCombo32(hHeroTab, IDC_H_TANK_LVL, data->heroTankLevel);
    WriteCombo32(hHeroTab, IDC_H_SPLATBOM_LVL, data->heroSplatBombLevel);
    WriteCombo32(hHeroTab, IDC_H_BURSTBOM_LVL, data->heroBurstBombLevel);
    WriteCombo32(hHeroTab, IDC_H_SEEKER_LVL, data->heroSeekerLevel);
    WriteCombo32(hHeroTab, IDC_H_BOMB_SELECT, data->heroBombSelect);

    CheckDlgButton(hHeroTab, IDC_H_FLAG_ZAPFISH, (data->gameStateFlags & 0x00000080u) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hHeroTab, IDC_H_FLAG_HEROSUIT, (data->gameStateFlags & 0x00000010u) ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hHeroTab, IDC_H_FLAG_CUTTLEFISH, (data->gameStateFlags & 0x00000100u) ? BST_CHECKED : BST_UNCHECKED);

    WriteInt32(hHeroTab, IDC_H_INKRAILS, data->heroInkrails);
    WriteInt32(hHeroTab, IDC_H_EVENTS, data->heroEvents);

    LoadStageData(currentStageIndex, data);
}

void SaveHeroTabData(SaveData* data) {
    if (!data) return;
    ReadInt32(hHeroTab, IDC_H_EGGS, data->powerEggs);
    if ((int32_t)data->powerEggs > 9999) data->powerEggs = 9999;
    if ((int32_t)data->powerEggs < 0) data->powerEggs = 0;
    ReadCombo32(hHeroTab, IDC_H_SHOT_LVL, data->heroShotLevel);
    ReadCombo32(hHeroTab, IDC_H_TANK_LVL, data->heroTankLevel);
    ReadCombo32(hHeroTab, IDC_H_SPLATBOM_LVL, data->heroSplatBombLevel);
    ReadCombo32(hHeroTab, IDC_H_BURSTBOM_LVL, data->heroBurstBombLevel);
    ReadCombo32(hHeroTab, IDC_H_SEEKER_LVL, data->heroSeekerLevel);
    ReadCombo32(hHeroTab, IDC_H_BOMB_SELECT, data->heroBombSelect);

    if (IsDlgButtonChecked(hHeroTab, IDC_H_FLAG_ZAPFISH) == BST_CHECKED) data->gameStateFlags = (uint32_t)data->gameStateFlags | 0x00000080u;
    else data->gameStateFlags = (uint32_t)data->gameStateFlags & ~0x00000080u;

    if (IsDlgButtonChecked(hHeroTab, IDC_H_FLAG_HEROSUIT) == BST_CHECKED) data->gameStateFlags = (uint32_t)data->gameStateFlags | 0x00000010u;
    else data->gameStateFlags = (uint32_t)data->gameStateFlags & ~0x00000010u;

    if (IsDlgButtonChecked(hHeroTab, IDC_H_FLAG_CUTTLEFISH) == BST_CHECKED) data->gameStateFlags = (uint32_t)data->gameStateFlags | 0x00000100u;
    else data->gameStateFlags = (uint32_t)data->gameStateFlags & ~0x00000100u;

    ReadInt32(hHeroTab, IDC_H_INKRAILS, data->heroInkrails);
    ReadInt32(hHeroTab, IDC_H_EVENTS, data->heroEvents);

    SaveStageToData(currentStageIndex, data);
}

bool HandleHeroTabCommand(HWND hControl, int wmEvent) {
    if (hControl == hStageList && wmEvent == LBN_SELCHANGE) {
        int newSel = (int)SendMessageW(hStageList, LB_GETCURSEL, 0, 0);
        if (newSel != LB_ERR && newSel != currentStageIndex) {
            SaveData* data = GetSaveData();
            if (data) {
                SaveStageToData(currentStageIndex, data);
                currentStageIndex = newSel;
                LoadStageData(currentStageIndex, data);
            }
        }
        return true;
    }
    if (wmEvent == BN_CLICKED) {
        int id = (int)(UINT_PTR)GetMenu(hControl);
        SaveData* data = GetSaveData();
        if (!data) return false;
        if (id == IDC_H_BTN_UNLOCK_WORLD) {
            data->heroInkrails = 0xFFFFFFFF;
            data->heroEvents = 0xFFFFFFFF;
            MessageBoxW(hHeroTab, L"All Inkrails and World Events unlocked.", L"Hero Mode", MB_OK);
            return true;
        }
        if (id == IDC_H_BTN_COMPLETE_ALL) {
            for (int i = 0; i < 32; i++) {
                int id_val = g_HeroStageIDs[i];
                bool found = false;
                for (int j = 0; j < 64; j++) {
                    if ((uint32_t)data->heroStages[j].stageID == (uint32_t)id_val) {
                        data->heroStages[j].zapfish = 3;
                        data->heroStages[j].scroll = 1;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    for (int j = 0; j < 64; j++) {
                        if ((uint32_t)data->heroStages[j].stageID == 0 || (int32_t)(uint32_t)data->heroStages[j].stageID == -1) {
                            data->heroStages[j].stageID = id_val;
                            data->heroStages[j].zapfish = 3;
                            data->heroStages[j].scroll = 1;
                            break;
                        }
                    }
                }
            }
            data->heroInkrails = 0x1FFFFFU;
            data->heroEvents = 0x3EFBEU;
            LoadStageData(currentStageIndex, data);
            MessageBoxW(hHeroTab, L"All 32 stages marked complete.\nAll inkrails and world events unlocked.", L"Hero Mode", MB_OK);
            return true;
        }
    }
    return false;
}
