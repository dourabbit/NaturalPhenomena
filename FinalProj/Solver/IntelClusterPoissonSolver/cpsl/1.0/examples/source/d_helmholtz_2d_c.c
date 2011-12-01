/*******************************************************************************
!                              INTEL CONFIDENTIAL
!   Copyright(C) 2008 Intel Corporation. All Rights Reserved.
!   The source code contained  or  described herein and all documents related to
!   the source code ("Material") are owned by Intel Corporation or its suppliers
!   or licensors.  Title to the  Material remains with  Intel Corporation or its
!   suppliers and licensors. The Material contains trade secrets and proprietary
!   and  confidential  information of  Intel or its suppliers and licensors. The
!   Material  is  protected  by  worldwide  copyright  and trade secret laws and
!   treaty  provisions. No part of the Material may be used, copied, reproduced,
!   modified, published, uploaded, posted, transmitted, distributed or disclosed
!   in any way without Intel's prior express written permission.
!   No license  under any  patent, copyright, trade secret or other intellectual
!   property right is granted to or conferred upon you by disclosure or delivery
!   of the Materials,  either expressly, by implication, inducement, estoppel or
!   otherwise.  Any  license  under  such  intellectual property  rights must be
!   express and approved by Intel in writing.
!
!*******************************************************************************
!  This C example computes the solution of a boundary value problem for 2D 
!  Helmholtz problem in a rectangular domain with the mixed Neumann-Dirichlet 
!  boundary conditions. Example details are printed during the example run.
!
!*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "icpsl.h"
#include "mpi.h"

int main(int argc, char *argv[])
{
  /* Number of intervals in x and y directions */	
  int nx=111, ny=222;

  /* Problem parameters */
  /* Tolerance parameter for error control
  Example may fail to complete properly if the tolconst has too small value */
  double tolconst=2.0e0;
  /* Boundaries of rectangular domain */
  double ax=0.0e0, bx=2.0e0, ay=0.0e0, by=1.0e0;
  /* Helmholtz coefficient */
  double q=1.0e0;

  long long tmp;

  /* MPI communicator */
  MPI_Comm comm;
  /* Some integer parameters */
  int ix,iy,i,stat=1,ya,nproc,t1,t2,t3,ny_start,ny_lenght,temp;
  /* It is highly recommended to declare ipar array of size 256
  for compatibility with future versions of CPSL */
  int ipar[256];
  /* Integer work array */
  int *cnts;
  /* Some real parameters */
  double lx,ly,hx,hy,c1,c2,tol,pi;
  /* dpar array has the size indicated in the Manual */
  double dpar[13*nx/2+7];
  /* Array u contains the solution of the original differential problem,
  Array f contains the right hand side and the solution of the grid problem 
  Array work is used to store intermediate results */
  double *f, *u, *work;
  /* Nonhomogenious boundary conditions */
  double bd_ax[ny+1], bd_bx[ny+1], bd_ay[nx+1], bd_by[nx+1];
  /* Data structure used by CPSL */
  DFTI_DESCRIPTOR_HANDLE xhandle;
  /* Array to store the types of boundary conditions */
  char *BCtype;

  /* MPI initialization */
  if (MPI_Init(&argc,&argv)) goto m880;
  if (MPI_Comm_dup(MPI_COMM_WORLD,&comm)) goto m890; 
  if (MPI_Comm_rank(comm,&ya)) goto m890;
  if (MPI_Comm_size(comm,&nproc)) goto m890;

  /* Printing the header for the example on process number 0 */
  if (!ya) 
  {
    printf("\n");
    printf("              Example of Cluster Poisson Library usage\n");
    printf(" *******************************************************************\n");
    printf("\n");
    printf(" This example computes the solution of 2D Helmholtz equation\n"); 
    printf("     - u_xx - u_yy + qu = f(x,y), ax<x<bx, ay<y<by\n"); 
    printf(" with the right hand side\n"); 
    printf(" f(x,y)=(pi*pi(1/lx**2+1/ly**2)+q)*sin(pi*(x-ax)/lx)*sin(pi*(y-ay)/ly)+");
    printf("q,\n");
    printf(" and with the boundary conditions\n");
    printf("     u(ax,y)=u(bx,y)=1 (Dirichlet boundary conditions),\n");
    printf("     u_y(x,ay)= pi/ly*sin(pi*(x-ax)/lx) (Neumann boundary condition),\n");
    printf("     u_y(x,by)=-pi/ly*sin(pi*(x-ax)/lx) (Neumann boundary condition),\n");
    printf(" where lx=bx-ax, ly=by-ay.\n");
    printf(" The exact solution of this problem is the following:\n");
    printf("     u(x,y) = sin(pi*(x-ax)/lx)*sin(pi*(y-ay)/ly)+1\n");
    printf(" -------------------------------------------------------------------\n");
    printf(" In general, the error should be of order O(1.0/nx^2+1.0/ny^2)\n");
    printf(" -------------------------------------------------------------------\n");
    printf("                    DOUBLE PRECISION COMPUTATIONS  \n");
    printf(" ===================================================================\n");
    printf("\n");
    printf(" The number of mesh intervals in x-direction is nx=%d\n",nx);
    printf(" The number of mesh intervals in y-direction is ny=%d\n",ny);
    printf("\n"); 
  }   	

  if (MPI_Barrier(comm)) goto m890;
  /* Parameter for the control of accuracy */
  tol=tolconst*(1.0e0/nx/nx+1.0e0/ny/ny);
  /* Parameters for memory allocation */ 
  t1=((ny+1)/nproc+1);
  t2=(((nx+1)/nproc+1)*(ny+1))/(nx+1)+1;
  if (t2<t1) t2=t1;
  t3=(nx+1)*t2;
  if (t3<(2*ny+2)) t3=2*(ny+1);
  /* Memory reservation */ 
  u    =(double*)malloc((nx+1)*(t1)*sizeof(double));
  if (u==NULL) goto m970;
  f    =(double*)malloc((nx+1)*(t2)*sizeof(double));
  if (f==NULL) 
  {
    free(u);
    goto m970;
  }
  work =(double*)malloc((t3)*sizeof(double));
  if (work==NULL)
  {
    free(u);
    free(f);
    goto m970;
  }
  cnts =(int*)malloc((4*nproc)*sizeof(int));
  if (cnts==NULL)
  {
    free(u);
    free(f);
    free(work);
    goto m970;
  }

  /* Computing the length of side lx and the mesh size hx in x-direction	*/
  lx=bx-ax;
  hx=lx/nx;
  /* Computing the length of side ly and the mesh size hy in y-direction	*/
  ly=by-ay;
  hy=ly/ny;

  temp=(ny+1)-nproc*((ny+1)/nproc);
  if (ya<temp)
  {
    ny_start=ya*(1+(ny+1)/nproc);
    ny_lenght=(1+(ny+1)/nproc);
  }
  else
  {
    ny_start=temp*(1+(ny+1)/nproc)+(ya-temp)*(((ny+1)/nproc));
    ny_lenght=(ny+1)/nproc;
  }

  /* Filling in the values of the TRUE solution u(x,y)=sin(pi*(x-ax))*sin(pi*(y-ay))+1 
  in the mesh points into the array u row by row
  Filling in the right-hand side 
  f(x,y)=(pi*pi*(1/lx**2+1/ly**2)+q)*sin(pi*(x-ax))*sin(pi*(y-ay))+q 
  in the mesh points into the array f row by row
  We choose the right-hand side to correspond to the TRUE solution of 
  Helmholtz equation */
  pi=3.14159265358979324e0;
  for (iy=ny_start; iy<(ny_start+ny_lenght); iy++)
  {
    for (ix=0; ix<=nx; ix++)
    {
      c1=sin(pi*hx*ix/lx);
      c2=sin(pi*hy*iy/ly);
      c1=c1*c2;
      c2=1.0e0/(lx*lx)+1.0e0/(ly*ly);
      f[ix+(iy-ny_start)*(nx+1)]=(c2*pi*pi+q)*c1+q;
      u[ix+(iy-ny_start)*(nx+1)]=c1+1.0e0;
    }
  }

  /* Setting the type of the boundary conditions on each side of the rectangular 
  domain:
  On the boundary laying on the line x=ax Dirichlet boundary condition will be used
  On the boundary laying on the line x=bx Dirichlet boundary condition will be used
  On the boundary laying on the line y=ay Neumann boundary condition will be used
  On the boundary laying on the line y=by Neumann boundary condition will be used */ 
  BCtype = "DDNN";

  /* Setting the boundary values that correspond to the TRUE solution
  in the mesh points laying on Dirichlet boundaries */ 
  for (iy=0; iy<=ny; iy++)
  {
    bd_ax[iy]=1.0e0; 
    bd_bx[iy]=1.0e0;
  }
  /* Setting the boundary values that are equal to the normal derivative
  of the TRUE solution in the mesh points laying on Neumann boundaries.
  NOTE: at y=ay bd_ay=-u_y(x,ay) */ 
  for (ix=0; ix<=nx; ix++)
  {
    bd_ay[ix]=-pi*sin(pi*ix/nx)/ly;
    bd_by[ix]=-pi*sin(pi*ix/nx)/ly;
  }

  /* Initializing ipar array to make it free from garbage */
  for (i=0; i<128; i++)
  {
    ipar[i]=0;
  }

  /* Initializing simple data structures of Poisson Library for 2D Helmholtz Solver */
  dmv0_init_helmholtz_2d(&ax,&bx,&ay,&by,&nx,&ny,BCtype,&q,ipar,dpar,&stat);
  if (stat) goto m900;

  /* Initializing complex data structures of Poisson Library for 2D Helmholtz Solver
  NOTE: Right-hand side f may be altered after the Commit step. If you want 
  to keep it, you should save it in another memory location! */
  dmv0_commit_helmholtz_2d(f,bd_ax,bd_bx,bd_ay,bd_by,&xhandle,ipar,dpar,cnts,&comm,
    &stat);
  if (stat) goto m900;

  /* Computing the approximate solution of 2D Helmholtz problem
  NOTE: Boundary data stored in the arrays bd_ax, bd_bx, bd_ay, bd_by should not 
  be changed between the Commit step and the subsequent call to the Solver routine!
  Otherwise the results may be wrong. */
  dmv0_helmholtz_2d(f,bd_ax,bd_bx,bd_ay,bd_by,&xhandle,ipar,dpar,work,cnts,&comm,
    &stat);
  if (stat) goto m900;

  /* Cleaning the memory used by xhandle */
  dmv0_free_helmholtz_2d(&xhandle, ipar, &stat);
  if (stat) goto m900;
  /* Now we may use xhandle to solve another 2D Helmholtz problem */

  /* Computation of the error on each processor */
  c1=0.0e0;
  for (ix=0; ix<=nx; ix++)
  {
    for (iy=0; iy<=ny; iy++)
    {
      if ((iy>=ny_start)&&(iy<=(ny_start+ny_lenght-1)))
      {
        c2=fabs(f[ix+(iy-ny_start)*(nx+1)]-u[ix+(iy-ny_start)*(nx+1)]); 
        if (c2>c1) c1=c2;
      }
    }
  }    

  printf("Process %d computed solution with error=%e\n",ya,c1);   

  /* Computation of global error */
  if (MPI_Allreduce(&c1,&c2,1,MPI_DOUBLE_PRECISION,MPI_MAX,comm)) goto m890;
  if(ya==nproc-1)
  {
    printf("\n");
    printf(" Global error=%e\n",c2);
    printf("\n");
  }
  if (MPI_Barrier(comm)) goto m890;
  if(c2>tol) 
  {
    if (!ya) 
    {
      printf("The computed solution seems to be inaccurate.\n");
      goto m960;
    }
  }
  else
  {
    free(u);  
    free(f);
    free(work);        
    free(cnts);    
    if (MPI_Finalize()) goto m990;
    if (!ya)
    {
      printf("Double precision 2D Helmholtz C example has successfully \n");
      printf("PASSED through all steps of computation!\n");
      printf("\n");
    }
    return 0;
  }  


  /* Failure message to print if something went wrong */ 
m880:
  printf("\n");
  printf("Failed to initialize MPI!\n");
  goto m999;

m890:
  printf("\n");
  printf("MPI routine terminated with non-zero status!\n");
  MPI_Abort(comm,stat);
  goto m980;

m900:
  printf("\n");
  printf("CPSL routine terminated with non-zero status %d\n",stat);
  MPI_Abort(comm,stat);

m960:
  free(u);
  free(f);
  free(work);
  free(cnts);
  goto m980;

m970:   
  printf("\n");
  printf("Memory allocation routine was unable to allocate memory!\n");
  MPI_Abort(comm,stat);

m980:   
  if (MPI_Finalize()) goto m990;
  goto m999;

m990:
  printf("\n");
  printf("Failed to finalize MPI!\n");

m999:
  if (!ya)
  {
    printf("\n");
    printf("Double precision 2D Helmholtz C example FAILED to compute the ");
    printf("solution...\n");
    printf("\n");
  }
  return 1;

}
/************************* End of C code example *************************/ 
