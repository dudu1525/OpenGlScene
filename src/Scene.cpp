#include "Scene.hpp"

#include <cmath>
#include <glm/gtc/noise.hpp>
gps::Scene::Scene()
{
	

}

void gps::Scene::initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD)
{
	//need to be sent once in initialization as it doesent change
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

void gps::Scene::renderWater(Shader waterShader, glm::mat4 projection, gps::Camera camera)
{
	this->water.renderWater(waterShader, projection, camera);
}

void gps::Scene::renderSceneObjects(Shader basicShader)
{
	basicShader.useShaderProgram();
	///////////////////////////////////////////PALM TREES
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < palmtree.size(); i++)
	{
		glm::mat3 palmnormal = glm::mat3(glm::inverseTranspose(palmtree.at(i).getModelMatrix()));
		glUniformMatrix3fv(glGetUniformLocation(basicShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(palmnormal));
		glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(palmtree.at(i).getModelMatrix()));
		palmtree.at(i).getModel()->Draw(basicShader);
	}
	/////////////////////////////////////////////PIER
	glm::mat3 pierNormal = glm::mat3(glm::inverseTranspose(pier.getModelMatrix()));
	glUniformMatrix3fv(glGetUniformLocation(basicShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(pierNormal));
	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pier.getModelMatrix()));
	pier.getModel()->Draw(basicShader);
	////////////////////////////////////////////////tent
	glm::mat3 tentNormal = glm::mat3(glm::inverseTranspose(tent.getModelMatrix()));
	glUniformMatrix3fv(glGetUniformLocation(basicShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(tentNormal));
	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(tent.getModelMatrix()));
	tent.getModel()->Draw(basicShader);
	///////////////////////////////////////////campfire
	glm::mat3 campfireNormal = glm::mat3(glm::inverseTranspose(campfire.getModelMatrix()));
	glUniformMatrix3fv(glGetUniformLocation(basicShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(campfireNormal));
	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(campfire.getModelMatrix()));
	campfire.getModel()->Draw(basicShader);
	////////////////////////////////////////////
	glm::mat3 fernnor = glm::mat3(glm::inverseTranspose(ferns.at(0).getModelMatrix()));
	glUniformMatrix3fv(glGetUniformLocation(basicShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(fernnor));
	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(ferns.at(0).getModelMatrix()));
	ferns.at(0).getModel()->Draw(basicShader);

	glEnable(GL_CULL_FACE);

}

void gps::Scene::positionTrees()
{ //width = width of the terrain on X axis
	//height = size of the terrain on Z axis
	srand(11);//1 or 4 
	//one more idea to make generating more complex and more natural
	// would be to put all the indeces between maxi and mini in another vector, where duplicates exist
	// but duplicate by 100%, 50% the ones closer to the center, so they have a higher chance to appear
	// for example, from 50->65, have multiple 50 to 55s and only one of the 55-65, and choose an index from this vector
	// then use the value given by the index, this way you get controlled randomness
	// 
	// 
	// could also generate multiple ellipses and between the ones closer to the oasis, generate more trees
	// 
	//positive x
	int maxi = REZ - 40;
	int mini = (innerWidth + WIDTH / 2.0f) * (REZ - 1) / WIDTH;//max i for which the width is over innerHeight

	int maxj = REZ - 40;
	int minj = 40;

	//generate x and z between the 2 bounds for x and z
	for (int i=0;i<11;i++)
	{
		int randomI = mini + rand() % (maxi - mini + 1);
		int randomJ = minj + rand() % (maxj - minj + 1);
		float computedX = -WIDTH / 2.0f + (float)randomI * WIDTH / (REZ - 1);
		float computedZ = -HEIGHT / 2.0f + (float)randomJ * HEIGHT / (REZ - 1);
		//generate the y corresponding to those x and z based on the terrain perlin function
		float y = perlinNoise(computedX, computedZ);

		Entity e(&palmtreeModel, glm::vec3(computedX, y, computedZ));
		e.scale = 5;
		palmtree.push_back(e);
	}
	//negative x
	maxi = (-innerWidth + WIDTH / 2.0f) * (REZ - 1) / WIDTH;
	mini = 40;
	for (int i = 0; i < 11; i++)
	{
		int randomI = mini + rand() % (maxi - mini + 1);
		int randomJ = minj + rand() % (maxj - minj + 1);
		float computedX = -WIDTH / 2.0f + (float)randomI * WIDTH / (REZ - 1);
		float computedZ = -HEIGHT / 2.0f + (float)randomJ * HEIGHT / (REZ - 1);
		//generate the y corresponding to those x and z based on the terrain perlin function
		float y = perlinNoise(computedX, computedZ);

		Entity e(&palmtreeModel, glm::vec3(computedX, y, computedZ));
		e.scale = 5;
		palmtree.push_back(e);
	}
	//positive z
	maxi = REZ - 40;
	mini = 45;
	maxj = REZ-40;
	minj = (innerHeight + HEIGHT / 2.0f) * (REZ - 1) / HEIGHT;

	for (int i = 0; i < 10; i++)
	{
		int randomI = mini + rand() % (maxi - mini + 1);
		int randomJ = minj + rand() % (maxj - minj + 1);
		float computedX = -WIDTH / 2.0f + (float)randomI * WIDTH / (REZ - 1);
		float computedZ = -HEIGHT / 2.0f + (float)randomJ * HEIGHT / (REZ - 1);
		//generate the y corresponding to those x and z based on the terrain perlin function
		float y = perlinNoise(computedX, computedZ);

		Entity e(&palmtreeModel, glm::vec3(computedX, y, computedZ));
		e.scale = 5;
		palmtree.push_back(e);
	}
	//negative z
	maxj= (-innerHeight + HEIGHT / 2.0f) * (REZ - 1) / HEIGHT;
	minj = 40;
	for (int i = 0; i < 10; i++)
	{
		int randomI = mini + rand() % (maxi - mini + 1);
		int randomJ = minj + rand() % (maxj - minj + 1);
		float computedX = -WIDTH / 2.0f + (float)randomI * WIDTH / (REZ - 1);
		float computedZ = -HEIGHT / 2.0f + (float)randomJ * HEIGHT / (REZ - 1);
		//generate the y corresponding to those x and z based on the terrain perlin function
		float y = perlinNoise(computedX, computedZ);

		Entity e(&palmtreeModel, glm::vec3(computedX, y, computedZ));
		e.scale = 5;
		palmtree.push_back(e);
	}


}


float gps::Scene::perlinNoise(float x, float z)
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

	float finalHeight = pow(mountains, 1.5f) * maxh; //before marking the 'small lake'
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

void gps::Scene::initLightsModels()
{
	sunModel.LoadModel("models/lights/sphere2.obj");
	moonModel.LoadModel("models/lights/sphere.obj");
	this->sun = Entity(&sunModel, glm::vec3(0.0f, 3150.0f, 0.0f));
	this->moon = Entity(&moonModel, glm::vec3(0.0f, 3150.0f, 0.0f));


	this->sun.scale = 150;
	this->moon.scale = 150;

}

void gps::Scene::initTerrain(const char* texturePath, const char* roughTexture,gps::Shader shader )
{
	terrain.initializeTerrain(texturePath, roughTexture, shader, lightSources);

	
}

void gps::Scene::initializeSkybox(gps::Shader shader)
{
	skybox.initializeSkybox( shader);

}

void gps::Scene::initializeSceneObjects()
{	//load models
	palmtreeModel.LoadModel("models/trees/palm_tree.obj");
	tropicalfern2.LoadModel("models/weeds/TropicFern01.obj");
	pierModel.LoadModel("models/outerworld/pier.obj");
	campfireModel.LoadModel("models/outerworld/campfire.obj");
	tentModel.LoadModel("models/outerworld/tent/tent.obj");
	//pier
	pier= Entity(&pierModel, glm::vec3(1200.0, -280.0f,3900.0f ));
	pier.rotation = glm::vec3(0, 270, 0);
	pier.scale = 120;
	pier.scaleY = 1.5f;
	//tent
	tent = Entity(&tentModel, glm::vec3(4800.0f, 220.0f, 8300.0f));
	tent.rotation = glm::vec3(0, 200, 0);
	glm::vec3 normal = terrain.getNormal(4800.0f, 8300.0f);
	tent.updateRotationFromNormal(normal);
	tent.scale = 555.0;
	///campfire
	campfire = Entity(&campfireModel, glm::vec3(4300.0, 312.0f, 6300.0f));
	campfire.rotation = glm::vec3(0, 290, 0);
	campfire.scale = 120;

	///////
	Entity e(&tropicalfern2, glm::vec3(7000.0, 405.0, 1010.0f));
	e.scale = 200;
	e.rotation = glm::vec3(-90, 0, 0);
	ferns.push_back(e);

	positionTrees();
}

void gps::Scene::initWater(gps::Shader waterShader)
{
	this->water.initializeWater(waterShader);
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
