/* 
 * File:   main.cpp
 * Author: AditiaPurboAji
 *
 * Created on 13 Juni 2012, 0:01
 */

#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
using namespace std;

/*
 * 
 */

//quadrant
GLUquadric *quad;

//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;

float cRadius = 20.0f; // our radius distance from our character

float lastx, lasty;

//positions of the star
float positionz[100];
float positionx[100];
float positiony[100];

void starPositions(void) { //set the positions of the star

    for (int i = 0; i < 100; i++) {
        positionz[i] = rand() % 130 + 1;
        positionx[i] = rand() % 130 + 1;
        positiony[i] = rand() % 130 + 1;
    }
}

//draw the star

void star(void) {
    for (int i = 0; i < 100 - 1; i++) {
        //  star 
        glPushMatrix();

        glTranslated(-positionx[i + 1] * 10, -positiony[i + 1] * 10, -positionz[i + 1] *10); //translate the star

        gluSphere(quad, 3.0, 5, 2);
        glPopMatrix();

    }

}

void starfield(void) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    star(); //call the star drawing function
    glPopMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 1.0f);
    glPushMatrix();
    glRotatef(180, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();

    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();


    //mirroring
    glPushMatrix();
    glRotatef(180, 0.0, 0.0, 1.0);
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    star();
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 1.0f);
    glPushMatrix();
    glRotatef(180, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();

    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();

    glPopMatrix();


}

void astronot(void) {

    //  kepala 
    glPushMatrix();

    glTranslatef(0.0, 0.0 + 4.0, 0.0);
    glRotatef(45, 0.0, 0.0, 1.0);
    gluSphere(quad, 3.0, 10, 20);
    glPopMatrix();

    //badan
    glPushMatrix();

    glScalef(1.05, 1.4, 1.0);

    glTranslatef(0.0, 0.0 + 0.3, 0.0);
    gluSphere(quad, 3.0, 5, 10);
    glPopMatrix();



    //bag
    glPushMatrix();
    glTranslatef(0.0 + 1.2, 0.0 + 1.3, 0.0);
    glPushMatrix();

    glScalef(1.0, 1.5, 1.0);
    glRotatef(45, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);

    gluSphere(quad, 3.0, 4, 10);
    glPopMatrix();
    glPopMatrix();

    //pinggang
    glPushMatrix();

    glTranslatef(0.0, 0.0 - 3.0, 0.0);
    glScalef(0.8, 1.0, 0.8);
    glRotatef(45, 0.0, 0.0, 1.0);

    gluSphere(quad, 3.0, 5, 10);
    glPopMatrix();

    //tangan kiri atas
    glPushMatrix();



    glTranslatef(0.0 - 1.5, 0.0, 0.0 - 4.7);
    glRotatef(45, -1.0, 1.0, -1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tangan kiri bawah
    glPushMatrix();



    glTranslatef(0.0 - 3.55, 0.0 - 1.05, 0.0 - 6.8);
    glRotatef(60, -1.0, 1.0, -1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tutup tangan kiri bawah
    glPushMatrix();



    glTranslatef(0.0 - 3.55, 0.0 - 1.05, 0.0 - 6.8);
    glRotatef(60, -1.0, 1.0, -1.0);

    gluCylinder(quad, 1.0, 0.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tangan kanan atas
    glPushMatrix();
    //


    glTranslatef(0.0, 0.0 + 1.5, 0.0 + 1.8);
    glRotatef(45, 1.0, -1.0, 1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tangan kanan bawah
    glPushMatrix();



    glTranslatef(0.0 - 1.0, 0.0, 0.0 + 4.4);
    glRotatef(60, 1.0, -1.0, 1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tutup tangan kanan bawah
    glPushMatrix();



    glTranslatef(0.0 - 2.1, 0.0 - 2.3, 0.0 + 6.7);
    glRotatef(60, 1.0, -1.0, 1.0);

    gluCylinder(quad, 1.0, 0.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //kaki kiri atas
    glPushMatrix();



    glTranslatef(0.0, 0.0 - 4.1, 0.0 + 1.0);
    glRotatef(80, 1.0, 1.0, 0.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //kaki kiri bawah
    glPushMatrix();



    glTranslatef(0.0 + 2.0, 0.0 - 6.3, 0.0 + 1.5);
    glRotatef(100, 1.0, 1.0, 1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tutup  kaki kiri bawah
    glPushMatrix();



    glTranslatef(0.0 + 5.4, 0.0 - 6.9, 0.0 + 2.3);
    glRotatef(100, 1.0, 1.0, 1.0);

    gluCylinder(quad, 0.0, 1.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();



    //kaki kanan atas
    glPushMatrix();



    glTranslatef(0.0, 0.0 - 4.1, 0.0 - 1.0);
    glRotatef(100, 1.0, 1.0, 0.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //kaki kanan bawah
    glPushMatrix();



    glTranslatef(0.0 + 2.0, 0.0 - 6.3, 0.0 - 1.5);
    glRotatef(100, 0.0, 1.0, 0.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //tutup  kaki kanan bawah
    glPushMatrix();



    glTranslatef(0.0 + 5.5, 0.0 - 6.3, 0.0 - 2.1);
    glRotatef(100, 0.0, 1.0, 0.0);

    gluCylinder(quad, 0.0, 1.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

}

void planet(void) {

    glPushMatrix();
    glTranslatef(0.0, 30.0, 60.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);


    gluSphere(quad, 20, 25, 25);
    glPopMatrix();


}

void ufo(void) {

    //wing
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);

    gluCylinder(quad, 9.0, 0.0, 1.0, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0 - 1.0);

    gluCylinder(quad, 0.0, 9.0, 1.0, 30, 30);
    glPopMatrix();


}

void init(void) {
    starPositions();


    quad = gluNewQuadric();
    glDepthFunc(GL_LESS);


}

void enable(void) {
    glEnable(GL_DEPTH_TEST); //enable the depth testing
    glEnable(GL_LIGHTING); //enable the lighting
    glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT); //set the shader to smooth shader
}

void camera(void) {

    glTranslatef(0.0f, 0.0f, -cRadius);
    glRotatef(1.0, 1.0, 0.0, 0.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidTeapot(2); //Our character to follow


    //	gluLookAt(xpos, ypos, 50, 0.0, 0.0, 5.0, 0.0, 1.0, 0.0);    
    glRotatef(xrot, 1.0, 0.0, 0.0); //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot, 0.0, 1.0, 0.0); //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos, -ypos, -zpos); //translate the screen to the position of our camera

}

void display(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();



    enable();
    camera();
    starfield();


    glColor3f(1.0f, 1.0f, 1.0f);
    //posisi astronot



    glPushMatrix();
    glTranslatef(0.0, 30.0, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glRotatef(-angle, 1.0, 0.0, 0.0);
    astronot();
    glPopMatrix();


    //posisi planet
    planet();

    //posisi ufo

    glPushMatrix();
    glTranslatef(13.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 1.0);
    glRotatef(-angle, 0.0, 0.0, 1.0);
    ufo();
    glPopMatrix();



















    glutSwapBuffers(); //swap the buffers



    angle = angle + 0.1; //increase the angle
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); //set the viewport to the current window specifications
    glMatrixMode(GL_PROJECTION); //set the matrix to projection

    glLoadIdentity();
    gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
    glMatrixMode(GL_MODELVIEW); //set the matrix back to model

}

void mouseMovement(int x, int y) {
    int diffx = x - lastx; //check the difference between the current x and the last x position
    int diffy = y - lasty; //check the difference between the current y and the last y position
    lastx = x; //set lastx to the current x position
    lasty = y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx; //set the xrot to yrot with the addition of the difference in the x position
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LuarAngkasa3D-IF10-Kelompok5");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);

    glutPassiveMotionFunc(mouseMovement); //check for mouse movement

    glutMainLoop();
    return 0;
}

