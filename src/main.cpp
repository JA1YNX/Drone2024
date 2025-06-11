// releaes使うかどうか
#define releace_f

#if defined(releace_f)
#include "./main.hpp"
#endif

#ifndef releace_f
#include "./debug.hpp"
#endif

/*
setup()
|
|センサ初期化    sens.setup()
|
|コントローラー初期化      c.setup()
|
|モーター初期化
|
|ステータス初期化
|
|
loop()
|
|センサ読み取り
|
|コントローラー読み取り
|
|pid計算
|
|モーターに反映

*/
