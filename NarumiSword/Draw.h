#include <GL/glu.h>
#include <GL/glut.h>
#include <cstring>
#include <iostream>
#include <math.h>
#include <stdio.h>

// --- Color Definitions ---
GLfloat colorGlow[] = {1.0f, 0.0f, 0.5f,
                       1.0f}; // Electric Pink (Narumi's Eye/Energy)
// GLfloat colorSilver[] = {0.75f, 0.75f, 0.80f, 1.0f};   // Bright Steel/Silver
GLfloat colorSilver[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat colorGunMetal[] = {0.15f, 0.16f, 0.18f, 1.0f}; // Dark Blue-Grey
GLfloat colorGrip[] = {0.05f, 0.05f, 0.05f, 1.0f};     // Almost Black
GLfloat colorBarrel[] = {0.10f, 0.10f, 0.10f, 1.0f};   // Matte Black
GLfloat colorMagazine[] = {0.25f, 0.26f, 0.28f, 1.0f}; // Lighter GunMetal
GLfloat colorTrigger[] = {0.6f, 0.6f, 0.65f, 1.0f};    // Metallic Silver/Grey
GLfloat colorStock[] = {0.76f, 0.60f, 0.42f, 1.0f};    // Desert Combat Tan

GLfloat noEmission[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Reset emission

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

void drawPollygon(float *v1, float t1, float *v2, float t2, float *v3, float t3,
                  float *v4, float t4, unsigned char direction = 'z') {
  float d1 = -t1;
  float d2 = -t2;
  float d3 = -t3;
  float d4 = -t4;

  glBegin(GL_QUADS);

  switch (direction) {
  // --- EXTRUDE ALONG X-AXIS ---
  case 'x':
  case 'X':
    // 1. FRONT FACE (Positive X)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3fv(v4);

    // 2. BACK FACE (Negative X)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(v4[0] + d4, v4[1], v4[2]);
    glVertex3f(v3[0] + d3, v3[1], v3[2]);
    glVertex3f(v2[0] + d2, v2[1], v2[2]);
    glVertex3f(v1[0] + d1, v1[1], v1[2]);

    // 3. SIDE WALLS
    // Bottom (v1-v2)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3f(v2[0] + d2, v2[1], v2[2]);
    glVertex3f(v1[0] + d1, v1[1], v1[2]);

    // Side 2 (v2-v3) - Normal points Z+ (Approx)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3f(v3[0] + d3, v3[1], v3[2]);
    glVertex3f(v2[0] + d2, v2[1], v2[2]);

    // Top (v3-v4)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(v3);
    glVertex3fv(v4);
    glVertex3f(v4[0] + d4, v4[1], v4[2]);
    glVertex3f(v3[0] + d3, v3[1], v3[2]);

    // Side 4 (v4-v1) - Normal points Z- (Approx)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3fv(v4);
    glVertex3fv(v1);
    glVertex3f(v1[0] + d1, v1[1], v1[2]);
    glVertex3f(v4[0] + d4, v4[1], v4[2]);
    break;

  // --- EXTRUDE ALONG Y-AXIS ---
  case 'y':
  case 'Y':
    // 1. FRONT FACE (Positive Y)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3fv(v4);

    // 2. BACK FACE (Negative Y)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(v4[0], v4[1] + d4, v4[2]);
    glVertex3f(v3[0], v3[1] + d3, v3[2]);
    glVertex3f(v2[0], v2[1] + d2, v2[2]);
    glVertex3f(v1[0], v1[1] + d1, v1[2]);

    // 3. SIDE WALLS
    // Side 1 (v1-v2) - Normal Z+ (Approx)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3f(v2[0], v2[1] + d2, v2[2]);
    glVertex3f(v1[0], v1[1] + d1, v1[2]);

    // Side 2 (v2-v3) - Normal X+
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3f(v3[0], v3[1] + d3, v3[2]);
    glVertex3f(v2[0], v2[1] + d2, v2[2]);

    // Side 3 (v3-v4) - Normal Z- (Approx)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3fv(v3);
    glVertex3fv(v4);
    glVertex3f(v4[0], v4[1] + d4, v4[2]);
    glVertex3f(v3[0], v3[1] + d3, v3[2]);

    // Side 4 (v4-v1) - Normal X-
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(v4);
    glVertex3fv(v1);
    glVertex3f(v1[0], v1[1] + d1, v1[2]);
    glVertex3f(v4[0], v4[1] + d4, v4[2]);
    break;

  // --- DEFAULT: EXTRUDE ALONG Z-AXIS ---
  case 'z':
  case 'Z':
  default:
    // 1. FRONT FACE (Positive Z)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3fv(v4);

    // 2. BACK FACE (Negative Z)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(v4[0], v4[1], v4[2] + d4);
    glVertex3f(v3[0], v3[1], v3[2] + d3);
    glVertex3f(v2[0], v2[1], v2[2] + d2);
    glVertex3f(v1[0], v1[1], v1[2] + d1);

    // 3. SIDE WALLS
    // Bottom (v1-v2)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3f(v2[0], v2[1], v2[2] + d2);
    glVertex3f(v1[0], v1[1], v1[2] + d1);

    // Right (v2-v3)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glVertex3f(v3[0], v3[1], v3[2] + d3);
    glVertex3f(v2[0], v2[1], v2[2] + d2);

    // Top (v3-v4)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3fv(v3);
    glVertex3fv(v4);
    glVertex3f(v4[0], v4[1], v4[2] + d4);
    glVertex3f(v3[0], v3[1], v3[2] + d3);

    // Left (v4-v1)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3fv(v4);
    glVertex3fv(v1);
    glVertex3f(v1[0], v1[1], v1[2] + d1);
    glVertex3f(v4[0], v4[1], v4[2] + d4);
    break;
  }

  glEnd();
}

/**
 * Wrapper for backward compatibility:
 * Draws a polygon with uniform thickness.
 */
void drawPollygon(float *v1, float *v2, float *v3, float *v4, float thickness,
                  unsigned char direction = 'z') {
  drawPollygon(v1, thickness, v2, thickness, v3, thickness, v4, thickness,
               direction);
}

/**
 * Function: drawTriangle
 * Purpose: Draws a 3D extruded triangle.
 * Used for: The pointy tip of the blade.
 */
void drawTriangle(float *mainBlade1, float *mainBlade2, float *mainBlade3,
                  float bladeThickness) {
  float depth = (-bladeThickness);

  // Front and Back Faces
  glBegin(GL_TRIANGLES);
  // Front
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3fv(mainBlade1);
  glVertex3fv(mainBlade2);
  glVertex3fv(mainBlade3);
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
  glVertex3fv(mainBlade1);
  glVertex3fv(mainBlade2);
  glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);
  glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);

  // Wall 2
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3fv(mainBlade2);
  glVertex3fv(mainBlade3);
  glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
  glVertex3f(mainBlade2[0], mainBlade2[1], mainBlade2[2] + depth);

  // Wall 3
  glNormal3f(1.0f, 1.0f, 0.0f);
  glVertex3fv(mainBlade3);
  glVertex3fv(mainBlade1);
  glVertex3f(mainBlade1[0], mainBlade1[1], mainBlade1[2] + depth);
  glVertex3f(mainBlade3[0], mainBlade3[1], mainBlade3[2] + depth);
  glEnd();
}

/**
 * Function: drawEdge
 * Purpose: Draws the glowing edge segments.
 * Logic: Creates two thin polygon layers (front and back) with emission
 * enabled.
 */
void drawEdge(float *mainBlade1, float *mainBlade2, float *mainBlade3,
              float *mainBlade4, float bladeThickness) {
  float depth = -bladeThickness / 2;
  float mainBlade1_cpy[3], mainBlade4_cpy[3];

  // Create copies of vertices to adjust depth manually
  std::memcpy(mainBlade1_cpy, mainBlade1, 3 * sizeof(GLfloat));
  std::memcpy(mainBlade4_cpy, mainBlade4, 3 * sizeof(GLfloat));

  mainBlade1_cpy[2] -= bladeThickness;
  mainBlade4_cpy[2] -= bladeThickness;

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

// Helper to draw the Stock with a rounded back end
void drawRoundedStock(float *p1, float *p2, float *p3, float *p4,
                      float thickness) {
  // p1: Top Left, p2: Bottom Left, p3: Bottom Right, p4: Top Right
  // We assume p1-p4 is the top edge, p2-p3 is the bottom edge.
  // We will round the side connecting p3 and p4.

  float halfThick = thickness / 2.0f;
  int segments = 20; // More segments = smoother curve

  // 1. Draw the Main Rectangular Body (Left side)
  // We stop x slightly before the end to leave room for the circle cap
  // Let's assume the curve starts where p3 and p4 x-coordinates are.

  // Find center point between p3 and p4
  float centerX = p3[0]; // Assuming p3 and p4 have aligned X
  float centerY = (p3[1] + p4[1]) / 2.0f;
  float centerZ = p3[2];

  float height = p4[1] - p3[1];
  float radius = height / 2.0f;

  // --- DRAW FLAT SIDES (Front and Back Faces) ---
  for (int face = 0; face < 2; face++) {
    float z = (face == 0) ? centerZ + halfThick : centerZ - halfThick;

    glBegin(GL_POLYGON);
    if (face == 0)
      glNormal3f(0, 0, 1);
    else
      glNormal3f(0, 0, -1);

    // Rectangular part vertices
    glVertex3f(p1[0], p1[1], z); // Top Left
    glVertex3f(p2[0], p2[1], z); // Bottom Left

    // Curved part vertices (Fan)
    // Angle goes from -90 degrees (Bottom) to +90 degrees (Top)
    for (int i = 0; i <= segments; i++) {
      float angle = -M_PI / 2.0f + (M_PI * i / segments);
      float x = centerX + cos(angle) * radius;
      float y = centerY + sin(angle) * radius;
      glVertex3f(x, y, z);
    }

    // Close shape back to top left if needed, but POLYGON handles it
    glEnd();
  }

  // --- DRAW EDGES (Extrusion) ---
  glBegin(GL_QUAD_STRIP);

  // 1. Left Wall (p1 to p2)
  glNormal3f(-1, 0, 0);
  glVertex3f(p1[0], p1[1], centerZ + halfThick);
  glVertex3f(p1[0], p1[1], centerZ - halfThick);
  glVertex3f(p2[0], p2[1], centerZ + halfThick);
  glVertex3f(p2[0], p2[1], centerZ - halfThick);
  glEnd();

  // 2. Top Wall (p1 to Top of Curve)
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0, 1, 0);
  glVertex3f(centerX, p1[1], centerZ + halfThick);
  glVertex3f(centerX, p1[1], centerZ - halfThick);
  glVertex3f(p1[0], p1[1], centerZ + halfThick);
  glVertex3f(p1[0], p1[1], centerZ - halfThick);
  glEnd();

  // 3. Bottom Wall (p2 to Bottom of Curve)
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0, -1, 0);
  glVertex3f(p2[0], p2[1], centerZ + halfThick);
  glVertex3f(p2[0], p2[1], centerZ - halfThick);
  glVertex3f(centerX, p2[1], centerZ + halfThick);
  glVertex3f(centerX, p2[1], centerZ - halfThick);
  glEnd();

  // 4. Curved Wall
  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i <= segments; i++) {
    float angle = -M_PI / 2.0f + (M_PI * i / segments);
    float x = centerX + cos(angle) * radius;
    float y = centerY + sin(angle) * radius;

    // Normal points outward from center
    glNormal3f(cos(angle), sin(angle), 0);

    glVertex3f(x, y, centerZ + halfThick);
    glVertex3f(x, y, centerZ - halfThick);
  }
  glEnd();
}
