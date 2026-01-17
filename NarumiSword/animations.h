#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <vector>


// --- Constants ---
extern const float MAX_BALL_RADIUS;
const int MAX_ELECTRONS = 50;
const int MAX_SPARKS = 100;

// --- Structs ---
struct Spark {
  float x, y, z;
  float vx, vy, vz;
  float life;
  bool active;
};

struct Electron {
  float x, y, z;
  float vx, vy, vz;
  float life;
  bool active;
  int mode; // 0 = Collection (to tip), 1 = Random Hover
};

// --- Global Variables (Animation State) ---
extern bool isCharging;
extern float chargeTimer;
extern float ballRadius;
extern bool isShooting;
extern float shootTimer;

extern Electron electrons[MAX_ELECTRONS];
extern Spark sparks[MAX_SPARKS];

// --- External Geometry References (Defined in main.cpp) ---
extern float barrel[];
extern float barrelLength;
extern float gunStock1[];
extern float gunBodyMain1[];
extern float rearBlade4[];
extern float gunGrip1[];
extern float mainBladeEdgeLeft1[];

// --- Function Prototypes ---
void spawnSpark(int i);
void initElectrons();
void spawnElectron(int i, int mode);
void updateElectrons();
void update(int value);

void drawElectrons();
void drawBolt3D(float *start, float *end, float spread, float width);
void drawEnergyBall(float x, float y, float z, float radius);
void drawLaserBeam(float startX, float startY, float startZ, float scaleFactor);

#endif
