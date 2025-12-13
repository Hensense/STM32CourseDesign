#ifndef SAVITZKYGOLAY_C_H
#define SAVITZKYGOLAY_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SavitzkyGolayFilter SavitzkyGolayFilter;

SavitzkyGolayFilter* sg_filter_create(int window_size, int poly_order);

void sg_filter_destroy(SavitzkyGolayFilter* filter);

float sg_filter_process(SavitzkyGolayFilter* filter, float x);

int sg_smooth_c(const double* input, int width, int deg, double* output);

#ifdef __cplusplus
}
#endif

#endif // SAVITZKYGOLAY_C_H