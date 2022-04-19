CXX  = g++
OUT  = dragon
LIBS = -lsfml-system -lsfml-window -lsfml-graphics
OBJS = main.o map.o

all: clear $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -g -o $(OUT)
clear:
	rm -rf $(OBJS) $(OUT)
