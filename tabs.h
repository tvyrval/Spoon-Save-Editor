#pragma once
#include <windows.h>
#include "save_data.h"

void InitWeaponTab(HWND hParent);
void UpdateWeaponTabUI(SaveData* data);
void SaveWeaponTabData(SaveData* data);
bool HandleWeaponTabCommand(HWND hControl, int wmEvent);

void InitGearTab(HWND hParent);
void UpdateGearTabUI(SaveData* data);
void SaveGearTabData(SaveData* data);
bool HandleGearTabCommand(HWND hControl, int wmEvent);

void InitPlayerTab(HWND hParent);
void UpdatePlayerTabUI(SaveData* data);
void SavePlayerTabData(SaveData* data);

void InitHeroTab(HWND hParent);
void UpdateHeroTabUI(SaveData* data);
void SaveHeroTabData(SaveData* data);
bool HandleHeroTabCommand(HWND hControl, int wmEvent);

void InitPlazaTab(HWND hParent);
void UpdatePlazaTabUI(SaveData* data);
void SavePlazaTabData(SaveData* data);
bool HandlePlazaTabCommand(HWND hControl, int wmEvent);
extern int currentPlazaIndex;
void LoadPlazaInkling(int index, SaveData* data);

void InitMinigameTab(HWND hParent);
void UpdateMinigameTabUI(SaveData* data);
void SaveMinigameTabData(SaveData* data);

void InitAmiiboTab(HWND hParent);
void UpdateAmiiboTabUI(SaveData* data);
void SaveAmiiboTabData(SaveData* data);
bool HandleAmiiboTabCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitSpykeTab(HWND hParent);
void UpdateSpykeTabUI(SaveData* data);
void SaveSpykeTabData(SaveData* data);
bool HandleSpykeTabCommand(HWND hControl, int wmEvent);
void UpdateGearList(HWND hTab, int catId, int listId, uint32_t activeID = 0);

void InitBitFlagTab(HWND hParent);
void UpdateBitFlagTabUI(SaveData* data);
void SaveBitFlagTabData(SaveData* data);
