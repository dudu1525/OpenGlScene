#ifndef Scene_hpp
#define Scene_hpp
#include <stdbool.h>

#include "LightSources.h"
#include "Shader.hpp"
#include "Entity.h"
#include "Skybox.h"
#include "Camera.hpp"
#include "Terrain.h"
#include "Water.h"
using glm::vec3;
//+x -> shadowed part on mountains when light
namespace gps {

	class Scene {
#define WIDTH 48048
#define HEIGHT 48048
#define REZ 128
	public:
		Scene();
		LightSources lightSources;

		void changeNightDayDirLight(Shader shader, Shader terrainShader);

		
		/////////////////////////////////////////////initializations

		void initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD);
		void initLightsModels();
		void initTerrain(const char* texturePath,  const char* roughTexture, gps::Shader shader);
		void initializeSkybox(Shader shader);
		void initializeSceneObjects();
		void initWater(gps::Shader waterShader);


		/// //////////////////////////////////////////////////////rendering

		void renderLights(Shader lightsshader);
		void drawSkybox(gps::Shader shader, gps::Camera camera, glm::mat4 projection);
		void renderTerrain(Shader terrainShader, glm::mat4 projection, gps::Camera camera);
		void renderWater(Shader waterShader, glm::mat4 projection, gps::Camera camera);
		void renderTrees(Shader basicShader);
		///////////////////////////////////////////////////////////////misc
		//x - width, z - height  ->perpendicular to the longer part of the oasis
		int outerWidth=13000, outerHeight=12000, innerWidth=5300, innerHeight=3600;//+- 
		void positionTrees();
		int maxh = 4449; //from terrain.cpp, height multiplier
	private:
		bool dayTime=true;
		gps::Model3D sunModel;
		gps::Model3D moonModel;
		Entity sun;
		Entity moon;
		Skybox skybox;
		Terrain terrain;
		Water water;
	
		gps::Model3D palmtreeModel;
		std::vector<Entity> palmtree;//load more of these and displace them


		////////////////////////////////////////////////helper functions
		float perlinNoise(float x, float z);

	};



}




#endif