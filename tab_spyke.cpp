#include "tabs.h"
#include "globals.h"
#include "ui_helpers.h"
#include <commctrl.h>
#include <string>
#include <time.h>

HWND hSpykeTab;
static const wchar_t* g_SpykeCategoryNames[] = { L"None", L"Shoes", L"Clothes", L"Hat" };
static const wchar_t* g_SpykeStateNames[] = { L"None", L"Coming Soon", L"Can Withdraw" };

void UpdateGearList(HWND hTab, int catId, int listId, uint32_t activeID) {
    HWND hCombo = GetDlgItem(hTab, listId);
    if (!hCombo) return;

    int catIdx = (int)SendMessageW(GetDlgItem(hTab, catId), CB_GETCURSEL, 0, 0);
    const wchar_t** names = nullptr;
    int count = 0;

    if (catIdx == 1) { names = g_ShoeNames; count = g_ShoeCount; }
    else if (catIdx == 2) { names = g_ClothNames; count = g_ClothCount; }
    else if (catIdx == 3) { names = g_HeadNames; count = g_HeadCount; }

    if (names) {
        FillPlazaGearCombobox(hCombo, names, count, activeID);
        SendMessageW(hCombo, CB_SETCURSEL, 0, 0);
    }
    else {
        SendMessageW(hCombo, CB_RESETCONTENT, 0, 0);
    }
}

static void CreateOrderBox(HWND hParent, int x, int y, const wchar_t* title, int idCat, int idGear, int idState, int idOrd, int idArr, int idNow) {
    CreateWindowW(L"BUTTON", title, WS_VISIBLE | WS_CHILD | BS_GROUPBOX, x, y, 235, 280, hParent, NULL, GetModuleHandle(NULL), NULL);

    auto LayoutTinyCombo = [&](int ly, const wchar_t* lbl, int idc, const wchar_t** items, int count) {
        CreateWindowW(L"STATIC", lbl, WS_VISIBLE | WS_CHILD, x + 10, y + ly + 1, 80, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
        HWND hC = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, x + 90, y + ly, 135, 200, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
        if (items) for (int i = 0; i < count; i++) SendMessageW(hC, CB_ADDSTRING, 0, (LPARAM)items[i]);
        };

    auto LayoutTinyEdit = [&](int ly, const wchar_t* lbl, int idc) {
        CreateWindowW(L"STATIC", lbl, WS_VISIBLE | WS_CHILD, x + 10, y + ly + 1, 80, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
        CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, x + 90, y + ly, 135, 21, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
        };

    LayoutTinyCombo(25, L"Category:", idCat, g_SpykeCategoryNames, 4);
    LayoutTinyCombo(55, L"Gear:", idGear, NULL, 0);
    LayoutTinyCombo(85, L"Status:", idState, g_SpykeStateNames, 3);

    LayoutTinyEdit(115, L"Ordered:", idOrd);
    LayoutTinyEdit(145, L"Arrival:", idArr);

    CreateWindowW(L"BUTTON", L"Set Times to Now", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x + 10, y + 180, 215, 25, hParent, (HMENU)(UINT_PTR)idNow, GetModuleHandle(NULL), NULL);
}

void InitSpykeTab(HWND hParent) {
    hSpykeTab = hParent;

    CreateOrderBox(hParent, 10, 5, L"Order 1", IDC_S_CAT_1, IDC_S_ID_1, IDC_S_STATE_1, IDC_S_ORD_1, IDC_S_ARR_1, IDC_S_NOW_1);
    CreateOrderBox(hParent, 250, 5, L"Order 2", IDC_S_CAT_2, IDC_S_ID_2, IDC_S_STATE_2, IDC_S_ORD_2, IDC_S_ARR_2, IDC_S_NOW_2);
    CreateOrderBox(hParent, 490, 5, L"Order 3", IDC_S_CAT_3, IDC_S_ID_3, IDC_S_STATE_3, IDC_S_ORD_3, IDC_S_ARR_3, IDC_S_NOW_3);

    if (g_hFont) {
        EnumChildWindows(hSpykeTab, [](HWND hwnd, LPARAM lp) -> BOOL {
            SendMessageW(hwnd, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            return TRUE;
            }, 0);
    }
}

static void SyncOrderUI(int idx, SaveData* data) {
    SpykeOrder& o = data->orders[idx];
    int catId = IDC_S_CAT_1 + idx;
    int gearId = IDC_S_ID_1 + idx;
    int stateId = IDC_S_STATE_1 + idx;
    int ordId = IDC_S_ORD_1 + idx;
    int arrId = IDC_S_ARR_1 + idx;

    int32_t catRaw = (int32_t)(uint32_t)o.category;
    int catSel = 0;
    if (catRaw == 0) catSel = 1;
    else if (catRaw == 1) catSel = 2;
    else if (catRaw == 2) catSel = 3;

    SendMessageW(GetDlgItem(hSpykeTab, catId), CB_SETCURSEL, catSel, 0);
    UpdateGearList(hSpykeTab, catId, gearId, (uint32_t)o.gearID);

    WriteCombo32(hSpykeTab, gearId, o.gearID);

    int32_t stateRaw = (int32_t)(uint32_t)o.state;
    int stateSel = 0;
    if (stateRaw == 0) stateSel = 1;
    else if (stateRaw == 1) stateSel = 2;
    SendMessageW(GetDlgItem(hSpykeTab, stateId), CB_SETCURSEL, stateSel, 0);

    WriteTime32(hSpykeTab, ordId, o.orderedTime);
    WriteTime32(hSpykeTab, arrId, o.arrivalTime);
}

void UpdateSpykeTabUI(SaveData* data) {
    if (!data) return;
    SyncOrderUI(0, data);
    SyncOrderUI(1, data);
    SyncOrderUI(2, data);
}

static void SyncOrderSave(int idx, SaveData* data) {
    SpykeOrder& o = data->orders[idx];
    int catId = IDC_S_CAT_1 + idx;
    int gearId = IDC_S_ID_1 + idx;
    int stateId = IDC_S_STATE_1 + idx;
    int ordId = IDC_S_ORD_1 + idx;
    int arrId = IDC_S_ARR_1 + idx;

    int catSel = (int)SendMessageW(GetDlgItem(hSpykeTab, catId), CB_GETCURSEL, 0, 0);
    if (catSel == 0) o.category = (uint32_t)-1;
    else o.category = (uint32_t)(catSel - 1);

    ReadInt32(hSpykeTab, gearId, o.gearID);

    int stateSel = (int)SendMessageW(GetDlgItem(hSpykeTab, stateId), CB_GETCURSEL, 0, 0);
    if (stateSel == 0) o.state = (uint32_t)-1;
    else o.state = (uint32_t)(stateSel - 1);

    ReadTime32(hSpykeTab, ordId, o.orderedTime);
    ReadTime32(hSpykeTab, arrId, o.arrivalTime);
}

void SaveSpykeTabData(SaveData* data) {
    if (!data) return;
    SyncOrderSave(0, data);
    SyncOrderSave(1, data);
    SyncOrderSave(2, data);
}

bool HandleSpykeTabCommand(HWND hControl, int wmEvent) {
    int id = GetDlgCtrlID(hControl);
    if (wmEvent == CBN_SELCHANGE) {
        if (id == IDC_S_CAT_1) UpdateGearList(hSpykeTab, IDC_S_CAT_1, IDC_S_ID_1);
        if (id == IDC_S_CAT_2) UpdateGearList(hSpykeTab, IDC_S_CAT_2, IDC_S_ID_2);
        if (id == IDC_S_CAT_3) UpdateGearList(hSpykeTab, IDC_S_CAT_3, IDC_S_ID_3);
        return true;
    }
    if (wmEvent == BN_CLICKED) {
        if (id >= IDC_S_NOW_1 && id <= IDC_S_NOW_3) {
            int idx = id - IDC_S_NOW_1;
            uint32_t t = (uint32_t)time(NULL);
            be32_t t_be; t_be = t;
            be32_t t_arr; t_arr = t + 86400;
            WriteTime32(hSpykeTab, IDC_S_ORD_1 + idx, t_be);
            WriteTime32(hSpykeTab, IDC_S_ARR_1 + idx, t_arr);
            return true;
        }
    }
    return false;
}
