#if defined (__APPLE__)
    #define GLFW_INCLUDE_GLCOREARB
    #define GL_SILENCE_DEPRECATION
#else
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/matrix_inverse.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>
#include "Scene.hpp"
#include "LightSources.h"
//////////////////////////// window
gps::Window myWindow;

///////////////////////////// camera
gps::Camera camera(
    glm::vec3(0.0f, 1600.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    4111.0f);
bool firstMouse = true; 
float lastX = myWindow.getWindowDimensions().width / 2; 
float lastY = myWindow.getWindowDimensions().height / 2;

/// ///////////////////////////////////
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
int wireframeon = 0;
bool _fullscreen = 1;
/// ////////////////////////////binds
GLboolean pressedKeys[1024];
bool cursorEnabled = true;

/////////////////////////// shaders
gps::Shader myBasicShader;
gps::Shader skyboxShader;
gps::Shader terrainShader;
gps::Shader waterShader;

///////////////////////////////////////scene related
gps::Scene scene;


// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// shader uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;




GLfloat angle;



//////////////////////////////////////////////////////////////////////////////////////////basic window init and callbacks
void initOpenGLWindow() {
    myWindow.Create(1024, 768, "Sailing the Desert");
}
GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)
void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
    glViewport(0, 0, width, height);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{   
    camera.ProcessMouseScroll((float)yoffset);

}
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) { 
        scene.changeNightDayDirLight(myBasicShader, terrainShader);
    }

    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        if (wireframeon==false)
        {
            wireframeon = true;
            glPolygonMode(GL_FRONT, GL_LINE);
            glPolygonMode(GL_BACK, GL_LINE);
        }
        else
        {
            wireframeon = false;
            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonMode(GL_BACK, GL_FILL);
        }
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        
            glfwSetWindowMonitor(window, _fullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, 1024, 768, GLFW_DONT_CARE);
            _fullscreen = !_fullscreen;


    }
    if (key == GLFW_KEY_C and action == GLFW_PRESS)
        {       if (cursorEnabled)
         {
         glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);//hide mouse
         }
            else{
        glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    cursorEnabled = !cursorEnabled;
    }
            



	if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.rotate(xoffset, yoffset);
}
void setWindowCallbacks() {
    glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
    glfwSetScrollCallback(myWindow.getWindow(), scrollCallback);
}
void initOpenGLState() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
    glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);//hide mouse
}
//////////////////////////////////////////////////////////////////////////////////////init shaders, uniforms, models
void initModels() {

    scene.initLightsModels();

 

}

void initShaders() {
    myBasicShader.loadShader("shaders/basic.vert","shaders/basic.frag");
    skyboxShader.loadShader( "shaders/skybox.vert","shaders/skybox.frag");

    terrainShader.loadMultipleShaders("shaders/tess/vert.in", "shaders/tess/frag.in", nullptr, "shaders/tess/tcs.in", "shaders/tess/tes.in");
    
    waterShader.loadShader("shaders/water.vert", "shaders/water.frag");

}
void initScene()
{   //initialize directional light - bright
    glm::vec3 direction(-11.0f, -5.0f, -1.0f);
    glm::vec3 ambientD(0.1f, 0.1f, 0.15f);
    glm::vec3 diffuseD(1.0f, 0.95f, 0.8f);
    glm::vec3 specularD(1.0f, 1.0f, 1.0f);

    scene.initializeLights(direction, ambientD, diffuseD, specularD);
    scene.initializeSceneObjects();
    scene.initializeSkybox(skyboxShader);

    scene.initTerrain("models/terrain/dessert.png","models/terrain/rough.png", terrainShader);

   

    scene.initWater(waterShader);
}
void initUniforms() {
    myBasicShader.useShaderProgram();


    view = camera.getViewMatrix();
    viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    projection = glm::perspective(glm::radians(45.0f),(float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,10.0f, 150000.0f);
    projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



    scene.lightSources.setLightUniforms(myBasicShader.shaderProgram);
}






void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {
		camera.move(gps::MOVE_FORWARD, deltaTime);

	}

	if (pressedKeys[GLFW_KEY_S]) {
		camera.move(gps::MOVE_BACKWARD, deltaTime);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		camera.move(gps::MOVE_LEFT, deltaTime);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		camera.move(gps::MOVE_RIGHT, deltaTime);

	}

    if (pressedKeys[GLFW_KEY_Q]) {
        angle -= 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    }

    if (pressedKeys[GLFW_KEY_E]) {
        angle += 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
      //  normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

   

   
}





void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //objects all generally share this view, projection matrixes
    view = camera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); //set the values for the uniform containing the view
    projection = glm::perspective(glm::radians(camera.zoom), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 10.0f, 100000.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); //set the projection for the uniform containing the view
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPositionCamera()));

    printf("camera position:x:%f y:%f z:%f\n", camera.getPositionCamera().x, camera.getPositionCamera().y, camera.getPositionCamera().z);


   scene.renderTerrain(terrainShader, projection, camera);
    scene.renderLights(myBasicShader);
    scene.renderSceneObjects(myBasicShader);
    scene.renderWater(waterShader, projection, camera);
    scene.drawSkybox(skyboxShader, camera, projection);
    //skybox rendered last!!


}

void cleanup() {
    myWindow.Delete();
    //delete skybox, moon/sun!!


}

int main(int argc, const char * argv[]) {
    
    try {
        initOpenGLWindow();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
	initModels();
	initShaders();
    initScene();
	initUniforms();
    setWindowCallbacks();
  
	glCheckError();



	while (!glfwWindowShouldClose(myWindow.getWindow())) 
    {
    

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        processMovement();
	    renderScene();





        glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}



	cleanup();
    return EXIT_SUCCESS;
}
