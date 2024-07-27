// MENU Variables
enum MenuState {
  START,
  LAN,
  BRIGHTNESS,
  MENU_COUNT  // Add this to keep track of the number of menu items
};

// Array to hold the menu items
MenuState menuItems[] = {START, LAN, BRIGHTNESS};
int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

int activeMenuIndex = 0;

// Debounce variables
unsigned long lastButtonUpTime = 0;
unsigned long lastButtonDownTime = 0;
unsigned long lastButtonSelectTime = 0;
const unsigned long debounceDelay = 200;

void initializeStars();
void animateStars();

void setupMenu() {
  initializeStars();
}

void loopMenu() {
  // Animate stars
  animateStars();

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
    
    // Move up the menu, wrap around to the last item if needed
    activeMenuIndex = (activeMenuIndex - 1 + menuItemCount) % menuItemCount;

    drawMenu();
  }

  // Handle DOWN button
  if (digitalRead(BUTTON_DOWN_PIN) == LOW && (currentTime - lastButtonDownTime > debounceDelay)) {
    lastButtonDownTime = currentTime;
    
    // Move down the menu, wrap around to the first item if needed
    activeMenuIndex = (activeMenuIndex + 1) % menuItemCount;

    drawMenu();
  }

  // Handle LEFT button
  if ((digitalRead(BUTTON_LEFT_PIN) == LOW) && (currentTime - lastButtonSelectTime > debounceDelay)) {
    lastButtonSelectTime = currentTime;

    leftButtonPushed();

    vibrate(100);
  }

  // Handle RIGHT button
  if ((digitalRead(BUTTON_RIGHT_PIN) == LOW) && (currentTime - lastButtonSelectTime > debounceDelay)) {
    lastButtonSelectTime = currentTime;

    rightButtonPushed()

    vibrate(100);
  }
}

void leftButtonPushed() {
  if (menuItems[activeMenuIndex] == BRIGHTNESS) {
    display.setContrast(0);
    return;
  }

  if (menuItems[activeMenuIndex] == LAN) {
    currentScreen = SCENE_LAN_MENU;
    return;
  }
  
  if (menuItems[activeMenuIndex] == START) {
    currentScreen = SCENE_GAME;
    return;
  }
}

void rightButtonPushed() {
  if (menuItems[activeMenuIndex] == BRIGHTNESS) {
    display.setContrast(255);
    return;
  }

  if (menuItems[activeMenuIndex] == LAN) {
    currentScreen = SCENE_LAN_MENU;
    return;
  }
  
  if (menuItems[activeMenuIndex] == START) {
    currentScreen = SCENE_GAME;
    return;
  }
}

void drawMenu() {
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 64, 128, WHITE);

  // Draw START button
  drawButton(10, 60, "START", menuItems[activeMenuIndex] == START);

  // Draw LAN button
  drawButton(10, 80, " LAN", menuItems[activeMenuIndex] == LAN);

  // Draw BRIGHTNESS button
  drawButton(10, 100, "LIGHT", menuItems[activeMenuIndex] == BRIGHTNESS);

  display.display();
}
