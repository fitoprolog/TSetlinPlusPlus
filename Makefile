cpp=gcpp
CXX=g++
RM=rm -f
CPPFLAGS= -std=c++11 -I .  -ggdb
LDFLAGS=
LDLIBS= `pkg-config --libs opencv4`

SRCS=main.cpp TsetlinMachine.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: main

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o main $(OBJS) $(LDLIBS)

main.o: main.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) main
