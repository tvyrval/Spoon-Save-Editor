#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <stdio.h>

static HWND hAmiiboTabControl;
HWND hAmiiboSubTabs[3];

const wchar_t* g_ChallengeStates[] = {
    L"0 (Hidden)",
    L"1 (Locked)",
    L"2 (Unlocked)",
    L"3 (Unlocked F)",
    L"4 (Finished)"
};

void InitAmiiboTab(HWND hParent) {
    hAmiiboTabControl = CreateWindowExW(0, WC_TABCONTROLW, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 5, 5, 730, 330, hParent, NULL, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(hAmiiboTabControl, WM_SETFONT, (WPARAM)g_hFont, TRUE);

    const wchar_t* subTabs[] = { L"Inkling Girl", L"Inkling Boy", L"Inkling Squid" };
    for (int i = 0; i < 3; ++i) {
        TCITEMW tie = { 0 };
        tie.mask = TCIF_TEXT;
        tie.pszText = (LPWSTR)subTabs[i];
        TabCtrl_InsertItem(hAmiiboTabControl, i, &tie);
        hAmiiboSubTabs[i] = CreateChildTab(hAmiiboTabControl);

        CreateWindowW(L"BUTTON", L"Amiibo Challenges", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 5, 660, 290, hAmiiboSubTabs[i], NULL, GetModuleHandle(NULL), NULL);

        int x = 20, y = 25;
        for (int j = 0; j < 20; j++) {
            wchar_t label[32];
            swprintf_s(label, L"Challenge %d:", j + 1);
            LayoutComboList(hAmiiboSubTabs[i], x, y, label, IDC_A_CHALLENGE_BASE + (i * 20) + j, g_ChallengeStates, 5, false, true);

            y += 26;
            if (j == 9) { x = 340; y = 25; }
        }

        if (g_hFont) {
            EnumChildWindows(hAmiiboSubTabs[i], [](HWND hWnd, LPARAM lp) -> BOOL {
                SendMessageW(hWnd, WM_SETFONT, (WPARAM)lp, TRUE);
                return TRUE;
                }, (LPARAM)g_hFont);
        }
    }
    ShowWindow(hAmiiboSubTabs[0], SW_SHOW);

    HWND btnUnlockAll = CreateWindowW(L"BUTTON", L"Unlock All (Finished)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 545, 340, 190, 30, hParent, (HMENU)IDC_A_BTN_UNLOCK_ALL, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(btnUnlockAll, WM_SETFONT, (WPARAM)g_hFont, TRUE);
}

void UpdateAmiiboTabUI(SaveData* data) {
    if (!data) return;
    for (int i = 0; i < 20; i++) {
        WriteCombo32(hAmiiboSubTabs[0], IDC_A_CHALLENGE_BASE + i, data->girlChallenges[i]);
        WriteCombo32(hAmiiboSubTabs[1], IDC_A_CHALLENGE_BASE + 20 + i, data->boyChallenges[i]);
        WriteCombo32(hAmiiboSubTabs[2], IDC_A_CHALLENGE_BASE + 40 + i, data->squidChallenges[i]);
    }
}

void SaveAmiiboTabData(SaveData* data) {
    if (!data) return;
    for (int i = 0; i < 20; i++) {
        ReadCombo32(hAmiiboSubTabs[0], IDC_A_CHALLENGE_BASE + i, data->girlChallenges[i]);
        ReadCombo32(hAmiiboSubTabs[1], IDC_A_CHALLENGE_BASE + 20 + i, data->boyChallenges[i]);
        ReadCombo32(hAmiiboSubTabs[2], IDC_A_CHALLENGE_BASE + 40 + i, data->squidChallenges[i]);
    }
}

bool HandleAmiiboTabCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NOTIFY) {
        LPNMHDR nm = (LPNMHDR)lParam;
        if (nm->hwndFrom == hAmiiboTabControl && nm->code == TCN_SELCHANGE) {
            int sel = TabCtrl_GetCurSel(hAmiiboTabControl);
            for (int i = 0; i < 3; i++) {
                ShowWindow(hAmiiboSubTabs[i], i == sel ? SW_SHOW : SW_HIDE);
            }
            return true;
        }
    }
    if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED) {
        int id = LOWORD(wParam);
        if (id == IDC_A_BTN_UNLOCK_ALL) {
            SaveData* data = GetSaveData();
            if (data) {
                for (int i = 0; i < 20; i++) {
                    data->girlChallenges[i] = 4;
                    data->boyChallenges[i] = 4;
                    data->squidChallenges[i] = 4;
                }
                UpdateAmiiboTabUI(data);
                MessageBoxW(g_hwndMain, L"All Amiibo Challenges set to Finished.", L"Success", MB_OK);
                return true;
            }
        }
    }
    return false;
}
