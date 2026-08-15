#pragma once
#include <windows.h>
#include "save_data.h"
#include <string>

void ReadInt32(HWND hwnd, int id, be32_t& out);
void ReadCombo32(HWND hwnd, int id, be32_t& out);
void WriteInt32(HWND hwnd, int id, be32_t val);
void WriteCombo32(HWND hwnd, int id, be32_t val);

void ReadTime32(HWND hwnd, int id, be32_t& out);
void WriteTime32(HWND hwnd, int id, be32_t val);

void RegisterSpoonContainer();
HWND CreateChildTab(HWND hwndParent);
void LayoutControls(HWND hParent, int startX, int startY, const wchar_t* label, int idc);
void LayoutCombo(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter = false, bool disableSort = false);
void LayoutComboList(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter = false, bool disableSort = false);
void LayoutSmallControls(HWND hParent, int startX, int startY, const wchar_t* label, int idc, int labelWidth = 110);
void LayoutSmallCombo(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter = false, bool disableSort = false, int labelWidth = 110);
void LayoutSmallComboList(HWND hParent, int startX, int startY, const wchar_t* label, int idc, const wchar_t** items, int itemCount, bool filter = false, bool disableSort = false, int labelWidth = 110);
void LayoutCheckbox(HWND hParent, int startX, int startY, const wchar_t* label, int idc);
void FillCombobox(HWND hCb, const wchar_t** items, int itemCount, bool filter = false, bool disableSort = false);
void FillAbilityCombobox(HWND hCb);
void FillPlazaGearCombobox(HWND hCb, const wchar_t** items, int itemCount, uint32_t activeID);
std::wstring GetDisplayItemText(const wchar_t* str);

LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
