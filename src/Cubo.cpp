#include "Cubo.h"

Cubo::Cubo()
{
    GLfloat v_cube1 [8][3] = {
        {0,0,0},{0,0,1},{0,1,0},{0,1,1},
        {1,0,0},{1,0,1},{1,1,0},{1,1,1}
        };
    v_cube = v_cube1;
    GLubyte c_ind1[6][4] = {
        {3,1,5,7},{2,0,1,3},{7,5,4,6},{2,3,7,6},{1,0,4,5},{6,4,0,2}
        };
    c_ind[6][4] = c_ind1
    //ctor
}

Cubo::~Cubo()
{
    //dtor
}

void Cubo::drawcube(float cr, float cg, float cb,int n=1, bool e=false){
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
        Cubo::getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
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

void Cubo::getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3){
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;
    Ux = x2-x1;Uy = y2-y1;Uz = z2-z1;

    Vx = x3-x1;Vy = y3-y1;Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;Ny = Uz*Vx - Ux*Vz;Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}
