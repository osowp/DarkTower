from enum import Enum, auto

DEBOUNCE = 500

BTN01 = 12335
BTN02 = 12965
BTN03 = 13625
BTN04 = 14360
BTN05 = 15185
BTN06 = 16100
BTN07 = 17150
BTN08 = 18350
BTN09 = 19705
BTN10 = 21295
BTN11 = 23165
BTN12 = 25365

#Queue Messages
class Queue_Msgs(Enum):
    eNoButton = auto()
    eInitialize = auto()
    eOneBtn = auto()
    eTwoBtn = auto()
    eThreeBtn = auto()
    eFourBtn = auto()
    eBazaarBtn = auto()
    eTombBtn = auto()
    eMoveBtn = auto()
    eCitadelBtn = auto()
    eDarkTwrBtn = auto()
    eFrontierBtn = auto()
    eAbortBtn = auto()
    eDisplaySetup1 = auto()
    eDisplaySetup2 = auto()
    eReadyPlayer = auto()
    eIdle = auto()
    eEndTurn = auto()
    eBazaarItem = auto()
    eBazaarInitBuy = auto()
    eBazaarBuy = auto()
    eBattleMsg = auto()
    eHandlerCount = auto()



Y_BTN_TXT 	= 551
Y_BTN_BOX 	= 550
LOG_Y       =  509
LOG_TEXT_Y  =  (LOG_Y + 1)

#define FONT_Y_OFFSET 40
#define Y_TXT_OFFSET  40

#define BAR_X_START   0
#define BAR_X_END     300
#define BOX_OFFSET    5
#define KEY_WIDTH     120
#define KEY_HEIGHT    88
#define EVENT_X0      305
#define EVENT_Y0      146
#define EVENT_X1      (EVENT_X0 + 490)
#define EVENT_Y1      (EVENT_Y0 + 358)

#define X_LOGISTICS   50
#define Y_LOGISTICS   230

#define LOG_WARRIOR_IDX   0
#define LOG_FOOD_IDX      1
#define LOG_GOLD_IDX      2
#define LOG_SCOUT         3
#define LOG_HEALER        4
#define LOG_BEAST         5
 
#define INV_IDX_SCOUT     0
#define INV_IDX_HEALER    1
#define INV_IDX_BEAST     2
#define INV_IDX_SWORD     3
#define INV_IDX_HOMEKEY   4
#define INV_IDX_BRASSKEY  5
#define INV_IDX_SILVERKEY 6
#define INV_IDX_GOLDKEY   7

#define INV_SCOUT     0x01
#define INV_HEALER    0x02
#define INV_BEAST     0x04
#define INV_SWORD     0x08
#define INV_HOMEKEY   0x10
#define INV_BRASSKEY  0x20
#define INV_SILVERKEY 0x40
#define INV_GOLDKEY   0x80
#define INV_INIT      (INV_HOMEKEY )

#define STAT_RETREAT    0
#define STAT_OK2DBL     1
#define STAT_RESERVED     2
//#define STAT_RESERVED   3
//#define STAT_RESERVED   4
//#define STAT_RESERVED   5
//#define STAT_RESERVED   6
//#define STAT_RESERVED   7
#define STAT_INIT      (0x00)

//ODDS
#define TOMB_TREASURE 10
#define TOMB_BATTLE   TOMB_TREASURE+70
#define TOMB_NO_EVENT 100

//10% Plague, 10% Lost, 10% Dragon, 20% Battle, 50% safe move
#define MOVE_BATTLE  20
#define MOVE_LOST    (MOVE_BATTLE +10)
#define MOVE_PLAGUE  (MOVE_LOST +10)
#define MOVE_DRAGON  (MOVE_PLAGUE +10)

#define AWARD_KEY    30
#define AWARD_SWORD (AWARD_KEY + 20)
#define AWARD_PEG   (AWARD_SWORD + 10)
#define AWARD_WIZ   (AWARD_PEG + 10)

