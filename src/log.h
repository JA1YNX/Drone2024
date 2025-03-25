#ifndef LOG_H
#define LOG_H
#include <Arduino.h>
#include "user.h"

enum class LogLevel : int
{
    _0NONE = 0,
    _1ERROR = 1,
    _2WARN = 2,
    _3INFO = 3,
    _4DEBUG = 4,
    _9ALL = 9
};
namespace Log
{
    /// @brief ログの表示レベルを設定
    /// @param l 表示レベル
    /// @return 変更後のログレベル
    LogLevel logset(LogLevel l);
    /// @brief ログの表示レベルを取得
    /// @return 現在のログレベル
    LogLevel logget();

    static LogLevel def = LogLevel::_3INFO;

    /// @brief ログ出力のデフォルトレベルを設定
    /// @param ll デフォルトレベル
    /// @return 設定後のデフォルトレベル
    inline LogLevel setdef(LogLevel ll)
    {
        return def = ll;
    }

    /// @brief ログ出力　改行なし
    /// @tparam T ログ情報の型
    /// @param l ログ
    /// @param ll ログレベル
    /// @return 出力結果
    template <typename T>
    int log(const T &l, LogLevel ll = def)
    {
        return (logget() >= ll) ? Serial.print(l) : 0;
    }
    /// @brief ログ出力　改行あり
    /// @tparam T ログ情報の型
    /// @param l ログ
    /// @param ll ログレベル
    /// @return 出力結果
    template <typename T>
    int logln(const T &l, LogLevel ll = def)
    {
        return (logget() >= ll) ? Serial.println(l) : 0;
    }
} // namespace Log

#endif
