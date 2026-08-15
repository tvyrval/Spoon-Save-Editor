#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "save_data.h"

extern std::vector<uint8_t> g_SaveBuffer;
extern std::wstring g_CurrentFilePath;
extern size_t g_OriginalFileSize;

// Tab indices
#define TAB_WEAPON 0
#define TAB_GEAR 1
#define TAB_PLAYER 2
#define TAB_HERO 3
#define TAB_PLAZA 4
#define TAB_AMIIBO 5
#define TAB_MINIGAME 6
#define TAB_SPYKE 7
#define TAB_BITFLAG 8

#define PLAYER_SUB_STATS 0
#define PLAYER_SUB_APPEAR 1
#define PLAYER_SUB_SPLATFEST 2
#define PLAYER_SUB_CONTROLS 3

#define GEAR_SUB_HEAD 0
#define GEAR_SUB_CLOTHES 1
#define GEAR_SUB_SHOES 2

// Control Ids
#define ID_BTN_OPEN 101
#define ID_BTN_SAVE 102
#define IDC_W_WEAPON 200
#define IDC_W_LIST 201
#define IDC_W_ADD_PICKER 202
#define IDC_W_BTN_ADD 203
#define IDC_W_BTN_DEL 204
#define IDC_W_TURF_INKED 205
#define IDC_W_BTN_SET_TURF 206
#define IDC_W_FLAG_NEW 211

// Gear Editor
#define IDC_G_CAT 300
#define IDC_G_LIST 301
#define IDC_G_MAIN 302
#define IDC_G_SUB1 303
#define IDC_G_SUB2 304
#define IDC_G_SUB3 305
#define IDC_G_EXP 306
#define IDC_G_LEVEL 307
#define IDC_G_SLOTS 308
#define IDC_G_BTN_APPLY 309
#define IDC_G_FLAG_NEW 310
#define IDC_G_BTN_ADD 311
#define IDC_G_BTN_DEL 312
#define IDC_G_ADD_PICKER 313
#define IDC_G_HEAD_EQUIP 320
#define IDC_G_CLOTH_EQUIP 321
#define IDC_G_SHOE_EQUIP 322

#define IDC_P_LEVEL 400
#define IDC_P_LEVELPTS 401
#define IDC_P_RANK 402

extern const wchar_t* g_AbilityNames[];
extern const int g_AbilityCount;
extern const int g_AbilityIDs[];
#define IDC_P_RANKPTS 403
#define IDC_P_MONEY 404
#define IDC_P_SNAILS 405
#define IDC_P_GENDER 406
#define IDC_P_SKIN 407
#define IDC_P_EYE 408
#define IDC_P_NEWS_VERSION 409
#define IDC_H_EGGS 500
#define IDC_H_SHOT_LVL 501
#define IDC_H_SHOT_UNLOCK 502
#define IDC_H_TANK_LVL 503
#define IDC_H_TANK_UNLOCK 504
#define IDC_H_BOMB_SELECT 505
#define IDC_H_SPLATBOM_LVL 506
#define IDC_H_SPLATBOM_UNLOCK 507
#define IDC_H_BURSTBOM_LVL 508
#define IDC_H_BURSTBOM_UNLOCK 509
#define IDC_H_SEEKER_LVL 510
#define IDC_H_SEEKER_UNLOCK 511
#define IDC_H_STAGE_LIST 512
#define IDC_H_STAGE_ZAPFISH 513
#define IDC_H_STAGE_SCROLL 514
#define IDC_H_BTN_UNLOCK_WORLD 515
#define IDC_H_BTN_COMPLETE_ALL 516
#define IDC_H_FLAG_ZAPFISH 517
#define IDC_H_FLAG_HEROSUIT 518
#define IDC_H_FLAG_CUTTLEFISH 519
#define IDC_H_INKRAILS 520
#define IDC_H_EVENTS 521

#define IDC_PL_SF_ID 600
#define IDC_PL_SF_TEAM 601
#define IDC_PL_SF_PWR 602
#define IDC_PL_SF_XP 603
#define IDC_PL_SF_RANK 604
#define IDC_Z_MIIVERSE 700
#define IDC_Z_LIST 701
#define IDC_Z_NAME 702
#define IDC_Z_LEVEL 703
#define IDC_Z_RANK 704
#define IDC_Z_TURF 705
#define IDC_Z_GENDER 706
#define IDC_Z_SKIN 707
#define IDC_Z_EYE 708
#define IDC_Z_HEAD 709
#define IDC_Z_CLOTH 710
#define IDC_Z_SHOE 711
#define IDC_Z_WEAPON 712
#define IDC_Z_BTN_FILL_FAKE 750

#define IDC_CTRL_COLOR_LOCK 713
#define IDC_CTRL_REV_Y 714
#define IDC_CTRL_REV_X 715
#define IDC_CTRL_MOTION 716
#define IDC_CTRL_CAM_SENS 717

#define IDC_M_SJ_LOCK 800
#define IDC_M_SB_LOCK 801
#define IDC_M_SR_LOCK 802
#define IDC_M_SBZ_LOCK 803
#define IDC_M_SJ_SCORE 804
#define IDC_M_SB_SCORE 805
#define IDC_M_SR_SCORE 806

#define IDC_A_SET 900
#define IDC_A_BTN_UNLOCK_ALL 901
#define IDC_A_CHALLENGE_BASE 910

#define IDC_S_CAT_1 1000
#define IDC_S_CAT_2 1001
#define IDC_S_CAT_3 1002
#define IDC_S_ID_1 1003
#define IDC_S_ID_2 1004
#define IDC_S_ID_3 1005
#define IDC_S_STATE_1 1006
#define IDC_S_STATE_2 1007
#define IDC_S_STATE_3 1008
#define IDC_S_ORD_1 1009
#define IDC_S_ORD_2 1010
#define IDC_S_ORD_3 1011

#define IDC_B_FLAG_1 1501
#define IDC_B_FLAG_2 1502
#define IDC_B_FLAG_3 1503
#define IDC_B_FLAG_4 1504
#define IDC_B_FLAG_5 1505
#define IDC_B_FLAG_6 1506
#define IDC_B_FLAG_7 1507
#define IDC_B_FLAG_8 1508
#define IDC_B_FLAG_9 1509
#define IDC_B_FLAG_10 1510
#define IDC_B_FLAG_11 1511
#define IDC_B_FLAG_12 1512
#define IDC_B_FLAG_13 1513
#define IDC_B_FLAG_14 1514
#define IDC_S_ARR_1 1012
#define IDC_S_ARR_2 1013
#define IDC_S_ARR_3 1014
#define IDC_S_NOW_1 1015
#define IDC_S_NOW_2 1016
#define IDC_S_NOW_3 1017

// Globals
extern HWND g_hwndMain;
extern HWND g_hwndTab;
extern HWND g_hwndChild[9];
extern HWND g_hwndPlayerTab;
extern HWND g_hwndPlayerSubChild[4];
extern HWND g_hwndGearTab;
extern HWND g_hwndGearSubChild[3];
extern HFONT g_hFont;

extern std::vector<uint8_t> g_SaveBuffer;
extern std::wstring g_CurrentFilePath;

SaveData* GetSaveData();

// Data arrays
extern const wchar_t* g_PlazaRankNames[];
extern const wchar_t* g_WeaponNames[];
extern const int g_WeaponCount;
extern const wchar_t* g_HeadNames[];
extern const int g_HeadCount;
extern const wchar_t* g_ClothNames[];
extern const int g_ClothCount;
extern const wchar_t* g_ShoeNames[];
extern const int g_ShoeCount;
extern const wchar_t* g_MiiverseNames[];
extern const int g_MiiverseCount;
extern const wchar_t* g_GenderNames[];
extern const wchar_t* g_SplatfestTeamNames[];
extern const wchar_t* g_SplatfestRankNames[];
extern const wchar_t* g_SkinColorNames[];
extern const wchar_t* g_RankNames[];
extern const wchar_t* g_EyeColorNames[];

#define ID_BTN_ABOUT 2000

struct EditorConfig {
    bool showInternalIDs = false;
    bool sortByAlphabet = false;
};
extern EditorConfig g_config;
void LoadConfig();
void SaveConfig();
void RefillAllComboboxes();
