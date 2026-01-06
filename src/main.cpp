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
#include "Water.h"
#include <random>
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
float fireTime = 0.0f;//used to change the color of the point light around the fire

/// ////////////////////////////binds
GLboolean pressedKeys[1024];
bool cursorEnabled = true;

/////////////////////////// shaders
gps::Shader myBasicShader;
gps::Shader skyboxShader;
gps::Shader terrainShader;
gps::Shader waterShader;
gps::Shader fireShader;
gps::Shader shadowShader;
///////////////////////////////////////scene related
gps::Scene scene;
//////////////////////////////////////SHADOWS made with: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
unsigned int depthMapFBO;
unsigned int depthMap;

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
    glEnable(GL_CLIP_DISTANCE0);
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

    fireShader.loadShader("shaders/fire.vert", "shaders/fire.frag");

    shadowShader.loadShader("shaders/shadows.vert", "shaders/shadows.frag");


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

    scene.initFire(fireShader);

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





void renderScene(float deltaTime, WaterFrameBuffers& buffers) {

    //objects all generally share this view, projection matrixes
    view = camera.getViewMatrix();
    myBasicShader.useShaderProgram();
    fireTime += deltaTime;
    if (fireTime >= 0.3f)
    {
      
        float min = 0.0014;
        float max = 0.014;
        float randomval = min + (float)(rand()) / ((float)(0x7fff / (max - min)));

        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointlight.linear"), randomval);

        terrainShader.useShaderProgram();
        glUniform1f(glGetUniformLocation(terrainShader.shaderProgram, "pointlight.linear"), randomval);
        fireTime = 0.0f;

    }
    myBasicShader.useShaderProgram();

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); //set the values for the uniform containing the view
    projection = glm::perspective(glm::radians(camera.zoom), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 10.0f, 100000.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); //set the projection for the uniform containing the view
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPositionCamera()));

    printf("camera position:x:%f y:%f z:%f\n", camera.getPositionCamera().x, camera.getPositionCamera().y, camera.getPositionCamera().z);


   scene.renderTerrain(terrainShader, projection, camera);

    scene.renderLights(myBasicShader);

    scene.renderSceneObjects(myBasicShader);
    scene.renderFire(fireShader, projection, camera, deltaTime);


    waterShader.useShaderProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffers.getReflectionTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, buffers.getRefractionTexture());
    scene.renderWater(waterShader, projection, camera);


    scene.drawSkybox(skyboxShader, camera, projection);
    //skybox rendered last!!


}
void initializeShadows()
{
    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    myBasicShader.useShaderProgram();
    glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "shadowMap"), 7); //set on 1?


    terrainShader.useShaderProgram();
    glUniform1i(glGetUniformLocation(terrainShader.shaderProgram, "shadowMap"), 7);
    scene.initSimpleModels(shadowShader);//initialize simple terrain
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

    initializeShadows();

    WaterFrameBuffers buffers;//init buffers
    int waterHeight = -3.0f;//for water reflect+refract


	while (!glfwWindowShouldClose(myWindow.getWindow())) 
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ////////////////////////////////////////////////light POV
        glm::vec3 lightDir = glm::normalize(glm::vec3(-10.0f, -7.0f, -3.0f));
        if (scene.dayTime)
            lightDir = glm::normalize(glm::vec3(-10.0f, -7.0f, -3.0f));
        else
            lightDir = glm::normalize(glm::vec3(5.0f, -2.0f, -1.0f));
 
        glm::vec3 lightPos = -lightDir * 35000.0f; //far away sun
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightProjection = glm::ortho(-30000.0f, 30000.0f, -30000.0f, 30000.0f, 0.1f, 80000.0f);
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        shadowShader.useShaderProgram();
        glUniformMatrix4fv(glGetUniformLocation(shadowShader.shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        glViewport(0, 0, 4096, 4096);
       glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
       // glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        scene.renderOnlyModels(shadowShader); //RENDER SCENE WITH ONLY OBJECTS AS MODELS!
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        //first reflection pass
        buffers.bindReflectionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float distance = 2 * (camera.getPositionCamera().y - waterHeight);
        camera.setPosition(camera.getPositionCamera() - glm::vec3(0, distance, 0));
        camera.invertPitch(); 
        view = camera.getViewMatrix();//!UPDATE CAMERA AND SEND IT
        glm::vec4 reflectionPlane = glm::vec4(0, 1, 0, -waterHeight); 
        myBasicShader.useShaderProgram();
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPositionCamera()));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        scene.renderSceneObjectsWithPlane(reflectionPlane, terrainShader, myBasicShader, projection, camera);

        //reset
        camera.setPosition(camera.getPositionCamera() + glm::vec3(0, distance, 0));
        camera.invertPitch();
      

        //refraction pass
        buffers.bindRefractionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec4 refractionPlane = glm::vec4(0, -1, 0, waterHeight);
        scene.renderSceneObjectsWithPlane( refractionPlane, terrainShader, myBasicShader, projection, camera);


        //glDisable(gl_clip_distance0?)
        buffers.unbindCurrentFrameBuffer();
        ////////////////////////////////////////////normal POV
        glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       myBasicShader.useShaderProgram();  //!!!!!!!NEED TO UPDATE VERTEX+FRAGMENT SHADER WITH LIGHTSPACE, LIGHTPOS, SHADOWMAP
        glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));


        terrainShader.useShaderProgram();
        glUniformMatrix4fv(glGetUniformLocation(terrainShader.shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        //activate depthmap which is set to slot 7
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        processMovement();
	    renderScene( deltaTime, buffers);





        glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}



	cleanup();
    return EXIT_SUCCESS;
}
