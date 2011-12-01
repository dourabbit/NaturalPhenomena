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
!  This Fortran example computes the solution of a boundary value problem for 2D 
!  Helmholtz problem in a rectangular domain with the mixed Neumann-Dirichlet 
!  boundary conditions. Example details are printed during the example run.
!
!*******************************************************************************

PROGRAM CPSL_HELMHOLTZ_2D_EXAMPLE_F

USE ICPSL

IMPLICIT NONE

INCLUDE "mpif.h"

! Number of intervals in x and y directions
INTEGER nx,ny
PARAMETER(nx=111, ny=222)

! Problem parameters
DOUBLE PRECISION tolconst, ax, bx, ay, by, q
! Tolerance parameter for error control
! Example may fail to complete properly if the tolconst has too small value
PARAMETER(tolconst=2.0D0)
! Boundaries of rectangular domain 
PARAMETER(ax=0.0D0, bx=1.0D0, ay=0.0D0, by=1.0D0)
! Helmholtz coefficient
PARAMETER(q=1.0D0)

! Some integer parameters
INTEGER ix,iy,i,stat,ya,comm,nproc,t1,t2,t3,ny_start,ny_lenght,temp
! It is highly recommended to declare ipar array of size 256
! for compatibility with future versions of CPSL
INTEGER ipar(256)
! Integer work array
INTEGER, POINTER :: cnts(:)
! Some real parameters
DOUBLE PRECISION lx,ly,hx,hy,c1,c2,tol,pi
! dpar array has the size indicated in the Manual
DOUBLE PRECISION dpar(13*nx/2+7)
! Array u contains the solution of the original differential problem,
! Array f contains the right hand side and the solution of the grid problem 
! Array work is used to store intermediate results
DOUBLE PRECISION, POINTER :: u(:,:), f(:,:), work(:)
! Non-homogenious boundary conditions will be stored in the arrays below
DOUBLE PRECISION bd_ax(ny+1), bd_bx(ny+1), bd_ay(nx+1), bd_by(nx+1)
! Data structure used by CPSL
TYPE(DFTI_DESCRIPTOR), POINTER :: xhandle
! Array to store the types of boundary conditions
CHARACTER(4) BCtype

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
      PRINT *, ' This example computes the solution of 2D Helmholtz equation'
      PRINT *, '      - u_xx - u_yy + qu = f(x,y), ax<x<bx, ay<y<by'
      PRINT *, ' with the right hand side'
      PRINT *, ' f(x,y)=(pi*pi(1/lx**2+1/ly**2)+q)*sin(pi*(x-ax)/lx)*'
      PRINT *, '                                                  sin(pi*(y-ay)/ly)+q,'
      PRINT *, ' and with the boundary conditions'
      PRINT *, '      u(ax,y)=u(bx,y)=1 (Dirichlet boundary conditions),'
      PRINT *, '      u_y(x,ay)= pi/ly*sin(pi*(x-ax)/lx) (Neumann boundary condition),'
      PRINT *, '      u_y(x,by)=-pi/ly*sin(pi*(x-ax)/lx) (Neumann boundary condition),'
      PRINT *, ' where lx=bx-ax, ly=by-ay.'
      PRINT *, ' The exact solution of this problem is the following:'
      PRINT *, '      u(x,y) = sin(pi*(x-ax)/lx)*sin(pi*(y-ay)/ly)+1'
      PRINT *, ' -------------------------------------------------------------------'
      PRINT *, ' In general, the error should be of order O(1.0/nx**2+1.0/ny**2)'
      PRINT *, ' -------------------------------------------------------------------'
      PRINT *, '                    DOUBLE PRECISION COMPUTATIONS                   '
      PRINT *, ' ==================================================================='
      PRINT *, ''
      PRINT *, ' The number of mesh intervals in x-direction is nx=',nx
      PRINT *, ' The number of mesh intervals in y-direction is ny=',ny
      PRINT *, ''
   END IF

   CALL MPI_BARRIER(comm, stat)
   IF (stat.NE.0) GO TO 890
! Parameter for the control of accuracy  
   tol=tolconst*(1.0D0/nx/nx+1.0D0/ny/ny)
! Parameters for memory allocation
   t1=((ny+1)/nproc+1)
   t2=(((nx+1)/nproc+1)*(ny+1))/(nx+1)+1
   IF(t2.lt.t1) t2=t1
   t3=(nx+1)*t2
   IF (t3.lt.(2*ny+2)) t3=2*(ny+1)
! Memory allocation   
   ALLOCATE(u(nx+1,t1),f(nx+1,t2),work(t3),cnts(4*nproc),STAT=stat)
   IF (stat.NE.0) GO TO 970 
      
! Computing the length of side lx and the mesh size hx in x-direction
   lx=bx-ax
   hx=lx/nx
! Computing the length of side ly and the mesh size hy in y-direction
   ly=by-ay 
   hy=ly/ny

   temp=(ny+1)-nproc*((ny+1)/nproc)
   IF (ya.LT.temp) THEN
      ny_start=1+ya*(1+(ny+1)/nproc)
      ny_lenght=(1+(ny+1)/nproc)
   ELSE
      ny_start=1+temp*(1+(ny+1)/nproc)+(ya-temp)*(((ny+1)/nproc))
      ny_lenght=(ny+1)/nproc
   END IF

! Filling in the values of the TRUE solution u(x,y)=sin(pi*(x-ax))*sin(pi*(y-ay))+1
! in the mesh points into the array u
! Filling in the values of right-hand side 
! f(x,y)=(pi*pi*(1/lx**2+1/ly**2)+q)*sin(pi*(x-ax))*sin(pi*(y-ay))+q
! in the mesh points into the array f
! We choose the right-hand side to correspond to the TRUE solution of 
! Helmholtz equation
   pi=3.14159265358979324D0
   DO iy=ny_start,ny_start+(ny_lenght-1)
      DO ix=1,nx+1
         c1=dsin(pi*hx*(ix-1)/lx)
         c2=dsin(pi*hy*(iy-1)/ly)
         c1=c1*c2
         c2=1.0D0/lx**2+1.0D0/ly**2
         f(ix,iy-ny_start+1)=(c2*pi*pi+q)*c1+q
         u(ix,iy-ny_start+1)=c1+1.0D0
      END DO
   END DO

! Setting the type of the boundary conditions on each side of the rectangular domain:
! On the boundary laying on the line x=ax Dirichlet boundary condition will be used
! On the boundary laying on the line x=bx Dirichlet boundary condition will be used
! On the boundary laying on the line y=ay Neumann boundary condition will be used
! On the boundary laying on the line y=by Neumann boundary condition will be used
   BCtype = 'DDNN'

! Setting the boundary values that correspond to the TRUE solution
! in the mesh points laying on Dirichlet boundaries
   DO iy = 1,ny+1
      bd_ax(iy)=1.0D0  
      bd_bx(iy)=1.0D0
   END DO
! Setting the boundary values that are equal to the normal derivative
! of the TRUE solution in the mesh points laying on Neumann boundaries.
! NOTE: at y=ay bd_ay=-u_y(x,ay)
   DO ix=1,nx+1
      bd_ay(ix)=-pi*dsin(pi*(ix-1)/nx)/ly 
      bd_by(ix)=-pi*dsin(pi*(ix-1)/nx)/ly 
   END DO

! Initializing ipar array to make it free from garbage
   DO i=1,256
      ipar(i)=0
   END DO

! Initializing simple data structures of Poisson Library for 2D Helmholtz Solver
   CALL dmv0_init_helmholtz_2d(ax,bx,ay,by,nx,ny,BCtype,q,ipar,dpar,stat)
   IF (stat.NE.0) GO TO 900
   
! Requesting CPSL to print errors/warnings in Fortran style
   ipar(22)=0

! Initializing complex data structures of Poisson Library for 2D Helmholtz Solver
! NOTE: Right-hand side f may be altered after the Commit step. If you want to keep it,
! you should save it in another memory location!
   CALL dmv0_commit_helmholtz_2d(f,bd_ax,bd_bx,bd_ay,bd_by,xhandle,ipar,dpar,cnts,&
                                                                             comm,stat)
   IF (stat.LT.0) GO TO 900

! Computing the approximate solution of 2D Helmholtz problem
! NOTE: Boundary data stored in the arrays bd_ax, bd_bx, bd_ay, bd_by should not 
! be changed between the Commit step and the subsequent call to the Solver routine!
! Otherwise the results may be wrong.
   CALL dmv0_helmholtz_2d(f,bd_ax,bd_bx,bd_ay,bd_by,xhandle,ipar,dpar,work,cnts,&
                                                                             comm,stat)
   IF (stat.NE.0) GO TO 900

! Cleaning the memory used by xhandle
   CALL dmv0_free_Helmholtz_2D(xhandle, ipar, stat)
   IF (stat.NE.0) GO TO 900
! Now we may use xhandle to solve another 2D Helmholtz problem

! Computation of the error on each processor
   c1=0.0D0
   DO ix=1,nx+1 
      DO iy=1,ny+1
         IF((iy.ge.ny_start).and.(iy.le.ny_start+ny_lenght-1)) THEN
            c2=dabs(f(ix,iy-ny_start+1)-u(ix,iy-ny_start+1))            
            IF (c2.GT.c1) c1=c2
         END IF
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
      DEALLOCATE(u,f,work,cnts,STAT=stat)
      IF (stat.ne.0) GO TO 970
      CALL MPI_FINALIZE(stat)
      if (stat.NE.0) GO TO 990
      IF (ya.EQ.0) THEN
         PRINT *,'Double precision 2D Helmholtz FORTRAN example has successfully'
         PRINT *,'PASSED through all steps of computation!' 
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
         PRINT *,'Double precision 2D Helmholtz FORTRAN example FAILED to '
         PRINT *,'compute the solution...'
         PRINT *,''
         STOP 1
      ELSE 
         STOP
      END IF
      
END
!************************* End of Fortran code example *************************
