
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define I2C_ADDRESS 0x3c
#define WIDTH 64    // OLED display width, in pixels
#define HEIGHT 128  // OLED display height, in pixels

// Game configurations
const short MARGIN_TOP = 19;
const short MARGIN_LEFT = 3;
const short SIZE = 5;
const short TYPES = 6;

const short BUTTON_LEFT_PIN = 10;
const short BUTTON_RIGHT_PIN = 20;
const short BUTTON_UP_PIN = 21;
const short BUTTON_DOWN_PIN = 7;

const short VIBRATOR_PIN = 3;

// Game pieces
static const unsigned char PROGMEM logo[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xf0, 
	0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xf8, 
	0xff, 0x81, 0xf0, 0xf1, 0xe0, 0xf0, 0xf1, 0xf8, 0xff, 0x00, 0x70, 0xe1, 0xc0, 0x70, 0xe1, 0xf8, 
	0xff, 0x00, 0x30, 0xe1, 0x80, 0x30, 0xe1, 0xfc, 0xff, 0x00, 0x30, 0x61, 0x80, 0x30, 0x61, 0xfc, 
	0xff, 0x0c, 0x10, 0x61, 0x84, 0x10, 0x61, 0xfe, 0xff, 0x0e, 0x10, 0x61, 0x06, 0x10, 0x61, 0xfe, 
	0xff, 0x0e, 0x10, 0x61, 0x06, 0x10, 0x61, 0xff, 0xff, 0x0e, 0x10, 0x21, 0x06, 0x10, 0x21, 0xff, 
	0xff, 0x0e, 0x10, 0x21, 0x87, 0xf0, 0x21, 0xff, 0xff, 0x0e, 0x10, 0x21, 0x83, 0xf0, 0x21, 0xff, 
	0xff, 0x0e, 0x10, 0x21, 0x83, 0xf0, 0x21, 0xff, 0xff, 0x0e, 0x10, 0x21, 0xc1, 0xf0, 0x21, 0xff, 
	0xff, 0x0c, 0x10, 0x01, 0xc0, 0xf0, 0x01, 0xff, 0xff, 0x00, 0x30, 0x01, 0xe0, 0xf0, 0x01, 0xff, 
	0xff, 0x00, 0x30, 0x81, 0xf0, 0x70, 0x81, 0xff, 0xff, 0x00, 0x70, 0x81, 0xf0, 0x30, 0x81, 0xff, 
	0xff, 0x00, 0xf0, 0x81, 0xf8, 0x30, 0x81, 0xff, 0xff, 0x0f, 0xf0, 0x81, 0xfc, 0x30, 0x81, 0xff, 
	0xff, 0x0f, 0xf0, 0x81, 0x8c, 0x10, 0x81, 0xff, 0xff, 0x0f, 0xf0, 0xc1, 0x06, 0x10, 0xc1, 0xff, 
	0xff, 0x0f, 0xf0, 0xc1, 0x0e, 0x10, 0xc1, 0xff, 0xff, 0x0f, 0xf0, 0xc1, 0x06, 0x10, 0xc1, 0xff, 
	0xff, 0x0f, 0xf0, 0xc1, 0x84, 0x10, 0xc1, 0xff, 0xff, 0x0f, 0xf0, 0xc1, 0x80, 0x30, 0xc1, 0xff, 
	0xff, 0x0f, 0xf0, 0xe1, 0x80, 0x30, 0xe1, 0xff, 0xff, 0x0f, 0xf0, 0xe1, 0xc0, 0x70, 0xe1, 0xff, 
	0xff, 0xaf, 0xf9, 0xf5, 0xe0, 0xf9, 0xf5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 
	0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xfc, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3f, 
	0xfc, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3f, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff
};

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

// Global variables
word currentType, nextType, rotation;
short pieceX, pieceY;
short piece[2][4];

int interval = 600, score;
long timer, delayer;

bool gameOver = false;

bool grid[10][18];
bool buttonLeftPressed, buttonRightPressed, buttonUpPressed;

Adafruit_SH1106G display(128, 64, &Wire, -1);

//-------------------
// --- SETUP ------
//-------------------
void setup() {
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(VIBRATOR_PIN, OUTPUT);

  display.begin(I2C_ADDRESS, true);
  display.setRotation(3);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 64, 128, SH110X_WHITE);
  display.display();

  //delay(100);

  display.clearDisplay();
  drawLayout();
  display.display();

  randomSeed(analogRead(0));
  nextType = random(TYPES);
  generate();
  timer = millis();
}

//-------------------
// --- LOOP ------
//-------------------
void loop() {
  if (!gameOver) {
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
  }

  checkGameOver();
  handleGameOver();
}

void restartGame() {
  display.invertDisplay(false);
  delay(200);
  for (short y = 17; y >= 0; y--) {
    for (short x = 0; x < 10; x++) {
      grid[x][y] = 0;
    }
  }

  score = 0;
  gameOver = false;
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
    interval = 600;
  }
}

void handleRightButton() {
  if (!digitalRead(BUTTON_RIGHT_PIN)) {
    if (!buttonRightPressed) {
      movePieceRight();
      buttonRightPressed = true;
    }
  } else {
    buttonRightPressed = false;
  }
}

void handleLeftButton() {
  if (!digitalRead(BUTTON_LEFT_PIN)) {
    if (!buttonLeftPressed) {
      movePieceLeft();
      buttonLeftPressed = true;
    }
  } else {
    buttonLeftPressed = false;
  }
}

void rotatePiece() {
  int maxRotation = getMaxRotation(currentType);

  // Rotate clockwise
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

void vibrate(int delayInterval = 100) {
  digitalWrite(VIBRATOR_PIN, HIGH);
  delay(delayInterval);
  digitalWrite(VIBRATOR_PIN, LOW);
}

// Helper functions
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
  display.invertDisplay(true);
  vibrate();
  display.invertDisplay(false);
  vibrate();

  // Increase the score by 10 points for breaking a line
  score += 10;
}

void handleGameOver() {
  if (!gameOver) {
    return;
  }

  if (
    !digitalRead(BUTTON_UP_PIN) ||
    !digitalRead(BUTTON_DOWN_PIN) ||
    !digitalRead(BUTTON_LEFT_PIN) ||
    !digitalRead(BUTTON_RIGHT_PIN)
  ) {
    restartGame();
  }
}

void displayGameOver() {
  display.fillRect(3, 50, 58, 40, SH110X_WHITE);
  display.drawRect(5, 52, 54, 36, SH110X_BLACK);

  display.setTextSize(2);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(9, 55);
  display.print("GAME");
  display.setCursor(9, 70);
  display.print("OVER");

  display.fillRoundRect(11, 100, 40, 14, 2, SH110X_BLACK);
  display.drawRoundRect(11, 100, 40, 14, 2, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(17, 103);
  display.print("retry");

  display.display();
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
        display.fillRect(MARGIN_LEFT + (SIZE + 1) * x, MARGIN_TOP + (SIZE + 1) * y, SIZE, SIZE, SH110X_WHITE);
      }
    }
  }
}

void checkGameOver() {
  if (gameOver) {
    return;
  }

  for (int x = 0; x < 10; x++) {
    if (grid[x][2]) {
      delay(500);
      display.invertDisplay(true);
      vibrate(100);
      delay(50);
      display.invertDisplay(false);
      vibrate(100);
      delay(50);
      display.invertDisplay(true);
      vibrate(100);
      delay(50);
      display.invertDisplay(false);
      delay(150);
      display.invertDisplay(true);
      delay(150);
      display.invertDisplay(false);
      delay(150);
      display.invertDisplay(true);
      delay(150);

      displayGameOver();
      gameOver = true;
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
    display.fillRect(MARGIN_LEFT + (SIZE + 1) * (x + piece[0][i]), MARGIN_TOP + (SIZE + 1) * (y + piece[1][i]), SIZE, SIZE, SH110X_WHITE);
  }
}

void drawNextPiece() {
  short nPiece[2][4];
  copyPiece(nPiece, nextType, 0);
  for (short i = 0; i < 4; i++) {
    display.fillRect(50 + 3 * nPiece[0][i], 4 + 3 * nPiece[1][i], 2, 2, SH110X_WHITE);
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

boolean canRotate(short rotation) {
  short piece[2][4];
  copyPiece(piece, currentType, rotation);
  return !nextHorizontalCollision(piece, 0);
}

void drawLayout() {
  display.drawLine(0, 15, WIDTH, 15, SH110X_WHITE);
  display.drawRect(0, 0, WIDTH, HEIGHT, SH110X_WHITE);
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
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SH110X_WHITE);  // Draw SH110X_WHITE text
  display.setCursor(x, y);             // Start at top-left corner

  for (short i = 0; i < length; i++) {
    display.write(text[i]);
  }
}
