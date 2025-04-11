#pragma once

#include "conf.h"

enum class uled_status : int
{
    ready = 0b011,
    set = 0b101,
    wait = 0b001,
    stop = 0b111,
    none = 0b000,
    def = 0b111,
};
enum ledbin : int
{
    R = 0b100,
    G = 0b010,
    Y = 0b001,
};
/// @brief ledセット
/// @param status_ 状態
/// @param y_va 黄レベル
/// @return エラーコード
int set_led(uled_status status_, int y_va = 128);
/// @brief led初期化
/// @return エラーコード
int setup_led();
