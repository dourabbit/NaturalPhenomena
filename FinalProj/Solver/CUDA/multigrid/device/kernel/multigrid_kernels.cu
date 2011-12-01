
#ifndef __multigrid_kernels_cu__
#define __multigrid_kernels_cu__

#include"../../common/common.co"

#define __compute_capability__ 10

#if __compute_capability__<=13
#define IM(x,y) __umul24(x,y)
#else
#define IM(x,y) ((x)*(y))
#endif

extern "C"
{

__global__
void kernel_setBC(float* field,
				  const float* BC,
				  const unsigned int xSize,
				  const unsigned int ySize
				  )
{
	const unsigned int tidx=IM(blockDim.x,blockIdx.x)+threadIdx.x;
	unsigned int idx=0;
	if(tidx<xSize){
	    const unsigned int bordT=tidx;
	    const unsigned int bordD=IM(xSize,xSize-1)+tidx;
	    field[bordT]=BC[idx]; idx+=xSize;
	    field[bordD]=BC[idx];
	}
	if(tidx<ySize){
		const unsigned int bordL=IM(xSize,tidx);
		const unsigned int bordR=IM(xSize,tidx)+xSize-1;
		idx+=ySize;
		field[bordL]=BC[idx]; idx+=ySize;
		field[bordR]=BC[idx];
	}
}

__global__
void kernel_residual(float* r,
					 const float* u,
					 const float* d,
					 const unsigned int xSize,
					 const unsigned int ySize,
					 const float qrdx
					 )
{
	__shared__ float cell[DY+2][DX+2];

	const unsigned int globalx=IM(DX,blockIdx.x)+threadIdx.x+1;
	const unsigned int globaly=IM(DY,blockIdx.y)+threadIdx.y+1;
	const unsigned int active=(globalx<xSize-1)&(globaly<ySize-1);
    const unsigned int global=IM(xSize,globaly)+globalx;
	const unsigned int localx=threadIdx.x+1;
	const unsigned int localy=threadIdx.y+1;

	if(active){
		cell[localy][localx]=u[global];
		if(threadIdx.x==0){
			cell[localy][0   ]=u[global-1 ];
			cell[localy][DX+1]=u[global+DX];
		} 
		if(threadIdx.y==0){
			cell[0   ][localx]=u[global-xSize       ];
			cell[DY+1][localx]=u[global+IM(xSize,DY)];
		}
	} __syncthreads();

	if(active){
		r[global]=qrdx*(cell[localy  ][localx-1]+
			            cell[localy  ][localx+1]+
						cell[localy-1][localx  ]+
			            cell[localy+1][localx  ]-
				   4.0f*cell[localy  ][localx  ])-d[global];
		
	}
}
	
__global__
void kernel_relax(float* uDst,
				  const float* uSrc,
				  const float* d,
				  const unsigned int xSize,
				  const unsigned int ySize,
				  const float qdx
				  )
{
    __shared__ float cell[DY+2][DX+2];

	const unsigned int globalx=IM(DX,blockIdx.x)+threadIdx.x+1;
	const unsigned int globaly=IM(DY,blockIdx.y)+threadIdx.y+1;
	const unsigned int active=(globalx<xSize-1)&(globaly<ySize-1);
    const unsigned int global=IM(xSize,globaly)+globalx;
	const unsigned int localx=threadIdx.x+1;
	const unsigned int localy=threadIdx.y+1;

	if(active){
		cell[localy][localx]=uSrc[global];
		if(threadIdx.x==0){
			cell[localy][0   ]=uSrc[global-1 ];
			cell[localy][DX+1]=uSrc[global+DX];
		} 
		if(threadIdx.y==0){
			cell[0   ][localx]=uSrc[global-xSize       ];
			cell[DY+1][localx]=uSrc[global+IM(xSize,DY)];
		}
	} __syncthreads();

	if(active){
		float temp=0.25f*(cell[localy-1][localx  ]+
			              cell[localy+1][localx  ]+
                          cell[localy  ][localx-1]+
			              cell[localy  ][localx+1]-qdx*d[global]);

		uDst[global]=0.66666667f*cell[localy][localx]+0.33333333f*temp;
	}
}

__global__
void kernel_restrict(float* coarse,
					 const float* fine,
					 const unsigned int xCoar,
					 const unsigned int yCoar,
					 const unsigned int xFine,
					 const unsigned int yFine
					 )
{
	__shared__ float cell[DY*2+1][DX*2+2];

	const unsigned int globalx=IM(DX,blockIdx.x)+threadIdx.x+1;
	const unsigned int globaly=IM(DY,blockIdx.y)+threadIdx.y+1;
    const unsigned int active=(globalx<xCoar-1)&(globaly<yCoar-1);

	if(active){
		const unsigned int u=IM(DX*2,blockIdx.x)+threadIdx.x+1;
		const unsigned int v=IM(DY*2,blockIdx.y)+threadIdx.y+1;
		unsigned int idx=IM(xFine,v)+u;
		cell[threadIdx.y   ][threadIdx.x     ]=fine[idx]; idx+=DX;
		cell[threadIdx.y   ][threadIdx.x+DX+1]=fine[idx]; idx+=IM(xFine,DY);
		cell[threadIdx.y+DY][threadIdx.x+DX+1]=fine[idx]; idx-=DX;
		cell[threadIdx.y+DY][threadIdx.x     ]=fine[idx];
	    if(threadIdx.x==0){
			idx=IM(xFine,v)+u+DX*2;
		    cell[threadIdx.y   ][DX*2+1]=fine[idx]; idx+=IM(xFine,DY);
		    cell[threadIdx.y+DY][DX*2+1]=fine[idx];
			cell[threadIdx.y   ][DX    ]=cell[threadIdx.y   ][DX+1];
			cell[threadIdx.y+DY][DX    ]=cell[threadIdx.y+DY][DX+1];
	    }

	    if(threadIdx.y==0){
			idx=IM(xFine,v+DY*2)+u;
		    cell[DY*2][threadIdx.x     ]=fine[idx]; idx+=DX;
		    cell[DY*2][threadIdx.x+DX+1]=fine[idx];
	    }

		if((threadIdx.x==0)&(threadIdx.y==0)){
			idx=IM(xFine,v+DY*2)+u+DX*2;
			cell[DY*2][DX*2+1]=fine[idx];
			cell[DY*2][DX    ]=cell[DY*2][DX+1];
		}

	} __syncthreads();

	if(active){
		const unsigned int localx=(threadIdx.x<<1)+1+(threadIdx.x>=8);
		const unsigned int localy=(threadIdx.y<<1)+1;
        const unsigned int global=IM(xCoar,globaly)+globalx;

		coarse[global]=0.0625f*(cell[localy-1][localx-1] +
			                    cell[localy-1][localx+1] +
							    cell[localy+1][localx-1] +
							    cell[localy+1][localx+1])+
				       0.1250f*(cell[localy-1][localx  ] +
					            cell[localy+1][localx  ] +
						    	cell[localy  ][localx+1] +
							    cell[localy  ][localx+1])+
				       0.2500f*(cell[localy  ][localx  ]);
	}
}

__global__
void kernel_interpc(float* fine,
				    const float* coarse,
				    const unsigned int xFine,
				    const unsigned int yFine,
				    const unsigned int xCoar,
				    const unsigned int yCoar
					)
{
	__shared__ float cell[DY/2+1][DX/2+1];

	if((threadIdx.x<=DY/2)&(threadIdx.y<=DX/2)){
		const unsigned int u=IM(DX/2,blockIdx.x)+threadIdx.x;
		const unsigned int v=IM(DY/2,blockIdx.y)+threadIdx.y;
		const unsigned int idx=IM(xCoar,v)+u;
		cell[threadIdx.y][threadIdx.x]=coarse[idx];
	} __syncthreads();

	const unsigned int globalx=IM(DX,blockIdx.x)+threadIdx.x+1;
	const unsigned int globaly=IM(DY,blockIdx.y)+threadIdx.y+1;
	
	if((globalx<xFine-1)&(globaly<yFine-1)){
		const unsigned int xOdded=(globalx&0x1u)!=0;
		const unsigned int yOdded=(globaly&0x1u)!=0;
		const unsigned int localx=threadIdx.x+1;
		const unsigned int localy=threadIdx.y+1;

		float node;
		if(xOdded){
			if(yOdded){
                node=0.25f*(cell[(localy-1)>>1][(localx-1)>>1]+
                            cell[(localy-1)>>1][(localx+1)>>1]+
                            cell[(localy+1)>>1][(localx-1)>>1]+
                            cell[(localy+1)>>1][(localx+1)>>1]);
			} else {
				node=0.50f*(cell[localy>>1][(localx-1)>>1]+
					        cell[localy>>1][(localx+1)>>1]);
			}
		} else {
			if(yOdded){
				node=0.50f*(cell[(localy-1)>>1][localx>>1]+
					        cell[(localy+1)>>1][localx>>1]);
			} else {
				node=cell[localy>>1][localx>>1];
			}
		}
		
		const unsigned int global=IM(xFine,globaly)+globalx;
		fine[global]-=node;
	}
}

__global__
void kernel_dirichletBC(float* coarse,
						const float* fine,
						const unsigned int xCoar,
						const unsigned int yCoar,
						const unsigned int xFine,
						const unsigned int yFine
						)
{
	const unsigned int tidx=IM(blockDim.x,blockIdx.x)+threadIdx.x;

	if(tidx<xCoar){
	    const unsigned int bordT=tidx;
		const unsigned int bordD=IM(xCoar,xCoar-1)+tidx;
		unsigned int idx=tidx<<1;
		coarse[bordT]=fine[idx]; idx+=IM(xFine,yFine-1);
		coarse[bordD]=fine[idx];
	}
	if(tidx<yCoar){
		const unsigned int bordL=IM(xCoar,tidx);
		const unsigned int bordR=bordL+xCoar-1;
		unsigned int idx=IM(xFine,tidx<<1);
		coarse[bordL]=fine[idx]; idx+=(xFine-1);
		coarse[bordR]=fine[idx];
	}
}

}

#endif