//
//  main.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/12/22.
//

#include <iostream>
#include <vector>


// CODE_SIGN_IDENTITY gets set to '-' this refuses to launch...

// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "shaders.h"
#include "proc_textures.h"
#include "pngreader.h"
#include "cube.h"
#include "sphere.h"
#include "pyramid.h"
#include "mathutil.h"

const char* kVertexShaderPath = "vertex_shader.glsl";
const char* kFragmentShaderPath = "fragment_shader.glsl";

const RGBColor red = {255, 0, 0};
const RGBColor blue = {0, 0, 255};
const RGBColor green = {0, 255, 0};
const RGBColor white = {255, 255, 255};
const RGBColor orange = {232, 99, 10};

static const GLfloat gTriangleUVBufferData[] = {
    0, 0,
    1, 0,
    0.5, 1
};



static bool RunTests(void)
{
    bool good = true;
    
    good = good && TestGenerateCheckers();
    
    return good;
}

static GLuint CreateTextureFromImage(RGBImageBuffer* imageBuffer)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB,
                 imageBuffer->Width(), imageBuffer->Height(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE, imageBuffer->Pixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return textureID;
}

static RGBImageBuffer* LoadImageBufferFromPNG(const char* path)
{
    uint32_t width, height;
    uint8_t *pixels = nullptr;
    
    if(!ReadPNGImage(path, &pixels, &width, &height)) {
        std::cerr << "Error loading image from " << path << std::endl;
        return nullptr;
    }
    
    return new RGBImageBuffer(pixels, width, height, width*3, 3);
}


int main(int argc, const char** argv)
{
    if(!RunTests()) {
        std::cerr << "Some tests failed" << std::endl;
        return 1;
    }
    
    const GLint  kWindowWidth = 1024;
    const GLint kWindowHeight = 768;

    std::cout << "GL setup example\n";

    if (!glfwInit())
        return 1;

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    
    // Required to work on Mac.
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    // Disallow resizing.
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(kWindowWidth, kWindowHeight, "OpenGL Setup Example", nullptr, nullptr);
    
    // may differ than expected due to Retina display
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    if( window == nullptr ) {
        std::cerr << "Failed creating window with GLFW" << std::endl;
        getchar(); // wait on keypress
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    
    glClearColor((GLfloat)0.5f, (GLfloat)0.5f, (GLfloat)0.5f, (GLfloat)1.0f);

    GLuint program;
    if(!InitShader(program, kVertexShaderPath, kFragmentShaderPath)) {
        std::cerr << "Could not initialize shaders" << std::endl;
        return 1;
    }

    const float fovyInDegrees = 45.0f;
    const float aspectRatio = (float) screenWidth / (float)screenHeight;
    const float znear = 0.001f;
    const float zfar = 100.0f;

    glm::mat4 Projection = glm::perspective(glm::radians(fovyInDegrees), aspectRatio, znear, zfar);
    
    glm::mat4 View = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), // position
        glm::vec3(0.0f, 0.0f, 0.0f), // target
        glm::vec3(0.0f, 1.0f, 0.0f) //  up vector
        );

    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
    
    
    // GLint mvpUniformLocation = glGetUniformLocation(program, "mvp_matrix");
    GLint mvUniformLocation = glGetUniformLocation(program, "mvMatrix");
    if (mvUniformLocation == -1) {
        std::cerr << "Could not bind mvUniformLocation" << std::endl;
    }
    GLint projUniformLocation = glGetUniformLocation(program, "projMatrix");
    if (projUniformLocation == -1) {
        std::cerr << "Could not bind projUniformLocation" << std::endl;
    }
    
    GLint aPositionLocation = glGetAttribLocation(program, "a_position");
    if (aPositionLocation == -1) {
        std::cerr << "Could not bind attribute a_position" << std::endl;
    }
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat triVertexBufferData[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    
    GLuint triVertexBuffer;
    glGenBuffers(1, &triVertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, triVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVertexBufferData), triVertexBufferData, GL_STATIC_DRAW);
    
    std::vector<float> cubeVertexData, cubeUVData;
    GenerateCube(cubeVertexData, cubeUVData);
    
    GLuint cubeVertexBuffer;
    glGenBuffers(1, &cubeVertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeVertexData.size() * sizeof(float), cubeVertexData.data(), GL_STATIC_DRAW);
    
    GLuint cubeTwoVertexBuffer;
    glGenBuffers(1, &cubeTwoVertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeTwoVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeVertexData.size() * sizeof(float), cubeVertexData.data(), GL_STATIC_DRAW);
    
    
    std::cout << "Loading textures" << std::endl;
    
    RGBImageBuffer *rockyTextureImage = LoadImageBufferFromPNG("textures/rocky.png");
    assert(rockyTextureImage);
    GLuint rockyTextureImageID = CreateTextureFromImage(rockyTextureImage);
    
    std::cout  << "Generating fractal browning motion" << std::endl;
    
    RGBImageBuffer* cubeTextureImage = GenerateFractalBrownianMotion(256 /*512*/, orange, 0.8, 1.8, 3.0, -0.5, 0.5, 64, false);
    assert(cubeTextureImage);
    
    GLuint cubeTextureID = CreateTextureFromImage(cubeTextureImage);
    
    GLuint cubeUVBuffer;
    glGenBuffers(1, &cubeUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeUVData.size() * sizeof(float), cubeUVData.data(), GL_STATIC_DRAW);
    
    std::cout  << "Generating marble" << std::endl;
    RGBImageBuffer* triTextureImage = GenerateMarble(512, blue, white);
    assert(triTextureImage);
    
    GLuint triTextureID = CreateTextureFromImage(triTextureImage);
    

    GLuint triangleUVBuffer;
    glGenBuffers(1, &triangleUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gTriangleUVBufferData), gTriangleUVBufferData, GL_STATIC_DRAW);
    
    
    std::vector<float> sphereVertices, sphereNormals, sphereUVData;
    std::vector<int> sphereIndices;
    GenerateSphere(1.5, 10, 12, sphereVertices, sphereNormals, sphereUVData, sphereIndices);
    
    GLuint sphereVertexBuffer;
    glGenBuffers(1, &sphereVertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
    
    GLuint sphereUVBuffer;
    glGenBuffers(1, &sphereUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphereUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sphereUVData.size() * sizeof(float), sphereUVData.data(), GL_STATIC_DRAW);
    
    GLuint sphereIndexBuffer;
    glGenBuffers(1, &sphereIndexBuffer);
    assert(sizeof(GLuint) == sizeof(int)); // TODO:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), sphereIndices.data(), GL_STATIC_DRAW);
    
    
    RGBImageBuffer *marsTextureImage = LoadImageBufferFromPNG("textures/mars.png");
    assert(marsTextureImage);
    GLuint marsTextureImageID = CreateTextureFromImage(marsTextureImage);
    
    std::vector<float> pyramidVertexData, pyramidUVData;
    GeneratePyramid(pyramidVertexData, pyramidUVData);
    
    GLuint pyramidVertexBuffer;
    glGenBuffers(1, &pyramidVertexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, pyramidVertexData.size() * sizeof(float), pyramidVertexData.data(), GL_STATIC_DRAW);
    
    
    GLuint pyramidUVBuffer;
    glGenBuffers(1, &pyramidUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, pyramidUVData.size() * sizeof(float), pyramidUVData.data(), GL_STATIC_DRAW);
    
    RGBImageBuffer* checkersTextureImage = GenerateCheckers(1024, 32, blue, green);
    assert(checkersTextureImage);
    
    GLuint checkersTextureID = CreateTextureFromImage(checkersTextureImage);
    
    
    
    
    float triAngle = 0.0f;
    float cubeAngle = 0.0f;
    float sphereAngle = 0.0f;
    float pyramidAngle = 0.0f;
    const float kTriRotSpeed = 0.02;
    const float kCubeRotSpeed = 0.03;
    const float kSphereRotSpeed = 0.01;
    const float kPyramidRotSpeed = 0.025;
    
    std::cout  << "starting main loop" << std::endl;
    while( !glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(program);
        
        glEnableVertexAttribArray(aPositionLocation);
        
        if (1) { // Triangle
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, triAngle, glm::vec3(0, 0.2, 1));
            
            // glm::mat4 mvp = Projection * View * Model;
            glm::mat4 mv = View * Model;
            glUniformMatrix4fv(mvUniformLocation, 1, GL_FALSE, glm::value_ptr(mv));
            glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(Projection));
            
            glBindBuffer(GL_ARRAY_BUFFER, triVertexBuffer);
            glVertexAttribPointer(
               0, // idx
               3, // size
               GL_FLOAT, // type
               GL_FALSE, // normalized
               0, // stride
               (void*)0 // pointer
            );
            
            glBindBuffer(GL_ARRAY_BUFFER, triangleUVBuffer);
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, triTextureID);
            
            
            glDrawArrays(GL_TRIANGLES, 0, 3);
            
            triAngle += kTriRotSpeed;
        }
        if(1){ // Cube one
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(-2.5, 0, -4));
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            glm::mat4 mv = View * Model;
            glUniformMatrix4fv(mvUniformLocation, 1, GL_FALSE, glm::value_ptr(mv));
            glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(Projection));
            
            glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
            glVertexAttribPointer(
               0,
               3,
               GL_FLOAT,
               GL_FALSE,
               0,
               (void*)0
            );
 
            glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cubeTextureID);
            
            
            // cube is 12 triangles, 2 each for 6 sides
            glDrawArrays(GL_TRIANGLES, 0, 12*3);
            
            cubeAngle += kCubeRotSpeed;
        }
        
        if(1)
        { // Cube two
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(2.5, 0, -4));
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            glm::mat4 mv = View * Model;
            glUniformMatrix4fv(mvUniformLocation, 1, GL_FALSE, glm::value_ptr(mv));
            glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(Projection));
            
            
            glBindBuffer(GL_ARRAY_BUFFER, cubeTwoVertexBuffer);
            glVertexAttribPointer(
               0,
               3,
               GL_FLOAT,
               GL_FALSE,
               0,
               (void*)0
            );

            glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rockyTextureImageID);
            
            
            // cube is 12 triangles, 2 each for 6 sides
            glDrawArrays(GL_TRIANGLES, 0, 12*3);
            
            // cubeAngle += kCubeRotSpeed;
        }
        if(1)
        { // Sphere
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(0, 1.0, -4));
            Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 0.5f));
            Model = glm::rotate(Model, (float)(PI / 2), glm::vec3(1.0, 0, 0));
            Model = glm::rotate(Model, sphereAngle, glm::vec3(0.0, 0.1, 1.0));
            
            glm::mat4 mv = View * Model;
            glUniformMatrix4fv(mvUniformLocation, 1, GL_FALSE, glm::value_ptr(mv));
            glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(Projection));
            
            
            glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBuffer);
            glVertexAttribPointer(
               0,
               3,
               GL_FLOAT,
               GL_FALSE,
               0,
               (void*)0
            );
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBuffer);
            
            
            glBindBuffer(GL_ARRAY_BUFFER, sphereUVBuffer);
            
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, marsTextureImageID);
            
            glDrawElements(GL_TRIANGLES,                    // primitive type
                           (GLsizei)sphereIndices.size(),          // # of indices
                           GL_UNSIGNED_INT,                 // data type
                           (void*)0);                       // offset to indices

            
            
            sphereAngle += kSphereRotSpeed;
        }
        if(1)
        { // Pyramid
            
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(0, -2, -4));
            // Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, (float)(PI / 8.0f), glm::vec3(1, 0, 0));
            Model = glm::rotate(Model, pyramidAngle, glm::vec3(0, 1, 0));
            
            glm::mat4 mv = View * Model;
            glUniformMatrix4fv(mvUniformLocation, 1, GL_FALSE, glm::value_ptr(mv));
            glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(Projection));
            
            
            glBindBuffer(GL_ARRAY_BUFFER, pyramidVertexBuffer);
            glVertexAttribPointer(
               0,
               3,
               GL_FLOAT,
               GL_FALSE,
               0,
               (void*)0
            );

            glBindBuffer(GL_ARRAY_BUFFER, pyramidUVBuffer);
            glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
            glEnableVertexAttribArray( 1 );
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, checkersTextureID);
            
            
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)pyramidVertexData.size());
            
            pyramidAngle += kPyramidRotSpeed;
        }
        
        glDisableVertexAttribArray(0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    std::cout  << "cleaning up" << std::endl;
    
    if(cubeTextureImage) {
        delete cubeTextureImage;
    }
    if(triTextureImage) {
        delete triTextureImage;
    }
    
    glfwTerminate();
    std::cout << "GL setup exiting cleanly\n";
    return 0;
}

