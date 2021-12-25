CXX=g++ -Wall -Wextra -Wpedantic
CXXFLAGS=-Og -std=c++17
TARGET=xrq
INC_FLAGS := -I include
CPPFLAGS = $(INC_FLAGS) -MT $@ -MMD -MP -MF build/$*.d
SRCS := main.cpp include/gen/xrq.cpp include/gen/xrq_wrapper.cpp
OBJS := $(patsubst %.cpp, ./build/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)
LDFLAGS := -ljoedb

all: xrq

xrq: $(OBJS)
	$(CXX) $^ -o $(TARGET) $(LDFLAGS)

include/gen/xrq.cpp: xrq.joedbc xrq.joedbi
	make -C include/gen

build:
	mkdir -p build build/include/gen

./build/%.o : ./%.cpp build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

./build/%.o : ./%.c build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -fr build xrq

-include $(DEPS)
