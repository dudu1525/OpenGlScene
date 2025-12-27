#ifndef Scene_hpp
#define Scene_hpp
#include <stdbool.h>
#include "LightSources.h"
#include "Shader.hpp"
#include "Entity.h"
#include "Skybox.h"
#include "Shader.hpp"
#include "Terrain.h"
using glm::vec3;

namespace gps {

	class Scene {


	public:
		Scene();
		LightSources lightSources;

		void changeNightDayDirLight(Shader shader);



		void renderLights(Shader lightsshader);
		void drawSkybox(gps::Shader shader, gps::Camera camera, glm::mat4 projection);
		void renderTerrain(Shader terrainShader, glm::mat4 projection, glm::mat4 view);


		void initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD);
		void initLightsModels();
		void initTerrain(const char* heightmap, gps::Shader shader);
		void initializeSkybox(Shader shader);
		
	private:
		bool dayTime=true;
		gps::Model3D sunModel;
		gps::Model3D moonModel;
		Entity sun;
		Entity moon;
		Skybox skybox;
		Terrain terrain;

	};



}




#endif