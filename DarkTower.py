import time
from .dt_constants import *
#import board
#import busio
#import adafruit_ads1x15.ads1115 as ADS
#from adafruit_ads1x15.analog_in import AnalogIn
import queue


class darkTower:
    m_nPlayers = 1
    m_nLevel = 1
    m_lstPlayers =[]

    q = queue.Queue()
    m_nState = Queue_Msgs.eInitialize

#i2c = busio.I2C(board.SCL, board.SDA)

#ads = ADS.ADS1115(i2c)
#chan = AnalogIn(ads, ADS.P0)
    def DecodeBtnPress(level):
        if (level > BTN12):
            m_nState = Queue_Msgs.eFourBtn
        elif(level > BTN11):
            #TODO split ruin off    buttonIdx = eBTN_HAGGLE
            n=0
        elif(level > BTN10):
            m_nState = Queue_Msgs.eAbortBtn
        elif(level > BTN09):
            m_nState = Queue_Msgs.eThreeBtn
        elif(level > BTN08):
            m_nState = Queue_Msgs.eTombBtn
        elif(level > BTN07):
            m_nState = Queue_Msgs.eDarkTwrBtn
        elif(level > BTN06):
            m_nState = Queue_Msgs.eTwoBtn
        elif(level > BTN05):
            m_nState = Queue_Msgs.eMoveBtn
        elif(level > BTN04):
            m_nState = Queue_Msgs.eCitadelBtn
        elif(level > BTN03):
            m_nState = Queue_Msgs.eOneBtn
        elif(level > BTN02):
            m_nState = Queue_Msgs.eBazaarBtn
        elif(level > BTN01):
            m_nState = Queue_Msgs.eFrontierBtn
        else:
            #buttonIdx = 0;
            print("no button")

g_nLastButton = Queue_Msgs.eNoButton
nBtnTime = 0

while True:
    currentButton = 0 #chan.value
    if(currentButton > 1000):
        if (currentButton != g_nLastButton & int(round(time.time() * 1000)) - nBtnTime > DEBOUNCE):
            nBtnTime = int(round(time.time() * 1000))
            g_nLastButton = currentButton
            #PostMsg(currentButton);
            print("{:5}".format(currentButton))
    time.sleep(0.5)
