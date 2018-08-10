#include "../headers/mesh.h"

mesh::mesh(unsigned int tX, int sP, const char *fileName, GLenum bM, MESH_TEXT_TYPE type)
{
    loadFromFile(fileName, type);
    bMode = bM;
    shaderProgram = sP;
    texture = tX;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, finalBuffer.size() * sizeof(finalBuffer[0]), &finalBuffer[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), &indexData[0], GL_STATIC_DRAW);

    if (type == MESH_COLOR_TEX)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    if (type == MESH_ONE_TEX)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //*transformMatrix=glm::mat4(1.0f);
    modelMatLoc = glGetUniformLocation(shaderProgram, "model");
    position = glm::vec3(0.0, 0.0, 0.0);

    rotationWithVec = 0;
    rotationNormal = glm::vec3(1.0, 1.0, 1.0);
}

mesh::~mesh()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void mesh::Display()
{
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 model;

    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::rotate(model, glm::radians(rotationWithVec), rotationNormal);

    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(bMode, nIndex, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void mesh::multipleRendering()
{
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 model;

    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::rotate(model, glm::radians(rotationWithVec), rotationNormal);

    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);

    for (int i = 0; i < multipleRenderingPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, multipleRenderingPositions[i]);
        glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(bMode, nIndex, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void mesh::setFinalBuffer(MESH_TEXT_TYPE type)
{
    int iP, iV, iC, iT, i, MulOffset;
    iP = 0;
    iV = 0;
    iC = 0;
    iT = 0;
    if (type == MESH_COLOR_TEX)
        MulOffset = 8;
    if (type == MESH_ONE_TEX)
        MulOffset = 5;
    if (type == MESH_TWO_TEX)
        MulOffset = 7;
    while (iP < n * MulOffset)
    {
        for (i = 0; i < 3; i++)
        {
            finalBuffer.push_back(vertexData[iV]);
            iP++;
            iV++;
        }

        if (type == MESH_COLOR_ONLY || type == MESH_COLOR_TEX)
        {
            for (i = 0; i < 3; i++)
            {
                finalBuffer.push_back(color[iC]);
                iP++;
                iC++;
            }
        }

        if (type == MESH_COLOR_TEX || type == MESH_ONE_TEX || type == MESH_TWO_TEX)
        {
            for (i = 0; i < 2; i++)
            {
                finalBuffer.push_back(texCord[iT]);
                iP++;
                iT++;
            }
        }
    }
}

void mesh::loadFromFile(const char *fileName, MESH_TEXT_TYPE type)
{
    int i;
    std::fstream meshFile;
    meshFile.open(fileName, std::ios::in);
    if (!meshFile.is_open())
    {
        std::cout << "failed to open " << fileName << std::endl;
    }
    meshFile >> n;

    float tempF;
    unsigned int tempI;

    for (i = 0; i < n * 3; i++)
    {
        meshFile >> tempF;
        vertexData.push_back(tempF);
    }

    if (type == MESH_COLOR_ONLY || type == MESH_COLOR_TEX)
    {
        for (i = 0; i < n * 3; i++)
        {
            meshFile >> tempF;
            color.push_back(tempF);
        }
    }

    if (type == MESH_COLOR_TEX || type == MESH_ONE_TEX || type == MESH_TWO_TEX)
    {
        for (i = 0; i < n * 2; i++)
        {
            meshFile >> tempF;
            texCord.push_back(tempF);
        }
    }

    meshFile >> nIndex;

    for (i = 0; i < nIndex; i++)
    {
        meshFile >> tempI;
        indexData.push_back(tempI);
    }

    setFinalBuffer(type);
}

void mesh::moveTo(glm::vec3 newPos)
{
    position = newPos;
}

void mesh::moveTo(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}

void mesh::setRotation(float rx, float ry, float rz)
{
    rotationX = rx;
    rotationY = ry;
    rotationZ = rz;
}

void mesh::setRotationWithVec(float angle, glm::vec3 normal)
{
    rotationWithVec = angle;
    rotationNormal = normal;
}

void mesh::setMultiplePositions(vector<glm::vec3> &mulPos)
{
    if (multipleRenderingPositions.size() != 0)
    {
        multipleRenderingPositions.clear();
    }

    multipleRenderingPositions = mulPos;
}
