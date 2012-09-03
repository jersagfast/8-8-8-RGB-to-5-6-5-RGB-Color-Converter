//***THIS IS THE NEW VERSION THAT WORKS WITH THE NEW LIBRARIES!!!***
// TFTLCD.h and TouchScreen.h are from http://adafruit.com where you can also purchase a really nice 2.8" TFT with touchscreen :)
// 2012 Jeremy Saglimbeni - http://thecustomgeek.com
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <EEPROM.h>

#if not defined USE_ADAFRUIT_SHIELD_PINOUT 
 #error "For use with the shield, make sure to #define USE_ADAFRUIT_SHIELD_PINOUT in the TFTLCD.h library file"
#endif

// These are the pins for the shield!
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0 
// Color definitions - in 5:6:5
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF
#define TEST            0x1BF5
#define JJCOLOR         0x1CB6
#define JJORNG          0xFD03
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, 0);
int i = 0;
int backlight = 3;
int blv = 255;
long r;
long g;
long b;
char red[10];
char green[10];
char blue[10];
char output[10];
unsigned long answer;
void setup(void) {
  pinMode(backlight, OUTPUT);
  Serial.begin(9600);
 tft.reset();
  
  uint16_t identifier = tft.readRegister(0x0);
  if (identifier == 0x9325) {
    Serial.println("Found ILI9325");
  } else if (identifier == 0x9328) {
    Serial.println("Found ILI9328");
  } else if (identifier == 0x7575) {
    Serial.println("Found HX8347G");
  } else {
    Serial.print("Unknown driver chip ");
    Serial.println(identifier, HEX);
    while (1);
  }

  tft.begin(identifier); 
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.fillRect(71, 70, 50, 100, JJCOLOR);
  tft.fillRect(134, 70, 50, 100, JJCOLOR);
  tft.fillRect(197, 70, 50, 100, JJCOLOR);
  tft.drawRect(46, 45, 228, 150, WHITE);
  for(i = 0 ; i <= blv; i+=1) { 
    analogWrite(backlight, i);
    delay(2);
  }
  delay(250);
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.setCursor(85, 100);
  tft.print("J");
  delay(250);
  tft.setCursor(147, 100);
  tft.print("O");
  delay(250);
  tft.setCursor(210, 100);
  tft.print("S");
  delay(500);
  tft.setTextSize(1);
  tft.setCursor(84, 210);
  tft.print("Jeremy Saglimbeni - 2012");
  tft.setCursor(123, 230);
  tft.print("jandjpro.com");
  delay(500);
  tft.setCursor(108, 20);
  tft.print("Thanks ktownsend!!!");
  delay(1500);
  tft.fillScreen(BLACK);
  drawscreen();
  showred();
  showgrn();
  showblu();
  tft.fillRect(0, 0, 320, 10, JJCOLOR);
  tft.setTextSize(2);
  tft.setCursor(247, 223);
  tft.print("0x");
  tft.drawRect(244, 220, 76, 20, WHITE);
  tft.setTextSize(1);
  tft.setCursor(252, 207);
  tft.print("HEX Output");
  tft.setCursor(60, 207);
  tft.print("Preview Swatch");
  tft.setCursor(60, 217);
  tft.print("(Press to reset)");
  showhex();
  tft.setTextSize(1);
  tft.setCursor(1, 1);
  tft.print("8:8:8 RGB to 5:6:5 HEX Color Converter  -  jersagfast");
}
#define MINPRESSURE 10
#define MAXPRESSURE 1000
void loop() {
  Point p = ts.getPoint();
  // if you're sharing pins, you'll need to fix the directions of the touchscreen pins!
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); 
     Serial.print(p.x);
     Serial.print("\tY = "); 
     Serial.print(p.y);
     Serial.print("\tPressure = "); 
     Serial.println(p.z);
     */
    // turn from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
    /*
    Serial.print("("); 
     Serial.print(p.x);
     Serial.print(", "); 
     Serial.print(p.y);
     Serial.println(")");
     */
    // red buttons
    if (p.y > 3 && p.y < 65 && p.x > 210 && p.x < 264) {
      r = r - 1;
      if (r <= 0) {
        r = 0;
      }
      showred();
      showhex();
    }
    if (p.y > 269 && p.y < 324 && p.x > 210 && p.x < 264) {
      r = r + 1;
      if (r >= 255) {
        r = 255;
      }
      showred();
      showhex();
    }
    // green buttons
    if (p.y > 3 && p.y < 65 && p.x > 139 && p.x < 195) {
      g = g - 1;
      if (g <= 0) {
        g = 0;
      }
      showgrn();
      showhex();
    }
    if (p.y > 269 && p.y < 324 && p.x > 139 && p.x < 195) {
      g = g + 1;
      if (g >= 255) {
        g = 255;
      }
      showgrn();
      showhex();
    }
    // blue buttons
    if (p.y > 3 && p.y < 66 && p.x > 72 && p.x < 126) {
      b = b - 1;
      if (b <= 0) {
        b = 0;
      }
      showblu();
      showhex();
    }
    if (p.y > 269 && p.y < 324 && p.x > 72 && p.x < 126) {
      b = b + 1;
      if (b >= 255) {
        b = 255;
      }
      showblu();
      showhex();
    }
    // reset button
    if (p.y > 1 && p.y < 55 && p.x > 12 && p.x < 57) {
      r = 0;
      g = 0;
      b = 0;
      showred();
      showgrn();
      showblu();
      showhex();
    }
    /*
    // uncomment this to get serial output when screen is pressed
    Serial.print("r:");
    Serial.print(r);
    Serial.print("   g:");
    Serial.print(g);
    Serial.print("   b:");
    Serial.println(b);
    Serial.print("HEX Output:0x");
    Serial.println(answer, HEX);
    */
  }
}
void showred() {
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  itoa (r, red, 10);
  tft.fillRect(144, 45, 36, 16, BLACK);
  tft.setCursor(144, 45);
  tft.print(red);
}
void showgrn() {
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  itoa (g, green, 10);
  tft.fillRect(144, 105, 36, 16, BLACK);
  tft.setCursor(144, 105);
  tft.print(green);
}
void showblu() {
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  itoa (b, blue, 10);
  tft.fillRect(144, 165, 36, 16, BLACK);
  tft.setCursor(144, 165);
  tft.print(blue);
}
void showhex() {
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  answer = ((r / 8) << 11) | ((g / 4) << 5) | (b / 8); //Thanks ktownsend for this!
  tft.fillRect(0, 200, 50, 40, answer);
  itoa (answer, output, HEX);
  tft.fillRect(271, 223, 46, 16, BLACK);
  tft.setCursor(271, 223);
  tft.print(output); 
}
void drawscreen() {
  // Red
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.fillRect(0, 20, 60, 50, RED);
  tft.drawRect(0, 20, 60, 50, WHITE);
  tft.drawRect(80, 20, 160, 50, JJCOLOR);
  tft.fillRect(260, 20, 60, 50, GREEN);
  tft.drawRect(260, 20, 60, 50, WHITE);
  tft.setCursor(22, 33);
  tft.print("-");
  tft.setCursor(134, 31);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.print("RED Value");
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(282, 33);
  tft.print("+");
  //Green
  tft.fillRect(0, 80, 60, 50, RED);
  tft.drawRect(0, 80, 60, 50, WHITE);
  tft.drawRect(80, 80, 160, 50, JJCOLOR);
  tft.fillRect(260, 80, 60, 50, GREEN);
  tft.drawRect(260, 80, 60, 50, WHITE);
  tft.setCursor(22, 93);
  tft.print("-");
  tft.setCursor(128, 91);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.print("Green Value");
  tft.setCursor(282, 93);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("+");
  //Blue
  tft.fillRect(0, 140, 60, 50, RED);
   tft.drawRect(0, 140, 60, 50, WHITE);
   tft.drawRect(80, 140, 160, 50, JJCOLOR);
   tft.fillRect(260, 140, 60, 50, GREEN);
   tft.drawRect(260, 140, 60, 50, WHITE);
   tft.setCursor(22, 153);
   tft.print("-");
   tft.setCursor(130, 151);
   tft.setTextColor(BLUE);
   tft.setTextSize(1);
   tft.print("Blue Value");
   tft.setCursor(282, 153);
   tft.setTextColor(WHITE);
   tft.setTextSize(3);
   tft.print("+");
}
