RPiWDのプラットフォームを外部SW(GPIO2接続)で起動するスクリプト

systemdを使用してLinux起動時にサービスとしてスクリプトを実行する。  
SWを3秒押し続けるとRPiWDが起動し、さらにSWを3秒押し続けるとLinuxをシャットダウンする。

## サービスをコピー
> sudo cp RPiWD_Startup.service /etc/systemd/system/
## サービスを自動起動
> sudo systemctl enable RPiWD_Startup.service

## 動作確認
> sudo systemctl start RPiWD_Startup.service  
> sudo systemctl status RPiWD_Startup.service
