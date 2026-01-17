#include <GL/glut.h>
#include <math.h>

// Rotation variables
float angleX = 0.0f;
float angleY = 0.0f;

// Lighting positions
GLfloat light_pos[] = { 2.0f, 5.0f, 5.0f, 1.0f };

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); // Important when scaling objects

    // Set light parameters
    GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
}

// Helper to draw a box with specific dimensions
void drawBox(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawSword() {
    // --- 1. THE HANDLE ---
    glColor3f(0.1f, 0.1f, 0.1f); // Dark Grey
    glPushMatrix();
    glTranslatef(-0.8f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    // Cylinder for grip
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.08, 0.08, 0.6, 20, 20);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // --- 2. THE GUARD/TRIGGER AREA ---
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.0f, 0.0f);
    drawBox(0.4f, 0.25f, 0.15f);
    glPopMatrix();

    // --- 3. THE MAIN GUN BODY (Chassis) ---
    glColor3f(0.15f, 0.15f, 0.15f); // Matte Black/Grey
    glPushMatrix();
    glTranslatef(0.8f, 0.05f, 0.0f);
    drawBox(1.6f, 0.3f, 0.2f);
    glPopMatrix();

    // --- 4. THE BARREL (Top part) ---
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(1.6f, 0.15f, 0.0f);
    glRotatef(90, 0, 1, 0);
    quad = gluNewQuadric();
    gluCylinder(quad, 0.06, 0.06, 0.5, 20, 20);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // --- 5. THE BLADE (Bayonet style - Bottom part) ---
    // The Blade Base
    glColor3f(0.7f, 0.7f, 0.8f); // Metallic Silver
    glPushMatrix();
    glTranslatef(1.8f, -0.15f, 0.0f); // Positioned under the barrel line

    // Draw the main length of the blade
    glPushMatrix();
    glScalef(2.5f, 0.2f, 0.05f); // Long, thin, sharp
    glutSolidCube(1.0f);
    glPopMatrix();

    // Draw the tip (Triangle fan)
    glTranslatef(1.25f, 0.0f, 0.0f); // Move to end of blade body
    glBegin(GL_TRIANGLES);
        // Top face of tip
        glNormal3f(0, 0, 1);
        glVertex3f(0.0f, 0.1f, 0.025f);
        glVertex3f(0.0f, -0.1f, 0.025f);
        glVertex3f(0.5f, 0.0f, 0.0f); // Pointy end

        // Bottom face of tip
        glNormal3f(0, 0, -1);
        glVertex3f(0.0f, 0.1f, -0.025f);
        glVertex3f(0.0f, -0.1f, -0.025f);
        glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();


    // --- 6. THE KAIJU EYE (Signature Feature) ---
    // Narumi's weapon has glowing eyes/cores

    // Set Emission to make it glow
    GLfloat emission[] = { 0.0f, 0.8f, 1.0f, 1.0f }; // Cyan Glow
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan

    glPushMatrix();
    glTranslatef(0.5f, 0.05f, 0.1f); // Positioned on the side of the gun body
    glutSolidSphere(0.12, 20, 20);   // The Eye
    glPopMatrix();

    // Second Eye (Other side)
    glPushMatrix();
    glTranslatef(0.5f, 0.05f, -0.1f);
    glutSolidSphere(0.12, 20, 20);
    glPopMatrix();

    // Reset Emission for other objects
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera View
    gluLookAt(0.0, 0.0, 5.0,  // Eye position
              1.0, 0.0, 0.0,  // Look at center
              0.0, 1.0, 0.0); // Up vector

    // Interactable rotation
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Draw the Weapon
    drawSword();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int x, int y) {
    // Arrow keys to rotate the sword
    switch (key) {
        case GLUT_KEY_RIGHT: angleY += 5.0f; break;
        case GLUT_KEY_LEFT:  angleY -= 5.0f; break;
        case GLUT_KEY_UP:    angleX += 5.0f; break;
        case GLUT_KEY_DOWN:  angleX -= 5.0f; break;
    }
    glutPostRedisplay();
}

int run(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Kaiju No. 8 - Gen Narumi's Weapon");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys); // Register arrow key inputs

    glutMainLoop();
    return 0;
}
