#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <cmath>

static HWND hSubTabs[4];

void InitPlayerTab(HWND hParent) {
    g_hwndPlayerTab = CreateWindowExW(0, WC_TABCONTROLW, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 5, 5, 730, 250, hParent, NULL, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(g_hwndPlayerTab, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    const wchar_t* subTabs[] = { L"Stats", L"Appearance", L"Splatfest", L"Control Settings" };
    for (int i = 0; i < 4; ++i) {
        TCITEMW tie = { 0 };
        tie.mask = TCIF_TEXT;
        tie.pszText = (LPWSTR)subTabs[i];
        TabCtrl_InsertItem(g_hwndPlayerTab, i, &tie);
        hSubTabs[i] = CreateChildTab(g_hwndPlayerTab);
        g_hwndPlayerSubChild[i] = hSubTabs[i];
    }

    CreateWindowW(L"BUTTON", L"Player Statistics", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 5, 660, 170, hSubTabs[PLAYER_SUB_STATS], NULL, GetModuleHandle(NULL), NULL);
    LayoutControls(hSubTabs[PLAYER_SUB_STATS], 20, 25, L"Level:", IDC_P_LEVEL);
    LayoutControls(hSubTabs[PLAYER_SUB_STATS], 340, 25, L"Level Points:", IDC_P_LEVELPTS);
    LayoutComboList(hSubTabs[PLAYER_SUB_STATS], 20, 60, L"Rank:", IDC_P_RANK, g_RankNames, 11);
    LayoutControls(hSubTabs[PLAYER_SUB_STATS], 340, 60, L"Rank Points:", IDC_P_RANKPTS);
    LayoutControls(hSubTabs[PLAYER_SUB_STATS], 20, 95, L"Money Value:", IDC_P_MONEY);

    HWND hLevel = GetDlgItem(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVEL);
    SendMessageW(hLevel, EM_SETLIMITTEXT, 4, 0);

    HWND hLevelPts = GetDlgItem(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVELPTS);
    SendMessageW(hLevelPts, EM_SETLIMITTEXT, 4, 0);

    HWND hRankPts = GetDlgItem(hSubTabs[PLAYER_SUB_STATS], IDC_P_RANKPTS);
    SendMessageW(hRankPts, EM_SETLIMITTEXT, 4, 0);

    HWND hMoney = GetDlgItem(hSubTabs[PLAYER_SUB_STATS], IDC_P_MONEY);
    SendMessageW(hMoney, EM_SETLIMITTEXT, 7, 0);
    SetWindowLongW(hMoney, GWL_STYLE, GetWindowLongW(hMoney, GWL_STYLE) | ES_NUMBER);

    LayoutControls(hSubTabs[PLAYER_SUB_STATS], 340, 95, L"Sea Snails:", IDC_P_SNAILS);

    HWND hSnails = GetDlgItem(hSubTabs[PLAYER_SUB_STATS], IDC_P_SNAILS);
    SendMessageW(hSnails, EM_SETLIMITTEXT, 3, 0);
    SetWindowLongW(hSnails, GWL_STYLE, GetWindowLongW(hSnails, GWL_STYLE) | ES_NUMBER);

    LayoutControls(hSubTabs[PLAYER_SUB_STATS], 20, 130, L"Update Version:", IDC_P_NEWS_VERSION);
    HWND hNews = GetDlgItem(hSubTabs[PLAYER_SUB_STATS], IDC_P_NEWS_VERSION);
    SendMessageW(hNews, EM_SETLIMITTEXT, 3, 0);
    SetWindowLongW(hNews, GWL_STYLE, GetWindowLongW(hNews, GWL_STYLE) | ES_NUMBER);

    CreateWindowW(L"BUTTON", L"Player Appearance", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 5, 660, 140, hSubTabs[PLAYER_SUB_APPEAR], NULL, GetModuleHandle(NULL), NULL);
    LayoutComboList(hSubTabs[PLAYER_SUB_APPEAR], 20, 25, L"Gender:", IDC_P_GENDER, g_GenderNames, 3);
    LayoutComboList(hSubTabs[PLAYER_SUB_APPEAR], 20, 60, L"Skin Color:", IDC_P_SKIN, g_SkinColorNames, 8, false, true);
    LayoutComboList(hSubTabs[PLAYER_SUB_APPEAR], 20, 95, L"Eye Color:", IDC_P_EYE, g_EyeColorNames, 7);

    CreateWindowW(L"BUTTON", L"Splatfest Data", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 5, 660, 140, hSubTabs[PLAYER_SUB_SPLATFEST], NULL, GetModuleHandle(NULL), NULL);
    LayoutControls(hSubTabs[PLAYER_SUB_SPLATFEST], 20, 25, L"Splatfest ID:", IDC_PL_SF_ID);
    HWND hSfId = GetDlgItem(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_ID);
    SendMessageW(hSfId, EM_SETLIMITTEXT, 4, 0);
    SetWindowLongW(hSfId, GWL_STYLE, GetWindowLongW(hSfId, GWL_STYLE) | ES_NUMBER);

    LayoutComboList(hSubTabs[PLAYER_SUB_SPLATFEST], 340, 25, L"Splatfest Team:", IDC_PL_SF_TEAM, g_SplatfestTeamNames, 3);
    LayoutControls(hSubTabs[PLAYER_SUB_SPLATFEST], 20, 60, L"Splatfest Power:", IDC_PL_SF_PWR);
    LayoutControls(hSubTabs[PLAYER_SUB_SPLATFEST], 340, 60, L"Splatfest XP:", IDC_PL_SF_XP);
    LayoutComboList(hSubTabs[PLAYER_SUB_SPLATFEST], 20, 95, L"Splatfest Title:", IDC_PL_SF_RANK, g_SplatfestRankNames, 5);

    CreateWindowW(L"BUTTON", L"Control Options", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 5, 580, 200, hSubTabs[PLAYER_SUB_CONTROLS], NULL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Color Lock", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 20, 25, 150, 24, hSubTabs[PLAYER_SUB_CONTROLS], (HMENU)IDC_CTRL_COLOR_LOCK, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Reverse Y-Axis", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 20, 55, 150, 24, hSubTabs[PLAYER_SUB_CONTROLS], (HMENU)IDC_CTRL_REV_Y, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Reverse X-Axis", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 20, 85, 150, 24, hSubTabs[PLAYER_SUB_CONTROLS], (HMENU)IDC_CTRL_REV_X, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Motion Controls", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 20, 115, 150, 24, hSubTabs[PLAYER_SUB_CONTROLS], (HMENU)IDC_CTRL_MOTION, GetModuleHandle(NULL), NULL);

    LayoutComboList(hSubTabs[PLAYER_SUB_CONTROLS], 20, 150, L"Miiverse Display:", IDC_Z_MIIVERSE, g_MiiverseNames, g_MiiverseCount);

    CreateWindowW(L"STATIC", L"Camera Sensitivity:", WS_CHILD | WS_VISIBLE, 300, 28, 130, 20, hSubTabs[PLAYER_SUB_CONTROLS], NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 440, 25, 60, 24, hSubTabs[PLAYER_SUB_CONTROLS], (HMENU)IDC_CTRL_CAM_SENS, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"STATIC", L"(-5 to 5)", WS_CHILD | WS_VISIBLE, 510, 28, 60, 20, hSubTabs[PLAYER_SUB_CONTROLS], NULL, GetModuleHandle(NULL), NULL);

    if (g_hFont) {
        for (int i = 0; i < 4; i++) {
            EnumChildWindows(hSubTabs[i], [](HWND hWnd, LPARAM lp) -> BOOL {
                SendMessageW(hWnd, WM_SETFONT, (WPARAM)lp, TRUE);
                return TRUE;
                }, (LPARAM)g_hFont);
        }
    }

    ShowWindow(hSubTabs[0], SW_SHOW);
}

void UpdatePlayerTabUI(SaveData* data) {
    if ((int32_t)(uint32_t)data->level == -1) {
        SetDlgItemTextW(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVEL, L"-1");
    }
    else {
        SetDlgItemTextW(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVEL, std::to_wstring((int32_t)(uint32_t)data->level + 1).c_str());
    }
    WriteInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVELPTS, data->levelPoints);
    WriteCombo32(hSubTabs[PLAYER_SUB_STATS], IDC_P_RANK, data->rank);
    WriteInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_RANKPTS, data->rankPoints);
    WriteInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_MONEY, data->money);
    WriteInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_SNAILS, data->seaSnails);
    WriteInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_NEWS_VERSION, data->newsVersionSeen);

    WriteCombo32(hSubTabs[PLAYER_SUB_APPEAR], IDC_P_GENDER, data->genderID);
    WriteCombo32(hSubTabs[PLAYER_SUB_APPEAR], IDC_P_SKIN, data->skinColor);
    WriteCombo32(hSubTabs[PLAYER_SUB_APPEAR], IDC_P_EYE, data->eyeColor);

    WriteInt32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_ID, data->splatfestID);
    WriteCombo32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_TEAM, data->splatfestTeamID);
    WriteInt32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_PWR, data->splatfestPower);
    WriteInt32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_XP, data->splatfestXP);
    WriteCombo32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_RANK, data->splatfestRank);

    WriteCombo32(hSubTabs[PLAYER_SUB_CONTROLS], IDC_Z_MIIVERSE, data->miiverseDisplay);

    uint8_t colorLock = (uint8_t)(data->miiverseDisplay & 0xFF);
    CheckDlgButton(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_COLOR_LOCK, colorLock == 1 ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_REV_Y, data->reverseY == 1 ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_REV_X, data->reverseX == 1 ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_MOTION, data->motionControls == 1 ? BST_CHECKED : BST_UNCHECKED);

    uint32_t rawValue = (uint32_t)data->cameraSensRaw;
    float camFloat = 0.0f;
    memcpy(&camFloat, &rawValue, sizeof(float));
    int camDisplay = (int)roundf(camFloat * 5.0f);
    SetDlgItemTextW(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_CAM_SENS, std::to_wstring(camDisplay).c_str());
}

void SavePlayerTabData(SaveData* data) {
    wchar_t buf[256];
    GetDlgItemTextW(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVEL, buf, 256);
    int32_t v = 0;
    swscanf_s(buf, L"%d", &v);
    if (v == -1) {
        data->level = 0xFFFFFFFF;
    }
    else {
        if (v < -128) v = -128;
        if (v > 127) v = 127;
        data->level = (uint32_t)(v - 1);
    }

    ReadInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_LEVELPTS, data->levelPoints);
    if ((int32_t)data->levelPoints != -1) {
        if ((int32_t)data->levelPoints > 999) data->levelPoints = 999;
        if ((int32_t)data->levelPoints < 0) data->levelPoints = 0;
    }

    ReadCombo32(hSubTabs[PLAYER_SUB_STATS], IDC_P_RANK, data->rank);
    ReadInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_RANKPTS, data->rankPoints);
    if ((int32_t)data->rankPoints != -1) {
        if ((int32_t)data->rankPoints > 999) data->rankPoints = 999;
        if ((int32_t)data->rankPoints < 0) data->rankPoints = 0;
    }

    ReadInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_MONEY, data->money);
    if ((uint32_t)data->money > 9999999) data->money = (uint32_t)9999999;
    ReadInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_SNAILS, data->seaSnails);
    if ((uint32_t)data->seaSnails > 999) data->seaSnails = (uint32_t)999;
    ReadInt32(hSubTabs[PLAYER_SUB_STATS], IDC_P_NEWS_VERSION, data->newsVersionSeen);
    if ((int32_t)data->newsVersionSeen > 15) data->newsVersionSeen = 15;
    if ((int32_t)data->newsVersionSeen < 0) data->newsVersionSeen = 0;

    ReadCombo32(hSubTabs[PLAYER_SUB_APPEAR], IDC_P_GENDER, data->genderID);
    ReadCombo32(hSubTabs[PLAYER_SUB_APPEAR], IDC_P_SKIN, data->skinColor);
    ReadCombo32(hSubTabs[PLAYER_SUB_APPEAR], IDC_P_EYE, data->eyeColor);

    ReadInt32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_ID, data->splatfestID);
    if ((int32_t)data->splatfestID > 9999) data->splatfestID = 9999;
    if ((int32_t)data->splatfestID < 0) data->splatfestID = 0;
    ReadCombo32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_TEAM, data->splatfestTeamID);
    ReadInt32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_PWR, data->splatfestPower);
    ReadInt32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_XP, data->splatfestXP);
    ReadCombo32(hSubTabs[PLAYER_SUB_SPLATFEST], IDC_PL_SF_RANK, data->splatfestRank);

    ReadInt32(hSubTabs[PLAYER_SUB_CONTROLS], IDC_Z_MIIVERSE, data->miiverseDisplay);

    uint32_t miiv = (uint32_t)data->miiverseDisplay;
    uint8_t colorLock = (IsDlgButtonChecked(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_COLOR_LOCK) == BST_CHECKED) ? 1 : 0;
    miiv = (miiv & 0xFFFFFF00u) | colorLock;
    data->miiverseDisplay = miiv;

    data->reverseY = (IsDlgButtonChecked(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_REV_Y) == BST_CHECKED) ? 1 : 0;
    data->reverseX = (IsDlgButtonChecked(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_REV_X) == BST_CHECKED) ? 1 : 0;
    data->motionControls = (IsDlgButtonChecked(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_MOTION) == BST_CHECKED) ? 1 : 0;

    wchar_t camBuf[32] = { 0 };
    GetDlgItemTextW(hSubTabs[PLAYER_SUB_CONTROLS], IDC_CTRL_CAM_SENS, camBuf, 32);
    float camDisplay = 0.0f;
    swscanf_s(camBuf, L"%f", &camDisplay);
    if (camDisplay < -5.0f) camDisplay = -5.0f;
    if (camDisplay > 5.0f) camDisplay = 5.0f;
    float camFloat = camDisplay / 5.0f;
    uint32_t rawVal = 0;
    memcpy(&rawVal, &camFloat, sizeof(float));
    data->cameraSensRaw = rawVal;
}
