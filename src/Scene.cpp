#include "Scene.hpp"

gps::Scene::Scene()
{
	
	//moon.LoadModel("models/lights/sphere.obj");

}

void gps::Scene::initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD)
{

	this->lightSources.setDirecLightParameters(direction, ambientD, diffuseD, specularD);
}

void gps::Scene::changeNightDayDirLight(Shader shader)
{
	shader.useShaderProgram();
	if (dayTime)
	{
		dayTime = false;
		glm::vec3 direction(-5.0f, -2.0f, -1.0f);
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
	skybox.changeFaces(!dayTime);
}

void gps::Scene::drawSkybox(gps::Shader shader, gps::Camera camera, glm::mat4 projection)
{

	skybox.drawSkybox(shader, camera, projection);


}

void gps::Scene::initLightsModels()
{
	sun.LoadModel("models/lights/sphere2.obj");
	moon.LoadModel("models/lights/sphere.obj");
}

void gps::Scene::initializeSkybox(gps::Shader shader)
{
	skybox.initializeSkybox( shader);

}

void gps::Scene::renderLights(Shader lightsshader, glm::mat4 view)
{
	lightsshader.useShaderProgram();

	if (dayTime)//render sun
	{
		glm::mat4 sunModel = glm::translate(glm::mat4(1.0f), glm::vec3(72.0f, 11.0f, -2.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightsshader.shaderProgram, "model"), 1,GL_FALSE, glm::value_ptr(sunModel)  );
		glm::mat3 sunNormal = glm::mat3(glm::inverseTranspose(view * sunModel));
		glUniformMatrix3fv(glGetUniformLocation(lightsshader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(sunNormal));
		
		sun.Draw(lightsshader);

	}
	else//render moon
	{
		glm::mat4 moonModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightsshader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
		glm::mat3 moonNormal = glm::mat3(glm::inverseTranspose(view * moonModel));
		glUniformMatrix3fv(glGetUniformLocation(lightsshader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(moonNormal));

		moon.Draw(lightsshader);


	}




}
