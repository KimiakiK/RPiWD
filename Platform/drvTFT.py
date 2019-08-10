import time
import RPi.GPIO as GPIO
import spidev
import numpy as np

class TFT:
  
  def __init__(self):
    # TFTサイズ
    self.width = 240
    self.height = 240
    # TFT端子
    self.pin_rst = 4
    self.pin_blk = 17
    self.pin_dc1 = 27
    self.pin_dc2 = 22
    self.spi = spidev.SpiDev(0, 0)
    self.spi.max_speed_hz = 80000000
    self.spi.mode = 0b11 # clock polarity and phase [CPOL|CPHA]
  
  RST_OFF = GPIO.HIGH
  RST_ON  = GPIO.LOW
  BLK_OFF = GPIO.HIGH
  BLK_ON  = GPIO.LOW
  DC_CMD  = GPIO.LOW
  DC_DATA = GPIO.HIGH
  
  def initGPIO(self):
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(self.pin_rst, GPIO.OUT)
    GPIO.output(self.pin_rst, self.RST_OFF)
    GPIO.setup(self.pin_blk, GPIO.OUT)
    GPIO.output(self.pin_blk, self.BLK_ON)
    GPIO.setup(self.pin_dc1, GPIO.OUT)
    GPIO.output(self.pin_dc1, self.DC_DATA)
    GPIO.setup(self.pin_dc2, GPIO.OUT)
    GPIO.output(self.pin_dc2, self.DC_DATA)
  
  CMD_NOP     = 0x00
  CMD_SWRESET = 0x01
  CMD_SLPOUT  = 0x11
  CMD_NORON   = 0x13
  CMD_INVON   = 0x21
  CMD_DISPON  = 0x29
  CMD_CASET   = 0x2A
  CMD_RASET   = 0x2B
  CMD_RAMWR   = 0x2C
  CMD_MADCTL  = 0x36
  CMD_COLMOD  = 0x3A
  
  def initTFT(self):
    # チップリセット
    GPIO.output(self.pin_rst, self.RST_ON)
    time.sleep(0.00001) # リセット実行、少なくとも10us
    GPIO.output(self.pin_rst, self.RST_OFF)
    time.sleep(0.12) # リセット明け120ms待機
    # TFT初期設定
    self.sendCmd(self.CMD_SWRESET, self.BOTH_TFT)
    self.sendCmd(self.CMD_SLPOUT, self.BOTH_TFT)
    time.sleep(0.12) # スリープ明け120ms待機
    self.sendCmd(self.CMD_COLMOD, self.BOTH_TFT)
    self.sendData(0x55) # 65K of RGB interface, 16bit/pixel
    self.sendCmd(self.CMD_MADCTL, self.BOTH_TFT)
    self.sendData(0xC0) # 縦横反転 注:縦方向はメモリ位置がずれる
    self.sendCmd(self.CMD_INVON, self.BOTH_TFT) # 黒:0x0000 白:0xFFFF
    self.sendCmd(self.CMD_NORON, self.BOTH_TFT)
    self.clearDisp()
    self.sendCmd(self.CMD_DISPON, self.BOTH_TFT)
    GPIO.output(self.pin_blk, self.BLK_OFF)
  
  P1_TFT = 1
  P2_TFT = 2
  BOTH_TFT = 3
  
  def sendCmd(self, cmd, tftSelect):
    # DC設定 コマンドの対象にするTFTを選択
    if tftSelect == self.P1_TFT:
      GPIO.output(self.pin_dc1, self.DC_CMD)
      GPIO.output(self.pin_dc2, self.DC_DATA)
    elif tftSelect == self.P2_TFT:
      GPIO.output(self.pin_dc1, self.DC_DATA)
      GPIO.output(self.pin_dc2, self.DC_CMD)
    else:
      GPIO.output(self.pin_dc1, self.DC_CMD)
      GPIO.output(self.pin_dc2, self.DC_CMD)
    # コマンド送信
    self.spi.writebytes([cmd])
  
  def sendData(self, data):
    # DC設定
    GPIO.output(self.pin_dc1, self.DC_DATA)
    GPIO.output(self.pin_dc2, self.DC_DATA)
    # データ送信
    self.spi.writebytes([data])
  
  def sendFrameMemory(self, frameMemory):
    GPIO.output(self.pin_dc1, self.DC_DATA)
    GPIO.output(self.pin_dc2, self.DC_DATA)
    self.spi.writebytes2(frameMemory)
  
  def setDispArea(self, x, y, w, h, tftSelect):
    # 縦横反転の影響で縦方向をずらす
    y += 80
    # 両方のTFTを一旦待ち状態に移行
    self.sendCmd(self.CMD_NOP, self.BOTH_TFT)
    # 横方向の表示位置設定
    self.sendCmd(self.CMD_CASET, tftSelect)
    self.sendData(0x00)
    self.sendData(x)
    self.sendData(0x00)
    self.sendData(x + w - 1)
    # 縦方向の表示位置設定
    self.sendCmd(self.CMD_RASET, tftSelect)
    self.sendData(0x00)
    self.sendData(y)
    self.sendData(0x00)
    self.sendData(y + h - 1)
    # 表示メモリ転送開始
    self.sendCmd(self.CMD_RAMWR, tftSelect)
  
  def clearDisp(self):
    frameMemory = np.zeros((self.width, self.height), dtype = np.uint16)
    self.setDispArea(0, 0, self.width, self.height, self.BOTH_TFT)
    self.sendFrameMemory(frameMemory)
  
  def drawRect(self, x, y, w, h, c, tftSelect):
    self.setDispArea(x, y, w, h, tftSelect)
    frameMemory = np.ones((w, h), dtype = np.uint16) * c
    self.sendFrameMemory(frameMemory)
  
  def drawArray(self, x, y, array, tftSelect):
    self.setDispArea(x, y, array.shape[1], array.shape[0], tftSelect)
    self.sendFrameMemory(array)