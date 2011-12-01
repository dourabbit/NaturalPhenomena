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
	MatrixP(){}
	MatrixP(int r, int c, const DATA *pData)
	{
		_pData = new DATA[r*c];
		
		for(int i=0;i<r*c;i++)
		{
			_data.push_back(*pData);
			pData++;
		}
		
		ROWS = r;
		COLS = c;
	};
	

	MatrixP(int r, int c)
	{
		_pData = new DATA[r*c];

		for(int i=0;i<r*c;i++)
		{
			_data.push_back(0);
			
		}
		
		ROWS = r;
		COLS = c;
	};
	~MatrixP(){
		delete[] _pData;
	};
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

	MatrixP MultiplyP(MatrixP const &a, DATA const &b) 
	{
		MatrixP ret(a.ROWS,a.COLS);
		for (unsigned int r = 0; r < a.ROWS; ++r) {
			for (unsigned int c = 0; c < a.COLS; ++c) {
				ret(r,c) = a(r,c)*b; 
			}
		}
		return ret;
	};
	MatrixP AddP(MatrixP const &a,MatrixP const &b) 
	{
		MatrixP ret(a.ROWS,a.COLS);
		for (unsigned int r = 0; r < a.ROWS; ++r) {
			for (unsigned int c = 0; c < a.COLS; ++c) {
				ret(r,c) = a(r,c)+b(r,c);
			}
		}
		return ret;
	};
	MatrixP Transpose(MatrixP a) 
	{
		MatrixP ret(a.COLS,a.ROWS);
		for (unsigned int r = 0; r < a.ROWS; ++r) {
			for (unsigned int c = 0; c < a.COLS; ++c) {
				ret(c,r) =a(r,c);
			}
		}
		return ret;
	};
	
	DATA &operator()(unsigned int row, unsigned int col) {
		return _data[row * COLS + col];
	}
	DATA const &operator()(unsigned int row, unsigned int col) const {
		return _data[row * COLS + col];
	}
	
	DATA* ToArray()
	{
		return _pData;
	}
private:
	std::vector<DATA> _data;
	DATA* _pData;
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
