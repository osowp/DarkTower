import time
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

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

i2c = busio.I2C(board.SCL, board.SDA)

ads = ADS.ADS1115(i2c)
chan = AnalogIn(ads, ADS.P0)

while True:
    print("{:5}\t{:>5.3f}".format(chan.value, chan.voltage))
    time.sleep(0.5)
