#ifndef _CHUNK_H
#define _CHUNK_H

#include <glm/glm.hpp>
#include <math.h>
#include <vector>

#define CHUNK_SIZE 16
#define CHUNK_OFFSET 8

using std::vector;

class chunk{
public:
                            chunk(int,int,int);
                            ~chunk();
    void                    updateBlockVector(vector<glm::vec3>&);
private:

    int                     globalChunkX,globalChunkZ;
    int                     chunkData[32][32][32];    
};



#endif