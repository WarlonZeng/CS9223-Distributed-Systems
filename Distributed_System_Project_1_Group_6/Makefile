THRIFT_VER =thrift-0.9.3
USR_DIR    =${HOME}/usr
THRIFT_DIR =${USR_DIR}/${THRIFT_VER}
INCS_DIRS  =-I${USR_DIR}/include -I${THRIFT_DIR}/include/thrift
LIBS_DIRS  =-L${USR_DIR}/lib -L${USR_DIR}/${THRIFT_VER}/lib
CPP_DEFS   =-D=HAVE_CONFIG_H
CPP_OPTS   =-Wall -O2
LIBS       =-lthrift

GEN_SRC    = gen-cpp/KVStore.cpp  \
             gen-cpp/kvstore_types.cpp
GEN_INC    = -I gen-cpp

default: server client

server: kvserver.cpp
	g++ ${CPP_OPTS} ${CPP_DEFS} -o kvserver ${GEN_INC} ${INCS_DIRS} kvserver.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS}

client: kvclient.cpp
	g++ ${CPP_OPTS} ${CPP_DEFS} -o kvclient ${GEN_INC} ${INCS_DIRS} kvclient.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS}

clean:
	$(RM) -r kvclient kvserver