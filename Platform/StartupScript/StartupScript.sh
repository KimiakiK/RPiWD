#!/bin/bash

#状態
STATE_INIT=0
STATE_RUN=1
STATE_SHUTDOWN=2

#状態変数
state=$STATE_INIT

# SW入力値
SW_ON=0
SW_OFF=1

# SW入力のONが連続している回数
sw_on_count=0

while [ $state -ne $STATE_SHUTDOWN ]
do
  # SW入力取得 GPIO2
  sw=$(gpio -g read 2)

  # SW ONが連続している回数をカウント
  if [ $sw -eq $SW_ON ]; then
    sw_on_count=$(($sw_on_count + 1))
  else
    sw_on_count=0
  fi

  # 状態遷移
  if [ $sw_on_count -eq 3 ]; then
    # SW ONが3回連続で次の状態に遷移
    state=$(($state + 1))

    # 状態遷移時のアクション
    if [ $state -eq $STATE_RUN ] ;then
      # RPiWDを起動
      sudo nohup nice -n -20 python3 main.py &
    elif [ $state -eq $STATE_SHUTDOWN ] ;then
      # 電源を落とす
      halt -p
    fi
  fi

  # SW入力を一定周期で監視
  sleep 1s
done
