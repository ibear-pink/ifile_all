#!/bin/ksh

IFILE_INCLUDE=-I${IFILE_PATH}/ifile/include/
IFILE_LIB=-L${IFILE_PATH}/ifile/lib -lifile

SDFS_INCLUDE=-I${SDFS_HOME}/include -I${DSC_PATH}/include
SDFS_LIB=-L${SDFS_HOME}/lib -lsdfs -L${DSC_PATH}/lib -ldsc

IFLAGS=-DSS_64BIT_SERVER -D_REENTRANT -D_DEBUG_INFO_

INCLUDE=-I. ${IFILE_INCLUDE} ${SDFS_INCLUDE}

LIB=-L. ${IFILE_LIB} ${SDFS_LIB}

.SUFFIXES:.cpp .o

CFLAGS = ${IFLAGS} ${INCLUDE}

CC=g++ -m64 -g  -lpthread  -ldl -O0 -fpermissive

OBJ = demo.o

BIN=demo

all:${BIN} install clean

.cpp.o:
	echo "Compiling the $<"
	$(CC) -c ${CFLAGS} $<
.c.o:
	echo "Compiling the $<"
	$(CC) -c ${CFLAGS} $<

clean:
	rm -f demo.o

######################################################################

${BIN}: $(OBJ)
	$(CC) ${CFLAGS} -o $@ $? ${LIB}

######################################################################

install:

#mv ${BIN} ${ACCT_BIN}


