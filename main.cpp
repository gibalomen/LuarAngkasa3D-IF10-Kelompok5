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

GLuint texture[20];

GLint slices = 16;
GLint stacks = 16;

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image; //struktur data untuk


//ukuran image #bisa di set sesuai kebutuhan
#define checkImageWidth 164
#define checkImageHeight 164

GLubyte checkImage[checkImageWidth][checkImageHeight][3];

//mengambil gambar BMP

int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // ukuran image dalam bytes
    unsigned long i; // standard counter.
    unsigned short int plane; // number of planes in image

    unsigned short int bpp; // jumlah bits per pixel
    char temp; // temporary color storage for var warna sementara untuk memastikan filenya ada


    if ((file = fopen(filename, "rb")) == NULL) {
        printf("File Not Found : %s\n", filename);
        return 0;
    }
    // mencari file header bmp
    fseek(file, 18, SEEK_CUR);
    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);
    // membaca nilai height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    //menghitung ukuran image(asumsi 24 bits or 3 bytes per pixel).

    size = image->sizeX * image->sizeY * 3;
    // read the planes
    if ((fread(&plane, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }
    if (plane != 1) {
        printf("Planes from %s is not 1: %u\n", filename, plane);
        return 0;
    }
    // read the bitsperpixel
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);

        return 0;
    }
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i = 0; i < size; i += 3) { // membalikan semuan nilai warna (gbr - > rgb)
        temp = image->data[i];
        image->data[i] = image->data[i + 2];
        image->data[i + 2] = temp;
    }
    // we're done.
    return 1;
}




//-------------------------models----------------------------------
//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;

float cRadius = 10.0f; // our radius distance from our character

float lastx, lasty;
float meteor;

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

    star(); //call the star drawing function
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();


    glPushMatrix();
    glRotatef(180, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();


    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();


    //mirroring
    glPushMatrix();
    glRotatef(180, 0.0, 0.0, 1.0);
    glPushMatrix();

    star();
    glPopMatrix();


    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();


    glPushMatrix();
    glRotatef(180, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();


    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    star();
    glPopMatrix();

    glPopMatrix();


}

//-------------------------models---------------
//model alien

void alien(void) {


    //  kepala 
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTranslatef(0.0, 0.0 + 4.0, 0.0);
    gluSphere(quad, 3.0, 20, 4);
    glPopMatrix();
    //  badan   
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTranslatef(0.0, 0.0 + 2.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quad, 2.0, 1.0, 6.0, 6, 4);
    glPopMatrix();
    //tutup badan  
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTranslatef(0.0, 0.0 - 4.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.0, 1.0, 0.0, 6, 4);
    glPopMatrix();

    //  tangan kanan  
    glPushMatrix();
    glTranslatef(0.0, 0.0 + 2.0, 0.0 - 1.0);
    glRotatef(45, 1.0, 0.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.5, 0.0, 6.0, 6, 4);
    glPopMatrix();
    glPopMatrix();

    //  tangan kiri  
    glPushMatrix();
    glTranslatef(0.0, 0.0 + 2.0, 0.0 + 1.0);
    glRotatef(-45, 1.0, 0.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.5, 0.0, 6.0, 6, 4);
    glPopMatrix();
    glPopMatrix();

    //  kaki kanan  
    glPushMatrix();
    glTranslatef(0.0, 0.0 - 4.0, 0.0 - 0.5);
    glRotatef(5, 1.0, 0.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.5, 0.0, 6.0, 6, 4);
    glPopMatrix();
    glPopMatrix();
    //  kaki kiri  
    glPushMatrix();
    glTranslatef(0.0, 0.0 - 4.0, 0.0 + 0.5);
    glRotatef(-5, 1.0, 0.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quad, 0.5, 0.0, 6.0, 6, 4);
    glPopMatrix();
    glPopMatrix();
}



//model2 astronot

void astronot(void) {

    //  kepala 
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTranslatef(0.0, 0.0 + 4.0, 0.0);
    glRotatef(45, 0.0, 0.0, 1.0);
    gluSphere(quad, 3.0, 10, 20);
    glPopMatrix();

    //badan
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glScalef(1.05, 1.4, 1.0);

    glTranslatef(0.0, 0.0 + 0.3, 0.0);
    gluSphere(quad, 3.0, 5, 10);
    glPopMatrix();



    //bag
    glPushMatrix();
    glTranslatef(0.0 + 1.2, 0.0 + 1.3, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glScalef(1.0, 1.5, 1.0);
    glRotatef(45, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);

    gluSphere(quad, 3.0, 4, 10);
    glPopMatrix();
    glPopMatrix();

    //pinggang
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTranslatef(0.0, 0.0 - 3.0, 0.0);
    glScalef(0.8, 1.0, 0.8);
    glRotatef(45, 0.0, 0.0, 1.0);

    gluSphere(quad, 3.0, 5, 10);
    glPopMatrix();

    //tangan kiri atas
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 - 1.5, 0.0, 0.0 - 4.7);
    glRotatef(45, -1.0, 1.0, -1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tangan kiri bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 - 3.55, 0.0 - 1.05, 0.0 - 6.8);
    glRotatef(60, -1.0, 1.0, -1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tutup tangan kiri bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 - 3.55, 0.0 - 1.05, 0.0 - 6.8);
    glRotatef(60, -1.0, 1.0, -1.0);

    gluCylinder(quad, 1.0, 0.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tangan kanan atas
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0, 0.0 + 1.5, 0.0 + 1.8);
    glRotatef(45, 1.0, -1.0, 1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tangan kanan bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 - 1.0, 0.0, 0.0 + 4.4);
    glRotatef(60, 1.0, -1.0, 1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tutup tangan kanan bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 - 2.1, 0.0 - 2.3, 0.0 + 6.7);
    glRotatef(60, 1.0, -1.0, 1.0);

    gluCylinder(quad, 1.0, 0.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //kaki kiri atas
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0, 0.0 - 4.1, 0.0 + 1.0);
    glRotatef(80, 1.0, 1.0, 0.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //kaki kiri bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 + 2.0, 0.0 - 6.3, 0.0 + 1.5);
    glRotatef(100, 1.0, 1.0, 1.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //tutup  kaki kiri bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 + 5.4, 0.0 - 6.9, 0.0 + 2.3);
    glRotatef(100, 1.0, 1.0, 1.0);

    gluCylinder(quad, 0.0, 1.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();



    //kaki kanan atas
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0, 0.0 - 4.1, 0.0 - 1.0);
    glRotatef(100, 1.0, 1.0, 0.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //kaki kanan bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 + 2.0, 0.0 - 6.3, 0.0 - 1.5);
    glRotatef(100, 0.0, 1.0, 0.0);

    gluCylinder(quad, 1.0, 1.0, 3.5, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //tutup  kaki kanan bawah
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);


    glTranslatef(0.0 + 5.5, 0.0 - 6.3, 0.0 - 2.1);
    glRotatef(100, 0.0, 1.0, 0.0);

    gluCylinder(quad, 0.0, 1.0, 0.0, 15, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


}









//model motherspaceship

void motherspaceship(void) {


    //  body 
    glPushMatrix();
    glScalef(2.0, 1.0, 1.23);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTranslatef(0.0, 0.0, 0.0 - 1.5);
    glRotatef(45, 0.0, 0.0, 1.0);


    gluCylinder(quad, 1.0, 1.0, 9.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPopMatrix();

    //tutup  body 
    glPushMatrix();
    glScalef(2.0, 1.0, 1.23);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTranslatef(0.0, 0.0, 0.0 + 7.5);
    glRotatef(45, 0.0, 0.0, 1.0);


    gluCylinder(quad, 1.0, 0.0, 0.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPopMatrix();


    //  tail 
    glPushMatrix();
    glScalef(1.3, 1.3, 0.6);
    glRotatef(90, 0.0, 1.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTranslatef(0.0 + 2.5, 0.0, 0.0 - 6.8);
    glRotatef(45, 0.0, 0.0, 1.0);


    gluCylinder(quad, 0.5, 1.0, 8.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPopMatrix();

    //tutup tail bawah
    glPushMatrix();
    glScalef(1.3, 1.3, 0.6);
    glRotatef(90, 0.0, 1.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTranslatef(0.0 + 2.5, 0.0, 0.0 - 6.8);
    glRotatef(45, 0.0, 0.0, 1.0);


    gluCylinder(quad, 0.0, 0.5, 0.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPopMatrix();

    //tutup tail atas
    glPushMatrix();
    glScalef(1.3, 1.3, 0.6);
    glRotatef(90, 0.0, 1.0, 0.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTranslatef(0.0 + 2.5, 0.0, 0.0 + 1.2);
    glRotatef(45, 0.0, 0.0, 1.0);


    gluCylinder(quad, 1.0, 0.0, 0.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPopMatrix();
}


//model spaceship

void spaceship(void) {



    //  body depan 
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTranslatef(0.0, 0.0, 0.0);
    gluCylinder(quad, 1.0, 1.0, 1.5, 6, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //  kepala
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glTranslatef(0.0, 0.0, 0.0 + 1.5);
    gluCylinder(quad, 1.0, 0.3, 2.0, 6, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //  tutup kepala
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0, 0.0, 0.0 + 3.5);
    gluCylinder(quad, 0.3, 0.0, 0.0, 6, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //  body belakang 
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTranslatef(0.0, 0.0, 0.0 - 1.5);
    gluCylinder(quad, 0.8, 1.0, 1.5, 6, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    // buritan
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTranslatef(0.0, 0.0, 0.0 - 2.5);
    gluCylinder(quad, 1.0, 0.8, 1.0, 6, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    // tutup buritan
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTranslatef(0.0, 0.0, 0.0 - 2.5);
    gluCylinder(quad, 1.0, 0.0, 0.0, 6, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //exhaust

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 - 0.45, 0.0 + 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.8, 0.8, 1.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 - 0.45, 0.0 - 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.8, 0.8, 1.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 + 0.45, 0.0 + 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.8, 0.8, 1.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 + 0.45, 0.0 - 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.8, 0.8, 1.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //tutup
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 - 0.45, 0.0 + 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.0, 0.8, 0.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 - 0.45, 0.0 - 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.0, 0.8, 0.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 + 0.45, 0.0 + 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.0, 0.8, 0.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTranslatef(0.0 + 0.45, 0.0 - 0.35, 0.0 - 3.2);
    glScalef(0.3, 0.3, 1.0);
    gluCylinder(quad, 0.0, 0.8, 0.0, 20, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();


    //wing kiri

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glTranslatef(0.0, 0.0 + 2.2, 0.0 - 2.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(45, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(quad, 0.2, 0.5, 1.5, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glTranslatef(0.0, 0.0 + 2.2, 0.0 - 2.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(45, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(quad, 0.0, 0.2, 0.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    //wing kanan
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glTranslatef(0.0, 0.0 - 2.2, 0.0 - 2.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glRotatef(-45, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(quad, 0.2, 0.5, 1.5, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glTranslatef(0.0, 0.0 - 2.2, 0.0 - 2.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glRotatef(-45, 0.0, 0.0, 1.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(quad, 0.0, 0.2, 0.0, 4, 3); //lebarbawah,lebaratas,panjang,potongan,
    glPopMatrix();



}



//model ufo

void ufo(void) {
    //wing
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glTranslatef(0.0, 0.0, 0.0);

    gluCylinder(quad, 9.0, 0.0, 1.0, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glTranslatef(0.0, 0.0, 0.0 - 1.0);

    gluCylinder(quad, 0.0, 9.0, 1.0, 30, 30);
    glPopMatrix();



}

//model planet

void planet(void) {


    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);

    gluSphere(quad, 50, 25, 25);
    glPopMatrix();

}


//-------------------------load texture----------------------------------
//mengambil tekstur img_model_01aliens

Image * loadTexture1() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_01aliens.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_02aliens

Image * loadTexture2() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_02aliens.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_03astronot

Image * loadTexture3() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_03astronot.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur1

Image * loadTexture4() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_04astronot.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_05motherspaceship

Image * loadTexture5() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_05motherspaceship.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_06motherspaceship

Image * loadTexture6() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_06motherspaceship.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_07ship

Image * loadTexture7() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_07ship.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_08ship

Image * loadTexture8() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_08ship.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_09ship

Image * loadTexture9() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_09ship.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_10ufo

Image * loadTexture10() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_10ufo.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_11planet

Image * loadTexture11() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_11planet.bmp", image1)) {
        exit(1);
    }
    return image1;
}
//mengambil tekstur img_model_11planet

Image * loadTexture12() {
    Image *image1;
    // alokasi memmory untuk tekstur
    image1 = (Image *) malloc(sizeof (Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    //pic.bmp is a 64x64 picture
    if (!ImageLoad("images//img_model_12yellowObject.bmp", image1)) {
        exit(1);
    }
    return image1;
}

void enable(void) {
    glEnable(GL_DEPTH_TEST); //enable the depth testing
    glEnable(GL_LIGHTING); //enable the lighting
    glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH); //set the shader to smooth shader
    glEnable(GL_TEXTURE_2D);
}

void init(void) {
    starPositions(); //membuat starfield
    enable();

    quad = gluNewQuadric();
    glDepthFunc(GL_LESS);
    gluQuadricTexture(quad, 1);



    Image *image1 = loadTexture1();
    Image *image2 = loadTexture2();
    Image *image3 = loadTexture3();
    Image *image4 = loadTexture4();
    Image *image5 = loadTexture5();
    Image *image6 = loadTexture6();
    Image *image7 = loadTexture7();
    Image *image8 = loadTexture8();
    Image *image9 = loadTexture9();
    Image *image10 = loadTexture10();
    Image *image11 = loadTexture11();
    Image *image12 = loadTexture12();


    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate texture/ membuat texture
    glGenTextures(20, texture);



    //-------------------------binding texture----------------------------------
    //image 01 img_model_01aliens
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image1->data);



    //image 02 img_model_02aliens
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image2->data);




    //image 03 img_model_03astronot
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image3->data);




    //image 04 img_model_04astronot
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[4]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image4->data);

    //image 05 img_model_05motherspaceship
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[5]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image5->data);



    //image 06 img_model_06motherspaceship
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[6]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image6->sizeX, image6->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image6->data);




    //image 07 img_model_07ship
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image7->sizeX, image7->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image7->data);




    //image 08 img_model_08ship
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[8]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image8->sizeX, image8->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image8->data);


    //image 09 img_model_09ship
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[9]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image9->sizeX, image9->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image9->data);




    //image 10 img_model_10ufo
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[10]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image10->sizeX, image10->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image10->data);


    //image 11 img_model_11planet
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[11]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image11->sizeX, image11->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image11->data);


    //image 11 img_model_11planet
    //binding texture untuk membuat texture 2D
    glBindTexture(GL_TEXTURE_2D, texture[12]);

    //menyesuaikan ukuran textur ketika image lebih besar dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
    //menyesuaikan ukuran textur ketika image lebih kecil dari texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image12->sizeX, image12->sizeY, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image12->data);

}

void camera(void) {
    glPushMatrix();
    glTranslatef(0.0f, -2.0f, -cRadius);
    glRotatef(-2, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glRotatef(180, 1.0, 0.0, 0.0);

    spaceship(); //Our character to follow


    glPopMatrix();

    //	gluLookAt(xpos, ypos, 50, 0.0, 0.0, 5.0, 0.0, 1.0, 0.0);    
    glRotatef(xrot, 1.0, 0.0, 0.0); //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot, 0.0, 1.0, 0.0); //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos, -ypos, -zpos); //translate the screen to the position of our camera

}

void display(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();




    camera();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    starfield();




    //posisi alien

    glPushMatrix();
    glTranslatef(-130.0, 10.0, 400.0);
    glRotatef(-90, 0.0, 1.0, 0.0);
    glRotatef(-angle, 1.0, 0.0, 0.0);
    alien();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-170.0, 9.0, 380.0);
    glRotatef(-90, 0.0, 1.0, 0.0);
    glRotatef(angle, 1.0, 1.0, 0.0);
    alien();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-170.0, 70.0, 350.0);
    glRotatef(-90, 0.0, 1.0, 0.0);
    glRotatef(-angle, 1.0, 1.0, 1.0);
    alien();
    glPopMatrix();

    //posisi astronot

    glPushMatrix();
    glTranslatef(-5.0, 3.0, 27.0);
    glScalef(0.11, 0.11, 0.11);
    glScalef(-1.0, 1.0, 1.0);
    glRotatef(45, 0.0, 0.0, 1.0);
    glRotatef(-angle, 1.0, 0.0, 0.0);
    astronot();
    glPopMatrix();


    //posisi kapal induk

    glPushMatrix();
    glTranslatef(100.0, 0.0, -70.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(25.0, 25.0, 25.0);
    motherspaceship();
    glPopMatrix();

    //posisi spaceship
    glPushMatrix();
    glTranslatef(-5.0, 0.0, 25.0);
    glRotatef(60, -1.0, 1.0, 0.0);
    glRotatef(-angle, 0.0, 0.0, 1.0);
    spaceship();
    glPopMatrix();


    //posisi ufo

    glPushMatrix();

    glTranslatef(-170.0, 60.0, 400.0);
    glRotatef(-60, 1.0, 0.0, 0.0);
    glRotatef(-angle, 0.0, 0.0, 1.0);

    glScalef(11.0, 11.0, 11.0);
    ufo();
    glPopMatrix();

    //posisi planet
    //earth
    glPushMatrix();

    glPushMatrix();
    glTranslatef(-30.0, 10.0, 150.0);
    //glScalef(2.0,2.0,2.0);
    glRotatef(-15, 0.0, 0.0, 01.0);
    glRotatef(180, 1.0, 0.0, 0.0);
    planet();
    glPopMatrix();
    glPopMatrix();



    //meteor

    glBindTexture(GL_TEXTURE_2D, texture[12]);

    //glTranslatef(0.0, 0.0, 300.0);
    glPushMatrix();
    glTranslatef(0.0, 200.0, 0.0);
    glPushMatrix();
    glScalef(0.1, 1.0, 1.0);
    glRotatef(meteor, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 3000.0);
    glScalef(10.0, 5.0, 5.0);
    gluSphere(quad, 1.0, 5, 2);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glPushMatrix();
    glScalef(0.1, 1.0, 1.0);
    glRotatef(meteor + 1, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 3000.0);
    glScalef(10.0, 5.0, 5.0);
    gluSphere(quad, 1.0, 5, 2);
    glPopMatrix();

    glPopMatrix();



    //glTranslatef(0.0, 0.0, 300.0);
    glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 200.0, 0.0);
    glPushMatrix();
    glScalef(0.1, 1.0, 1.0);
    glRotatef(meteor + 35, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 3000.0);
    glScalef(10.0, 5.0, 5.0);
    gluSphere(quad, 1.0, 5, 2);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glPushMatrix();
    glScalef(0.1, 1.0, 1.0);
    glRotatef(meteor + 36, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 3000.0);
    glScalef(10.0, 5.0, 5.0);
    gluSphere(quad, 1.0, 5, 2);
    glPopMatrix();

    glPopMatrix();






    glutSwapBuffers(); //swap the buffers



    angle = angle + 0.1; //increase the angle
    meteor = meteor + 0.5; //increase meteor speed
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); //set the viewport to the current window specifications
    glMatrixMode(GL_PROJECTION); //set the matrix to projection

    glLoadIdentity();
    gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
    glMatrixMode(GL_MODELVIEW); //set the matrix back to model

}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'q') {
        xrot += 1;
        if (xrot > 360) xrot -= 360;
    }

    if (key == 'z') {
        xrot -= 1;
        if (xrot < -360) xrot += 360;
    }

    if (key == 'w') {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        xpos += float(sin(yrotrad));
        zpos -= float(cos(yrotrad));
        ypos -= float(sin(xrotrad));
    }

    if (key == 's') {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        xpos -= float(sin(yrotrad));
        zpos += float(cos(yrotrad));
        ypos += float(sin(xrotrad));
    }

    if (key == 'd') {
        float yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xpos += float(cos(yrotrad)) * 0.2;
        zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key == 'a') {
        float yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xpos -= float(cos(yrotrad)) * 0.2;
        zpos -= float(sin(yrotrad)) * 0.2;
    }

    if (key == 27) {
        exit(0);
    }
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

    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

