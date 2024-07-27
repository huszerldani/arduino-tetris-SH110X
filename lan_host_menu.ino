// MENU Variables
enum MenuState {
  START,
  BACK
};

// Array to hold the menu items
MenuState menuItems[] = {
  START,
  BACK
};

int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);

int activeMenuIndex = 0;

// Debounce variables
unsigned long lastButtonUpTime = 0;
unsigned long lastButtonDownTime = 0;
unsigned long lastButtonSelectTime = 0;
const unsigned long debounceDelay = 200;


void setupLanHostMenu() {

}

void loopLanHostMenu() {
  // Handle pulsing effect
  unsigned long currentTime = millis();

  if (currentTime - lastPulseTime > pulseInterval) {
    lastPulseTime = currentTime;
    pulseWidth += pulseUp ? 1 : -1;

    if (pulseWidth > 2 || pulseWidth < 1) {
      pulseUp = !pulseUp;
    }

    drawLanHostMenu();
  }

  // Handle UP button
  if (digitalRead(BUTTON_UP_PIN) == LOW && (currentTime - lastButtonUpTime > debounceDelay)) {
    lastButtonUpTime = currentTime;
    
    // Move up the menu, wrap around to the last item if needed
    activeMenuIndex = (activeMenuIndex - 1 + menuItemCount) % menuItemCount;

    drawLanHostMenu();
  }

  // Handle DOWN button
  if (digitalRead(BUTTON_DOWN_PIN) == LOW && (currentTime - lastButtonDownTime > debounceDelay)) {
    lastButtonDownTime = currentTime;
    
    // Move down the menu, wrap around to the first item if needed
    activeMenuIndex = (activeMenuIndex + 1) % menuItemCount;

    drawLanHostMenu();
  }

  // Handle LEFT button
  if ((digitalRead(BUTTON_LEFT_PIN) == LOW) && (currentTime - lastButtonSelectTime > debounceDelay)) {
    lastButtonSelectTime = currentTime;

    selectButtonPushed();

    vibrate(100);
  }

  // Handle RIGHT button
  if ((digitalRead(BUTTON_RIGHT_PIN) == LOW) && (currentTime - lastButtonSelectTime > debounceDelay)) {
    lastButtonSelectTime = currentTime;

    selectButtonPushed()

    vibrate(100);
  }
}

void selectButtonPushed() {
  if (menuItems[activeMenuIndex] == START) {

    return;
  }

  if (menuItems[activeMenuIndex] == JOIN) {
    
    return;
  }
  
  if (menuItems[activeMenuIndex] == BACK) {
    
    return;
  }
}

void drawLanMenu() {
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 64, 128, WHITE);

  // Draw HOST button
  drawButton(10, 60, "HOST", menuItems[activeMenuIndex] == HOST);

  // Draw JOIN button
  drawButton(10, 80, "JOIN", menuItems[activeMenuIndex] == JOIN);

  // Draw BACK button
  drawButton(10, 100, "BACK", menuItems[activeMenuIndex] == BACK);

  display.display();
}