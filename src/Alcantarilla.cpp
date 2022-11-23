#include "Alcantarilla.h"
#include <GL/glut.h>
#include <GL/gl.h>

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

///Dibujar Cubo
void drawcube(float cr, float cg, float cb,int n=1, bool e=false){
    GLfloat m_no[] = {0, 0, 0, 1.0};
    GLfloat m_amb[] = {cr,cg,cb,1};
    GLfloat m_diff[] = {cr,cg,cb,1};
    GLfloat m_spec[] = {1,1,1,1};
    GLfloat m_sh[] = {90};
    GLfloat m_em[] = {1,1,1,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_sh);

    if(e){
        glMaterialfv(GL_FRONT,GL_EMISSION, m_diff);
    }
    else{
        glMaterialfv(GL_FRONT,GL_EMISSION, m_no);
    }

    glBegin(GL_QUADS);
    for(GLint i = 0; i<6; i++){
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(n,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(n,n);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
        glTexCoord2f(0,n);
    }
    glEnd();
}


void Alcantarilla::draw(){

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,15);
    glEnable(GL_TEXTURE_2D);
    glScaled(3,0.1,21);
    drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    for(int i=0; i<15; i++){
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,15);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0.2,0,i*1.5);
        glScaled(0.1,2,0.1);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    for(int i=0; i<15; i++){
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,15);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(2.7,0,i*1.5);
        glScaled(0.1,2,0.1);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
