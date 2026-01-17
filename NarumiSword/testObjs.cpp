#include <GL/glut.h>
#include <math.h>

// Rotation variables
float angleX = 0.0f;
float angleY = -15.0f; // Initial slight angle to see the 3D depth

// --- COLORS ---
GLfloat colorGunMetal[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat colorBarrel[]   = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat colorGlow[]     = { 1.0f, 0.2f, 0.8f, 1.0f }; // Hot Pink/Magenta
GLfloat noEmission[]    = { 0.0f, 0.0f, 0.0f, 1.0f };

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    // Setup Lighting
    GLfloat light_pos[] = { 2.0f, 5.0f, 5.0f, 1.0f };
    GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    glClearColor(0.05f, 0.05f, 0.07f, 1.0f); // Dark tint background
}

// Helper: Sets material to glowing pink or dull metal
void setGlow(bool active) {
    if (active) {
        glMaterialfv(GL_FRONT, GL_EMISSION, colorGlow);
        glColor3fv(colorGlow);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    }
}

// Helper: Draw a box centered at origin
void drawBox(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Custom function to draw the jagged blade profile with thickness
void drawBlade() {
    float thickness = 0.08f;

    // We draw the blade in two parts: The dark inner metal, and the glowing edge.

    // --- 1. THE GLOWING EDGE (Slightly larger/offset) ---
    setGlow(true);
    glPushMatrix();
    glTranslatef(0.0f, -0.02f, 0.0f); // Slight offset downwards
    // We use a simplified shape for the glow effect
    glBegin(GL_QUADS);
        // Bottom glowing edge
        glNormal3f(0, 0, 1);
        glVertex3f(0.5f, -0.7f, thickness/2);
        glVertex3f(3.5f, -0.7f, thickness/2);
        glVertex3f(3.5f, -0.8f, thickness/2); // Thickness of the glow
        glVertex3f(0.5f, -0.8f, thickness/2);

        // Angled Tip Glow
        glVertex3f(3.5f, -0.7f, thickness/2);
        glVertex3f(4.2f, -0.3f, thickness/2);
        glVertex3f(4.25f, -0.35f, thickness/2);
        glVertex3f(3.55f, -0.8f, thickness/2);
    glEnd();

    // Duplicate for the other side (backface)
    glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(0.5f, -0.7f, -thickness/2);
        glVertex3f(3.5f, -0.7f, -thickness/2);
        glVertex3f(3.5f, -0.8f, -thickness/2);
        glVertex3f(0.5f, -0.8f, -thickness/2);

        glVertex3f(3.5f, -0.7f, -thickness/2);
        glVertex3f(4.2f, -0.3f, -thickness/2);
        glVertex3f(4.25f, -0.35f, -thickness/2);
        glVertex3f(3.55f, -0.8f, -thickness/2);
    glEnd();
    glPopMatrix();


    // --- 2. THE DARK BLADE BODY ---
    setGlow(false);
    glColor3fv(colorBarrel); // Dark Black/Grey

    // Define vertices for the main blade shape (polygon) based on image
    // Coordinates (X, Y) relative to gun body
    float v[][2] = {
        {0.2f, -0.2f},  // Top Left (near trigger)
        {4.0f, -0.2f},  // Top Right (near tip)
        {4.2f, -0.3f},  // Tip Point
        {3.5f, -0.7f},  // Bottom Angled Cut
        {2.0f, -0.7f},  // Bottom Straight
        {1.8f, -0.5f},  // Cutout Start (step up)
        {1.0f, -0.5f},  // Cutout End
        {0.8f, -0.7f},  // Step down
        {0.5f, -0.7f},  // Back to near handle
        {0.4f, -0.4f}   // Angled up to handle
    };
    int numV = 10;

    // Draw Front Face
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    for(int i=0; i<numV; i++) glVertex3f(v[i][0], v[i][1], thickness/2);
    glEnd();

    // Draw Back Face
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, -1);
    for(int i=numV-1; i>=0; i--) glVertex3f(v[i][0], v[i][1], -thickness/2);
    glEnd();

    // Draw Sides (Connecting walls to make it solid)
    glBegin(GL_QUAD_STRIP);
    for(int i=0; i<numV; i++) {
        // Calculate normals roughly (pointing out) - simplified
        glNormal3f(0, 1, 0);
        glVertex3f(v[i][0], v[i][1], thickness/2);
        glVertex3f(v[i][0], v[i][1], -thickness/2);
    }
    // Close the loop
    glVertex3f(v[0][0], v[0][1], thickness/2);
    glVertex3f(v[0][0], v[0][1], -thickness/2);
    glEnd();

    // --- 3. THE CUTOUT SLOT (The oval hole in the image) ---
    // We simulate this by drawing a glowing box *inside* the blade area
    // In a real engine we would use boolean subtraction, here we just overlay.
    setGlow(true); // Make the inside of the slot glow
    glPushMatrix();
    glTranslatef(1.4f, -0.35f, 0.0f);
    drawBox(0.8f, 0.1f, 0.06f); // The glowing slot
    glPopMatrix();
}

void drawSword() {
    // --- 1. MAIN RECEIVER (The Gun Body) ---
    setGlow(false);
    glColor3fv(colorGunMetal);
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    // Main block
    drawBox(2.2f, 0.45f, 0.3f);

    // Add Vents (Diagonal lines on body from image)
    glColor3f(0.0f, 0.0f, 0.0f); // Black vents
    for(int i=0; i<3; i++) {
        glPushMatrix();
        glTranslatef(0.2f + (i*0.15f), 0.0f, 0.16f); // Push out slightly
        glRotatef(20, 0, 0, 1); // Diagonal
        glScalef(0.05f, 0.3f, 0.02f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix();

    // --- 2. BARREL (Top Cylinder) ---
    setGlow(false);
    glColor3fv(colorBarrel);
    glPushMatrix();
    glTranslatef(1.8f, 0.15f, 0.0f); // Top of body
    glRotatef(90, 0, 1, 0); // Point along X
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.08, 0.08, 2.5, 20, 20); // Long barrel

    // Muzzle tip
    glTranslatef(0.0f, 0.0f, 2.5f);
    gluCylinder(quad, 0.1, 0.1, 0.2, 20, 20);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // --- 3. THE STOCK (Handle Area) ---
    setGlow(false);
    glColor3fv(colorGunMetal);
    glPushMatrix();
    glTranslatef(-0.5f, -0.1f, 0.0f);

    // Angled stock piece top
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    drawBox(1.0f, 0.2f, 0.2f);
    glPopMatrix();

    // Angled stock piece bottom (Triangle shape connection)
    glPushMatrix();
    glTranslatef(-0.2f, -0.2f, 0.0f);
    glRotatef(-45, 0, 0, 1);
    drawBox(0.8f, 0.15f, 0.15f);
    glPopMatrix();

    // The Grip
    glPushMatrix();
    glTranslatef(0.3f, -0.3f, 0.0f);
    glRotatef(10, 0, 0, 1);
    glScalef(0.15f, 0.6f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Glowing Stock Accent (Pink bit at the back)
    setGlow(true);
    glTranslatef(-0.6f, 0.2f, 0.0f);
    drawBox(0.2f, 0.05f, 0.22f);
    glPopMatrix();

    // --- 4. THE BLADE (Custom Shape) ---
    drawBlade();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera
    gluLookAt(0.0, 0.0, 6.0,
              2.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // User rotation
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Center the sword roughly
    glTranslatef(-2.0f, 0.0f, 0.0f);

    drawSword();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT: angleY += 5.0f; break;
        case GLUT_KEY_LEFT:  angleY -= 5.0f; break;
        case GLUT_KEY_UP:    angleX += 5.0f; break;
        case GLUT_KEY_DOWN:  angleX -= 5.0f; break;
    }
    glutPostRedisplay();
}

