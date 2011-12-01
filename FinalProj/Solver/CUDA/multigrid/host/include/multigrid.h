
#ifndef __multigrid_h__
#define __multigrid_h__

#include"../../common/common.co"

#include<iostream>
using namespace std;

#include<cuda.h>
#pragma comment(lib,"cuda.lib")

#if defined(DEBUG)||defined(_DEBUG)
#define CU_SAFE_CALL(call){\
	CUresult errcode=call;\
	if(errcode!=CUDA_SUCCESS){\
	    printf("%d\n",errcode);\
        return;\
	}\
}
#else
#define CU_SAFE_CALL(call) { call; }
#endif

#define MAX(a,b) (((a)>(b))?(a):(b))
#define kernel(s) cu_kernel_##s

int SRC=0;
#define DST !SRC
#define SWAP()  (SRC=DST)

#define OFFSET(T) { offset+=sizeof(T); }

struct gridShape
{
	CUdeviceptr  cell;
	unsigned int xRes;
	unsigned int yRes;
};

CUcontext  cu_ctx;
CUmodule   cu_module;
CUfunction cu_kernel_setBC;
CUfunction cu_kernel_residual;
CUfunction cu_kernel_relax;
CUfunction cu_kernel_restrict;
CUfunction cu_kernel_interpc;
CUfunction cu_kernel_dirichletBC;
gridShape  source;
gridShape  target[2];
gridShape  resi[4];
gridShape  lerp[4];
gridShape  grid[2][4];
gridShape  boundary;

const unsigned int xDim=1025;
const unsigned int yDim=1025;

void create_grid(gridShape& grid,unsigned int xDim,unsigned int yDim)
{
	CU_SAFE_CALL(cuMemAlloc(&grid.cell,xDim*yDim*sizeof(float)))
	CU_SAFE_CALL(cuMemsetD8(grid.cell,0,xDim*yDim*sizeof(float)))
	grid.xRes=xDim; grid.yRes=yDim;
}

void grid_creator_init(unsigned int xDim,unsigned int yDim,unsigned int nLevels)
{
	if(((xDim&0x1u)!=1)||((yDim&0x1u)!=1)){
		cout<<"Invalid grid size!"<<endl;
		return;
	}
	unsigned int xRes=xDim;
	unsigned int yRes=yDim;
	for(unsigned int k=0;k<nLevels;++k){
		xRes=(++xRes)>>1;
		yRes=(++yRes)>>1;
	}
	if((xRes==0)||(yRes==0)){
		cout<<"Too many levels for the grid with size"
			<<"[ "<<xDim<<" ][ "
			<<yDim<<" ]"<<endl;
		return;
	}
}

void clear(const gridShape& grid)
{
	CU_SAFE_CALL(cuMemsetD8(grid.cell,0,grid.xRes*grid.yRes*sizeof(float)))
}

void __setBC__(const gridShape&,const gridShape&);

void init()
{
	CUdevice dev;
	CU_SAFE_CALL(cuInit(0))
	CU_SAFE_CALL(cuDeviceGet(&dev,0))
	CU_SAFE_CALL(cuCtxCreate(&cu_ctx,CU_CTX_SCHED_AUTO,dev))
	CU_SAFE_CALL(cuModuleLoad(&cu_module,"device/cubin/multigrid_kernels.cubin"))

	CU_SAFE_CALL(cuModuleGetFunction(&kernel(setBC),cu_module,"kernel_setBC"))
	CU_SAFE_CALL(cuFuncSetBlockShape(kernel(setBC),256,1,1))

	CU_SAFE_CALL(cuModuleGetFunction(&kernel(residual),cu_module,"kernel_residual"))
	CU_SAFE_CALL(cuFuncSetBlockShape(kernel(residual),DX,DY,1))

	CU_SAFE_CALL(cuModuleGetFunction(&kernel(dirichletBC),cu_module,"kernel_dirichletBC"))
	CU_SAFE_CALL(cuFuncSetBlockShape(kernel(dirichletBC),256,1,1))

	CU_SAFE_CALL(cuModuleGetFunction(&kernel(relax),cu_module,"kernel_relax"))
	CU_SAFE_CALL(cuFuncSetBlockShape(kernel(relax),DX,DY,1))

	CU_SAFE_CALL(cuModuleGetFunction(&kernel(restrict),cu_module,"kernel_restrict"))
	CU_SAFE_CALL(cuFuncSetBlockShape(kernel(restrict),DX,DY,1))

	CU_SAFE_CALL(cuModuleGetFunction(&kernel(interpc),cu_module,"kernel_interpc"))
	CU_SAFE_CALL(cuFuncSetBlockShape(kernel(interpc),DX,DY,1))

    unsigned int xRes=(xDim+1)>>1; 
	unsigned int yRes=(yDim+1)>>1;
    grid_creator_init(xRes,yRes,4);
	
	for(unsigned int level=0;level<4;++level){
		create_grid(lerp[level],xRes,yRes);    clear(lerp[level]);
		create_grid(grid[0][level],xRes,yRes); clear(grid[0][level]);
		create_grid(grid[1][level],xRes,yRes); clear(grid[1][level]);
		xRes=(++xRes)>>1;
		yRes=(++yRes)>>1;
	}
	
	xRes=xDim; yRes=yDim;

	create_grid(source   ,xDim,yDim); clear(source   );
	create_grid(target[0],xDim,yDim); clear(target[0]);
	create_grid(target[1],xDim,yDim); clear(target[1]);

	for(unsigned int level=0;level<4;++level){
		create_grid(resi[level],xRes,yRes);
		clear(resi[level]);
		xRes=(++xRes)>>1;
		yRes=(++yRes)>>1;
	}
	
	create_grid(boundary,(xDim*yDim)<<1,1);
	clear(boundary);
	__setBC__(target[0],boundary);
	__setBC__(target[1],boundary);
}

void __setBC__(const gridShape& target,const gridShape& BC)
{
	int offset=0;
	const int nBlocks=MAX(target.xRes,target.yRes)/256+1;
	CU_SAFE_CALL(cuParamSeti(kernel(setBC),offset,  target.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(setBC),offset,boundary.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(setBC),offset,  target.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(setBC),offset,  target.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSetSize(kernel(setBC),offset))
	CU_SAFE_CALL(cuLaunchGrid(kernel(setBC),nBlocks,1))
	CU_SAFE_CALL(cuCtxSynchronize())
}

void __residual__(const gridShape& R,const gridShape& U,const gridShape& D)
{
	int offset=0;
	const float rdx=(float)MAX(R.xRes,R.yRes);
	const float qrdx=rdx*rdx;
	CU_SAFE_CALL(cuParamSeti(kernel(residual),offset,R.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(residual),offset,U.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(residual),offset,D.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(residual),offset,R.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(residual),offset,R.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSetf(kernel(residual),offset,qrdx  )) OFFSET(float)
	CU_SAFE_CALL(cuParamSetSize(kernel(residual),offset))
	CU_SAFE_CALL(cuLaunchGrid(kernel(residual),R.xRes/DX+1,R.yRes/DY+1))
	CU_SAFE_CALL(cuCtxSynchronize())
}

void __relax__(const gridShape& uDST,const gridShape& uSRC,const gridShape& D)
{
	int offset=0;
	const float dx=1.f/MAX(D.xRes,D.yRes);
	const float qdx=dx*dx;
	CU_SAFE_CALL(cuParamSeti(kernel(relax),offset,uDST.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(relax),offset,uSRC.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(relax),offset,   D.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(relax),offset,uDST.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(relax),offset,uDST.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSetf(kernel(relax),offset,qdx      )) OFFSET(float)
	CU_SAFE_CALL(cuParamSetSize(kernel(relax),offset))
	CU_SAFE_CALL(cuLaunchGrid(kernel(relax),D.xRes/DX+1,D.yRes/DY+1))
	CU_SAFE_CALL(cuCtxSynchronize())
}

void __restrict__(const gridShape& coar,const gridShape& fine)
{
	int offset=0;
	CU_SAFE_CALL(cuParamSeti(kernel(restrict),offset,coar.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(restrict),offset,fine.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(restrict),offset,coar.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(restrict),offset,coar.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(restrict),offset,fine.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(restrict),offset,fine.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSetSize(kernel(restrict),offset))
	CU_SAFE_CALL(cuLaunchGrid(kernel(restrict),coar.xRes/DX,coar.yRes/DY))
	CU_SAFE_CALL(cuCtxSynchronize())
}

void __interpolate__(const gridShape& fine,const gridShape& coar)
{
	int offset=0;
	CU_SAFE_CALL(cuParamSeti(kernel(interpc),offset,fine.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(interpc),offset,coar.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(interpc),offset,fine.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(interpc),offset,fine.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(interpc),offset,coar.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(interpc),offset,coar.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSetSize(kernel(interpc),offset))
	CU_SAFE_CALL(cuLaunchGrid(kernel(interpc),fine.xRes/DX,fine.yRes/DY))
	CU_SAFE_CALL(cuCtxSynchronize())
}

void __dirichletBC__(const gridShape& coar,const gridShape& fine)
{
	int offset=0;
	const int nBlocks=MAX(coar.xRes,coar.yRes)/256+1;
	CU_SAFE_CALL(cuParamSeti(kernel(dirichletBC),offset,coar.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(dirichletBC),offset,fine.cell)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(dirichletBC),offset,coar.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(dirichletBC),offset,coar.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(dirichletBC),offset,fine.xRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSeti(kernel(dirichletBC),offset,fine.yRes)) OFFSET(CUdeviceptr)
	CU_SAFE_CALL(cuParamSetSize(kernel(dirichletBC),offset))
	CU_SAFE_CALL(cuLaunchGrid(kernel(dirichletBC),nBlocks,1))
	CU_SAFE_CALL(cuCtxSynchronize())
}

void release()
{
	for(int i=0;i<4;++i){
		if(resi[i].cell){
			CU_SAFE_CALL(cuMemFree(resi[i].cell))
			resi[i].xRes=resi[i].yRes=0;
		}
	}
	for(int i=0;i<3;++i){
		if(grid[0][i].cell){
			CU_SAFE_CALL(cuMemFree(grid[0][i].cell))
			grid[0][i].xRes=grid[0][i].yRes=0;
		}
		if(grid[1][i].cell){
			CU_SAFE_CALL(cuMemFree(grid[1][i].cell))
			grid[1][i].xRes=grid[1][i].yRes=0;
		}
		if(lerp[i].cell){
			CU_SAFE_CALL(cuMemFree(lerp[i].cell))
			lerp[i].xRes=lerp[i].yRes=0;
		}
	}
	if(source.cell){
		CU_SAFE_CALL(cuMemFree(source.cell))
		source.xRes=source.yRes=0;
	}
	if(target[0].cell){
		CU_SAFE_CALL(cuMemFree(target[0].cell))
		target[0].xRes=target[0].yRes=0;
	}
	if(target[1].cell){
		CU_SAFE_CALL(cuMemFree(target[1].cell))
		target[1].xRes=target[1].yRes=0;
	}
	CU_SAFE_CALL(cuModuleUnload(cu_module))
	CU_SAFE_CALL(cuCtxDestroy(cu_ctx))
}

void vcycle()
{
	for(int i=0;i<20;++i){
		__relax__(target[DST],target[SRC],source); SWAP();
	}

	__residual__(resi[0],target[SRC],source);
	__restrict__(lerp[0],resi[0]);
	clear(grid[SRC][0]);	
	__dirichletBC__(grid[0][0],target[SRC]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][0],grid[SRC][0],lerp[0]); SWAP();
	}

	__residual__(resi[1],grid[SRC][0],lerp[0]);
	__restrict__(lerp[1],resi[1]);
	clear(grid[SRC][1]);
	__dirichletBC__(grid[SRC][1],target[SRC]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][1],grid[SRC][1],lerp[1]); SWAP();
	}

	__residual__(resi[2],grid[SRC][1],lerp[1]);
	__restrict__(lerp[2],resi[2]);
	clear(grid[SRC][2]);
	__dirichletBC__(grid[SRC][2],target[SRC]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][2],grid[SRC][2],lerp[2]); SWAP();
	}

	__residual__(resi[3],grid[SRC][2],lerp[2]);
	__restrict__(lerp[3],resi[2]);
	clear(grid[SRC][3]);
	__dirichletBC__(grid[SRC][3],target[SRC]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][3],grid[SRC][3],lerp[3]); SWAP();
	}

	__interpolate__(grid[SRC][2],grid[SRC][3]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][2],grid[SRC][2],lerp[2]); SWAP();
	}

	__interpolate__(grid[SRC][1],grid[SRC][2]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][1],grid[SRC][1],lerp[1]); SWAP();
	}

	__interpolate__(grid[SRC][0],grid[SRC][1]);
	for(int i=0;i<10;++i){
		__relax__(grid[DST][0],grid[SRC][0],lerp[0]); SWAP();
	}

	__interpolate__(target[SRC],grid[SRC][0]);
	for(int i=0;i<20;++i){
		__relax__(target[DST],target[SRC],source); SWAP();
	}
}


#endif