#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <stdio.h>

static HWND hPlazaTab;
static HWND hPlazaList;
int currentPlazaIndex = -1;

void InitPlazaTab(HWND hParent) {
    hPlazaTab = hParent;

    CreateWindowW(L"BUTTON", L"Plaza Players", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 5, 160, 275, hPlazaTab, NULL, GetModuleHandle(NULL), NULL);
    hPlazaList = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
        15, 25, 140, 245, hPlazaTab, (HMENU)IDC_Z_LIST, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(hPlazaList, WM_SETFONT, (WPARAM)g_hFont, TRUE);

    CreateWindowW(L"BUTTON", L"Player Details", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 175, 5, 280, 230, hPlazaTab, NULL, GetModuleHandle(NULL), NULL);
    LayoutSmallControls(hPlazaTab, 185, 25, L"Name:", IDC_Z_NAME, 80);
    LayoutSmallControls(hPlazaTab, 185, 55, L"Level:", IDC_Z_LEVEL, 80);
    HWND hPlazaLevel = GetDlgItem(hPlazaTab, IDC_Z_LEVEL);
    SendMessageW(hPlazaLevel, EM_SETLIMITTEXT, 4, 0);

    LayoutSmallComboList(hPlazaTab, 185, 85, L"Rank:", IDC_Z_RANK, g_PlazaRankNames, 12, false, false, 80);

    LayoutSmallControls(hPlazaTab, 185, 115, L"Turf Inked:", IDC_Z_TURF, 80);
    HWND hPlazaTurf = GetDlgItem(hPlazaTab, IDC_Z_TURF);
    SendMessageW(hPlazaTurf, EM_SETLIMITTEXT, 7, 0);

    LayoutSmallComboList(hPlazaTab, 185, 145, L"Gender:", IDC_Z_GENDER, g_GenderNames, 3, false, false, 80);
    LayoutSmallComboList(hPlazaTab, 185, 175, L"Skin Color:", IDC_Z_SKIN, g_SkinColorNames, 8, false, true, 80);
    LayoutSmallComboList(hPlazaTab, 185, 205, L"Eye Color:", IDC_Z_EYE, g_EyeColorNames, 7, false, false, 80);

    CreateWindowW(L"BUTTON", L"Equipped Layout", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 465, 5, 280, 145, hPlazaTab, NULL, GetModuleHandle(NULL), NULL);
    LayoutSmallComboList(hPlazaTab, 475, 25, L"Hat:", IDC_Z_HEAD, g_HeadNames, g_HeadCount, true, false, 80);
    LayoutSmallComboList(hPlazaTab, 475, 55, L"Clothes:", IDC_Z_CLOTH, g_ClothNames, g_ClothCount, true, false, 80);
    LayoutSmallComboList(hPlazaTab, 475, 85, L"Shoes:", IDC_Z_SHOE, g_ShoeNames, g_ShoeCount, true, false, 80);
    LayoutSmallComboList(hPlazaTab, 475, 115, L"Weapon:", IDC_Z_WEAPON, g_WeaponNames, g_WeaponCount, true, false, 80);

    CreateWindowW(L"BUTTON", L"Fill Plaza with Fake Players", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 555, 250, 200, 30, hPlazaTab, (HMENU)IDC_Z_BTN_FILL_FAKE, GetModuleHandle(NULL), NULL);

    if (g_hFont) {
        EnumChildWindows(hPlazaTab, [](HWND hWnd, LPARAM lp) -> BOOL {
            SendMessageW(hWnd, WM_SETFONT, (WPARAM)lp, TRUE);
            return TRUE;
            }, (LPARAM)g_hFont);
    }
}

void LoadPlazaInkling(int index, SaveData* data) {
    if (index < 0 || index >= 25 || !data) return;

    PlazaInkling* ink = &data->plazaInklings[index];
    if ((int32_t)(uint32_t)ink->status == -1) {
        SetDlgItemTextW(hPlazaTab, IDC_Z_NAME, L"[Fake Player]");
        SetDlgItemTextW(hPlazaTab, IDC_Z_LEVEL, L"0");
        be32_t zero; zero = 0;
        be32_t firstWeapon; firstWeapon = 1000;
        WriteCombo32(hPlazaTab, IDC_Z_RANK, zero);
        WriteInt32(hPlazaTab, IDC_Z_TURF, zero);
        WriteCombo32(hPlazaTab, IDC_Z_GENDER, zero);
        WriteCombo32(hPlazaTab, IDC_Z_SKIN, zero);
        WriteCombo32(hPlazaTab, IDC_Z_EYE, zero);

        FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_HEAD), g_HeadNames, g_HeadCount, 0);
        WriteCombo32(hPlazaTab, IDC_Z_HEAD, zero);

        FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_CLOTH), g_ClothNames, g_ClothCount, 0);
        WriteCombo32(hPlazaTab, IDC_Z_CLOTH, zero);

        FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_SHOE), g_ShoeNames, g_ShoeCount, 0);
        WriteCombo32(hPlazaTab, IDC_Z_SHOE, zero);

        FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_WEAPON), g_WeaponNames, g_WeaponCount, 1000);
        WriteCombo32(hPlazaTab, IDC_Z_WEAPON, firstWeapon);
        return;
    }

    std::wstring nameStr = L"";
    for (int i = 0; i < 8; i++) {
        wchar_t c = (wchar_t)ink->name[i];
        if (c == 0) break;
        nameStr += c;
    }
    SetDlgItemTextW(hPlazaTab, IDC_Z_NAME, nameStr.c_str());
    if ((int32_t)(uint32_t)ink->level == -1) {
        SetDlgItemTextW(hPlazaTab, IDC_Z_LEVEL, L"-1");
    }
    else {
        SetDlgItemTextW(hPlazaTab, IDC_Z_LEVEL, std::to_wstring((int32_t)(uint32_t)ink->level + 1).c_str());
    }

    WriteCombo32(hPlazaTab, IDC_Z_RANK, ink->rank);
    WriteInt32(hPlazaTab, IDC_Z_TURF, ink->turfInked);
    WriteCombo32(hPlazaTab, IDC_Z_GENDER, ink->gender);
    WriteCombo32(hPlazaTab, IDC_Z_SKIN, ink->skinColor);
    WriteCombo32(hPlazaTab, IDC_Z_EYE, ink->eyeColor);

    FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_HEAD), g_HeadNames, g_HeadCount, (uint32_t)ink->headGear);
    WriteCombo32(hPlazaTab, IDC_Z_HEAD, ink->headGear);

    FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_CLOTH), g_ClothNames, g_ClothCount, (uint32_t)ink->clothGear);
    WriteCombo32(hPlazaTab, IDC_Z_CLOTH, ink->clothGear);

    FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_SHOE), g_ShoeNames, g_ShoeCount, (uint32_t)ink->shoeGear);
    WriteCombo32(hPlazaTab, IDC_Z_SHOE, ink->shoeGear);

    FillPlazaGearCombobox(GetDlgItem(hPlazaTab, IDC_Z_WEAPON), g_WeaponNames, g_WeaponCount, (uint32_t)ink->weapon);
    WriteCombo32(hPlazaTab, IDC_Z_WEAPON, ink->weapon);
}

static void SavePlazaInklingSlot(int index, SaveData* data) {
    if (index < 0 || index >= 25 || !data) return;

    PlazaInkling* ink = &data->plazaInklings[index];
    if ((int32_t)(uint32_t)ink->status == -1) return;

    wchar_t buf[256];
    GetDlgItemTextW(hPlazaTab, IDC_Z_NAME, buf, 256);
    for (int i = 0; i < 8; i++) {
        ink->name[i] = buf[i];
        if (buf[i] == 0) {
            for (int j = i + 1; j < 8; j++) ink->name[j] = 0;
            break;
        }
    }

    GetDlgItemTextW(hPlazaTab, IDC_Z_LEVEL, buf, 256);
    int32_t v = 0;
    swscanf_s(buf, L"%d", &v);
    if (v == -1) {
        ink->level = 0xFFFFFFFF;
    }
    else {
        if (v < 1) v = 1;
        if (v > 999) v = 999;
        ink->level = (uint32_t)(v - 1);
    }

    ReadCombo32(hPlazaTab, IDC_Z_RANK, ink->rank);
    ReadInt32(hPlazaTab, IDC_Z_TURF, ink->turfInked);
    if ((int32_t)ink->turfInked != -1) {
        if ((int32_t)ink->turfInked > 999999) ink->turfInked = 999999;
        if ((int32_t)ink->turfInked < 0) ink->turfInked = 0;
    }
    ReadCombo32(hPlazaTab, IDC_Z_GENDER, ink->gender);
    ReadCombo32(hPlazaTab, IDC_Z_SKIN, ink->skinColor);
    ReadCombo32(hPlazaTab, IDC_Z_EYE, ink->eyeColor);
    ReadCombo32(hPlazaTab, IDC_Z_HEAD, ink->headGear);
    ReadCombo32(hPlazaTab, IDC_Z_CLOTH, ink->clothGear);
    ReadCombo32(hPlazaTab, IDC_Z_SHOE, ink->shoeGear);
    ReadCombo32(hPlazaTab, IDC_Z_WEAPON, ink->weapon);
}

void UpdatePlazaTabUI(SaveData* data) {
    SendMessageW(hPlazaList, LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < 25; i++) {
        PlazaInkling* ink = &data->plazaInklings[i];
        if ((int32_t)(uint32_t)ink->status == -1) {
            SendMessageW(hPlazaList, LB_ADDSTRING, 0, (LPARAM)L"[Fake Player]");
        }
        else {
            std::wstring nameStr = L"";
            for (int j = 0; j < 8; j++) {
                wchar_t c = (wchar_t)ink->name[j];
                if (c == 0) break;
                nameStr += c;
            }
            if (nameStr.empty()) nameStr = L"[No Name]";
            SendMessageW(hPlazaList, LB_ADDSTRING, 0, (LPARAM)nameStr.c_str());
        }
    }
    currentPlazaIndex = 0;
    SendMessageW(hPlazaList, LB_SETCURSEL, 0, 0);
    LoadPlazaInkling(currentPlazaIndex, data);
}

void SavePlazaTabData(SaveData* data) {
    SavePlazaInklingSlot(currentPlazaIndex, data);
}

bool HandlePlazaTabCommand(HWND hControl, int wmEvent) {
    if (hControl == hPlazaList && wmEvent == LBN_SELCHANGE) {
        int newSel = (int)SendMessageW(hPlazaList, LB_GETCURSEL, 0, 0);
        if (newSel != LB_ERR && newSel != currentPlazaIndex) {
            SaveData* data = GetSaveData();
            if (data) {
                SavePlazaInklingSlot(currentPlazaIndex, data);
                currentPlazaIndex = newSel;
                LoadPlazaInkling(currentPlazaIndex, data);
            }
        }
        return true;
    }

    int id = GetDlgCtrlID(hControl);
    if (id == IDC_Z_BTN_FILL_FAKE && wmEvent == BN_CLICKED) {
        SaveData* data = GetSaveData();
        if (data) {
            for (int i = 0; i < 25; i++) {
                memset(&data->plazaInklings[i], 0, sizeof(PlazaInkling));
                data->plazaInklings[i].status = 0xFFFFFFFF; // -1 fake players
            }
            UpdatePlazaTabUI(data);
            MessageBoxW(g_hwndMain, L"The Plaza has been filled with fake players.", L"Spoon Editor", MB_OK | MB_ICONINFORMATION);
        }
        return true;
    }

    return false;
}
