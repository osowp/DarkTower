import serial
import RPi.GPIO as GPIO

_cmd_update = bytes([0xA5, 0x00, 0x09, 0x0A, 0xCC, 0x33, 0xC3, 0x3C])

def main():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(4, GPIO.OUT, initial=GPIO.LOW)
    GPIO.output(4, GPIO.HIGH)
    GPIO.setup(17, GPIO.OUT, initial=GPIO.LOW)
    GPIO.output(17, GPIO.LOW)

    print("init e-Ink done")

    ser = serial.Serial('/dev/serial0', 115200, timeout=1)
    ser.open()
    
    ser.write(_cmd_update)
                        
#def _putchars()
main()
