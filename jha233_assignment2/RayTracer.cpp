/*========================================================================
* COSC 363  Computer Graphics (2018)
* Jack Hay Assignment 2
* jha233
* 82432942
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/glut.h>
#include "Plane.h"
#include "Cylinder.h"
#include <glm/gtc/matrix_transform.hpp>
#include "TextureBMP.h"
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
const float SHEAR = 6.0;

const float THETA = 60 * (3.14159265/180);


TextureBMP texture;

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0);
    glm::vec3 light(10, 40, -3);
    glm::vec3 ambientCol(0.2);   //Ambient color of light

    glm::vec3 light1(-100, 40, -3);


    ray.closestPt(sceneObjects);        //Compute the closest point of intersetion of objects with the ray
    
    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour

    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor(); //else return object's colour
    

    //my code
    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
    glm::vec3 lightVector = light - ray.xpt;
    lightVector = glm::normalize(lightVector);
    
    float lDotn = glm::dot(lightVector, normalVector);
    
    glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
    
    float rDotV = glm::dot(reflVector, -ray.dir);

    rDotV = max(rDotV, 0.0f);
    
    glm::vec3 specVector = pow(rDotV, 7) * glm::vec3(1);
    
    Ray shadow(ray.xpt, lightVector);
    shadow.closestPt(sceneObjects);
    
    glm::vec3 colorSum(0);
    
    //light source 2
    glm::vec3 normalVector1 = sceneObjects[ray.xindex]->normal(ray.xpt);
    glm::vec3 lightVector1 = light1 - ray.xpt;
    lightVector1 = glm::normalize(lightVector1);
    
    float lDotn1 = glm::dot(lightVector1, normalVector1);
    
    glm::vec3 reflVector1 = glm::reflect(-lightVector1, normalVector1);
    
    float rDotV1 = glm::dot(reflVector1, -ray.dir);

    rDotV1 = max(rDotV1, 0.0f);
    
    glm::vec3 specVector1 = pow(rDotV1, 7) * glm::vec3(1);
    
    Ray shadow1(ray.xpt, lightVector1);
    shadow1.closestPt(sceneObjects);
    
    // -- Floor texture
    if (ray.xindex == 0) {
        if ((int(ray.xpt.x)) % 5 == 1 || int(ray.xpt.x) % 5 == -1) {
            colorSum = glm::vec3(1, 1, 1);
        } else { 
            colorSum = glm::vec3(0, 0, 0);
        }
    }
    // -- Cylinder Texture
    if (ray.xindex == 1) {
        if ((int(ray.xpt.y+ray.xpt.z-4)) % 2 == 1 || int(ray.xpt.y+ray.xpt.z-4) % 2 == -1) {
            colorSum = glm::vec3(1, 1, 1);
        } else { 
            colorSum = glm::vec3(0.5, 0.5, 0);
        }
    }
    
    // -- image texture
    if (ray.xindex == 2) {
        float s = (ray.xpt.x+50)/100;
        float t = (ray.xpt.y+20)/70;
        colorSum = texture.getColorAt(s,t);
    }
    
    // -- Shadow to floor
    if(lDotn <= 0 || (shadow.xindex > -1 && (shadow.xdist < glm::length(light - ray.xpt))) || 
    lDotn1 <= 0 || (shadow1.xindex > -1 && (shadow1.xdist < glm::length(light1 - ray.xpt)))) {
        colorSum += ambientCol*materialCol;
    } else {
        colorSum += (ambientCol*materialCol) + (lDotn*materialCol) + specVector + specVector1;
    }
    
    if (ray.xindex > 2 && step < MAX_STEPS && ray.xindex <= 5)
    {
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
        Ray reflectedRay(ray.xpt, reflectedDir);
        glm::vec3 reflectedCol = trace(reflectedRay, step+1);  //Recursion!
        colorSum = colorSum + (0.8f*reflectedCol);  
    }
    
    return colorSum;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
    float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

    glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a quad.

    for(int i = 0; i < NUMDIV; i++)     //For each grid point xp, yp
    {
        xp = XMIN + i*cellX;
        for(int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j*cellY;

            glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);  //direction of the primary ray

            Ray ray = Ray(eye, dir);        //Create a ray originating from the camera in the direction 'dir'
            ray.normalize();                //Normalize the direction of the ray to a unit vector
            glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value

            glColor3f(col.r, col.g, col.b);
            glVertex2f(xp, yp);             //Draw each cell with its color value
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);
    
    texture = TextureBMP((char*)"wall.bmp");


    glm::vec3 cubeA = glm::vec3(68, -80, -50);
    glm::vec3 cubeB = glm::vec3(73, -80, -50);
    glm::vec3 cubeC = glm::vec3(73, -80, -45);
    glm::vec3 cubeD = glm::vec3(68, -80, -45);
    
    glm::vec3 cubeE = glm::vec3(68, -85, -50);
    glm::vec3 cubeF = glm::vec3(73, -85, -50);
    glm::vec3 cubeG = glm::vec3(73, -85, -45);
    glm::vec3 cubeH = glm::vec3(68, -85, -45);
    
    
    glm::mat4 im = glm::mat4(1.0f);
    glm::mat4 rotation = glm::rotate(im, THETA, glm::vec3(1, 1, 0));

    
    // -- Sphere Objects
    Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -5.0, -150.0), 15.0, glm::vec3(0, 0, 1));
    Sphere *sphere2 = new Sphere(glm::vec3(9.0, -7.0, -75.0), 3.0, glm::vec3(0, 1, 0));
    Sphere *sphere3 = new Sphere(glm::vec3(6.0, 4.0, -75.0), 4, glm::vec3(0, 0, 0));
    
    // -- Cylinder Object
    Cylinder *cylinder = new Cylinder(glm::vec3(-15, -15, -90), 4, 10, glm::vec3(1, 0, 0));

    // -- Floor
    Plane *plane = new Plane (glm::vec3(-20., -20, -40), glm::vec3(20., -20, -40), 
        glm::vec3(20., -20, -200), glm::vec3(-20., -20, -200), glm::vec3(1, 0.5, 0.3));  
        
    // -- Cube
    Plane *cubeTop = new Plane (glm::vec3(rotation * glm::vec4(cubeA, 1)), glm::vec3(rotation * glm::vec4(cubeB, 1)),
    glm::vec3(rotation * glm::vec4(cubeC, 1)), glm::vec3(rotation * glm::vec4(cubeD, 1)), glm::vec3(128,0,0));  
    
    Plane *cubeBottom = new Plane (glm::vec3(rotation * glm::vec4(cubeE, 1)), glm::vec3(rotation * glm::vec4(cubeF, 1)),
    glm::vec3(rotation * glm::vec4(cubeG, 1)), glm::vec3(rotation * glm::vec4(cubeH, 1)), glm::vec3(128,0,0));  
    
    Plane *cubeLeft = new Plane (glm::vec3(rotation * glm::vec4(cubeA, 1)), glm::vec3(rotation * glm::vec4(cubeD, 1)),
    glm::vec3(rotation * glm::vec4(cubeH, 1)), glm::vec3(rotation * glm::vec4(cubeE, 1)), glm::vec3(0,0,128));  
    
    Plane *cubeRight = new Plane (glm::vec3(rotation * glm::vec4(cubeB, 1)), glm::vec3(rotation * glm::vec4(cubeC, 1)), 
    glm::vec3(rotation * glm::vec4(cubeG, 1)), glm::vec3(rotation * glm::vec4(cubeF, 1)), glm::vec3(128,0,128));
    
    Plane *cubeFront = new Plane (glm::vec3(rotation * glm::vec4(cubeD, 1)), glm::vec3(rotation * glm::vec4(cubeC, 1)), 
    glm::vec3(rotation * glm::vec4(cubeG, 1)), glm::vec3(rotation * glm::vec4(cubeH, 1)), glm::vec3(128,0,128));
    
    Plane *cubeBack = new Plane (glm::vec3(rotation * glm::vec4(cubeA, 1)), glm::vec3(rotation * glm::vec4(cubeB, 1)), 
    glm::vec3(rotation * glm::vec4(cubeF, 1)), glm::vec3(rotation * glm::vec4(cubeE, 1)), glm::vec3(128,0,128));


    // - list of scene objects
    sceneObjects.push_back(plane);
    sceneObjects.push_back(cylinder);
    
    sceneObjects.push_back(sphere3);

    sceneObjects.push_back(sphere1);
    sceneObjects.push_back(sphere2); 
    
    sceneObjects.push_back(cubeTop);
    sceneObjects.push_back(cubeBottom);
    sceneObjects.push_back(cubeLeft);
    sceneObjects.push_back(cubeRight);
    sceneObjects.push_back(cubeFront);
    sceneObjects.push_back(cubeBack);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
