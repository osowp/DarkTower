#include <Arduino.h>
#include "epd.h"
#include "darkTower.h"
#include "MsgHandlers.h"

char m_nPlayers = 0;
char m_nLevel = 0;
char m_nCurrentPlayer = 0;
int m_nTurn = 0;
char DT_Brigands=0;
unsigned long g_nLastBattleTurn;


gamestate m_eGameState = ePlayer;
playerClass m_arrPlayers[4];

void SetNPlayers(int nPlayers){m_nPlayers = nPlayers;}
void SetLevel(int nLevel){m_nLevel = nLevel;}


void LogMsg(char * pMsg)
{
  epd_clear_rect(0, LOG_TEXT_Y, 800, Y_BTN_BOX - 1);
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string(pMsg, 0, LOG_TEXT_Y);
  epd_update();  
}


void Init()
{
  SERIALLOG_LN("Init");

  for(char nIdx = 0; nIdx<4;nIdx++)
  {
    m_arrPlayers[nIdx].SetPlayerNumber(nIdx+1);
//    m_arrPlayers[nIdx].SetGamePointer(this);
  }
}

void DisplayDarkTower()
{  
  epd_set_en_font(ASCII64); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Dark Tower",250,0 );

  if(m_nLevel >0)
  {
    epd_set_en_font(ASCII32); //ASCII32, ASCII48,  ASCII64
    epd_disp_string("Level:",700,0 );
    char strTmp[5];
    String((int )m_nLevel).toCharArray(strTmp,4);
    epd_disp_string(strTmp, 775, 0);
  }
  epd_draw_line(BAR_X_START,75, 800, 75 );
  epd_draw_line(BAR_X_START,76, 800, 76 );
}

void DisplayDynBtnBoxes()
{
  epd_draw_line( 0,Y_BTN_BOX,  0, 600 );
  epd_draw_line(200,Y_BTN_BOX, 200, 600 );
  epd_draw_line(400,Y_BTN_BOX, 400, 600 );
  epd_draw_line(600,Y_BTN_BOX, 600, 600 );
  epd_draw_line(800,Y_BTN_BOX, 800, 600 );
  epd_draw_line( 0,Y_BTN_BOX, 800, Y_BTN_BOX );  
}

void BlankButton(int nBtn)
{
  epd_clear_rect(1 + (200*(nBtn-1)), Y_BTN_TXT, 199 + (200*(nBtn-1)), 600);
}

void DisplayMoveButton()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(1);
  epd_disp_string("Move", 50, Y_BTN_TXT);
}

void DisplayYesButton()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(2);
  epd_disp_string("YES", 266, Y_BTN_TXT);
}

void DisplayNoButton()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(3);
  epd_disp_string("No", 470, Y_BTN_TXT);
}

void DisplayBackButton()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(1);
  epd_disp_string("Back", 66, Y_BTN_TXT);
}
void DisplayNextButton()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(4);
  epd_disp_string("Next", 660, Y_BTN_TXT);
}

void DisplayHaggleBtn()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(2);
  epd_disp_string("Haggle", 250, Y_BTN_TXT);
}
void DisplayBuyButton()
{
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  BlankButton(3);
  epd_disp_string("Buy", 470, Y_BTN_TXT);
}

void DisplayEndButton()
{
  SERIALLOG_LN("Display End Button");
  BlankButton(1);
  BlankButton(2);
  BlankButton(3);
  BlankButton(4);

  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("End", 680, Y_BTN_TXT);
}

void DisplayRetreatButton()
{
  SERIALLOG_LN("Display End Button");
  BlankButton(1);
  BlankButton(2);
  BlankButton(3);
  BlankButton(4);

  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Reteat!", 640, Y_BTN_TXT);
}

void DisplayDoneButton()
{
  SERIALLOG_LN("Display Done Button");
  BlankButton(4);

  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Done", 665, Y_BTN_TXT);
}


void DisplayNumButtons()
{  
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  
  epd_disp_string("1", 100, Y_BTN_TXT);
  epd_disp_string("2", 300, Y_BTN_TXT);
  epd_disp_string("3", 500, Y_BTN_TXT);
  epd_disp_string("4", 700, Y_BTN_TXT);
}

// *********************************************  Button message handlers  
void OneBtn()
{
  SERIALLOG_LN("Handle OneBtn");
  switch(m_eGameState)
  {
    case  ePlayer:
      m_eGameState = eDisabledBtns;
      SetNPlayers(1);
      PostMsg(eMSG_SETUP2);
      break;
    case eLevel:
      m_eGameState = eDisabledBtns;
      SetLevel(1);
      PostMsg(eMSG_READY);
      break;
    case  eIdle:
      m_eGameState = eDisabledBtns;
//      BlankButton(1);
      m_arrPlayers[m_nCurrentPlayer].Move();
      break;
    case eBazaar:
      m_eGameState = eDisabledBtns;
      m_arrPlayers[m_nCurrentPlayer].BazaarItemBack();
      PostMsg(eMSG_BAZAAR);
      break;
  
    default:

      SERIALLOG_LN("OneBtn Invalid ");
      break;
  }
}
void TwoBtn()
{
  switch(m_eGameState)
  {
    case  ePlayer:
      m_eGameState = eDisabledBtns;
      SetNPlayers(2);
      PostMsg(eMSG_SETUP2);
      break;
    case eLevel:
      m_eGameState = eDisabledBtns;
      SetLevel(2);
      PostMsg(eMSG_READY);
      break;
    case eBazaar:
//      m_eGameState = eDisabledBtns;
      m_arrPlayers[m_nCurrentPlayer].Haggle();
      break;
    case eReadyPlayer:
      m_eGameState = eDisabledBtns;
      m_arrPlayers[m_nCurrentPlayer].ConsumeFood();
      PostMsg(eMSG_IDLE);
      break;
   default:
     SERIALLOG_LN("TwoBtn Invalid ");
     break; 
  }
  
}
void ThreeBtn()
{
  switch(m_eGameState)
  {
    case  ePlayer:
      m_eGameState = eDisabledBtns;
      SetNPlayers(3);
      PostMsg(eMSG_SETUP2);
      break;
    case eLevel:
      m_eGameState = eDisabledBtns;
      SetLevel(3);
      PostMsg(eMSG_READY);
      break;
    case eBazaar:
      m_eGameState = eBuy;
      PostMsg(eMSG_INITBUY);
      break;
    case eBuy:
      PostMsg(eMSG_BUY);
      break;  
    default:
      SERIALLOG_LN("ThreeBtn Invalid ");
      break;
  }
  
}
void FourBtn()
{
  switch(m_eGameState)
  {
    case  ePlayer:
      m_eGameState = eDisabledBtns;
      SetNPlayers(4);
      PostMsg(eMSG_SETUP2);
      break;
    case eLevel:
      m_eGameState = eDisabledBtns;
      SetLevel(4);
      PostMsg(eMSG_READY);
      break;
    case eEndTurn:
      m_eGameState = eDisabledBtns;
      m_nTurn++;
      m_nCurrentPlayer = /*1 + */(m_nTurn % m_nPlayers); 
      SERIALLOGN(m_nTurn, DEC);
      SERIALLOG(" - ");
      SERIALLOGN(m_nPlayers,DEC);
      SERIALLOG(" - ");
      SERIALLOG_LNN(m_nCurrentPlayer, DEC);    
      PostMsg(eMSG_READY);
      break;
    case eBazaar:
      m_eGameState = eDisabledBtns;
      m_arrPlayers[m_nCurrentPlayer].BazaarItemNext();
      PostMsg(eMSG_BAZAAR);
      break;
    case eBuy:
      PostMsg(eMSG_ENDTURN);
      break;
    default:
      SERIALLOG_LN("FourBtn Invalid ");
      break;
  }

}

void TombBtn()
{

  int nEvent = random(1,100);

//  ItemAward();
  if(nEvent < TOMB_TREASURE)
  {
    epd_disp_string("Treasure room", 500, 85);

    m_arrPlayers[m_nCurrentPlayer].Tomb(true);
    SERIALLOG_LN("Treasure only"); 
    
  }
  else if(nEvent < TOMB_BATTLE)
  {
    SERIALLOG_LN("Battle"); 
    m_eGameState = eBattle;
    DisplayRetreatButton();
    m_arrPlayers[m_nCurrentPlayer].Tomb(false);
    return;
  }
  else 
  {
      epd_disp_string("Empty room", 600, 85);
  }

  PostMsg(eMSG_ENDTURN);

}
void MoveBtn(){m_arrPlayers[m_nCurrentPlayer].Move();}
void Idle()     {m_eGameState = eIdle;  m_arrPlayers[m_nCurrentPlayer].DisplayPlayer(m_eGameState); }
void EndTurn()  {m_eGameState = eEndTurn;   m_arrPlayers[m_nCurrentPlayer].DisplayPlayer(m_eGameState); }
void BazaarItem(){m_eGameState = eBazaar; m_arrPlayers[m_nCurrentPlayer].BazaarItem();}
void BazaarInitBuy(){m_arrPlayers[m_nCurrentPlayer].BazaarInitBuy();}
void BazaarBuy(){m_arrPlayers[m_nCurrentPlayer].BazaarBuy();}
void BattleMsg()
{
  if((millis()-g_nLastBattleTurn)<5000)
  {
    delay(50);
    PostMsg(eMSG_BATTLE);
    return;
  }
  SERIALLOGN(g_nLastBattleTurn, DEC);
  SERIALLOG_LN(" - Time of Last Battle Round");

//  m_nLastBattleTurn = millis();
  m_arrPlayers[m_nCurrentPlayer].BattleRound();
};
void DarkTwrBtn(){};

//    void HaggleBtn(){}
void BazaarBtn()
{
  switch(m_eGameState)
  {
    case  eIdle:
      m_eGameState = eBazaar;
      m_arrPlayers[m_nCurrentPlayer].InitBazaar();
      break;
    default:
      break;
  }
}

void CitadelBtn()
{
  switch(m_eGameState)
  {
    case  eIdle:
      m_eGameState = eDisabledBtns;
      m_arrPlayers[m_nCurrentPlayer].Citadel();
      break;
    default:
      break;
  }

}

void FrontierBtn()
{
  switch(m_eGameState)
  {
    case  eIdle:
      m_eGameState = eDisabledBtns;
      m_arrPlayers[m_nCurrentPlayer].Frontier();
      break;
    default:
      break;
  }
};

void AbortBtn()
{ 
  switch(m_eGameState)
  {
    case eLevel:
      SetNPlayers(1);
      PostMsg(eMSG_SETUP1);
      break;
    case eReadyPlayer:
      PostMsg(eMSG_SETUP2);
      SetLevel(0);
      break;
    case eBazaar:
      m_eGameState = eIdle;
      PostMsg(eMSG_IDLE);
      break;
    default:
      SERIALLOG_LN("AbortBtn Invalid ");
      break;
  }
};


void DisplaySetup1()
{
  SERIALLOG_LN("N Players?");

  m_eGameState = ePlayer;
  epd_clear();
  DisplayDarkTower();
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  // put your setup code here, to run once:
  epd_disp_string("Select Number of Players", 25, 85);

  DisplayDynBtnBoxes();
  DisplayNumButtons();
  
  epd_update();
  
}

void DisplaySetup2()
{
  SERIALLOG_LN("Level?")
  m_eGameState = eLevel;
  
  epd_clear();
  DisplayDarkTower();
  epd_set_en_font(ASCII48); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Players", 75, 85);
  
  DisplayDynBtnBoxes();
  DisplayNumButtons();

  char strTmp[5];
  String((int )m_nPlayers).toCharArray(strTmp,4);
  epd_disp_string(strTmp, 25, 85);

  epd_disp_string("Select Level", 25, 170);
  epd_update();
}

void ReadyPlayer()
{
  SERIALLOG_LN("Ready Player?");
  m_eGameState = eReadyPlayer;

  epd_clear();
  DisplayDarkTower();

  epd_set_en_font(ASCII64); //ASCII32, ASCII48,  ASCII64
  epd_disp_string("Ready Player",215, 80);

  char strTmp[5];
  String((int )m_nCurrentPlayer+1).toCharArray(strTmp,4);
  epd_disp_string(strTmp, 580, 80);

  DisplayDynBtnBoxes();
  DisplayYesButton();

  epd_update();
}  


void WizardCurse(int nPlayer)
{
  // give immediate bonus if 1 player
  if(m_nPlayers == 1)
  {
    int nWarriors, nGold; 
    m_arrPlayers[m_nCurrentPlayer].QuarterDamage(nWarriors, nGold, false);
    m_arrPlayers[m_nCurrentPlayer].AddLoot(nWarriors*2, nGold*2);
    return;
  }
  DisplayNumButtons();
  //Hide Button for current player
  BlankButton(nPlayer);
  //Hide buttons for non-existance players
  for(int nIdx = m_nPlayers; nIdx <=4; nIdx++)
  {
    BlankButton(nIdx);
  }
}
