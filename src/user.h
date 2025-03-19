#ifndef USER
#define USER
// とりあえずデータ入れるやつ
// 分かり易いようにz,y,z,turnってしてるけど実際何でもいい
template <typename T>
struct user
{
    T x;
    T y;
    T z;
    T turn;
};
#endif