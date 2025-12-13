#ifndef _SGSMOOTH_H
#define _SGSMOOTH_H

#include <vector>

// savitzky golay smoothing.
std::vector<double> sg_smooth(const std::vector<double> &v, const int w, const int deg);
//! numerical derivative based on savitzky golay smoothing.
std::vector<double> sg_derivative(const std::vector<double> &v, const int w,
                                const int deg, const double h=1.0);

#endif // _SGSMOOTH_H