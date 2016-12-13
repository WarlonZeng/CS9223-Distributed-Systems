THRIFT_VER = thrift-0.9.3
#USR_DIR    =${HOME}/usr
#THRIFT_DIR =${USR_DIR}/${THRIFT_VER}
FLAGS      = -std=c++11
ROOT_DIR   = ${VAR}/lib
THRIFT_DIR = ${ROOT_DIR}/${THRIFT_VER}
INCS_DIRS  = -I${USR_DIR}/include -I${THRIFT_DIR}/include/thrift
LIBS_DIRS  = -L${USR_DIR}/lib -L${USR_DIR}/${THRIFT_VER}/lib
CPP_DEFS   = -D=HAVE_CONFIG_H
CPP_OPTS   = -Wall -O2
LIBS       = -lthrift

GEN_SRC    = gen-cpp/KVStore.cpp  \
             gen-cpp/kvstore_types.cpp
GEN_INC    = -I gen-cpp

default: server client

server: kvserver.cpp
	g++ $(FLAGS) ${CPP_OPTS} ${CPP_DEFS} -o kvserver ${GEN_INC} ${INCS_DIRS} kvserver.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS}

client: kvclient.cpp kvclient_1.cpp kvclient_2.cpp consistency_test.cpp
	g++ $(FLAGS) ${CPP_OPTS} ${CPP_DEFS} -o kvclient ${GEN_INC} ${INCS_DIRS} kvclient.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS}
	g++ $(FLAGS) ${CPP_OPTS} ${CPP_DEFS} -o kvclient_1 ${GEN_INC} ${INCS_DIRS} kvclient_1.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS}
	g++ $(FLAGS) ${CPP_OPTS} ${CPP_DEFS} -o kvclient_2 ${GEN_INC} ${INCS_DIRS} kvclient_2.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS}
	g++ $(FLAGS) ${CPP_OPTS} ${CPP_DEFS} -o consistency_test ${GEN_INC} ${INCS_DIRS} consistency_test.cpp ${GEN_SRC} ${LIBS_DIRS} ${LIBS} -pthread

clean:
	$(RM) -r kvclient kvclient_1 kvclient_2 consistency_test kvserver