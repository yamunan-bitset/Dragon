CXX  = g++
OUT  = dragon
LIBS = -lsfml-system -lsfml-window -lsfml-graphics
OBJS = main.o map.o

all: $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -g -o $(OUT)
