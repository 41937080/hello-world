#!/bin/bash -xe

OS_SYSTEM="win7"

export XILINX_SDK=C:/Xilinx/SDK/2017.2
: ${XILINX_SDK?"Need to set path to Xilinx SDK, example: export XILINX_SDK=/opt/Xilinx/SDK/2017.2"}


# Where to find the HDF
HDF_FILE=../hw_rep/hdf/system_wrapper.hdf



BUILD_CONFIG="Debug"

#CAMERA_MODEL=$1

#if [ "${CAMERA_MODEL}" == "x1dmk2" ]; then
#    BUILD_CONFIG="Debug"
#elif [ "${CAMERA_MODEL}" == "x2d" ]; then
#    BUILD_CONFIG="Debug"
#else
#    echo "Unkown camera model"
#    exit 1
#fi


# Remove workspace metadata
rm -rf .metadata/

# Setup Xilinx SDK
source ${XILINX_SDK}/settings64.sh

# import project into workspace
if [ "${OS_SYSTEM}" == "win7" ]; then
	${XILINX_SDK}/eclipse/win64.o/eclipse -nosplash \
	-application org.eclipse.cdt.managedbuilder.core.headlessbuild -import ./app \
	-data . -vmargs -Dorg.eclipse.cdt.core.console=org.eclipse.cdt.core.systemConsole
else	
	${XILINX_SDK}/eclipse/lnx64.o/eclipse -nosplash \
	-application org.eclipse.cdt.managedbuilder.core.headlessbuild -import ./app \
	-data . -vmargs -Dorg.eclipse.cdt.core.console=org.eclipse.cdt.core.systemConsole
fi

#if [ "${OS_SYSTEM}" == "win7" ]; then
#	${XILINX_SDK}/eclipse/win64.o/eclipse -nosplash \
#	-application org.eclipse.cdt.managedbuilder.core.headlessbuild -import ./sample_helloworld\
#	-data . -vmargs -Dorg.eclipse.cdt.core.console=org.eclipse.cdt.core.systemConsole
#else	
#	${XILINX_SDK}/eclipse/lnx64.o/eclipse -nosplash \
#	-application org.eclipse.cdt.managedbuilder.core.headlessbuild -import ./sample_helloworld \
#	-data . -vmargs -Dorg.eclipse.cdt.core.console=org.eclipse.cdt.core.systemConsole
#fi


# Remove previous projects
rm -rf app_hw/
rm -rf app_bsp/

# Create HW, BSP and FSBL projects
${XILINX_SDK}/bin/xsdk -batch -source build.tcl -tclargs $HDF_FILE

rm SDK.log

# build application
#${XILINX_SDK}/eclipse/lnx64.o/eclipse -nosplash \
#-application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "app/""${BUILD_CONFIG}" \
#-data . -vmargs \
#-Dorg.eclipse.cdt.core.console=org.eclipse.cdt.core.systemConsole \

# build fsbl
#${XILINX_SDK}/eclipse/lnx64.o/eclipse -nosplash \
#-application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild fsbl \
#-data . -vmargs \
#-Dorg.eclipse.cdt.core.console=org.eclipse.cdt.core.systemConsole \
#-DFSBL_DEBUG_INFO=1

# Create a bif file ()boot image file )
#./create_bif.sh "${BUILD_CONFIG}"

# build the bootimage artifact
#${XILINX_SDK}/bin/bootgen -image boot_image.bif -o bootimage.mcs -w on
#${XILINX_SDK}/bin/bootgen -image boot_image.bif -o bootimage.bin -w on

# split bin file for dual parallel qspi programming
#gcc -o tools/bitsplit tools/bitsplit.c
#./tools/bitsplit ./bootimage.bin ./bootimage_even.bin ./bootimage_odd.bin
