TARGET = transmitter

SRCS = transmitter.c

OBJS = $(addsuffix .o, $(basename $(SRCS)))

CXX = gcc

CFLAGS = -std=gnu99 -Wall -Wextra -msse4.2 -mavx -g

CXXFLAGS = -std=gnu99 -Wall -Wextra -msse4.2 -mavx -g


all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) -lm

clean:
	$(RM) $(OBJS) $(TARGET)

transmitter.o: transmitter.h transmitter.c
