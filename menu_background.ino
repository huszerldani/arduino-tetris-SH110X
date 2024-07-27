struct Star {
  float x;
  float y;
  float speed;
};

#define NUM_STARS 15
Star stars[NUM_STARS];

void initializeStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(0, 64);
    stars[i].y = random(128, 150);
    stars[i].speed = 0.1 + random(0, 5) / 20.0;
  }
}

void animateStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].y -= stars[i].speed;

    if (stars[i].y < -5) {
      stars[i].y = 128;
      stars[i].x = random(0, 64);
    }
    
    display.drawPixel((int)stars[i].x, (int)stars[i].y, SH110X_WHITE);
    display.display();
  }
}

