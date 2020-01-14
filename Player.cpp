#include <Arduino.h>
#include "epd.h"
#include "darkTower.h"
#include "player.h"

char g_strLogistics[3][15] = {"Warriors", "Food", "Gold"};
char g_strItems[4][15] =  {"Scout", "Healer", "Beast", "Sword"};



//***************************************************************************************
// Adjust inventory helper functions
/*
 each warrior in your force is responsible for carrying a maximum of 6 bags of your gold. 
 A Beast will carry 50 bags of gold. 
*/
void playerClass::AdjustGold(void)
{
  SERIALLOG_LN("Adjust Gold")
  int nMaxGold;
  if(bitRead(m_btInventory, INV_IDX_BEAST))//beast
    nMaxGold = 50;
  else
    nMaxGold = 0;

  nMaxGold += (m_arrLogistics[LOG_WARRIOR_IDX]* 6);
  if (m_arrLogistics[LOG_GOLD_IDX] > nMaxGold)
    m_arrLogistics[LOG_GOLD_IDX] = nMaxGold;
}

void playerClass::Check4Curse()
{
  SERIALLOG_LN("Check 4 curse")
    if(m_nCursed>0)
    {        
      //print("Cursed!");
    }

    while(m_nCursed>0)
    {
      m_nCursed -=1;
      QuarterDamage();
    }
      
}

/* playerClass::QuarterDamage()
 *  Reduse players warriors and gold by 1/4  
 *  Called by dragon attack or by curse
*/
void playerClass::QuarterDamage(int &nWarriors, int &nGold, bool bCurse)
{
  SERIALLOG_LN("QuarterDamage")
  if(m_arrLogistics[LOG_WARRIOR_IDX] > 3)
  {
    nWarriors = m_arrLogistics[LOG_WARRIOR_IDX] * 0.25;
      m_arrLogistics[LOG_WARRIOR_IDX] -= nWarriors;
      SERIALLOG("Warriors Lost: ");
      SERIALLOG_LNN(nWarriors, DEC)
  }
  if(m_arrLogistics[LOG_GOLD_IDX] > 3)
  {
    nGold = m_arrLogistics[LOG_GOLD_IDX] * 0.25;
      m_arrLogistics[LOG_GOLD_IDX] -= nGold;
      SERIALLOG("Gold Lost: ");
      SERIALLOG_LNN(nGold, DEC)
  }
  if(bCurse)
    m_nCursed++;
}

void playerClass::QuarterDamage()
{
  int nWar, nGold;
  QuarterDamage(nWar, nGold, false);
      SERIALLOG("Warriors Lost: ");
      SERIALLOG_LNN(nWar, DEC)

}

/* playerClass::itemAward()
...and then perhaps a second item, if you don't already have it:
30% Key, 20% Sword, 10% Pegasus, 10% Wizard, and 30% nothing else
*/
void playerClass::ItemAward()
{
  SERIALLOG_LN("ItemAward");
  unsigned long seed = (analogRead(1) +analogRead(5))* millis() + analogRead(2) -analogRead(4);
  randomSeed(seed);
    SERIALLOG("Seed: ");
    SERIALLOG_LNN(seed,DEC);

  SERIALLOG_LN("ItemAward")
  if(random(1,100) <=5)
    return;//There's a 5% chance after a battle that you get jack squat...
  int OldGold = m_arrLogistics[LOG_GOLD_IDX];
  m_arrLogistics[LOG_GOLD_IDX] += random(10,20);
  AdjustGold();
  
  m_eEndTurnState = eGold;
  epd_clear_rect(EVENT_X0, 84, 800, 145);
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Gold", EVENT_X0+100, 85 );

   
  epd_disp_bitmap("G_.bmp",EVENT_X0,EVENT_Y0);
  char strTmp[5];
  int nTmp = (m_arrLogistics[LOG_GOLD_IDX]- OldGold);
  String(nTmp).toCharArray(strTmp,4);
  epd_disp_string(strTmp, EVENT_X0+35, 85);
  DisplayInventory(); 

  int nKey = INV_IDX_HOMEKEY;
  switch(m_nLand)
  {
    case 1:
      nKey = INV_IDX_BRASSKEY;
      break;
    case 2:
      nKey = INV_IDX_SILVERKEY;
      break;
    case 3: 
      nKey = INV_IDX_GOLDKEY;
      break;       
  }
  
  int nAward = random(1,100);
    SERIALLOG_LNN(nAward, DEC);
    SERIALLOG("Inventory: 0x");
    SERIALLOGN(m_btInventory,HEX);
  
  if(nAward <= AWARD_KEY )
  {
    if(bitRead(m_btInventory, nKey))
    {
      SERIALLOG_LN("nothing extra");
      return;        
    }
    epd_update();
    delay(2000);

    SERIALLOGN(nKey,DEC);
    SERIALLOGN(!bitRead(m_btInventory, nKey),DEC);
    SERIALLOG_LN(" Key");
    
    bitSet(m_btInventory, nKey);
    epd_disp_string("Key!", X_LOGISTICS, Y_TXT_OFFSET);
    DisplayKeys();
    m_eEndTurnState = eKey;
    
  }
  else if(nAward <= AWARD_SWORD )
  {
    if(bitRead(m_btInventory, INV_IDX_SWORD))
    {
      SERIALLOG_LN("nothing extra");
      return;        
    }
    epd_update();
    delay(2000);

    SERIALLOG_LN("AWARD_SWORD");
    bitSet(m_btInventory, INV_IDX_SWORD);    
//    epd_disp_string("Sword!", X_LOGISTICS, Y_TXT_OFFSET);
//    epd_disp_bitmap("sword.bmp",EVENT_X0,EVENT_Y0);
    DisplayInventory(); 
    m_eEndTurnState = eSword;

  }
  else if(nAward <= AWARD_PEG)
  {
    SERIALLOG_LN("AWARD_PEG");
    epd_update();
    delay(2000);

    epd_disp_string("Pegasus!", X_LOGISTICS, Y_TXT_OFFSET);
//    epd_disp_bitmap("Pega.bmp",EVENT_X0,EVENT_Y0);
    m_eEndTurnState = ePegasus;

  }
  else if(nAward <= AWARD_WIZ)
  {
    epd_update();
    delay(2000);

    SERIALLOG_LN("AWARD_WIZ");
    epd_disp_string("Wizard!", X_LOGISTICS, LOG_TEXT_Y);
 //   epd_disp_bitmap("Wiz.bmp",EVENT_X0,EVENT_Y0);
    //TODO
    WizardCurse(m_nPlayer);
    DisplayInventory();
    m_eEndTurnState = eWizard;

  }
  else
  {//nothing extra
    SERIALLOG_LN("nothing extra");
    return;
  }
//  epd_update();

}

/*RATIONS OF FOOD. Food is a valuable commodity. Each player begins the game with 25 rations of food. 
Each turn you take, your warrior force consumes a certain amount of food whether you move your warrior 
pawn or not on the gameboard. 
 - 15 warriors or less will consume one ration of food per turn; 
 - 16 to 30 warriors will consume two rations of food per turn; 
 - 31 to 45 warriors will consume three rations of food per turn; 
 - 46 to 60 warriors will consume four rations of food per turn; 
 - 61 to 75 warriors will consume 5 rations of food per turn; 
 - 76 to 90 warriors will consume 6 rations of food per turn; 
 - 91 to 99 warriors will consume 7 rations of food per turn.

 Warning when only 4 turns of food left
 */
void playerClass::ConsumeFood()
{
  SERIALLOG_LN("ConsumeFood")
  m_eEndTurnState = eEndTurn;
  int old= m_arrLogistics[LOG_FOOD_IDX];
  m_arrLogistics[LOG_FOOD_IDX] -= (floor(m_arrLogistics[0]-1)/15)/*+1*/;
  
  if(m_arrLogistics[LOG_FOOD_IDX] <0)
  {
      m_arrLogistics[LOG_WARRIOR_IDX] += m_arrLogistics[LOG_FOOD_IDX];// lose 1 warrior for every ration of food short
      m_arrLogistics[LOG_FOOD_IDX] = 0;
      AdjustGold();
  }
      //print("Starvation! Warriors: " + str(self.m_nWarriors));
  //print ("Food " + str(old) + " => " + str(self.m_nFood ));

}

//***************************************************************************************
// MOVES
/**/
void playerClass::Frontier()
{
  SERIALLOG_LN("Frontier")
  if(m_nLand>=4)
    return;
  SERIALLOG("m_btInventory - ")
  SERIALLOG_LNN(m_btInventory, HEX)

  SERIALLOG("land - ")
  SERIALLOG_LNN((m_nLand), DEC)

  SERIALLOG("bitRead - ")
  SERIALLOG_LNN(bitRead(m_btInventory, (INV_IDX_HOMEKEY+m_nLand)), DEC)
    
  if(bitRead(m_btInventory,  m_nLand+INV_IDX_HOMEKEY))
  {
    m_nLand += 1;
  }
  else
  {
//    LOG_MSG("eMissing")
    m_eEndTurnState = eMissKey;
  }
  PostMsg(eMSG_ENDTURN);

}

void playerClass::Citadel()
{
  //back home with all the keys - more generous rule
  if(m_nLand == 4 and m_arrLogistics[LOG_WARRIOR_IDX] > 4 and m_arrLogistics[LOG_WARRIOR_IDX] < 25 and bitRead(m_btStat, STAT_OK2DBL))
      m_arrLogistics[LOG_WARRIOR_IDX] = m_arrLogistics[LOG_WARRIOR_IDX] * 2;
      bitSet(m_btStat, STAT_OK2DBL);
      
  if(m_arrLogistics[LOG_WARRIOR_IDX] < 5)
      m_arrLogistics[LOG_WARRIOR_IDX] += random(4,10);
  if(m_arrLogistics[LOG_FOOD_IDX] <6)
      m_arrLogistics[LOG_FOOD_IDX] += random(4,25);  
  if(m_arrLogistics[LOG_GOLD_IDX] < 8)
      m_arrLogistics[LOG_GOLD_IDX] += random(4,20);

  DisplayInventory();
  PostMsg(eMSG_ENDTURN);  
}

void playerClass::Move()
{
  unsigned long Event = (analogRead(1) +analogRead(5)+ millis() - analogRead(2) -analogRead(4))%100;
    char EventBMP[] = "__.bmp";

  if(Event <=  MOVE_BATTLE)
  {
    SERIALLOG_LN("Battle");
    LOG_MSG("Battle");
  }
  else if(Event <=  MOVE_LOST)
  {
    //_L
    SERIALLOG_LN("Lost");
//    LOG_MSG("Lost");
    if(!bitRead(m_btInventory, INV_IDX_SCOUT))
    {
      m_eEndTurnState = eLost;
      PostMsg(eMSG_ENDTURN);
      return;        
    }
    EventBMP[1] = 'L';
    epd_disp_bitmap(EventBMP,EVENT_X0,EVENT_Y0);
 
    epd_update();
    delay(2000);

//    LOG_MSG("Scout");
    SERIALLOG_LN("Scout found trail");
    m_eEndTurnState = eScout;   
  }
  else if(Event <=  MOVE_PLAGUE)
  {
    //_P
    SERIALLOG_LN("Plague");
//    LOG_MSG("Plague");
    if(!bitRead(m_btInventory, INV_IDX_HEALER))
    {
      m_eEndTurnState = ePlague;
      QuarterDamage();
      PostMsg(eMSG_ENDTURN);
      return;        
    }
    EventBMP[1] = 'P';
    epd_disp_bitmap(EventBMP,EVENT_X0,EVENT_Y0);
    
    epd_update();
    delay(2000);
 //   LOG_MSG("Healer");
    SERIALLOG_LN("Healer cures");
    m_eEndTurnState = eHealer;   
  }
  else if(Event <=  MOVE_DRAGON)
  {//_D
    SERIALLOG_LN("Dragon");
 //   LOG_MSG("Dragon");
    if(!bitRead(m_btInventory, INV_IDX_SWORD))
    {
      int W;
      int G;
      m_eEndTurnState = eDragon;
      QuarterDamage(W, G, false);
      //todo add damage to pool
      PostMsg(eMSG_ENDTURN);
      return;        
    }
    EventBMP[1] = 'D';
    epd_disp_bitmap(EventBMP,EVENT_X0,EVENT_Y0);
 
    epd_update();
    delay(2000);

//    LOG_MSG("Used Sword");
    SERIALLOG_LN("Used_SWORD");
    bitClear(m_btInventory, INV_IDX_SWORD);    
    //TODO Add Damage pool contents
    DisplayInventory(); 
    m_eEndTurnState = eSword;   
  }
    
  PostMsg(eMSG_ENDTURN);
}

void playerClass::Tomb(bool bItemAward)
{
  SERIALLOG_LN("*** Tomb ***"); 

  bitSet(m_btStat, STAT_OK2DBL);
  if(bItemAward)
  {
    ItemAward();
  }
  else
  {
    BattleInit();
  }
}

/**/

//*******************************************************
// Display
void playerClass::DisplayInventory()
{    
    epd_clear_rect(10, Y_LOGISTICS, X_LOGISTICS-1, LOG_Y - 1);
    epd_set_en_font(ASCII32); //ASCII32, ASCII48,  ASCII64

    char strTmp[5];

    for(int nIdx= 0; nIdx <3; nIdx++) 
    {// display warriors, food & gold
      String(int(m_arrLogistics[nIdx])).toCharArray(strTmp,5);
      int X0 = 10;
      if(m_arrLogistics[nIdx] < 10)
        X0 += 17;        

      epd_disp_string(strTmp, X0, Y_LOGISTICS +(nIdx*Y_TXT_OFFSET));
      epd_disp_string(g_strLogistics[nIdx], X_LOGISTICS, Y_LOGISTICS+(nIdx*Y_TXT_OFFSET));
    }

    for(int nIdx= 0; nIdx <4; nIdx++) 
    {// display scout, healer, beast, sword
      epd_disp_string(g_strItems[nIdx], X_LOGISTICS, Y_LOGISTICS+((3+nIdx)*Y_TXT_OFFSET));
      epd_draw_circle(25, 15 + Y_LOGISTICS +  ((3+nIdx)*Y_TXT_OFFSET),10, (m_btInventory & (INV_SCOUT<<nIdx)));
    }
}

void playerClass::DisplayKeys()
{
    int x2 = 180;
    int x3 = x2 + KEY_WIDTH;

    for(int nIdx= 1; nIdx <=3; nIdx++) 
    {
      
      int y = 137 + ((KEY_HEIGHT + BOX_OFFSET) * nIdx) ;
      int y1 = y + KEY_HEIGHT;
      char keys[] = "kX.bmp";
      if(m_btInventory & (INV_HOMEKEY<<nIdx))
      {
        keys[1] = '0' + nIdx;
         epd_disp_bitmap(keys,x2, y); 
      }
      epd_draw_rect( x2, y, x3, y1, false);
    }
}
void playerClass::DisplayPlayer(gamestate nState)
{
    SERIALLOG_LN("DisplayPlayer"); 

    epd_clear();

    DisplayDarkTower();    
    
    epd_set_en_font(ASCII32); //ASCII32, ASCII48,  ASCII64
    epd_disp_string("Player", 0, 0);

    char strTmp[5];
    int nTmp = m_nPlayer;
    String(nTmp).toCharArray(strTmp,4);
    epd_disp_string(strTmp, 85, 0);

    epd_draw_line(BAR_X_START,146, BAR_X_END, 146 );
    
    epd_set_en_font(ASCII32); //ASCII32, ASCII48,  ASCII64
    epd_disp_string("Territory", 10, 170);

    epd_draw_line(BAR_X_START,220, BAR_X_END, 220 );

    DisplayInventory();
    DisplayKeys();
    
// DRAW EVENT GRAPHIC
    char EventBMP[] = "__.bmp";

    SERIALLOG("End Turn State - ")
    SERIALLOG_LNN(m_eEndTurnState, DEC)
    SERIALLOG_LNN(eMissKey,DEC)
    switch(m_eEndTurnState)
    {
      case eClosed:
        EventBMP[1] = 'B';
        break;
      case eCursed:
        EventBMP[1] = 'C';
        break; //_c
      case eDragon:
        // LOG_MSG("Dragon");
        EventBMP[1] = 'D';
        break;//_d
      case eMissKey:
        SERIALLOG_LN("Missing Key")
        EventBMP[1] = 'K';
        break;
      case eLost:
        //LOG_MSG("Lost");
        EventBMP[1] = 'L';
        break;  //_l
      case eScout:
        EventBMP[0] = '-';
        EventBMP[1] = 'S';
        break;
      case ePlague:
        EventBMP[1] = 'P';
        //LOG_MSG("Plague");
        break; //_p 
      case eHealer:
        EventBMP[0] = '-';
        EventBMP[1] = 'H';
        break;
      case eGold:
        EventBMP[0] = 'G';
        break; //G_ 
      case eKey:
        EventBMP[0] = '1';
        break; //1_  TODO Key number
      case eSword:
        EventBMP[0] = 'S';
        break; //S_
      case ePegasus:
        EventBMP[0] = 'P';
        break; //P_         
      case eWizard:  
        EventBMP[0] = 'W';
        break; //W_ 
        
      case eVictory:
        EventBMP[1] = 'V';
        break; //_v  
/*      case eBuy:
        char Item[] = "WFSHB";
        EventBMP[0] = '-';
        EventBMP[1] = Item[m_nBazaarIdx];
        break;
*/
      default:
        SERIALLOG_LN("default End Turn")
        char L[] = "ABDZa";
        EventBMP[0] = 'L';
        EventBMP[1] = L[m_nLand];
        break;
    }
   // LOG_MSG(EventBMP)
    SERIALLOG_LN(EventBMP)

    epd_disp_bitmap(EventBMP,EVENT_X0,EVENT_Y0);
    epd_draw_rect( EVENT_X0,EVENT_Y0,EVENT_X1,EVENT_Y1, false);

// Display Turn log
    epd_draw_line(BAR_X_START, LOG_Y, 800, LOG_Y );

//    epd_disp_string("Turn Log", X_LOGISTICS, LOG_TEXT_Y);
  DisplayDynBtnBoxes();
  switch (nState)
  {
    case eIdle:
      DisplayMoveButton();
    break;
    case eEndTurn:
      DisplayEndButton();
    break;
    default:
    break;
  }
  epd_update();
}
//*************************************************************************************
//Bazaar

void playerClass::InitBazaar()
{
    SERIALLOG_LN("InitBazaar");

    m_nBazaarIdx = 0;
    m_arrBazaarPrices[0] = random( 5, 10);  //warrior
    m_arrBazaarPrices[1] = random( 1,  3);  //food
    m_arrBazaarPrices[2] = random(15, 25);  //Scout
    m_arrBazaarPrices[3] = random(15, 25);  //Healer
    m_arrBazaarPrices[4] = random(15, 25);  //Beast
    
    DisplayDynBtnBoxes();
    DisplayNextButton();
    DisplayHaggleBtn();
    DisplayBuyButton();
    DisplayBackButton();

    m_parrItemNames[0] = g_strLogistics[0];
    m_parrItemNames[1] = g_strLogistics[1];
//    m_nBazaarItems = 2;

    for (int nIdx = 0; nIdx <3; nIdx++)
    {
      SERIALLOG(g_strItems[nIdx]);
      if(bitRead(m_btInventory, /*(INV_SCOUT<<*/nIdx/*)*/))
      {
        SERIALLOG_LN(" Skipped");
        m_parrItemNames[nIdx+2] = NULL;
      }
      else
      {
        SERIALLOG_LN(" Added");
        m_parrItemNames[nIdx+2] = g_strItems[nIdx];
      }
    }
    PostMsg(eMSG_BAZAAR);    
}

void playerClass::BazaarItemNext()
{
  SERIALLOG_LN("ItemNext"); 

  //LOG_MSG("Bazaar Next");
  do
  {
    m_nBazaarIdx++;
    if(m_nBazaarIdx > 4)
      m_nBazaarIdx = 0;
  }while(m_parrItemNames[m_nBazaarIdx] == NULL);
}

void playerClass::BazaarItemBack()
{
  do
  {
    if(m_nBazaarIdx ==0)
      m_nBazaarIdx = 4;
    else
      m_nBazaarIdx --;
  }while(m_parrItemNames[m_nBazaarIdx] == NULL);
   
}


void playerClass::BazaarItem()
{
  SERIALLOG("BazaarItem - "); 

  epd_clear_rect(EVENT_X0, 84, 800, 145);
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Gold", EVENT_X0+100, 85 );
  
  char ItemBmp[] = "-X.bmp";
  char IB[] = "WFSHB";
  ItemBmp[1] = IB[m_nBazaarIdx];
  epd_disp_bitmap(ItemBmp,EVENT_X0,EVENT_Y0);

  char strTmp[5];
  int nTmp = m_arrBazaarPrices[m_nBazaarIdx];
  String(nTmp).toCharArray(strTmp,4);
  epd_disp_string(strTmp, EVENT_X0+35, 85);
  SERIALLOG_LN(m_parrItemNames[m_nBazaarIdx]); 

  epd_update();
}

void playerClass::Haggle()
{
/*  haggle()
  The rulebook states "approximately a 50/50 chance of lowering the stated price..."
  48% haggle success, 52% bazaar closed
*/
  SERIALLOG_LN("Haggle"); 

  unsigned long seed = (analogRead(1) +analogRead(5))* millis() + analogRead(2) -analogRead(4);
  randomSeed(seed);
  if((random(1,1000) <=480) && (m_arrBazaarPrices[m_nBazaarIdx]> m_arrMinPrices[m_nBazaarIdx]))
  {
    //LOG_MSG("Bazaar Success")
    m_arrBazaarPrices[m_nBazaarIdx]--;
    BazaarItem();
  }
  else
  {
    //LOG_MSG("Bazaar Closed")
    m_eEndTurnState =eClosed;
    PostMsg(eMSG_ENDTURN);
    }
}

void playerClass::BazaarInitBuy()
{
  //LOG_MSG("BazaarInitBuy");
  SERIALLOG_LN("BazaarInitBuy"); 

  epd_clear_rect(0, Y_BTN_TXT, 199, 600);
  epd_clear_rect(201, Y_BTN_TXT, 399, 600);
  DisplayDoneButton();
  BazaarBuy();
}

void playerClass::BazaarBuy()
{
  SERIALLOG("BazaarBuy - "); 
  //LOG_MSG("BazaarBuy");
  SERIALLOGN(m_nBazaarIdx, DEC)
  SERIALLOG_LN(m_parrItemNames[m_nBazaarIdx])
  
  if(m_arrBazaarPrices[m_nBazaarIdx] > m_arrLogistics[2])
  {
    SERIALLOG_LN("No loans"); 
    SERIALLOG("Price - "); 
    SERIALLOG_LNN(m_arrBazaarPrices[m_nBazaarIdx], DEC); 
    SERIALLOG("Gold - "); 
    SERIALLOG_LNN(m_arrLogistics[2], DEC); 

    m_eEndTurnState =eClosed;
    PostMsg(eMSG_ENDTURN);
    return;
  }

  SERIALLOGN(m_arrLogistics[LOG_GOLD_IDX],DEC)
  m_arrLogistics[LOG_GOLD_IDX] -= m_arrBazaarPrices[m_nBazaarIdx];
  SERIALLOG_LNN(m_arrLogistics[LOG_GOLD_IDX],DEC)
  
  if(m_nBazaarIdx >1)
  {//Buy Now (single item)
    //LOG_MSG("Buy Now (single item)"); 
    SERIALLOG_LN("Buy Now (single item)")
    SERIALLOGN(m_btInventory,HEX)
    bitSet(m_btInventory, m_nBazaarIdx-2);
    SERIALLOG_LNN(m_btInventory,HEX)
    
    PostMsg(eMSG_ENDTURN);
  }
  else
  { 
    if(m_arrLogistics[m_nBazaarIdx] >= 99)
    { 
      m_eEndTurnState =eClosed;
      PostMsg(eMSG_ENDTURN);
    }
    m_arrLogistics[m_nBazaarIdx]++;
    DisplayInventory();
    epd_update();
  }
}
//*************************************************************************************
//Battle
void playerClass::BattleInit()
{
  SERIALLOG_LN("*** BattleInit *** "); 
  
  unsigned long nTmp = (analogRead(1) +analogRead(5)+ millis())%8;
  m_nBrigands = m_arrLogistics[LOG_WARRIOR_IDX] + nTmp - 2;
  g_nLastBattleTurn = millis();
  
  SERIALLOG("BattleInit - ");
  SERIALLOG_LNN(nTmp, DEC); 
  SERIALLOG("Brigands - ");
  SERIALLOG_LNN(m_nBrigands, DEC); 
 
  DisplayBattleEvent(false);
  PostMsg(eMSG_BATTLE);  
}

void playerClass::DisplayBattleEvent(bool bWarrior)
{
  SERIALLOG_LN("*** DisplayBattleEvent *** "); 
  char Event[]= "BS.BMP";
  char strTmp[5] = {0,0,0,0,0};
  int nTmp;

  DisplayInventory(); 
  epd_clear_rect(0, 84, 800, 145);

  if(  bWarrior)
    Event[0] = 'W';
  epd_disp_bitmap(Event,EVENT_X0,EVENT_Y0);

  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Warriors", 60, 85);
  nTmp = m_arrLogistics[LOG_WARRIOR_IDX];
  String(nTmp).toCharArray(strTmp,5);
  epd_disp_string(strTmp, 1, 85);

  epd_disp_string("Brigands", 600, 85);
  nTmp = m_nBrigands;
  String(nTmp).toCharArray(strTmp,4);
  SERIALLOG("Display Brigands - ");
  SERIALLOG_LNN(m_nBrigands, DEC);

  epd_disp_string(strTmp, 565, 85);

  epd_update();
}

double playerClass::GetOdds(double warriors, double brigands)
{
    if ( warriors > brigands )
      return (0.75 - ( brigands / ( 4.0 * warriors) ))*100;
    return (0.25 + ( warriors / ( 4.0 * brigands) ))*100;

}

void playerClass::BattleRound()
{
  //This is the most complicated formula. This basically takes the proportional 
  //delta between the Warriors and Brigands, and scales them up and down from a 50% 
  //victory with the same number, to a best-case Warrior percentage of 75% and a worst-case of 25%.
  SERIALLOG_LN("*** BattleRound *** "); 

//  double nW = m_arrLogistics[LOG_WARRIOR_IDX];
//  double nB = m_nBrigands;
  double nOdds = GetOdds(m_arrLogistics[LOG_WARRIOR_IDX], m_nBrigands);
 
  SERIALLOG("Odds - "); 
  SERIALLOG_LNN(nOdds, DEC); 
  int BattleResult = ((analogRead(1) +analogRead(5)+ millis())%100);
  SERIALLOG("Battle Result - "); 
  SERIALLOG_LNN(BattleResult, DEC); 

//  int nTmp = nOdds;
  char strTmp[21];
  String xString = "O: " + String(nOdds);
  xString += " R: " + String(BattleResult);
  xString.toCharArray(strTmp,20);
  
  LOG_MSG(strTmp);

  if( BattleResult < nOdds)
  {//warriors win
    unsigned long nTmp = (analogRead(1) +analogRead(5)+ millis())%6;
    SERIALLOG("Warriors Win nTmp - "); 
    SERIALLOG_LNN(nTmp, DEC); 
    double dTmp = nTmp;
    double PercentKilled = (dTmp)/10;
    SERIALLOG("PercentKilled - "); 
    SERIALLOG_LNN(PercentKilled, DEC); 

    nTmp = floor(m_nBrigands * PercentKilled);
    if(nTmp <1)
      nTmp = 1;

    SERIALLOG("Brigands Killed - "); 
    SERIALLOG_LNN(nTmp, DEC); 
    
    m_nBrigands -= nTmp;
    if(m_nBrigands <0)
      m_nBrigands =0;
    DisplayBattleEvent(true);
    
    SERIALLOGN(m_arrLogistics[LOG_WARRIOR_IDX], DEC);
    SERIALLOG_LN(" - Warriors win"); 
    SERIALLOGN(m_nBrigands, DEC);
    SERIALLOG_LN(" - Brigands"); 
    if(m_nBrigands == 0)
    {
      ItemAward();
      PostMsg(eMSG_ENDTURN);
      return;
    }
  }
  else
  {//brigands win
    int nTmp;
//    if(m_arrLogistics[LOG_WARRIOR_IDX]>8)
//      nTmp = ((analogRead(1) +analogRead(5)+ millis())%5)+1;
//    else
      nTmp = 1;
    m_arrLogistics[LOG_WARRIOR_IDX] -= nTmp;
    DisplayBattleEvent(false);
    SERIALLOGN(m_arrLogistics[LOG_WARRIOR_IDX], DEC);
    SERIALLOG_LN(" - Warriors"); 
    SERIALLOGN(m_nBrigands, DEC);
    SERIALLOG_LN(" - Brigands win"); 

    if(m_arrLogistics[LOG_WARRIOR_IDX] <0)
    {
      if(m_nPlayers >1)
        m_arrLogistics[LOG_WARRIOR_IDX] = 1;
      else
        m_arrLogistics[LOG_WARRIOR_IDX] = 0;
    }

    if(m_nPlayers >1 && m_arrLogistics[LOG_WARRIOR_IDX] ==1 )
    {
      SERIALLOG_LN("End Turn");
      PostMsg(eMSG_ENDTURN);
      return;     
    }
    if(m_nPlayers ==1 && m_arrLogistics[LOG_WARRIOR_IDX] ==0 )
    {// game over
      SERIALLOG_LN("Game Over");
      PostMsg(eMSG_ENDTURN);
      return;     
    }
  }

  SERIALLOG_LN("reset battleTurn");
  g_nLastBattleTurn = millis();
  PostMsg(eMSG_BATTLE);   
}
