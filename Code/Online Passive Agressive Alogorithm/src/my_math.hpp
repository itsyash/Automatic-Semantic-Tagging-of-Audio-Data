#ifndef _MY_MATH_H_
#define _MY_MATH_H_
///
/// @brief
/// @author
/// @date
///

#include <vector>
#include <cmath>

namespace math {

#if 1
template<class T> T log_sum (T a, T b)
{
        if (a != -INFINITY && b != -INFINITY) {
                T e = exp(a - b);
                if (e == INFINITY) { return a > b ? a : b; }
                else { return b + log(1 + e); }
        }
        else if (a == -INFINITY) { return b; }
        else if (b == -INFINITY) { return a; }
        else { return -INFINITY; }
}
#else
#define MINUS_LOG_EPSILON  50
template<class T> T log_sum (T x, T y)
{
        const double vmin = std::min(x, y);
        const double vmax = std::max(x, y);
        if (vmax > vmin + MINUS_LOG_EPSILON) {
                return vmax;
        } else {
                return vmax + std::log(std::exp(vmin - vmax) + 1.0);
        }
}
#endif

template<class T>
T square(T a)
{
        return a * a;
}

template<class T> T max (T a, T b, T c)
{
        return std::max(a, std::max(b, c));
}

template<class T> T sgn(T x)
{
	return (x > 0 ? 1 : (x == 0 ? 0 : -1));
}

template<class T> T logistic(T x)
{
	return 1.0 / (1.0 + std::exp(-x));
}

} //namespace math {

#endif // #ifndef _MY_MATH_H_
