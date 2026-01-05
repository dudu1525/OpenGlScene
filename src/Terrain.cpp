#include "Terrain.h"

#include <glm/gtc/noise.hpp>
#include <cmath>

void Terrain::initializeTerrain(const char* texturePath,const char* roughPath, gps::Shader terrainShader, gps::LightSources lights)
{
    //load first texture
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath, &textureWidth, &textureHeight, &nrChannels, 0);

    glGenTextures(1, &this->sandTexture);
    glBindTexture(GL_TEXTURE_2D, this->sandTexture);
   // glGenTextures(1, &this->roughTexture);
   // glBindTexture(GL_TEXTURE_2D, this->roughTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_SRGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    //second texture
    unsigned char* data2 = stbi_load(roughPath, &textureWidth, &textureHeight, &nrChannels, 3);
    glGenTextures(1, &this->roughTexture);
    glBindTexture(GL_TEXTURE_2D, this->roughTexture);
    if (data) {

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_SRGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data2);
    }

    //normals




    //with help of learnOpenGl.com - heightmaps
    //set vertices
    for (int i = 0; i < REZ; i++) {
        for (int j = 0; j < REZ; j++) {
            //position                  //divide by rez, to get the 'unit' = width/(rez-1) and height/(rez-1)
            float x = -WIDTH / 2.0f + (float)i * WIDTH / (REZ - 1); //towards pozitive values, from left to right
            float z = -HEIGHT / 2.0f + (float)j * HEIGHT / (REZ - 1);//towards pozitive, from 'bottom' to top
            vertices.push_back(x); //x
            float y = perlinNoise(x, z);
                vertices.push_back(y);
            vertices.push_back(z); //z
            //normals
            glm::vec3 n = getNormal(x, z);
            vertices.push_back(n.x);
            vertices.push_back(n.y);
            vertices.push_back(n.z);
            //textures
            vertices.push_back((float)i / (REZ - 1)*50);       //first divide the 0 to 1 to (rez-1) parts            
            vertices.push_back((float)j / (REZ - 1)*50);                   //then obtain 50x50 -> 2500 total textures across the terrain
        }
    }



    //ebo
    for (int i = 0; i < REZ - 1; i++) {//width
        for (int j = 0; j < REZ - 1; j++) {//height
            indices.push_back(i * REZ + j);
            indices.push_back((i + 1) * REZ + j);
            indices.push_back(i * REZ + (j + 1));
            indices.push_back((i + 1) * REZ + (j + 1));
        }
    }

    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//location
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);//normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);//texture coords

    glPatchParameteri(GL_PATCH_VERTICES, 4);


    //send light
    setLightUniforms(terrainShader, lights);
}

void Terrain::renderTerrain(gps::Shader terrainShader, glm::mat4 projection, gps::Camera camera)
{
    terrainShader.useShaderProgram();

    ///////////////////////////////////////////////bind and activate texture 'sandTexture'
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->sandTexture);
    glUniform1i(glGetUniformLocation(terrainShader.shaderProgram, "sandTexture"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, roughTexture);
    glUniform1i(glGetUniformLocation(terrainShader.shaderProgram, "roughTexture"), 2);


    /////////////////////////////////////send model, view, projection
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    
    
    ////////////////////////////////////// world transformation and send uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(glGetUniformLocation(terrainShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniform3fv(glGetUniformLocation(terrainShader.shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPositionCamera()));

    // render the vertices
    glBindVertexArray(terrainVAO);
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_PATCHES, (REZ -1) * (REZ -1) * 4, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

void Terrain::setLightUniforms(gps::Shader terrainShader, gps::LightSources lights)
{
    terrainShader.useShaderProgram();
    lights.setLightUniforms(terrainShader.shaderProgram);

}

void Terrain::initSimple()
{   
    for (int i = 0; i < REZ; i++) {
        for (int j = 0; j < REZ; j++) {
            float x = -WIDTH / 2.0f + (float)i * WIDTH / (REZ - 1);
            float z = -HEIGHT / 2.0f + (float)j * HEIGHT / (REZ - 1);
            verticessimple.push_back(x); //x
            float y = perlinNoise(x, z);
            verticessimple.push_back(y);
            verticessimple.push_back(z); //z

        }
    }

    for (int i = 0; i < REZ - 1; i++) {
        for (int j = 0; j < REZ - 1; j++) {
            int topLeft = i * REZ + j;
            int topRight = (i + 1) * REZ + j;
            int bottomLeft = i * REZ + (j + 1);
            int bottomRight = (i + 1) * REZ + (j + 1);

          
            indicessimple.push_back(topLeft);
            indicessimple.push_back(bottomLeft);
            indicessimple.push_back(topRight);

        
            indicessimple.push_back(topRight);
            indicessimple.push_back(bottomLeft);
            indicessimple.push_back(bottomRight);
        }
    }
    glGenVertexArrays(1, &vaosimple);
    glBindVertexArray(vaosimple);

    glGenBuffers(1, &vbosimple);
    glBindBuffer(GL_ARRAY_BUFFER, vbosimple);
    glBufferData(GL_ARRAY_BUFFER, verticessimple.size() * sizeof(float), verticessimple.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &ebosimple);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebosimple);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicessimple.size() * sizeof(unsigned int), indicessimple.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//location

}

void Terrain::renderSimple(gps::Shader shader)
{
    shader.useShaderProgram();
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(vaosimple);

    glDrawElements(GL_TRIANGLES, (GLsizei)indicessimple.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}

float Terrain::perlinNoise(float x, float z)
{
    float nx = x / (WIDTH / 2.0f);//normalize the x and y to -1 1
    float nz = z / (HEIGHT / 2.0f);
    float squareDist = glm::max(glm::abs(nx), glm::abs(nz));//take max value from positive values

    //apply 2 times the perlin, more realistic this way
    float freq = 0.0001f;
    float noiseValue = (glm::perlin(glm::vec2(x * freq, z * freq)) + 1.0f) * 0.5f;

    float detail = (glm::perlin(glm::vec2(x * 0.0005f, z * 0.0005f)) + 1.0f) * 0.5f;
    float combinedNoise = (noiseValue * 0.95f) + (detail * 0.05f);

        //smoother inner part for the 'lake'
    float innerPart = glm::smoothstep(0.0f, 0.5f, squareDist);//up to 50% of map, smoother the mountains
    float mountains = combinedNoise * innerPart;

    float finalHeight= pow(mountains, 1.5f) * maxh; //before marking the 'small lake'
    //make an elypse for the oasis
    float xcomp = 1.8f;
    float zcomp = 1.0f;
    float ellipticalDist = glm::length(glm::vec2(nx / xcomp, nz / zcomp));
    float oasisSize = 0.17f;//percentage of how much is lake
    float oasisDepth = 2515.0f;//depth of oasis
        //as you approach center, the lake is deeper
    float oasis = 1.0f - glm::smoothstep(0.0f, oasisSize, ellipticalDist);


    // Subtract from finalheight, in order to obtain the depth
    return finalHeight - (oasis * oasisDepth);


}

//function to calculate normals of a vertex based on 'neighbouring' elements and taking the 'slope'
glm::vec3 Terrain::getNormal(float x, float z)
{
    float el = 1.0f; //we set the offset to 1.0f
    //neighbour vertices
    float hL = perlinNoise(x - el, z);//left
    float hR = perlinNoise(x + el, z);//right
    float hD = perlinNoise(x, z - el);//down
    float hU = perlinNoise(x, z + el);//up

    //normal computed
    glm::vec3 normal = glm::vec3(hL - hR, 2.0f * el, hD - hU);//cross product (matrix)
    return glm::normalize(normal);
}
