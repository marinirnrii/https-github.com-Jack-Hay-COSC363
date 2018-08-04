//  ========================================================================
//  COSC363 Assignment 1: 
//  Jack Hay
//  82432942
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <GL/glut.h>
#include "loadTGA.h"
#include "loadBMP.h"
#include <math.h>
using namespace std;

GLuint txId[13];   
float angle=0, look_x, look_z=-1., eye_x=0, eye_y=1, eye_z=50;
float savedAngle, eye_x2, eye_y2=1, eye_z2; 
float theta = 20;

float cTimer = 7;
int angle2 = 0;
int direction = -1;

const int N = 38;
float candleAngle = 10*M_PI/180;

float vx[N] = {0, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
              3, 3, 3, 3, 3, 3, 3, 3, 0.2, 0.2, 1.1, 2, 2.4, 2, 1.8, 1.3, 0.6, 0.1};
float vy[N] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
               19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
float vz[N] = {0};

int isSecondCam = 0;
#define PARTICLE_MOVEMENT 3500

typedef struct                      
{
    float x;
    float y;
    float z;
    float xdir;
    float ydir;
    float zdir;
}Particle; 


Particle particle[1];

void update(){   
    for (int i = 0;i < 100; i++)      
    {
        if (particle[i].y > 0.7)
        {
            particle[i].x=0.0;
            particle[i].y=0.1;
            particle[i].z=0.0;
        } else {
            glTranslatef(particle[i].x,particle[i].y,particle[i].z);
            gluSphere(gluNewQuadric(),.015,3.9,3.9);
            glTranslatef(-particle[i].x,-particle[i].y,-particle[i].z);
            particle[i].x += particle[i].xdir/3000;
            particle[i].y += particle[i].ydir/PARTICLE_MOVEMENT;
            particle[i].z += particle[i].zdir/PARTICLE_MOVEMENT;
        }
    }
}

//--------------------------------------------------------------------------------
void loadTexture()              
{
    glGenTextures(12, txId);  

    glBindTexture(GL_TEXTURE_2D, txId[0]); 
    loadTGA("textures/WoodenWalls.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 

    glBindTexture(GL_TEXTURE_2D, txId[1]);  
    loadTGA("textures/cow.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    
    
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    
    
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("skybox/left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("skybox/front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("skybox/right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

    // *** back***
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("skybox/back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("skybox/up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("skybox/down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
        glBindTexture(GL_TEXTURE_2D, txId[8]);  //Use this texture
    loadTGA("textures/Grass.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    
            glBindTexture(GL_TEXTURE_2D, txId[9]);  //Use this texture
    loadTGA("textures/Roof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    
                glBindTexture(GL_TEXTURE_2D, txId[10]);  //Use this texture
    loadTGA("textures/outsideRoof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 

                glBindTexture(GL_TEXTURE_2D, txId[11]);  //Use this texture
    loadTGA("textures/creeper.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 

                glBindTexture(GL_TEXTURE_2D, txId[12]);  //Use this texture
    loadBMP("textures/cFace.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT && isSecondCam == 0) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT && isSecondCam == 0) angle += 0.1;
    else if(key == GLUT_KEY_DOWN && isSecondCam == 0)
    {  //Move backward
        eye_x -= 0.4*sin(angle);
        eye_z += 0.4*cos(angle);
    }
    else if(key == GLUT_KEY_UP && isSecondCam == 0)
    { //Move forward
        eye_x += 0.4*sin(angle);
        eye_z -= 0.4*cos(angle);
    } else if(key == GLUT_KEY_F1){
        if(isSecondCam == 0){
        
        eye_x2 = eye_x;
        eye_z2 = eye_z;
        savedAngle = angle;
        isSecondCam = 1;
        angle = 279.5;
        eye_y = 1;
        eye_z = -3;
        
    } else {
        eye_x = eye_x2;
        eye_z = eye_z2;
        angle = savedAngle;
        isSecondCam = 0;
    }
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

//--------------------------------------------------------------------------------

void initialise()
{ 
    loadTexture();
    float white[4]   = {1.0, 1.0, 1.0, 1.0};
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glEnable(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0., 1., 1., 1.);    //Background colour 
    
    glMatrixMode (GL_PROJECTION); 
    glLoadIdentity();
       
}

void roof(){
        glBindTexture(GL_TEXTURE_2D, txId[9]);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 16.0);
        glVertex3f(-15, 8, -15);
         
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-15, 8, 15);
         
        glTexCoord2f(16.0, 0.0);
        glVertex3f(15, 8, 15);
         
        glTexCoord2f(16.0, 16.0);
        glVertex3f(15, 8, -15);

    glEnd();
    
    
}

void outsideRoof() {
    glBindTexture(GL_TEXTURE_2D, txId[10]);

    glBegin(GL_QUADS);
        
            glTexCoord2f(0.0, 4.0);
    glVertex3f(15, 8, -15);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0, 12, -15);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(0, 12, 15);
    
    glTexCoord2f(12.0, 4.0);
    glVertex3f(15, 8, 15);
    
    glEnd();
    
    
    glBindTexture(GL_TEXTURE_2D, txId[10]);

    glBegin(GL_QUADS);
        
            glTexCoord2f(0.0, 4.0);
    glVertex3f(-15, 8, -15);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0, 12, -15);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(0, 12, 15);
    
    glTexCoord2f(12.0, 4.0);
    glVertex3f(-15, 8, 15);
    
    glEnd();
    
    
    
    
        glBindTexture(GL_TEXTURE_2D, txId[10]);

    glBegin(GL_QUADS);
        
            glTexCoord2f(0.0, 2.0);
    glVertex3f(-15, 8, 15);
    
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0, 12, 15);
    glTexCoord2f(12.0, 0.0);
    glVertex3f(0, 12, 15);
    
    glTexCoord2f(12.0, 2.0);
    glVertex3f(15, 8, 15);
    
    glEnd();
    
    
            glBindTexture(GL_TEXTURE_2D, txId[10]);

    glBegin(GL_QUADS);
        
            glTexCoord2f(0.0, 2.0);
    glVertex3f(-15, 8, -15);
    
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0, 12, -15);
    glTexCoord2f(12.0, 0.0);
    glVertex3f(0, 12, -15);
    
    glTexCoord2f(12.0, 2.0);
    glVertex3f(15, 8, -15);
    
    glEnd();
    
    
}
    

void skybox(){
  glEnable(GL_TEXTURE_2D);
  
  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, txId[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  
  glVertex3f(-1000,  -500, 1000);
  
  glTexCoord2f(1.0, 0.0);
  
  glVertex3f(-1000, -500, -1000);
  
  glTexCoord2f(1.0, 1.0);
  
  glVertex3f(-1000, 500., -1000);
  
  glTexCoord2f(0.0, 1.0);
  
  glVertex3f(-1000, 500, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, txId[3]);
  glBegin(GL_QUADS);
  
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000,  -500, -1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, -500, -1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 500, -1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000,  500, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, txId[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(1000,  -500, -1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, -500, 1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 500,  1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(1000,  500,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, txId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f( 1000, -500, 1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-1000, -500,  1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-1000, 500,  1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f( 1000, 500, 1000);
  glEnd();
  
  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, txId[6]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000, 500, -1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, 500,  -1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, 500,  1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000, 500, 1000);
  glEnd();
  
  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, txId[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1000, -500, 1000);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1000, -500,  1000);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1000, -500, -1000);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1000, -500, -1000);
  glEnd();
  
}

//--------------------------------------------------------------------------------
void walls()
{
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    
    glBegin(GL_QUADS);
    
    ////////////////////// LEFT WALL ///////////////////////

    glTexCoord2f(0.0, 4.0);
    glVertex3f(-15, 8, -15);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-15, -1, -15);
    
    glTexCoord2f(12.0, 0.0);
    glVertex3f(-15, -1, 15);
    
    glTexCoord2f(12.0, 4.0);
    glVertex3f(-15, 8, 15);

    ////////////////////// BACK WALL ///////////////////////

    glTexCoord2f(0.0, 4.0);
    glVertex3f(-15, 8, -15);
    
    glTexCoord2f(0.0, 0.0);        
    glVertex3f(-15, -1, -15);
    
    glTexCoord2f(12.0, 0.0);
    glVertex3f(15, -1, -15);
    
    glTexCoord2f(12.0, 4.0);
    glVertex3f(15, 8, -15);

    ////////////////////// FRONT WALL LEFT ///////////////////////

    glTexCoord2f(0.0, 4.0);
    glVertex3f(-15, 8, 15);
     
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-15, -1, 15);
     
    glTexCoord2f(12.0, 0.0);
    glVertex3f(-2, -1, 15);
     
    glTexCoord2f(12.0, 4.0);
    glVertex3f(-2, 8, 15);
    
    ////////////////////// FRONT WALL RIGHT ///////////////////////

    glTexCoord2f(0.0, 4.0);
    glVertex3f(2, 8, 15);
     
    glTexCoord2f(0.0, 0.0);
    glVertex3f(2, -1, 15);
     
    glTexCoord2f(12.0, 0.0);
    glVertex3f(15, -1, 15);
     
    glTexCoord2f(12.0, 4.0);
    glVertex3f(15, 8, 15);
    
        ////////////////////// FRONT WALL ABOVE DOOR ///////////////////////

    glTexCoord2f(0.0, 1.5);
    glVertex3f(2, 8, 15);
     
    glTexCoord2f(0.0, 0.0);
    glVertex3f(2, 5, 15);
     
    glTexCoord2f(1.5, 0.0);
    glVertex3f(-2, 5, 15);
     
    glTexCoord2f(1.5, 1.5);
    glVertex3f(-2, 8, 15);



    ////////////////////// RIGHT WALL ///////////////////////

    glTexCoord2f(0.0, 4.0);
    glVertex3f(15, 8, -15);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(15, -1, -15);
    
    glTexCoord2f(12.0, 0.0);
    glVertex3f(15, -1, 15);
    
    glTexCoord2f(12.0, 4.0);
    glVertex3f(15, 8, 15);
    


    glEnd();
}
//--------------------------------------------------------------------------------
void floor()
{
    glBindTexture(GL_TEXTURE_2D, txId[8]);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1000);
        glVertex3f(-1500, -1, -1500);
         
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-1500, -1, 1500);
         
        glTexCoord2f(1000.0, 0.0);
        glVertex3f(15, -1, 1500);
         
        glTexCoord2f(1000.0, 1000.0);
        glVertex3f(1500, -1, -1500);
    glEnd();
}

//--------------------------------------------------------------------------------
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}

//--------------------------------------------------------------------------------

void candle()
{
    float wx[N], wy[N], wz[N];
    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0, 0.0, 0.19, 1.0);
    for (int j= 0; j < 36; j++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < N; i++)
        {
            wx[i] = vx[i]*cos(candleAngle) + vz[i]*sin(candleAngle);
            wy[i] = vy[i];
            wz[i] = -vx[i]*sin(candleAngle) + vz[i]*cos(candleAngle);
            if (i > 0){normal(vx[i-1],vy[i-1],vz[i-1],vx[i],vy[i],vz[i], wx[i],wy[i],wz[i]);}
            
            glVertex3f(vx[i],vy[i],vz[i]);
            glVertex3f(wx[i],wy[i],wz[i]);
        }
        glEnd();
        for (int i = 0; i < N; i++)
        {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }
}


void creeper() {
     //0, 2, 0
     glColor3f(0., 1, 0.06);      //Head
    glPushMatrix();
      glTranslatef(0, 2, 0);
      glScalef(1, 0.8, 0.8);
      glutSolidCube(1);
    
    glPopMatrix();
    
    //glColor3f(0., 1, 0.1);      //Body
    glPushMatrix();
    
      glTranslatef(0, 1, 0);
      glScalef(0.8, 1.8, 0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    
        glColor3f(0., 1, 0.1);      //Front foot
    glPushMatrix();
      glTranslatef(0, 0, 0.4);
      glScalef(1, 0.3, 0.3);
      glutSolidCube(1);
    glPopMatrix();
    
    
            glColor3f(0., 1, 0.1);      //Front foot
    glPushMatrix();
      glTranslatef(0, 0, -0.4);
      glScalef(1, 0.3, 0.3);
      glutSolidCube(1);
    glPopMatrix();
    
}

void creeperFace()
{
               //Eye
    glPushMatrix();
    glColor3f(0, 0, 0);
      glTranslatef(-0.2, 2.1, 0.4);
      glScalef(0.1, 0.1, 0.1);
      glutSolidCube(1);
    
    glPopMatrix();
    
                   //Eye
    glPushMatrix();
    glColor3f(0, 0, 0);
      glTranslatef(0.2, 2.1, 0.4);
      glScalef(0.1, 0.1, 0.1);
      glutSolidCube(1);
    
    glPopMatrix();
 
 
                    //Eye
    glPushMatrix();
    glColor3f(0, 0, 0);
      glTranslatef(0., 1.8, 0.4);
      glScalef(0.6, 0.1, 0.1);
      glutSolidCube(1);
    
    glPopMatrix();   
}

void drawCow()
{
    
    glColor3f(1., 1, 0.06);      //Head
    glPushMatrix();
      glTranslatef(-10, 2.2, -3);
      glScalef(0.5, 0.5, 0.75);
      glutSolidCube(1.4);
    glPopMatrix();

    glColor3f(1., 1., 0.);          //Torso
    glPushMatrix();
      glTranslatef(-10, 1.5, -5);
      glRotatef(90, 1, 0, 0);
      glScalef(1.5, 3, 2);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 1., 1.);          //Right leg
    glPushMatrix();
      glTranslatef(-10.5, 1.5, -6);
      glRotatef(-theta, 1, 0 ,0);
      glTranslatef(0.8, -4, 0);
      glTranslatef(-0.8, 2.2, 0);
      glScalef(0.5, 1.5, 0.5);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 1., 1.);          //Left leg
    glPushMatrix();
      glTranslatef(-9.4, 1.5, -6);
      glRotatef(theta, 1, 0 ,0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(0.8, 2.2, 0);
      glScalef(0.5, 1.5, 0.5);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 1., 1.);          //Right arm
    glPushMatrix();
      glTranslatef(-9.4, 1.5, -3.8);
      glRotatef(theta, 1, 0 ,0);
      glTranslatef(2, -6.5, 0);
      glTranslatef(-2, 5, 0);
      glScalef(0.5, 1.5, 0.5);
      glutSolidCube(1);
    glPopMatrix();

    glColor3f(0., 1., 1.);          //Left arm
    glPushMatrix();
    glTranslatef(-10.5, 1.5, -3.8);
    glRotatef(-theta, 1, 0 ,0);
    glTranslatef(-2, -6.5, 0);
    glTranslatef(2, 5, 0);
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1);
    glPopMatrix();
}

void drawCowBody(){
    glColor3f(1., 1., 0.);          //Torso
    glPushMatrix();
    glTranslatef(-10, 1.5, -5);
    glRotatef(90, 1, 0, 0);
    glScalef(1.5, 3, 2);
    glutSolidCube(1);
    glPopMatrix();
}

void dogTimer(int value)
{
    
    theta += 1 * direction;
    if (theta > 20){
        theta = 19;
        direction = -1;
    } else if(theta < -20){
        theta = -19;
        direction = 1;
    }
    
    angle2++;
    glutPostRedisplay();
    glutTimerFunc(50, dogTimer, 0);
}

void creeperTimer(int value)
{
    
    cTimer += .1 * direction;
    if (cTimer > 20){
        cTimer = 19;
        direction = -1;
    } else if(cTimer < 5){
        cTimer = 6;
        direction = 1;
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, creeperTimer, 0);
}

void snowman()
{
    glPushMatrix();
    
    glTranslatef(0, 0, -4);
    
    
    
    glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0.2, 1.2, 0.3);
    glutSolidSphere(0.08, 22, 22);
    glPopMatrix();
    
    
    glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(-0.2, 1.2, 0.3);
    glutSolidSphere(0.08, 22, 22);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(1.0, 0.41, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0, 1, 0);
    glutSolidCone(0.05, 1, 22, 22);
    glPopMatrix();
    
    
        glPushMatrix();
        glColor3f(1.0, 1.0, 1);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0, 1, 0);
    glutSolidSphere(0.4, 22, 22);
    glPopMatrix();
    
    
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(0, 0, 0);
    glutSolidSphere(0.7, 22, 22);
    glPopMatrix();
    
    glPopMatrix();
}
//--------------------------------------------------------------------------------

void penguin()
{
    
    glPushMatrix();
    glTranslatef(7, 0, 7);
    glRotatef(-angle2, 0, 1, 0);
    glTranslatef(-7, 0, -5);
    glTranslatef(8, 0, 6);
    
    glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0.15, 1.6, 0.25);
        glScalef(0.3, 0.3, 0.2);
    glutSolidCube(0.2);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(-0.15, 1.6, 0.25);
        glScalef(0.3, 0.3, 0.2);
    glutSolidCube(0.2);
    glPopMatrix();
    
    
    glPushMatrix();
        glColor3f(1.0, 0.41, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0, 1.5, 0.35);
        glScalef(1, 0.2, 1);
    glutSolidCube(0.2);
    glPopMatrix();
    
    
    glPushMatrix();
        glColor3f(1, 1, 1);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0, 1.5, 0);
        glScalef(1, 1, 1);
    glutSolidSphere(0.3, 40, 22);
    glPopMatrix();
    
    
    glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(-theta, 1, 0, 0);
        glTranslatef(0.2, -0.3, 0);
        glScalef(1, 0.4, 1);
    glutSolidCube(0.2);
    glPopMatrix();
    
       glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 1, 0, 0);
        glTranslatef(-0.2, -0.3, 0);
        glScalef(1, 0.4, 1);
    glutSolidCube(0.2);
    glPopMatrix();
    
    
   glPushMatrix();
        glColor3f(1, 1, 1);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0, 0.5, 0);
        glScalef(1, 1.2, 0.6);
    glutSolidSphere(0.7, 40, 22);
    glPopMatrix();
    
    
    
        glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(0.7, 0.5, 0);
        glScalef(0.3, 0.7, 0.3);
    glutSolidSphere(0.7, 40, 22);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(-0.7, 0.5, 0);
        glScalef(0.3, 0.7, 0.3);
    glutSolidSphere(0.7, 40, 22);
    glPopMatrix();
    
    glPopMatrix();
}

void houseFloor(){
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    glColor3f(1, 1, 1);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glBegin(GL_QUADS);
    for(int i = -300; i < 300; i++)
    {
        for(int j = -300;  j < 300; j++)
        {
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glVertex3f(i+1, 0, j+1);
            glVertex3f(i+1, 0, j);
        }
    }
    glEnd();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    float lgt_pos1[] = {0, 3, 0, 1};
    float lgt_ang[] = {0, -1, theta};
    float lposDog[4] = {0, 2.2, -3, 1};
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1);
    glMatrixMode(GL_PROJECTION);                        
    glLoadIdentity();
    gluPerspective(45., 1., 1., 5000.);

    glMatrixMode(GL_MODELVIEW);                             
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z,  look_x, 0, look_z,   0, 1, 0);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0,GL_POSITION, lposDog);   //Set light position
    skybox();
    floor();
    glEnable(GL_LIGHTING);         //Enable lighting when drawing the model
    
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    drawCowBody();
    glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    
    glDisable(GL_TEXTURE_2D);
    drawCow();
    
    glEnable(GL_TEXTURE_2D);
    
    //------------------CREEPER--------------------------//
    glPushMatrix();
    glTranslatef(10, -0.4, -8);
    glScalef(cTimer*0.15, cTimer*0.15, cTimer*0.15);
    glRotatef(-40, 0, 1, 0);
    glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, txId[11]);
    creeper();
    glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    creeperFace();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(0, -0.92, 0);
    glScalef(0.05, 0.05, 0.05);
    houseFloor();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();


    //-----------CANDLE---------------//
    glPushMatrix();
    
    glTranslatef(-8, -0.5, 9);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(theta, 1, 0, 0);  
     
    glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt_ang);
    
    glDisable(GL_TEXTURE_2D);
    candle();    
    glEnable(GL_TEXTURE_2D);
    
    glPopMatrix();
    //-----------------------//
    
    //-----------PARTICLES--------//
    
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.83, 0.83, 0.83);
    glTranslatef(-8, 3, 9);
    glScalef(20,10,20);
    glRotatef(theta, 1, 0, 0);
    update();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    
    for (int i=0;i<100;i++)              
    {               
        particle[i].xdir=(rand()%55)-30;   
        particle[i].ydir=(rand()%55)-10;       
        particle[i].zdir=(rand()%55)-30;
    }
    //-------------------------------//
    glDisable(GL_TEXTURE_2D);
    snowman();
    glEnable(GL_TEXTURE_2D);
    
    
    glDisable(GL_TEXTURE_2D);
    penguin();
    glEnable(GL_TEXTURE_2D);
    
    if(isSecondCam == 1){
        glPushMatrix();
        eye_x = theta*0.05;
        glRotatef(theta, 0, 0, 1);
        glPopMatrix();
    }
    
    walls();
    roof();
    outsideRoof();

    glFlush();     
    glutSwapBuffers();   
                              
}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
    glutInitWindowSize (1000, 1000); 
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("My house");
    initialise();
    dogTimer(0);
    creeperTimer(0);
    glutDisplayFunc(display); 
    glutSpecialFunc(special);
    glutMainLoop();
   return 0;
}
