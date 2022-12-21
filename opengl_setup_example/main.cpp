//
//  main.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/12/22.
//

#include <iostream>
#include <vector>

// Define before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "model_object.h"
#include "shaders.h"
#include "textures.h"
#include "proc_textures.h"
#include "cube.h"
#include "sphere.h"
#include "pyramid.h"
#include "mathutil.h"
#include "trianglemesh.h"

const char* kVertexShaderPath = "phong_vertex_shader.glsl";
const char* kFragmentShaderPath = "phong_fragment_shader.glsl";

const RGBColor red = {255, 0, 0};
const RGBColor blue = {0, 0, 255};
const RGBColor green = {0, 255, 0};
const RGBColor white = {255, 255, 255};
const RGBColor black = { 0, 0, 0};
const RGBColor orange = {232, 99, 10};

static bool RunTests(void)
{
    bool good = true;
    good = good && TestGenerateCheckers();
    return good;
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
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // glClearColor((GLfloat)0.5f, (GLfloat)0.5f, (GLfloat)0.5f, (GLfloat)1.0f);
    glClearColor((GLfloat)0.2f, (GLfloat)0.2f, (GLfloat)0.2f, (GLfloat)1.0f);

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
    
    auto eyePosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::mat4 View = glm::lookAt(
        eyePosition, // position
        glm::vec3(0.0f, 0.0f, 0.0f), // target
        glm::vec3(0.0f, 1.0f, 0.0f) //  up vector
        );

    
    GLint mUniformLocation = glGetUniformLocation(program, "modelMatrix");
    if (mUniformLocation == -1) {
        std::cerr << "Could not bind modelMatrix  location" << std::endl;
    }
    
    GLint vUniformLocation = glGetUniformLocation(program, "viewMatrix");
    if (vUniformLocation == -1) {
        std::cerr << "Could not bind viewMatrix  location" << std::endl;
    }
    
    GLint normMatUniformLocation = glGetUniformLocation(program, "normMatrix");
    if (normMatUniformLocation == -1) {
        std::cerr << "Could not bind normMatrix location" << std::endl;
    }
    
    GLint projUniformLocation = glGetUniformLocation(program, "projMatrix");
    if (projUniformLocation == -1) {
        std::cerr << "Could not bind projUniformLocation" << std::endl;
    }
    
    GLint aPositionLocation = glGetAttribLocation(program, "a_position");
    if (aPositionLocation == -1) {
        std::cerr << "Could not bind attribute a_position" << std::endl;
    }
    
    GLint lightPositionID = glGetUniformLocation(program, "lightPosition");
    if (lightPositionID == -1) {
        std::cerr << "Could not bind lightPositionID location" << std::endl;
    }
    
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    
    //  Textures
    std::cout  << "Generating checkers" << std::endl;
    RGBImageBuffer* blueGreenCheckersImage = GenerateCheckers(1024, 32, blue, green);
    assert(blueGreenCheckersImage);
    GLuint blueGreenCheckersTextureID = CreateTextureFromImage(blueGreenCheckersImage);

    RGBImageBuffer* redBlackCheckersImage = GenerateCheckers(1024, 64, red, black);
    assert(redBlackCheckersImage);
    GLuint redBlackCheckersTextureID = CreateTextureFromImage(redBlackCheckersImage);

    
    
    
    std::cout  << "Generating marble" << std::endl;
    RGBImageBuffer* marbleImage = GenerateMarble(512, blue, white);
    assert(marbleImage);
    GLuint marbleTextureID = CreateTextureFromImage(marbleImage);
    
    std::cout << "Loading image textures" << std::endl;
    RGBImageBuffer *rockyTextureImage = LoadImageBufferFromPNG("textures/rocky.png");
    assert(rockyTextureImage);
    GLuint rockyTextureImageID = CreateTextureFromImage(rockyTextureImage);
    
    RGBImageBuffer *marsTextureImage = LoadImageBufferFromPNG("textures/mars.png");
    assert(marsTextureImage);
    GLuint marsTextureImageID = CreateTextureFromImage(marsTextureImage);

    RGBImageBuffer *fuzzyTextureImage = LoadImageBufferFromPNG("textures/fuzzy.png");
    assert(fuzzyTextureImage);
    GLuint fuzzyTextureID = CreateTextureFromImage(fuzzyTextureImage);

    
    std::cout  << "Generating fractal browning motion" << std::endl;
    RGBImageBuffer* fbmImage = GenerateFractalBrownianMotion(256 /*512*/, orange, 0.8, 1.8, 3.0, -0.5, 0.5, 64, false);
    assert(fbmImage);
    GLuint fbmTextureID = CreateTextureFromImage(fbmImage);
    
    
    // Shapes
    std::vector<ModelObject> objects;
    
    objects.push_back(ModelObject());
    ModelObject& triObj(objects.back());
    
    MakeTriangle(triObj);
    BindObjectBuffers(triObj);
    
    triObj.textureID = marbleTextureID;
    
    
    // CUBE
    ModelObject cubeObj;
    
    GenerateCube(cubeObj.vertexData, cubeObj.texCoords);
    GenerateNormals(cubeObj.vertexData, cubeObj.normals);
    BindObjectBuffers(cubeObj);
    
    cubeObj.textureID = fbmTextureID;
    
    // CUBE TWO
    ModelObject cubeTwoObj;
    GenerateCube(cubeTwoObj.vertexData, cubeTwoObj.texCoords);
    GenerateNormals(cubeTwoObj.vertexData, cubeTwoObj.normals);
    BindObjectBuffers(cubeTwoObj);
    
    cubeTwoObj.textureID = rockyTextureImageID;
        // SPHERE
    ModelObject sphereObj;
    GenerateSphere(1.5, 20, 25, sphereObj.vertexData, sphereObj.normals, sphereObj.texCoords, sphereObj.vertexIndexes);
    BindObjectBuffers(sphereObj);
    
    sphereObj.isIndexed = true;
    
    sphereObj.textureID = marsTextureImageID;
    
    // Pyramid
    ModelObject pyramidObj;
    GeneratePyramid(pyramidObj.vertexData, pyramidObj.texCoords);
    GenerateNormals(pyramidObj.vertexData, pyramidObj.normals);
    BindObjectBuffers(pyramidObj);

    pyramidObj.textureID = blueGreenCheckersTextureID;
    
    // Triangle mesh loaded from SMF file.
    TriangleMesh mesh;
    ModelObject meshObj;
    const char* meshPath = "mesh/bound-bunny_200.smf";
    if(!mesh.LoadFromSMF(meshPath)) {
        std::cerr << "Error loading mesh from " << meshPath << std::endl;
        
    } else {
        
        mesh.CalcNormals();
        
       
        MakeMeshObject(mesh, meshObj);
        
        BindObjectBuffers(meshObj);
        
        // todo: use a better texture.
        meshObj.textureID = redBlackCheckersTextureID;
    }
    
    // Second mesh
    TriangleMesh meshTwo;
    ModelObject meshTwoObj;
    const char* meshTwoPath = "mesh/teddy.smf";
    if(!meshTwo.LoadFromSMF(meshTwoPath)) {
        std::cerr << "Error loading mesh from " << meshPath << std::endl;
        
    } else {
        
        meshTwo.CalcNormals();
        
        MakeMeshObject(meshTwo, meshTwoObj);
        
        BindObjectBuffers(meshTwoObj);
        
        // todo: use a better texture.
        meshTwoObj.textureID = fuzzyTextureID;
    }
    
    
    auto lightPosition = glm::vec3 {-5, 5, 0};

    // Rotation angles and speeds
    float triAngle = 0.0f;
    float cubeAngle = 0.0f;
    float sphereAngle = 0.0f;
    float pyramidAngle = 0.0f;
    float meshAngle = 0.0f;
    
    const float kTriRotSpeed = 0.02;
    const float kCubeRotSpeed = 0.03;
    const float kSphereRotSpeed = 0.01;
    const float kPyramidRotSpeed = 0.025;
    const float kMeshRotSpeed = 0.025;
    
    std::cout  << "starting main loop" << std::endl;
    
    bool rotating = true;
    
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 ) {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(program);
        
        glEnableVertexAttribArray(aPositionLocation);
        
        
        // NOTE: These don't actually change each time through loop currently.
        glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniformMatrix4fv(vUniformLocation, 1, GL_FALSE, glm::value_ptr(View));
        
        
        glUniform3f(lightPositionID, lightPosition.x, lightPosition.y, lightPosition.z);
        
        
        if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS) {
            rotating = false;
        } else {
            rotating = true;
        }
        
        
        if (1) { // Triangle
            glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, triAngle, glm::vec3(0, 0.2, 1));
            triObj.mMat = Model;
            DrawObject(triObj, View, mUniformLocation, normMatUniformLocation);
            
            if(rotating)
                triAngle += kTriRotSpeed;
        }
        if(1){ // Cube one
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5, 0, -4));
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            cubeObj.mMat = Model;
            DrawObject(cubeObj, View, mUniformLocation, normMatUniformLocation);
            
            if(rotating)
                cubeAngle += kCubeRotSpeed;
        }
        
        if(1)
        { // Cube two
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(2.5, 0, -4));
            Model = glm::scale(Model, glm::vec3(0.6f, 0.5f, 0.5f));
            Model = glm::rotate(Model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            cubeTwoObj.mMat = Model;
            DrawObject(cubeTwoObj, View, mUniformLocation, normMatUniformLocation);
            
            // cubeAngle += kCubeRotSpeed;
        }
        if(1)
        { // Sphere
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.0, -4));
            Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 0.5f));
            Model = glm::rotate(Model, (float)(PI / 2), glm::vec3(1.0, 0, 0));
            Model = glm::rotate(Model, sphereAngle, glm::vec3(0.0, 0.1, 1.0));
            
            sphereObj.mMat = Model;
            DrawObject(sphereObj, View, mUniformLocation, normMatUniformLocation);
            
            
            if(rotating)
                sphereAngle += kSphereRotSpeed;
        }
        if(1)
        { // Pyramid
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -2, -4));
            Model = glm::rotate(Model, (float)(PI / 8.0f), glm::vec3(1, 0, 0));
            Model = glm::rotate(Model, pyramidAngle, glm::vec3(0, 1, 0));
            
            pyramidObj.mMat = Model;
            DrawObject(pyramidObj, View, mUniformLocation, normMatUniformLocation);

            if(rotating)
                pyramidAngle += kPyramidRotSpeed;
        }
        
        if(1) {
            // mesh 1
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, -2, -5));
            
            Model  = glm::scale(Model, glm::vec3(1.5, 1.5, 1.8));
            Model = glm::rotate(Model, meshAngle, glm::vec3(0, 1, 0));
            
            meshObj.mMat = Model;
            DrawObject(meshObj, View, mUniformLocation, normMatUniformLocation);

            if(rotating)
                meshAngle += kMeshRotSpeed;
        }
        
        if(1) {
            // mesh 2
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(2, -2, -5));
            
            Model  = glm::scale(Model, glm::vec3(0.05, 0.05, 0.05));
            Model = glm::rotate(Model, meshAngle, glm::vec3(1, 0, 0));
            
            meshTwoObj.mMat = Model;
            DrawObject(meshTwoObj, View, mUniformLocation, normMatUniformLocation);

//            if(rotating)
//                meshAngle += kMeshRotSpeed;
        }
        
        glDisableVertexAttribArray(0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    std::cout  << "cleaning up" << std::endl;
    
    if(fbmImage) {
        delete fbmImage;
    }
    if(marbleImage) {
        delete marbleImage;
    }
    if(marsTextureImage) {
        delete marsTextureImage;
    }
    if(fuzzyTextureImage) {
        delete fuzzyTextureImage;
    }
    if(blueGreenCheckersImage) {
        delete blueGreenCheckersImage;
    }
    if(redBlackCheckersImage) {
        delete redBlackCheckersImage;
    }
    
    // TODO: actually cleanup gl stuff properly
    
    
    // TODO: for every object.
    //    glDeleteBuffers(1, &vertexbuffer);
    //    glDeleteBuffers(1, &uvbuffer);
        
    glDeleteProgram(program);
    // TODO: for  every texture
    // glDeleteTextures(1, &TextureID);
    // TODO: for every vertex array
    // glDeleteVertexArrays(1, &VertexArrayID);
    
    
    glfwTerminate();
    std::cout << "GL setup exiting cleanly\n";
    return 0;
}

