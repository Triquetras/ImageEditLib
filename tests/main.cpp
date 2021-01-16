// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
// #include "framebuffer.h"

#include "imagearch.h"
#include "utils.h"
#include <stb_image.h>



// bool LoadTextureFromMemory(unsigned char* image_data, GLuint* out_texture, int* out_width, int* out_height, int size, int image_width, int image_height)
// {

//     unsigned char* data;
//     if (image_width == -1 || image_height == -1) {
//         data = stbi_load_from_memory(image_data, size, &image_width, &image_height, NULL, 4);
//         if (data == NULL)
//             return false;
//     } else {
//         data = image_data;
//     }

//     // Create a OpenGL texture identifier
//     GLuint image_texture;
//     glGenTextures(1, &image_texture);
//     glBindTexture(GL_TEXTURE_2D, image_texture);

//     // Setup filtering parameters for display
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     // Upload pixels into texture
//     glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

//     *out_texture = image_texture;
//     *out_width = image_width;
//     *out_height = image_height;

//     return true;
// }

// bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
// {
//     // Load from file
//     int image_width = 0;
//     int image_height = 0;
//     unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
//     if (image_data == NULL)
//         return false;

//     bool ret =  LoadTextureFromMemory(image_data, out_texture, out_width, out_height, 0, image_width, image_height);
//     stbi_image_free(image_data);

//     return ret;
// }



int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "libsublirip test", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "./src/shaders/TransformVertexShader.vertexshader", "./src/shaders/ColorFragmentShader.fragmentshader" );


   // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // GLfloat g_vertex_buffer_data[] = {
    //     0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
    //     0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
    // };

    GLfloat g_vertex_buffer_data[] = {
        //  Position           Texcoords
        -1.f,  1.f, 0.0f, 0.0f, 0.0f, // Top-left
        1.f,  1.f,  0.0f, 1.0f, 0.0f, // Top-right
        1.f, -1.f,  0.0f, 1.0f, 1.0f, // Bottom-right
        -1.f, -1.f, 0.0f, 0.0f, 1.0f  // Bottom-left
    };

    GLuint _index;
    glGenBuffers(1, &_index);
    glBindBuffer(GL_ARRAY_BUFFER, _index);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(programID, "vertex");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    GLint texAttrib = glGetAttribLocation(programID, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    //ImageArch::CreateLine::Request r1 { 0x0000ff00, 1.0 };
    // ImageArch::LayerSet canvas(1024, 768, "test canva");
    // ImageArch::OperationManager m;
    // auto lineOp = m.GetOperation("TCreateLine");
    // int col = 0x0000ff00;
    // float thickness = 1.0;
    // lineOp->SetParameter(0, (void*)&col, false);
    // lineOp->SetParameter(1, (void*)&thickness, false);
    // lineOp->SetParameter(ImageArch::E_PARAM_LAYER, canvas.CurrentLayer());
    // auto layer = canvas.CurrentLayer();
    // layer->AddOperation(lineOp);
    // canvas.SetRepresenter(lineOp->GetRepresenter());
    // canvas.OnPrimaryMouseButtonDown(false, false, false, false);
    // canvas.OnMouseMove(100, 100);
    // canvas.OnMouseMove(1000, 500);
    // int size;
    // auto pp = lineOp->GetParameters(size);
    // canvas.GetImage();

    ImageArch::OperationManager mng;
    auto opLine = mng.GetOperation("TCreateLine");
    auto op1 = mng.GetOperation("TMove");
    auto op2 = mng.GetOperation("TImage");
    auto op3 = mng.GetOperation("TScale");

    //GLuint tex;
    //int w, h;
    //ImageArch::LoadTextureFromFile("./TestXW.png", &tex, &w, &h);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        glBindBuffer(GL_ARRAY_BUFFER, _index);
        glEnableVertexAttribArray(posAttrib);
        glEnableVertexAttribArray(texAttrib);

        //glDrawArrays(GL_TRIANGLES, 0, 4);
        glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
        //glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, _data);

		glDisableVertexAttribArray(posAttrib);
		glDisableVertexAttribArray(texAttrib);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
