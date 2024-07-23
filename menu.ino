// MENU Variables
enum MenuState {START, BRIGHTNESS};
MenuState activeMenu = START;

// Pulsing effect variables
bool pulseUp = true;
int pulseWidth = 1;
unsigned long lastPulseTime = 0;
const int pulseInterval = 100; // Pulse interval in milliseconds

// Debounce variables
unsigned long lastButtonUpTime = 0;
unsigned long lastButtonDownTime = 0;
unsigned long lastButtonSelectTime = 0;
const unsigned long debounceDelay = 200;

void loopMenu() {
  // Handle pulsing effect
  unsigned long currentTime = millis();

  if (currentTime - lastPulseTime > pulseInterval) {
    lastPulseTime = currentTime;
    pulseWidth += pulseUp ? 1 : -1;

    if (pulseWidth > 2 || pulseWidth < 1) {
      pulseUp = !pulseUp;
    }

    drawMenu();
  }

  // Handle UP button
  if (digitalRead(BUTTON_UP_PIN) == LOW && (currentTime - lastButtonUpTime > debounceDelay)) {
    lastButtonUpTime = currentTime;
    
    if (activeMenu == BRIGHTNESS) {
      activeMenu = START;
    }

    drawMenu();
  }

  // Handle DOWN button
  if (digitalRead(BUTTON_DOWN_PIN) == LOW && (currentTime - lastButtonDownTime > debounceDelay)) {
    lastButtonDownTime = currentTime;
    
    if (activeMenu == START) {
      activeMenu = BRIGHTNESS;
    }

    drawMenu();
  }

  // Handle LEFT button
  if ((digitalRead(BUTTON_LEFT_PIN) == LOW)
    && (currentTime - lastButtonSelectTime > debounceDelay)
  ) {
    lastButtonSelectTime = currentTime;

    if (activeMenu == BRIGHTNESS) {
      display.setContrast(0);
    } else if (activeMenu == START) {
      currentScreen = SCENE_GAME;
    }

    vibrate(100);
  }

  // Handle RIGHT button
  if ((digitalRead(BUTTON_RIGHT_PIN) == LOW)
    && (currentTime - lastButtonSelectTime > debounceDelay)
  ) {
    lastButtonSelectTime = currentTime;

    if (activeMenu == BRIGHTNESS) {
      display.setContrast(255);
    } else if (activeMenu == START) {
      currentScreen = SCENE_GAME;
    }

    vibrate(100);
  }
}

void drawMenu() {
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 64, 128, SH110X_WHITE);

  // Draw START button
  if (activeMenu == START) {
    drawButton(10, 80, "START", true);
  } else {
    drawButton(10, 80, "START", false);
  }

  // Draw BRIGHTNESS button
  if (activeMenu == BRIGHTNESS) {
    drawButton(10, 100, "LIGHT", true);
  } else {
    drawButton(10, 100, "LIGHT", false);
  }

  display.display();
}

void drawButton(int x, int y, const char* label, bool active) {
  if (active) {
    display.drawRoundRect(x - pulseWidth, y - pulseWidth, 44 + pulseWidth * 2, 12 + pulseWidth * 2, 4, SH110X_WHITE);
  } else {
    display.drawRoundRect(x, y, 44, 12, 3, SH110X_WHITE);
  }

  display.setCursor(x + 8, y + 2);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(0.5F);
  display.print(label);
}