#include "Resource.h"
#include "globals.h"
#include "ui_helpers.h"
#include "tabs.h"
#include "utils.h"
#include <commctrl.h>
#include <commdlg.h>
#include <fstream>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")

HWND g_hwndMain = NULL;
HWND g_hwndTab = NULL;
HWND g_hwndChild[9] = { NULL };
HWND g_hwndPlayerTab = NULL;
HWND g_hwndPlayerSubChild[4] = { NULL };
HWND g_hwndGearTab = NULL;
HWND g_hwndGearSubChild[3] = { NULL };
HFONT g_hFont = NULL;

std::vector<uint8_t> g_SaveBuffer;
std::wstring g_CurrentFilePath;
size_t g_OriginalFileSize = 0;

SaveData* GetSaveData() {
    if (g_SaveBuffer.size() == sizeof(SaveData)) {
        return reinterpret_cast<SaveData*>(g_SaveBuffer.data());
    }
    return nullptr;
}

const wchar_t* g_PlazaRankNames[] = {
    L"0 (C-)", L"1 (C)", L"2 (C+)", L"3 (B-)", L"4 (B)", L"5 (B+)",
    L"6 (A-)", L"7 (A)", L"8 (A+)", L"9 (S)", L"10 (S+)", L"11 (S+ 99)"
};
const wchar_t* g_GenderNames[] = { L"0 (Inkling Girl)", L"1 (Inkling Boy)", L"2 (Octoling)" };
const wchar_t* g_SplatfestTeamNames[] = { L"-1 (None)", L"0 (Callie)", L"1 (Marie)" };
const wchar_t* g_RankNames[] = {
    L"0 (C-)", L"1 (C)", L"2 (C+)", L"3 (B-)", L"4 (B)", L"5 (B+)",
    L"6 (A-)", L"7 (A)", L"8 (A+)", L"9 (S)", L"10 (S+)"
};
const wchar_t* g_EyeColorNames[] = {
    L"0 (Brown)", L"1 (Pink)", L"2 (Blue)", L"3 (Orange)",
    L"4 (Turquoise)", L"5 (Black)", L"6 (Yellow)"
};

void UpdateUIFromData();

EditorConfig g_config;

std::wstring GetConfigPath() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring s(path);
    size_t lastSlash = s.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        return s.substr(0, lastSlash + 1) + L"config.json";
    }
    return L"config.json";
}

void SaveConfig() {
    FILE* f = nullptr;
    std::wstring path = GetConfigPath();
    _wfopen_s(&f, path.c_str(), L"w");
    if (f) {
        fprintf(f, "{\n");
        fprintf(f, "  \"showInternalIDs\": %s,\n", g_config.showInternalIDs ? "true" : "false");
        fprintf(f, "  \"sortByAlphabet\": %s\n", g_config.sortByAlphabet ? "true" : "false");
        fprintf(f, "}\n");
        fclose(f);
    }
}

void LoadConfig() {
    FILE* f = nullptr;
    std::wstring path = GetConfigPath();
    _wfopen_s(&f, path.c_str(), L"r");
    if (f) {
        char buf[1024];
        size_t r = fread(buf, 1, 1023, f);
        buf[r] = 0;
        fclose(f);

        std::string s(buf);
        size_t posId = s.find("\"showInternalIDs\"");
        if (posId != std::string::npos) {
            g_config.showInternalIDs = (s.find("true", posId) < s.find("false", posId));
        }

        size_t posAbc = s.find("\"sortByAlphabet\"");
        if (posAbc != std::string::npos) {
            g_config.sortByAlphabet = (s.find("true", posAbc) < s.find("false", posAbc));
        }
    }
    else {
        SaveConfig();
    }
}

extern const wchar_t* g_AbilityNames[];
extern const int g_AbilityCount;
extern const wchar_t* g_HeroUpgradeNames[];
extern const wchar_t* g_BombNames[];
extern const wchar_t* g_SplatfestRankNames[];
extern const wchar_t* g_SkinColorNames[];
extern HWND hAmiiboSubTabs[3];
extern const wchar_t* g_ChallengeStates[];
extern HWND hSpykeTab;

void RefillAllComboboxes() {
    if (!g_hwndMain) return;

    FillCombobox(GetDlgItem(g_hwndChild[TAB_WEAPON], IDC_W_WEAPON), g_WeaponNames, g_WeaponCount, false);
    HWND hAddPick = GetDlgItem(g_hwndChild[TAB_WEAPON], IDC_W_ADD_PICKER);
    if (hAddPick) FillCombobox(hAddPick, g_WeaponNames, g_WeaponCount, false);

    for (int i = 0; i < 3; i++) {
        HWND hSub = g_hwndGearSubChild[i];
        if (hSub) {
            int equipIdc = (i == 0) ? IDC_G_HEAD_EQUIP : ((i == 1) ? IDC_G_CLOTH_EQUIP : IDC_G_SHOE_EQUIP);
            const wchar_t** equipNames = (i == 0) ? g_HeadNames : ((i == 1) ? g_ClothNames : g_ShoeNames);
            int equipCount = (i == 0) ? g_HeadCount : ((i == 1) ? g_ClothCount : g_ShoeCount);

            FillCombobox(GetDlgItem(hSub, equipIdc), equipNames, equipCount, false);
            FillAbilityCombobox(GetDlgItem(hSub, IDC_G_SUB1));
            FillAbilityCombobox(GetDlgItem(hSub, IDC_G_SUB2));
            FillAbilityCombobox(GetDlgItem(hSub, IDC_G_SUB3));

            HWND hGearAddPick = GetDlgItem(hSub, IDC_G_ADD_PICKER);
            if (hGearAddPick) FillCombobox(hGearAddPick, equipNames, equipCount, false);
        }
    }

    if (g_hwndPlayerSubChild[PLAYER_SUB_STATS]) FillCombobox(GetDlgItem(g_hwndPlayerSubChild[PLAYER_SUB_STATS], IDC_P_RANK), g_RankNames, 11, false);
    if (g_hwndPlayerSubChild[PLAYER_SUB_APPEAR]) {
        FillCombobox(GetDlgItem(g_hwndPlayerSubChild[PLAYER_SUB_APPEAR], IDC_P_GENDER), g_GenderNames, 3, false);
        FillCombobox(GetDlgItem(g_hwndPlayerSubChild[PLAYER_SUB_APPEAR], IDC_P_SKIN), g_SkinColorNames, 8, false, true);
        FillCombobox(GetDlgItem(g_hwndPlayerSubChild[PLAYER_SUB_APPEAR], IDC_P_EYE), g_EyeColorNames, 7, false);
    }
    if (g_hwndPlayerSubChild[PLAYER_SUB_SPLATFEST]) {
        FillCombobox(GetDlgItem(g_hwndPlayerSubChild[PLAYER_SUB_SPLATFEST], IDC_PL_SF_TEAM), g_SplatfestTeamNames, 3, false);
        FillCombobox(GetDlgItem(g_hwndPlayerSubChild[PLAYER_SUB_SPLATFEST], IDC_PL_SF_RANK), g_SplatfestRankNames, 5, false);
    }

    if (g_hwndChild[TAB_HERO]) {
        FillCombobox(GetDlgItem(g_hwndChild[TAB_HERO], IDC_H_SHOT_LVL), g_HeroUpgradeNames, 5, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_HERO], IDC_H_TANK_LVL), g_HeroUpgradeNames, 5, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_HERO], IDC_H_SPLATBOM_LVL), g_HeroUpgradeNames, 5, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_HERO], IDC_H_BURSTBOM_LVL), g_HeroUpgradeNames, 5, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_HERO], IDC_H_SEEKER_LVL), g_HeroUpgradeNames, 5, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_HERO], IDC_H_BOMB_SELECT), g_BombNames, 3, false);
    }

    if (g_hwndChild[TAB_PLAZA]) {
        FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_RANK), g_PlazaRankNames, 12, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_GENDER), g_GenderNames, 3, false);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_SKIN), g_SkinColorNames, 8, false, true);
        FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_EYE), g_EyeColorNames, 7, false);

        SaveData* data = GetSaveData();
        if (data && currentPlazaIndex >= 0 && currentPlazaIndex < 25) {
            LoadPlazaInkling(currentPlazaIndex, data);
        }
        else {
            FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_HEAD), g_HeadNames, g_HeadCount, true);
            FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_CLOTH), g_ClothNames, g_ClothCount, true);
            FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_SHOE), g_ShoeNames, g_ShoeCount, true);
            FillCombobox(GetDlgItem(g_hwndChild[TAB_PLAZA], IDC_Z_WEAPON), g_WeaponNames, g_WeaponCount, true);
        }
    }

    for (int i = 0; i < 3; i++) {
        HWND hSub = hAmiiboSubTabs[i];
        if (hSub) {
            for (int j = 0; j < 20; j++) {
                HWND hCb = GetDlgItem(hSub, IDC_A_CHALLENGE_BASE + (i * 20) + j);
                if (hCb) FillCombobox(hCb, g_ChallengeStates, 5, false);
            }
        }
    }

    if (hSpykeTab) {
        SaveData* data = GetSaveData();
        if (data) {
            UpdateSpykeTabUI(data);
        }
        else {
            for (int i = 0; i < 3; i++) {
                UpdateGearList(hSpykeTab, IDC_S_CAT_1 + i, IDC_S_ID_1 + i, 0);
            }
        }
    }

    UpdateUIFromData();
}

#define IDC_OPT_ID 3001
#define IDC_OPT_ABC 3002
#define IDC_OPT_SAVE 3003
#define IDC_OPT_BRICK 3004

LRESULT CALLBACK AboutWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hChkId = NULL;
    static HWND hChkAbc = NULL;

    switch (msg) {
    case WM_CREATE: {
        RECT rcParent;
        GetWindowRect(g_hwndMain, &rcParent);
        int w = 360;
        int h = 230;
        int x = rcParent.left + (rcParent.right - rcParent.left - w) / 2;
        int y = rcParent.top + (rcParent.bottom - rcParent.top - h) / 2;
        SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER);

        HWND hCredits = CreateWindowW(L"STATIC", L"Spoon Save Editor\nMade by @tvyrval / vyrval",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 15, 320, 40, hwnd, NULL, GetModuleHandle(NULL), NULL);

        HWND hVersion = CreateWindowW(L"STATIC", L"Version: 1.0.0",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 55, 320, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);

        hChkId = CreateWindowW(L"BUTTON", L"Enable internal ID showing",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 20, 85, 300, 20, hwnd, (HMENU)IDC_OPT_ID, GetModuleHandle(NULL), NULL);
        hChkAbc = CreateWindowW(L"BUTTON", L"Sort text by Alphabet",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 20, 115, 320, 20, hwnd, (HMENU)IDC_OPT_ABC, GetModuleHandle(NULL), NULL);

        CheckDlgButton(hwnd, IDC_OPT_ID, g_config.showInternalIDs ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDC_OPT_ABC, g_config.sortByAlphabet ? BST_CHECKED : BST_UNCHECKED);

        HWND btnSave = CreateWindowW(L"BUTTON", L"OK",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 70, 155, 100, 30, hwnd, (HMENU)IDC_OPT_SAVE, GetModuleHandle(NULL), NULL);

        HWND btnBrick = CreateWindowW(L"BUTTON", L"Brick Save",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 180, 155, 100, 30, hwnd, (HMENU)IDC_OPT_BRICK, GetModuleHandle(NULL), NULL);

        if (g_hFont) {
            SendMessageW(hCredits, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            SendMessageW(hChkId, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            SendMessageW(hChkAbc, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            SendMessageW(btnSave, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            SendMessageW(btnBrick, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            SendMessageW(hVersion, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        }

        EnableWindow(g_hwndMain, FALSE);
        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == IDC_OPT_SAVE) {
            g_config.showInternalIDs = (IsDlgButtonChecked(hwnd, IDC_OPT_ID) == BST_CHECKED);
            g_config.sortByAlphabet = (IsDlgButtonChecked(hwnd, IDC_OPT_ABC) == BST_CHECKED);
            SaveConfig();
            RefillAllComboboxes();
            EnableWindow(g_hwndMain, TRUE);
            DestroyWindow(hwnd);
        }
        break;
    }

    case WM_CLOSE: {
        EnableWindow(g_hwndMain, TRUE);
        DestroyWindow(hwnd);
        break;
    }

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void InitTabs(HWND hMain) {
    RegisterSpoonContainer();
    g_hwndTab = CreateWindowExW(0, WC_TABCONTROLW, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 10, 50, 760, 440, hMain, NULL, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(g_hwndTab, WM_SETFONT, (WPARAM)g_hFont, TRUE);

    const wchar_t* mainTabs[] = { L"Weapon Editor", L"Gear Editor", L"Player Editor", L"Hero Mode", L"Plaza Editor", L"Amiibo Challenges", L"Minigames", L"Spyke Orders", L"BitFlags" };
    for (int i = 0; i < 9; ++i) {
        TCITEMW tie = { 0 };
        tie.mask = TCIF_TEXT;
        tie.pszText = (LPWSTR)mainTabs[i];
        TabCtrl_InsertItem(g_hwndTab, i, &tie);
        
        g_hwndChild[i] = CreateWindowW(L"SpoonContainer", NULL, WS_CHILD | (i == 0 ? WS_VISIBLE : 0), 0, 25, 800, 575, g_hwndTab, NULL, GetModuleHandle(NULL), NULL);
    }

    InitWeaponTab(g_hwndChild[TAB_WEAPON]);
    InitGearTab(g_hwndChild[TAB_GEAR]);
    InitPlayerTab(g_hwndChild[TAB_PLAYER]);
    InitHeroTab(g_hwndChild[TAB_HERO]);
    InitPlazaTab(g_hwndChild[TAB_PLAZA]);
    InitAmiiboTab(g_hwndChild[TAB_AMIIBO]);
    InitMinigameTab(g_hwndChild[TAB_MINIGAME]);
    InitSpykeTab(g_hwndChild[TAB_SPYKE]);
    InitBitFlagTab(g_hwndChild[TAB_BITFLAG]);

    ShowWindow(g_hwndChild[0], SW_SHOW);
}

void UpdateUIFromData() {
    SaveData* data = GetSaveData();
    if (!data) return;

    UpdateWeaponTabUI(data);
    UpdateGearTabUI(data);
    UpdatePlayerTabUI(data);
    UpdateHeroTabUI(data);
    UpdatePlazaTabUI(data);
    UpdateAmiiboTabUI(data);
    UpdateMinigameTabUI(data);
    UpdateSpykeTabUI(data);
    UpdateBitFlagTabUI(data);
}

void UpdateDataFromUI() {
    SaveData* data = GetSaveData();
    if (!data) return;

    SaveWeaponTabData(data);
    SaveGearTabData(data);
    SavePlayerTabData(data);
    SaveHeroTabData(data);
    SavePlazaTabData(data);
    SaveAmiiboTabData(data);
    SaveMinigameTabData(data);
    SaveSpykeTabData(data);
    SaveBitFlagTabData(data);
}

void SaveFile() {
    if (g_CurrentFilePath.empty() || g_SaveBuffer.empty()) {
        MessageBoxW(g_hwndMain, L"No file loaded.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    UpdateDataFromUI();

    SaveData* data = reinterpret_cast<SaveData*>(g_SaveBuffer.data());
    size_t writeSize = g_OriginalFileSize > 0 ? g_OriginalFileSize : sizeof(SaveData);

    uint32_t crc = CalculateCRC32(g_SaveBuffer.data() + 16, writeSize - 16);
    data->checksum = crc;

    std::ofstream file(g_CurrentFilePath, std::ios::binary);
    if (!file) {
        MessageBoxW(g_hwndMain, L"Failed to write file.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    file.write(reinterpret_cast<char*>(g_SaveBuffer.data()), writeSize);
    file.close();
    MessageBoxW(g_hwndMain, L"Saved Successfully.", L"Success", MB_OK | MB_ICONINFORMATION);
}

void LoadFile() {
    OPENFILENAMEW ofn;
    wchar_t szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hwndMain;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 260;
    ofn.lpstrFilter = L"Dat Files\0*.dat\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        std::ifstream file(ofn.lpstrFile, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t s = file.tellg();
            if (s != sizeof(SaveData) && s != 52292) {
                MessageBoxW(g_hwndMain, L"Invalid file size. Expected ~53,888 bytes (Cemu Splatoon 1 save).", L"Error", MB_OK | MB_ICONERROR);
                return;
            }
            file.seekg(0, std::ios::beg);

            g_SaveBuffer.assign(sizeof(SaveData), 0);
            file.read(reinterpret_cast<char*>(g_SaveBuffer.data()), s);
            g_OriginalFileSize = s;

            g_CurrentFilePath = ofn.lpstrFile;
            UpdateUIFromData();
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        g_hwndMain = hwnd;

        NONCLIENTMETRICSW ncm;
        ncm.cbSize = sizeof(NONCLIENTMETRICSW);
        SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
        g_hFont = CreateFontIndirectW(&ncm.lfMessageFont);

        HWND btnOpen = CreateWindowW(L"BUTTON", L"Open .dat File",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 10, 130, 32, hwnd, (HMENU)ID_BTN_OPEN, GetModuleHandle(NULL), NULL);

        HWND btnSave = CreateWindowW(L"BUTTON", L"Save .dat File",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            150, 10, 130, 32, hwnd, (HMENU)ID_BTN_SAVE, GetModuleHandle(NULL), NULL);

        if (g_hFont) {
            SendMessageW(btnOpen, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            SendMessageW(btnSave, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        }

        HWND btnAbout = CreateWindowW(L"BUTTON", L"?", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 728, 451, 22, 22, hwnd, (HMENU)ID_BTN_ABOUT, GetModuleHandle(NULL), NULL);
        if (g_hFont) SendMessageW(btnAbout, WM_SETFONT, (WPARAM)g_hFont, TRUE);

        InitTabs(hwnd);
        break;
    }

    case WM_NOTIFY: {
        LPNMHDR nm = (LPNMHDR)lParam;
        if (nm->code == TCN_SELCHANGE) {
            if (nm->hwndFrom == g_hwndTab) {
                int sel = TabCtrl_GetCurSel(g_hwndTab);
                for (int i = 0; i < 9; ++i) ShowWindow(g_hwndChild[i], i == sel ? SW_SHOW : SW_HIDE);
            }
            else if (nm->hwndFrom == g_hwndPlayerTab) {
                int sel = TabCtrl_GetCurSel(g_hwndPlayerTab);
                for (int i = 0; i < 4; ++i) ShowWindow(g_hwndPlayerSubChild[i], i == sel ? SW_SHOW : SW_HIDE);
            }
            else if (nm->hwndFrom == g_hwndGearTab) {
                int sel = TabCtrl_GetCurSel(g_hwndGearTab);
                for (int i = 0; i < 3; ++i) ShowWindow(g_hwndGearSubChild[i], i == sel ? SW_SHOW : SW_HIDE);
            }
        }
        break;
    }

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);

        if (HandleWeaponTabCommand((HWND)lParam, wmEvent)) {
            return 0;
        }
        if (HandleGearTabCommand((HWND)lParam, wmEvent)) {
            return 0;
        }

        if (HandlePlazaTabCommand((HWND)lParam, wmEvent)) {
            return 0;
        }
        if (HandleHeroTabCommand((HWND)lParam, wmEvent)) {
            return 0;
        }
        if (HandleSpykeTabCommand((HWND)lParam, wmEvent)) {
            return 0;
        }
        if (HandleAmiiboTabCommand(hwnd, msg, wParam, lParam)) {
            return 0;
        }

        if (wmId == ID_BTN_OPEN) LoadFile();
        if (wmId == ID_BTN_SAVE) SaveFile();
        if (wmId == ID_BTN_ABOUT) {
            HWND hwndAbout = CreateWindowExW(
                WS_EX_DLGMODALFRAME, L"SpoonAboutClass", L"About & Options",
                WS_POPUPWINDOW | WS_CAPTION,
                CW_USEDEFAULT, CW_USEDEFAULT, 360, 230,
                hwnd, NULL, GetModuleHandle(NULL), NULL
            );
            if (hwndAbout) {
                ShowWindow(hwndAbout, SW_SHOW);
                UpdateWindow(hwndAbout);
            }
            return 0;
        }
        break;
    }

    case WM_DESTROY:
        if (g_hFont) DeleteObject(g_hFont);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    INITCOMMONCONTROLSEX icex = { 0 };
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    const wchar_t CLASS_NAME[] = L"SpoonSaveEditorClass";

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_SPOONSAVEEDITOR));

    LoadConfig();

    WNDCLASSW wcAbout = { 0 };
    wcAbout.lpfnWndProc = AboutWndProc;
    wcAbout.hInstance = hInstance;
    wcAbout.lpszClassName = L"SpoonAboutClass";
    wcAbout.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    wcAbout.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    RegisterClassW(&wcAbout);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Spoon Save Editor",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 790, 530,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(g_hwndTab, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return 0;
}
