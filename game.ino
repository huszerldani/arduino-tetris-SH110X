// Game configurations
#include "constants.h"
#include "globals.h"

/*  - X X
    X X -  */
const char pieces_S_l[2][2][4] = { 
  { { 0, 0, 1, 1 }, { 0, 1, 1, 2 } },
  { { 0, 1, 1, 2 }, { 1, 1, 0, 0 } } 
};

/*  X X -
    - X X  */
const char pieces_S_r[2][2][4] = { 
  { { 1, 1, 0, 0 }, { 0, 1, 1, 2 } },
  { { 0, 1, 1, 2 }, { 0, 0, 1, 1 } } 
};

/*  - X -
    - X -
    X X - */
const char pieces_L_l[4][2][4] = { 
  { { 0, 0, 0, 1 }, { 0, 1, 2, 2 } },
  { { 0, 1, 2, 2 }, { 1, 1, 1, 0 } },
  { { 0, 1, 1, 1 }, { 0, 0, 1, 2 } },
  { { 0, 0, 1, 2 }, { 1, 0, 0, 0 } } 
};

/*  X X
    X X */
const char pieces_Sq[1][2][4] = { 
  { { 0, 1, 0, 1 }, { 0, 0, 1, 1 } } 
};

/*  - X -
    X X X */
const char pieces_T[4][2][4] = { 
  { { 0, 0, 1, 0 }, { 0, 1, 1, 2 } },
  { { 0, 1, 1, 2 }, { 1, 0, 1, 1 } },
  { { 1, 0, 1, 1 }, { 0, 1, 1, 2 } },
  { { 0, 1, 1, 2 }, { 0, 0, 1, 0 } } 
};

/*  X -
    X -
    X -
    X - */
const char pieces_l[2][2][4] = { 
  { { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
  { { 0, 0, 0, 0 }, { 0, 1, 2, 3 } } 
};

word currentType, nextType, rotation;
short pieceX, pieceY;
short piece[2][4];

int interval = 400, score;
long timer, delayer;

bool grid[10][18];

// DEBOUNCE
const unsigned long DEBOUNCE_DELAY = 100;
const unsigned long INITIAL_SLIDE_DELAY = 200;
const unsigned long CONTINUOUS_SLIDE_DELAY = 50;

// PUSH BUTTONS
bool buttonLeftState, buttonRightState, buttonUpState, buttonUpPressed;
unsigned long lastLeftButtonPress = 0;
unsigned long lastRightButtonPress = 0;
bool leftButtonState = HIGH;
bool rightButtonState = HIGH;
bool leftButtonHeld = false;
bool rightButtonHeld = false;
unsigned long lastLeftMoveTime = 0;
unsigned long lastRightMoveTime = 0;

// SHAKE
int shakeDuration = 10;
int shakeIntensity = 2;
int shakeTimer = 0;
bool shaking = false;
int shakeOffsetX = 0;
int shakeOffsetY = 0;

void loopGame() {
  loopShake();

  if (millis() - timer > interval) {
    checkLines();
    refresh();

    if (nextCollision()) {
      for (short i = 0; i < 4; i++) {
        grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
      }
      generate();
    } else {
      pieceY++;
    }

    timer = millis();
  }

  handleLeftButton();
  handleRightButton();
  handleUpButton();
  handleDownButton();

  checkGameOver();
}

//-------------------
// --- BUTTONS ------
//-------------------

void handleUpButton() {
  if (!digitalRead(BUTTON_UP_PIN)) {
    if (!buttonUpPressed) {
      rotatePiece();
    }

    buttonUpPressed = true;
    delayer = millis();
  } else if(millis() - delayer > 50) {
    buttonUpPressed = false;
  }
}

void handleDownButton() {
  if (!digitalRead(BUTTON_DOWN_PIN)) {
    interval = 10;
  } else {
    interval = 400;
  }
}

void handleRightButton() {
  bool currentRightButtonState = digitalRead(BUTTON_RIGHT_PIN);

  if (currentRightButtonState != rightButtonState) {
    if (currentRightButtonState == LOW) { // Button is pressed
      if (millis() - lastRightButtonPress >= DEBOUNCE_DELAY) {
        // Move piece right on initial press
        movePieceRight();
        lastRightButtonPress = millis();
        lastRightMoveTime = millis();
        rightButtonHeld = true; // Start the hold state
      }
    } else {
      // Button released
      rightButtonHeld = false; // Reset hold state
    }

    rightButtonState = currentRightButtonState;
  }

  if (rightButtonHeld) {
    if (millis() - lastRightButtonPress >= INITIAL_SLIDE_DELAY &&
        millis() - lastRightMoveTime >= CONTINUOUS_SLIDE_DELAY) {
      movePieceRight();
      lastRightMoveTime = millis();
    }
  }
}

void handleLeftButton() {
  bool currentLeftButtonState = digitalRead(BUTTON_LEFT_PIN);

  if (currentLeftButtonState != leftButtonState) {
    if (currentLeftButtonState == LOW) { // Button is pressed
      if (millis() - lastLeftButtonPress >= DEBOUNCE_DELAY) {
        // Move piece left on initial press
        movePieceLeft();
        lastLeftButtonPress = millis();
        lastLeftMoveTime = millis();
        leftButtonHeld = true; // Start the hold state
      }
    } else {
      // Button released
      leftButtonHeld = false; // Reset hold state
    }

    leftButtonState = currentLeftButtonState;
  }

  if (leftButtonHeld) {
    if (millis() - lastLeftButtonPress >= INITIAL_SLIDE_DELAY &&
        millis() - lastLeftMoveTime >= CONTINUOUS_SLIDE_DELAY) {
      movePieceLeft();
      lastLeftMoveTime = millis();
    }
  }
}

void rotatePiece() {
  int maxRotation = getMaxRotation(currentType);

  if (rotation == 0 && canRotate(maxRotation - 1)) {
    rotation = maxRotation - 1;
  } else if (canRotate(rotation - 1)) {
    rotation--;
  }

  copyPiece(piece, currentType, rotation);
  refresh();
}

void movePieceLeft() {
  if (!nextHorizontalCollision(piece, -1)) {
    pieceX--;
    refresh();
  }
}

void movePieceRight() {
  if (!nextHorizontalCollision(piece, 1)) {
    pieceX++;
    refresh();
  }
}

void checkLines() {
  boolean full;
  for (short y = 17; y >= 0; y--) {
    full = true;
    for (short x = 0; x < 10; x++) {
      full = full && grid[x][y];
    }
    if (full) {
      breakLine(y);
      y++;
    }
  }
}

void breakLine(short line) {
  triggerShake();

  // Shift each row above the completed line down by one row
  for (short y = line; y >= 0; y--) {
    for (short x = 0; x < 10; x++) {
      // Move the block from the row above to the current row
      grid[x][y] = grid[x][y - 1];
    }
  }
  
  // Clear the topmost row after shifting all rows down
  for (short x = 0; x < 10; x++) {
    grid[x][0] = 0; // Set each block in the top row to empty
  }

  // Briefly invert the display to provide visual feedback for line break
  invertDisplay(100);
  vibrate();

  // Increase the score by 10 points for breaking a line
  score += 10;
}

void refresh() {
  display.clearDisplay();
  drawLayout();
  drawGrid();
  drawPiece(currentType, 0, pieceX, pieceY);
  display.display();
}

void drawGrid() {
  for (short x = 0; x < 10; x++) {
    for (short y = 0; y < 18; y++) {
      if (grid[x][y]) {
        display.fillRect((MARGIN_LEFT + (SIZE + 1) * x) + shakeOffsetX, (MARGIN_TOP + (SIZE + 1) * y) + shakeOffsetY, SIZE, SIZE, WHITE);
      }
    }
  }
}

void triggerShake() {
  shaking = true;
  shakeTimer = shakeDuration;
}

void loopShake() {
  if (!shaking) {
    return;
  }

  shakeOffsetX = random(-shakeIntensity, shakeIntensity + 1);
  shakeOffsetY = random(-shakeIntensity, shakeIntensity + 1);
  shakeTimer--;

  if (shakeTimer <= 0) {
    shaking = false;
    shakeOffsetX = 0;
    shakeOffsetY = 0;
  }

  refresh();
}

void checkGameOver() {
  for (int x = 0; x < 10; x++) {
    if (grid[x][2]) {
      invertDisplay(100);
      vibrate(100);

      displayGameOver();
      currentScreen = SCENE_GAME_OVER;
    }
  }
}

bool nextHorizontalCollision(short piece[2][4], int amount) {
  for (short i = 0; i < 4; i++) {
    short newX = pieceX + piece[0][i] + amount;
    if (newX > 9 || newX < 0 || grid[newX][pieceY + piece[1][i]]) {
      return true;
    }
  }

  return false;
}

boolean nextCollision() {
  for (short i = 0; i < 4; i++) {
    short y = pieceY + piece[1][i] + 1;
    short x = pieceX + piece[0][i];
    if (y > 17 || grid[x][y]) {
      return true;
    }
  }

  return false;
}

void generate() {
  currentType = nextType;
  nextType = random(TYPES);

  if (currentType == nextType) {
    nextType = random(TYPES);
  }

  if (currentType != 5) {
    pieceX = random(9);
  } else {
    pieceX = random(7);
  }

  pieceY = 0;
  rotation = 0;
  copyPiece(piece, currentType, rotation);
}

void drawPiece(short type, short rotation, short x, short y) {
  for (short i = 0; i < 4; i++) {
    display.fillRect(
      (MARGIN_LEFT + (SIZE + 1) * (x + piece[0][i])) + shakeOffsetX,
      (MARGIN_TOP + (SIZE + 1) * (y + piece[1][i])) + shakeOffsetY,
      SIZE,
      SIZE,
      WHITE
    );
  }
}

void drawNextPiece() {
  short nPiece[2][4];
  copyPiece(nPiece, nextType, 0);
  for (short i = 0; i < 4; i++) {
    display.fillRect(50 + 3 * nPiece[0][i], 4 + 3 * nPiece[1][i], 2, 2, WHITE);
  }
}

void copyPiece(short piece[2][4], short type, short rotation) {
  switch (type) {
    case 0:  // L_l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pieces_L_l[rotation][0][i];
        piece[1][i] = pieces_L_l[rotation][1][i];
      }
      break;
    case 1:  // S_l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pieces_S_l[rotation][0][i];
        piece[1][i] = pieces_S_l[rotation][1][i];
      }
      break;
    case 2:  // S_r
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pieces_S_r[rotation][0][i];
        piece[1][i] = pieces_S_r[rotation][1][i];
      }
      break;
    case 3:  // Sq
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pieces_Sq[0][0][i];
        piece[1][i] = pieces_Sq[0][1][i];
      }
      break;
    case 4:  // T
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pieces_T[rotation][0][i];
        piece[1][i] = pieces_T[rotation][1][i];
      }
      break;
    case 5:  // l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pieces_l[rotation][0][i];
        piece[1][i] = pieces_l[rotation][1][i];
      }
      break;
  }
}

short getMaxRotation(short type) {
  if (type == 1 || type == 2 || type == 5) {
    return 2;
  } else if (type == 0 || type == 4) {
    return 4;
  } else if (type == 3) {
    return 1;
  } else {
    return 0;
  }
}

bool canRotate(short rotation) {
  short piece[2][4];
  copyPiece(piece, currentType, rotation);
  return !nextHorizontalCollision(piece, 0);
}

void drawLayout() {
  display.drawLine(0, 15, WIDTH, 15, WHITE);
  display.drawRect(0, 0, WIDTH, HEIGHT, WHITE);
  drawNextPiece();
  char text[6];
  itoa(score, text, 10);
  drawText(text, getNumberLength(score), 7, 4);
}

short getNumberLength(int n) {
  short counter = 1;
  
  while (n >= 10) {
    n /= 10;
    counter++;
  }

  return counter;
}

void drawText(char text[], short length, int x, int y) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(x, y);

  for (short i = 0; i < length; i++) {
    display.write(text[i]);
  }
}

void restartGame() {
  display.invertDisplay(false);
  delay(100);

  for (short y = 17; y >= 0; y--) {
    for (short x = 0; x < 10; x++) {
      grid[x][y] = 0;
    }
  }

  score = 0;
  currentScreen = SCENE_GAME;
}