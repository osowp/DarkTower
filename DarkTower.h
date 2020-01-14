#ifndef  DarkTower_H
#define  DarkTower_H


#include "cppQueue.h"
#include "Player.h"
#include "MsgHandlers.h"

//void LogMsg(char * pMsg);

//class darkTower
//{
//  private:
    //Queue  qMsg(sizeof(Queue_Msgs), 10, FIFO); // Instantiate queue
    //Queue  m_qMsg(4, 10, FIFO); // Instantiate queue
    extern gamestate; // m_eGameState = ePlayer;
    extern playerClass m_arrPlayers[4];
    extern char DT_Brigands;
    extern unsigned long g_nLastBattleTurn;
    extern char m_nPlayers;

    void SetNPlayers(int nPlayers);//{m_nPlayers = nPlayers;}
    void SetLevel(int nLevel);//{m_nLevel = nLevel;}
    
//  public:
    void Init();
    void BlankButton(int nBtn);
    void DisplayDynBtnBoxes();
    void DisplayNumButtons();
    void DisplayDarkTower();
    void DisplayYesButton();
    void DisplayNoButton();
    void DisplayEndButton();
    void DisplayRetreatButton();
    
    void DisplayNextButton();
    void DisplayMoveButton();
    void DisplayHaggleBtn();
    void DisplayBuyButton();
    void DisplayDoneButton();
    void DisplayBackButton();

    void OneBtn();
    void TwoBtn();
    void ThreeBtn();
    void FourBtn();
//    void HaggleBtn();
    void BazaarBtn();
    void TombBtn();//{m_arrPlayers[m_nCurrentPlayer].Tomb();}
    void MoveBtn();//{m_arrPlayers[m_nCurrentPlayer].Move();}
    void CitadelBtn();
   void DarkTwrBtn();
    void FrontierBtn();
    void AbortBtn();
    void DisplaySetup1();
    void DisplaySetup2();
    void ReadyPlayer();
    void Idle();//     {m_eGameState = eIdle;  m_arrPlayers[m_nCurrentPlayer].DisplayPlayer(m_eGameState); }
    void EndTurn();//  {m_eGameState = eEndTurn;   m_arrPlayers[m_nCurrentPlayer].DisplayPlayer(m_eGameState); }
    void BazaarItem();//{m_eGameState = eBazaar; m_arrPlayers[m_nCurrentPlayer].BazaarItem();}
    void BazaarInitBuy();//{m_arrPlayers[m_nCurrentPlayer].BazaarInitBuy();}
    void BazaarBuy();//{m_arrPlayers[m_nCurrentPlayer].BazaarBuy();}
    void BazaarClosed();
    void BattleMsg();
    void WizardCurse(int Player);
//};

/*
moveResult()
When the Move button is pressed:
10% Plague, 10% Lost, 10% Dragon, 20% Battle, 50% safe move
* /
void moveResult(void);
/*
tombruinResult()
When the Tomb/Ruin button is pressed:
10% treasure, 20% empty, 70% battle
* /
void tombruinResult(void);

/*
startingBrigands()
When the battle begins, the starting Brigand count is:
your Warriors plus 0 to 5 more
* /
void startingBrigands(int nWarriors);

/*
oddsOfVictory()
This is the most complicated formula. This basically takes the proportional delta between the Warriors and Brigands, and scales them up and down from a 50% victory with the same number, to a best-case Warrior percentage of 75% and a worst-case of 25%.
* /
void oddsOfVictory(int nWarriors, int nBrigands);

/*
anyTreasure()
There's a 5% chance after a battle that you get jack squat...
* /
void anyTreasure(void);

/*
goldAward()
...but if you do get some treasure, you start out with 10-20 bags of gold...
* /
void goldAward(void);

/*
itemAward()
...and then perhaps a second item, if you don't already have it:
30% Key, 20% Sword, 10% Pegasus, 10% Wizard, and 30% nothing else
* /
void itemAward(void);

/*
haggle()
The rulebook states "approximately a 50/50 chance of lowering the stated price..."
48% haggle success, 52% bazaar closed
* /
void haggle(void);

/*
finalScore()
Final score is 128 minus:
the sum of the moves it took to finish plus the warriors left alive at the end
* /
void finalScore(void);
*/
#endif /* DarkTower_H*/
