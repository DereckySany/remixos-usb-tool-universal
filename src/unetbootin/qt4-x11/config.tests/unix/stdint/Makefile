#############################################################################
# Makefile for building: stdint
# Generated by qmake (2.01a) (Qt 4.5.0) on: Thu Mar 5 18:25:48 2009
# Project:  stdint.pro
# Template: app
# Command: /afs/csail.mit.edu/u/g/gkovacs/public/qt4-x11/bin/qmake -nocache -spec ../../../mkspecs/linux-g++ -unix CONFIG+=\ release\ static\ largefile\ stl\ precompile_header\ mmx\ 3dnow\ sse\ sse2 LIBS*= INCLUDEPATH*= QMAKE_CXXFLAGS*= -o Makefile stdint.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE
CFLAGS        = -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W $(DEFINES)
INCPATH       = -I../../../mkspecs/linux-g++ -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)   
AR            = ar cqs
RANLIB        = 
QMAKE         = /afs/csail.mit.edu/u/g/gkovacs/public/qt4-x11/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp 
OBJECTS       = main.o
DIST          = ../../../mkspecs/common/g++.conf \
		../../../mkspecs/common/unix.conf \
		../../../mkspecs/common/linux.conf \
		../../../mkspecs/features/qt_functions.prf \
		../../../mkspecs/features/qt_config.prf \
		../../../mkspecs/features/exclusive_builds.prf \
		../../../mkspecs/features/default_pre.prf \
		../../../mkspecs/features/release.prf \
		../../../mkspecs/features/default_post.prf \
		../../../mkspecs/features/unix/largefile.prf \
		../../../mkspecs/features/static.prf \
		../../../mkspecs/features/warn_on.prf \
		../../../mkspecs/features/resources.prf \
		../../../mkspecs/features/uic.prf \
		../../../mkspecs/features/yacc.prf \
		../../../mkspecs/features/lex.prf \
		stdint.pro
QMAKE_TARGET  = stdint
DESTDIR       = 
TARGET        = stdint

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: stdint.pro ../../../mkspecs/linux-g++/qmake.conf ../../../mkspecs/common/g++.conf \
		../../../mkspecs/common/unix.conf \
		../../../mkspecs/common/linux.conf \
		../../../mkspecs/features/qt_functions.prf \
		../../../mkspecs/features/qt_config.prf \
		../../../mkspecs/features/exclusive_builds.prf \
		../../../mkspecs/features/default_pre.prf \
		../../../mkspecs/features/release.prf \
		../../../mkspecs/features/default_post.prf \
		../../../mkspecs/features/unix/largefile.prf \
		../../../mkspecs/features/static.prf \
		../../../mkspecs/features/warn_on.prf \
		../../../mkspecs/features/resources.prf \
		../../../mkspecs/features/uic.prf \
		../../../mkspecs/features/yacc.prf \
		../../../mkspecs/features/lex.prf
	$(QMAKE) -nocache -spec ../../../mkspecs/linux-g++ -unix CONFIG+=\ release\ static\ largefile\ stl\ precompile_header\ mmx\ 3dnow\ sse\ sse2 LIBS*= INCLUDEPATH*= QMAKE_CXXFLAGS*= -o Makefile stdint.pro
../../../mkspecs/common/g++.conf:
../../../mkspecs/common/unix.conf:
../../../mkspecs/common/linux.conf:
../../../mkspecs/features/qt_functions.prf:
../../../mkspecs/features/qt_config.prf:
../../../mkspecs/features/exclusive_builds.prf:
../../../mkspecs/features/default_pre.prf:
../../../mkspecs/features/release.prf:
../../../mkspecs/features/default_post.prf:
../../../mkspecs/features/unix/largefile.prf:
../../../mkspecs/features/static.prf:
../../../mkspecs/features/warn_on.prf:
../../../mkspecs/features/resources.prf:
../../../mkspecs/features/uic.prf:
../../../mkspecs/features/yacc.prf:
../../../mkspecs/features/lex.prf:
qmake:  FORCE
	@$(QMAKE) -nocache -spec ../../../mkspecs/linux-g++ -unix CONFIG+=\ release\ static\ largefile\ stl\ precompile_header\ mmx\ 3dnow\ sse\ sse2 LIBS*= INCLUDEPATH*= QMAKE_CXXFLAGS*= -o Makefile stdint.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/stdint1.0.0 || $(MKDIR) .tmp/stdint1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/stdint1.0.0/ && (cd `dirname .tmp/stdint1.0.0` && $(TAR) stdint1.0.0.tar stdint1.0.0 && $(COMPRESS) stdint1.0.0.tar) && $(MOVE) `dirname .tmp/stdint1.0.0`/stdint1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/stdint1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


compiler_rcc_make_all:
compiler_rcc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

