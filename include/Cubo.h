#ifndef CUBO_H
#define CUBO_H
#include <GL/glut.h>
#include <GL/gl.h>


class Cubo
{
    public:
        static GLfloat v_cube[8][3] ;
        static GLubyte c_ind[6][4] ;
        Cubo();
        static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
        static void drawcube(float cr, float cg, float cb,int n=1, bool e=false);
        virtual ~Cubo();

    protected:

    private:
};

#endif // CUBO_H
