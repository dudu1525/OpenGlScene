#ifndef ERROR_REPORTING_H
#define ERROR_REPORTING_H

#include <GL/glew.h>
//https://learnopengl.com/In-Practice/Debugging
// Function to initialize the debug logger
void enableReportGlErrors();

// The callback function itself (needs to be visible if you use it elsewhere, 
// but usually it's kept private in the .cpp)
void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);

#endif