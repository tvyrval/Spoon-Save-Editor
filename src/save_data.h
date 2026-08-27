#ifndef _MSC_VER
#define _byteswap_ulong __builtin_bswap32
#endif

#pragma once
#include <stdint.h>
#include <stdlib.h>

#define FULL_SAVE_SIZE_V1 295840

#pragma pack(push, 1)

struct be32_t {
    uint32_t value;
    operator uint32_t() const { return _byteswap_ulong(value); }
    be32_t& operator=(uint32_t v) { value = _byteswap_ulong(v); return *this; }
};

struct le32_t {
    uint32_t value;
    operator uint32_t() const { return value; }
    le32_t& operator=(uint32_t v) { value = v; return *this; }
};

struct PlazaInkling {
    be32_t status;
    uint8_t unk0[3];
    char16_t name[8];
    uint8_t unk1[29];
    be32_t gender;
    be32_t skinColor;
    be32_t eyeColor;
    be32_t weapon;
    uint8_t unk2[12];
    be32_t turfInked;
    be32_t shoeGear;
    uint8_t unk3[24];
    be32_t clothGear;
    uint8_t unk4[24];
    be32_t headGear;
    uint8_t unk5[28];
    be32_t level;
    be32_t rank;
    uint8_t unk6[40];
    be32_t colorR;
    be32_t colorG;
    be32_t colorB;
    uint8_t padding[4];
};

struct HeroStage {
    be32_t stageID;
    be32_t zapfish;   // 3 = Collected
    be32_t scroll;    // 1 = Collected
};

struct SpykeOrder {
    be32_t category;     // 0xA hex = -1 (None), 0=Shoes, 1=Cloth, 2=Hat
    be32_t gearID;
    be32_t unk[7];
    be32_t orderedTime;  // 0x24 (36)
    be32_t unkpad[1];
    be32_t arrivalTime;  // 0x2C (44)
    be32_t unk2[2];
    be32_t state;        // 0x38 (56) (-1=None, 0=Soon, 1=Ready)
    be32_t unk3[1];
};

// 0x001 = Tutorial finished, 0x002 = Splatfest seen, 0x008 = Lobby seen,
// 0x010 = Hero Suit given, 0x080 = Great Zapfish returned, 0x100 = Cuttlefish dialogue,
// 0x800 = Ranked unlocked, 0x1000 = Rank shown, 0x10000 = Snails shown, 0x100000 = Lvl cap raised
#define GAME_FLAG_WARNING        0x00200000u // Seen stats warning
#define GAME_FLAG_LEVELCAP       0x00100000u // Level cap raised to 50
#define GAME_FLAG_SNAILS         0x00010000u // Snails shown in plaza/lobby
#define GAME_FLAG_RANKSHOWN      0x00001000u // Rank shown in plaza/lobby
#define GAME_FLAG_RANKED         0x00000800u // Ranked battles unlocked
#define GAME_FLAG_CUTTLEFISH     0x00000100u // Cuttlefish dialogue seen
#define GAME_FLAG_GREATZAPFISH   0x00000080u // Great Zapfish returned
#define GAME_FLAG_HEROSUIT       0x00000010u // Hero Suit given
#define GAME_FLAG_LOBBY          0x00000008u // Lobby explanation seen
#define GAME_FLAG_RANKEDNEWS     0x00000004u // Ranked news seen
#define GAME_FLAG_SPLATFEST      0x00000002u // Splatfest explanation seen
#define GAME_FLAG_TUTORIAL       0x00000001u // Tutorial finished

#define SPYKE_FLAG_TALKED        0x00000018u // Seen Spyke explanation
#define SPYKE_FLAG_SSS           0x00000010u // Seen Spyke SSS explanation

struct OwnedWeapon {
    be32_t id;          // 0x00
    be32_t unk04;       // 0x04
    be32_t subID;       // 0x08
    be32_t specialID;   // 0x0C
    be32_t turfInked;   // 0x10
    be32_t unk14;       // 0x14
    be32_t unk18;       // 0x18
    be32_t timestamp;   // 0x1C
    uint8_t flags;      // 0x20 (1 = New)
    uint8_t unk21[7];   // 0x21
};

struct OwnedGearS1 {
    be32_t id;             // 0x00
    be32_t level;          // 0x04
    be32_t unlockedSlots;  // 0x08
    be32_t sub1;           // 0x0C
    be32_t sub2;           // 0x10
    be32_t sub3;           // 0x14
    be32_t exp;            // 0x18
    uint8_t unk1C[8];      // 0x1C
    be32_t timestamp;      // 0x24
    uint8_t newFlag;       // 0x28
    uint8_t padding[7];
};

struct SaveData {
    uint8_t unk_hdr[8];
    be32_t checksum;         // Offset 0x08
    uint8_t unk_hdr2[4];     // Offset 0x0C
    be32_t miiverseDisplay;  // Offset 0x10
    be32_t cameraSensRaw;    // Offset 0x14
    uint8_t reverseY;        // Offset 0x18 (1 = enabled) (gt7)
    uint8_t reverseX;        // Offset 0x19 (1 = enabled) (PtJ)
    uint8_t motionControls;  // Offset 0x1A (1 = enabled) (StL)
    uint8_t unk_1B[0x24 - 0x1B];
    be32_t gameStateFlags;   // Offset 0x24
    uint8_t unk1_28[8];
    be32_t newsVersionSeen;  // Offset 0x30
    uint8_t unk1_34[8];
    be32_t lastSaveTime;     // Offset 0x3C
    be32_t minigameUnlocks; // Offset 0x40
    be32_t squidJumpScore;  // Offset 0x44
    be32_t squidballScore;  // Offset 0x48
    be32_t squidRacerScore; // Offset 0x4C
    uint8_t unk1a[0x180 - 0x50];
    be32_t currentClothGear; // Offset 0x180
    be32_t currentShoeGear;  // Offset 0x184
    be32_t currentHeadGear;  // Offset 0x188
    be32_t currentWeapon;    // Offset 0x18C
    be32_t genderID;
    be32_t skinColor;
    be32_t eyeColor;
    uint8_t unk1b[0x1A0 - 0x19C];
    OwnedGearS1 shoes[256];      // 0x1A0 (12288 bytes)
    OwnedGearS1 clothes[256];    // 0x31A0 (12288 bytes)
    OwnedGearS1 head[256];       // 0x61A0 (12288 bytes)
    OwnedWeapon ownedWeapons[128]; // 0x91A0 (5120 bytes)
    be32_t money;
    be32_t levelPoints;
    be32_t level;
    be32_t rank;
    be32_t rankPoints;
    be32_t seaSnails;
    uint8_t gap_A5B8[0xA6B8 - 0xA5B8];
    HeroStage heroStages[64]; // Offset 0xA6B8
    uint8_t gap_A9B8[4];
    be32_t heroInkrails;      // 0xA9BC
    be32_t heroUnk1;          // 0xA9C0
    be32_t heroEvents;        // 0xA9C4
    be32_t heroShotLevel;     // 0xA9C8
    be32_t heroTankLevel;     // 0xA9CC
    be32_t heroBombSelect;    // 0xA9D0 (0=Splat, 1=Burst, 2=Seeker)
    be32_t heroSplatBombLevel;// 0xA9D4
    be32_t heroBurstBombLevel;// 0xA9D8
    be32_t heroSeekerLevel;   // 0xA9DC
    be32_t powerEggs;         // 0xA9E0
    uint8_t gap_AA44[0xB100 - 0xA9E4];
    SpykeOrder orders[3];     // 0xB100
    uint8_t gap_B1C0_pad1[4];
    be32_t sessionPlayTime;   // Offset 0xB1C4
    uint8_t gap_B1C8_pad2[16];
    be32_t talkedToSpyke;     // 0xB1D8
    be32_t spykeSSS;         // 0xB1DC
    uint8_t gap_B1E0[0xB2E0 - 0xB1E0];
    be32_t splatfestID;
    be32_t splatfestTeamID;
    be32_t splatfestPower;
    be32_t splatfestRank;
    be32_t splatfestXP; 
    uint8_t padding1[46100 - 0xB2F4]; 
    PlazaInkling plazaInklings[25];  
    uint8_t unk_before_amiibo[52292 - 52000];
    be32_t girlChallenges[20];   // 0xCC44
    uint8_t girl_pad[4];
    be32_t boyChallenges[20];    // 0xCC98
    uint8_t boy_pad[4];
    be32_t squidChallenges[20];  // 0xCCEC
    uint8_t final_pad[53888 - 52540]; 
};

#pragma pack(pop)
