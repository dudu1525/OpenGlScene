#include "Fire.h"

void Fire::initializeFire(gps::Shader shader)
{



	glGenVertexArrays(1, &fireVAO);
	glGenBuffers(1, &fireEBO);
	glGenBuffers(1, &fireVBO);
	glBindVertexArray(fireVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fireEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shader.useShaderProgram();

	//frametime,scrollSpeeds, scales
	glUniform1f(glGetUniformLocation(shader.shaderProgram, "frameTime"), this->frameTime);
	glUniform3fv(glGetUniformLocation(shader.shaderProgram, "scrollSpeeds"), 1,  glm::value_ptr(scrollSpeeds));
	glUniform3fv(glGetUniformLocation(shader.shaderProgram, "scales"), 1,  glm::value_ptr(scales));

	//textures
	glUniform1i(glGetUniformLocation(shader.shaderProgram, "fireTexture"), 4); 
	glUniform1i(glGetUniformLocation(shader.shaderProgram, "noiseTexture"), 5);
	glUniform1i(glGetUniformLocation(shader.shaderProgram, "alphaTexture"), 6);


	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data;
	data = stbi_load(fireTexture, &textureWidth, &textureHeight, &nrChannels, 4);
	glGenTextures(1, &this->fireText);
	glBindTexture(GL_TEXTURE_2D, this->fireText);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);



	data = stbi_load(noiseTexture, &textureWidth, &textureHeight, &nrChannels, 4);
	glGenTextures(1, &this->noiseText);
	glBindTexture(GL_TEXTURE_2D, this->noiseText);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	data = stbi_load(alphaTexture, &textureWidth, &textureHeight, &nrChannels, 4);
	glGenTextures(1, &this->alphaText);
	glBindTexture(GL_TEXTURE_2D, this->alphaText);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);




	//distortion arrays
	glUniform2fv(glGetUniformLocation(shader.shaderProgram, "distortion1"), 1,  glm::value_ptr(distortion1));
	glUniform2fv(glGetUniformLocation(shader.shaderProgram, "distortion2"), 1,  glm::value_ptr(distortion2));
	glUniform2fv(glGetUniformLocation(shader.shaderProgram, "distortion3"), 1, glm::value_ptr(distortion3));
	glUniform1f(glGetUniformLocation(shader.shaderProgram, "distortionBias"), this->distortionBias);
	glUniform1f(glGetUniformLocation(shader.shaderProgram, "distortionScale"), this->distortionScale);
}

void Fire::renderFire(gps::Shader shader, glm::mat4 projection, gps::Camera camera, float deltaTime)
{//need to activate textures!
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fireText);
	glActiveTexture(GL_TEXTURE5); 
	glBindTexture(GL_TEXTURE_2D, noiseText);
	glActiveTexture(GL_TEXTURE6); 
	glBindTexture(GL_TEXTURE_2D, alphaText);

	shader.useShaderProgram();
	//send projection, view, model matrices
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	// world transformation and send uniforms
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	frameTime += deltaTime;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}
	glUniform1f(glGetUniformLocation(shader.shaderProgram, "frameTime"), this->frameTime);
	//render
	glBindVertexArray(fireVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
