#include <Arduino.h>
#include "epd.h"

#include "darkTower.h"
#include "MsgHandlers.h"

#define DEBOUNCE 500

#define BTN01   474
#define BTN02   498
#define BTN03   524
#define BTN04   552
#define BTN05   584
#define BTN06   620
#define BTN07   660
#define BTN08   706
#define BTN09   759
#define BTN10   820
#define BTN11   892
#define BTN12   977

Queue_Msgs g_nLastButton=0;
int currentAction = 0;
//darkTower g_game;
Queue  g_qMsg(sizeof(Queue_Msgs), 10, FIFO); // Instantiate queue


void (*msgHandlers[eHandlerCount])() = 
  { //order must match Queue_Msgs in MsgHandlers.h
     &OneBtn        //0
    ,&TwoBtn        //1
    ,&ThreeBtn      //2
    ,&FourBtn       //3
//    ,&HaggleBtn     //4
    ,&BazaarBtn     //4
    ,&TombBtn       //5
    ,&MoveBtn       //6
    ,&CitadelBtn    //7
    ,&DarkTwrBtn    //8
    ,&FrontierBtn   //9
    ,&AbortBtn      //10
    ,&DisplaySetup1 //11
    ,&DisplaySetup2 //12
    ,&ReadyPlayer   //13
    ,&Idle          //14
    ,&EndTurn       //15
    ,&BazaarItem    //16
    ,&BazaarInitBuy //17
    ,&BazaarBuy     //18
    ,&BattleMsg     //19
   };

//**************************************

void PostMsg(Queue_Msgs nMsg)
{ 
  g_qMsg.push(&nMsg);
}

void HandleMsg()
{ 
  Queue_Msgs nMsg;
  if(!g_qMsg.isEmpty())
  {
    g_qMsg.pop(&nMsg);
    msgHandlers[nMsg]();
    if(nMsg != 19)
    {
      SERIALLOG("HandleMsg - ");
      SERIALLOG_LNN(nMsg, DEC);
    }
  }
}

Queue_Msgs decodeAnalog(int level)
{
  int buttonIdx =0;
  if(level > BTN12)
  {
    buttonIdx =  eBTN_FOUR; 
  }
  else if(level > BTN11)
  {
//TODO split ruin off    buttonIdx = eBTN_HAGGLE ; 
  }
  else if(level > BTN10)
  {
    buttonIdx = eBTN_ABORT ; 
  }
  else if(level > BTN09)
  {
    buttonIdx = eBTN_THREE ; 
  }
  else if(level > BTN08)
  {
    buttonIdx = eBTN_TOMB; 
  }
  else if(level > BTN07)
  {
    buttonIdx = eBTN_DARKTWR ; 
  }
  else if(level > BTN06)
  {
    buttonIdx = eBTN_TWO ; 
  }
  else if(level > BTN05)
  {
    buttonIdx = eBTN_MOVE; 
  }
  else if(level > BTN04)
  {
    buttonIdx = eBTN_CITADEL;
  }
  else if(level > BTN03)
  {
    buttonIdx = eBTN_ONE ;
  }
  else if(level > BTN02)
  {
    buttonIdx = eBTN_BAZAAR ;
  }
  else if(level > BTN01)
  {
    buttonIdx = eBTN_FRONTR ;
  }
  else
  {
    buttonIdx = 0;
  }

  return buttonIdx;

}

//**********************************************************
void setup() {

#ifdef DISABLE_EPD
  Serial.begin(115200);
#else
/**/
  epd_init();
  epd_wakeup();
  epd_set_memory(MEM_TF);
  epd_set_color(BLACK, WHITE);
/**/
#endif

  SERIALLOG_LN("DISABLE_EPD");

  int seed = analogRead(1) +analogRead(2)+analogRead(3)+ analogRead(4)+analogRead(5);
  randomSeed(seed);
  /*g_game.*/Init();
  PostMsg(eMSG_SETUP1);
}


void loop() {
  // put your main code here, to run repeatedly:
  int analogLevel=0;
  Queue_Msgs currentButton=0;
  static unsigned long nBtnTime =0;
  
  analogLevel = analogRead(0);/*returns (0 to 1023)*/

  currentButton = decodeAnalog(analogLevel);

  if(analogLevel >0)
  {    
    if(currentButton != g_nLastButton && millis() - nBtnTime > DEBOUNCE)
    {
      nBtnTime = millis();
      int seed = (analogRead(1) +analogRead(5))* nBtnTime + analogRead(2) -analogRead(4);
      randomSeed(seed);
      
      g_nLastButton = currentButton;
      PostMsg(currentButton);
    }
  }
  else
  {
    g_nLastButton = 511;
  }

  HandleMsg();

  delay(50);
}

// **********************************************************
//  Message Handlers
/*
void OneBtn()
{
//  MYERROR
  g_game.OneBtn();
}

void TwoBtn(){g_game.TwoBtn();}
void ThreeBtn(){g_game.ThreeBtn();}
void FourBtn(){g_game.FourBtn();}
void HaggleBtn(){}
void BazaarBtn(){g_game.BazaarBtn();}
void BazaarItem(){g_game.BazaarItem();}
void BazaarInitBuy(){g_game.BazaarInitBuy();}
void BazaarBuy(){g_game.BazaarBuy();}
void TombBtn(){g_game.TombBtn();}
void MoveBtn(){g_game.MoveBtn();}
void CitadelBtn(){g_game.CitadelBtn();}
void DarkTwrBtn(){}
void FrontierBtn(){g_game.FrontierBtn();}
void AbortBtn(){g_game.AbortBtn();}
void DisplaySetup1(){g_game.DisplaySetup1();}
void DisplaySetup2(){g_game.DisplaySetup2();}
void ReadyPlayer(){g_game.ReadyPlayer();}
void Idle(){g_game.Idle();}
void EndTurn(){g_game.EndTurn();}
void BattleMsg(){}
*/
