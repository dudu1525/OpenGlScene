#ifndef Scene_hpp
#define Scene_hpp
#include <stdbool.h>
#include "LightSources.h"
#include "Shader.hpp"
#include "Entity.h"
#include "Skybox.h"
using glm::vec3;

namespace gps {

	class Scene {


	public:
		Scene();
		LightSources lightSources;
		void initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD);
		void changeNightDayDirLight(Shader shader);
		
		void drawSkybox(gps::Shader shader, gps::Camera camera, glm::mat4 projection);
		void initLightsModels();
		void initializeSkybox(Shader shader);
		void renderLights(Shader lightsshader);
	private:
		bool dayTime=true;
		gps::Model3D sunModel;
		gps::Model3D moonModel;
		Entity sun;
		Entity moon;
		Skybox skybox;

	};



}




#endif