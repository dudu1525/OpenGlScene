#ifndef Scene_hpp
#define Scene_hpp

#include "LightSources.h"

using glm::vec3;

namespace gps {

	class Scene {


	public:
		Scene();
		LightSources lightSources;
		void initializeLights(vec3 direction, vec3 ambientD, vec3 diffuseD, vec3 specularD);

	private:


	};



}




#endif