#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

float angle = 0.0f;
GLuint earthTextureID;
GLuint moonTextureID;

float light0_angle = 0.0f;
float light0_heigth = 0.0f;
float light0_radius = 10.0f;
float light1_dir[] = { 0.0f, 1.0f, 0.0f, 0.0f };

float moonAngle = 0.0f;
float satelliteAngle = 0.0f;

float camAngle = 45.0f;
float camRadius = 25.0f;

void initLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    float ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    float light1_pos[] = { 0.0f, 30.0f, 0.0f, 1.0f };
    float light1_dir[] = { 0.0f, -1.0f, 0.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

    glPushMatrix();
    glTranslatef(light1_pos[0], light1_pos[1], light1_pos[2]);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.2, 10, 10);
    glPopMatrix();
}

GLuint loadTexture(const char* filename) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        exit(1);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SOIL_free_image_data(image);
    return texID;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = camRadius * sin(glm::radians(camAngle));
    float camZ = camRadius * cos(glm::radians(camAngle));
    gluLookAt(camX, 6, camZ, 0, 0, 0, 0, 1, 0);

    float light0_pos[] = {
        light0_radius * cos(light0_angle),
        light0_heigth,
        light0_radius * sin(light0_angle),
        1.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_dir);


    glPushMatrix();
    glRotatef(-270, 1, 0, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(-1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, earthTextureID);
    GLUquadric* earth = gluNewQuadric();
    gluQuadricTexture(earth, GL_TRUE);
    gluQuadricNormals(earth, GLU_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    gluSphere(earth, 1.5, 75, 75);
    gluDeleteQuadric(earth);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();


    glPushMatrix();
    float moonX = 10.0f * cos(moonAngle);
    float moonZ = 10.0f * sin(moonAngle);
    glTranslatef(moonX, 0.0f, moonZ);

    glBindTexture(GL_TEXTURE_2D, moonTextureID);
    GLUquadric* moon = gluNewQuadric();
    gluQuadricTexture(moon, GL_TRUE);
    gluQuadricNormals(moon, GLU_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    gluSphere(moon, 0.5, 30, 30);
    gluDeleteQuadric(moon);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();


    glPushMatrix();
    float satX = 3.0f * cos(satelliteAngle);
    float satZ = 3.0f * sin(satelliteAngle);
    glTranslatef(satX, 0.0f, satZ);

    glColor3f(0.5f, 0.5f, 1.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1, 0.1, 0.3, 20, 20);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0, 0.1, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.3f);
    gluDisk(quad, 0.0, 0.1, 20, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.15f);
    glScalef(0.3f, 0.02f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0.15f);
    glScalef(0.3f, 0.02f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    gluDeleteQuadric(quad);
    glPopMatrix();

    glutSwapBuffers();
}

void idle() {
    angle += 0.1f;
    if (angle > 360) angle -= 360;
    moonAngle += 0.02f;
    satelliteAngle += 0.05f;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
    case 'w': light0_heigth += 0.2f; break;
    case 's': light0_heigth -= 0.2f; break;
    case 'a': light0_angle += 0.1f; break;
    case 'd': light0_angle -= 0.1f; break;
    case 27: exit(0);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT: camAngle -= 4.0f; break;
    case GLUT_KEY_RIGHT: camAngle += 4.0f; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Projekt GK - Krzysztof Bicz");

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    initLights();
    earthTextureID = loadTexture("worldtex.jpg");
    moonTextureID = loadTexture("moontex.jpg");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
