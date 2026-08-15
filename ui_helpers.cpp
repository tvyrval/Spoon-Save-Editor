#include "ui_helpers.h"
#include "globals.h"
#include <commctrl.h>
#include <string>
#include <stdio.h>
#include <ctime>
#include <vector>
#include <algorithm>

int32_t ParseItemID(const wchar_t* str) {
    if (!str) return -1;
    int32_t id = -1;
    const wchar_t* p = str;
    while (*p == L' ' || *p == L'[') p++;
    if (swscanf_s(p, L"%d", &id) == 1) {
        return id;
    }
    return -1;
}

std::wstring GetDisplayItemText(const wchar_t* str) {
    if (!str) return L"";
    if (g_config.showInternalIDs) {
        return str;
    }
    const wchar_t* start = wcschr(str, L'(');
    if (start) {
        start++;
        const wchar_t* end = wcschr(start, L')');
        if (end) {
            return std::wstring(start, end - start);
        }
    }
    int32_t id = -1;
    const wchar_t* p = str;
    int charsRead = 0;
    if (swscanf_s(p, L"%d %n", &id, &charsRead) == 1 && charsRead > 0) {
        return p + charsRead;
    }
    return str;
}

struct ComboboxItem {
    std::wstring display;
    int32_t id = 0;
    const wchar_t* raw = nullptr;
};

void FillCombobox(HWND hCb, const wchar_t** items, int itemCount, bool filter, bool disableSort) {
    if (!hCb) return;
    SendMessageW(hCb, CB_RESETCONTENT, 0, 0);
    std::vector<ComboboxItem> list;
    for (int i = 0; i < itemCount; ++i) {
        if (filter && items[i]) {
            if (wcsstr(items[i], L"(Test_Clt_") || wcsstr(items[i], L"(HAP")) {
                continue;
            }
        }
        ComboboxItem item;
        item.raw = items[i];
        item.id = ParseItemID(items[i]);
        item.display = GetDisplayItemText(items[i]);
        list.push_back(item);
    }

    if (g_config.sortByAlphabet && !disableSort) {
        std::sort(list.begin(), list.end(), [](const ComboboxItem& a, const ComboboxItem& b) {
            return _wcsicmp(a.display.c_str(), b.display.c_str()) < 0;
            });
    }

    for (const auto& item : list) {
        int idx = (int)SendMessageW(hCb, CB_ADDSTRING, 0, (LPARAM)item.display.c_str());
        SendMessageW(hCb, CB_SETITEMDATA, idx, item.id);
    }
}

void FillAbilityCombobox(HWND hCb) {
    if (!hCb) return;
    SendMessageW(hCb, CB_RESETCONTENT, 0, 0);

    struct AbilityItem {
        std::wstring display;
        int id = 0;
    };
    std::vector<AbilityItem> list;
    for (int i = 0; i < g_AbilityCount; i++) {
        AbilityItem item;
        item.id = g_AbilityIDs[i];
        item.display = GetDisplayItemText(g_AbilityNames[i]);
        list.push_back(item);
    }

    if (g_config.sortByAlphabet) {
        std::sort(list.begin(), list.end(), [](const AbilityItem& a, const AbilityItem& b) {
            return _wcsicmp(a.display.c_str(), b.display.c_str()) < 0;
            });
    }

    for (const auto& item : list) {
        int idx = (int)SendMessageW(hCb, CB_ADDSTRING, 0, (LPARAM)item.display.c_str());
        SendMessageW(hCb, CB_SETITEMDATA, idx, item.id);
    }
}

void FillPlazaGearCombobox(HWND hCb, const wchar_t** items, int itemCount, uint32_t activeID) {
    if (!hCb) return;
    SendMessageW(hCb, CB_RESETCONTENT, 0, 0);

    std::vector<ComboboxItem> list;
    for (int i = 0; i < itemCount; ++i) {
        if (!items[i]) continue;

        int32_t id = ParseItemID(items[i]);



        ComboboxItem item;
        item.raw = items[i];
        item.id = id;
        item.display = GetDisplayItemText(items[i]);
        list.push_back(item);
    }

    if (g_config.sortByAlphabet) {
        std::sort(list.begin(), list.end(), [](const ComboboxItem& a, const ComboboxItem& b) {
            return _wcsicmp(a.display.c_str(), b.display.c_str()) < 0;
            });
    }

    for (const auto& item : list) {
        int idx = (int)SendMessageW(hCb, CB_ADDSTRING, 0, (LPARAM)item.display.c_str());
        SendMessageW(hCb, CB_SETITEMDATA, idx, item.id);
    }
}

void ReadInt32(HWND hwnd, int id, be32_t& out) {
    wchar_t buf[256] = { 0 };
    GetDlgItemTextW(hwnd, id, buf, 256);
    int32_t v = 0;
    swscanf_s(buf, L"%d", &v);
    out = (uint32_t)v;
}

void ReadCombo32(HWND hwnd, int id, be32_t& out) {
    HWND hCombo = GetDlgItem(hwnd, id);
    int sel = (int)SendMessageW(hCombo, CB_GETCURSEL, 0, 0);
    if (sel != CB_ERR) {
        out = (uint32_t)SendMessageW(hCombo, CB_GETITEMDATA, sel, 0);
    }
    else {
        wchar_t buf[256] = { 0 };
        GetDlgItemTextW(hwnd, id, buf, 256);
        int32_t v = -1;
        if (swscanf_s(buf, L"%d", &v) == 1) {
            out = (uint32_t)v;
        }
    }
}

void WriteInt32(HWND hwnd, int id, be32_t val) {
    SetDlgItemTextW(hwnd, id, std::to_wstring((int32_t)(uint32_t)val).c_str());
}

void WriteCombo32(HWND hwnd, int id, be32_t val) {
    HWND hCombo = GetDlgItem(hwnd, id);
    int count = (int)SendMessageW(hCombo, CB_GETCOUNT, 0, 0);
    bool found = false;
    for (int i = 0; i < count; i++) {
        int32_t itemVal = (int32_t)SendMessageW(hCombo, CB_GETITEMDATA, i, 0);
        if (itemVal == (int32_t)(uint32_t)val) {
            SendMessageW(hCombo, CB_SETCURSEL, i, 0);
            found = true;
            break;
        }
    }
    if (!found) {
        SetDlgItemTextW(hwnd, id, std::to_wstring((int32_t)(uint32_t)val).c_str());
    }
}

void ReadTime32(HWND hwnd, int id, be32_t& out) {
    wchar_t buf[256] = { 0 };
    GetDlgItemTextW(hwnd, id, buf, 256);
    struct tm tm = { 0 };
    int y, m, d, h, min, s;
    if (swscanf_s(buf, L"%d-%d-%d %d:%d:%d", &y, &m, &d, &h, &min, &s) == 6) {
        tm.tm_year = y - 1900;
        tm.tm_mon = m - 1;
        tm.tm_mday = d;
        tm.tm_hour = h;
        tm.tm_min = min;
        tm.tm_sec = s;
        tm.tm_isdst = -1;
        time_t t = mktime(&tm);
        if (t != -1) {
            out = (uint32_t)t;
        }
    }
}

void WriteTime32(HWND hwnd, int id, be32_t val) {
    time_t t = (time_t)(uint32_t)val;
    struct tm tm;
    if (localtime_s(&tm, &t) == 0) {
        wchar_t buf[64];
        wcsftime(buf, 64, L"%Y-%m-%d %H:%M:%S", &tm);
        SetDlgItemTextW(hwnd, id, buf);
    }
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NOTIFY || msg == WM_COMMAND) {
        return SendMessageW(g_hwndMain, msg, wParam, lParam);
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void RegisterSpoonContainer() {
    static bool reg = false;
    if (!reg) {
        WNDCLASSW wc = { 0 };
        wc.lpfnWndProc = ChildProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"SpoonContainer";
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        RegisterClassW(&wc);
        reg = true;
    }
}

HWND CreateChildTab(HWND hwndParent) {
    RegisterSpoonContainer();
    RECT rc;
    GetClientRect(hwndParent, &rc);
    TabCtrl_AdjustRect(hwndParent, FALSE, &rc);
    rc.left -= 3;   rc.top -= 2;
    rc.right += 3;  rc.bottom += 3;
    return CreateWindowExW(0, L"SpoonContainer", L"", WS_CHILD, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwndParent, NULL, GetModuleHandle(NULL), NULL);
}

void LayoutControls(HWND hParent, int startX, int startY, const wchar_t* label, int idc) {
    HWND hStatic = CreateWindowW(L"STATIC", label, WS_VISIBLE | WS_CHILD, startX, startY + 1, 100, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
    HWND hEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, startX + 110, startY, 200, 21, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) {
        SendMessageW(hStatic, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        SendMessageW(hEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    }
}

void LayoutCombo(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter, bool disableSort) {
    HWND hStatic = CreateWindowW(L"STATIC", label, WS_VISIBLE | WS_CHILD, startX, startY + 1, 100, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | CBS_DROPDOWN | WS_VSCROLL, startX + 110, startY, 200, 250, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) {
        SendMessageW(hStatic, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        SendMessageW(hCb, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    }
    FillCombobox(hCb, items, itemCount, filter, disableSort);
}

void LayoutComboList(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter, bool disableSort) {
    HWND hStatic = CreateWindowW(L"STATIC", label, WS_VISIBLE | WS_CHILD, startX, startY + 1, 100, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, startX + 110, startY, 200, 250, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) {
        SendMessageW(hStatic, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        SendMessageW(hCb, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    }
    FillCombobox(hCb, items, itemCount, filter, disableSort);
}

void LayoutSmallControls(HWND hParent, int startX, int startY, const wchar_t* label, int idc, int labelWidth) {
    HWND hStatic = CreateWindowW(L"STATIC", label, WS_VISIBLE | WS_CHILD, startX, startY + 1, labelWidth, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
    HWND hEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, startX + labelWidth + 5, startY, 180, 21, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) {
        SendMessageW(hStatic, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        SendMessageW(hEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    }
}

void LayoutSmallCombo(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter, bool disableSort) {
    HWND hStatic = CreateWindowW(L"STATIC", label, WS_VISIBLE | WS_CHILD, startX, startY + 1, 110, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | CBS_DROPDOWN | WS_VSCROLL, startX + 115, startY, 180, 250, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) {
        SendMessageW(hStatic, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        SendMessageW(hCb, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    }
    FillCombobox(hCb, items, itemCount, filter, disableSort);
}

void LayoutCheckbox(HWND hParent, int startX, int startY, const wchar_t* label, int idc) {
    HWND hCheck = CreateWindowW(L"BUTTON", label, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP, startX, startY, 600, 20, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) SendMessageW(hCheck, WM_SETFONT, (WPARAM)g_hFont, TRUE);
}

void LayoutSmallComboList(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter, bool disableSort, int labelWidth) {
    HWND hStatic = CreateWindowW(L"STATIC", label, WS_VISIBLE | WS_CHILD, startX, startY + 1, labelWidth, 20, hParent, NULL, GetModuleHandle(NULL), NULL);
    HWND hCb = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, startX + labelWidth + 5, startY, 180, 250, hParent, (HMENU)(UINT_PTR)idc, GetModuleHandle(NULL), NULL);
    if (g_hFont) {
        SendMessageW(hStatic, WM_SETFONT, (WPARAM)g_hFont, TRUE);
        SendMessageW(hCb, WM_SETFONT, (WPARAM)g_hFont, TRUE);
    }
    FillCombobox(hCb, items, itemCount, filter, disableSort);
}
