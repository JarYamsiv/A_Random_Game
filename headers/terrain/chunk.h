#ifndef _CHUNK_H
#define _CHUNK_H

#include <glm/glm.hpp>
#include <math.h>
#include <vector>

using std::vector;

class chunk{
public:
                            chunk(int,int,int);
                            ~chunk();
    vector<glm::vec3>       blockPos;
private:
    int                     globalChunkX,globalChunkZ;
    const static int        chunkSize = 16;
    const static int        chunkCenterOffset = 8;
    
};



#endif