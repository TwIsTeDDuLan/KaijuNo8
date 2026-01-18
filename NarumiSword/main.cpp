/**
 * Narumi's Weapon - OpenGL Implementation
 *
 * This program renders a 3D model of Gen Narumi's weapon (Kaiju No. 8)
 * using OpenGL (GLUT/GLU). It features manual geometric modeling for the
 * blade, extrusion logic, and a cylindrical barrel.
 *
 * Controls:
 * - Arrow Keys: Rotate the model (X and Y axis).
 * - '4' / '6' Keys: Pan the camera Left/Right.
 * - Mouse Scroll: Zoom In/Out.
 */

#include "Draw.h"
#include "animations.h"
#include "utils.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cstring>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

// --- Global Transformation Variables ---
float angleX = 0.0f; // Rotation angle around X-axis
float angleY = 0.0f; // Rotation angle around Y-axis
float posX = 0.0f;   // Camera panning position
float posY = 0.0f;
float zoom = 5.0f; // Camera distance (Zoom level)

// =========================================================
// ANIMATION GLOBALS & PARTICLE SYSTEM
// =========================================================
// Animation logic moved to animations.cpp

// ==========================================================================
// 1. CONFIGURATION & DIMENSIONS
// ==========================================================================

// --- Dimensions ---
float bladeAndBarrelJointHeight = 0.3f;
float bladeThickness = 0.2f;
float edgeLen = 0.3f;
float mainBladeLength = 4.5f;
float mainBladeHeight = 0.3f;
float barrelLength = mainBladeLength + 1.0f;
float barrelR = 0.01273f;

// --- Gun Body Dimensions ---
float gunBodyBarrelCoverThickness = 0.05f;
float gunBodyMainLength1 = 0.5f;
float gunBodyMainLength2 = gunBodyMainLength1 + 0.5f;
float gunBodyMainHeight = mainBladeHeight + 0.5f;
float rearBladeLength = 2.0f;
float rearBladeZ = 0.1f;
float rearBladeHeight = -0.2f;
float rearEdgeLen = 0.2f;

float gunBodyHandleLength = 2.0f;
float gunBodyHandleHeight = 0.2f;
float gunTopHandleHeight = 0.2f;
float gunTopHandleLength = 5.0f;
float gunTopHandleSpacer1Length = 1.5f;
float gunTopHandleSpacer2Length = 0.2f;
float gunTopHandleSpacerHeight = 0.2f;
float gunGripHeight1 = 0.4f;
float gunGripLen = 1.0f;
float gunStock1Height = 0.4f;
float gunStock2Height = 0.5f;
float gunStockLen1 = 2.0f;
float gunStockLen2 = gunStockLen1 - gunGripLen;

// --- Colors ---
// --- Colors ---
// Colors are now defined in Draw.h

// --- Lighting & Position ---
GLfloat light_pos[] = {4.0f, 10.0f, 10.0f, 1.0f};
float barrel[] = {1.0f, 0.0f, 0.0f}; // Barrel Attachment Pos

// ==========================================================================
// 2. BASE REFERENCE CALCULATIONS
// ==========================================================================

float barrelRadius = 2 * M_PI * barrelR;
float bladeZ = bladeThickness / 2.0f;

// Y-Axis References
float jointTopY = -barrelR - 0.04f; // Gap below barrel
float mainBladeBodyTopY = jointTopY - bladeAndBarrelJointHeight;

// X-Axis References
float mainBladeBodyStartX = -0.05f;
float mainBladeLenStartBottomX = mainBladeBodyStartX - 0.4f;

// ==========================================================================
// 3. JOINTS (Connects Blade to Gun)
// ==========================================================================

float mainBladeJointDist = 2.5f;
float mainBladeJointAndTriangularStart = -0.8f;
float part1Len = 1.0f;

// --- Joint 1 (Closest to Barrel) ---
float mainBladeJointOne1[] = {mainBladeJointAndTriangularStart, jointTopY,
                              bladeZ};
float mainBladeJointOne2[] = {mainBladeJointOne1[0], mainBladeBodyTopY, bladeZ};
float mainBladeJointOne3[] = {mainBladeJointOne1[0] - part1Len,
                              mainBladeJointOne2[1], bladeZ};
float mainBladeJointOne4[] = {mainBladeJointOne1[0] - part1Len,
                              mainBladeJointOne1[1], bladeZ};

// --- Joint 2 (Further Out) ---
float part2Len = 0.6f;
float jointTwoStartX = mainBladeJointAndTriangularStart - mainBladeJointDist;

float mainBladeJointTwo1[] = {jointTwoStartX, jointTopY, bladeZ};
float mainBladeJointTwo2[] = {jointTwoStartX, mainBladeBodyTopY, bladeZ};
float mainBladeJointTwo3[] = {jointTwoStartX - part2Len, mainBladeBodyTopY,
                              bladeZ};
float mainBladeJointTwo4[] = {jointTwoStartX - part2Len, jointTopY, bladeZ};

// ==========================================================================
// 4. MAIN BLADE GEOMETRY
// ==========================================================================

// --- Main Blade Body ---
float mainBlade1[] = {-mainBladeLength, mainBladeBodyTopY - mainBladeHeight,
                      bladeZ}; // Tip Bottom
float mainBlade2[] = {-mainBladeBodyStartX, mainBladeBodyTopY - mainBladeHeight,
                      bladeZ}; // Rear Bottom
float mainBlade3[] = {-mainBladeLenStartBottomX, mainBladeBodyTopY,
                      bladeZ}; // Rear Top
float mainBlade4[] = {-0.5f - mainBladeLength, mainBladeBodyTopY,
                      bladeZ}; // Tip Top

// --- Triangular Blade Tip ---
float mainBladeTriangleSpacing = -0.4f;
float mainBladeTriangleMid = mainBladeBodyTopY - mainBladeHeight - 0.4f;

float mainBlade5[] = {mainBladeJointAndTriangularStart,
                      mainBladeBodyTopY - mainBladeHeight, bladeZ};
float mainBlade6[] = {mainBladeJointAndTriangularStart +
                          mainBladeTriangleSpacing,
                      mainBladeTriangleMid, bladeZ};
float mainBlade7[] = {mainBladeJointAndTriangularStart +
                          mainBladeTriangleSpacing * 2,
                      mainBladeBodyTopY - mainBladeHeight, bladeZ};

// ==========================================================================
// 5. GLOWING EDGES
// ==========================================================================

// Top Edge
float mainBladeEdgeLeft1[] = {mainBlade1[0] - edgeLen - 0.2f, mainBlade1[1],
                              bladeZ - bladeThickness / 2};
float mainBladeEdgeLeft2[] = {mainBlade4[0] - edgeLen - 0.2f, mainBlade4[1],
                              bladeZ - bladeThickness / 2};

// Bottom Edge Path
float mainBladeEdgeBottomOrigin[] = {mainBlade5[0],
                                     mainBlade5[1] - edgeLen + 0.05f,
                                     bladeZ - bladeThickness / 2};
float mainBladeTriangleEdgeMid[] = {mainBlade6[0],
                                    mainBlade6[1] - edgeLen + 0.05f,
                                    bladeZ - bladeThickness / 2};
float mainBladeTriangleEdgeEnd[] = {mainBlade7[0],
                                    mainBlade7[1] - edgeLen + 0.05f,
                                    bladeZ - bladeThickness / 2};
float mainBladeTriangleBegin[] = {mainBlade2[0],
                                  mainBlade2[1] - edgeLen + 0.05f,
                                  bladeZ - bladeThickness / 2};

// ==========================================================================
// 6. GUN BODY: BARREL COVERS
// ==========================================================================

float gunBodyMag = -mainBladeLenStartBottomX + 0.1f;
float gunBodyBaseZ = bladeZ + barrelR + 0.01f;
float frontCoverZ = gunBodyBaseZ + gunBodyBarrelCoverThickness;
float backCoverZ = -gunBodyBaseZ;
float gunBodyTopY = mainBladeBodyTopY + barrelR * 2 + 0.5f;

// --- Front Cover ---
float gunBodyBarrelCover1[] = {mainBladeJointTwo1[0] + 0.2f, gunBodyTopY,
                               frontCoverZ};
float gunBodyBarrelCover2[] = {mainBladeJointTwo1[0] + 0.7f,
                               jointTopY - bladeAndBarrelJointHeight / 2,
                               frontCoverZ};
float gunBodyBarrelCover3[] = {gunBodyMag, gunBodyBarrelCover2[1], frontCoverZ};
float gunBodyBarrelCover4[] = {gunBodyMag, gunBodyBarrelCover1[1], frontCoverZ};

// --- Back Cover ---
float gunBodyBarrelCover5[] = {gunBodyBarrelCover1[0], gunBodyBarrelCover1[1],
                               backCoverZ};
float gunBodyBarrelCover6[] = {gunBodyBarrelCover2[0], gunBodyBarrelCover2[1],
                               backCoverZ};
float gunBodyBarrelCover7[] = {gunBodyBarrelCover3[0], gunBodyBarrelCover3[1],
                               backCoverZ};
float gunBodyBarrelCover8[] = {gunBodyBarrelCover4[0], gunBodyBarrelCover4[1],
                               backCoverZ};

// ==========================================================================
// 7. GUN BODY: MAIN SEGMENTS (Sequential Build)
// ==========================================================================

float gunBodyMainThickness = backCoverZ - frontCoverZ;

// --- Part One ---
float gunBodyMain1[] = {gunBodyBarrelCover3[0], gunBodyBarrelCover3[1],
                        gunBodyBarrelCover3[2]};
float gunBodyMain2[] = {gunBodyBarrelCover3[0],
                        gunBodyBarrelCover3[1] - gunBodyMainHeight,
                        gunBodyBarrelCover3[2]};
float gunBodyMain3[] = {gunBodyMain2[0] + gunBodyMainLength2, gunBodyMain2[1],
                        gunBodyMain2[2]};
float gunBodyMain4[] = {gunBodyMain1[0] + gunBodyMainLength1, gunBodyMain1[1],
                        gunBodyMain1[2]};

// --- Part Two ---
float gunBodyMain5[] = {gunBodyMain4[0], gunBodyMain4[1] - 0.5f,
                        gunBodyMain4[2]};
float gunBodyMain6[] = {gunBodyMain3[0], gunBodyMain3[1], gunBodyMain3[2]};
float gunBodyMain7[] = {gunBodyMain6[0] + gunBodyMainLength1, gunBodyMain6[1],
                        gunBodyMain6[2]};
float gunBodyMain8[] = {gunBodyMain6[0] + gunBodyMainLength1, gunBodyMain5[1],
                        gunBodyMain5[2]};

// --- Part Three ---
float gunBodyMain9[] = {gunBodyMain8[0], gunBodyMain1[1], gunBodyMain8[2]};
float gunBodyMain10[] = {gunBodyMain7[0], gunBodyMain7[1], gunBodyMain7[2]};
float gunBodyMain11[] = {gunBodyMain7[0] + gunBodyMainLength1, gunBodyMain7[1],
                         gunBodyMain7[2]};
float gunBodyMain12[] = {gunBodyMain9[0] + gunBodyMainLength1, gunBodyMain1[1],
                         gunBodyMain9[2]};

// --- Part Four (Upper Connector) ---
float gunBodyMain13[] = {gunBodyBarrelCover4[0], gunBodyBarrelCover4[1],
                         gunBodyMain9[2]};
float gunBodyMain14[] = {gunBodyBarrelCover3[0], gunBodyMain9[1],
                         gunBodyMain9[2]};
float gunBodyMain15[] = {gunBodyMain12[0], gunBodyMain12[1], gunBodyMain12[2]};
float gunBodyMain16[] = {gunBodyMain12[0], gunBodyBarrelCover4[1],
                         gunBodyMain9[2]};

// ==========================================================================
// 8. GUN BODY: stock
// ==========================================================================

//--- 1. Grip ---
float gunGrip1[] = {gunBodyMain11[0], gunBodyMain11[1] + gunGripHeight1,
                    gunBodyMain7[2]};
float gunGrip2[] = {gunBodyMain11[0], gunBodyMain11[1], gunBodyMain7[2]};
float gunGrip3[] = {gunBodyMain11[0] + gunGripLen,
                    gunBodyMain7[1] - gunGripHeight1, gunBodyMain7[2]};
float gunGrip4[] = {gunGrip3[0], gunGrip3[1] + gunGripHeight1, gunGrip3[2]};

//--- 2.Stock (part that connect to the main body) ---
float gunStock1[] = {gunBodyMain15[0], gunBodyMain15[1] + gunStock1Height,
                     gunBodyMain15[2]};
float gunStock2[] = {gunBodyMain15[0], gunBodyMain15[1], gunBodyMain15[2]};
float gunStock3[] = {gunStock2[0] + gunStockLen1, gunStock2[1], gunStock2[2]};
float gunStock4[] = {gunStock3[0], gunStock1[1], gunStock3[2]};

//--- 3.Lower Stock (Part that connect Stock and Grip) ---
float gunLowStock1[] = {gunGrip4[0], gunGrip4[1], gunGrip4[2]};
float gunLowStock2[] = {gunGrip3[0], gunGrip3[1], gunGrip3[2]};
float gunLowStock3[] = {gunStock3[0] + gunStock2Height, gunStock3[1],
                        gunStock3[2]};
float gunLowStock4[] = {gunStock3[0], gunStock3[1], gunStock3[2]};

// ==========================================================================
// 9. GUN BODY: TOP HANDLE
// ==========================================================================
// --- 1. REAR SPACER (Connector at the back) ---

float gunTopHandleSpacer1[] = {gunBodyMain16[0],
                               gunBodyMain16[1] + gunTopHandleSpacerHeight,
                               gunBodyMain16[2]}; // Top Right
float gunTopHandleSpacer2[] = {gunBodyMain16[0], gunBodyMain16[1],
                               gunBodyMain16[2]}; // Bottom Right
float gunTopHandleSpacer3[] = {gunBodyMain16[0] - gunTopHandleSpacer1Length,
                               gunBodyMain16[1],
                               gunBodyMain16[2]}; // Bottom Left
float gunTopHandleSpacer4[] = {gunBodyMain16[0] - gunTopHandleSpacer1Length,
                               gunBodyMain16[1] + gunTopHandleSpacerHeight,
                               gunBodyMain16[2]}; // Top Left

// --- 2. TOP HANDLE BAR ---

float gunTopHandle1[] = {gunTopHandleSpacer1[0] - 0.1f,
                         gunTopHandleSpacer1[1] + gunTopHandleHeight,
                         gunTopHandleSpacer1[2] - 0.05f}; // Top Right
float gunTopHandle2[] = {gunTopHandleSpacer1[0], gunTopHandleSpacer1[1],
                         gunTopHandleSpacer1[2]}; // Bottom Right
float gunTopHandle3[] = {gunBodyMain16[0] - gunTopHandleLength,
                         gunTopHandleSpacer1[1],
                         gunTopHandleSpacer1[2]}; // Bottom Left
float gunTopHandle4[] = {gunBodyMain16[0] - gunTopHandleLength + 0.1f,
                         gunTopHandleSpacer1[1] + gunTopHandleHeight,
                         gunTopHandleSpacer1[2] - 0.05f}; // Top Left

// --- 3. FRONT SPACER (Connector at the front) ---

float gunTopHandleSpacer5[] = {gunTopHandle3[0], gunTopHandle3[1],
                               gunTopHandle3[2]}; // Top Left (Touches Handle)
float gunTopHandleSpacer6[] = {
    gunTopHandle3[0], gunTopHandle3[1] - gunTopHandleSpacerHeight,
    gunTopHandle3[2]}; // Bottom Left (Touches Gun Body)
float gunTopHandleSpacer7[] = {gunTopHandle3[0] + gunTopHandleSpacer2Length,
                               gunTopHandle3[1] - gunTopHandleSpacerHeight,
                               gunTopHandle3[2]}; // Bottom Right
float gunTopHandleSpacer8[] = {gunTopHandle3[0] + gunTopHandleSpacer2Length,
                               gunTopHandle3[1], gunTopHandle3[2]}; // Top Right

// ==========================================================================
// 10. REAR BLADE (Added)
// ==========================================================================

// --- 1. Rear Blade Body ---
// Starts at the back of the stock (gunStock4/3) and goes UP/RIGHT
// Connection points (Base)
float rearBlade1[] = {gunStock4[0], gunStock4[1] + rearBladeHeight,
                      rearBladeZ};                             // Top Left
float rearBlade2[] = {gunStock3[0], gunStock3[1], rearBladeZ}; // Bottom Left
float rearBlade3[] = {gunStock3[0] + rearBladeLength + 0.5f, gunStock3[1],
                      rearBladeZ}; // Bottom right
float rearBlade4[] = {gunStock4[0] + rearBladeLength,
                      gunStock4[1] + rearBladeHeight, rearBladeZ}; // Top Right

// --- 2. Rear Triangular Spike (Glowing Part) ---
// Similar logic to the front blade's spike, but inverted direction
float rearSpikeStart[] = {rearBlade1[0] + 0.5f, rearBlade1[1],
                          rearBladeZ}; // Start slightly in
float rearSpikeEnd[] = {rearBlade1[0] + 1.3f, rearBlade1[1], rearBladeZ};
float rearSpikeTip[] = {rearBlade1[0] + 0.9f, rearBlade1[1] + 0.3f,
                        rearBladeZ}; // Points UP

// --- 3. Rear Glowing Edges (Neon) ---
// Edge for the main top slope
float rearEdge1[] = {rearBlade1[0], rearBlade1[1] + rearEdgeLen,
                     bladeZ - bladeThickness / 2}; // Tip Top Inner
float rearEdge2[] = {rearBlade3[0] + rearEdgeLen + 0.3f, rearBlade3[1],
                     bladeZ - bladeThickness / 2}; // Base Top Inner
float rearEdge3[] = {rearBlade4[0] + rearEdgeLen + 0.3f, rearBlade4[1],
                     bladeZ - bladeThickness / 2};

// Edge for the Triangle Spike
float rearSpikeEdge1[] = {rearSpikeStart[0], rearSpikeStart[1] + rearEdgeLen,
                          bladeZ - bladeThickness / 2}; // Start Inner
float rearSpikeEdge2[] = {rearSpikeTip[0], rearSpikeTip[1] + rearEdgeLen,
                          bladeZ - bladeThickness / 2}; // Tip Inner
float rearSpikeEdge3[] = {rearSpikeEnd[0], rearSpikeEnd[1] + rearEdgeLen,
                          bladeZ - bladeThickness / 2}; // End Inner

// --- Initialization ---
// Sets up OpenGL state, lighting, and materials
void init() {
  glClearColor(0.25f, 0.25f, 0.30f,
               1.0f); // Dark blue-grey background for contrast
  glEnable(GL_DEPTH_TEST); // Enable Z-buffer (so objects don't draw over each other)
  glEnable(GL_COLOR_MATERIAL); // Allow glColor to affect material properties
  glEnable(GL_NORMALIZE);      // Fix normal vectors after scaling
  glEnable(GLUT_MULTISAMPLE);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable Lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Light Properties
  GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  // Initialize Electrons
  initElectrons();
}

// Animation functions moved to animations.cpp

// --- Main Render Loop ---
void display() {
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Setup Camera (Eye Position moves with posX)
  gluLookAt(posX, posY, zoom, posX, posY, 0.0, 0.0, 1.0, 0.0);

  // Apply Rotation
  glRotatef(angleX, 1.0f, 0.0f, 0.0f);
  glRotatef(angleY, 0.0f, 1.0f, 0.0f);

  glColor3f(0.2f, 0.9f, 0.2f); // Default color (overwritten later)

  // --- 1. Draw Main Blade Parts ---

  // Main Body
  glPushMatrix();
  glColor3fv(colorSilver);
  drawPollygon(mainBlade1, mainBlade2, mainBlade3, mainBlade4, bladeThickness);
  glPopMatrix();

  // Triangular Edge
  glPushMatrix();
  drawTriangle(mainBlade5, mainBlade6, mainBlade7, bladeThickness);
  glPopMatrix();

  // Joint 1
  glPushMatrix();
  drawPollygon(mainBladeJointOne1, mainBladeJointOne2, mainBladeJointOne3,
               mainBladeJointOne4, bladeThickness);
  glPopMatrix();

  // Joint 2
  glPushMatrix();
  drawPollygon(mainBladeJointTwo1, mainBladeJointTwo2, mainBladeJointTwo3,
               mainBladeJointTwo4, bladeThickness);
  glPopMatrix();

  // --- 2. Draw Glowing Edges ---
  // Calculate intersection points for the edge geometry
  float mainBlade1_cpy[3];
  float mainBladeEdgeLeft1_cpy[3];

  findPointAtDistanceToY(mainBladeEdgeLeft2, mainBladeEdgeLeft1,
                         -edgeLen + 0.05f, mainBladeEdgeLeft1_cpy);
  setGlow(true);
  glPushMatrix();
  drawEdge(mainBlade1, mainBladeEdgeLeft1_cpy, mainBladeEdgeLeft2, mainBlade4,
           bladeThickness);
  glPopMatrix();

  glPushMatrix();
  drawEdge(mainBlade1, mainBladeEdgeLeft1_cpy, mainBladeEdgeBottomOrigin,
           mainBlade5, bladeThickness);
  glPopMatrix();

  glPushMatrix();
  drawEdge(mainBlade5, mainBladeEdgeBottomOrigin, mainBladeTriangleEdgeMid,
           mainBlade6, bladeThickness);
  glPopMatrix();

  glPushMatrix();
  drawEdge(mainBlade6, mainBladeTriangleEdgeMid, mainBladeTriangleEdgeEnd,
           mainBlade7, bladeThickness);
  glPopMatrix();

  glPushMatrix();
  drawEdge(mainBlade7, mainBladeTriangleEdgeEnd, mainBladeTriangleBegin,
           mainBlade2, bladeThickness);
  glPopMatrix();
  setGlow(false);

  // --- 3. Draw Barrel ---
  glColor3fv(colorBarrel);
  glPushMatrix();
  glTranslatef(barrel[0], barrel[1], barrel[2]);
  glRotatef(270, 0, 1, 0); // Rotate to point along X-axis

  // Create and draw cylinder
  GLUquadric *quad = gluNewQuadric();
  gluCylinder(quad, 0.08, 0.08, barrelLength, 20, 20);
  gluDeleteQuadric(quad);
  glPopMatrix();

  // --- 4.Draw Gun Body ---
  // 1. Define how much to shrink (The "Blend" factor)
  float blendOffset = 0.001f;

  // 2. Create temporary copies of the 4 corner vertices
  float top1[3], top5[3], top8[3], top4[3];

  // Copy original values
  std::memcpy(top1, gunBodyBarrelCover1, 3 * sizeof(float));
  std::memcpy(top5, gunBodyBarrelCover5, 3 * sizeof(float));
  std::memcpy(top8, gunBodyBarrelCover8, 3 * sizeof(float));
  std::memcpy(top4, gunBodyBarrelCover4, 3 * sizeof(float));

  top1[1] += blendOffset; // Move Left side IN
  top5[1] += blendOffset;
  top8[1] += blendOffset; // Move Right side IN
  top4[1] += blendOffset;

  top1[2] -= blendOffset;
  top4[2] -= blendOffset;

  top5[2] -= gunBodyBarrelCoverThickness - 0.001f;
  top8[2] -= gunBodyBarrelCoverThickness - 0.001f;

  // create gun body barrel cover
  glColor3fv(colorGunMetal);
  glPushMatrix();
  drawPollygon(gunBodyBarrelCover1, gunBodyBarrelCover2, gunBodyBarrelCover3,
               gunBodyBarrelCover4, +gunBodyBarrelCoverThickness);
  drawPollygon(gunBodyBarrelCover5, gunBodyBarrelCover6, gunBodyBarrelCover7,
               gunBodyBarrelCover8, +gunBodyBarrelCoverThickness);
  drawPollygon(top1, top5, top8, top4, gunBodyBarrelCoverThickness, 'y');
  glPopMatrix();

  // Main Gun Body Segments
  float thick = -gunBodyMainThickness + gunBodyBarrelCoverThickness;

  drawPollygon(gunBodyMain2, gunBodyMain3, gunBodyMain4, gunBodyMain1, thick);

  // Segment 2
  drawPollygon(gunBodyMain6, gunBodyMain7, gunBodyMain8, gunBodyMain5, thick);

  // Segment 3
  drawPollygon(gunBodyMain10, gunBodyMain11, gunBodyMain12, gunBodyMain9,
               thick);

  // Segment 4
  drawPollygon(gunBodyMain14, gunBodyMain15, gunBodyMain16, gunBodyMain13,
               thick);

  // grip
  glColor3fv(colorGrip);
  drawPollygon(gunGrip2, gunGrip3, gunGrip4, gunGrip1, thick);

  // stock
  glColor3fv(colorGunMetal);
  drawPollygon(gunStock2, gunStock3, gunStock4, gunStock1, thick);
  // drawRoundedStock(gunStock1, gunStock2, gunStock3, gunStock4, thick);

  // Lower stock
  drawPollygon(gunLowStock2, gunLowStock3, gunLowStock4, gunLowStock1, thick);

  // Handle Assembly
  glColor3fv(colorGrip);
  drawPollygon(gunTopHandle3, thick, gunTopHandle2, thick, gunTopHandle1,
               thick - 0.1f, gunTopHandle4, thick - 0.1f, thick);

  // Spacers
  drawPollygon(gunTopHandleSpacer3, gunTopHandleSpacer2, gunTopHandleSpacer1,
               gunTopHandleSpacer4, thick);
  drawPollygon(gunTopHandleSpacer6, gunTopHandleSpacer7, gunTopHandleSpacer8,
               gunTopHandleSpacer5, thick);

  glPopMatrix();

  // --- REAR BLADE ---
  // 1. Blade Body (Metallic)
  glPushMatrix();
  glColor3fv(colorSilver);
  // Draw rectangularish body: BL -> BR -> TR -> TL
  // Logic: BaseBottom -> TipBottom -> TipTop -> BaseTop
  drawPollygon(rearBlade2, rearBlade3, rearBlade4, rearBlade1, bladeThickness);

  // Draw Spike
  drawTriangle(rearSpikeStart, rearSpikeEnd, rearSpikeTip, bladeThickness);
  glPopMatrix();

  // 2. Rear Glowing Edges (Neon)
  float rearEdge3_cpy[3];
  findPointAtDistanceToY(rearEdge2, rearEdge3, rearEdgeLen, rearEdge3_cpy);
  setGlow(true); // Helper to enable emission
  glPushMatrix();

  // Top Edge of Rear Blade
  // Connects Tip Top (4) back to Base Top (3)
  drawEdge(rearBlade1, rearEdge1, rearEdge3_cpy, rearBlade4, bladeThickness);
  drawEdge(rearBlade4, rearEdge3_cpy, rearEdge2, rearBlade3, bladeThickness);
  // Triangle Spike Edges (Tracing the triangle)
  // 1. Up Slope
  drawEdge(rearSpikeStart, rearSpikeEdge1, rearSpikeEdge2, rearSpikeTip,
           bladeThickness);
  // 2. Down Slope
  drawEdge(rearSpikeTip, rearSpikeEdge2, rearSpikeEdge3, rearSpikeEnd,
           bladeThickness);

  glPopMatrix();
  setGlow(false);

  // --- DRAW ELECTRIC ARCS ALONG EDGES ---
  // --- DRAW ELECTRIC ARCS ALONG EDGES ---
  if (isCharging) {

    // Calculate Tip Position for arcs
    float tipX = barrel[0] - barrelLength - 0.5f;
    float tipPos[] = {tipX, barrel[1], barrel[2]};

    if (ballRadius < MAX_BALL_RADIUS) {
      // --- PHASE 1: COLLECTION ---
      // "Little electric thunders collect to the energy ball"
      // We spawn random arcs from various parts of the gun to the tip

      // --- DRAW ELECTRIC ARCS ALONG EDGES ---
      // REPLACED WITH ELECTRONS
      drawElectrons();

      // --- Draw Sparks (Background Effect) ---
      glDisable(GL_LIGHTING);
      glBegin(GL_POINTS);
      for (int i = 0; i < MAX_SPARKS; i++) {
        if (sparks[i].active) {
          glColor4f(1.0f, 0.5f, 1.0f, sparks[i].life);
          // Jitter spark position slightly for vibration effect
          float jitter = ((float)rand() / RAND_MAX - 0.5f) * 0.05f;
          glVertex3f(sparks[i].x + jitter, sparks[i].y + jitter,
                     sparks[i].z + jitter);
        }
      }
      glEnd();
      glEnable(GL_LIGHTING);
    } else {
      // --- PHASE 2: MAX ENERGY ---
      // "Blade edges glowing by energy spark"
      // We restore the edge bolts here

      drawBolt3D(mainBladeTriangleEdgeEnd, mainBladeTriangleBegin, 0.3f, 0.5f);
      drawBolt3D(mainBladeEdgeLeft1, mainBladeTriangleEdgeEnd, 0.3f, 0.5f);
      drawBolt3D(mainBladeEdgeLeft1, mainBladeEdgeLeft2, 0.3f, 0.5f);
      drawBolt3D(mainBlade5, mainBlade6, 0.2f, 0.5f);
      drawBolt3D(mainBlade6, mainBlade7, 0.2f, 0.5f);
      drawBolt3D(rearBlade4, rearBlade3, 0.3f, 0.5f);
      drawBolt3D(rearBlade1, rearSpikeStart, 0.3f, 0.5f);
      drawBolt3D(rearSpikeEnd, rearBlade3, 0.3f, 0.5f);
      drawBolt3D(rearSpikeStart, rearSpikeTip, 0.15f, 0.5f);
      drawBolt3D(rearSpikeTip, rearSpikeEnd, 0.15f, 0.5f);

      // Also draw electrons (hovering mode)
      drawElectrons();

      // Draw background sparks
      glDisable(GL_LIGHTING);
      glBegin(GL_POINTS);
      for (int i = 0; i < MAX_SPARKS; i++) {
        if (sparks[i].active) {
          glColor4f(1.0f, 0.5f, 1.0f, sparks[i].life);
          float jitter = ((float)rand() / RAND_MAX - 0.5f) * 0.05f;
          glVertex3f(sparks[i].x + jitter, sparks[i].y + jitter,
                     sparks[i].z + jitter);
        }
      }
      glEnd();
      glEnable(GL_LIGHTING);
    }
  } // End of isCharging

  // ALWAYS draw the ball if it has size
  if (ballRadius > 0.0f) {
    // Calculate Barrel Tip Position
    // Barrel Origin: barrel[0], barrel[1], barrel[2]
    // Vector: (-barrelLength, 0, 0) due to 270 deg rotation logic
    float tipX = barrel[0] - barrelLength - 0.5f;
    float tipY = barrel[1];
    float tipZ = barrel[2];

    drawEnergyBall(tipX, tipY, tipZ, ballRadius);
  }

  // Draw Laser Beam if Shooting
  if (isShooting) {
    float tipX = barrel[0] - barrelLength - 0.5f; // Same tip pos
    // Calculate scale factor: 1.0 at start (shootTimer=0.5), 0.05 at end (0.0)
    // Actually user said vanish at end, so 0.0 is fine, but maybe clamp to 0.05
    // min
    float progress = shootTimer / 0.5f;
    if (progress < 0.05f)
      progress = 0.05f; // Prevent completely 0 scale if requested "end 5%"

    drawLaserBeam(tipX, barrel[1], barrel[2], progress);
  }

  glutSwapBuffers();
}

// --- Input Handling ---

// Arrow Keys for Rotation
void specialKey(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_RIGHT:
    angleY += 5.0f;
    break;
  case GLUT_KEY_LEFT:
    angleY -= 5.0f;
    break;
  case GLUT_KEY_UP:
    angleX += 5.0f;
    break;
  case GLUT_KEY_DOWN:
    angleX -= 5.0f;
    break;
  }
  glutPostRedisplay();
}

// Number keys for Panning
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case '4':
    posX += 0.5f;
    break; // Pan Right
  case '6':
    posX -= 0.5f;
    break; // Pan Left
  case '8':
    posY -= 0.5f;
    break; // Pan Up
  case '2':
    posY += 0.5f;
    break; // Pan Down

  case 'r':
  case 'R':
    isCharging = !isCharging; // Toggle the effect
    if (isCharging) {
      // Reset sparks when starting (only if you have sparks)
      for (int i = 0; i < MAX_SPARKS; i++)
        spawnSpark(i);
    }
    for (int i = 0; i < MAX_SPARKS; i++)
      spawnSpark(i);
    break;

  case 's':
  case 'S':
    // Shoot only if charging and fully charged (or close to it)
    if (isCharging && ballRadius >= MAX_BALL_RADIUS * 0.9f) {
      isShooting = true;
      shootTimer = 0.5f; // 0.5 Seconds duration (Quick Shot)
      break;
    }
  }

  glutPostRedisplay();
}

// Mouse for Zooming
void mouse(int button, int state, int x, int y) {
  if (button == 3) {
    zoom -= 0.1f;
    if (zoom < 1.0f)
      zoom = 1.0f;
  } else if (button == 4) {
    zoom += 0.1f;
  }
  glutPostRedisplay();
}

// --- Entry Point ---
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(1920, 1080);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Narumi's Weapon");

  init(); // Initialize OpenGL settings

  // Register Callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(specialKey);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);

  glutTimerFunc(60, update, 0); // Use the new wrapper loop

  glutMainLoop();
  return 0;
}
