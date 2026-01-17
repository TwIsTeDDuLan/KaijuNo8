// --- Window Resizing ---
// Recalculates the perspective projection when window size changes
void reshape(int w, int h) {
  if (h == 0)
    h = 1;
  float ratio = (float)w / h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Perspective view: 45 degree FOV
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/**
 * Function: findPointAtDistance
 * Purpose: Calculates a point on a line defined by P1 and P2, at distance k.
 * Returns: Fills the 'result' array.
 */
void findPointAtDistance(float *Point2, float *Point1, float k, float *result) {
  float dx = Point1[0] - Point2[0];
  float dy = Point1[1] - Point2[1];
  float length = sqrt(dx * dx + dy * dy);

  if (length == 0.0f) {
    result[0] = Point1[0];
    result[1] = Point1[1];
    result[2] = Point1[2];
    return;
  }

  float unitX = dx / length;
  float unitY = dy / length;

  result[0] = Point1[0] - (unitX * k);
  result[1] = Point1[1] - (unitY * k);
  result[2] = Point1[2];
}

/**
 * Function: findPointAtDistanceToY
 * Purpose: Finds a point on the line P1-P2 where the Y-coordinate matches
 * target. Used for: Calculating intersections for the edge glow.
 */
void findPointAtDistanceToY(float *Point2, float *Point1, float k,
                            float *result) {
  float dx = Point1[0] - Point2[0];
  float dy = Point1[1] - Point2[1];
  float m = dy / dx; // Slope

  float y = Point1[1] + k; // Target Y

  if (dx == 0.0f) {
    result[0] = Point1[0];
    result[1] = Point1[1];
    result[2] = Point1[2];
    return;
  }

  float x = (k / m) + Point1[0]; // Calculate X based on slope

  result[0] = x;
  result[1] = y;
  result[2] = Point1[2];
}

/**
 * Function: loadBMP
 * Purpose: Loads a BMP image file and returns an OpenGL texture ID.
 */
GLuint loadBMP(const char *imagepath) {
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize;
  unsigned char *data;

  FILE *file = fopen(imagepath, "rb");
  if (!file) {
    printf("Image could not be opened\n");
    return 0;
  }

  if (fread(header, 1, 54, file) != 54) {
    printf("Not a correct BMP file\n");
    fclose(file);
    return 0;
  }

  if (header[0] != 'B' || header[1] != 'M') {
    printf("Not a correct BMP file\n");
    fclose(file);
    return 0;
  }

  dataPos = *(int *)&(header[0x0A]);
  imageSize = *(int *)&(header[0x22]);
  width = *(int *)&(header[0x12]);
  height = *(int *)&(header[0x16]);

  if (imageSize == 0)
    imageSize = width * height * 3;
  if (dataPos == 0)
    dataPos = 54;

  data = new unsigned char[imageSize];
  fread(data, 1, imageSize, file);
  fclose(file);

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // BGR to RGB swap if necessary (BMP is usually BGR)
  for (unsigned int i = 0; i < imageSize; i += 3) { // BGR -> RGB
    unsigned char t = data[i];
    data[i] = data[i + 2];
    data[i + 2] = t;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  delete[] data;
  return textureID;
}
