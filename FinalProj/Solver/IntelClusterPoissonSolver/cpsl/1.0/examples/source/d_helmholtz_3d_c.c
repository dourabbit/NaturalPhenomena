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
!
!  This C example computes the solution of a boundary value problem for 3D 
!  Poisson equation in a parallelepiped with the mixed periodic-Neumann-Dirichlet 
!  boundary conditions using Intel(R) Cluster Poisson Solver LIbrary (Intel(R) 
!  CPSL). Example details are printed during the example run.
!
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "icpsl.h"
#include "mpi.h"

int main(int argc, char *argv[])
{
  /* Number of intervals in x, y, and z directions	
  NOTE: The number of intervals nx and ny must be divisible by the number of MPI processes
  Otherwise, CPSL routine will return error code -1. For instance, this example will run OK
  for 4 processes as 240/4=60 and 260/4=65 (both OK), while it will fail with 8 processes as 
  240/8=30 (OK), while 260/8=32,5 (not OK)*/
  int nx=240, ny=260, nz=200;

  /* Problem parameters */
  /* Tolerance parameter for error control
  Example may fail to complete properly if the tolconst has too small value */
  double tolconst=10.0e0;
  /* Boundaries of parallelelepiped domain */
  double ax=0.0e0, bx=1.0e0, ay=0.0e0, by=1.0e0, az=0.0e0, bz=1.0e0;

  /* MPI communicator */
  MPI_Comm comm;
  /* Some integer parameters */
  int i,j,k,stat=1,ya,nproc,t;
  /* It is currently recommended to declare ipar array of size 256
  to avoid memory problems */
  int ipar[256];
  /* Integer work array */
  int *cnts;
  /* Some real parameters */
  double lx,ly,lz,hx,hy,hz,c1,c2,tol,pi;
  /* dpar array has the size indicated in the Manual */
  double dpar[5*(nx+ny)/2+20];
  /* Array u contains the solution of the original differential problem,
  Array f contains the right hand side and the solution of the grid problem 
  Array work is used to store intermediate results */
  double *f, *u, *work;
  /* Arrays for non-uniform mesh in z-direction */ 
  double *z, *zh, *zh2;
  /* Data structures used by CPSL */
  DFTI_DESCRIPTOR_HANDLE xhandle1;
  DFTI_DESCRIPTOR_HANDLE xhandle2;
  DFTI_DESCRIPTOR_HANDLE yhandle1;
  DFTI_DESCRIPTOR_HANDLE yhandle2;
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
    printf("               Example of Cluster Poisson Library usage\n");
    printf("******************************************************************\n");
    printf("\n");
    printf("  This example computes the solution of 3D Poisson equation\n");
    printf("       - u_xx - u_yy - u_zz = f(x,y,z), ax<x<bx, ay<y<by, az<z<bz\n"); 
    printf("  with the right hand side\n"); 
    printf("  f(x,y,z)=pi*pi*(4/lx**2+4/ly**2+0.25/lz**2)*sin(2*pi*(x-ax)/lx)*\n");
    printf("                       *cos(2*pi*(y-ay)/ly)*cos(pi*(z-az)/(2 lz)),\n");
    printf("  and with the boundary conditions\n");
    printf("       u(ax,y,z)=u(bx,y,z), u_x(ax,y,z)=u_x(bx,y,z)");
    printf(" (periodic boundary conditions),\n");
    printf("       u(x,ay,z)=u(x,by,z), u_y(x,ay,z)=u_y(x,by,z)");
    printf(" (periodic boundary conditions),\n");
    printf("       u_z(x,y,az)= 0 (Neumann boundary condition),\n");
    printf("       u(x,y,bz)= 0 (Dirichlet boundary condition),\n");
    printf("  where lx=bx-ax, ly=by-ay, lz=bz-az.\n");
    printf("  The exact solution of this problem is the following:\n");
    printf("  u(x,y,z)=sin(2*pi*(x-ax)/lx)*cos(2*pi*(y-ay)/ly)*");
    printf("cos(pi*(z-az)/2/lz))\n");
    printf("------------------------------------------------------------------\n");
    printf("  In general, the error should be of order");
    printf(" O(1/nx**2+1/ny**2+1/nz**2)\n");
    printf("------------------------------------------------------------------\n");
    printf("                     DOUBLE PRECISION COMPUTATIONS \n");
    printf("==================================================================\n");
    printf("\n");
    printf("  The number of mesh intervals in x-direction is nx=%d\n",nx);
    printf("  The number of mesh intervals in y-direction is ny=%d\n",ny);
    printf("  The number of mesh intervals in z-direction is nz=%d\n",nz);      
    printf("\n");
  }

  if (MPI_Barrier(comm)) goto m890;

  /* Parameter for the accuracy control */
  tol=tolconst*(1.0e0/nx/nx+1.0e0/ny/ny);
  /* Parameter for memory allocation */ 
  t=nx/nproc;
  /* Memory allocation */ 
  u = (double*)malloc((t+1)*ny*(nz+1)*sizeof(double));
  if (u==NULL) goto m970;
  f = (double*)malloc((t+1)*ny*(nz+1)*sizeof(double));
  if (f==NULL) 
  {
    free(u);
    goto m970;
  }
  work =(double*)malloc(2*(t+1)*ny*(nz+1)*sizeof(double));
  if (work==NULL)
  {
    free(u);
    free(f);
    goto m970; 
  }
  z = (double*)malloc((nz+1)*sizeof(double));
  if (z==NULL)
  {
    free(u);
    free(f);
    free(work);
    goto m970;
  }
  zh =(double*)malloc(nz*sizeof(double));
  if (zh==NULL)
  {
    free(u);
    free(f);
    free(work);
    free(z);
    goto m970;
  }
  zh2 =(double*)malloc(nz*sizeof(double));
  if (zh2==NULL)
  {
    free(u);
    free(f);
    free(work);
    free(z);
    free(zh);
    goto m970;
  }
  cnts =(int*)malloc(nproc*sizeof(int));
  if (cnts==NULL)
  {
    free(u);
    free(f);
    free(work);
    free(z);
    free(zh);
    free(zh2);
    goto m970;
  }

  /* Computing the length of side lx and the mesh size hx in x-direction */ 
  lx=bx-ax;
  hx=lx/nx;
  /* Computing the length of side ly and the mesh size hy in y-direction */ 
  ly=by-ay; 
  hy=ly/ny;
  /*  Computing the length of side lz and generic step size for non-uniform 
  mesh generation in z-direction */ 
  lz=bz-az;
  hz=lz/(pow(1.01e0,nz)-1.0e0);

  /* Non-uniform mesh generation */ 
  for(i=0;i<=nz;i++)
  {
    z[i]=az+hz*(pow(1.01e0,i)-1.0e0);
  }
  /* Filling the work arrays zh and zh2 according to the BCtype parameter below */
  zh[0]=1.0e0/(z[1]-z[0]);
  zh2[0]=zh[0];
  for(i=1;i<nz;i++)
  {
    zh[i]=2.0e0/(z[i+1]-z[i-1]); 
    zh2[i]=1.0e0/(z[i+1]-z[i]);
  }
  /*  Exact solution and right hand side */ 
  pi=3.14159265358979324e0;
  for(k=0;k<nz;k++)
  {
    for(j=0;j<ny;j++)
    {
      for(i=0;i<t;i++)
      {
        u[k+nz*j+nz*ny*i]=sin((2.0e0*pi*(i+ya*t))/nx)*cos((2.0e0*pi*j)/ny)*
          cos(0.5e0*pi*(z[k]-az)/lz);
        f[k+nz*j+nz*ny*i]=-pi*pi*(4.0e0/(lx*lx)+4.0e0/(ly*ly)+0.25e0/(lz*lz))*
          u[k+nz*j+nz*ny*i];
      }
    }
  }

  /* Setting the type of the boundary conditions in z-direction;
  NOTE: in the current implementation of CPSL the value of BCtype 
  parameter is fixed */ 
  BCtype = "PPPPND";

  /* Initializing ipar array to make it free from garbage */
  for (i=0; i<256; i++)
  {
    ipar[i]=0;
  }

  /* Initializing simple data structures of CPSL for 3D Solver */ 
  dmv0_init_helmholtz_3d(&ax,&bx,&ay,&by,&az,&bz,&nx,&ny,&nz,BCtype,ipar,dpar,&stat);
  if (stat) goto m900;

  /* ipar[29]=2 requests multiplication of rows in the matrix corresponding 
  to Neumann boundary condition by 2 to get second order approximation. 
  Otherwise, approximation would be of first order. */ 
  ipar[29]=2;   	

  /* Initializing complex data structures of Poisson Library for 3D Solver
  NOTE: Right-hand side f may be altered after the Commit step. If you 
  want to keep it, you should save it in another memory location! */ 
  dmv0_commit_helmholtz_3d(f,&xhandle1,&xhandle2,&yhandle1,&yhandle2,ipar,dpar,
    cnts,&comm,&stat);
  if(stat) goto m900;

  /* Computing the approximate solution of 3D problem */ 
  dmv0_helmholtz_3d(f,&xhandle1,&xhandle2,&yhandle1,&yhandle2,ipar,dpar,zh,zh2,
    work,cnts,&comm,&stat);
  if(stat) goto m900;

  /* Cleaning the memory used by xhandle1,xhandle2,yhandle1,yhandle2 */ 
  dmv0_free_helmholtz_3d(&xhandle1,&xhandle2,&yhandle1,&yhandle2,ipar,&stat);
  if (stat) goto m900;

  /* Computation of the error on each process */ 
  c1=0.0e0;
  for(k=0;k<nz;k++)
  {
    for(j=0;j<ny;j++)
    {
      for(i=0;i<t;i++)
      {
        c2=fabs(u[k+nz*j+nz*ny*i]-f[k+nz*j+nz*ny*i]);
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
      printf("Double precision 3D Poisson C example has successfully PASSED\n");
      printf("through all steps of computation!\n");
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
  free(z);
  free(zh);
  free(zh2);
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
    printf("Double precision 3D Poisson C example FAILED to compute the solution...\n");
    printf("\n");
  }
  return 1;
}
/************************* End of C code example *************************/ 
