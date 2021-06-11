SRCS = src/*.cpp
OBJS = $(SRCS:.cpp=.o)
CC = g++
CFLAGS = -std=c++17 
TARGET = simpledb
DEPENDS = $(OBJS:.o=.d)
INCLUDES = -I./src

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCS)
	mv *.o src/

all: clean $(OBJS) $(TARGET)
clean:
	-rm -f $(OBJS) $(TARGET)
