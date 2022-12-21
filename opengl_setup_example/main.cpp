//
//  main.cpp
//  opengl_setup_example
//
//  Created by Richard Anton on 12/12/22.
//

#include <iostream>
#include <vector>
#include <list>
#include <memory>

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
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    
    //  Textures
    std::list<std::unique_ptr<RGBImageBuffer>> imageBuffers;
    std::vector<GLuint> textureIDs;
    
    std::cout  << "Generating checkers" << std::endl;
    RGBImageBuffer* blueGreenCheckersImage = GenerateCheckers(1024, 32, blue, green);
    assert(blueGreenCheckersImage);
    GLuint blueGreenCheckersTextureID = CreateTextureFromImage(blueGreenCheckersImage);
    textureIDs.push_back(blueGreenCheckersTextureID);
    
    RGBImageBuffer* redBlackCheckersImage = GenerateCheckers(1024, 64, red, black);
    assert(redBlackCheckersImage);
    GLuint redBlackCheckersTextureID = CreateTextureFromImage(redBlackCheckersImage);
    textureIDs.push_back(redBlackCheckersTextureID);
    
    std::cout  << "Generating marble" << std::endl;
    RGBImageBuffer* marbleImage = GenerateMarble(512, blue, white);
    assert(marbleImage);
    GLuint marbleTextureID = CreateTextureFromImage(marbleImage);
    textureIDs.push_back(marbleTextureID);
    
    std::cout << "Loading image textures" << std::endl;
    RGBImageBuffer *rockyTextureImage = LoadImageBufferFromPNG("textures/rocky.png");
    assert(rockyTextureImage);
    GLuint rockyTextureImageID = CreateTextureFromImage(rockyTextureImage);
    textureIDs.push_back(rockyTextureImageID);
    
    RGBImageBuffer *marsTextureImage = LoadImageBufferFromPNG("textures/mars.png");
    assert(marsTextureImage);
    GLuint marsTextureImageID = CreateTextureFromImage(marsTextureImage);
    textureIDs.push_back(marsTextureImageID);
    
    RGBImageBuffer *fuzzyTextureImage = LoadImageBufferFromPNG("textures/fuzzy.png");
    assert(fuzzyTextureImage);
    GLuint fuzzyTextureID = CreateTextureFromImage(fuzzyTextureImage);
    textureIDs.push_back(fuzzyTextureID);
    
    std::cout  << "Generating fractal browning motion" << std::endl;
    RGBImageBuffer* fbmImage = GenerateFractalBrownianMotion(256 /*512*/, orange, 0.8, 1.8, 3.0, -0.5, 0.5, 64, false);
    assert(fbmImage);
    GLuint fbmTextureID = CreateTextureFromImage(fbmImage);
    textureIDs.push_back(fbmTextureID);
    
    // Add these all to list of unique ptrs for automatic cleanup.
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(blueGreenCheckersImage));
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(redBlackCheckersImage));
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(marbleImage));
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(rockyTextureImage));
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(marsTextureImage));
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(fuzzyTextureImage));
    imageBuffers.push_back(std::unique_ptr<RGBImageBuffer>(fbmImage));
    
    
    
    // Shapes
    std::list<std::unique_ptr<ModelObject>> objects;
    
    // Triangle
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* triObjPtr = objects.back().get();
    {
        MakeTriangle(*triObjPtr);
        BindObjectBuffers(*triObjPtr);
        
        triObjPtr->textureID = marbleTextureID;
    }
    
    // CUBE
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* cubeObjPtr = objects.back().get();
    {
        
        GenerateCube(cubeObjPtr->vertexData, cubeObjPtr->texCoords);
        GenerateNormals(cubeObjPtr->vertexData, cubeObjPtr->normals);
        BindObjectBuffers(*cubeObjPtr);
        
        cubeObjPtr->textureID = fbmTextureID;
    }
    
    // CUBE TWO
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* cubeTwoObjPtr = objects.back().get();
    {
        GenerateCube(cubeTwoObjPtr->vertexData, cubeTwoObjPtr->texCoords);
        GenerateNormals(cubeTwoObjPtr->vertexData, cubeTwoObjPtr->normals);
        BindObjectBuffers(*cubeTwoObjPtr);
        
        cubeTwoObjPtr->textureID = rockyTextureImageID;
    }
        
    // SPHERE
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* sphereObjPtr = objects.back().get();
    {
        GenerateSphere(1.5, 20, 25, sphereObjPtr->vertexData, sphereObjPtr->normals, sphereObjPtr->texCoords, sphereObjPtr->vertexIndexes);
        BindObjectBuffers(*sphereObjPtr);
        
        sphereObjPtr->isIndexed = true;
        
        sphereObjPtr->textureID = marsTextureImageID;
    }
    
    // Pyramid
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* pyramidObjPtr = objects.back().get();
    {
        GeneratePyramid(pyramidObjPtr->vertexData, pyramidObjPtr->texCoords);
        GenerateNormals(pyramidObjPtr->vertexData, pyramidObjPtr->normals);
        BindObjectBuffers(*pyramidObjPtr);
        
        pyramidObjPtr->textureID = blueGreenCheckersTextureID;
    }
    
    // Triangle mesh loaded from SMF file.
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* meshObjPtr = objects.back().get();
    {
        // TODO: refactor mesh loading into a function.
        TriangleMesh mesh;
        const char* meshPath = "mesh/bound-bunny_200.smf";
        if(!mesh.LoadFromSMF(meshPath)) {
            std::cerr << "Error loading mesh from " << meshPath << std::endl;
            
        } else {
            
            mesh.CalcNormals();
            MakeMeshObject(mesh, *meshObjPtr);
            BindObjectBuffers(*meshObjPtr);
            
            // todo: use a better texture.
            meshObjPtr->textureID = redBlackCheckersTextureID;
        }
    }
    
    // Second mesh
    objects.push_back(std::make_unique<ModelObject>());
    ModelObject* meshTwoObjPtr = objects.back().get();
    {
        TriangleMesh meshTwo;
        
        const char* meshTwoPath = "mesh/teddy.smf";
        if(!meshTwo.LoadFromSMF(meshTwoPath)) {
            std::cerr << "Error loading mesh from " << meshTwoPath << std::endl;
            
        } else {
            
            meshTwo.CalcNormals();
            
            MakeMeshObject(meshTwo, *meshTwoObjPtr);
            
            BindObjectBuffers(*meshTwoObjPtr);
            
            // todo: use a better texture.
            meshTwoObjPtr->textureID = fuzzyTextureID;
        }
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
            glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.5f, 0.5f));
            model = glm::rotate(model, triAngle, glm::vec3(0, 0.2, 1));
            triObjPtr->mMat = model;
            
            if(rotating)
                triAngle += kTriRotSpeed;
        }
        
        for(auto& modelObj : objects) {
            DrawObject(*modelObj, View, mUniformLocation, normMatUniformLocation);
        }

        if(1){ // Cube one
            
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5, 0, -4));
            model = glm::scale(model, glm::vec3(0.6f, 0.5f, 0.5f));
            model = glm::rotate(model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            cubeObjPtr->mMat = model;
            if(rotating)
                cubeAngle += kCubeRotSpeed;
        }
        
        if(1)
        { // Cube two
            
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.5, 0, -4));
            model = glm::scale(model, glm::vec3(0.6f, 0.5f, 0.5f));
            model = glm::rotate(model, cubeAngle, glm::vec3(0.5, 0.0, 0.5));
            
            cubeTwoObjPtr->mMat = model;
        }
        
        if(1)
        { // Sphere
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.0, -4));
            Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 0.5f));
            Model = glm::rotate(Model, (float)(PI / 2), glm::vec3(1.0, 0, 0));
            Model = glm::rotate(Model, sphereAngle, glm::vec3(0.0, 0.1, 1.0));
            
            sphereObjPtr->mMat = Model;
            
            if(rotating)
                sphereAngle += kSphereRotSpeed;
        }
        if(1)
        { // Pyramid
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -2, -4));
            Model = glm::rotate(Model, (float)(PI / 8.0f), glm::vec3(1, 0, 0));
            Model = glm::rotate(Model, pyramidAngle, glm::vec3(0, 1, 0));
            
            pyramidObjPtr->mMat = Model;
            
            if(rotating)
                pyramidAngle += kPyramidRotSpeed;
        }
        
        if(1) {
            // mesh 1
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, -2, -5));
            
            Model  = glm::scale(Model, glm::vec3(1.5, 1.5, 1.8));
            Model = glm::rotate(Model, meshAngle, glm::vec3(0, 1, 0));
            
            meshObjPtr->mMat = Model;
            
            if(rotating)
                meshAngle += kMeshRotSpeed;
        }
        
        if(1) {
            // mesh 2
            
            glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(2, -2, -5));
            
            Model  = glm::scale(Model, glm::vec3(0.05, 0.05, 0.05));
            Model = glm::rotate(Model, meshAngle, glm::vec3(1, 0, 0));
            
            meshTwoObjPtr->mMat = Model;
        }
        
        glDisableVertexAttribArray(0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    std::cout  << "cleaning up" << std::endl;
    

    //  Cleanup GL stuff
    
    glDeleteProgram(program);
    
    for(auto textureID: textureIDs) {
        glDeleteTextures(1, &textureID);
    }
        
    glDeleteVertexArrays(1, &VertexArrayID);
    
    for(const auto& modelObj : objects) {
        glDeleteBuffers(1, &modelObj->vertexBuffer);
        glDeleteBuffers(1, &modelObj->uvBuffer);
        glDeleteBuffers(1, &modelObj->normalBuffer);
        if(modelObj->isIndexed) {
            glDeleteBuffers(1, &modelObj->indexBuffer);
        }
    }
    
    glfwTerminate();
    std::cout << "GL setup exiting cleanly\n";
    return 0;
}

