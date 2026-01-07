#include "Water.h"
#include "LightSources.h"
void Water::initializeWater(gps::Shader& shader)
{
    unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
    };

	glGenVertexArrays(1, &waterVAO);
	glGenBuffers(1, &waterEBO);
	glGenBuffers(1, &waterVBO);
	glBindVertexArray(waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    //create dudvmap texture


    modelLoc = glGetUniformLocation(shader.shaderProgram, "model");
    viewLoc = glGetUniformLocation(shader.shaderProgram, "view");
    projLoc = glGetUniformLocation(shader.shaderProgram, "projection");
    moveFactorLoc = glGetUniformLocation(shader.shaderProgram, "moveFactor");
    dudvMap = glGetUniformLocation(shader.shaderProgram, "dudvMap");
    cameraposLoc = glGetUniformLocation(shader.shaderProgram, "cameraPosition");
    normalMap = glGetUniformLocation(shader.shaderProgram, "normalMap");
    lightColour = glGetUniformLocation(shader.shaderProgram, "lightColour");
    lightPos = glGetUniformLocation(shader.shaderProgram, "lightPosition");
    glGenTextures(1, &dudvMapTextureId);
    int width, height, nrComponents;
    unsigned char* data = stbi_load("models/water/waterDUDV.png", &width, &height, &nrComponents, 0);
        glBindTexture(GL_TEXTURE_2D, dudvMapTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);


        glGenTextures(1, &normalMapTextureId);
         data = stbi_load("models/water/normal.png", &width, &height, &nrComponents, 0);
        glBindTexture(GL_TEXTURE_2D, normalMapTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);




	shader.useShaderProgram();
    glUniform1i(glGetUniformLocation(shader.shaderProgram, "reflectionTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.shaderProgram, "refractionTexture"), 1);
    //send light colour and light position
    glUniform1i(dudvMap, 2);
    glUniform1i(normalMap, 3);
}

void Water::renderWater(gps::Shader& shader, glm::mat4 projection, gps::Camera camera, float deltaTime, gps::LightSources& lightSources, WaterFrameBuffers& buffers)
{
	shader.useShaderProgram();
  //  glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    moveFactor += WAVE_SPEED * deltaTime;
    if (moveFactor > 1.0f) moveFactor-=1.0f;
    glUniform1f(moveFactorLoc, moveFactor);


    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMapTextureId);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalMapTextureId);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, buffers.getRefractionDepthTexture());
    glUniform1i(glGetUniformLocation(shader.shaderProgram, "depthMap"), 4);


	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniform3fv(cameraposLoc, 1, glm::value_ptr(camera.getPositionCamera()));


 glUniform3fv(lightPos, 1, glm::value_ptr(lightSources.dirrlight.direction));   
 glUniform3fv(lightColour, 1, glm::value_ptr(lightSources.dirrlight.diffuse));   


	// world transformation and send uniforms
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	//render
	glBindVertexArray(waterVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // glDisable(GL_BLEND);
}


WaterFrameBuffers::WaterFrameBuffers() {
    initialiseReflectionFrameBuffer();
    initialiseRefractionFrameBuffer();
}

WaterFrameBuffers::~WaterFrameBuffers() {
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);

    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}

void WaterFrameBuffers::bindReflectionFrameBuffer() {
    bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffers::bindRefractionFrameBuffer() {
    bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, 1024, 768);
}

void WaterFrameBuffers::initialiseReflectionFrameBuffer() {
    reflectionFrameBuffer = createFrameBuffer();
    reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer() {
    refractionFrameBuffer = createFrameBuffer();
    refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height) {
    glBindTexture(GL_TEXTURE_2D, 0); 
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

GLuint WaterFrameBuffers::createFrameBuffer() {
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return frameBuffer;
}

GLuint WaterFrameBuffers::createTextureAttachment(int width, int height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    return texture;
}

GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    return texture;
}

GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height) {
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    return depthBuffer;
}