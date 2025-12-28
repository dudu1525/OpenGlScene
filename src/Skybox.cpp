#include "Skybox.h"

void Skybox::initializeSkybox(gps:: Shader shader)
{

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    cubemapTexture = loadCubemap();
    shader.useShaderProgram();
    glUniform1i(glGetUniformLocation(shader.shaderProgram, "skybox"), 10);




}

void Skybox::drawSkybox(gps::Shader shader, gps::Camera camera, glm::mat4 projection)
{
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    shader.useShaderProgram();
    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix

    //set 2 view and projection in the shaders
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

}

void Skybox::changeFaces(bool isDay)
{


    if (isDay)//if is day, switch to night
    {
        vector<std::string> faces2 = {
        "models/skybox/nightskybox_right.png",
        "models/skybox/nightskybox_left.png",
        "models/skybox/nightskybox_top.png",
        "models/skybox/nightskybox_bottom.png",
        "models/skybox/nightskybox_front.png",
        "models/skybox/nightskybox_back.png"
        };
        faces = faces2;
    }
    else//switch to day
    {
        vector<std::string> faces2
        {
            "models/skybox/dayskybox_right.png",
            "models/skybox/dayskybox_left.png",
            "models/skybox/dayskybox_top.png",
            "models/skybox/dayskybox_bottom.png",
            "models/skybox/dayskybox_front.png",
            "models/skybox/dayskybox_back.png"
        };
        faces = faces2;

    }

    cubemapTexture = loadCubemap();

}

unsigned int Skybox::loadCubemap()
{
    stbi_set_flip_vertically_on_load(false);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < this->faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}