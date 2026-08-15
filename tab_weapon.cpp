#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

static HWND hWeaponTab;
static HWND hWeaponList;
static SaveData* g_lastData = nullptr;

static int GetWeaponIDFromList(int index) {
    if (index < 0 || index >= g_WeaponCount) return -1;
    const wchar_t* name = g_WeaponNames[index];
    int id = -1;
    swscanf_s(name, L"%d", &id);
    return id;
}

static const wchar_t* GetWeaponNameByID(int id) {
    for (int i = 0; i < g_WeaponCount; i++) {
        if (GetWeaponIDFromList(i) == id) return g_WeaponNames[i];
    }
    return L"Unknown Weapon";
}

void InitWeaponTab(HWND hParent) {
    hWeaponTab = hParent;

    LayoutCombo(hWeaponTab, 20, 20, L"Equipped Weapon:", IDC_W_WEAPON, g_WeaponNames, g_WeaponCount);

    CreateWindowW(L"BUTTON", L"Weapon Inventory", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 55, 370, 260, hWeaponTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"STATIC", L"Inventory (Turf Inked):", WS_CHILD | WS_VISIBLE, 20, 75, 250, 20, hWeaponTab, NULL, GetModuleHandle(NULL), NULL);
    hWeaponList = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | WS_TABSTOP | LBS_EXTENDEDSEL, 20, 95, 350, 140, hWeaponTab, (HMENU)IDC_W_LIST, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"BUTTON", L"Edit Selected Attributes", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 390, 55, 240, 170, hWeaponTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"STATIC", L"Turf Inked:", WS_CHILD | WS_VISIBLE, 405, 80, 100, 20, hWeaponTab, NULL, GetModuleHandle(NULL), NULL);
    HWND hTurf = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 405, 100, 140, 24, hWeaponTab, (HMENU)IDC_W_TURF_INKED, GetModuleHandle(NULL), NULL);
    SendMessageW(hTurf, EM_SETLIMITTEXT, 7, 0);

    CreateWindowW(L"BUTTON", L"Mark as 'New'", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, 405, 135, 120, 24, hWeaponTab, (HMENU)IDC_W_FLAG_NEW, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"BUTTON", L"Apply Stats", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 405, 175, 140, 30, hWeaponTab, (HMENU)IDC_W_BTN_SET_TURF, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"STATIC", L"Add New Weapon:", WS_CHILD | WS_VISIBLE, 20, 245, 200, 20, hWeaponTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 20, 265, 200, 200, hWeaponTab, (HMENU)IDC_W_ADD_PICKER, GetModuleHandle(NULL), NULL);
    HWND btnAdd = CreateWindowW(L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 230, 264, 60, 26, hWeaponTab, (HMENU)IDC_W_BTN_ADD, GetModuleHandle(NULL), NULL);
    HWND btnDel = CreateWindowW(L"BUTTON", L"Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 300, 264, 70, 26, hWeaponTab, (HMENU)IDC_W_BTN_DEL, GetModuleHandle(NULL), NULL);

    if (g_hFont) {
        EnumChildWindows(hWeaponTab, [](HWND hWnd, LPARAM lp) -> BOOL {
            SendMessageW(hWnd, WM_SETFONT, lp, TRUE);
            return TRUE;
            }, (LPARAM)g_hFont);
    }
}

static void RefreshOwnedList() {
    SendMessageW(hWeaponList, LB_RESETCONTENT, 0, 0);

    if (!g_lastData) return;

    struct OwnedWeaponItem {
        std::wstring display;
        uint32_t turfInked;
        int originalIndex;
    };

    std::vector<OwnedWeaponItem> list;
    for (int i = 0; i < 128; i++) {
        uint32_t id = g_lastData->ownedWeapons[i].id;
        if (id == 0 || id == 0xFFFFFFFF) continue;

        OwnedWeaponItem item;
        item.originalIndex = i;
        item.turfInked = (uint32_t)g_lastData->ownedWeapons[i].turfInked;
        item.display = GetDisplayItemText(GetWeaponNameByID(id));
        list.push_back(item);
    }

    if (g_config.sortByAlphabet) {
        std::sort(list.begin(), list.end(), [](const OwnedWeaponItem& a, const OwnedWeaponItem& b) {
            return _wcsicmp(a.display.c_str(), b.display.c_str()) < 0;
            });
    }

    for (const auto& item : list) {
        wchar_t buf[256];
        swprintf_s(buf, L"%s - %dpt", item.display.c_str(), (int32_t)item.turfInked);
        int idx = (int)SendMessageW(hWeaponList, LB_ADDSTRING, 0, (LPARAM)buf);
        SendMessageW(hWeaponList, LB_SETITEMDATA, idx, item.originalIndex);
    }
}

static void RefreshAddPicker() {
    HWND hPicker = GetDlgItem(hWeaponTab, IDC_W_ADD_PICKER);
    if (!hPicker) return;

    if (!g_lastData) {
        SendMessageW(hPicker, CB_RESETCONTENT, 0, 0);
        return;
    }

    std::vector<const wchar_t*> unownedList;
    for (int i = 0; i < g_WeaponCount; i++) {
        int id = GetWeaponIDFromList(i);
        bool owned = false;
        for (int j = 0; j < 128; j++) {
            if (g_lastData->ownedWeapons[j].id == (uint32_t)id) {
                owned = true;
                break;
            }
        }
        if (!owned) {
            unownedList.push_back(g_WeaponNames[i]);
        }
    }

    FillCombobox(hPicker, unownedList.data(), (int)unownedList.size(), false);
    SendMessageW(hPicker, CB_SETCURSEL, 0, 0);
}

void UpdateWeaponTabUI(SaveData* data) {
    g_lastData = data;
    WriteCombo32(hWeaponTab, IDC_W_WEAPON, data->currentWeapon);
    RefreshOwnedList();
    RefreshAddPicker();
}

void SaveWeaponTabData(SaveData* data) {
    ReadCombo32(hWeaponTab, IDC_W_WEAPON, data->currentWeapon);
}

bool HandleWeaponTabCommand(HWND hControl, int wmEvent) {
    if (!g_lastData || !hControl) return false;

    int id = GetDlgCtrlID(hControl);
    HWND hParent = GetParent(hControl);

    if (id == IDC_W_LIST && wmEvent == LBN_SELCHANGE) {
        int count = (int)SendMessageW(hControl, LB_GETSELCOUNT, 0, 0);
        if (count == 1) {
            int sel;
            SendMessageW(hControl, LB_GETSELITEMS, 1, (LPARAM)&sel);
            int arrayIdx = (int)SendMessageW(hControl, LB_GETITEMDATA, sel, 0);
            wchar_t buf[32] = { 0 };
            swprintf_s(buf, 32, L"%d", (int32_t)(uint32_t)g_lastData->ownedWeapons[arrayIdx].turfInked);

            SetDlgItemTextW(hParent, IDC_W_TURF_INKED, buf);
            CheckDlgButton(hParent, IDC_W_FLAG_NEW, g_lastData->ownedWeapons[arrayIdx].flags == 1 ? BST_CHECKED : BST_UNCHECKED);
        }
        else {
            SetDlgItemTextW(hParent, IDC_W_TURF_INKED, L"");
            CheckDlgButton(hParent, IDC_W_FLAG_NEW, BST_UNCHECKED);
        }
        return true;
    }

    if (id == IDC_W_BTN_SET_TURF && wmEvent == BN_CLICKED) {
        int count = (int)SendMessageW(hWeaponList, LB_GETSELCOUNT, 0, 0);
        if (count == 1) {
            int sel = 0;
            SendMessageW(hWeaponList, LB_GETSELITEMS, 1, (LPARAM)&sel);
            int arrayIdx = (int)SendMessageW(hWeaponList, LB_GETITEMDATA, sel, 0);
            wchar_t buf[32] = { 0 };
            GetDlgItemTextW(hParent, IDC_W_TURF_INKED, buf, 32);
            int32_t val = 0;
            swscanf_s(buf, L"%d", &val);
            if (val > 999999) val = 999999;
            if (val < -999999) val = -999999;
            g_lastData->ownedWeapons[arrayIdx].turfInked = (uint32_t)val;

            g_lastData->ownedWeapons[arrayIdx].flags = (IsDlgButtonChecked(hParent, IDC_W_FLAG_NEW) == BST_CHECKED ? 1 : 0);

            RefreshOwnedList();
            SendMessageW(hWeaponList, LB_SETSEL, TRUE, sel);
            MessageBoxW(hParent, L"Weapon stats applied!", L"Success", MB_OK);
        }
        else {
            MessageBoxW(hParent, L"Please select exactly one weapon to apply stats.", L"Error", MB_OK | MB_ICONERROR);
        }
        return true;
    }

    if (id == IDC_W_BTN_ADD && wmEvent == BN_CLICKED) {
        HWND hPicker = GetDlgItem(hParent, IDC_W_ADD_PICKER);
        int sel = (int)SendMessageW(hPicker, CB_GETCURSEL, 0, 0);
        if (sel != CB_ERR) {
            wchar_t buf[256] = { 0 };
            SendMessageW(hPicker, CB_GETLBTEXT, sel, (LPARAM)buf);
            int weaponID = -1;
            swscanf_s(buf, L"%d", &weaponID);

            int emptyIdx = -1;
            for (int i = 0; i < 128; i++) {
                if (g_lastData->ownedWeapons[i].id == 0 || g_lastData->ownedWeapons[i].id == 0xFFFFFFFF) {
                    emptyIdx = i;
                    break;
                }
            }

            if (emptyIdx != -1) {
                g_lastData->ownedWeapons[emptyIdx].id = weaponID;
                g_lastData->ownedWeapons[emptyIdx].turfInked = 0;
                g_lastData->ownedWeapons[emptyIdx].flags = 1;
                RefreshOwnedList();
                RefreshAddPicker();
            }
            else {
                MessageBoxW(hParent, L"Weapon list is full.", L"Error", MB_OK | MB_ICONERROR);
            }
        }
        return true;
    }

    if (id == IDC_W_BTN_DEL && wmEvent == BN_CLICKED) {
        int count = (int)SendMessageW(hWeaponList, LB_GETSELCOUNT, 0, 0);
        if (count > 0) {
            wchar_t msg[128];
            swprintf_s(msg, L"Delete %d selected weapon(s)?", count);
            if (MessageBoxW(hParent, msg, L"Confirm", MB_YESNO | MB_ICONWARNING) == IDYES) {
                std::vector<int> selections(count);
                SendMessageW(hWeaponList, LB_GETSELITEMS, count, (LPARAM)selections.data());

                for (int i = 0; i < count; i++) {
                    int arrayIdx = (int)SendMessageW(hWeaponList, LB_GETITEMDATA, selections[i], 0);
                    g_lastData->ownedWeapons[arrayIdx].id = 0xFFFFFFFF;
                }

                RefreshOwnedList();
                RefreshAddPicker();

                SetDlgItemTextW(hParent, IDC_W_TURF_INKED, L"");
                CheckDlgButton(hParent, IDC_W_FLAG_NEW, BST_UNCHECKED);
            }
        }
        return true;
    }

    return false;
}
