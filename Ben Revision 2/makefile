CXX = g++
CXXFLAGS = -std=c++0x 
CXXFLAGS += -Wall 
CXXFLAGS += -pedantic-errors
CXXFLAGS += -g
#CXXFLAGS += -O3
#LDFLAGS = -lboost_date_time

OBJS = read.o

SRCS = read.cpp

HEADERS =

#target: dependencies
#	rule to build
#

Program: ${OBJS} ${HEADERS}
	${CXX} ${LDFLAGS} ${OBJS} -o Program

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c $(@:.o=.cpp)

Debug: 
