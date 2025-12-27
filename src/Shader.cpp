//
//  Shader.cpp
//  Lab3
//
//  Created by CGIS on 05/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "Shader.hpp"

namespace gps {
    std::string Shader::readShaderFile(std::string fileName) {

        std::ifstream shaderFile;
        std::string shaderString;
        
        //open shader file
        shaderFile.open(fileName);
        
        std::stringstream shaderStringStream;
        
        //read shader content into stream
        shaderStringStream << shaderFile.rdbuf();
        
        //close shader file
        shaderFile.close();
        
        //convert stream into GLchar array
        shaderString = shaderStringStream.str();
        return shaderString;
    }
    
    void Shader::shaderCompileLog(GLuint shaderId) {

        GLint success;
        GLchar infoLog[512];
        
        //check compilation info
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if(!success) {

            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "Shader compilation error\n" << infoLog << std::endl;
        }
    }
    
    void Shader::shaderLinkLog(GLuint shaderProgramId) {

        GLint success;
        GLchar infoLog[512];
        
        //check linking info
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "Shader linking error\n" << infoLog << std::endl;
        }
    }
    
    void Shader::loadShader(std::string vertexShaderFileName, std::string fragmentShaderFileName) {

        //read, parse and compile the vertex shader
        std::string v = readShaderFile(vertexShaderFileName);
        const GLchar* vertexShaderString = v.c_str();
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
        glCompileShader(vertexShader);
        //check compilation status
        shaderCompileLog(vertexShader);
        
        //read, parse and compile the vertex shader
        std::string f = readShaderFile(fragmentShaderFileName);
        const GLchar* fragmentShaderString = f.c_str();
        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
        glCompileShader(fragmentShader);
        //check compilation status
        shaderCompileLog(fragmentShader);
        
        //attach and link the shader programs
        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, vertexShader);
        glAttachShader(this->shaderProgram, fragmentShader);
        glLinkProgram(this->shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //check linking info
        shaderLinkLog(this->shaderProgram);
    }
    //from learnopengl.com
void Shader::loadMultipleShaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr)
    {

            // 1. retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::string geometryCode;
            std::string tessControlCode;
            std::string tessEvalCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            std::ifstream gShaderFile;
            std::ifstream tcShaderFile;
            std::ifstream teShaderFile;

            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();

                if (geometryPath != nullptr)
                {
                    gShaderFile.open(geometryPath);
                    std::stringstream gShaderStream;
                    gShaderStream << gShaderFile.rdbuf();
                    gShaderFile.close();
                    geometryCode = gShaderStream.str();
                }
                if (tessControlPath != nullptr)
                {
                    tcShaderFile.open(tessControlPath);
                    std::stringstream tcShaderStream;
                    tcShaderStream << tcShaderFile.rdbuf();
                    tcShaderFile.close();
                    tessControlCode = tcShaderStream.str();
                }
                if (tessEvalPath != nullptr)
                {
                    teShaderFile.open(tessEvalPath);
                    std::stringstream teShaderStream;
                    teShaderStream << teShaderFile.rdbuf();
                    teShaderFile.close();
                    tessEvalCode = teShaderStream.str();
                }
            }
            catch (std::ifstream::failure& e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
           // checkCompileErrors(vertex, "VERTEX");
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
           // checkCompileErrors(fragment, "FRAGMENT");

            // if geometry shader is given, compile geometry shader
            unsigned int geometry;
            if (geometryPath != nullptr)
            {
                const char* gShaderCode = geometryCode.c_str();
                geometry = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometry, 1, &gShaderCode, NULL);
                glCompileShader(geometry);
                //checkCompileErrors(geometry, "GEOMETRY");
            }

            // if tessellation shaders are given, compile them
            unsigned int tessControl, tessEval;
            if (tessControlPath != nullptr)
            {
                const char* tcShaderCode = tessControlCode.c_str();
                tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
                glShaderSource(tessControl, 1, &tcShaderCode, NULL);
                glCompileShader(tessControl);
               // checkCompileErrors(tessControl, "TESS_CONTROL");
            }
            if (tessEvalPath != nullptr)
            {
                const char* teShaderCode = tessEvalCode.c_str();
                tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
                glShaderSource(tessEval, 1, &teShaderCode, NULL);
                glCompileShader(tessEval);
               // checkCompileErrors(tessEval, "TESS_EVALUATION");
            }

            // shader Program
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertex);
            glAttachShader(shaderProgram, fragment);
            if (geometryPath != nullptr)
                glAttachShader(shaderProgram, geometry);
            if (tessControlPath != nullptr)
                glAttachShader(shaderProgram, tessControl);
            if (tessEvalPath != nullptr)
                glAttachShader(shaderProgram, tessEval);
            glLinkProgram(shaderProgram);
            //checkCompileErrors(shaderProgram, "PROGRAM");

            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            if (geometryPath != nullptr)
                glDeleteShader(geometry);
            if (tessControlPath != nullptr)
                glDeleteShader(tessControl);
            if (tessEvalPath != nullptr)
                glDeleteShader(tessEval);
        
            std::cout << "Succesfully loaded multiple shaders!" << std::endl;
 }

    void Shader::useShaderProgram() {

        glUseProgram(this->shaderProgram);
    }

}
