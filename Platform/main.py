import drvTFT
import drvController
import time
import numpy as np
import RPi.GPIO as GPIO
import subprocess

from threading import (Event, Thread)


def imgBlend(base, target, mask, x, y):
  slice = base[y:y+target.shape[0], x:x+target.shape[1]]
  np.multiply(slice, mask, out=slice)
  np.add(slice, target, out=slice)

imgTft1 = np.zeros((240, 240), dtype = np.uint16)
imgTft2 = np.zeros((240, 240), dtype = np.uint16)

PIN_AMP_CE = 23
AMP_CE_ON  = GPIO.LOW
AMP_CE_OFF = GPIO.HIGH
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(PIN_AMP_CE, GPIO.OUT)
GPIO.output(PIN_AMP_CE, AMP_CE_ON)

def taskDraw():
  global imgTft1
  global imgTft2
  
  # TFTドライバ初期化
  start_time = time.time()
  tft = drvTFT.TFT()
  tft.initGPIO()
  tft.initTFT()
  print('Initialize TFT: {0:.3f}ms'.format( (time.time() - start_time) * 1000.0) )
  
  # TFT初期化完了通知
  eventTftInit.set()
  
  while(True):
    # レンダリング完了待ち
    eventRenderFinish.wait()
    eventRenderFinish.clear()
    
    # レンダリング結果をコピー
    imgCopyTft1 = imgTft1.copy()
    imgCopyTft2 = imgTft2.copy()
    # レンダリング結果のコピー完了通知
    eventImgCopyFinish.set()
    
    # TFT転送実行
    start_time = time.time()
    tft.drawArray(0, 0, imgCopyTft1, tft.P1_TFT)
    print('SPI TFT Transmit1: {0:.3f}ms'.format( (time.time() - start_time) * 1000.0) )
    start_time = time.time()
    tft.drawArray(0, 0, imgCopyTft2, tft.P2_TFT)
    print('SPI TFT Transmit2: {0:.3f}ms'.format( (time.time() - start_time) * 1000.0) )
    

def taskRender():
  global imgTft1
  global imgTft2
  
  # コントローラドライバ初期化
  start_time = time.time()
  ctrlRecv = drvController.Receiver()
  print('Initialize Controller: {0:.3f}ms'.format( (time.time() - start_time) * 1000.0) )
  
  # 画像読み込み
  start_time = time.time()
  imgController = np.load('Controller.npy')
  imgPointer = np.load('Pointer.npy')
  imgPointerGray = np.load('Pointer_gray.npy')
  imgMaskPointer = np.load('Pointer_mask.npy')
  print('Loding Images: {0:.3f}ms'.format( (time.time() - start_time) * 1000.0) )
  
  # TFT初期化完了待ち
  print('Event wait: eventTftInit')
  eventTftInit.wait()
  print('Event set : eventTftInit')
  
  # コントローラ入力のバッファクリア
  ctrlRecv.clearBuffer()
  
  while(True):
    # レンダリング実行
    imgTft1 = imgController.copy()
    imgTft2 = imgController.copy()
    
    ctrl = ctrlRecv.receiveMessage()
    if ctrl.p1_a == ctrl.ON:
      imgBlend(imgTft1, imgPointer, imgMaskPointer, 201, 141)
      eventPlaySE.set()
    else:
      imgBlend(imgTft1, imgPointerGray, imgMaskPointer, 201, 141)
    if ctrl.p1_b == ctrl.ON:
      imgBlend(imgTft1, imgPointer, imgMaskPointer, 166, 176)
    else:
      imgBlend(imgTft1, imgPointerGray, imgMaskPointer, 166, 176)
    if ctrl.p1_c == ctrl.ON:
      imgBlend(imgTft1, imgPointer, imgMaskPointer, 131, 141)
    else:
      imgBlend(imgTft1, imgPointerGray, imgMaskPointer, 131, 141)
    if ctrl.p1_d == ctrl.ON:
      imgBlend(imgTft1, imgPointer, imgMaskPointer, 166, 106)
    else:
      imgBlend(imgTft1, imgPointerGray, imgMaskPointer, 166, 106)
    if ctrl.p2_a == ctrl.ON:
      imgBlend(imgTft2, imgPointer, imgMaskPointer, 201, 141)
    else:
      imgBlend(imgTft2, imgPointerGray, imgMaskPointer, 201, 141)
    if ctrl.p2_b == ctrl.ON:
      imgBlend(imgTft2, imgPointer, imgMaskPointer, 166, 176)
    else:
      imgBlend(imgTft2, imgPointerGray, imgMaskPointer, 166, 176)
    if ctrl.p2_c == ctrl.ON:
      imgBlend(imgTft2, imgPointer, imgMaskPointer, 131, 141)
    else:
      imgBlend(imgTft2, imgPointerGray, imgMaskPointer, 131, 141)
    if ctrl.p2_d == ctrl.ON:
      imgBlend(imgTft2, imgPointer, imgMaskPointer, 166, 106)
    else:
      imgBlend(imgTft2, imgPointerGray, imgMaskPointer, 166, 106)
    
    x = 56
    y = 141
    if ctrl.p1_v == ctrl.UP:
      y -= ctrl.p1_v_lv * 7
    elif ctrl.p1_v == ctrl.DOWN:
      y += ctrl.p1_v_lv * 7
    if ctrl.p1_h == ctrl.LEFT:
      x -= ctrl.p1_h_lv * 7
    elif ctrl.p1_h == ctrl.RIGHT:
      x += ctrl.p1_h_lv * 7
    if x == 56  and  y == 141:
      imgBlend(imgTft1, imgPointerGray, imgMaskPointer, x, y)
    else:
      imgBlend(imgTft1, imgPointer, imgMaskPointer, x, y)
    x = 56
    y = 141
    if ctrl.p2_v == ctrl.UP:
      y -= ctrl.p2_v_lv * 7
    elif ctrl.p2_v == ctrl.DOWN:
      y += ctrl.p2_v_lv * 7
    if ctrl.p2_h == ctrl.LEFT:
      x -= ctrl.p2_h_lv * 7
    elif ctrl.p2_h == ctrl.RIGHT:
      x += ctrl.p2_h_lv * 7
    if x == 56  and  y == 141:
      imgBlend(imgTft2, imgPointerGray, imgMaskPointer, x, y)
    else:
      imgBlend(imgTft2, imgPointer, imgMaskPointer, x, y)
    
    # レンダリング完了通知
    eventRenderFinish.set()
    
    #レンダリング結果のコピー待ち
    eventImgCopyFinish.wait()
    eventImgCopyFinish.clear()

def taskSound():
  while(True):
    eventPlaySE.clear()
    eventPlaySE.wait()
    subprocess.run(['aplay', 'SE01.wav'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

eventTftInit = Event()
eventRenderFinish = Event()
eventImgCopyFinish = Event()
eventPlaySE = Event()

threadDraw = Thread(target=taskDraw)
threadRender = Thread(target=taskRender)
threadSound = Thread(target=taskSound)

threadDraw.start()
threadRender.start()
threadSound.start()
