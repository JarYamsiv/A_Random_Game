#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class triangle{
public:
                            triangle(int);
                            ~triangle();

    void                    Display();
    void                    setFinalBuffer();

private:
    unsigned int            VAO,VBO;
    int                     shaderProgram;
    float                   vertexData[9];
    float                   color[9];
    float                   finalBuffer[18];
};

#endif