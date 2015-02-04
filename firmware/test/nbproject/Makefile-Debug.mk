#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1472/ff.o \
	${OBJECTDIR}/_ext/1472/flashee-eeprom.o \
	${OBJECTDIR}/CircularBufferTest.o \
	${OBJECTDIR}/DevicesTest.o \
	${OBJECTDIR}/FSTest.o \
	${OBJECTDIR}/FakeFlashDeviceTest.o \
	${OBJECTDIR}/FlashDeviceRegionTest.o \
	${OBJECTDIR}/FlashDeviceTest.o \
	${OBJECTDIR}/LogicalPageMapperTest.o \
	${OBJECTDIR}/MultiWriteFlashStoreTest.o \
	${OBJECTDIR}/PageSpanFlashDeviceTest.o \
	${OBJECTDIR}/SinglePageWearTest.o \
	${OBJECTDIR}/gmock-gtest-all.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-static-libgcc -static-libstdc++ -std=gnu++11
CXXFLAGS=-static-libgcc -static-libstdc++ -std=gnu++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1472/ff.o: ../ff.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1472
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1472/ff.o ../ff.cpp

${OBJECTDIR}/_ext/1472/flashee-eeprom.o: ../flashee-eeprom.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1472
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1472/flashee-eeprom.o ../flashee-eeprom.cpp

${OBJECTDIR}/CircularBufferTest.o: CircularBufferTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CircularBufferTest.o CircularBufferTest.cpp

${OBJECTDIR}/DevicesTest.o: DevicesTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DevicesTest.o DevicesTest.cpp

${OBJECTDIR}/FSTest.o: FSTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FSTest.o FSTest.cpp

${OBJECTDIR}/FakeFlashDeviceTest.o: FakeFlashDeviceTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FakeFlashDeviceTest.o FakeFlashDeviceTest.cpp

${OBJECTDIR}/FlashDeviceRegionTest.o: FlashDeviceRegionTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FlashDeviceRegionTest.o FlashDeviceRegionTest.cpp

${OBJECTDIR}/FlashDeviceTest.o: FlashDeviceTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FlashDeviceTest.o FlashDeviceTest.cpp

${OBJECTDIR}/LogicalPageMapperTest.o: LogicalPageMapperTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LogicalPageMapperTest.o LogicalPageMapperTest.cpp

${OBJECTDIR}/MultiWriteFlashStoreTest.o: MultiWriteFlashStoreTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MultiWriteFlashStoreTest.o MultiWriteFlashStoreTest.cpp

${OBJECTDIR}/PageSpanFlashDeviceTest.o: PageSpanFlashDeviceTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PageSpanFlashDeviceTest.o PageSpanFlashDeviceTest.cpp

${OBJECTDIR}/SinglePageWearTest.o: SinglePageWearTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SinglePageWearTest.o SinglePageWearTest.cpp

${OBJECTDIR}/gmock-gtest-all.o: gmock-gtest-all.cc 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gmock-gtest-all.o gmock-gtest-all.cc

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I. -I../../../core-firmware/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/test.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
