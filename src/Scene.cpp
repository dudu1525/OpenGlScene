#include "Scene.hpp"

gps::Scene::Scene()
{
	

}

void gps::Scene::initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD)
{

	this->lightSources.setDirecLightParameters(direction, ambientD, diffuseD, specularD);
}

void gps::Scene::changeNightDayDirLight(Shader shader, Shader terrainShader)
{
	shader.useShaderProgram();
	if (dayTime)
	{
		dayTime = false;
		glm::vec3 direction(5.0f, -2.0f, -1.0f);
		glm::vec3 ambientD(0.02f, 0.02f, 0.08f);
		glm::vec3 diffuseD(0.15f, 0.15f, 0.25f);
		glm::vec3 specularD(0.2f, 0.2f, 0.3f);
		this->lightSources.setDirecLightParameters(direction, ambientD, diffuseD, specularD);
	}
	else
	{
		dayTime = true;
		glm::vec3 direction(-5.0f, -2.0f, -1.0f);
		glm::vec3 ambientD(0.1f, 0.1f, 0.15f);
		glm::vec3 diffuseD(1.0f, 0.95f, 0.8f);
		glm::vec3 specularD(1.0f, 1.0f, 1.0f);
		this->lightSources.setDirecLightParameters(direction, ambientD, diffuseD, specularD);
	}
	lightSources.setLightUniforms(shader.shaderProgram);
	terrain.setLightUniforms(terrainShader, lightSources);
	skybox.changeFaces(!dayTime);
}

void gps::Scene::drawSkybox(gps::Shader shader, gps::Camera camera, glm::mat4 projection)
{

	skybox.drawSkybox(shader, camera, projection);


}

void gps::Scene::renderTerrain(Shader terrainShader, glm::mat4 projection,gps::Camera camera)
{
	terrain.renderTerrain(terrainShader, projection, camera);
}

void gps::Scene::initLightsModels()
{
	sunModel.LoadModel("models/lights/sphere2.obj");
	moonModel.LoadModel("models/lights/sphere.obj");
	this->sun = Entity(&sunModel, glm::vec3(0.0f, 50.0f, 0.0f));
	this->moon = Entity(&moonModel, glm::vec3(0.0f, 50.0f, 0.0f));
	this->sun.scale = 50;
	this->moon.scale = 50;
}

void gps::Scene::initTerrain(const char* texturePath,gps::Shader shader )
{
//	terrain.loadFromFile("models/terrain/heightmap.save");
	terrain.initializeTerrain(texturePath, shader, lightSources);

	
}

void gps::Scene::initializeSkybox(gps::Shader shader)
{
	skybox.initializeSkybox( shader);

}

void gps::Scene::renderLights(Shader lightsshader)
{
	lightsshader.useShaderProgram();

	if (dayTime)//render sun
	{
	
		glUniformMatrix4fv(glGetUniformLocation(lightsshader.shaderProgram, "model"), 1,GL_FALSE, glm::value_ptr(sun.getModelMatrix())  );
		glm::mat3 sunNormal = glm::mat3(glm::inverseTranspose(sun.getModelMatrix()));
		glUniformMatrix3fv(glGetUniformLocation(lightsshader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(sunNormal));
		
		sun.getModel()->Draw(lightsshader);

	}
	else//render moon
	{
		glUniformMatrix4fv(glGetUniformLocation(lightsshader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(moon.getModelMatrix()));
		glm::mat3 moonNormal = glm::mat3(glm::inverseTranspose(moon.getModelMatrix()));
		glUniformMatrix3fv(glGetUniformLocation(lightsshader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(moonNormal));

		moon.getModel()->Draw(lightsshader);


	}




}
