#include "../headers/block.h"

block::block(unsigned int tX, int sP, const char *fileName)
{
    loadFromFile(fileName);
    shaderProgram = sP;
    texture = tX;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, finalBuffer.size() * sizeof(finalBuffer[0]), &finalBuffer[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), &indexData[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    PVMLoc = glGetUniformLocation(shaderProgram, "PVM");
}

block::~block()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void block::Display(glm::mat4 view, glm::mat4 projection)
{

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 PVM, model;

    model = glm::mat4(1.0f);

    PVM = projection * view * model;

    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(PVM));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, nIndex, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void block::multipleRendering()
{
}

void block::setFinalBuffer()
{
    int iP, iV, iN, iT, i, MulOffset;
    iP = 0;
    iV = 0;
    iN = 0;
    iT = 0;

    while (iP < n * 8)
    {
        for (i = 0; i < 3; i++)
        {
            finalBuffer.push_back(vertexData[iV]);
            iP++;
            iV++;
        }

        for (i = 0; i < 3; i++)
        {
            finalBuffer.push_back(vertexNormals[iN]);
            iP++;
            iN++;
        }

        for (i = 0; i < 2; i++)
        {
            finalBuffer.push_back(texCord[iT]);
            iP++;
            iT++;
        }
    }
}

void block::loadFromFile(const char *fileName)
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

    for (i = 0; i < n * 3; i++)
    {
        meshFile >> tempF;
        vertexNormals.push_back(tempF);
    }

    for (i = 0; i < n * 2; i++)
    {
        meshFile >> tempF;
        texCord.push_back(tempF);
    }

    meshFile >> nIndex;

    for (i = 0; i < nIndex; i++)
    {
        meshFile >> tempI;
        indexData.push_back(tempI);
    }

    setFinalBuffer();
}

void block::setMultiplePositions(vector<glm::vec3> &mulPos)
{
    if (multipleRenderingPositions.size() != 0)
    {
        multipleRenderingPositions.clear();
    }

    multipleRenderingPositions = mulPos;
}
