import serial

class Receiver:
  def __init__(self):
    # シリアル通信開始
    self.ser = serial.Serial('/dev/ttyAMA0', 38400, timeout=0)
    # 入力バッファ初期化
    self.ser.reset_input_buffer()
    # 入力値保管変数
    self.ctrl = Controller()
  
  def clearBuffer(self):
    # 入力バッファ初期化
    self.ser.reset_input_buffer()
  
  def receiveMessage(self):
    # バッファを多めに読み出す
    buffer = self.ser.read(20)
    # 4byte以上読み出せた場合だけ処理を行う
    if len(buffer) >= 4:
      # 最新4byteだけを入力判定
      for input in buffer[-4:]:
        index = input >> 6
        if index == 0:
          # 1Pスイッチ
          self.ctrl.p1_a = input >> 3 & 1
          self.ctrl.p1_b = input >> 2 & 1
          self.ctrl.p1_c = input >> 1 & 1
          self.ctrl.p1_d = input >> 0 & 1
        elif index == 1:
          # 2Pスイッチ
          self.ctrl.p2_a = input >> 3 & 1
          self.ctrl.p2_b = input >> 2 & 1
          self.ctrl.p2_c = input >> 1 & 1
          self.ctrl.p2_d = input >> 0 & 1
        elif index == 2:
          # 1P JoyStick H
          dir = input >> 5 & 1
          level = input >> 3 & 3
          if level > 0:
            self.ctrl.p1_h = dir + 1
          else:
            self.ctrl.p1_h = self.ctrl.OFF
          self.ctrl.p1_h_lv = level
          # 1P JoyStick V
          dir = input >> 2 & 1
          level = input >> 0 & 3
          if level > 0:
            self.ctrl.p1_v = dir + 1
          else:
            self.ctrl.p1_v = self.ctrl.OFF
          self.ctrl.p1_v_lv = level
        else:
          # 2P JoyStick H
          dir = input >> 5 & 1
          level = input >> 3 & 3
          if level > 0:
            self.ctrl.p2_h = dir + 1
          else:
            self.ctrl.p2_h = self.ctrl.OFF
          self.ctrl.p2_h_lv = level
          # 2P JoyStick V
          dir = input >> 2 & 1
          level = input >> 0 & 3
          if level > 0:
            self.ctrl.p2_v = dir + 1
          else:
            self.ctrl.p2_v = self.ctrl.OFF
          self.ctrl.p2_v_lv = level
          pass
    return self.ctrl

class Controller:
  
  OFF = 0
  ON  = 1
  
  MIN = 1
  MID = 2
  MAX = 3
  
  UP    = 2
  DOWN  = 1
  LEFT  = 2
  RIGHT = 1
  
  def __init__(self):
    self.p1_a = self.OFF
    self.p1_b = self.OFF
    self.p1_c = self.OFF
    self.p1_d = self.OFF
    self.p2_a = self.OFF
    self.p2_b = self.OFF
    self.p2_c = self.OFF
    self.p2_d = self.OFF
    self.p1_v = self.OFF
    self.p1_h = self.OFF
    self.p2_v = self.OFF
    self.p2_h = self.OFF
    self.p1_v_lv = self.OFF
    self.p1_h_lv = self.OFF
    self.p2_v_lv = self.OFF
    self.p2_h_lv = self.OFF
  