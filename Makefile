SRCDIR = src
SRCS = $(wildcard ${SRCDIR}/*/*.cpp)
OBJS = $(SRCS:.cpp=.o)

TESTDIR = test
TESTSRCS = $(wildcard ${TESTDIR}/*.cpp)
TESTEXE = test/main_test
TESTOBJS = $(TESTSRCS:.cpp=.o)

SCRIPTDIR = scripts
SCRIPTSRCS = $(wildcard ${SCRIPTDIR}/*.cpp)
SCRIPTEXES = $(SCRIPTSRCS:.cpp=)
SCRIPTOBJS = $(SCRIPTSRCS:.cpp=.o)

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

$(SCRIPTDIR)/%.o: $(SCRIPTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(SCRIPTEXES): $(SCRIPTOBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(SCRIPTOBJS) -o $@

.PHONY: clean
clean: 
	-$(RM) $(OBJS) $(TESTOBJS) $(TESTEXE) $(SCRIPTOBJS) $(SCRIPTEXES)
	-rm -rf *Test*

.PHONY: test
test: clean $(TESTOBJS) $(TESTEXE)

.PHONY: createdb
createdb: $(OBJS) $(SCRIPTOBJS) $(SCRIPTEXES)
