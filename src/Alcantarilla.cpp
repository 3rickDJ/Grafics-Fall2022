#include "Alcantarilla.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include "Cubo.h"

Alcantarilla::Alcantarilla()
{
    //ctor
}

Alcantarilla::~Alcantarilla()
{
    //dtor
}

///V_CUBO
static GLfloat v_cube[8][3] ={
    {0,0,0},{0,0,1},{0,1,0},{0,1,1},
    {1,0,0},{1,0,1},{1,1,0},{1,1,1}
};

///C_IND
static GLubyte c_ind[6][4] ={
    {3,1,5,7},{2,0,1,3},{7,5,4,6},{2,3,7,6},{1,0,4,5},{6,4,0,2}
};


void Alcantarilla::draw(){

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,15);
    glEnable(GL_TEXTURE_2D);
    glScaled(3,0.1,21);
    Cubo::drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    for(int i=0; i<15; i++){
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,15);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0.2,0,i*1.5);
        glScaled(0.1,2,0.1);
        Cubo::drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    for(int i=0; i<15; i++){
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,15);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(2.7,0,i*1.5);
        glScaled(0.1,2,0.1);
        Cubo::drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
