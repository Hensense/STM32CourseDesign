#include "sgsmooth.h"
#include "sgsmooth_interface.h"
#include <vector>

//SavitzkyGolayFilter* sg_filter_create(int window_size, int poly_order) {
////		return new ::SavitzkyGolayFilter(window_size, poly_order);
//		return nullptr;
//}

//void sg_filter_destroy(SavitzkyGolayFilter* filter) {
////    if (filter) {
////        delete filter;
////    }
//		delete filter;
//}

//float sg_filter_process(SavitzkyGolayFilter* filter, float x) {
//    if (!filter) return x;
//    return (*filter)(x);
//}


int sg_smooth_check_params(size_t length, int width, int deg) {
    if ((width < 1) || (deg < 0) || (length < (2 * width + 2))) {
        return 0; 
    }
    return 1; 
}

int get_data_length(const double* data) {
    if (!data) return 0;
    
    int length = 0;
    while (!std::isnan(data[length]) && std::fabs(data[length]) > 1e-300) {
        length++;
    }
    return length;
}

/**
 * CSavitzky-Golay????
 * ???????????(?0.0??????)
 * 
 * @param input ??????,?0.0??????
 * @param width ?????
 * @param deg ?????
 * @param output ????(??????????)
 * @return ?????????,-1????
 */
int sg_smooth_c(const double* input, int width, int deg, double* output) {
    if (!input || !output) {
        return -1;
    }
    
    // ????????
    int length = get_data_length(input);
    if (length <= 0) {
        return -1;
    }
    
    // ???????
    if (!sg_smooth_check_params(length, width, deg)) {
        return -1;
    }
    
		std::vector<double> input_vec(input, input + length);
		
		std::vector<double> result = sg_smooth(input_vec, width, deg);
		
		if (result.size() != static_cast<size_t>(length)) {
				return -1;
		}
		
		std::memcpy(output, result.data(), length * sizeof(double));
		return length;
}