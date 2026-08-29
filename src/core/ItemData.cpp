#include "ItemData.h"

const wchar_t* g_PlazaRankNames[] = {
    L"0 (C-)", L"1 (C)", L"2 (C+)", L"3 (B-)", L"4 (B)", L"5 (B+)",
    L"6 (A-)", L"7 (A)", L"8 (A+)", L"9 (S)", L"10 (S+)", L"11 (S+ 99)"
};
const int g_PlazaRankCount = 12;

const wchar_t* g_GenderNames[] = { L"0 (Inkling Girl)", L"1 (Inkling Boy)", L"2 (Octoling)" };
const int g_GenderCount = 3;

const wchar_t* g_SplatfestTeamNames[] = { L"-1 (None)", L"0 (Callie)", L"1 (Marie)" };
const int g_SplatfestTeamCount = 3;

const wchar_t* g_RankNames[] = {
    L"0 (C-)", L"1 (C)", L"2 (C+)", L"3 (B-)", L"4 (B)", L"5 (B+)",
    L"6 (A-)", L"7 (A)", L"8 (A+)", L"9 (S)", L"10 (S+)"
};
const int g_RankCount = 11;

const wchar_t* g_EyeColorNames[] = {
    L"0 (Brown)", L"1 (Pink)", L"2 (Blue)", L"3 (Orange)",
    L"4 (Turquoise)", L"5 (Black)", L"6 (Yellow)"
};
const int g_EyeColorCount = 7;

const wchar_t* g_BombNames[] = { L"0 (Splat Bomb)", L"1 (Burst Bomb)", L"2 (Seeker)" };
const int g_BombCount = 3;

const wchar_t* g_HeroUpgradeNames[] = {
    L"-1 (Locked)", L"0 (Level 1)", L"1 (Level 2)", L"2 (Level 3)", L"3 (Level MAX)"
};
const int g_HeroUpgradeCount = 5;

const wchar_t* g_ChallengeStates[] = {
    L"0 (Hidden)",
    L"1 (Locked)",
    L"2 (Unlocked)",
    L"3 (Unlocked F)",
    L"4 (Finished)"
};
const int g_ChallengeStateCount = 5;

const wchar_t* g_SpykeCategoryNames[] = { L"None", L"Shoes", L"Clothes", L"Hat" };
const int g_SpykeCategoryCount = 4;

const wchar_t* g_SpykeStateNames[] = { L"None", L"Coming Soon", L"Can Withdraw" };
const int g_SpykeStateCount = 3;

const wchar_t* g_SkinColorNames[] = {
    L"0 (Lightest)",
    L"1 (Fair)",
    L"2 (Medium Light)",
    L"3 (Medium)",
    L"4 (Medium Dark)",
    L"5 (Dark)",
    L"6 (Darkest)",
    L"7 (Sunburnt)"
};

const wchar_t* g_WeaponNames[] = {
    L"1000 (Sploosh-o-matic)", L"1001 (Neo Sploosh-o-matic)", L"1003 (?Shot_Msn0Lv0)", L"1004 (?Shot_Msn0Lv1)",
    L"1005 (?Shot_Msn0Lv2)", L"1006 (?Shot_Msn0Lv3)", L"1012 (?Shot_Rvl0Lv0)", L"1007 (?Shot_Rvl0Lv1)",
    L"1008 (?Shot_Rvl0Lv2)", L"1009 (?Shot_Rvl0Lv3)", L"1002 (Sploosh-o-matic 7)", L"1010 (Splattershot Jr.)",
    L"1011 (Custom Splattershot Jr.)", L"1020 (Splash-o-matic)", L"1021 (Neo Splash-o-matic)", L"1030 (Aerospray MG)",
    L"1031 (Aerospray RG)", L"1032 (Aerospray PG)", L"1040 (Splattershot)", L"1041 (Tentatek Splattershot)",
    L"1042 (Wasabi Splattershot)", L"1045 (Hero Shot Replica)", L"1046 (Octoshot Replica)", L"1050 (.52 Gal)",
    L"1051 (.52 Gal Deco)", L"1060 (N-ZAP '85)", L"1061 (N-ZAP '89)", L"1062 (N-ZAP '83)",
    L"1070 (Splattershot Pro)", L"1071 (Forge Splattershot Pro)", L"1072 (Berry Splattershot Pro)", L"1080 (.96 Gal)",
    L"1081 (.96 Gal Deco)", L"1090 (Dual Squelcher)", L"1091 (Custom Dual Squelcher)", L"1100 (Jet Squelcher)",
    L"1101 (Custom Jet Squelcher)", L"1110 (Luna Blaster)", L"1111 (Luna Blaster Neo)", L"1120 (Blaster)",
    L"1121 (Custom Blaster)", L"1130 (Range Blaster)", L"1131 (Custom Range Blaster)", L"1132 (Grim Range Blaster)",
    L"1140 (Rapid Blaster)", L"1141 (Rapid Blaster Deco)", L"1150 (Rapid Blaster Pro)", L"1151 (Rapid Blaster Pro Deco)",
    L"1160 (L-3 Nozzlenose)", L"1161 (L-3 Nozzlenose D)", L"1170 (H-3 Nozzlenose)", L"1171 (H-3 Nozzlenose D)",
    L"1172 (Cherry H-3 Nozzlenose)", L"2000 (Carbon Roller)", L"2001 (Carbon Roller Deco)", L"2002 (?Roller_KingSquid)",
    L"2003 (?Roller_Mission)", L"2010 (Splat Roller)", L"2011 (Krak-On Splat Roller)", L"2012 (CoroCoro Splat Roller)",
    L"2015 (Hero Roller Replica)", L"2020 (Dynamo Roller)", L"2021 (Gold Dynamo Roller)", L"2022 (Tempered Dynamo Roller)",
    L"2030 (Inkbrush)", L"2031 (Inkbrush Nouveau)", L"2032 (Permanent Inkbrush)", L"2040 (Octobrush)",
    L"2041 (Octobrush Nouveau)", L"3000 (Slosher)", L"3001 (Slosher Deco)", L"3002 (Soda Slosher)",
    L"3010 (Tri-Slosher)", L"3011 (Tri-Slosher Nouveau)", L"3020 (Sloshing Machine)", L"3021 (Sloshing Machine Neo)",
    L"3022 (?BigBall_SpecAdjust01)", L"3023 (?BigBall_SpecAdjust02)", L"4000 (Classic Squiffer)", L"4001 (New Squiffer)",
    L"4002 (Fresh Squiffer)", L"4010 (Splat Charger)", L"4011 (Kelp Splat Charger)", L"4012 (Bento Splat Charger)",
    L"4015 (Hero Charger Replica)", L"4016 (?Charge_Mission)", L"4020 (Splatterscope)", L"4021 (Kelp Splatterscope)",
    L"4022 (Bento Splatterscope)", L"4030 (E-Litre 3K)", L"4031 (Custom E-Litre 3K)", L"4040 (E-Litre 3K Scope)",
    L"4041 (Custom E-Litre 3K Scope)", L"4050 (Bamboozler 14 Mk I)", L"4051 (Bamboozler 14 Mk II)", L"4052 (Bamboozler 14 Mk III)",
    L"5000 (Mini Splatling)", L"5001 (Zink Mini Splatling)", L"5002 (Refurbished Mini Splatling)", L"5010 (Heavy Splatling)",
    L"5011 (Heavy Splatling Deco)", L"5012 (Heavy Splatling Remix)", L"5020 (Hydra Splatling)", L"5021 (Custom Hydra Splatling)"
};
extern const int g_WeaponCount = 104;

const wchar_t* g_HeadNames[] = {
    L"0 (ARINOMAMA)", L"1 (White Headband)", L"1000 (Urchins Cap)", L"1001 (Lightweight Cap)", L"1002 (Takoroka Mesh)", L"1003 (Fashion Cap)",
    L"1004 (Squid-Stitch Cap)", L"1005 (Squidvader Cap)", L"1006 (Camo Mesh Cap)", L"1007 (5-Panel Cap)", L"1008 (Zekko Mesh)", L"1009 (Backwards Cap)",
    L"1010 (2-Stripe Mesh Cap)", L"1011 (Jet Cap)", L"1012 (Cycling Cap)", L"1013 (SQUID GIRL Hat)", L"1014 (Cycle King Cap)", L"1015 (Legendary Cap)",
    L"1016 (CoroCoro Cap)", L"2000 (Bobble Hat)", L"2001 (Short Beanie)", L"2002 (Striped Beanie)", L"2003 (Sporty Bobble Hat)", L"2004 (Special Forces Beret)",
    L"2005 (Squid Nordic)", L"3000 (Retro Specs)", L"3001 (Splash Goggles)", L"3002 (Pilot Goggles)", L"3003 (Coloured Shades)", L"3004 (Black Arrowbands)",
    L"3005 (Snorkel)", L"3006 (White Arrowbands)", L"3007 (Fake Contacts)", L"3008 (18K Aviators)", L"3009 (Full Moon Glasses)", L"3010 (Octoglasses)",
    L"4000 (Jungle Hat)", L"4001 (Safari Hat)", L"4002 (Camping Hat)", L"4003 (Fugu Bell Hat)", L"4004 (Bambo Hat)", L"4005 (Straw Boater)",
    L"4006 (Classic Straw Boater)", L"4007 (Treasure Hunter)", L"5000 (Studio Headphones)", L"5001 (Colourful Headphones)", L"5002 (Noise Cancellers)",
    L"6000 (Golf Visor)", L"6001 (FishFry Visor)", L"6002 (Sun Visor)", L"7000 (Cycle Helmet)", L"7002 (Stealth Goggles)", L"7003 (Tentacles Helmet)",
    L"7004 (Skate Helmet)", L"7005 (Visor Skate Helmet)", L"8000 (Gas Mask)", L"8001 (Paintball Mask)", L"8002 (Paisley Bandana)", L"8003 (Skull Bandana)",
    L"9001 (B-Ball Headband)", L"9002 (Squash Headband)", L"9003 (Tennis Headband)", L"9004 (Joggling Headband)", L"9005 (Football Headband)",
    L"9006 (Traditional Headband)", L"25000 (Squid Hairclip)", L"25001 (Samurai Helmet)", L"25002 (Power Mask)", L"27000 (Hero Headset Replica)",
    L"27001 (?MSN001)", L"27002 (?MSN002)", L"27003 (?MSN003)", L"27004 (Armour Helmet Replica)", L"28000 (Octoling Scope)", L"28001 (\u30D8\u30EB\u30E1\u30C3\u30C8\u30E9\u30A4\u30D0\u30EB\u7528\u5F37)",
    L"29000 (?Attack_Up)", L"29001 (?Defense_Up)", L"29002 (?MainInk_Save)", L"29003 (?SubInk_Save)", L"29004 (?Inkrecovery_Up)", L"29005 (?HumanMove_Up)",
    L"29006 (?SquidMove_up)", L"29007 (?SpecialIncrease_Up)", L"29008 (?SpecialTime_Up)", L"29009 (?RespawnTime_Save)", L"29010 (?RespawnSpecialGauge_Save)",
    L"29011 (?JumpTime_Save)", L"29012 (?BombDistance_Up)", L"29013 (?StartAllUp)", L"29014 (?EndAllUp)", L"29015 (?MinorityUp)", L"29016 (?ComeBack)",
    L"29500 (?SUP000)", L"29501 (?SUP001)"
};
extern const int g_HeadCount = 94;

const wchar_t* g_ClothNames[] = {
    L"0 (?NoClothes)", L"1 (Basic Tee)", L"1000 (White Tee)", L"1001 (Black Squideye)", L"1003 (Sky Blue Squideye)", L"1004 (Rockenberg White)",
    L"1005 (RockenBerg Black)", L"1006 (Black Tee)", L"1007 (Sunny Day Tee)", L"1008 (Rainy Day Tee)", L"1009 (Reggae Tee)", L"1010 (Fugu Tee)",
    L"1011 (Mint Tee)", L"1012 (Grape Tee)", L"1013 (Red Vector Tee)", L"1014 (Grey Vector Tee)", L"1015 (Blue Peaks Tee)", L"1016 (Ivory Peaks Tee)",
    L"1017 (Squid-Stitch Tee)", L"1018 (Pirate Stripes Tee)", L"1019 (Sailor Stripes Tee)", L"1020 (White 8-Bit FishFry)", L"1021 (Black 8-Bit FishFry)",
    L"1022 (White Anchor Tee)", L"1023 (Black Anchor Tee)", L"1024 (White Line Tee)", L"1025 (Black Pipe Tee)", L"1026 (Carnivore Tee)", L"1027 (Pearl Tee)",
    L"1028 (Octo Tee)", L"1029 (Herbivore Tee)", L"2000 (White Striped LS)", L"2001 (Black LS)", L"2002 (Purple Camo LS)", L"2003 (Navy Striped LS)",
    L"2004 (Zekko Baseball LS)", L"2005 (Varsity Baseball)", L"2006 (Black Baseball LS)", L"2007 (White Baseball LS)", L"2008 (White LS)",
    L"2009 (Green Striped LS)", L"2010 (Squidmark LS)", L"2011 (Zink LS)", L"2012 (Striped Peaks LS)", L"3000 (White Layered LS)", L"3001 (Yellow Layered LS)",
    L"3002 (Layered Camo LS)", L"3003 (Black Layered LS)", L"3004 (Zink Layered LS)", L"3005 (Layered Anchor LS)", L"3006 (Choco Layered LS)",
    L"3007 (Part-Time Pirate)", L"3008 (Layered Vector LS)", L"3009 (Green Tee)", L"4000 (Pink Shrimp Polo)", L"4001 (Striped Rugby)", L"4002 (Tricolour Rugby)",
    L"4003 (Sage Green Polo)", L"4004 (Black Polo)", L"4005 (Cycling Shirt)", L"4006 (Cycle King Jersey)", L"4007 (Slipstream United)", L"4008 (FC Albacore)",
    L"5000 (Olive Ski Jacket)", L"5002 (Berry Ski Jacket)", L"5003 (Varsity Jacket)", L"5004 (School Jersey)", L"5005 (Green Cardigan)", L"5006 (Black Inky Rider)",
    L"5007 (White Inky Rider)", L"5008 (Retro Gamer Jersey)", L"5009 (Orange Cardigan)", L"5010 (Forge Inkling Perka)", L"5011 (Forge Octarian Jacket)",
    L"5012 (Blue Sailor Suit)", L"5013 (White Sailor Suit)", L"5014 (Squid Satin Jacket)", L"5015 (Zapfish Satin Jacket)", L"5016 (Krak-On 528)",
    L"6000 (B-Ball Vest (Home))", L"6001 (B-Ball Vest (Away))", L"6002 (SQUID GIRL Tunic)", L"7000 (Grey College Sweat)", L"7001 (Squidmark Sweat)",
    L"7002 (Retro Sweat)", L"7003 (Firefin Sweat Navy)", L"7004 (Navy Collage Sweat)", L"7005 (Reel Sweat)", L"7006 (Anchor Sweat)", L"8000 (Lumberjack Shirt)",
    L"8001 (Rodeo Shirt)", L"8002 (Green Check Shirt)", L"8003 (White Shirt)", L"8004 (Urchins Jersey)", L"8005 (Aloha Shirt)", L"8006 (Red Check Shirt)",
    L"8007 (Baby Jelly Shirt)", L"8008 (Baseball Jersey)", L"8009 (Grey Mixed Shirt)", L"8010 (Vintage Check)", L"8011 (Round Collar Shirt)",
    L"8012 (Logo Aloha Shirt)", L"8013 (Striped Shirt)", L"8014 (Linen Shirt)", L"8015 (Shirt and Tie)", L"8016 (Traditional Apron)", L"9000 (Mountain Gilet)",
    L"9001 (Forest Gilet)", L"9002 (Dark Urban Gilet)", L"9003 (Yellow Urban Gilet)", L"9004 (Squid Pattern Waistcoat)", L"9005 (Squidstar Waistcoat)",
    L"10000 (Camo Zip Hoodie)", L"10001 (Green Zip Hoodie)", L"10002 (Zekko Hoodie)", L"10003 (CoroCoro Hoodie)", L"25000 (School Uniform)",
    L"25001 (Samurai Jacket)", L"25002 (Power Armour)", L"27000 (?MSN000)", L"27001 (?MSN001)", L"27002 (?MSN002)", L"27003 (?MSN003)",
    L"27004 (Armour Jacket Replica)", L"28000 (Octoling Armour)", L"26000 (Splatfest Tee)", L"26001 (HAP001_A)", L"26002 (Test_Clt_HAP001_B)",
    L"26033 (Test_Clt_HAP1001_A)", L"26034 (Test_Clt_HAP1001_B)", L"26035 (Test_Clt_HAP2001_A)", L"26036 (Test_Clt_HAP2001_B)", L"26037 (Test_Clt_HAP3001_A)",
    L"26038 (Test_Clt_HAP3001_B)", L"26039 (Test_Clt_HAP1002_A)", L"26040 (Test_Clt_HAP1002_B)", L"26041 (Test_Clt_HAP2002_A)", L"26042 (Test_Clt_HAP2002_B)",
    L"26043 (Test_Clt_HAP3002_A)", L"26044 (Test_Clt_HAP3002_B)", L"26045 (Test_Clt_HAP1003_A)", L"26046 (Test_Clt_HAP1003_B)", L"26047 (Test_Clt_HAP2003_A)",
    L"26048 (Test_Clt_HAP2003_B)", L"26049 (Test_Clt_HAP3003_A)", L"26050 (Test_Clt_HAP3003_B)", L"26051 (Test_Clt_HAP1012_A)", L"26052 (Test_Clt_HAP1012_B)",
    L"26053 (Test_Clt_HAP2004_A)", L"26054 (Test_Clt_HAP2004_B)", L"26055 (Test_Clt_HAP3004_A)", L"26056 (Test_Clt_HAP3004_B)", L"26057 (Test_Clt_HAP1005_A)",
    L"26058 (Test_Clt_HAP1005_B)", L"26059 (Test_Clt_HAP2005_A)", L"26060 (Test_Clt_HAP2005_B)", L"26061 (HAP1007_A)", L"26062 (HAP1007_B)",
    L"26063 (HAP2006_A)", L"26064 (HAP2006_B)", L"26065 (HAP1008_A)", L"26066 (HAP1008_B)", L"26067 (HAP2007_A)", L"26068 (HAP2007_B)",
    L"26069 (HAP3007_A)", L"26070 (HAP3007_B)", L"26071 (HAP1009_A)", L"26072 (HAP1009_B)", L"26073 (HAP2008_A)", L"26074 (HAP2008_B)",
    L"26075 (HAP3008_A)", L"26076 (HAP3008_B)", L"26077 (HAP1015_A)", L"26078 (HAP1015_B)", L"26079 (HAP2009_A)", L"26080 (HAP2009_B)",
    L"26081 (HAP3009_A)", L"26082 (HAP3009_B)", L"26083 (HAP3010_A)", L"26084 (HAP3010_B)", L"26085 (HAP1011_A)", L"26086 (HAP1011_B)",
    L"26087 (HAP2011_A)", L"26088 (HAP2011_B)", L"26089 (HAP3011_A)", L"26090 (HAP3011_B)", L"26091 (HAP3012_A)", L"26092 (HAP3012_B)",
    L"26093 (HAP1013_A)", L"26094 (HAP1013_B)", L"26095 (HAP3013_A)", L"26096 (HAP3013_B)", L"26097 (HAP1017_A)", L"26098 (HAP1017_B)",
    L"26099 (HAP1018_A)", L"26100 (HAP1018_B)", L"26101 (HAP3014_A)", L"26102 (HAP3014_B)", L"26105 (HAP2016_A)", L"26106 (HAP2016_B)",
    L"26107 (HAP3019_A)", L"26108 (HAP3019_B)", L"26103 (HAP2019_A)", L"26104 (HAP2019_B)", L"26109 (HAP1021_A)", L"26110 (HAP1021_B)",
    L"26111 (HAP2020_A)", L"26112 (HAP2020_B)", L"26113 (HAP3020_A)", L"26114 (HAP3020_B)", L"26115 (HAP3021_A)", L"26116 (HAP3021_B)",
    L"26117 (HAP1022_A)", L"26118 (HAP1022_B)", L"26119 (HAP1023_A)", L"26120 (HAP1023_B)", L"26121 (HAP3022_A)", L"26122 (HAP3022_B)",
    L"26123 (HAP3023_A)", L"26124 (HAP3023_B)", L"26125 (HAP1024_A)", L"26126 (HAP1024_B)", L"26127 (HAP1025_A)", L"26128 (HAP1025_B)",
    L"26129 (HAP1020_A)", L"26130 (HAP1020_B)", L"26131 (HAP1026_A)", L"26132 (HAP1026_B)", L"26133 (HAP3024_A)", L"26134 (HAP3024_B)",
    L"26135 (HAP2021_A)", L"26136 (HAP2021_B)", L"29000 (?Attack_Up)", L"29001 (?Defense_Up)", L"29002 (?MainInk_Save)", L"29003 (?SubInk_Save)",
    L"29004 (?InkRecovery_Up)", L"29005 (?HumanMove_up)", L"29006 (?SquidMove_up)", L"29007 (?SpecialIncrease_Up)", L"29008 (?SpecialTime_Up)",
    L"29009 (?RespawnTime_Save)", L"29010 (?RespawnSpecialGauge_Save)", L"29011 (?JumpTime_Save)", L"29012 (?BombDistance_Up)", L"29013 (?MarkingCancel)",
    L"29014 (?SquidMoveSpatter_Reduction)", L"29015 (?DeathMarking)", L"29016 (?RespawnRadar)", L"29500 (?SUP000)", L"29501 (?SUP001)"
};
extern const int g_ClothCount = 251;

const wchar_t* g_ShoeNames[] = {
    L"0 (\u3042\u308A\u306E\u307E\u307E)", L"1 (Cream Basics)", L"1000 (Blue Lo-Tops)", L"1001 (Banana Basics)", L"1002 (LE Lo-Tops)", L"1003 (White Seahorses)",
    L"1004 (Orange Lo-Tops)", L"1005 (Black Seahorses)", L"1006 (Clownfish Basics)", L"1007 (Yellow Seahorses)", L"1008 (Strapping Whites)",
    L"1009 (Strapping Reds)", L"1010 (Soccer Cleats)", L"1011 (LE Soccer Cleats)", L"2000 (Red Hi-Horses)", L"2001 (Zombie Hi-Horses)",
    L"2002 (Cream Hi-Tops)", L"2003 (Purple Hi-Horses)", L"2004 (Hunter Hi-Tops)", L"2005 (Red Hi-Tops)", L"2006 (Gold Hi_Horses)",
    L"2007 (SQUID GIRL Shoes)", L"2008 (Shark Mawcasins)", L"2009 (Mawcasins)", L"3000 (Pink Trainers)", L"3001 (Orange Arrows)",
    L"3002 (Neon Sea Slugs)", L"3003 (White Arrows)", L"3004 (Cyan Trainers)", L"3005 (Purple Sea Slugs)", L"3006 (Red Sea Slugs)",
    L"3007 (Blue Sea Slugs)", L"3008 (Crazy Arrows)", L"3009 (Black Trainers)", L"4000 (Oyster Clogs)", L"4001 (Choco Clogs)",
    L"4002 (Blueberry Casuals)", L"4003 (Plum Casuals)", L"4006 (Traditional Sandals)", L"5000 (Trail Boots)", L"5001 (Custom Trail Boots)",
    L"5002 (Pro Trail Boots)", L"6000 (Moto Boots)", L"6001 (Tan Work Boots)", L"6002 (Red Work Boots)", L"6003 (Blue Moto Boots)",
    L"6004 (Green Rain Boots)", L"6005 (Acerola Rain Boots)", L"6006 (Punk Whites)", L"6007 (Punk Cherries)", L"6008 (Punk Yellows)",
    L"6009 (Bubble Rain Boots)", L"6010 (Snowy Down Boots)", L"6011 (Icy Down Boots)", L"7000 (Blue Slip-Ons)", L"7001 (Red Slip-Ons)",
    L"7002 (Squid-Stitch Slip-Ons)", L"8000 (White Kicks)", L"8001 (Cherry Kicks)", L"8002 (Turquoise Kicks)", L"8003 (Squink Wingtips)",
    L"8004 (Roasted Brogues)", L"25000 (School Shoes)", L"25001 (Samurai Shoes)", L"25002 (Power Boots)", L"27000 (Hero Runner Replica)",
    L"27001 (?MSN001)", L"27002 (?MSN002)", L"27003 (?MSN003)", L"27004 (Armour Boot Replicas)", L"28000 (Octoling Boots)",
    L"29000 (?Attack_Up)", L"29001 (?Defense_Up)", L"29002 (?MainInk_Save)", L"29003 (?SubInk_Save)", L"29004 (?Inkrecovery_Up)",
    L"29005 (?HumanMove_up)", L"29006 (?SquidMove_up)", L"29007 (?SpecialIncrease_Up)", L"29008 (?SpecialTime_Up)", L"29009 (?RespawnTime_Save)",
    L"29010 (?RespawnSpecialGauge_Save)", L"29011 (?JumpTime_Save)", L"29012 (?BombDistance_Up)", L"29013 (?TrapDetect)",
    L"29014 (?EnemyInkEffect_Reduction)", L"29015 (?SuperJumpSign_Hide)", L"29500 (?SUP000)", L"29501 (?SUP001)"
};
extern const int g_ShoeCount = 89;

const wchar_t* g_AbilityNames[] = {
    L"None", L"Unknown",
    L"0 (Damage Up)", L"1 (Defense Up)", L"2 (Ink Saver Main)", L"3 (Ink Saver Sub)",
    L"4 (Ink Recovery Up)", L"5 (Run Speed Up)", L"6 (Swim Speed Up)", L"7 (Special Charge Up)",
    L"8 (Special Duration Up)", L"9 (Quick Respawn)", L"10 (Special Saver)", L"11 (Quick Super Jump)",
    L"12 (Bomb Range Up)",
    L"100 (Opening Gambit)", L"101 (Last-Ditch Effort)", L"102 (Tenacity)", L"103 (Comeback)",
    L"104 (Cold-Blooded)", L"105 (Ninja Squid)", L"106 (Haunt)", L"107 (Recon)",
    L"108 (Bomb Sniffer)", L"109 (Ink Resistance Up)", L"110 (Stealth Jump)"
};
extern const int g_AbilityCount = 26;

const int g_AbilityIDs[] = {
    -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110
};

const wchar_t* g_SplatfestRankNames[] = { L"0 (Fangirl/boy)", L"1 (Fiend)", L"2 (Defender)", L"3 (Champion)", L"4 (Queen/king)" };
extern const int g_SplatfestRankCount = 5;

const wchar_t* g_MiiverseNames[] = { L"0 (On)", L"1 (Off)" };
extern const int g_MiiverseCount = 2;
