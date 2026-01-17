#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstring>

// --- Color Definitions ---
GLfloat colorGlow[]     = { 1.0f, 0.2f, 0.8f, 1.0f }; // Hot Pink/Magenta (Energy glow)

GLfloat noEmission[]    = { 0.0f, 0.0f, 0.0f, 1.0f }; // Reset emission

// --- Helper: Glow Effect ---
// Toggles the emission property to make parts look like they are glowing
void setGlow(bool active) {
    if (active) {
        glMaterialfv(GL_FRONT, GL_EMISSION, colorGlow);
        glColor3fv(colorGlow);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    }
}

/**
 * Function: drawPollygon
 * Purpose: Draws a 3D extruded rectangle (Prism) given 4 front vertices.
 * Used for: Main blade body, joints.
 * order: bottom left -> bottom right -> top right -> top left
 */

void drawPollygon(float* v1, float* v2, float* v3, float* v4, float thickness, unsigned char direction='z') {
    float depth = -thickness; // Extrude backwards

    glBegin(GL_QUADS);

    switch (direction) {
    // --- EXTRUDE ALONG X-AXIS ---
    case 'x':
    case 'X':
        // 1. FRONT FACE (Positive X)
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v4);

        // 2. BACK FACE (Negative X)
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(v4[0] + depth, v4[1], v4[2]);
        glVertex3f(v3[0] + depth, v3[1], v3[2]);
        glVertex3f(v2[0] + depth, v2[1], v2[2]);
        glVertex3f(v1[0] + depth, v1[1], v1[2]);

        // 3. SIDE WALLS
        // Bottom (v1-v2)
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3fv(v1); glVertex3fv(v2);
        glVertex3f(v2[0] + depth, v2[1], v2[2]);
        glVertex3f(v1[0] + depth, v1[1], v1[2]);

        // Side 2 (v2-v3) - Normal points Z+ (Approx)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3fv(v2); glVertex3fv(v3);
        glVertex3f(v3[0] + depth, v3[1], v3[2]);
        glVertex3f(v2[0] + depth, v2[1], v2[2]);

        // Top (v3-v4)
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(v3); glVertex3fv(v4);
        glVertex3f(v4[0] + depth, v4[1], v4[2]);
        glVertex3f(v3[0] + depth, v3[1], v3[2]);

        // Side 4 (v4-v1) - Normal points Z- (Approx)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3fv(v4); glVertex3fv(v1);
        glVertex3f(v1[0] + depth, v1[1], v1[2]);
        glVertex3f(v4[0] + depth, v4[1], v4[2]);
        break;

    // --- EXTRUDE ALONG Y-AXIS ---
    case 'y':
    case 'Y':
        // 1. FRONT FACE (Positive Y)
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v4);

        // 2. BACK FACE (Negative Y)
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(v4[0], v4[1] + depth, v4[2]);
        glVertex3f(v3[0], v3[1] + depth, v3[2]);
        glVertex3f(v2[0], v2[1] + depth, v2[2]);
        glVertex3f(v1[0], v1[1] + depth, v1[2]);

        // 3. SIDE WALLS
        // Side 1 (v1-v2) - Normal Z+ (Approx)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3fv(v1); glVertex3fv(v2);
        glVertex3f(v2[0], v2[1] + depth, v2[2]);
        glVertex3f(v1[0], v1[1] + depth, v1[2]);

        // Side 2 (v2-v3) - Normal X+
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3fv(v2); glVertex3fv(v3);
        glVertex3f(v3[0], v3[1] + depth, v3[2]);
        glVertex3f(v2[0], v2[1] + depth, v2[2]);

        // Side 3 (v3-v4) - Normal Z- (Approx)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3fv(v3); glVertex3fv(v4);
        glVertex3f(v4[0], v4[1] + depth, v4[2]);
        glVertex3f(v3[0], v3[1] + depth, v3[2]);

        // Side 4 (v4-v1) - Normal X-
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3fv(v4); glVertex3fv(v1);
        glVertex3f(v1[0], v1[1] + depth, v1[2]);
        glVertex3f(v4[0], v4[1] + depth, v4[2]);
        break;

    // --- DEFAULT: EXTRUDE ALONG Z-AXIS ---
    case 'z':
    case 'Z':
    default:
        // 1. FRONT FACE (Positive Z)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v4);

        // 2. BACK FACE (Negative Z)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(v4[0], v4[1], v4[2] + depth);
        glVertex3f(v3[0], v3[1], v3[2] + depth);
        glVertex3f(v2[0], v2[1], v2[2] + depth);
        glVertex3f(v1[0], v1[1], v1[2] + depth);

        // 3. SIDE WALLS
        // Bottom (v1-v2)
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3fv(v1); glVertex3fv(v2);
        glVertex3f(v2[0], v2[1], v2[2] + depth);
        glVertex3f(v1[0], v1[1], v1[2] + depth);

        // Right (v2-v3)
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3fv(v2); glVertex3fv(v3);
        glVertex3f(v3[0], v3[1], v3[2] + depth);
        glVertex3f(v2[0], v2[1], v2[2] + depth);

        // Top (v3-v4)
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(v3); glVertex3fv(v4);
        glVertex3f(v4[0], v4[1], v4[2] + depth);
        glVertex3f(v3[0], v3[1], v3[2] + depth);

        // Left (v4-v1)
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3fv(v4); glVertex3fv(v1);
        glVertex3f(v1[0], v1[1], v1[2] + depth);
        glVertex3f(v4[0], v4[1], v4[2] + depth);
        break;
    }

    glEnd();
}

/**
 * Function: drawTriangle
 * Purpose: Draws a 3D extruded triangle.
 * Used for: The pointy tip of the blade.
 */
void drawTriangle(float* mainBlade1, float* mainBlade2, float* mainBlade3, float bladeThickness ){
    float depth = (-bladeThickness);

    // Front and Back Faces
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

    // Side Walls
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

        // Wall 3
        glNormal3f(1.0f, 1.0f, 0.0f);
        glVertex3fv(mainBlade3); glVertex3fv(mainBlade1);
        glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);
        glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
    glEnd();
}

/**
 * Function: drawEdge
 * Purpose: Draws the glowing edge segments.
 * Logic: Creates two thin polygon layers (front and back) with emission enabled.
 */
void drawEdge(float* mainBlade1, float* mainBlade2, float* mainBlade3, float* mainBlade4, float bladeThickness){
    float depth = -bladeThickness/2;
    float mainBlade1_cpy[3], mainBlade4_cpy[3];

    // Create copies of vertices to adjust depth manually
    std::memcpy(mainBlade1_cpy,mainBlade1, 3*sizeof(GLfloat));
    std::memcpy(mainBlade4_cpy,mainBlade4, 3*sizeof(GLfloat));

    mainBlade1_cpy[2]-= bladeThickness;
    mainBlade4_cpy[2]-= bladeThickness;

    setGlow(true); // Enable glowing color

    // Front Edge Face
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, -0.5f);
        glVertex3fv(mainBlade1);
        glVertex3fv(mainBlade2);
        glVertex3fv(mainBlade3);
        glVertex3fv(mainBlade4);
    glEnd();

    // Back Edge Face
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 0.5f);
        glVertex3fv(mainBlade1_cpy);
        glVertex3fv(mainBlade2);
        glVertex3fv(mainBlade3);
        glVertex3fv(mainBlade4_cpy);
    glEnd();

    // Top Cover
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3fv(mainBlade1);
        glVertex3fv(mainBlade2);
        glVertex3fv(mainBlade1_cpy);
    glEnd();

    // Bottom Cover
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3fv(mainBlade4);
        glVertex3fv(mainBlade3);
        glVertex3fv(mainBlade4_cpy);
    glEnd();

    setGlow(false); // Disable glow
}
