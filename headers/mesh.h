#ifndef _MESH_H
#define _MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;

enum MESH_TEXT_TYPE{
    MESH_CONST_COLOR,
    MESH_COLOR_ONLY,
    MESH_COLOR_TEX,
    MESH_ONE_TEX,
    MESH_TWO_TEX
};



class mesh{
public:
                            mesh(unsigned int,int,const char*,GLenum buildMode,MESH_TEXT_TYPE);
                            ~mesh();

    void                    Display();
    void                    setFinalBuffer(MESH_TEXT_TYPE);
    void                    moveTo(glm::vec3 newPos);
    void                    moveTo(float x,float y,float z);
    void                    setRotation(float rx,float ry,float rz);
    void                    setRotationWithVec(float angle,glm::vec3 normal);

    void                    setMultiplePositions(vector<glm::vec3>&);
    void                    multipleRendering();

private:
    void                    loadFromFile(const char*,MESH_TEXT_TYPE);

    unsigned int            VAO,VBO,EBO;
    int                     shaderProgram;
    unsigned int            texture;

    int                     n;
    vector<float >          vertexData;

    int                     nIndex;
    vector<unsigned int>    indexData;

    vector<float>           color;
    vector<float>           finalBuffer;
    vector<float>           texCord;

    GLenum                  bMode;//build mode whether it's triangle quads or etc..
    unsigned int            modelMatLoc;
    glm::vec3               position;
    float                   rotationX,rotationY,rotationZ,rotationWithVec;
    glm::vec3               rotationNormal;
    vector<glm::vec3>       multipleRenderingPositions;


};

#endif