#ifndef ZDPLASKIN_INCLUDE_HELPERS_VECTOR_HELPERS_H_
#define ZDPLASKIN_INCLUDE_HELPERS_VECTOR_HELPERS_H_

#include <vector>

namespace utils {

template<typename T>
using matrix = std::vector<std::vector<T>>;

template<typename T>
std::vector<T *> toPtrVec(matrix<T> &vec) {
	const size_t n = vec.size();
	std::vector<T *> res;
	for (int i = 0; i < n; i++) {
		res[i] = &vec[i][0];
	}
	return std::move(res);
}

template<typename T>
matrix<T> matrixFromSizes(unsigned i, unsigned j) {
	matrix<T> result(i);
	for (int k = 0; k < i; k++) {
		result[k].resize(j);
	}
	return result;
}

template<typename T>
std::vector<T> ptrArrayToVec(T *ptrArray, size_t size) {
	std::vector<T> res(size);
	for (int i = 0; i < size; i++) {
		res[i] = ptrArray[i];
	}
	return res;
}

template<typename T>
matrix<T> arrayToMatrix(T *ptrArray, size_t rows, size_t cols) {
	// Difference in memory allocation fortran <-> cpp
	// rows and cols are swapped to match the fortran array
	auto mat = matrixFromSizes<T>(rows, cols);
	for (int a = 0; a < rows; a++) {
		for (int b = 0; b < cols; b++) {
			size_t i = a == 0 ? (a*rows) + b : (a*rows) + b + 1;
			mat[a][b] = ptrArray[i];
		}
	}
	return mat;
}

} // namespace utils


#endif //ZDPLASKIN_INCLUDE_HELPERS_VECTOR_HELPERS_H_
