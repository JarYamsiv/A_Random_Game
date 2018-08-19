/*
# MADE BY : JARYAMSIV




*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/shaders.h"
#include "headers/mesh.h"
#include "headers/block.h"
#include "headers/camera.h"
#include "headers/terrain/chunk.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

//======================================functions forward declaration=============================
int initWindow();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
//===========================================END==================================================

//============================================GLOBAL VARIABLES================================
// settings
GLFWwindow *window;
const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 900;

//game related global variables

//mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//keys

//timing
float deltaTime = 0.0;
float lastFrame = 0.0;
float currentFrame;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, 0.0);
bool isFPS = false;

//==========================================GLOBAL VARIABLES END=================================

//========================================TEXTURE CLASS==========================================
class Texture
{
  public:
    Texture(const char *texturePath, int sP, int id_)
    {
        id = id_;
        shaderProg = sP;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            //std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    ~Texture()
    {
    }
    unsigned int tex;

  private:
    int shaderProg;
    int id;
};
//==============================================END==============================================

int main()
{

    initWindow();

    //creating a shader for traingle
    Shader traingleShader("shaders/vertex/oneTex.fs", "shaders/fragment/oneTex.fs");
    Shader blockShader("shaders/vertex/shadedOneTex.fs", "shaders/fragment/shadedOneTex.fs");
    //creating the wall texture
    Texture traingleTex("resources/texture/grass_green.jpg", traingleShader.ID, 0);

    //triangle *T = new triangle(traingleShader.ID);
    mesh P(traingleTex.tex, traingleShader.ID, "resources/mesh/grass_block.dat", GL_TRIANGLES, MESH_ONE_TEX);
    block B(traingleTex.tex, blockShader.ID, "resources/mesh/block.dat");

    //initialising projection and view matrix
    glm::mat4 projection;
    unsigned int projMatLoc;
    glm::mat4 view;
    unsigned int viewMatLoc;

    //getting the uniform location for projection and veiw matrix from vertex shader
    projMatLoc = glGetUniformLocation(traingleShader.ID, "projection");
    viewMatLoc = glGetUniformLocation(traingleShader.ID, "view");
    //setting up projection matrix and uploading it's value to uniform from vertexShader
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    traingleShader.use();
    traingleShader.setmat4("projection", projection);
    blockShader.use();
    blockShader.setmat4("projection", projection);

    //setting a chunk of size 16x16 with seed 0 with global positions x=0 z=0
    chunk c1(0, 0, 0);
    chunk c2(0, 1, 0);
    vector<glm::vec3> bVector;
    c1.updateBlockVector(bVector);
    c2.updateBlockVector(bVector);
    B.setMultiplePositions(bVector);

    // render loop
    // -----------
    glm::mat4 model = glm::mat4(1.0f);
    glEnable(GL_DEPTH_TEST);

    double lastTime = glfwGetTime();
    double currentTime;
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
        { 
            std::cout << "fps: " << nbFrames << std::endl;
            nbFrames = 0;
            lastTime += 1.0;
        }

        currentFrame = currentTime;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.getViewMat();
        traingleShader.use();
        traingleShader.setmat4("view", view);
        blockShader.use();
        blockShader.setmat4("view", view);

        //B.multipleRendering();
        B.Display(view,projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

//=============================================function definitions==============================

int initWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif*/

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    //fullscreen
    //window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Game",glfwGetPrimaryMonitor(),NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (isFPS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CAM_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CAM_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CAM_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CAM_RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isFPS = !isFPS;
}
