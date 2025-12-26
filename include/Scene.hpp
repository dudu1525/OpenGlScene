#ifndef Scene_hpp
#define Scene_hpp
#include <stdbool.h>
#include "LightSources.h"
#include "Shader.hpp"
#include "Model3D.hpp"
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
		void renderLights(Shader lightsshader, glm::mat4 view);
	private:
		bool dayTime=true;
		gps::Model3D sun;
		gps::Model3D moon;
		Skybox skybox;

	};



}




#endif