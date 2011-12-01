/*******************************************************************************
!                             INTEL CONFIDENTIAL
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
!*******************************************************************************/
#ifndef _ICPSL_H_
#define _ICPSL_H_

/* definitions of types */ 
#include "mkl_dfti.h"
#include "mpi.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**/
#if defined( _WIN32 ) || ( _WIN64 )

#define   dmv0_init_helmholtz_2d   DMV0_INIT_HELMHOLTZ_2D
#define dmv0_commit_helmholtz_2d DMV0_COMMIT_HELMHOLTZ_2D
#define        dmv0_helmholtz_2d        DMV0_HELMHOLTZ_2D
#define   dmv0_free_helmholtz_2d   DMV0_FREE_HELMHOLTZ_2D

#define   dmv0_init_helmholtz_3d   DMV0_INIT_HELMHOLTZ_3D
#define dmv0_commit_helmholtz_3d DMV0_COMMIT_HELMHOLTZ_3D
#define        dmv0_helmholtz_3d        DMV0_HELMHOLTZ_3D
#define   dmv0_free_helmholtz_3d   DMV0_FREE_HELMHOLTZ_3D

#else

#define   dmv0_init_helmholtz_2d   dmv0_init_helmholtz_2d_
#define dmv0_commit_helmholtz_2d dmv0_commit_helmholtz_2d_
#define        dmv0_helmholtz_2d        dmv0_helmholtz_2d_
#define   dmv0_free_helmholtz_2d   dmv0_free_helmholtz_2d_

#define   dmv0_init_helmholtz_3d   dmv0_init_helmholtz_3d_
#define dmv0_commit_helmholtz_3d dmv0_commit_helmholtz_3d_
#define        dmv0_helmholtz_3d        dmv0_helmholtz_3d_
#define   dmv0_free_helmholtz_3d   dmv0_free_helmholtz_3d_

#endif
/**/


void   dmv0_init_helmholtz_2d(double*,double*,double*,double*,int*,int*,char*,double*,int*,double*,int*);
void dmv0_commit_helmholtz_2d(double*,double*,double*,double*,double*,DFTI_DESCRIPTOR_HANDLE*,int*,double*,int*,MPI_Comm*,int*);
void        dmv0_helmholtz_2d(double*,double*,double*,double*,double*,DFTI_DESCRIPTOR_HANDLE*,int*,double*,double*,int*,MPI_Comm*,int*);
void   dmv0_free_helmholtz_2d(DFTI_DESCRIPTOR_HANDLE*,int*,int*);

void   dmv0_init_helmholtz_3d(double*,double*,double*,double*,double*,double*,int*,int*,int*,char*,int*,double*,int*);
void dmv0_commit_helmholtz_3d(double*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,int*,double*,int*,MPI_Comm*,int*);
void        dmv0_helmholtz_3d(double*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,int*,double*,double*,double*,double*,int*,MPI_Comm*,int*);
void   dmv0_free_helmholtz_3d(DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,DFTI_DESCRIPTOR_HANDLE*,int*,int*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ICPSL_H_ */
