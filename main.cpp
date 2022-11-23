#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
//#include <cmath>
#include <bits/stdc++.h>
//#include "BmpLoader.h"
#include "RgbImage.h"
#include <string>
#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <math.h>
#include <stdio.h>

#include "RgbImage.h"

//se define la cantidad de texturas que se manejaran
#define NTextures 500

//BENEMERITA UNIVERSIDAD AUTONOMA DE PUEBLA
//GRAFICACION

///INTEGRANTES:
//1.CRUZ LULE MIGUEL ANGEL    MATRICULA: 202080748
//2.JAVIER MONTERO ROSAS      MATRICULA: 202047629
//3.ERICK DE JESUS            MATRICULA: 202033682

///ESTE PROYECTO FINAL CONSTA DE LA PROGRAMACION DE UN VIDEOJUEGO DE UN COCHE
///QUE TRATA DE EVITAR OBSTACULOS EN UNA CALLE, ADEMAS DE QUE GANARA PUNTOS POR EL
///TIEMPO DE SUPERVIVENCIA AL EVITAR EL ATAQUE DE VARIOS OBSTACULOS



///------------------------------------------------------------------------------------------///
//puntuacion maxima de cada integrante del equipo
//1.-miguel:  1083 puntos
//2.-javier:
//3.-erick:
using namespace std;
unsigned int ID;

//se define la cantidad de texturas que se manejaran
#define NTextures 500
GLuint	texture[NTextures];


float rot=0;
float X_MIN=-50;
float X_MAX=50;
float Y_MIN=-50;
float Y_MAX=50;
float Z_MIN=-50;
float Z_MAX=50;
bool l_on1 = true;
bool l_on2 = true;
bool l_on3 = true;
bool ambflag=true;
bool difflag=true;
bool specflag=true;


bool pause = false;
bool start= true;

char* filename0 = "textura0.bmp";

static float zz = 50;
static float yy=1;
static float x_look = 0;
double kon=0.1;
double factor=0.2;
double eyex=0.0+ x_look;
double eyey=1.0;
double eyez=7.5 + zz;
double posx=x_look;
double posy=0.0;
double posz=-10.0+zz;

static float score = 0;
static float final_score = 0;

GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
double Txval=0,Tyval=0,Tzval=0;
double moving=0;

///código de la curva///

const double PI = 3.14159265389;

int anglex= 0, angley = 0, anglez = 0;          //ANGULOS DE ROTACION
int window;
int wired=0;
int shcpt=1;
int animat = 0;

const int L=3;
const int dgre=3;

int ncpt=L+1;
int clikd=0;

const int nt = 40;				//número de rebanadas a lo largo de la dirección x
const int ntheta = 20;

int counter=0;

GLfloat ctrlpoints[L+1][3] ={
    {-2,2,0},{-1,2,0},{0,2,0},{1,2,0}
};


double ex=0, ey=0, ez=15, lx=0,ly=0,lz=0, hx=0,hy=1,hz=0;

float wcsClkDn[3],wcsClkUp[3];


///CLASE PUNTO
class point1{
public:
    point1(){
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];

int flag=0;
GLint viewport[4]; //var para mantener la información de la ventana gráfica
GLdouble modelview[16]; //var para contener la información de la vista del modelo
GLdouble projection[16]; //var para contener la información de la matriz de proyección


void scsToWcs(float sx,float sy, float wcsv[3] );
void processMouse(int button, int state, int x, int y);
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);

void scsToWcs(float sx,float sy, float wcsv[3] ){

    GLfloat winX, winY, winZ; //variables para mantener las coordenadas x,y,z de la pantalla
    GLdouble worldX, worldY, worldZ; //variables para mantener las coordenadas x, y, z del mundo

    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //obtener la información de la matriz de proyección
    glGetIntegerv( GL_VIEWPORT, viewport ); //obtener la información de la ventana gráfica

    winX = sx;
    winY = (float)viewport[3] - (float)sy;
    winZ = 0;

    //obtener las coordenadas mundiales de las coordenadas de la pantalla
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX; wcsv[1]=worldY; wcsv[2]=worldZ;


}


///PUNTOS DE CONTROL
long long nCr(int n, int r){
    if(r > n / 2) r = n - r;
    long long ans = 1;
    int i;
    for(i = 1; i <= r; i++){
        ans *= n - r + i;
        ans /= i;
    }
    return ans;
}

///interpretación polinomial para N puntos
///CURVA DE BAZIER
void BezierCurve ( double t,  float xy[2]){
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++){
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];
    }
    xy[0] = float(x); xy[1] = float(y);
}

void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3){
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;
    Ux = x2-x1;Uy = y2-y1;Uz = z2-z1;
    Vx = x3-x1;Vy = y3-y1;Vz = z3-z1;
    Nx = Uy*Vz - Uz*Vy;Ny = Uz*Vx - Ux*Vz;Nz = Ux*Vy - Uy*Vx;
    glNormal3f(-Nx,-Ny,-Nz);
}

///Botella de Bézier
void bottleBezier(){
    int i, j;
    float x, y, z, r;				//coordenadas actuales
    float x1, y1, z1, r1;			//SIGUIENTES COORDENADAS
    float theta;

    const float startx = 0, endx = ctrlpoints[L][0];

    //número de cortes angulares
    const float dx = (endx - startx) / nt;	//tamaño de paso x
    const float dtheta = 2*PI / ntheta;		//tamaño de paso angular

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //girar sobre el eje z
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i ){//paso a través de x
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];
        //dibuja la superficie compuesta de cuadriláteros barriendo theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta/2; ++j ){
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//actual y próximo y
            z = r * sina;
            z1 = r1 * sina;	//actual y próximo z
            //arista desde el punto en x hasta el punto en el siguiente x
            glTexCoord2d(theta,t-dt);
            glVertex3f (x, y, z);
            if(j>0){
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else{
                p1x=x;p1y=y;p1z=z;
                p2x=x1;p2y=y1;p2z=z1;

            }
            glTexCoord2d(theta,t);
            glVertex3f (x1, y1, z1); //forma quad con el siguiente par de puntos con valor theta incrementado
        }
        glEnd();
        x = x1;
        r = r1;
    }

}

///Fin del código de la curva//


///Cambiar el Tamaño
static void resize(int width, int height){

    //VARIABLE FLOTANTE CONSTANTE
    const float ar = (float) width / (float) height;
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //VARIABLE FLOTANTE
    float d=1;
    //Esta function multiplica la matriz por otra de perspectiva
    glFrustum(-ar*d, ar*d, -1.0*d, 1.0*d, 2.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


///obtenerNormal3p
static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3){
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;
    Ux = x2-x1;Uy = y2-y1;Uz = z2-z1;

    Vx = x3-x1;Vy = y3-y1;Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;Ny = Uz*Vx - Ux*Vz;Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

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

///Cargado de Texturas
void LoadTexture(char *filename, int index){
    glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texture[index]);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, texture[index]);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0,
                     GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
    theTexMap.Reset();
}

///Cilindro 3D
void Cilindro3D(double a1,double b1,double c1){

    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat mat_specular[] = { 1,1,1, 1.0 };
    GLfloat mat_shininess[] = {90};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    GLUquadricObj *quadratic;

    quadratic = gluNewQuadric();
    gluQuadricTexture(quadratic, GL_TRUE);
    glRotatef(-90.0f, 1.0f,0.0f, 0.0f);
    gluCylinder(quadratic,a1,b1,c1,32,32);
    gluDeleteQuadric(quadratic);

}

///Circulo 3D
void circle_3D(GLdouble radius){
    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat mat_specular[] = { 1,1,1, 1.0 };
    GLfloat mat_shininess[] = {90};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);

    glRotatef(270, 1, 0, 0);
    gluSphere(qobj, radius, 20, 20);
    gluDeleteQuadric(qobj);

}

///Arbol
void Arbol(){
    int randm;
    randm = (rand() % 9) + 8;
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glEnable(GL_TEXTURE_2D);
    Cilindro3D(0.4,0.3,randm);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,randm+1,0);
    glBindTexture(GL_TEXTURE_2D,2);
    glEnable(GL_TEXTURE_2D);
    circle_3D(2.3);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8,randm-0.3,0);
    glBindTexture(GL_TEXTURE_2D,2);
    glEnable(GL_TEXTURE_2D);
    circle_3D(2.3);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8,randm-0.3,-0.2);
    glBindTexture(GL_TEXTURE_2D,2);
    glEnable(GL_TEXTURE_2D);
    circle_3D(2.3);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

///Neumático
void Tire()
{
    glPushMatrix();
    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat mat_specular[] = { 0.1,0.1,0.1, 1.0 };
    GLfloat mat_shininess[] = {90};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glutSolidTorus(0.2, 0.8, 5, 50);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0,-0.8,0);
    glScaled(0.1,1.5,0.1);
    drawcube(1,1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.8,0.0,0);
    glRotatef(90,0,0,1);
    glScaled(0.1,1.5,0.1);
    drawcube(1,1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.1,0.0,0);
    glRotatef(45,0,0,1);
    glPushMatrix();
    glTranslated(0.8,0.0,0);
    glRotatef(90,0,0,1);
    glScaled(0.1,1.5,0.1);
    drawcube(1,1,1,1);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-45,0,0,1);
    glPushMatrix();
    glTranslated(0.8,0.0,0);
    glRotatef(90,0,0,1);
    glScaled(0.1,1.5,0.1);
    drawcube(1,1,1,1);
    glPopMatrix();
    glPopMatrix();

}

void reff(void){

    glPushMatrix();
    glScaled(1,10,1);
    drawcube(0,1,0);
    glPopMatrix();
}


void luz(){



    //light
    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_amb[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat l_dif[] = {1,1,1,1};
    GLfloat l_spec[] = {0.2,0.2,0.2,1};
    //GLfloat l_pos1[] = {l1pos,1.0};
    GLfloat l_pos1[] = {0,50,50,1.0};
    GLfloat l_pos2[] = {0,50,-850,1.0};
    //GLfloat l_pos3[] = {l3posx,l3posy,l3posz,1.0};


    //if(l_on1)
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHT2);
    if(l_on1)
    {
        if(ambflag)
        {
            glLightfv(GL_LIGHT0,GL_AMBIENT,l_amb);
        }
        else
        {
            glLightfv(GL_LIGHT0,GL_AMBIENT,l_no);
        }
        if(difflag)
        {
            glLightfv(GL_LIGHT0,GL_DIFFUSE,l_dif);
        }
        else
        {
            glLightfv(GL_LIGHT0,GL_DIFFUSE,l_no);
        }
        if(specflag)
        {
            glLightfv(GL_LIGHT0,GL_SPECULAR,l_spec);
        }
        else
        {
            glLightfv(GL_LIGHT0,GL_SPECULAR,l_no);
        }
    }
    else
    {
        glLightfv(GL_LIGHT0,GL_AMBIENT,l_no);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,l_no);
        glLightfv(GL_LIGHT0,GL_SPECULAR,l_no);

    }
    glLightfv(GL_LIGHT0,GL_POSITION,l_pos1);



    if(l_on2)
    {

        if(ambflag)
        {
            glLightfv(GL_LIGHT1,GL_AMBIENT,l_amb);
        }
        else
        {
            glLightfv(GL_LIGHT1,GL_AMBIENT,l_no);
        }
        if(difflag)
        {
            glLightfv(GL_LIGHT1,GL_DIFFUSE,l_dif);
        }
        else
        {
            glLightfv(GL_LIGHT1,GL_DIFFUSE,l_no);
        }
        if(specflag)
        {
            glLightfv(GL_LIGHT1,GL_SPECULAR,l_spec);
        }
        else
        {
            glLightfv(GL_LIGHT1,GL_SPECULAR,l_no);
        }
    }
    else
    {
        glLightfv(GL_LIGHT1,GL_AMBIENT,l_no);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,l_no);
        glLightfv(GL_LIGHT1,GL_SPECULAR,l_no);

    }
    glLightfv(GL_LIGHT1,GL_POSITION,l_pos2);



}


void spotlight(float x,float y, float z,float spt_cutoff){

    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_amb[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat l_dif[] = {1,1,1,1};
    GLfloat l_spec[] = {0.2,0.2,0.2,1};
    GLfloat l_pos3[] = {x,y+10,z+10,1.0};
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2,GL_AMBIENT,l_amb);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,l_dif);
    glLightfv(GL_LIGHT2,GL_SPECULAR,l_spec);
    glLightfv(GL_LIGHT2,GL_POSITION,l_pos3);

    if(l_on3)
    {

        if(ambflag)
        {
            glLightfv(GL_LIGHT2,GL_AMBIENT,l_amb);
        }
        else
        {
            glLightfv(GL_LIGHT2,GL_AMBIENT,l_no);
        }
        if(difflag)
        {
            glLightfv(GL_LIGHT2,GL_DIFFUSE,l_dif);
        }
        else
        {
            glLightfv(GL_LIGHT2,GL_DIFFUSE,l_no);
        }
        if(specflag)
        {
            glLightfv(GL_LIGHT2,GL_SPECULAR,l_spec);
        }
        else
        {
            glLightfv(GL_LIGHT2,GL_SPECULAR,l_no);
        }
    }
    else
    {
        glLightfv(GL_LIGHT2,GL_AMBIENT,l_no);
        glLightfv(GL_LIGHT2,GL_DIFFUSE,l_no);
        glLightfv(GL_LIGHT2,GL_SPECULAR,l_no);
    }

    GLfloat l_spt[] = {0,0,-1,1};
    GLfloat spt_ct[] = {spt_cutoff};
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l_spt);
    glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spt_ct);


}

///Ejes
void axis(void){
//X axis in red
     glBegin(GL_LINES);
       glColor3f(1.0f,0.0f,0.0f);
       glVertex3f(X_MIN,0.0,0.0);
       glColor3f(1.0f,0.0f,0.0f);
       glVertex3f(X_MAX,0.0,0.0);
     glEnd();
     //Y axis in green
     glBegin(GL_LINES);
       glColor3f(0.0f,1.0f,0.0f);
       glVertex3f(0.0,Y_MIN,0.0);
       glColor3f(0.0f,1.0f,0.0f);
       glVertex3f(0.0,Y_MAX,0.0);
     glEnd();
     //Z axis in blue
     glBegin(GL_LINES);
       glColor3f(0.0f,0.0f,1.0f);
       glVertex3f(0.0,0.0,Z_MIN);
       glColor3f(0.0f,0.0f,1.0f);
       glVertex3f(0.0,0.0,Z_MAX);
     glEnd();
}

///Piso
void Piso(void){

    for(int i=1; i<180; i++)
    {

        if(i<=18||i>=34)
        {
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);

            glPushMatrix();
            glTranslatef(-100,-3,i*-10);
            glScaled(200,1,110);
            drawcube(.322,.745,.5,30);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        }
    }
    glBindTexture(GL_TEXTURE_2D,12);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-100,-3.5,21*-10);
    glScaled(200,1,110);
    drawcube(1,1,1,30);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

///Pista del juego por donde pasa el carro
void pista(void){

    for(int i=1; i<180; i++)
    {
        if(i<=19||i>=22)
        {
            glBindTexture(GL_TEXTURE_2D,1);
            glEnable(GL_TEXTURE_2D);

            glPushMatrix();
            glTranslatef(-7.5,-2.9,-10*i);
            glScaled(17,1,10);
            drawcube(1,1,1,1);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
        }
    }
}

///Apilador
void apilador(){
    for(int i=0; i<180; i++){
        glPushMatrix();
        glTranslatef(-8.5,-2.9,-10*i);

        glPushMatrix();
        glTranslatef(-3.7,0,-10*i);
        glScaled(1,1,1);
        Arbol();
        glPopMatrix();

        GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
        GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
        GLfloat mat_specular[] = { 0.1,0.1,0.1, 1.0 };
        GLfloat mat_shininess[] = {90};

        glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

        glBindTexture(GL_TEXTURE_2D,6);
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        gluQuadricTexture(quadratic, GL_TRUE);
        glRotatef(-90.0f, 1.0f,0.0f, 0.0f);
        gluCylinder(quadratic,0.2f,0.2f,3.0f,32,32);
        gluDeleteQuadric(quadratic);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    for(int i=0; i<180; i++){
        glPushMatrix();
        glTranslatef(10,-2.9,-10*i);
        glPushMatrix();
        glTranslatef(2.8,0,-10*i);
        Arbol();
        glPopMatrix();

        GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
        GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
        GLfloat mat_specular[] = { 0.1,0.1,0.1, 1.0 };
        GLfloat mat_shininess[] = {80};

        glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

        glBindTexture(GL_TEXTURE_2D,6);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        gluQuadricTexture(quadratic, GL_TRUE);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quadratic,0.2f,0.2f,3.0f,32,32);
        gluDeleteQuadric(quadratic);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }


}

///Sendero
void sendero(){
    for(int i=1; i<180; i++){
        glBindTexture(GL_TEXTURE_2D,3);
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glTranslatef(-15,-2.7,-10*i);
        glScaled(7.5,1,10);
        drawcube(1,1,1,3);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }

    for(int i=1; i<180; i++){
        glBindTexture(GL_TEXTURE_2D,3);
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glTranslatef(9,-2.7,-10*i);
        glScaled(6,1,10);
        drawcube(1,1,1,3);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
}

void edificio(void){
    for(int i = 1; i<18; i++){
        int rand1,rand2,rand3,rand4;
        srand(i);
        rand1 = (rand() % 13) + 7;
        srand(i+1);
        rand2 = (rand() % 13) + 7;
        srand(i+2);
        rand3 = (rand() % 13) + 7;
        srand(i+3);
        rand4 = (rand() % 13) + 7;
        //printf("%d :rand1 %d rand2 %d rand3 \n",rand1,rand2,rand3);

        ///edificios de la derecha
        glBindTexture(GL_TEXTURE_2D,13);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(15,-4,-i*100);
        glScaled(10,rand1,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,14);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(15,-4,(-i*100)+25);
        glScaled(10,rand3,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,5);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(15,-4,(-i*100)+50);
        glScaled(10,rand2,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,4);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(15,-4,(-i*100)+75);
        glScaled(10,rand4,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        ///edificios de la izquierda
        glBindTexture(GL_TEXTURE_2D,4);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-25,-4,-i*100);
        glScaled(10,rand4,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,5);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-25,-4,(-i*100)+25);
        glScaled(10,rand2,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,14);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-25,-4,(-i*100)+50);
        glScaled(10,rand1,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,13);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-25,-4,(-i*100)+75);
        glScaled(10,rand3,10);
        drawcube(1,1,1,1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

///Carro
void Carro(void){
    ///Carrocería de nicho de coche
    glBindTexture(GL_TEXTURE_2D,7);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-0.1-1+x_look,-2,-2+zz);
    glScaled(2.2,1,4);
    drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    spotlight(-1+x_look,-1.25,-1.9+zz,30);
    ///luz roja 1
    glBindTexture(GL_TEXTURE_2D,11);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-1+x_look,-1.25,-1.9+zz);
    glScaled(.4,.2,4);
    drawcube(1,1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///luz roja 2
    glBindTexture(GL_TEXTURE_2D,11);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0.6+x_look,-1.25,-1.9+zz);
    glScaled(.4,.2,4);
    drawcube(1,1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///número de placa
    glBindTexture(GL_TEXTURE_2D,10);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-.3+x_look,-1.6,-1.9+zz);
    glScaled(.6,.4,4);
    drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///parachoque
    glPushMatrix();
    glTranslatef(-1+x_look,-1.8,-1.9+zz);
    glScaled(2,.1,4);
    drawcube(255.1/255,255.1/255,255.1/255);
    glPopMatrix();

    ///parte superior del coche
    glBindTexture(GL_TEXTURE_2D,8);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-1+.05+x_look,-1,-1.1+zz);
    glScaled(1.9,.7,2);
    drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///hierba trasera
    glBindTexture(GL_TEXTURE_2D,9);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslated(-1+.15+.05+x_look,-1+.07,-1.1+zz);
    glScaled(1.6,.6,2.1);
    drawcube(1,1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///neumático 1
    glPushMatrix();
    glTranslatef(0.3-1.5+x_look,-1.9,-1.4+zz);
    glRotatef(90,0,1,0);
    glScaled(0.5,0.5,0.5);
    Tire();
    glPopMatrix();

    ///neumático 2
    glPushMatrix();
    glTranslatef(-0.25+1.5+x_look,-1.9,-1.4+zz);
    glRotatef(90,0,1,0);
    glScaled(0.5,0.5,0.5);
    Tire();
    glPopMatrix();

    float dist=2.2;
    ///neumático 3
    glPushMatrix();
    glTranslatef(0.3-1.5+x_look,-1.9,-1.4+zz+dist);
    glRotatef(90,0,1,0);
    glScaled(0.5,0.5,0.5);
    Tire();
    glPopMatrix();

    ///neumático 4
    glPushMatrix();
    glTranslatef(-0.25+1.5+x_look,-1.9,-1.4+zz+dist);
    glRotatef(90,0,1,0);
    glScaled(0.5,0.5,0.5);
    Tire();
    glPopMatrix();
}

///Esfera
void sphere(){
    GLfloat mat_ambient[] = { 1, 0, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 0, 1, 1.0 };
    GLfloat mat_specular[] = { 1,1,1, 1.0 };
    GLfloat mat_shininess[] = {10};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glutSolidSphere (1.0, 20, 16);
}


///Cilindrokata
void cylinderkata(){

    GLfloat mat_ambient[] = { 1, 0, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 0, 1, 1.0 };
    GLfloat mat_specular[] = { 1,1,1, 1.0 };
    GLfloat mat_shininess[] = {10};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);


    glPushMatrix();
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.5f,0.5f,3.0f,32,32);

    ///kata en cilindro 1
        glPushMatrix();
        glRotatef(-40,0,0,1);
        glTranslatef(-1,0,0);
        glScaled(2,.1,0.1);
        drawcube(1,0,0);
        glPopMatrix();

    ///kata en cilindro 2
        glPushMatrix();
        glRotatef(40,0,0,1);
        glTranslatef(-1,0,0.5);
        glScaled(2,.1,0.1);
        drawcube(1,0,0);
        glPopMatrix();

    ///kata en cilindro 3
        glPushMatrix();
        glRotatef(-80,0,0,1);
        glTranslatef(-1,0,1.0);
        glScaled(2,.1,0.1);
        drawcube(1,0,0);
        glPopMatrix();

    ///kata en cilindro 4
        glPushMatrix();
        glRotatef(-30,0,0,1);
        glTranslatef(-1,0,1.5);
        glScaled(2,.1,0.1);
        drawcube(1,0,0);
        glPopMatrix();

    ///kata en cilindro 5
        glPushMatrix();
        glRotatef(60,0,0,1);
        glTranslatef(-1,0,2.0);
        glScaled(2,.1,0.1);
        drawcube(1,0,0);
        glPopMatrix();

    ///kata en cilindro 6
        glPushMatrix();
        glRotatef(100,0,0,1);
        glTranslatef(-1,0,2.5);
        glScaled(2,.1,0.1);
        drawcube(1,0,0);
        glPopMatrix();

    glPopMatrix();
}

void bulbinbuilding(){

    GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
    GLfloat mat_specular[] = { 1,1,1, 1.0 };
    GLfloat mat_shininess[] = {10};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    glutSolidDodecahedron();
    glPopMatrix();

}

///Alcantarilla
void alcantarilla(){
    Alcantarilla alcanta;
    alcanta.draw();
}

///Texto de Display
void textDisplay(string str,int x,int y,int z){

    GLfloat mat_ambient[] = { 1, 1, 0, 1.0 };
    GLfloat mat_diffuse[] = { 1, 1, 0, 1.0 };
    GLfloat mat_specular[] = { 1,1,1, 1.0 };
    GLfloat mat_shininess[] = {10};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);
    glPushMatrix();
    glTranslatef(x, y,z);
    glScalef(0.003f,0.002f,1);

    for (int i=0; i<str.size(); i++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}


float scoresave=0;
float rotat;
float life=5;

string str="";
string strlife="";
stringstream stringtext;
stringstream pausetext;
stringstream openingtext;
stringstream lifee;
float speed=2.0;

///Display
static void display(void){

    //string st;
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    //glClearColor(.2, 0.593, .85540, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    axis();
    /// GLUE LOOK AT
    glLoadIdentity();

    ///Bucle en un rango fijo de pista y hacer que se vea impecable.
    if (zz>-850){
        if(pause){
            zz = zz - speed;
        }
        else{
            zz=zz-0;
        }
    }
    if(zz<-850){
        zz=-50;
        speed+=1.0;///aumento de la velocidad después de cada ciclo
        factor+=0.2;///aumento de la relación de movimiento de izquierda a derecha
    }
    if(pause){
        scoresave=scoresave+1;
    }
    else{
        scoresave=scoresave;
    }
    gluLookAt(0.0+ x_look, yy, 7.5 + zz, x_look, 0.0, -10.0 + zz, 0.0, 1.0, 0.0);
    glRotatef(rot,0,1,0);


    ///mostrando puntuación en el cielo, juego terminado, instrucciones....

    glPushMatrix();
    stringtext.str("");
    if(life==0.0){
        pause=false;

        string str2;
        str2="GAME OVER BUAPO!!";
        textDisplay(str2,x_look-3.5,3,zz);
        stringstream lifeoutput;
        lifeoutput<<scoresave;

        string str3;
        str3="PUNTUACION: " + lifeoutput.str()+" LOBO-PUNTOS";
        textDisplay(str3,x_look-3.5,2,zz);

        string str4;
        str4="REINTENTAR (Z)";
        textDisplay(str4,x_look-3.5,1,zz);
    }
    else{
        if(start){
            string pausestr;
            pausestr="INICIAR (P) ||  PAUSAR (E)";
            textDisplay(pausestr,x_look-3.5,3.5,zz);
            pausestr="CONTROLES:";
            textDisplay(pausestr,x_look-3.5,2.5,zz);

            string contr2;
            contr2="A(IZQ), W(ARRIBA), D(DER), S(ABAJO)";
            textDisplay(contr2,x_look-3.5,1,zz);


        }
        else{
            if(pause){
                stringtext<<scoresave;
                str="Puntuacion: " + stringtext.str();
            }
            else{
                stringtext<<scoresave;
                str="Puntuacion : " + stringtext.str();
                strlife="VIDa"+lifee.str();

                string pausestr;
                pausestr="Presione 'E' para reanudar..";
                textDisplay(pausestr,x_look-3.5,2,zz);
            }


            stringstream lifeoutput;
            lifeoutput<<life;
            string str1;
            str1="VIDAS: " + lifeoutput.str()+"/5";
            textDisplay(str1,x_look-3.5,3,zz);



            textDisplay(str,x_look,3,zz);
            str="";
            strlife="";
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.8,-2,-350);
        glRotatef(107,0.0,0.0,1.0);
        glPushMatrix();
        glRotatef(90,0.0,1.0,0.0);
        glScaled(50,5,5);


        ///código de túnel y texturizado..

        glPopMatrix();
        glPopMatrix();
    }

    luz(); //funcion luz

    glPushMatrix();
    glTranslatef(-5.4+2+1,-1.9,-2-2-2-5-200);
    alcantarilla();
    glPopMatrix();

    glPushMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-20);
    glScaled(5,5,5);
    glPopMatrix();


    ///llamado de los objetos
    apilador();
    sendero();
    Piso();
    pista();
    edificio();

    glPushMatrix();
    glTranslatef(0,0.17,0);
    Carro();
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    glPopMatrix();


    ///detección de colisión
    ///textura del cubo
    ///obtaculo numero 1
    float slide=0,look=0;
    slide = 100;
    look = 3;

    glPushMatrix();
    glTranslatef(-7.4+look,-2.3,-5-slide);
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glScaled(6,1,2);
    drawcube(1,1,1,2);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4 + look,-2.3,-5- slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    ///esfera del edificio
    glPushMatrix();
    glTranslated(-17,6,-42);
    glBindTexture(GL_TEXTURE_2D,8);
    glEnable(GL_TEXTURE_2D);
    bulbinbuilding();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    if (zz>-6.7 - slide&& zz<-2.3- slide){
        if(x_look <2.6 || x_look > 3.3){
            zz = zz + 60;
            life--;
        }
    }

    ///obtaculo numero 2
    slide = 200;
    look = -3;

    if (zz>-6.7 - slide&& zz<10- slide){
        if(x_look <-1.4 || x_look > -.6){
            zz = zz + 60;
            life--;
        }
    }

    ///obtaculo numero 3
    slide = 300;
    look = 3;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-5.4-2 + look,-2.3,-5-slide);
    glScaled(6,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4 + look,-2.3,-5- slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    if (zz>-6.7 - slide&& zz<-2.3- slide){
        if(x_look <2.6 || x_look > 3.3){
            zz = zz + 60;
            life--;
        }
    }

    ///obtaculo numero 4
    slide = 400;
    look = -3;

    glPushMatrix();
    glTranslatef(-5.4+2 +look+moving,-2.3,-5-slide);
    //printf("ML posicion: x: %lf  y:-2.3 z=: %d\n",-5.4+2 +look+moving,-5-slide);
    //printf("Coche posicion: x: %lf  y:-2.3 z=: %d\n",x_look,zz);
    if(zz>=-410&& zz<=-396){
        if(abs((x_look)-(-5.4+2 +look+moving))<=1.4){
            zz = zz + 60;
            life--;
        }
    }
        glPushMatrix();
            glRotatef(rotat,0,1,0);
            glScaled(1,2,2);

                glPushMatrix();
                cylinderkata();
                glPopMatrix();

        glPopMatrix();
    glPopMatrix();

    ///obtaculo numero 5

    slide = -500;
    look = 0;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-5.4,-2.3,-5+ slide);
    glScaled(4,1,2);
    drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4,-2.3,-5+ slide);
    glScaled(4,1,2);
    drawcube(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    if (zz>-507 && zz<-502){
        if(x_look>.4 || x_look < -.4){
            zz = zz + 60;
            life--;

        }
    }


    ///obtaculo numero 6
    slide =-600 ;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-5.4,-2.3,-5 + slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4,-2.3,-5+ slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    if (zz>-6.7+slide && zz<-2.3+slide){
        if(x_look>.4 || x_look < -.4){
            zz = zz + 60;
            life--;

        }
    }


    ///obtaculo numero 7
    slide = 700;
    look = 3;

        glPushMatrix();
        glTranslatef(-7.4+look+moving,0,-5-slide);
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,17);
        glEnable(GL_TEXTURE_2D);
        glRotatef(rotat,0,0,-1);
        circle_3D(2.3);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();

    if (zz>-707 && zz<-702){
        if(abs((x_look)-(-7.4+look+moving))<=1.7){
            zz = zz +60;
            life--;
        }
    }

    ///obtaculo numero 8
    slide = 800;
    look = -3;

        glPushMatrix();
        glTranslatef((-5.4+2 +look+moving)*-1,0,-5-slide);
        glBindTexture(GL_TEXTURE_2D,18);
        glEnable(GL_TEXTURE_2D);
        glRotatef(rotat,0,0,1);
        circle_3D(2.3);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();

    if (zz>-810 && zz<-796){
        if(abs((x_look)-(-1*(-5.4+2+look+moving)))<=1.7){
            zz = zz + 60;
            life--;
        }
    }


    ///Mostrar objetos........
    ///obtaculo numero 9-1
    slide = 900;
    look = 3;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-7.4+look,-2.3,-5-slide);
    glScaled(6,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4 + look,-2.3,-5- slide);

    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///obtaculo numero 10-2
    slide = 1000;
    look = -3;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-5.4+2 + look,-2.3,-5-slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4 +2 + look,-2.3,-5- slide);
    glScaled(8,1,2);
    drawcube(1,1,1,2);
    glPopMatrix();

    ///obtaculo numero 11-3

    slide = -1100;
    look = 0;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-5.4-2 + look,-2.3,-5-slide);
    glScaled(6,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4 + look,-2.3,-5- slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///obtaculo numero 12-4
    slide = 1200;
    look = -3;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(-5.4+2 + look,-2.3,-5-slide);
    glScaled(4,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,20);
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.4 +2 + look,-2.3,-5- slide);
    glScaled(8,1,2);
    drawcube(1,1,1,2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glutSwapBuffers();
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  //  gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glClearColor(0,0,0,0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    LoadTexture(filename0,0);
    //LoadTexture(filename1,1);
    //loadTextureFromFile(filename2,2);
    //loadTextureFromFile(filename3,3);
    //loadTextureFromFile(filename4,4);
    //loadTextureFromFile(filename5,5);
}

///Teclado
static void key(unsigned char key, int x, int y){
    switch (key){
    case 27 :
    ///cerrar ventana del juego ///
    case 'q':case 'Q':
        exit(0);
        break;
    ///pausa del juego///
    case'e':case'E':
        pause=1-pause;
        break;

    ///hacia adelante
    case 'w':case 'W':
        zz = zz - 1.8;
        break;
    ///hacia atras
    case 's': case 'S':
        zz = zz + 0.3;
        break;
    ///hacia la derecha
    case 'd': case 'D':
        if ( x_look < 6.7)
            x_look = x_look + factor;
        else
            x_look = x_look;
        break;
    ///hacia la izquierda
    case 'a': case 'A':
        if ( x_look > -5)
            x_look = x_look - factor;
        else
            x_look = x_look;
        break;
        break;
    //rotacion en el ojo x hacia la izquierda
    case 'm': case 'M':

        kon+=0.01;
        rot+=5;
        break;
    //rotacion en el ojo x hacia la derecha
    case 'n': case 'N':
        kon-=0.01;
        rot-=5;
        break;



        ///Iluminaciones y efectos

    case 'K': case 'k':
        //iluminacion 1
        yy=yy+0.1;

        //iluminacion 1 baja en sentido de rotacion
    case 'j': case 'J':
        yy=yy-0.1;

        //iluminacion 2(quita la iluminacion del pasto)
    case 'f': case 'F':
        l_on1=1-l_on1;
        break;
        //iluminacion 3(como si fuera de noche y letras mas obscuras)
    case 'g':case 'G':
        l_on2=1-l_on2;
        break;
        //iluminacion 4(iluminacion de profundidad en el circuito)
    case 'h': case 'H':
        l_on3=1-l_on3;
        break;

        //iluminacion 5(quita la iluminacion de los arboles)
    case 'r': case 'R':
        ambflag=1-ambflag;
        break;

        ///PAUSA///
    case 'p': case 'P':
        start=1-start;
        pause=1-pause;
        break;
        //iluminacion 6 (total)
    case 't': case 'T':
        difflag=1-difflag;
        break;
        //iluminacion 7(sombras de las banderas)
    case 'y':
    case 'Y':
        specflag=1-specflag;
        break;
        ///REINTENTAR///
    case'z':
    case'Z':
        //pause=true;
        life=3;
        scoresave=0;
        zz=50;
        start=true;
        x_look=0;
        break;

    }
    glutPostRedisplay();
}

///Idle
static void idle(void){
    rotat+=10;
    if(rotat > 360)
        rotat = 0;

    moving+=0.20;
    if(moving>=15){
        moving=0;
    }
    glutPostRedisplay();
}

///Main
int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("PROYECTO FINAL GRAFICACION_NOMBRE DEL JUEGO: Extreme Object Simulador 3000");

    glutReshapeFunc(resize);
    //glutReshapeFunc(reshape)
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
init();
    ///llamado de las texturas desde el folder de documentos madre del proyecto
    glutMainLoop();
    return EXIT_SUCCESS;
}
