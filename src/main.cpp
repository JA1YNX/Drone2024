//releaes使うかどうか
#define releace_f
//PIDにするかどうか
#define PID_f

#if defined(releace_f)
#include "./releace.hpp"
#elif defined(PID_f)
#include "./PID.hpp"
#endif

#ifndef releace_f
#include "./debug.hpp"
#endif
