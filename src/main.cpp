// Assignment 03: Raytracing

#include "imgui_setup.h"
#include "camera.h"
#include "renderengine.h"
#include "world.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "quad.h"
#include "lightsource.h"
#include "pointlightsource.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../depends/stb/stb_image.h"
#include "../depends/stb/stb_image_write.h"

#define RENDER_BATCH_COLUMNS 20 // Number of columns to render in a single go. Increase to gain some display/render speed!

Camera *camera;
RenderEngine *engine;

int screen_width = 800, screen_height = 600; // This is window size, used to display scaled raytraced image.
int image_width = 1920, image_height = 1080; // This is raytraced image size. Change it if needed.
GLuint texImage;

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    // Setup raytracer camera. This is used to spawn rays.
    Vector3D camera_position(0, 0, 10);
    Vector3D camera_target(0, 0, 0); //Looking down -Z axis
    Vector3D camera_up(0, 1, 0);
    float camera_fovy =  45;
    camera = new Camera(camera_position, camera_target, camera_up, camera_fovy, image_width, image_height);

    //Create a world
    World *world = new World;
    world->setAmbient(Color(1));
    world->setBackground(Color(1, 1, 1));
    
    Vector3D dontmove(0,0,0);
    Vector3D stepx(0.5,0,0);
    Vector3D stepy(0,-0.5,0);
    Vector3D stepz(0,0,0.5);
    
    

    //triangle1 material
    Material *m1 = new Material(world);
    m1->color = Color(0.1, 0.7, 0.0);
    m1->ka = 0.1;
    m1->kd = 0.1;
    m1->ks = 1;
    m1->n = 1;
    m1->kr = 0.8;
    m1->kt = 0.4;
    
    //lower triangle
    Object *triangle1 = new Triangle(Vector3D(-10, 1, -12), Vector3D(1, 5, -24), Vector3D(-8, 5, -21), m1, stepx);
    world->addObject(triangle1);

    //sphere material
    Material *msp = new Material(world);
    msp->color = Color(0.3, 0.2, 0.5);
    msp->ka = 0.1;
    msp->kd = 0.1;
    msp->ks = 1;
    msp->n = 1;
    msp->kr = 0.9;
    msp->kt = 0.9;

    //sphere
    //Object *sphere = new Sphere(Vector3D(-10, 0, -10), 4, msp, stepx);
    //world->addObject(sphere);

    // Material *msp2 = new Material(world);
    // msp2->color = Color(0.9, 0, 0);

    // Object *sphere2 = new Sphere(Vector3D(-6, 0, -20), 4, msp2);
    // world->addObject(sphere2);

    // Object *sphere5 = new Sphere(Vector3D(5, 0, 0), 3, msp2);
    // world->addObject(sphere5);

    // Material *msp3 = new Material(world);
    // msp3->color = Color(0, 0.9, 0);

    // Object *sphere3 = new Sphere(Vector3D(-14, 0, -30), 4, msp3);
    // world->addObject(sphere3);

    // Material *msp4 = new Material(world);
    // msp4->color = Color(0, 0, 0.9);

    // Object *sphere4 = new Sphere(Vector3D(-22, 0, -40), 4, msp4);
    // world->addObject(sphere4);

    //upper triangle material
    Material *m2 = new Material(world);
    m2->color = Color(0.7, 0.4, 0.9);
    m2->ka = 0.4;
    m2->kd = 0.5;
    m2->ks = 0.1;
    m2->n = 32;
    msp->kr = 0.5;
    msp->kt = 0.9;

    
    //upper triangle
    Object *triangle2 = new Triangle(Vector3D(1, 4, -5), Vector3D(1, 7, -24), Vector3D(-8, 8, -21), m2, dontmove);
    world->addObject(triangle2);

    //quardilateral material
    Material *mq = new Material(world);
    mq->color = Color(0.9, 0.0, 0.0);
    mq->ka = 0.1;
    mq->kd = 0.1;
    mq->ks = 1;
    mq->n = 1;
    mq->kr = 0.9;
    mq->kt = 0.9;


    //quardilateral object
    //Object *quardilateral = new Quad(Vector3D(1, -5, -5), Vector3D(2, -8, -10), Vector3D(3, -3, -10), Vector3D(6, -7, -8), mq);
    //world->addObject(quardilateral);

    LightSource *light = new PointLightSource(world, Vector3D(0, 10, 0), Color(1, 1, 1));
    world->addLight(light);

    // for(int i=0;i<5;i++){
    //     for(int j=0;j<5;j++){
    //         for(int k=0;k<5;k++){
    //             world->addLight(new PointLightSource(world,Vector3D(0+0.1*i,10+0.1*j,0+0.1*k),Color(1,1,1)));
    //         }
    //     }
    // }
    
    engine = new RenderEngine(world, camera);

    //Initialise texture
    glGenTextures(1, &texImage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool render_status;
        for(int i=0; i<RENDER_BATCH_COLUMNS; i++) 
            render_status = engine->renderLoop(); // RenderLoop() raytraces 1 column of pixels at a time.
        if(!render_status)
        {
            // Update texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texImage);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
        } 

        ImGui::Begin("Lumina", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Size: %d x %d", image_width, image_height);
        if(ImGui::Button("Save")){
          char filename[] = "motionblurz.png";
          stbi_write_png(filename, image_width, image_height, 3, camera->getBitmap(),0);        
        }
        //Display render view - fit to width
        int win_w, win_h;
        glfwGetWindowSize(window, &win_w, &win_h);
        float image_aspect = (float)image_width/(float)image_height;
        float frac = 0.95; // ensure no horizontal scrolling
        ImGui::Image((void*)(intptr_t)texImage, ImVec2(frac*win_w, frac*win_w/image_aspect), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteTextures(1, &texImage);

    cleanup(window);

    return 0;
}
