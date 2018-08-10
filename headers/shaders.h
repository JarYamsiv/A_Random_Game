#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Shader{
public:
                        Shader(const char*,const char*);
                        ~Shader();
    void                use();
    void                setInt(const std::string &name,int vlaue) const;
    void                setFloat(const std::string &name,float value) const;
    void                setmat4(const std::string &name,const glm::mat4 &mat) const;
    int                 ID;
private:
    
};

#endif