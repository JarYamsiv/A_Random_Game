CC = g++
GDB = -g
CXXFLAGS += -std=c++11
OBJS = objects/main.o objects/triangle.o objects/shaders.o objects/mesh.o objects/camera.o
LIBS = -lGL -lGLU -lglut -lglfw -lGLEW

a.out : ${OBJS}
	${CC} ${GDB} ${CXXFLAGS} ${OBJS} ${LIBS} -o bin/main.out 

objects/main.o: main.cpp
	${CC} -c main.cpp -o objects/main.o

objects/triangle.o: src/triangle.cpp headers/triangle.h
	${CC} ${CXXFLAGS} -c src/triangle.cpp -o objects/triangle.o

objects/shaders.o: src/shaders.cpp headers/shaders.h
	${CC} -c src/shaders.cpp -o objects/shaders.o

objects/mesh.o: src/mesh.cpp headers/mesh.h
	${CC} -c src/mesh.cpp -o objects/mesh.o

objects/camera.o: src/camera.cpp headers/camera.h
	${CC} -c src/camera.cpp -o objects/camera.o


clean:
	\rm a.out *.o *.dot
