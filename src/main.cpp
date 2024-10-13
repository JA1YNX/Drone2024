//releaes使うかどうか
#define releace
//PIDにするかどうか
#define PID_f

#ifdef defined(releace)&&defined(PID_f)
#include "./PID.hpp"
#elif defined(releace)
#include "./releace.hpp"
#endif

#ifndef releace_f
#include "./debug.hpp"
#endif
