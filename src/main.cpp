//releaes使うかどうか
#define releace_f
//PIDにするかどうか
#define PID_f

#if defined(releace_f)
#if defined(PID_f)
#include "./PID.hpp"
#else
#include "./releace.hpp"
#endif
#endif

#ifndef releace_f
#include "./debug.hpp"
#endif
