#include "incl/math_func.h"

template <typename T> int sgn( T val )
{
    return (T(0) < val) - (val < T(0)); // T(0) acts like a type cast to 0
}