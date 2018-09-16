#include "../../headers/terrain/chunk.h"

chunk::chunk(int seed,int gX,int gZ)
{
    srand(seed);
    globalChunkX = gX;
    globalChunkZ = gZ;

    float x,y,z;
    std::vector<glm::vec3> pseudoRandGradVec;
    for(int i=0; i<8; i++)
    {
        x=(rand()%1000)/1000.0;
        y=(rand()%1000)/1000.0;
        z=(rand()%1000)/1000.0;
        pseudoRandGradVec.push_back(glm::normalize(glm::vec3(x,y,z)));
    }

    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                glm::vec3 posVector=glm::vec3(i,j,k);
                double sum=0;
                for(int l=0; l<8; l++)
                {
                    sum+=glm::dot(pseudoRandGradVec[l],posVector);
                }
                if(sum>50.0)
                {
                    chunkData[i][j][k]=1;
                }

            }
        }
    }


    /*for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                chunkData[i][j][k]=rand()%2;
            }
        }
    }*/
    
}

chunk::~chunk()
{

}


void chunk::updateBlockVector(vector<glm::vec3> &bVector)
{
    float x,y,z;
    for(int i=0; i<CHUNK_SIZE; i++)
    {
        for(int j=0; j<CHUNK_SIZE; j++)
        {
            /*x=(float)(globalChunkX*CHUNK_SIZE+ i-CHUNK_OFFSET);
            z=(float)(globalChunkZ*CHUNK_SIZE+ j-CHUNK_OFFSET);
            y=(float)floorl(2.0*(rand()%100)/100.0);
            bVector.push_back(glm::vec3(x,y,z));*/
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                if(chunkData[i][j][k]==0)continue;

                bool expr=   chunkData[i+1][j][k]==1&&
                            chunkData[i-1][j][k]==1&&
                            chunkData[i][j+1][k]==1&&
                            chunkData[i][j-1][k]==1&&
                            chunkData[i][j][k+1]==1&&
                            chunkData[i][j][k-1]==1;

                if(expr)continue;
                
                x=(float)(globalChunkX*CHUNK_SIZE+ i-CHUNK_OFFSET);
                z=(float)(globalChunkZ*CHUNK_SIZE+ j-CHUNK_OFFSET);
                y=(float)(k-CHUNK_OFFSET);
                bVector.push_back(glm::vec3(x,y,z));
            }
        }
    }
}