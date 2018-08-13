#include "../../headers/terrain/chunk.h"

chunk::chunk(int seed,int gX,int gZ)
{
    srand(seed);
    float x,y,z;
    globalChunkX = gX;
    globalChunkZ = gZ;
    for(int i=0; i<chunkSize; i++)
    {
        for(int j=0; j<chunkSize; j++)
        {
            x=(float)(globalChunkX*16+ i-chunkCenterOffset);
            z=(float)(globalChunkZ*16+ j-chunkCenterOffset);
            y=(float)floorl(2.0*(rand()%100)/100.0);
            blockPos.push_back(glm::vec3(x,y,z));
        }
    }
}

chunk::~chunk()
{

}
