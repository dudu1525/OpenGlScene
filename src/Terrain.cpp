#include "Terrain.h"

#include <glm/gtc/noise.hpp>
#include <cmath>

void Terrain::initializeTerrain(const char* texturePath, gps::Shader terrainShader, gps::LightSources lights)
{
    //load texture
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath, &textureWidth, &textureHeight, &nrChannels, 0);

    glGenTextures(1, &this->sandTexture);
    glBindTexture(GL_TEXTURE_2D, this->sandTexture);
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



    //set vertices
    for (int i = 0; i < rez; i++) {
        for (int j = 0; j < rez; j++) {
            float x = -width / 2.0f + (float)i * width / (rez - 1);
            float z = -height / 2.0f + (float)j * height / (rez - 1);
            vertices.push_back(x); //x
            float frequency = 0.5f; // Smaller = bigger, smoother dunes
            float amplitude = 15.0f;
            float y = perlinNoise(x, z);
                vertices.push_back(y);

            vertices.push_back(z); //z
            vertices.push_back((float)i / (rez - 1)*50);                    //u
            vertices.push_back((float)j / (rez - 1)*50);                    //v
        }
    }



    //ebo
    for (int i = 0; i < rez - 1; i++) {
        for (int j = 0; j < rez - 1; j++) {
            indices.push_back(i * rez + j);
            indices.push_back((i + 1) * rez + j);
            indices.push_back(i * rez + (j + 1));
            indices.push_back((i + 1) * rez + (j + 1));
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glPatchParameteri(GL_PATCH_VERTICES, 4);


    //send light
   // lights.setLightUniforms(terrainShader.shaderProgram);
}

void Terrain::renderTerrain(gps::Shader terrainShader, glm::mat4 projection, gps::Camera camera)
{
    terrainShader.useShaderProgram();

    //bind and activate texture 'sandTexture'
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->sandTexture);
    glUniform1i(glGetUniformLocation(terrainShader.shaderProgram, "sandTexture"), 1);

    //send model, view, projection
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    // world transformation


    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // render the vertices
    glBindVertexArray(terrainVAO);
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_PATCHES, (rez-1) * (rez-1) * 4, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

float Terrain::perlinNoise(float x, float z)
{
    float nx = x / (width / 2.0f);
    float nz = z / (height / 2.0f);
    float squareDist = glm::max(glm::abs(nx), glm::abs(nz));


    float freq = 0.0001f;
    float noiseValue = (glm::perlin(glm::vec2(x * freq, z * freq)) + 1.0f) * 0.5f;

    float detail = (glm::perlin(glm::vec2(x * 0.0005f, z * 0.0005f)) + 1.0f) * 0.5f;
    float combinedNoise = (noiseValue * 0.8f) + (detail * 0.2f);

        //smoother inner part
    float innerMask = glm::smoothstep(0.0f, 0.5f, squareDist);
    float mountains = combinedNoise * innerMask;

    return pow(mountains, 1.5f) * maxh;
}
