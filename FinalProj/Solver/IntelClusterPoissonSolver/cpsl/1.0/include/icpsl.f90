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
!*******************************************************************************

MODULE ICPSL

USE MKL_DFTI

INTERFACE 

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!INTERFACES FOR 2D CASE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


SUBROUTINE DMV0_INIT_HELMHOLTZ_2D(AX,BX,AY,BY,NX,NY,BCTYPE,Q,IPAR,DPAR,STAT)
   USE MKL_DFTI
   
   INTEGER NX, NY, STAT
   INTEGER IPAR(*)
   DOUBLE PRECISION AX,BX,AY,BY,Q
   CHARACTER(4) BCTYPE
   DOUBLE PRECISION DPAR(*) 
END SUBROUTINE

!---------------------------------------------------------------------

SUBROUTINE DMV0_COMMIT_HELMHOLTZ_2D(F,BD_AX,BD_BX,BD_AY,BD_BY,HANDLE,IPAR,DPAR,CNTS,COMM,STAT)
   USE MKL_DFTI
   
   INTEGER STAT,COMM
   INTEGER IPAR(*),CNTS(*)
   DOUBLE PRECISION F(IPAR(11)+1,*)   
   DOUBLE PRECISION BD_AX(*),BD_BX(*),BD_AY(*),BD_BY(*)
   DOUBLE PRECISION DPAR(*) 
   TYPE(DFTI_DESCRIPTOR), POINTER :: HANDLE
END SUBROUTINE

!---------------------------------------------------------------------

SUBROUTINE DMV0_HELMHOLTZ_2D(F,BD_AX,BD_BX,BD_AY,BD_BY,HANDLE,IPAR,DPAR,WORK,CNTS,COMM,STAT)
   USE MKL_DFTI
   
   INTEGER STAT,COMM
   INTEGER IPAR(*),CNTS(*)
   DOUBLE PRECISION F(IPAR(11)+1,*), WORK(*)   
   DOUBLE PRECISION BD_AX(*),BD_BX(*),BD_AY(*),BD_BY(*)
   DOUBLE PRECISION DPAR(*)
   TYPE(DFTI_DESCRIPTOR), POINTER :: HANDLE
END SUBROUTINE

!---------------------------------------------------------------------

SUBROUTINE DMV0_FREE_HELMHOLTZ_2D(HANDLE,IPAR,STAT)
   USE MKL_DFTI
   
   INTEGER STAT
   INTEGER IPAR(*)
   TYPE(DFTI_DESCRIPTOR), POINTER :: HANDLE
END SUBROUTINE


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!INTERFACES FOR 3D CASE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!---------------------------------------------------------------------

SUBROUTINE DMV0_INIT_HELMHOLTZ_3D(AX,BX,AY,BY,AZ,BZ,NX,NY,NZ,BCTYPE,IPAR,DPAR,STAT)
   USE MKL_DFTI
   
   INTEGER NX, NY, NZ, STAT
   INTEGER IPAR(*)
   CHARACTER(6) BCTYPE     
   DOUBLE PRECISION AX,BX,AY,BY,AZ,BZ,Q
   DOUBLE PRECISION DPAR(*) 
END SUBROUTINE

!---------------------------------------------------------------------

SUBROUTINE DMV0_COMMIT_HELMHOLTZ_3D(F,XHANDLE1,XHANDLE2,YHANDLE1,YHANDLE2,IPAR,DPAR,CNTS,COMM,STAT)
   USE MKL_DFTI
   
   INTEGER STAT, COMM
   INTEGER IPAR(*), CNTS(*)
   DOUBLE PRECISION F(*)   
   DOUBLE PRECISION DPAR(*) 
   TYPE(DFTI_DESCRIPTOR), POINTER :: XHANDLE1,XHANDLE2,YHANDLE1,YHANDLE2
END SUBROUTINE

!---------------------------------------------------------------------

SUBROUTINE DMV0_HELMHOLTZ_3D(F,XHANDLE1,XHANDLE2,YHANDLE1,YHANDLE2,IPAR,DPAR,DDZU,DDZW,WORK,CNTS,COMM,STAT)
   USE MKL_DFTI
   
   INTEGER STAT,COMM
   INTEGER IPAR(*),CNTS(*)
   DOUBLE PRECISION F(*),WORK(*)
   DOUBLE PRECISION DPAR(*),DDZU(*),DDZW(*)
   TYPE(DFTI_DESCRIPTOR), POINTER :: XHANDLE1,XHANDLE2,YHANDLE1,YHANDLE2
END SUBROUTINE

!---------------------------------------------------------------------

SUBROUTINE DMV0_FREE_HELMHOLTZ_3D(XHANDLE1,XHANDLE2,YHANDLE1,YHANDLE2,IPAR,STAT)
   USE MKL_DFTI

   INTEGER IPAR(*),STAT
   TYPE(DFTI_DESCRIPTOR), POINTER :: XHANDLE1,XHANDLE2,YHANDLE1,YHANDLE2
END SUBROUTINE

END INTERFACE
END MODULE
