SRCDIR = src
SRCS = $(wildcard ${SRCDIR}/*.cpp)
OBJS = $(SRCS:.cpp=.o)

TESTDIR = test
TESTSRCS = $(wildcard ${TESTDIR}/*.cpp)
TESTEXE = $(TESTSRCS:.cpp=)
TESTOBJS = $(TESTSRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lgtest -lgtest_main -pthread
TARGET = simpledb
LIBS = -L/usr/local/lib
INCLUDES = -I./src

.PHONY: all
all: $(OBJS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TESTDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TESTEXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(TESTOBJS) $(LDFLAGS) $(LIBS) -o $@

.PHONY: clean
clean: 
	-$(RM) $(OBJS) $(TESTOBJS) $(TESTEXE)

.PHONY: test
test: clean $(TESTOBJS) $(TESTEXE)
