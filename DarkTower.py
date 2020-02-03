import logging
import time
from dt_constants import *
from epd import *
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
import queue



class darkTower:
    m_nPlayers = 1
    m_nLevel = 1
    m_lstPlayers =[]

    q = queue.Queue()
    m_eGameState = Game_States.ePlayer
    def DisplayDarkTower():
        epd_set_en_font(ASCII64)
        epd_disp_string("Dark Tower", 250,0)
        epd_draw_line(BAR_X_START, 75,800,75)
        epd_draw_line(BAR_X_START, 76,800,76)

    def DisplayDynBtnBoxes():
        epd_draw_line(BAR_X_START, Y_BTN_BOX, BAR_X_START, 600)
        epd_draw_line(200, Y_BTN_BOX, 200, 600)
        epd_draw_line(400, Y_BTN_BOX, 400, 600)
        epd_draw_line(600, Y_BTN_BOX, 600, 600)
        epd_draw_line(800, Y_BTN_BOX, 800, 600)
        epd_draw_line(BAR_X_START, Y_BTN_BOX, 800, Y_BTN_BOX)


    def DisplayNumButtons():
        epd_set_en_font(ASCII48)

        epd_disp_string("1", 100, Y_BTN_TXT)
        epd_disp_string("2", 300, Y_BTN_TXT)
        epd_disp_string("3", 500, Y_BTN_TXT)
        epd_disp_string("4", 700, Y_BTN_TXT)

    def DisplaySetup1(self):
        logging.debug("DisplaySetup1")
        self.m_eGameState = Game_States.ePlayer
        epd_clear()
        self.DisplayDarkTower()
        epd_set_en_font(ASCII48)
        epd_disp_string("Select Number of Players", 25,85)

        self.DisplayDynBtnBoxes()
        self.DisplayNumButtons()
        
        epd_update()

    def DisplaySetup2(self):
        logging.debug("DisplaySetup2")

    def ReadyPlayer(self):
        logging.debug("ReadyPlayer")


    def OneBtn(self):
        logging.debug("OneBtn")
        if self.m_eGameState==Game_States.ePlayer:
            logging.info("1 Player")
            self.m_nPlayers = 1
            self.q.put(Queue_Msgs.eDisplaySetup2)
       
        elif self.m_eGameState==Game_States.eLevel:
            logging.info("Level 1")
            self.m_nLevel = 1
            self.q.put(Queue_Msgs.eReadyPlayer)
            
    def TwoBtn(self):
        logging.debug("TwoBtn")
        if self.m_eGameState==Game_States.ePlayer:
            logging.info("2 Players")
            self.m_nPlayers = 2
            self.q.put(Queue_Msgs.eDisplaySetup2)
       
        elif self.m_eGameState==Game_States.eLevel:
            logging.info("Level 2")
            self.m_nLevel = 2
            self.q.put(Queue_Msgs.eReadyPlayer)
            
    def ThreeBtn(self):
        logging.debug("Button Three")
        if self.m_eGameState==Game_States.ePlayer:
            logging.info("3 Players")
            self.m_nPlayers = 3
            self.q.put(Queue_Msgs.eDisplaySetup2)
       
        elif self.m_eGameState==Game_States.eLevel:
            logging.info("Level 3")
            self.m_nLevel = 3
            self.q.put(Queue_Msgs.eReadyPlayer)

    def FourBtn(self):
        logging.debug("Button Four")
        if self.m_eGameState==Game_States.ePlayer:
            logging.info("4 Players")
            self.m_nPlayers = 4
            self.q.put(Queue_Msgs.eDisplaySetup2)
       
        elif self.m_eGameState==Game_States.eLevel:
            logging.info("Level 4")
            self.m_nLevel = 4
            self.q.put(Queue_Msgs.eReadyPlayer)
        

    funcdict = {
        Queue_Msgs.eDisplaySetup1: DisplaySetup1,
        Queue_Msgs.eDisplaySetup2: DisplaySetup2,
        Queue_Msgs.eReadyPlayer: ReadyPlayer,

        Queue_Msgs.eOneBtn: OneBtn,
        Queue_Msgs.eTwoBtn: TwoBtn,
        Queue_Msgs.eThreeBtn: ThreeBtn,
        Queue_Msgs.eFourBtn: FourBtn,
        #Queue_Msgs.eBazaarBtn:
        #Queue_Msgs.eTombBtn:
        #Queue_Msgs.eMoveBtn:
        #Queue_Msgs.eCitadelBtn:
        #Queue_Msgs.eDarkTwrBtn:
        #Queue_Msgs.eFrontierBtn:
        #Queue_Msgs.eAbortBtn:
        #Queue_Msgs.eIdle:
        #Queue_Msgs.eEndTurn:
        #Queue_Msgs.eBazaarItem:
        #Queue_Msgs.eBazaarInitBuy:
        #Queue_Msgs.eBazaarBuy:
        #Queue_Msgs.eBattleMsg:
        #Queue_Msgs.eHandlerCount:

    }

    def HandleMsg(self):
        if not self.q.empty():
            self.funcdict[self.q._get()](self)


def DecodeBtnPress(level):
    if (level > BTN12):
        return Queue_Msgs.eFourBtn
    elif(level > BTN11):
        #TODO split ruin off    buttonIdx = eBTN_HAGGLE
        n=0
    elif(level > BTN10):
        return Queue_Msgs.eAbortBtn
    elif(level > BTN09):
        return Queue_Msgs.eThreeBtn
    elif(level > BTN08):
        return Queue_Msgs.eTombBtn
    elif(level > BTN07):
        return Queue_Msgs.eDarkTwrBtn
    elif(level > BTN06):
        return Queue_Msgs.eTwoBtn
    elif(level > BTN05):
        return Queue_Msgs.eMoveBtn
    elif(level > BTN04):
        return Queue_Msgs.eCitadelBtn
    elif(level > BTN03):
        return Queue_Msgs.eOneBtn
    elif(level > BTN02):
        return Queue_Msgs.eBazaarBtn
    elif(level > BTN01):
        return Queue_Msgs.eFrontierBtn
    else:
        #buttonIdx = 0;
        print("no button")

logging.basicConfig(filename="DarkTower.log", level=logging.DEBUG)
logging.critical("************************* Start Dark Tower *************************")
i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
chan = AnalogIn(ads, ADS.P0)

g_nLastButton = Queue_Msgs.eNoButton
nBtnTime = 0
game = darkTower
game.q.put(Queue_Msgs.eDisplaySetup1)

while True:
    currentButton = chan.value
    if(currentButton > BTN01):
        currentButton = DecodeBtnPress(currentButton)
        if ((currentButton != g_nLastButton) & ((int(round(time.time() * 1000)) - nBtnTime) > DEBOUNCE)):
            nBtnTime = int(round(time.time() * 1000))
            g_nLastButton = currentButton
            #PostMsg(currentButton);
            game.q.put(currentButton)
            logging.info("{:5}".format(currentButton))
    else:
        g_nLastButton = Queue_Msgs.eNoButton
    

    game.HandleMsg(game)
    time.sleep(0.5)
