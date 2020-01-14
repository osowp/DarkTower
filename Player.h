#ifndef  playerClass_H
#define  playerClass_H
#include "MsgHandlers.h"

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


extern char g_strLogistics[3][15];
extern char g_strItems[4][15];
class darkTower;

class playerClass
{
  private:
    darkTower *m_pGame;
    gamestate m_eEndTurnState;

    char m_nLand = 0;
    char m_nPlayer;
    char m_nBrigands;

//    char m_arrLogistics[3] = {3,3,3};//test 
    char m_arrLogistics[3] = {10,25,30};

    char m_btInventory = INV_INIT;// bit flags 
    char m_btStat = 0; 
    
    char m_nCursed = 0;// could be cursed by more than one player at the same time
    int m_nBazaarIdx;
    char* m_parrItemNames[5];
//    int m_nBazaarItems;
    int m_arrBazaarPrices[5];
    int m_arrMinPrices[5] = {5,1,14,14,14};
    
//adjust inventory functions
    void AdjustGold();// ready for test
    void ItemAward();
    
    void Check4Curse();
    void BattleInit();
    void DisplayBattleEvent(bool bWarrior);
    double GetOdds(double warriors, double brigands);

  public://init
    void SetPlayerNumber(char nPlayer){  m_nPlayer = nPlayer;}; 
    void SetGamePointer(darkTower *pGame){m_pGame = pGame;}
    void ConsumeFood();
    void QuarterDamage();
    void QuarterDamage(int &nWarriors, int &nGold, bool bCurse);
    void AddLoot(int nWarriors, int nGold){m_arrLogistics[LOG_WARRIOR_IDX] += nWarriors; m_arrLogistics[LOG_GOLD_IDX] += nGold;}
  public:// Moves
    void DisplayInventory();
    void DisplayKeys();
    void DisplayPlayer(gamestate nState);
    void Frontier();
    void Citadel();
    void Tomb(bool bItemAward);
    void Move();
    void BazaarPurchase(int Item, int nItems, int nGoldCost);
    void InitBazaar();
    void BazaarItemNext();
    void BazaarItemBack();
    void BazaarItem();
    void Haggle();
    void BazaarInitBuy();
    void BazaarBuy();
    void BattleRound();
};

#endif
