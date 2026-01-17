#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include <stdio.h>
#include<math.h>
#include <cstring>

using namespace std;

float angleX = 0.0f;
float angleY = 0.0f;
float posX = 0.0f;
float zoom = 5.0f;

float bladeAndBarrelJointHeight = 0.1f;
float bladeThickness = 0.1f;
float edgeLen = 0.2;
float mainBladeLength = 5.5f;
float mainBladeHeight = 0.5f;
float barrelLength = mainBladeLength+1.0f;
float barrelR = 0.01273f;

// --- COLORS ---
GLfloat colorGunMetal[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat colorBarrel[]   = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat colorGlow[]     = { 1.0f, 0.2f, 0.8f, 1.0f }; // Hot Pink/Magenta
GLfloat noEmission[]    = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat light_pos[] = { 2.0f, 5.0f, 5.0f, 1.0f};

/*
    Format: { x, y, z }
    -x and -y area
*/


/*
    Barrel
*/
float barrel[] = {1.5f, 0.0f, 0.0f};// x,y,z
float barrelRadius = 2*M_PI*barrelR;

/*
    Blade:
    mainBladeJoint part to the barrel
*/
float mainBladeJointDist = 2.5f;
float mainBladeJointAndTriangularStart = -0.8f;
float part1Len = 1.0f;
float bladeAndBarrelJointStart = -barrelR-0.04;

float bladeZ = 0.05f;
float mainBladeLenStartTopX = bladeAndBarrelJointStart;
float mainBladeLenStartBottomX = mainBladeLenStartTopX-0.4f;
float mainBladeHeightStartTopY = bladeAndBarrelJointStart-bladeAndBarrelJointHeight;


float mainBladeJointOne1[] = { mainBladeJointAndTriangularStart, bladeAndBarrelJointStart , bladeZ };//bottom right
float mainBladeJointOne2[] = { mainBladeJointOne1[0], bladeAndBarrelJointStart-bladeAndBarrelJointHeight, bladeZ };//top right
float mainBladeJointOne3[] = { mainBladeJointOne1[0]-part1Len, mainBladeJointOne2[1], bladeZ };//top left
float mainBladeJointOne4[] = { mainBladeJointOne1[0]-part1Len, mainBladeJointOne1[1], bladeZ };// bottom left

float part2Len = 0.6f;
float mainBladeJointTwo1[] = { mainBladeJointOne1[0]-mainBladeJointDist, mainBladeJointOne1[1], bladeZ };
float mainBladeJointTwo2[] = { mainBladeJointOne2[0]-mainBladeJointDist, mainBladeJointOne2[1], bladeZ };
float mainBladeJointTwo3[] = { mainBladeJointOne1[0]-mainBladeJointDist-part2Len, mainBladeJointOne3[1], bladeZ };
float mainBladeJointTwo4[] = { mainBladeJointOne1[0]-mainBladeJointDist-part2Len, mainBladeJointOne4[1], bladeZ };


/*
    Main Blade, body
    first side
*/
float mainBlade1[] = { -mainBladeLength, mainBladeHeightStartTopY-mainBladeHeight,  bladeZ }; // Bottom Left
float mainBlade2[] = { -mainBladeLenStartTopX, mainBladeHeightStartTopY-mainBladeHeight, bladeZ }; // Bottom Right
float mainBlade3[] = { -mainBladeLenStartBottomX, mainBladeHeightStartTopY, bladeZ }; // Top Right
float mainBlade4[] = { -0.5f - mainBladeLength, mainBladeHeightStartTopY, bladeZ }; // Top Left

/*
    Blade:
    Triangular blade part
*/
float mainBladeTriangleSpacing = -0.4f;
float mainBladeTriangleMid = mainBladeHeightStartTopY-mainBladeHeight-0.4f;

float mainBlade5[] = { mainBladeJointAndTriangularStart, mainBladeHeightStartTopY-mainBladeHeight, bladeZ }; //bottom pointy begin
float mainBlade6[] = { mainBladeJointAndTriangularStart+mainBladeTriangleSpacing, mainBladeTriangleMid, bladeZ }; //bottom pointy mid
float mainBlade7[] = { mainBladeJointAndTriangularStart+mainBladeTriangleSpacing*2, mainBladeHeightStartTopY-mainBladeHeight, bladeZ }; //bottom pointy end

/*
    Blade:
    Edge
    uses mainBlade1,mainBlade4 vertices as base
*/

//Front Front
float mainBladeEdgeLeft1[] = { mainBlade1[0]-edgeLen, mainBlade1[1], bladeZ-bladeThickness/2 };
float mainBladeEdgeLeft2[] = { mainBlade4[0]-edgeLen, mainBlade4[1], bladeZ-bladeThickness/2 };

//Front Bottom Edge begin (from base)
float mainBladeEdgeBottomOrigin[] = { mainBlade5[0], mainBlade5[1]-edgeLen+0.05f, bladeZ-bladeThickness/2 };

//Front Triangle Edge mid
float mainBladeTriangleEdgeMid[] = { mainBlade6[0], mainBlade6[1]-edgeLen+0.05f, bladeZ-bladeThickness/2 };

//Front Triangle Edge End
float mainBladeTriangleEdgeEnd[] = { mainBlade7[0], mainBlade7[1]-edgeLen+0.05f, bladeZ-bladeThickness/2 };

//Front Front last
float mainBladeTriangleBegin[] = { mainBlade2[0], mainBlade2[1]-edgeLen+0.05f, bladeZ-bladeThickness/2 };

/*
    Gun Body
*/



void init(){
    glClearColor(0.6f, 0.6f, 0.6f, 0.0f); // Background color
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

}

void setGlow(bool active) {
    if (active) {
        glMaterialfv(GL_FRONT, GL_EMISSION, colorGlow);
        glColor3fv(colorGlow);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    }
}

// New Function: Handles window resizing and sets up the camera lens
void reshape(int w, int h) {
    if (h == 0) h = 1; // Prevent divide by zero
    float ratio = (float)w / h;

    // Switch to Projection Matrix to set up the "Lens"
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set a perspective view (FOV, Aspect Ratio, Near Clip, Far Clip)
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Switch back to ModelView to draw objects
    glMatrixMode(GL_MODELVIEW);
}

void drawPollygon(float* mainBlade1, float* mainBlade2, float* mainBlade3, float* mainBlade4, float bladeThickness) {
    float depth = -bladeThickness; // Extrude backwards

    glBegin(GL_QUADS);

    // 1. FRONT FACE (The original shape)
    // Normal points towards camera (+Z)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(mainBlade1);
    glVertex3fv(mainBlade2);
    glVertex3fv(mainBlade3);
    glVertex3fv(mainBlade4);

    // 2. BACK FACE (Same shape, pushed back)
    // Normal points away from camera (-Z)
    glNormal3f(0.0f, 0.0f, -1.0f);
    // Note: Order reversed (4 -> 3 -> 2 -> 1) so it faces "outwards"
    glVertex3f(mainBlade4[0], mainBlade4[1], mainBlade4[2] + depth);
    glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
    glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);
    glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);

    // 3. BOTTOM WALL (Connects mainBlade1 -> mainBlade2 to Back)
    glNormal3f(0.0f, -1.0f, 0.0f); // Approximate normal
    glVertex3fv(mainBlade1);
    glVertex3fv(mainBlade2);
    glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);
    glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);

    // 4. RIGHT WALL (Connects mainBlade2 -> mainBlade3 to Back)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(mainBlade2);
    glVertex3fv(mainBlade3);
    glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
    glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);

    // 5. TOP WALL (Connects mainBlade3 -> mainBlade4 to Back)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(mainBlade3);
    glVertex3fv(mainBlade4);
    glVertex3f(mainBlade4[0], mainBlade4[1], mainBlade4[2] + depth);
    glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);


    // 6. LEFT WALL (Connects mainBlade4 -> mainBlade1 to Back)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(mainBlade4);
    glVertex3fv(mainBlade1);
    glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);
    glVertex3f(mainBlade4[0], mainBlade4[1], mainBlade4[2] + depth);

    glEnd();
}

void drawTriangle(float* mainBlade1, float* mainBlade2, float* mainBlade3, float bladeThickness ){
    float depth = (-bladeThickness);

    glBegin(GL_TRIANGLES);
        // Front
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3fv(mainBlade1); glVertex3fv(mainBlade2); glVertex3fv(mainBlade3);
        // Back
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
        glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);
        glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);
    glEnd();

    glBegin(GL_QUADS);
        // Wall 1
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3fv(mainBlade1); glVertex3fv(mainBlade2);
        glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);
        glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);

        // Wall 2
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3fv(mainBlade2); glVertex3fv(mainBlade3);
        glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
        glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);

        // Wall 3 (The one you were missing)
        glNormal3f(1.0f, 1.0f, 0.0f);
        glVertex3fv(mainBlade3); glVertex3fv(mainBlade1);
        glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);
        glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
    glEnd();
}

void drawEdge(float* mainBlade1, float* mainBlade2, float* mainBlade3, float* mainBlade4, float bladeThickness){
    float depth = -bladeThickness/2;
    float mainBlade1_cpy[3], mainBlade4_cpy[3];

    std::memcpy(mainBlade1_cpy,mainBlade1, 3*sizeof(GLfloat));
    std::memcpy(mainBlade4_cpy,mainBlade4, 3*sizeof(GLfloat));

    mainBlade1_cpy[2]-= bladeThickness;
    mainBlade4_cpy[2]-= bladeThickness;

    setGlow(true);

    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, -0.5f);
        glVertex3fv(mainBlade1);
        glVertex3fv(mainBlade2);
        glVertex3fv(mainBlade3);
        glVertex3fv(mainBlade4);
    glEnd();

    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 0.5f);
        glVertex3fv(mainBlade1_cpy);
        glVertex3fv(mainBlade2);
        glVertex3fv(mainBlade3);
        glVertex3fv(mainBlade4_cpy);
    glEnd();

    //top cover
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(mainBlade1);
        glVertex3fv(mainBlade2);
        glVertex3fv(mainBlade1_cpy);
    glEnd();

    //bottom cover
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3fv(mainBlade4);
        glVertex3fv(mainBlade3);
        glVertex3fv(mainBlade4_cpy);
    glEnd();

    setGlow(false);
}

// Change: function returns void, takes 'result' array as an argument
void findPointAtDistance(float* mainBlade1, float* mainBlade2, float k, float* result) {

    // 1. Calculate the vector from P1 to P2
    float dx = mainBlade2[0] - mainBlade1[0];
    float dy = mainBlade2[1] - mainBlade1[1];

    // 2. Calculate the length
    float length = sqrt(dx * dx + dy * dy);

    // Safety check
    if (length == 0.0f) {
        result[0] = mainBlade2[0];
        result[1] = mainBlade2[1];
        result[2] = mainBlade2[2];
        return;
    }

    // 3. Normalize
    float unitX = dx / length;
    float unitY = dy / length;

    // 4. Scale and Add
    result[0] = mainBlade2[0] - (unitX * k);
    result[1] = mainBlade2[1] - (unitY * k);

    // Preserve the Z height of the starting point (mainBlade2)
    result[2] = mainBlade2[2];
}

void findPointAtDistanceToY(float* mainBlade1, float* mainBlade2, float k, float* result) {

    // 1. Calculate the vector from P1 to P2
    float dx = mainBlade2[0] - mainBlade1[0];
    float dy = mainBlade2[1] - mainBlade1[1];
    float m = dy/dx;

    // 2. Calculate the y0
    float y = mainBlade2[1]+k;

    // Safety check
    if (dx == 0.0f) {
        result[0] = mainBlade2[0];
        result[1] = mainBlade2[1];
        result[2] = mainBlade2[2];
        return;
    }

    // 3. Normalize
    float x = (k/m) + mainBlade2[0];

    // 4. Scale and Add
    result[0] = x;
    result[1] = y;

    // Preserve the Z height of the starting point (mainBlade2)
    result[2] = mainBlade2[2];
}

void display(){
    // FIX 1: Clear the Depth Buffer too!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIX 2: Reset the matrix!
    glLoadIdentity();

    // Camera setup
    gluLookAt(posX, 0.0, zoom,  // Camera position (Eye)
              posX, 0.0, 0.0,  // Look at center (Target)
              0.0, 1.0, 0.0); // Up vector

    // Apply Rotations
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    glColor3f(0.2f, 0.9f, 0.2f);

    //----------The blade----------
    //main blade body
    glPushMatrix();
    glColor3fv(colorGunMetal);
    drawPollygon(mainBlade1,mainBlade2,mainBlade3,mainBlade4, bladeThickness);
    glPopMatrix();

    //main blade body triangular edge
    glPushMatrix();
    drawTriangle(mainBlade5,mainBlade6,mainBlade7,bladeThickness);
    glPopMatrix();

    //main blade body mainBladeJoint 1
    glPushMatrix();
    drawPollygon(mainBladeJointOne1,mainBladeJointOne2,mainBladeJointOne3,mainBladeJointOne4, bladeThickness);
    glPopMatrix();

    //main blade body mainBladeJoint 1
    glPushMatrix();
    drawPollygon(mainBladeJointTwo1,mainBladeJointTwo2,mainBladeJointTwo3,mainBladeJointTwo4, bladeThickness);
    glPopMatrix();

    //front edge
    float mainBlade1_cpy[3];
    float mainBladeEdgeLeft1_cpy[3];

    findPointAtDistanceToY(mainBladeEdgeLeft2,mainBladeEdgeLeft1,-edgeLen+0.05f,mainBladeEdgeLeft1_cpy);

    glPushMatrix();
    drawEdge(mainBlade1,mainBladeEdgeLeft1_cpy,mainBladeEdgeLeft2,mainBlade4, bladeThickness);
    glPopMatrix();

    glPushMatrix();
    drawEdge(mainBlade1,mainBladeEdgeLeft1_cpy,mainBladeEdgeBottomOrigin,mainBlade5,bladeThickness);
    glPopMatrix();

    glPushMatrix();
    drawEdge(mainBlade5,mainBladeEdgeBottomOrigin,mainBladeTriangleEdgeMid,mainBlade6,bladeThickness);
    glPopMatrix();

    glPushMatrix();
    drawEdge(mainBlade6,mainBladeTriangleEdgeMid,mainBladeTriangleEdgeEnd,mainBlade7,bladeThickness);
    glPopMatrix();

    glPushMatrix();
    drawEdge(mainBlade7,mainBladeTriangleEdgeEnd,mainBladeTriangleBegin,mainBlade2,bladeThickness);
    glPopMatrix();

    //----------The Barrel----------
    glColor3f(0.1f, 0.1f, 0.1f); // Dark Grey
    glPushMatrix();
    glTranslatef(barrel[0],barrel[1],barrel[2]);
    glRotatef(270, 0, 1, 0);
    // Cylinder for grip
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.08, 0.08, barrelLength, 20, 20);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glutSwapBuffers();
}

void specialKey(int key, int x, int y){
    switch(key) {
        case GLUT_KEY_RIGHT: angleY += 5.0f; break;
        case GLUT_KEY_LEFT: angleY -= 5.0f; break;
        case GLUT_KEY_UP: angleX += 5.0f; break;
        case GLUT_KEY_DOWN: angleX -= 5.0f; break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '6': posX += 0.5f; break;
        case '4': posX -= 0.5f; break;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    // SCROLL WHEEL ZOOM
    // (In many GLUT versions, button 3 is Up, 4 is Down)
    if (button == 3) {
         zoom -= 0.1f;
         if (zoom < 1.0f) zoom = 1.0f;
    }
    else if (button == 4) {
         zoom += 0.1f;
    }

    glutPostRedisplay();
}

int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Ensure GLUT_DEPTH is here
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(2800, 100);
    glutCreateWindow("Narumi's Weapon");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Register the reshape function
    glutSpecialFunc(specialKey);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
