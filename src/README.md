# something
## define
### read_
レシーバーからのアナログ入力の変換倍率
### in1
レシーバーch1のマイコン側入力ピン番号
### in2
レシーバーch2のマイコン側入力ピン番号
### in3
レシーバーch3のマイコン側入力ピン番号
### in4
レシーバーch4のマイコン側入力ピン番号
### in5
レシーバーch5のマイコン側入力ピン番号
### in6
レシーバーch6のマイコン側入力ピン番号
### puls
ledcの設定周波数
### dutyd
escの初期値（最低）
### dutys
escの最低回転周波数
### p_max
escの最高回転周波数
### BNO055interval
bno055のデータ取得間隔(ms)
## struct
### user
x,y,z,turnのintデータを入れられる
## class
### motor
モーターの回転を一括でやるとこ
#### nf
モーターを回転させるか0/1
#### d
モーターの標準回転周波数（[最低回転周波数](#dutys)に足されたものが計算される）
#### ch1
モーター1の周波数変更値
#### ch2
モーター2の周波数変更値
#### ch3
モーター3の周波数変更値
#### ch4
モーター4の周波数変更値
#### rotate();
上記に設定した値をピンに反映させる
#### motor(...);
コンストラクタ:引数にモーターのピンとチャンネルを代入する
#### setup();
pinModeとかledcの初期設定してくれる
### contloler
レシーバーを扱うところ
#### setup();
[ここ](#contloler-1)で設定したピンの初期化してくれる
#### read();
レシーバーの値を読んで[移動する値](#user)で返してくれる（[倍率](#read_)設定必須）
#### contloler(...);
コンストラクタ:レシーバーの入力ピンとそれがどこの値に当たるのかを設定する