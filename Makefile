CXX=g++ -Wall -Wextra -Wno-unused-parameter
CXXFLAGS=-Og -std=c++17
TARGET=xrq
INC_FLAGS := -I include
CPPFLAGS = $(INC_FLAGS) -MT $@ -MMD -MP -MF build/$*.d
SRCS := main.cpp include/gen/xrq.cpp include/gen/xrq_wrapper.cpp $(shell find src/ -path "*.cpp")
OBJS := $(patsubst %.cpp, ./build/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)
LDFLAGS := -ljoedb

all: xrq

xrq: $(OBJS)
	$(CXX) $^ -o $(TARGET) $(LDFLAGS)

include/gen/xrq.cpp: xrq.joedbc xrq.joedbi
	make -C include/gen

build:
	mkdir -p build build/include/gen build/src/commands

./build/%.o : ./%.cpp build include/gen/xrq.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -fr build xrq xrq.joedb

-include $(DEPS)
