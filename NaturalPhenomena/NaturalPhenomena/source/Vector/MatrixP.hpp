#ifndef MATRIXP_HPP
#define MATRIXP_HPP
//
//#include "Vector.hpp"
//#include "Quat.hpp"

#include <Vector\Vector.hpp>
#include <Vector\Quat.hpp>
#include <vector>
#include <assert.h>


class MatrixP {
public:
	
	
	
	MatrixP(int r, int c, const DATA *pData)
	{
		
		for(int i=0;i<r*c;i++)
		{
			Data.push_back(*pData);
			pData++;
		}
		
		ROWS = r;
		COLS = c;
	};
	

	MatrixP(int r, int c)
	{
		
		for(int i=0;i<r*c;i++)
		{
			Data.push_back(0);
			
		}
		
		ROWS = r;
		COLS = c;
	};
	~MatrixP(){};
	int ROWS;
	int COLS;
	
	MatrixP MultiplyP(MatrixP const &a, MatrixP const &b) 
	{
		MatrixP ret(a.ROWS,b.COLS);
		for (unsigned int r = 0; r < a.ROWS; ++r) {
			for (unsigned int c = 0; c < b.COLS; ++c) {
				ret(r,c) = 0;
				for (unsigned int i = 0; i < a.COLS; ++i) {
					ret(r,c) += a(r,i) * b(i, c);
				}
			}
		}
		return ret;
		
	};
	
	DATA &operator()(unsigned int row, unsigned int col) {
		return Data[row * COLS + col];
	}
	DATA const &operator()(unsigned int row, unsigned int col) const {
		return Data[row * COLS + col];
	}
	
private:
	std::vector<DATA> Data;
};
//
//MatrixP MultiplyP::MultiplyP(MatrixP const &a, MatrixP const &b) {
//	MatrixP ret(a.ROWS,b.COLS);
//	for (unsigned int r = 0; r < a.ROWS; ++r) {
//		for (unsigned int c = 0; c < b.COLS; ++c) {
//			ret(r,c) = 0;
//			for (unsigned int i = 0; i < a.COLS; ++i) {
//				ret(r,c) += a(r,i) * b(i, c);
//			}
//		}
//	}
//	return ret;
//}

#endif //MATRIX_HPP
