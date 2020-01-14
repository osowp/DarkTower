#ifndef    MsgHandlers_H
#define  MsgHandlers_H

#define Y_BTN_TXT 551
#define Y_BTN_BOX 550
#define LOG_Y         509
#define LOG_TEXT_Y    (LOG_Y + 1)

#define LOG_MSG(v) \
{\
  epd_clear_rect(0, LOG_TEXT_Y, 800, Y_BTN_BOX - 1);\
  epd_set_en_font(ASCII32);\
  epd_disp_string(v, 0, LOG_TEXT_Y);\
  epd_update();\
  }
/**/
#ifdef DISABLE_EPD
#define SERIALLOG(v) {Serial.print(v);}
#define SERIALLOGN(v, t) {Serial.print(v, t);}
#define SERIALLOG_LN(v) {Serial.println(v);}
#define SERIALLOG_LNN(v, t) {Serial.println(v,t);}
#define MYERROR {error}
#else
#define SERIALLOG(v) 
#define SERIALLOGN(v, t)
#define SERIALLOG_LN(v) 
#define SERIALLOG_LNN(v, t) 
#define MYERROR 
#endif


enum gamestate
{
  eDisabledBtns,
  ePlayer,
  eLevel,
  eReadyPlayer,
  eIdle,
  eBazaar,
  eBuy, //5
  eBattle,
// turn ending states
  eClosed, //_b
  eCursed, //_c
  eDragon, //_d  
  eMissKey,//_k  10
  eLost,  //_l
  eScout, //-S
  ePlague, //_p
  eHealer, //-H 
  eGold,
  eKey,
  eSword,
  ePegasus,
  eWizard,  
  eVictory, //_v
  eEndTurn //no event end 14
};

//Queue Messages
enum Queue_Msgs
{// order must match array in DarkTower.ino
   eBTN_ONE         //0
  ,eBTN_TWO         //1
  ,eBTN_THREE       //2
  ,eBTN_FOUR        //3
//  ,eBTN_HAGGLE      //4
  ,eBTN_BAZAAR      //4
  ,eBTN_TOMB        //5
  ,eBTN_MOVE        //6
  ,eBTN_CITADEL     //7
  ,eBTN_DARKTWR     //8
  ,eBTN_FRONTR  /*9*/
  ,eBTN_ABORT       //10
  ,eMSG_SETUP1      //11
  ,eMSG_SETUP2      //12
  ,eMSG_READY       //13
  ,eMSG_IDLE        //14
  ,eMSG_ENDTURN     //15
  ,eMSG_BAZAAR      //16
  ,eMSG_INITBUY     //17
  ,eMSG_BUY         //18
  ,eMSG_BATTLE      //19
  ,eHandlerCount  
};

//message Handlers
    void OneBtn();
    void TwoBtn();
    void ThreeBtn();
    void FourBtn();
//    void HaggleBtn();
    void BazaarBtn();
    void TombBtn();
    void MoveBtn();
    void CitadelBtn();
    void DarkTwrBtn();
    void FrontierBtn();
    void AbortBtn();
    void DisplaySetup1();
    void DisplaySetup2();
    void ReadyPlayer();
    void Idle();
    void EndTurn();
    void BazaarItem();
    void BazaarInitBuy();
    void BazaarBuy();
    void BazaarClosed();
    void BattleMsg();

void PostMsg(Queue_Msgs nMsg);


#endif
