#include "animations.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cstdlib> // FIX: Added for rand()
#include <math.h>
#include <vector>


// --- Global Variable Definitions ---
bool isCharging = false;
float chargeTimer = 0.0f;
float ballRadius = 0.0f;
const float MAX_BALL_RADIUS = 0.4f;

bool isShooting = false;
float shootTimer = 0.0f;

struct Spark sparks[MAX_SPARKS];
struct Electron electrons[MAX_ELECTRONS];

// --- Helper to spawn a new spark ---
void spawnSpark(int i) {
  sparks[i].active = true;
  sparks[i].life = 1.0f;

  // Random position along the gun body
  sparks[i].x = ((float)rand() / RAND_MAX) * 7.0f - 2.0f;
  sparks[i].y = ((float)rand() / RAND_MAX) * 1.0f - 0.5f;
  sparks[i].z = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;

  // Random velocity (Slower)
  sparks[i].vx = (((float)rand() / RAND_MAX) - 0.5f) * 0.03f;
  sparks[i].vy = (((float)rand() / RAND_MAX) - 0.5f) * 0.03f;
  sparks[i].vz = (((float)rand() / RAND_MAX) - 0.5f) * 0.03f;
}

// --- Electron Logic ---
void spawnElectron(int i, int mode) {
  electrons[i].active = true;
  electrons[i].mode = mode;
  electrons[i].life = 1.0f;

  // Target Tip Position (approximate based on barrel rotation logic)
  float tipX = barrel[0] - barrelLength - 0.5f;
  float tipY = barrel[1];
  float tipZ = barrel[2];

  if (mode == 0) {
    // COLLECTION MODE: Spawn on gun parts -> Move to Tip
    float *startPos;
    int r = rand() % 5;
    if (r == 0)
      startPos = gunStock1;
    else if (r == 1)
      startPos = gunBodyMain1;
    else if (r == 2)
      startPos = rearBlade4;
    else if (r == 3)
      startPos = gunGrip1;
    else
      startPos = mainBladeEdgeLeft1;

    // Random offset
    electrons[i].x = startPos[0] + ((float)rand() / RAND_MAX - 0.5f) * 1.5f;
    electrons[i].y = startPos[1] + ((float)rand() / RAND_MAX - 0.5f) * 0.8f;
    electrons[i].z = startPos[2] + ((float)rand() / RAND_MAX - 0.5f) * 0.4f;

    // Velocity towards tip
    float dx = tipX - electrons[i].x;
    float dy = tipY - electrons[i].y;
    float dz = tipZ - electrons[i].z;
    float dist = sqrt(dx * dx + dy * dy + dz * dz) + 0.001f;

    float speed = 0.3f;
    electrons[i].vx = (dx / dist) * speed;
    electrons[i].vy = (dy / dist) * speed;
    electrons[i].vz = (dz / dist) * speed;
  } else {
    // MAX MODE: Random movement around gun
    electrons[i].x = ((float)rand() / RAND_MAX) * 12.0f - 8.0f;
    electrons[i].y = ((float)rand() / RAND_MAX) * 3.0f - 1.5f;
    electrons[i].z = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

    // Very slow random velocity
    electrons[i].vx = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;
    electrons[i].vy = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;
    electrons[i].vz = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;

    electrons[i].life = 1.0f + ((float)rand() / RAND_MAX);
  }
}

void initElectrons() {
  for (int i = 0; i < MAX_ELECTRONS; i++) {
    electrons[i].active = false;
  }
}

void updateElectrons() {
  if (!isCharging)
    return;

  int currentMode = (ballRadius < MAX_BALL_RADIUS) ? 0 : 1;

  for (int i = 0; i < MAX_ELECTRONS; i++) {
    if (electrons[i].active) {
      electrons[i].x += electrons[i].vx;
      electrons[i].y += electrons[i].vy;
      electrons[i].z += electrons[i].vz;
      electrons[i].life -= 0.015f;

      if (electrons[i].mode == 0) {
        float tipX = barrel[0] - barrelLength - 0.5f;
        float dist = sqrt(pow(electrons[i].x - tipX, 2) +
                          pow(electrons[i].y - barrel[1], 2) +
                          pow(electrons[i].z - barrel[2], 2));
        if (dist < 0.2f)
          electrons[i].life = 0;
      }

      if (electrons[i].life <= 0.0f) {
        spawnElectron(i, currentMode);
      }
      if (electrons[i].mode != currentMode && electrons[i].life < 0.5f) {
        electrons[i].life = 0;
      }
    } else {
      spawnElectron(i, currentMode);
    }
  }
}

void update(int value) {
  if (isCharging) {
    chargeTimer += 0.05f;

    updateElectrons(); // Update electron particles

    // Update Sparks
    for (int i = 0; i < MAX_SPARKS; i++) {
      if (sparks[i].active) {
        sparks[i].x += sparks[i].vx;
        sparks[i].y += sparks[i].vy;
        sparks[i].z += sparks[i].vz;
        sparks[i].life -= 0.02f; // Slower fade out

        if (sparks[i].life <= 0.0f)
          spawnSpark(i);
      }
    }

    // Grow the energy ball
    if (ballRadius < MAX_BALL_RADIUS) {
      ballRadius += 0.01f;
    }
  } else {
    // Shrink/Reset if not charging AND not shooting
    if (!isShooting && ballRadius > 0.0f) {
      ballRadius -= 0.02f;
      if (ballRadius < 0.0f)
        ballRadius = 0.0f;
    }
  }

  // Handle Shooting Timer
  if (isShooting) {
    shootTimer -= 0.06f; // Approx 60ms decrement

    // Linearly interpolate ball radius from MAX to 0 based on remaining time
    // Duration is 0.5s.
    float progress = shootTimer / 0.5f;
    if (progress < 0.0f)
      progress = 0.0f;

    ballRadius = MAX_BALL_RADIUS * progress;

    if (shootTimer <= 0.0f) {
      isShooting = false;
      isCharging = false; // Stop charging
      ballRadius = 0.0f;  // Deplete energy

      // Clear sparks/electrons visual if needed
      for (int i = 0; i < MAX_ELECTRONS; i++)
        electrons[i].active = false;
    }
  }

  glutPostRedisplay();

  // DELAY INCREASED: 16ms -> 60ms
  glutTimerFunc(60, update, 0);
}

// Helper: Draws a 3D electric arc
void drawBolt3D(float *start, float *end, float spread, float width) {
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  float dx = end[0] - start[0];
  float dy = end[1] - start[1];
  float dz = end[2] - start[2];

  const int segments = 20;
  float pathX[segments + 1];
  float pathY[segments + 1];
  float pathZ[segments + 1];

  // Generate Path
  for (int i = 0; i <= segments; i++) {
    float t = (float)i / segments;
    float base_x = start[0] + dx * t;
    float base_y = start[1] + dy * t;
    float base_z = start[2] + dz * t;

    float scale = (i == 0 || i == segments) ? 0.0f : spread;
    float jitterX = ((float)rand() / RAND_MAX - 0.5f) * scale;
    float jitterY = ((float)rand() / RAND_MAX - 0.5f) * scale;
    float jitterZ = ((float)rand() / RAND_MAX - 0.5f) * (scale * 2.5f);

    pathX[i] = base_x + jitterX;
    pathY[i] = base_y + jitterY;
    pathZ[i] = base_z + jitterZ;
  }

  // --- LAYER 1: OUTER HALO ---
  glLineWidth(width * 12.0f);
  glColor4f(1.0f, 0.2f, 1.0f, 0.6f);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= segments; i++)
    glVertex3f(pathX[i], pathY[i], pathZ[i]);
  glEnd();

  // --- LAYER 2: INNER GLOW ---
  glLineWidth(width * 6.0f);
  glColor4f(1.0f, 0.0f, 0.6f, 1.0f);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= segments; i++)
    glVertex3f(pathX[i], pathY[i], pathZ[i]);
  glEnd();

  // --- LAYER 3: CORE ---
  glLineWidth(width * 2.0f);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= segments; i++)
    glVertex3f(pathX[i], pathY[i], pathZ[i]);
  glEnd();

  // Reset
  glLineWidth(1.0f);
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
}

void drawElectrons() {
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  glEnable(GL_POINT_SMOOTH);
  glPointSize(3.0f);
  glBegin(GL_POINTS);
  for (int i = 0; i < MAX_ELECTRONS; i++) {
    if (electrons[i].active) {
      glColor4f(1.0f, 0.2f, 0.8f, electrons[i].life);
      glVertex3f(electrons[i].x, electrons[i].y, electrons[i].z);
    }
  }
  glEnd();

  glPointSize(1.0f);
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
}

// --- Draw Energy Ball ---
void drawEnergyBall(float x, float y, float z, float radius) {
  if (radius <= 0.01f)
    return;

  glPushMatrix();
  glTranslatef(x, y, z);

  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  // 1. Inner Core
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  glutSolidSphere(radius * 0.5f, 16, 16);

  // 2. Outer Glow
  glColor4f(1.0f, 0.0f, 0.8f, 0.5f);
  glutSolidSphere(radius, 20, 20);

  // 3. Arcs/Sparks radiating from it
  for (int i = 0; i < 5; i++) {
    float theta = ((float)rand() / RAND_MAX) * 2 * M_PI;
    float phi = ((float)rand() / RAND_MAX) * M_PI;
    float r = radius * 1.5f;

    float ex = r * sin(phi) * cos(theta);
    float ey = r * sin(phi) * sin(theta);
    float ez = r * cos(phi);

    float start[] = {0.0f, 0.0f, 0.0f};
    float end[] = {ex, ey, ez};

    drawBolt3D(start, end, 0.1f * radius, 1.0f);
  }

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glPopMatrix();
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glPopMatrix();
}

// --- Draw Laser Beam ---
// Added scale parameter to shrink the beam over time
void drawLaserBeam(float startX, float startY, float startZ,
                   float scaleFactor) {
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  // Laser Length
  float length = 15.0f;

  glPushMatrix();
  glTranslatef(startX, startY, startZ);
  // Align with negative X axis (Barrel direction)
  glRotatef(-90, 0, 1, 0);

  // Core Beam (White) - Thinner base radius (0.05f) * scale
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  GLUquadric *quad = gluNewQuadric();
  gluCylinder(quad, 0.05f * scaleFactor, 0.05f * scaleFactor, length, 10, 1);

  // Outer Glow (Pink/Magenta) - Thinner base radius (0.15f) * scale
  glColor4f(1.0f, 0.0f, 0.8f, 0.4f);
  gluCylinder(quad, 0.15f * scaleFactor, 0.2f * scaleFactor, length, 10, 1);

  // Electric Spirals around the beam
  int segments = 40;
  float spiralRad = 0.2f * scaleFactor; // Thinner spiral rad
  glLineWidth(2.0f);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < segments; i++) {
    float t = (float)i / segments;                         // 0 to 1
    float angle = t * 10.0f * M_PI + (shootTimer * 10.0f); // Rotate over time
    float r = spiralRad * (1.0f + 0.2f * sin(t * 20.0f));  // vary radius

    float z = t * length;
    float x = r * cos(angle);
    float y = r * sin(angle);

    glColor4f(1.0f, 0.2f, 1.0f, 0.8f);
    glVertex3f(x, y, z);
  }
  glEnd();

  // Second Spiral (Offset)
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < segments; i++) {
    float t = (float)i / segments;
    float angle = t * 10.0f * M_PI + (shootTimer * 10.0f) + M_PI;
    float r = spiralRad * (1.0f + 0.2f * cos(t * 20.0f));

    float z = t * length;
    float x = r * cos(angle);
    float y = r * sin(angle);

    glColor4f(0.5f, 0.8f, 1.0f, 0.8f); // Cyan accent
    glVertex3f(x, y, z);
  }
  glEnd();

  gluDeleteQuadric(quad);
  glPopMatrix();

  glLineWidth(1.0f);
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
}
