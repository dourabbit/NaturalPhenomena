!*******************************************************************************
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
!  This Fortran example computes the solution of a boundary value problem for 3D 
!  Poisson equation in a parallelepiped with the mixed Periodic-Neumann-Dirichlet 
!  boundary conditions using Intel(R) Cluster Poisson Solver Library (Intel(R) 
!  CPSL). Example details are printed during the example run.
!
!*******************************************************************************

PROGRAM CPSL_HELMHOLTZ_3D_EXAMPLE_F

USE ICPSL

IMPLICIT NONE

INCLUDE "mpif.h"

! Number of intervals in x, y, and z directions
! NOTE: The number of intervals nx and ny must be divisible by the number of MPI processes
! Otherwise, CPSL routine will return error code -1. For instance, this example will run OK
! for 4 processes as 240/4=60 and 260/4=65 (both OK), while it will fail with 8 processes as 
! 240/8=30 (OK), while 260/8=32,5 (not OK)
INTEGER nx,ny,nz
PARAMETER(nx=240, ny=260, nz=200)

! Problem parameters
DOUBLE PRECISION tolconst, ax, bx, ay, by, az, bz
! Tolerance parameter for error control
! Example may fail to complete properly if the tolconst has too small value
PARAMETER(tolconst=10.0D0)
! Boundaries of parallelelepiped domain 
PARAMETER(ax=0.0D0, bx=1.0D0, ay=0.0D0, by=1.0D0, az=0.0D0, bz=1.0D0)

! Some integer parameters
INTEGER i,j,k,stat,ya,comm,nproc,t
! It is currently recommended to declare ipar array of size 256
! to avoid memory problems
INTEGER ipar(256)
! Integer work array
INTEGER, POINTER :: cnts(:)
! Some real parameters
DOUBLE PRECISION lx,ly,lz,hx,hy,hz,c1,c2,tol,pi       
! dpar array has the size indicated in the Manual
DOUBLE PRECISION dpar(5*(nx+ny)/2+20)
! Array u contains the solution of the original differential problem,
! Array f contains the right hand side and the solution of the grid problem 
! Array work is used to store intermediate results
DOUBLE PRECISION, POINTER :: u(:), f(:), work(:)
! Arrays for non-uniform mesh in z-direction
DOUBLE PRECISION, POINTER :: z(:), zh(:), zh2(:)
! Data structures used by CPSL
TYPE(DFTI_DESCRIPTOR), POINTER :: xhandle1, xhandle2, yhandle1, yhandle2 
! Array to store the types of boundary conditions
CHARACTER(6) BCtype

! MPI initialization
   CALL MPI_INIT(stat)
   IF (stat.NE.0) GOTO 880
   CALL MPI_COMM_DUP(MPI_COMM_WORLD,comm,stat)
   IF (stat.NE.0) GO TO 890
   CALL MPI_COMM_RANK(comm,ya,stat)
   IF (stat.NE.0) GO TO 890
   CALL MPI_COMM_SIZE(comm,nproc,stat)
   IF (stat.NE.0) GO TO 890

! Printing the header for the example on process number 0
   IF(ya.EQ.0) THEN
      PRINT *, ''
      PRINT *, '              Example of Cluster Poisson Library usage'
      PRINT *, ' *******************************************************************'
      PRINT *, ''
      PRINT *, ' This example computes the solution of 3D Poisson equation'
      PRINT *, '      - u_xx - u_yy - u_zz = f(x,y,z), ax<x<bx, ay<y<by, az<z<bz' 
      PRINT *, ' with the right hand side'
      PRINT *, '     f(x,y,z)=pi*pi*(4/lx**2+4/ly**2+0.25/lz**2)*sin(2*pi*(x-ax)/lx)*'
      PRINT *, '                          *cos(2*pi*(y-ay)/ly)*cos(pi*(z-az)/(2 lz)),'
      PRINT *, ' and with the boundary conditions'
      PRINT *, '     u(ax,y,z)=u(bx,y,z), u_x(ax,y,z)=u_x(bx,y,z)'
      PRINT *, '                                      (periodic boundary conditions),'
      PRINT *, '     u(x,ay,z)=u(x,by,z), u_y(x,ay,z)=u_y(x,by,z)'
      PRINT *, '                                      (periodic boundary conditions),'
      PRINT *, '     u_z(x,y,az)= 0 (Neumann boundary condition),'
      PRINT *, '     u(x,y,bz)= 0 (Dirichlet boundary condition),'
      PRINT *, ' where lx=bx-ax, ly=by-ay, lz=bz-az.'
      PRINT *, ' The exact solution of this problem is the following:'
      PRINT *, 'u(x,y,z)=sin(2*pi*(x-ax)/lx)*cos(2*pi*(y-ay)/ly)*cos(pi*(z-az)/2/lz)'
      PRINT *, ' -------------------------------------------------------------------'
      PRINT *, ' In general, the error should be of order O(1/nx**2+1/ny**2+1/nz**2)'
      PRINT *, ' -------------------------------------------------------------------'
      PRINT *, '                    DOUBLE PRECISION COMPUTATIONS                   '
      PRINT *, ' ==================================================================='
      PRINT *, ''
      PRINT *, ' The number of mesh intervals in x-direction is nx=',nx
      PRINT *, ' The number of mesh intervals in y-direction is ny=',ny
      PRINT *, ' The number of mesh intervals in z-direction is nz=',nz      
      PRINT *, ''
   END IF
   
   CALL MPI_BARRIER(comm, stat)
   IF (stat.NE.0) GO TO 890

! Parameter for the accuracy control 
   tol=tolconst*(1.0D0/nx/nx+1.0D0/ny/ny+1.0D0/nz/nz)
! Parameter for memory allocation
   t=nx/nproc
! Memory allocation      
   ALLOCATE(u((t+1)*ny*(nz+1)),f((t+1)*ny*(nz+1)),work(2*(t+1)*ny*(nz+1)),z(nz+1),&
                                                  zh(nz),zh2(nz),cnts(nproc),STAT=stat)
   IF (stat.NE.0) GO TO 970 

! Computing the length of side lx and the mesh size hx in x-direction
   lx=bx-ax
   hx=lx/nx
! Computing the length of side ly and the mesh size hy in y-direction
   ly=by-ay 
   hy=ly/ny
! Computing the length of side lz and generic step size for non-uniform mesh 
! generation in z-direction
   lz=bz-az
   hz=lz/(1.01D0**nz-1.0D0)
! Non-uniform mesh generation
   DO i=1,nz+1
      z(i)=az+hz*(1.01D0**(i-1)-1.0D0)
   END DO
! Filling the work arrays zh and zh2 according to the BCtype parameter below
   zh(1)=1.0D0/(z(2)-z(1))
   zh2(1)=zh(1)
   DO i=2,nz
      zh(i)=2.0D0/(z(i+1)-z(i-1)) 
      zh2(i)=1.0D0/(z(i+1)-z(i)) 
   END DO

! Exact solution and right hand side
   pi=3.14159265358979324D0
   DO k=1,nz
      DO j=1,ny
         DO i=1,t
            u(k+nz*(j-1)+nz*ny*(i-1))=dsin(2.0D0*pi*(i+ya*t-1)/nx)*&
                                    dcos(2.0D0*pi*(j-1)/ny)*dcos(0.5D0*pi*(z(k)-az)/lz)
            f(k+nz*(j-1)+nz*ny*(i-1))=-pi*pi*(4.0D0/lx**2+4.0D0/ly**2+0.25D0/lz**2)*&
                                                              u(k+nz*(j-1)+nz*ny*(i-1))
         END DO
      END DO
   END DO

! Setting the type of the boundary conditions in z-direction;
! NOTE: in the current implementation of CPSL the value of BCtype parameter is fixed
BCtype='PPPPND'

! Initializing ipar array to make it free from garbage
   DO i=1,256
      ipar(i)=0
   END DO

! Initializing simple data structures of CPSL for 3D Solver
   CALL dmv0_init_helmholtz_3d(ax,bx,ay,by,az,bz,nx,ny,nz,BCtype,ipar,dpar,stat)
   IF (stat.NE.0) GO TO 900

! Requesting CPSL to print errors/warnings in Fortran style
   ipar(22)=0

! ipar(30)=2 requests multiplication of rows in the matrix corresponding to Neumann 
! boundary condition by 2 to get second order approximation. Otherwise, approximation 
! would be of first order. 
   ipar(30)=2  
   
! Initializing complex data structures of Poisson Library for 3D Solver
! NOTE: Right-hand side f may be altered after the Commit step. If you want to keep it,
!       you should save it in another memory location!
   CALL dmv0_commit_helmholtz_3d(f,xhandle1,xhandle2,yhandle1,yhandle2,ipar,dpar,&
                                                                        cnts,comm,stat)
   IF (stat.LT.0) GO TO 900 

! Computing the approximate solution of 3D problem
   CALL dmv0_helmholtz_3d(f,xhandle1,xhandle2,yhandle1,yhandle2,ipar,dpar,zh,zh2,&
                                                                   work,cnts,comm,stat)
   IF (stat.NE.0) GO TO 900
  
! Cleaning the memory used by xhandle1,xhandle2,yhandle1,yhandle2
   CALL dmv0_free_Helmholtz_3D(xhandle1,xhandle2,yhandle1,yhandle2,ipar,stat)
   IF (stat.NE.0) GO TO 900       
        
! Computation of the error on each process
   c1=0.0D0
   DO k = 1,nz+1
      DO j = 1,ny
         DO i = 1,t 
            c2=dabs(u(k+nz*(j-1)+nz*ny*(i-1))-f(k+nz*(j-1)+nz*ny*(i-1)))
            IF (c2.GT.c1) c1=c2
         END DO
      END DO   
   END DO

   PRINT *, 'Process ',ya,' computed solution with error=',c1

! Computation of global error   
   CALL MPI_ALLREDUCE(c1, c2, 1, MPI_DOUBLE_PRECISION, MPI_MAX, comm, stat)
   IF (stat.NE.0) GO TO 890
   IF (ya.EQ.nproc-1) THEN
      PRINT*,''      
      PRINT *,' Global error=',c2
      PRINT *,''
   END IF
   CALL MPI_BARRIER(comm, stat)
   IF (stat.NE.0) GO TO 890
   IF (c2.GT.tol) THEN
      IF (ya.EQ.0) PRINT *,'The computed solution seems to be inaccurate!'
      GO TO 960
   ELSE
      DEALLOCATE(u,f,work,z,zh,zh2,cnts,STAT=stat)
      IF (stat.ne.0) GO TO 970
      CALL MPI_FINALIZE(stat)
      if (stat.NE.0) GO TO 990
      IF (ya.EQ.0) THEN
         PRINT *,'Double precision 3D Poisson FORTRAN example has successfully PASSED'
         PRINT *,'through all steps of computation!' 
         PRINT *,''
      END IF
      IF (ya.EQ.0) THEN 
         STOP 0
      ELSE
         STOP
      ENDIF
   END IF

! Failure message to print if something went wrong
880   CONTINUE
      PRINT *,''
      PRINT *,'Failed to initilize MPI! Returned status=',stat
      GO TO 999

890   CONTINUE
      PRINT *,''
      PRINT *,'MPI routine terminated with non-zero status=',stat
      CALL MPI_ABORT(comm,stat)
      GO TO 980
            
900   CONTINUE 
      PRINT *,''
      PRINT *,'CPSL routine exited with non-zero status=', stat
      CALL MPI_ABORT(comm,stat)

960   CONTINUE
      DEALLOCATE(u,f,work,cnts,STAT=stat)
      GO TO 980

970   CONTINUE
      PRINT *,''
      PRINT *,'Memory (de)allocation routine exited with non-zero status=', stat
      CALL MPI_ABORT(comm,stat)
      
980   CONTINUE
      CALL MPI_FINALIZE(stat)
      IF (stat.ne.0) GO TO 990
      GO TO 999

990   CONTINUE
      PRINT *,''
      PRINT *,'Failed to finalize MPI! Returned status=',stat

999   CONTINUE
      IF (ya.EQ.0) THEN
         PRINT *,'Double precision 3D Poisson FORTRAN example FAILED to compute'
         PRINT *,'the solution...'
         PRINT *,''
         STOP 1
      ELSE 
         STOP
      END IF

END
!************************* End of Fortran code example *************************