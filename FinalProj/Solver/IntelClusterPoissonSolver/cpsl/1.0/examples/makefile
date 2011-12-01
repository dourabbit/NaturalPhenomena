##******************************************************************************
##                              INTEL CONFIDENTIAL
##  Copyright(C) 2008 Intel Corporation. All Rights Reserved.
##  The source code contained  or  described herein and all documents related to
##  the source code ("Material") are owned by Intel Corporation or its suppliers
##  or licensors.  Title to the  Material remains with  Intel Corporation or its
##  suppliers and licensors. The Material contains trade secrets and proprietary
##  and  confidential  information of  Intel or its suppliers and licensors. The
##  Material  is  protected  by  worldwide  copyright  and trade secret laws and
##  treaty  provisions. No part of the Material may be used, copied, reproduced,
##  modified, published, uploaded, posted, transmitted, distributed or disclosed
##  in any way without Intel's prior express written permission.
##  No license  under any  patent, copyright, trade secret or other intellectual
##  property right is granted to or conferred upon you by disclosure or delivery
##  of the Materials,  either expressly, by implication, inducement, estoppel or
##  otherwise.  Any  license  under  such  intellectual property  rights must be
##  express and approved by Intel in writing.
##
##******************************************************************************
##  Content:
##      Intel(R) Cluster Posson Solver Library examples creation and run
##
##******************************************************************************

all:
	@echo '====================================================================='
	@echo 'This is makefile to run Intel(R) CPSL examples'
	@echo '---------------------------------------------------------------------'
	@echo 'Usage:'
	@echo 'make {impi|ompi|mvapich} [NP=nproc] [examples=example_names] [MKL_PATH=mkl_path]'
	@echo '+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
	@echo 'Target impi assumes that your cluster has properly set environment'
	@echo 'for Intel(R) MPI and C/C++ and/or Fortran Compiler'
	@echo 'Target ompi assumes that your cluster has properly set environment'
	@echo 'for Open MPI* and Intel(R) C/C++ and/or Fortran Compiler'
	@echo 'Target mvapich assumes that your cluster has properly set environment'
	@echo 'for MVAPICH* and Intel(R) C/C++ and/or Fortran Compiler'
	@echo '+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
	@echo 'Although the CPSL might work well with other MPI and Compilers, we'
	@echo 'suggest to use the following versions of software:'
	@echo 'Intel(R) MPI 3.1.038 or higher (run mpdboot prior to running the'
	@echo 'examples!)'
	@echo 'Intel(R) Compilers 10.1.017 or higher'
	@echo 'Open MPI* 1.2.6 or higher built with the above compilers'
	@echo 'MVAPICH2* 1.2 or higher built with the above compilers (run mpdboot'
	@echo 'prior to running the examples!)'
	@echo '+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
	@echo 'nproc sets the number of processes to run for the example'
	@echo 'Default value for nproc is 4'
	@echo '+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
	@echo 'Default value for examples=all'
	@echo 'Admissible values for example_names are all, d_helmholtz_2d_f,'
	@echo 'd_helmholtz_2d_c, d_helmholtz_3d_f, and d_helmholtz_3d_c'
	@echo '+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
	@echo 'Please make sure that your cluster has properly set environment for'
	@echo 'Intel(R) MKL and you set the correct path to MKL home folder either'
	@echo 'at line 65 of this makefile or through mkl_path!'
	@echo 'Please make sure that you have Intel(R) MKL 10.0 Update 1 or higher'
	@echo '---------------------------------------------------------------------'
	@echo '* Other brands and names may be claimed as property of others'
	@echo '---------------------------------------------------------------------'
	@echo 'Example: make impi MKL_PATH=/opt/intel/mkl/10.0.3.021'
	@echo 'Runs 4 available examples using Intel compilers and MKL 10.0 Update 3'
	@echo '====================================================================='

## Place here the correct pathes to the local Intel(R) MKL folders
MKL_PATH?=<local path to MKL folder>
MKL_INCLUDE=$(MKL_PATH)/include

ifeq ($(NP),)
  NP=4
endif

ifeq ($(examples),)
  examples=all
endif

ifeq ($(examples),all)
  SOURCES=d_helmholtz_2d_f.f90 d_helmholtz_2d_c.c d_helmholtz_3d_f.f90 d_helmholtz_3d_c.c
  examples=d_helmholtz_2d_f d_helmholtz_2d_c d_helmholtz_3d_f d_helmholtz_3d_c
else
  SOURCES=$(subst d_f,d_f.f90,$(subst d_c,d_c.c,$(examples)))
endif

examples_c=$(filter %_c,$(examples))
examples_f90=$(filter %_f,$(examples))

OBJS= $(subst .f90,.o,$(subst .c,.o,$(SOURCES)))

compiler=intel

CPSL_INCLUDE=../include
CPSL_LIB=../../../lib/intel64

## Intel(R) MPI C/C++ and Fortran compilers
ICC = mpiicc
IFORT = mpiifort
## OpenMPI* and MVAPICH* C/C++ and Fortran compiler wrappers
## They should wrap up appropriate Intel compilers
OCC = mpicc
OFORT = mpif90
ACC = mpicc
AFORT = mpif90

CFLAGS = -O3 -I../../$(CPSL_INCLUDE)
FFLAGS = -O3 

FOLDER=_results/foo_$(compiler)

  ILIB=-openmp $(CPSL_LIB)/libcpsl_impi_s.a     -L$(MKL_PATH)/lib/em64t \
    -Wl,--start-group -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -Wl,--end-group \
    -liomp5 -mt_mpi -lm -lpthread

  OLIBC=        $(CPSL_LIB)/libcpsl_ompi_c_s.a     -L$(MKL_PATH)/lib/em64t \
    -Wl,--start-group -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -Wl,--end-group \
    -liomp5 -lmpi_f77 -lm -lpthread
  OLIBF=        $(CPSL_LIB)/libcpsl_ompi_f_s.a     -L$(MKL_PATH)/lib/em64t \
    -Wl,--start-group -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -Wl,--end-group \
    -liomp5 -lmpi_f77 -lm -lpthread

  ALIB=        $(CPSL_LIB)/libcpsl_mvapich_s.a -L$(MKL_PATH)/lib/em64t \
    -Wl,--start-group -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core  -Wl,--end-group \
    -liomp5 -lm -lpthread

  INC=$(MKL_INCLUDE)/mkl_dfti.f90 $(CPSL_INCLUDE)/icpsl.f90
  INCF=mkl_dfti.f90 icpsl.f90
  
.SUFFIXES: .f90 .c

vpath %.f90 source
vpath %.c source

impi:
	mkdir -p ./$(subst foo,impi,$(FOLDER))
	$(MAKE) chkenv F90=$(IFORT) CC=$(ICC) TARGET=impi
	$(MAKE) cr examples="$(examples)" F90=$(IFORT) CC=$(ICC) FOLDER=$(subst foo,impi,$(FOLDER)) LIB="$(ILIB)" TARGET=impi compiler=$(compiler)
	$(MAKE) cleanup FOLDER=$(subst foo,impi,$(FOLDER))

ompi:
	echo $$LD_LIBRARY_PATH
	mkdir -p ./$(subst foo,ompi,$(FOLDER))
	$(MAKE) chkenv F90=$(OFORT) CC=$(OCC) TARGET=ompi
	echo $(examples_c)
	echo $(examples_f90)
ifneq ($(examples_c),)
	$(MAKE) cr examples="$(examples_c)" CC=$(OCC) FOLDER=$(subst foo,ompi,$(FOLDER)) LIB="$(OLIBC)" TARGET=ompi compiler=$(compiler)
endif
ifneq ($(examples_f90),)
	$(MAKE) cr examples="$(examples_f90)" F90=$(OFORT) FOLDER=$(subst foo,ompi,$(FOLDER)) LIB="$(OLIBF)" TARGET=ompi compiler=$(compiler)
endif
	$(MAKE) cleanup FOLDER=$(subst foo,ompi,$(FOLDER))

mvapich:
	mkdir -p ./$(subst foo,mvapich,$(FOLDER))
	$(MAKE) chkenv F90=$(AFORT) CC=$(ACC) TARGET=mvapich
	$(MAKE) cr examples="$(examples)" F90=$(AFORT) CC=$(ACC) FOLDER=$(subst foo,mvapich,$(FOLDER)) LIB="$(ALIB)" TARGET=mvapich compiler=$(compiler)
	$(MAKE) cleanup FOLDER=$(subst foo,mvapich,$(FOLDER))

chkf:
	@echo 'Looking for $(F90):'
	@((which $(F90)) && (echo '$(F90) found!') || \
	(echo ''; \
	 echo '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'; \
	 echo 'Aborting... Could you please set environment for $(F90) needed for target "$(TARGET)" first?'; \
	 echo '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'; \
	 echo ''; \
	 $(MAKE) abort));

chkc:
	@echo 'Looking for $(CC):'
	@((which $(CC)) && (echo '$(CC) found!') || \
	(echo ''; \
	 echo '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'; \
	 echo 'Aborting... Could you please set environment for $(CC) needed for target "$(TARGET)" first?'; \
	 echo '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'; \
	 echo ''; \
	 $(MAKE) abort));

chkmkl:
	@echo ''
	@echo 'Looking for Intel(R) MKL:'
ifeq ($(MKL_PATH),<local path to MKL folder>)
	@echo ''
	@echo '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
	@echo 'We have assumed that you either changed line 61 of this makefile or provided the path to your'
	@echo 'local Intel(R) MKL installation via MKL_PATH=mkl_path variable...' 
	@echo 'Aborting... Could you please set local path to Intel(R) MKL at line 61 of this makefile or'
	@echo 'call this makefile with MKL_PATH=mkl_path'
	@echo '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
	@echo ''
	$(MAKE) abort
else
	@echo 'Using Intel(R) MKL from $(MKL_PATH) to run examples!'
endif

chkenv:
	@echo ''
	@echo 'Checking if environment is set properly...'
	@echo ''
ifneq ($(filter %.f90,$(SOURCES)),)
	$(MAKE) chkf F90="$(F90)" TARGET="$(TARGET)"
endif
ifneq ($(filter %.c,$(SOURCES)),)
	$(MAKE) chkc CC="$(CC)" TARGET="$(TARGET)"
endif
	$(MAKE) chkmkl

abort:
	@echo ''
	@echo '================================================================================'
	@echo 'Sorry, unable to complete the task properly... See the output above for details.'
	@echo '================================================================================'
	@echo ''
	$(error )

cr: cleanup $(OBJS)
	cd ./$(FOLDER); rm -f *.c *.f90

.f90.o:
	which $(F90)
	cp -f $(INC) ./$(FOLDER)
	cp -f $< ./$(FOLDER)
	cd ./$(FOLDER); $(F90) -c $(INCF)
	cd ./$(FOLDER); $(F90) $(FFLAGS) -o ./$(subst source/,,$(subst .f90,.out,$<)) ../../$< $(LIB)
	cd ./$(FOLDER); export LD_LIBRARY_PATH=$(MKL_PATH)/lib/em64t:$$LD_LIBRARY_PATH; \
	mpiexec -np $(NP) ./$(subst source/,,$(subst .f90,.out,$<)) > ./$(subst source/,,$(subst .f90,.res,$<)) 2>&1

.c.o:
	which $(CC)
	cp -f $< ./$(FOLDER)
	cd ./$(FOLDER); $(CC) $(CFLAGS) -I$(MKL_PATH)/include -c -o ./$(subst source/,,$(subst .c,.o,$<)) ../../$<; \
	$(CC) -o ./$(subst source/,,$(subst .c,.out,$<)) ./$(subst source/,,$(subst .c,.o,$<)) $(LIB)
	cd ./$(FOLDER); export LD_LIBRARY_PATH=$(MKL_PATH)/lib/em64t:$$LD_LIBRARY_PATH; \
	mpiexec -np $(NP) ./$(subst source/,,$(subst .c,.out,$<)) > ./$(subst source/,,$(subst .c,.res,$<)) 2>&1

cleanup:
	cd ./$(FOLDER); rm -f *.o *.mod $(INCF)
