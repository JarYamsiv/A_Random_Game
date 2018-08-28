CC = g++
GDB = -g
CXXFLAGS += -std=c++11
OBJS = objects/main.o objects/shaders.o objects/mesh.o objects/camera.o objects/block.o objects/chunk.o
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW

a.out : ${OBJS}
	${CC} ${GDB} ${CXXFLAGS} ${OBJS} ${LIBS} -o bin/main.out 

objects/main.o: main.cpp
	${CC} -c main.cpp ${CXXFLAGS} -o objects/main.o

objects/shaders.o: src/shaders.cpp headers/shaders.h
	${CC} -c src/shaders.cpp -o objects/shaders.o

objects/mesh.o: src/mesh.cpp headers/mesh.h
	${CC} -c src/mesh.cpp -o objects/mesh.o

objects/camera.o: src/camera.cpp headers/camera.h
	${CC} -c src/camera.cpp -o objects/camera.o

objects/block.o: src/block.cpp headers/block.h
	${CC} -c src/block.cpp -o objects/block.o

objects/chunk.o: src/terrain/chunk.cpp headers/terrain/chunk.h
	${CC} -c src/terrain/chunk.cpp -o objects/chunk.o


clean:
	\rm a.out *.o *.dot
