import serial
import RPi.GPIO as GPIO
from time import sleep

ser = serial.Serial('/dev/serial0', 115200, timeout=1)

#e-Ink DIO lines
RESET_LINE = 17
WAKE_LINE  = 4

#FRAMES
FRAME_B  = 0xA5
FRAME_E0 = 0xCC
FRAME_E1 = 0x33
FRAME_E2 = 0xC3
FRAME_E3 = 0x3C

#MEMORY MODE define
MEMORY_FLASH    =                   0x00
MEMORY_MICROSD  =                   0x01

#Screen Rotation define
DISPLAY_0       =                   0x00
DISPLAY_90      =                   0x01
DISPLAY_180     =                   0x02
DISPLAY_270     =                   0x03
DISPLAY_NORMAL  =                   DISPLAY_0

#Color define
WHITE          =                    0x03
L_GRAY         =                    0x02
D_GRAY         =                    0x01
BLACK          =                    0x00

#EN_FONT define
ASCII32        =                    0x01
ASCII48        =                    0x02
ASCII64        =                    0x03


#Commands
CMD_HANDSHAKE           =           0x00                                                     #handshake
CMD_SET_BAUD            =           0x01                                                     #set baud
CMD_READ_BAUD           =           0x02                                                     #read baud
CMD_READ_MEMORYMODE     =           0x06
CMD_MEMORYMODE          =           0x07                                                     #set memory mode
CMD_STOPMODE            =           0x08                                                     #enter stop mode 
CMD_UPDATE              =           0x0A                                                     #update
CMD_READ_SCREEN_ROT     =           0x0C
CMD_SCREEN_ROTATION     =           0x0D                                                     #set screen rotation
CMD_LOAD_FONT           =           0x0E                                                     ##load font
CMD_LOAD_PIC            =           0x0F                                                     #load picture

CMD_SET_COLOR           =           0x10                                                     #set color
CMD_SET_EN_FONT         =           0x1E                                                     #set english font
#CMD_SET_CH_FONT         =           0x1F                                                     #set chinese font

#CMD_DRAW_PIXEL          =           0x20                                                     #set pixel
CMD_DRAW_LINE           =           0x22                                                     #draw line
CMD_FILL_RECT           =           0x24                                                     #fill rectangle
CMD_DRAW_RECT           =           0x25                                                     #draw Rectangle
CMD_DRAW_CIRCLE         =           0x26                                                     #draw circle
CMD_FILL_CIRCLE         =           0x27                                                     #fill circle
#CMD_DRAW_TRIANGLE       =           0x28                                                     #draw triangle
#CMD_FILL_TRIANGLE       =           0x29                                                     #fill triangle
#CMD_CLEAR               =           0x2E                                                     #clear screen use back color

#CMD_DRAW_STRING         =           0x30                                                     #draw string

#CMD_DRAW_BITMAP         =           0x70 
##CMD_INPUT_STREAM       =           0x40


#*******************************************************************************
#* Function Name  : void epd_init(void)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_init(bWake, bReset):
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(WAKE_LINE, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.setup(RESET_LINE, GPIO.OUT, initial=GPIO.LOW)

    if(bWake):
        epd_wakeup()
    if(bReset):
        epd_reset()

#/*******************************************************************************
#* Function Name  : static unsigned char _verify(const void * ptr, int n)
#* Description    : calculated the checksum
#* Input          : ptr      
#                   n        
#* Output         : None
#* Return         : 
#*******************************************************************************/
def _verify(data, nlen):
    result =0	
    for i in range(0,nlen):
        result ^= data[i]
	
    return result

def _fixed_cmd(_cmd):
    _cmd_template  = bytearray([FRAME_B, 0x00, 0x09, 0xFF, FRAME_E0, FRAME_E1, FRAME_E2, FRAME_E3, 0xFF]) 

    _cmd_template[3] = _cmd
    _cmd_template[8] = _verify(_cmd_template, 8)
    ser.write(_cmd_template)
    retVal = ser.readline()
    return retVal
#    print(ser.readline())



#*******************************************************************************
#* Function Name  : void epd_wakeup(void)
#* Description    : exit Sleep
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/

def epd_wakeup():
    print("e-Ink Wakeup")
    GPIO.output(WAKE_LINE, GPIO.LOW)
    sleep(0.010)
    GPIO.output(WAKE_LINE, GPIO.HIGH)
    sleep(0.500)
    GPIO.output(WAKE_LINE, GPIO.LOW)
    sleep(10)

#*******************************************************************************
#* Function Name  : void epd_enter_sleep(void)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_enter_sleep():
    _fixed_cmd(CMD_STOPMODE)


#*******************************************************************************
#* Function Name  : void epd_reset(void)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/

def epd_reset():
    print("e-Ink Reset")
    GPIO.output(RESET_LINE, GPIO.LOW)
    sleep(0.010)
    GPIO.output(RESET_LINE, GPIO.HIGH)
    sleep(0.500)
    GPIO.output(RESET_LINE, GPIO.LOW)
    sleep(3)

#*******************************************************************************
#* Function Name  : void epd_handshake(void)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_handshake():
    _fixed_cmd(CMD_HANDSHAKE)
  
#*******************************************************************************
#* Function Name  : void epd_read_baud(void)
#* Description    : read uart baud
#* Input          : 
#* Output         : "9600", "115200", ....
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_read_baud():
    retVal = _fixed_cmd(CMD_READ_BAUD)
    print(retVal)
    return retVal

#*******************************************************************************
#* Function Name  : void epd_read_memory(void)
#* Description    : read memory mode 
#* Input          : 
#* Output         : MEMORY_FLASH or MEMORY_MICROSD
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_read_memory():
    retVal = _fixed_cmd(CMD_READ_MEMORYMODE)
    print(retVal)
    return retVal

#*******************************************************************************
#* Function Name  : void epd_set_memory(unsigned char mode)
#* Description    : 
#* Input          : MEMORY_FLASH or MEMORY_MICROSD
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_set_memory(mode):
    _cmd_buff = [0]*10

    _cmd_buff[0] = FRAME_B
	
    _cmd_buff[1] = 0x00
    _cmd_buff[2] = 0x0A	
	
    _cmd_buff[3] = CMD_MEMORYMODE
	
    _cmd_buff[4] = mode
	
    _cmd_buff[5] = FRAME_E0
    _cmd_buff[6] = FRAME_E1
    _cmd_buff[7] = FRAME_E2
    _cmd_buff[8] = FRAME_E3	
    _cmd_buff[9] = _verify(_cmd_buff, 9)
	
    ser.write(_cmd_buff)
#    print(''.join('{:02x}'.format(x) for x in _cmd_buff))
    print(ser.readline())

#*******************************************************************************
#* Function Name  : void epd_load_font(void)
#* Description    : import font library: 48MB
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_load_font():
    _fixed_cmd(CMD_LOAD_FONT)

#*******************************************************************************
#* Function Name  : void epd_load_pic(void)
#* Description    : import image library: 80MB
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention	  : None
#*******************************************************************************/
def epd_load_pic():
    _fixed_cmd(CMD_LOAD_PIC)


#*******************************************************************************
#* Function Name  : void epd_update(void)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_update():
    _fixed_cmd(CMD_UPDATE)

#*******************************************************************************
#* Function Name  : void epd_read_screen_rotation(void)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : DISPLAY_0, DISPLAY_90, DISPLAY_180,
#*                  DISPLAY_270, DISPLAY_NORMAL
#* Attention	  : None
#*******************************************************************************/
def epd_read_screen_rotation():
    retVal = _fixed_cmd(CMD_READ_SCREEN_ROT)
    print(retVal)
    return retVal

    
#*******************************************************************************
#* Function Name  : void epd_screen_rotation(unsigned char mode)
#* Description    : 
#* Input          : DISPLAY_0, DISPLAY_90, DISPLAY_180,
#*                  DISPLAY_270, DISPLAY_NORMAL
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_screen_rotation(mode):
    _cmd_buff = [0]*10

    _cmd_buff[0] = FRAME_B
	
    _cmd_buff[1] = 0x00
    _cmd_buff[2] = 0x0A	
	
    _cmd_buff[3] = CMD_SCREEN_ROTATION;
	
    _cmd_buff[4] = mode
	
    _cmd_buff[5] = FRAME_E0
    _cmd_buff[6] = FRAME_E1
    _cmd_buff[7] = FRAME_E2
    _cmd_buff[8] = FRAME_E3	
    _cmd_buff[9] = _verify(_cmd_buff, 9)
	
    ser.write(_cmd_buff)
#    print(''.join('{:02x}'.format(x) for x in _cmd_buff))
    print(ser.readline())
        
#/*******************************************************************************
#* Function Name  : void epd_set_color(unsigned char color, unsigned char bkcolor)
#* Description    : 
#* Input          : WHITE, L_GRAY, D_GRAY, BLACK
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_set_color(color, bkcolor):

    _cmd_buff = [0]*11
    _cmd_buff[0] = FRAME_B
	
    _cmd_buff[1] = 0x00
    _cmd_buff[2] = 0x0B
	
    _cmd_buff[3] = CMD_SET_COLOR
	
    _cmd_buff[4] = color
    _cmd_buff[5] = bkcolor
	
    _cmd_buff[6] = FRAME_E0
    _cmd_buff[7] = FRAME_E1
    _cmd_buff[8] = FRAME_E2
    _cmd_buff[9] = FRAME_E3
    _cmd_buff[10] = _verify(_cmd_buff, 10)
	
    ser.write(_cmd_buff)
#    print(''.join('{:02x}'.format(x) for x in _cmd_buff))
    print(ser.readline())

#*******************************************************************************
#* Function Name  : void epd_set_en_font(unsigned char font)
#* Description    : 
#* Input          : ASCII32, ASCII48, ASCII64
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_set_en_font( font):
    _cmd_buff[0] = FRAME_B
	
    _cmd_buff[1] = 0x00
    _cmd_buff[2] = 0x0A	
	
    _cmd_buff[3] = CMD_SET_EN_FONT
	
    _cmd_buff[4] = font
	
    _cmd_buff[5] = FRAME_E0
    _cmd_buff[6] = FRAME_E1
    _cmd_buff[7] = FRAME_E2
    _cmd_buff[8] = FRAME_E3
    _cmd_buff[9] = _verify(_cmd_buff, 9)
	
    ser.write(_cmd_buff)
#    print(''.join('{:02x}'.format(x) for x in _cmd_buff))
    print(ser.readline())

#*******************************************************************************
#* Function Name  : void epd_draw_line(int x0, int y0, int x1, int y1)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_draw_line( x0,  y0,  x1,  y1):
    _cmd_buff = [0]*17
    _cmd_buff[0] = FRAME_B;
	
    _cmd_buff[1] = 0x00;
    _cmd_buff[2] = 0x11;	
	
    _cmd_buff[3] = CMD_DRAW_LINE;	
	
    _cmd_buff[4] = (x0 >> 8) & 0xFF;
    _cmd_buff[5] = x0 & 0xFF;
    _cmd_buff[6] = (y0 >> 8) & 0xFF;
    _cmd_buff[7] = y0 & 0xFF;
    _cmd_buff[8] = (x1 >> 8) & 0xFF;
    _cmd_buff[9] = x1 & 0xFF;
    _cmd_buff[10] = (y1 >> 8) & 0xFF;
    _cmd_buff[11] = y1 & 0xFF;	
	
    _cmd_buff[12] = FRAME_E0;
    _cmd_buff[13] = FRAME_E1;
    _cmd_buff[14] = FRAME_E2;
    _cmd_buff[15] = FRAME_E3;	
    _cmd_buff[16] = _verify(_cmd_buff, 16);
	
    ser.write(_cmd_buff)
#    print(''.join('{:02x}'.format(x) for x in _cmd_buff))
    print(ser.readline())


#*******************************************************************************
#* Function Name  : void epd_fill_rect(int x0, int y0, int x1, int y1)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_draw_rect( x0,  y0,  x1,  y1,  bFill):

    _cmd_buff = [0]*17
    _cmd_buff[0] = FRAME_B
    _cmd_buff[1] = 0x00
    _cmd_buff[2] = 0x11	
	
    if(bFill ==False ):
        _cmd_buff[3] = CMD_DRAW_RECT
    else:
        _cmd_buff[3] = CMD_FILL_RECT 
	
    _cmd_buff[4] = (x0 >> 8) & 0xFF;
    _cmd_buff[5] = x0 & 0xFF;
    _cmd_buff[6] = (y0 >> 8) & 0xFF;
    _cmd_buff[7] = y0 & 0xFF;
    _cmd_buff[8] = (x1 >> 8) & 0xFF;
    _cmd_buff[9] = x1 & 0xFF;
    _cmd_buff[10] = (y1 >> 8) & 0xFF;
    _cmd_buff[11] = y1 & 0xFF;	
	
    _cmd_buff[12] = FRAME_E0;
    _cmd_buff[13] = FRAME_E1;
    _cmd_buff[14] = FRAME_E2;
    _cmd_buff[15] = FRAME_E3;	
    _cmd_buff[16] = _verify(_cmd_buff, 16);
	
    ser.write(_cmd_buff)
#    print(''.join('{:02x}'.format(x) for x in _cmd_buff))
    print(ser.readline())

#*******************************************************************************
#* Function Name  : epd_clear_rect(int x0, int y0, int x1, int y1)
#* Description    : clears a portion of the screen.  requires update after
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/

def epd_clear_rect( x0,  y0,  x1,  y1):
#TODO get color reverse it then put it back
#for now hard coded as I am always using black on white
  epd_set_color(WHITE, BLACK )
  epd_draw_rect(x0, y0, x1, y1, true)
  epd_set_color(BLACK, WHITE)
  print(ser.readline())

#*******************************************************************************
#* Function Name  : void epd_draw_circle(int x0, int y0, int r)
#* Description    : 
#* Input          : 
#* Output         : None
#* Return         : 
#* Attention		   : None
#*******************************************************************************/
def epd_draw_circle( x0,  y0,  r,  bFill):
    _cmd_buff = [0]*15

    _cmd_buff[0] = FRAME_B;
	
    _cmd_buff[1] = 0x00;
    _cmd_buff[2] = 0x0F;	

    if(bFill == 0):
        _cmd_buff[3] = CMD_DRAW_CIRCLE;	
    else:
        _cmd_buff[3] = CMD_FILL_CIRCLE; 

    _cmd_buff[4] = (x0 >> 8) & 0xFF;
    _cmd_buff[5] = x0 & 0xFF;
    _cmd_buff[6] = (y0 >> 8) & 0xFF;
    _cmd_buff[7] = y0 & 0xFF;
    _cmd_buff[8] = (r >> 8) & 0xFF;
    _cmd_buff[9] = r & 0xFF;
	
	
    _cmd_buff[10] = FRAME_E0;
    _cmd_buff[11] = FRAME_E1;
    _cmd_buff[12] = FRAME_E2;
    _cmd_buff[13] = FRAME_E3;	
    _cmd_buff[14] = _verify(_cmd_buff, 14);
	
    ser.write(_cmd_buff)
    print(ser.readline())


def main():
    epd_init(False, True)
    epd_read_baud()

    print("init e-Ink done")
    epd_read_screen_rotation()
    epd_screen_rotation(DISPLAY_0)
    epd_set_color(L_GRAY, BLACK) 
    epd_draw_rect(10,20,100,200, True)
    epd_draw_rect(50,50,150,250, False)
    epd_draw_circle(300,300,20, True)
    epd_draw_circle(105,105,10, False)
    epd_handshake()
    epd_draw_line(0,300,300,0)
    epd_draw_line(0,301,301,0)
    epd_update()


    ser.close()
    GPIO.cleanup()
                        
#def _putchars()
main()
