#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

static SaveData* g_lastData = nullptr;

static int GetItemIDFromList(const wchar_t** list, int count, int index) {
    if (index < 0 || index >= count) return -1;
    const wchar_t* name = list[index];
    int id = -1;
    swscanf_s(name, L"[%d]", &id);
    return id;
}

static const wchar_t* GetItemNameByID(const wchar_t** list, int count, int id) {
    for (int i = 0; i < count; i++) {
        int listID = -1;
        swscanf_s(list[i], L"%d", &listID);
        if (listID == id) return list[i];
    }
    return L"Unknown Gear";
}

static void LayoutInventoryTab(HWND hSubTab, const wchar_t* equipLabel, int equipIdc, const wchar_t** equipNames, int equipCount) {
    // 1. Equipped Gear Section
    LayoutCombo(hSubTab, 20, 20, equipLabel, equipIdc, equipNames, equipCount);

    // 2. Inventory Selection
    CreateWindowW(L"BUTTON", L"Gear Inventory", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 55, 370, 275, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"STATIC", L"Inventory (Select to Edit):", WS_CHILD | WS_VISIBLE, 20, 75, 250, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | WS_TABSTOP | LBS_NOINTEGRALHEIGHT | LBS_EXTENDEDSEL, 20, 95, 350, 166, hSubTab, (HMENU)IDC_G_LIST, GetModuleHandle(NULL), NULL);

    // 3. Ability Editor
    CreateWindowW(L"BUTTON", L"Edit Selected Attributes", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 390, 55, 330, 230, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    int editX = 405;
    int yBase = 90;
    CreateWindowW(L"STATIC", L"Ability 1:", WS_CHILD | WS_VISIBLE, editX, yBase, 100, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb1 = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, editX, yBase + 20, 180, 400, hSubTab, (HMENU)IDC_G_SUB1, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"STATIC", L"Ability 2:", WS_CHILD | WS_VISIBLE, editX, yBase + 50, 100, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb2 = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, editX, yBase + 70, 180, 400, hSubTab, (HMENU)IDC_G_SUB2, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"STATIC", L"Ability 3:", WS_CHILD | WS_VISIBLE, editX, yBase + 100, 100, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb3 = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, editX, yBase + 120, 180, 400, hSubTab, (HMENU)IDC_G_SUB3, GetModuleHandle(NULL), NULL);

    FillAbilityCombobox(hCb1);
    FillAbilityCombobox(hCb2);
    FillAbilityCombobox(hCb3);

    int statX = 600;
    CreateWindowW(L"STATIC", L"Level:", WS_CHILD | WS_VISIBLE, statX, yBase + 20, 50, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    HWND hGLevel = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, statX + 45, yBase + 17, 40, 24, hSubTab, (HMENU)IDC_G_LEVEL, GetModuleHandle(NULL), NULL);
    SendMessageW(hGLevel, EM_SETLIMITTEXT, 1, 0);

    CreateWindowW(L"STATIC", L"Slots:", WS_CHILD | WS_VISIBLE, statX, yBase + 50, 50, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    HWND hGSlots = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, statX + 45, yBase + 47, 40, 24, hSubTab, (HMENU)IDC_G_SLOTS, GetModuleHandle(NULL), NULL);
    SendMessageW(hGSlots, EM_SETLIMITTEXT, 1, 0);

    CreateWindowW(L"STATIC", L"Exp:", WS_CHILD | WS_VISIBLE, statX, yBase + 80, 50, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP, statX + 45, yBase + 77, 65, 24, hSubTab, (HMENU)IDC_G_EXP, GetModuleHandle(NULL), NULL);

    CreateWindowW(L"BUTTON", L"Mark as 'New'", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, statX, yBase + 115, 110, 24, hSubTab, (HMENU)IDC_G_FLAG_NEW, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Apply Stats", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 560, 240, 140, 32, hSubTab, (HMENU)IDC_G_BTN_APPLY, GetModuleHandle(NULL), NULL);

    // 4. Add / Delete Gear Section
    CreateWindowW(L"STATIC", L"Add New Gear:", WS_CHILD | WS_VISIBLE, 20, 275, 200, 20, hSubTab, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExW(WS_EX_CLIENTEDGE, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 20, 295, 200, 300, hSubTab, (HMENU)IDC_G_ADD_PICKER, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 230, 294, 60, 26, hSubTab, (HMENU)IDC_G_BTN_ADD, GetModuleHandle(NULL), NULL);
    CreateWindowW(L"BUTTON", L"Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 300, 294, 70, 26, hSubTab, (HMENU)IDC_G_BTN_DEL, GetModuleHandle(NULL), NULL);
}

void InitGearTab(HWND hParent) {
    g_hwndGearTab = CreateWindowExW(0, WC_TABCONTROLW, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 5, 5, 730, 450, hParent, NULL, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(g_hwndGearTab, WM_SETFONT, (WPARAM)g_hFont, TRUE);

    const wchar_t* subTabs[] = { L"Head", L"Clothes", L"Shoes" };
    for (int i = 0; i < 3; ++i) {
        TCITEMW tie = { 0 };
        tie.mask = TCIF_TEXT;
        tie.pszText = (LPWSTR)subTabs[i];
        TabCtrl_InsertItem(g_hwndGearTab, i, &tie);
        g_hwndGearSubChild[i] = CreateChildTab(g_hwndGearTab);
    }

    LayoutInventoryTab(g_hwndGearSubChild[GEAR_SUB_HEAD], L"Equipped Head:", IDC_G_HEAD_EQUIP, g_HeadNames, g_HeadCount);
    LayoutInventoryTab(g_hwndGearSubChild[GEAR_SUB_CLOTHES], L"Equipped Clothes:", IDC_G_CLOTH_EQUIP, g_ClothNames, g_ClothCount);
    LayoutInventoryTab(g_hwndGearSubChild[GEAR_SUB_SHOES], L"Equipped Shoes:", IDC_G_SHOE_EQUIP, g_ShoeNames, g_ShoeCount);

    if (g_hFont) {
        for (int i = 0; i < 3; i++) {
            EnumChildWindows(g_hwndGearSubChild[i], [](HWND hWnd, LPARAM lp) -> BOOL {
                SendMessageW(hWnd, WM_SETFONT, (WPARAM)lp, TRUE);
                return TRUE;
                }, (LPARAM)g_hFont);
        }
    }

    ShowWindow(g_hwndGearSubChild[0], SW_SHOW);
}

static void WriteAbilityCombo(HWND hParent, int idc, int abilityID) {
    HWND hCb = GetDlgItem(hParent, idc);
    if (!hCb) return;
    int count = (int)SendMessageW(hCb, CB_GETCOUNT, 0, 0);
    for (int i = 0; i < count; i++) {
        if ((int)SendMessageW(hCb, CB_GETITEMDATA, i, 0) == abilityID) {
            SendMessageW(hCb, CB_SETCURSEL, i, 0);
            return;
        }
    }
    SendMessageW(hCb, CB_SETCURSEL, 0, 0);
}

static int ReadAbilityCombo(HWND hParent, int idc) {
    HWND hCb = GetDlgItem(hParent, idc);
    if (!hCb) return -2;
    int sel = (int)SendMessageW(hCb, CB_GETCURSEL, 0, 0);
    if (sel == CB_ERR) return -2;
    return (int)SendMessageW(hCb, CB_GETITEMDATA, sel, 0);
}

static OwnedGearS1* GetGearArray(int tabIndex) {
    if (!g_lastData) return nullptr;
    if (tabIndex == GEAR_SUB_HEAD)    return g_lastData->head;
    if (tabIndex == GEAR_SUB_CLOTHES) return g_lastData->clothes;
    if (tabIndex == GEAR_SUB_SHOES)   return g_lastData->shoes;
    return nullptr;
}

static const wchar_t** GetGearNames(int tabIndex, int* outCount) {
    if (tabIndex == GEAR_SUB_HEAD) { *outCount = g_HeadCount;  return g_HeadNames; }
    if (tabIndex == GEAR_SUB_CLOTHES) { *outCount = g_ClothCount; return g_ClothNames; }
    if (tabIndex == GEAR_SUB_SHOES) { *outCount = g_ShoeCount;  return g_ShoeNames; }
    *outCount = 0; return nullptr;
}

static void RefreshTabList(int tabIndex) {
    if (!g_lastData) return;
    HWND hSubTab = g_hwndGearSubChild[tabIndex];
    HWND hList = GetDlgItem(hSubTab, IDC_G_LIST);
    SendMessageW(hList, LB_RESETCONTENT, 0, 0);

    int count = 0;
    const wchar_t** names = GetGearNames(tabIndex, &count);
    OwnedGearS1* arr = GetGearArray(tabIndex);
    if (!arr) return;

    struct OwnedGearItem {
        std::wstring display;
        uint32_t id;
        int originalIndex;
    };

    std::vector<OwnedGearItem> list;
    std::vector<uint32_t> seenIDs;
    for (int i = 0; i < 256; i++) {
        uint32_t id = arr[i].id;
        if (id == 0xFFFFFFFF) continue;

        // Skip duplicate IDs — show only first occurrence
        if (std::find(seenIDs.begin(), seenIDs.end(), id) != seenIDs.end()) continue;
        seenIDs.push_back(id);

        OwnedGearItem item;
        item.originalIndex = i;
        item.id = id;
        item.display = GetDisplayItemText(GetItemNameByID(names, count, id));
        list.push_back(item);
    }

    if (g_config.sortByAlphabet) {
        std::sort(list.begin(), list.end(), [](const OwnedGearItem& a, const OwnedGearItem& b) {
            return _wcsicmp(a.display.c_str(), b.display.c_str()) < 0;
            });
    }
    else {
        std::sort(list.begin(), list.end(), [](const OwnedGearItem& a, const OwnedGearItem& b) {
            return a.id < b.id;
            });
    }

    for (const auto& item : list) {
        wchar_t buf[256] = { 0 };
        swprintf_s(buf, 256, L"%ls", item.display.c_str());
        int idx = (int)SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)buf);
        SendMessageW(hList, LB_SETITEMDATA, idx, item.originalIndex);
    }
}

static void RefreshAddPicker(int tabIndex) {
    if (!g_lastData) return;
    HWND hSubTab = g_hwndGearSubChild[tabIndex];
    HWND hPicker = GetDlgItem(hSubTab, IDC_G_ADD_PICKER);
    if (!hPicker) return;

    int count = 0;
    const wchar_t** names = GetGearNames(tabIndex, &count);
    OwnedGearS1* arr = GetGearArray(tabIndex);
    if (!arr) {
        SendMessageW(hPicker, CB_RESETCONTENT, 0, 0);
        return;
    }

    std::vector<const wchar_t*> unownedList;
    for (int i = 0; i < count; i++) {
        const wchar_t* name = names[i];

        // Block adding test tees
        if (wcsstr(name, L"(Test_Clt_") || wcsstr(name, L"(HAP")) {
            continue;
        }

        int gearID = -1;
        swscanf_s(name, L"%d", &gearID);
        if (gearID < 0) continue;

        // Only allow test tees slot adding when they already have one in the save
        bool owned = false;
        for (int j = 0; j < 256; j++) {
            if (arr[j].id != 0xFFFFFFFF && arr[j].id == (uint32_t)gearID) {
                owned = true;
                break;
            }
        }
        if (!owned) {
            unownedList.push_back(name);
        }
    }

    FillCombobox(hPicker, unownedList.data(), (int)unownedList.size(), false);
    SendMessageW(hPicker, CB_SETCURSEL, 0, 0);
}

void UpdateGearTabUI(SaveData* data) {
    g_lastData = data;

    WriteCombo32(g_hwndGearSubChild[GEAR_SUB_HEAD], IDC_G_HEAD_EQUIP, data->currentHeadGear);
    WriteCombo32(g_hwndGearSubChild[GEAR_SUB_CLOTHES], IDC_G_CLOTH_EQUIP, data->currentClothGear);
    WriteCombo32(g_hwndGearSubChild[GEAR_SUB_SHOES], IDC_G_SHOE_EQUIP, data->currentShoeGear);

    RefreshTabList(GEAR_SUB_HEAD);
    RefreshTabList(GEAR_SUB_CLOTHES);
    RefreshTabList(GEAR_SUB_SHOES);

    RefreshAddPicker(GEAR_SUB_HEAD);
    RefreshAddPicker(GEAR_SUB_CLOTHES);
    RefreshAddPicker(GEAR_SUB_SHOES);
}

void SaveGearTabData(SaveData* data) {
    ReadCombo32(g_hwndGearSubChild[GEAR_SUB_HEAD], IDC_G_HEAD_EQUIP, data->currentHeadGear);
    ReadCombo32(g_hwndGearSubChild[GEAR_SUB_CLOTHES], IDC_G_CLOTH_EQUIP, data->currentClothGear);
    ReadCombo32(g_hwndGearSubChild[GEAR_SUB_SHOES], IDC_G_SHOE_EQUIP, data->currentShoeGear);
}

bool HandleGearTabCommand(HWND hControl, int wmEvent) {
    if (!g_lastData || !hControl) return false;

    int id = GetDlgCtrlID(hControl);
    HWND hParent = GetParent(hControl);

    int category = -1;
    if (hParent == g_hwndGearSubChild[GEAR_SUB_HEAD])    category = GEAR_SUB_HEAD;
    else if (hParent == g_hwndGearSubChild[GEAR_SUB_CLOTHES]) category = GEAR_SUB_CLOTHES;
    else if (hParent == g_hwndGearSubChild[GEAR_SUB_SHOES])   category = GEAR_SUB_SHOES;

    if (category == -1) return false;

    OwnedGearS1* arr = GetGearArray(category);

    if (id == IDC_G_LIST && wmEvent == LBN_SELCHANGE) {
        int selCount = (int)SendMessageW(hControl, LB_GETSELCOUNT, 0, 0);
        if (selCount > 0) {
            std::vector<int> selections(selCount);
            SendMessageW(hControl, LB_GETSELITEMS, selCount, (LPARAM)selections.data());
            int sel = selections[0];

            int arrayIdx = (int)SendMessageW(hControl, LB_GETITEMDATA, sel, 0);
            OwnedGearS1& gear = arr[arrayIdx];

            WriteAbilityCombo(hParent, IDC_G_SUB1, (int)gear.sub1);
            WriteAbilityCombo(hParent, IDC_G_SUB2, (int)gear.sub2);
            WriteAbilityCombo(hParent, IDC_G_SUB3, (int)gear.sub3);

            WriteInt32(hParent, IDC_G_LEVEL, gear.level);
            WriteInt32(hParent, IDC_G_SLOTS, gear.unlockedSlots);
            WriteInt32(hParent, IDC_G_EXP, gear.exp);

            CheckDlgButton(hParent, IDC_G_FLAG_NEW, gear.newFlag == 1 ? BST_CHECKED : BST_UNCHECKED);
        }
        return true;
    }

    if (id == IDC_G_BTN_APPLY && wmEvent == BN_CLICKED) {
        HWND hList = GetDlgItem(hParent, IDC_G_LIST);
        int selCount = (int)SendMessageW(hList, LB_GETSELCOUNT, 0, 0);
        if (selCount > 0) {
            std::vector<int> selections(selCount);
            SendMessageW(hList, LB_GETSELITEMS, selCount, (LPARAM)selections.data());

            for (int i = 0; i < selCount; i++) {
                int arrayIdx = (int)SendMessageW(hList, LB_GETITEMDATA, selections[i], 0);
                OwnedGearS1& gear = arr[arrayIdx];

                gear.sub1 = (uint32_t)ReadAbilityCombo(hParent, IDC_G_SUB1);
                gear.sub2 = (uint32_t)ReadAbilityCombo(hParent, IDC_G_SUB2);
                gear.sub3 = (uint32_t)ReadAbilityCombo(hParent, IDC_G_SUB3);

                ReadInt32(hParent, IDC_G_LEVEL, gear.level);
                if ((int32_t)gear.level < 1) gear.level = 1;
                if ((int32_t)gear.level > 4) gear.level = 4;

                ReadInt32(hParent, IDC_G_SLOTS, gear.unlockedSlots);
                if ((int32_t)gear.unlockedSlots < 1) gear.unlockedSlots = 1;
                if ((int32_t)gear.unlockedSlots > 4) gear.unlockedSlots = 4;

                ReadInt32(hParent, IDC_G_EXP, gear.exp);

                gear.newFlag = (IsDlgButtonChecked(hParent, IDC_G_FLAG_NEW) == BST_CHECKED ? 1 : 0);
            }
            if (selCount == 1) {
                MessageBoxW(hParent, L"Gear stats applied!", L"Success", MB_OK);
            } else {
                MessageBoxW(hParent, L"Gear stats applied to multiple items!", L"Success", MB_OK);
            }
        }
        return true;
    }

    if (id == IDC_G_BTN_ADD && wmEvent == BN_CLICKED) {
        HWND hPicker = GetDlgItem(hParent, IDC_G_ADD_PICKER);
        int sel = (int)SendMessageW(hPicker, CB_GETCURSEL, 0, 0);
        if (sel != CB_ERR) {
            wchar_t buf[256] = { 0 };
            SendMessageW(hPicker, CB_GETLBTEXT, sel, (LPARAM)buf);
            int gearID = -1;
            swscanf_s(buf, L"%d", &gearID);

            int emptyIdx = -1;
            for (int i = 0; i < 256; i++) {
                if ((int32_t)(uint32_t)arr[i].id == -1) {
                    emptyIdx = i;
                    break;
                }
            }

            if (emptyIdx != -1) {
                arr[emptyIdx].id = (uint32_t)gearID;
                arr[emptyIdx].level = 1;
                arr[emptyIdx].unlockedSlots = 1;
                arr[emptyIdx].sub1 = 0;
                arr[emptyIdx].sub2 = 0;
                arr[emptyIdx].sub3 = 0;
                arr[emptyIdx].exp = 0;
                arr[emptyIdx].newFlag = 1;
                RefreshTabList(category);
                RefreshAddPicker(category);
            }
            else {
                MessageBoxW(hParent, L"Gear inventory is full (256 items).", L"Error", MB_OK | MB_ICONERROR);
            }
        }
        return true;
    }

    if (id == IDC_G_BTN_DEL && wmEvent == BN_CLICKED) {
        HWND hList = GetDlgItem(hParent, IDC_G_LIST);
        int selCount = (int)SendMessageW(hList, LB_GETSELCOUNT, 0, 0);
        if (selCount > 0) {
            if (MessageBoxW(hParent, L"Delete selected gear item(s)?", L"Confirm", MB_YESNO) == IDYES) {
                std::vector<int> selections(selCount);
                SendMessageW(hList, LB_GETSELITEMS, selCount, (LPARAM)selections.data());

                for (int i = 0; i < selCount; i++) {
                    int arrayIdx = (int)SendMessageW(hList, LB_GETITEMDATA, selections[i], 0);
                    arr[arrayIdx].id = 0xFFFFFFFF;
                }
                RefreshTabList(category);
                RefreshAddPicker(category);
            }
        }
        return true;
    }

    return false;
}
