#pragma once
#include "./conf.h"

// 制御用クラス
class contloler
{
public:
  /// @brief コンストラクタ
  /// @param set_ ピン番号設定(x,y,z,turn)
  contloler(user<int> set_);
  /// @brief ピン設定
  void setup();
  /// @brief 値取得
  /// @return 取得した値
  user<int> read();
  /// @brief 指定したピン
  /// @return user<int>型
  user<int> pin() const;
  /// @brief 加工する前のデータ（readで値更新）
  /// @return user<int>型
  user<int> data() const;

private:
  // 入力ピン格納用変数構造体
  user<int> set;
  user<int> c;  // スタック用変数
  user<int> ud; // 標準値
};
