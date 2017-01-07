#ifndef _H_MAT_TRANS
#define _H_MAT_TRANS
#include "math.hpp"

template<typename T,N>
Vec<T,N> mat_trans(MatNxm<T,N,N> &m, Vec<T,N> v)
{
	return m * v;
}





#endif