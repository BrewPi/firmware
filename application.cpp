/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#include "Adafruit_ILI9341/Adafruit_ILI9341.h"
#include "ScrollBox/ScrollBox.h"
#include "DS2482/DS2482.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// Define the pins we're going to call pinMode on
int act1 = A0;
int act2 = A1;
int act3 = A6;
int buzz = A2;
Adafruit_ILI9341 tft = Adafruit_ILI9341(D4, D5, 0);
ScrollBox debugBox(&tft);
DS2482 ds(0);

unsigned long testText();

void setup() {
    Serial.begin(57600);
    Wire.begin(); 
    ds.reset();
    pinMode(act1, OUTPUT);
    pinMode(act2, OUTPUT);
    pinMode(act3, OUTPUT);
    pinMode(buzz, OUTPUT);
    digitalWrite(buzz, HIGH);
    tft.begin();
    tft.setRotation(1);
    testText();
    
    //configure DS2482 to use active pull-up instead of pull-up resistor 
    //configure returns 0 if it cannot find DS2482 connected 
    if (!ds.configure(DS2482_CONFIG_APU)) 
    { 
        Serial.print("DS2482 not found\n");
        debugBox.println("DS2482 not found\n");
    }
    
    debugBox.println("BrewPi started");
    /*
    debugBox.print("It is ");
    debugBox.print(Time.timeStr());
    
    debugBox.print("Connected to ");
    debugBox.println(WiFi.SSID());
    
    debugBox.print("Signal strength: ");
    debugBox.print(WiFi.RSSI());
    debugBox.println("dB");
    
    debugBox.print("My IP is: ");
    debugBox.println(WiFi.localIP());
    */
    
}

void loop(void) {  
    byte addr[8];
    if ( !ds.search(addr)) {
       debugBox.println("No more addresses.");
       ds.reset_search();
       delay(250);
       return;
     }
    else{
        Serial.print("R=");
        for(uint8_t i = 0; i < 8; i++) {
          Serial.print(addr[i], HEX);
          debugBox.print(addr[i], HEX);
          Serial.print(" ");
          debugBox.print(" ");
        }
    }
    debugBox.println("");
    delay(1000);
}

unsigned long testFillScreen() {
    unsigned long start = micros();
    tft.fillScreen(ILI9341_BLACK);
    tft.fillScreen(ILI9341_RED);
    tft.fillScreen(ILI9341_GREEN);
    tft.fillScreen(ILI9341_BLUE);
    tft.fillScreen(ILI9341_BLACK);
    return micros() - start;
}

unsigned long testText() {
    tft.fillScreen(ILI9341_BLACK);
    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);

    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(3);
    tft.println("BrewPi");
    tft.setTextColor(ILI9341_GREEN);
    tft.println("LCD test 2");
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    tft.setTextColor(ILI9341_WHITE);
    tft.print("HLT in: ");
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(68.1);
    tft.setTextColor(ILI9341_WHITE);
    tft.print("HLT out: ");
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(68.4);
    tft.setTextColor(ILI9341_WHITE);
    tft.print("Mash in: ");
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(66.5);
    tft.setTextColor(ILI9341_WHITE);
    tft.print("Mash out: ");
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(68.3);
    tft.setTextColor(ILI9341_WHITE);
    tft.print("Boil out: ");
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(52.3);
    tft.setTextSize(1);
    tft.print("And in the smallest font, space on the screen is almost unlimited!");
    tft.print("Bla bla bla...");
    tft.print("This could be the fine print disclaimer");

    return micros() - start;
}

unsigned long testLines(uint16_t color) {
    unsigned long start, t;
    int x1, y1, x2, y2,
            w = tft.width(),
            h = tft.height();

    tft.fillScreen(ILI9341_BLACK);

    x1 = y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    t = micros() - start; // fillScreen doesn't count against timing

    tft.fillScreen(ILI9341_BLACK);

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    tft.fillScreen(ILI9341_BLACK);

    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    tft.fillScreen(ILI9341_BLACK);

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);

    return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

unsigned long testRects(uint16_t color) {
    unsigned long start;
    int n, i, i2,
            cx = tft.width() / 2,
            cy = tft.height() / 2;

    tft.fillScreen(ILI9341_BLACK);
    n = min(tft.width(), tft.height());
    start = micros();
    for (i = 2; i < n; i += 6) {
        i2 = i / 2;
        tft.drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
    unsigned long start, t = 0;
    int n, i, i2,
            cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    n = min(tft.width(), tft.height());
    for (i = n; i > 0; i -= 6) {
        i2 = i / 2;
        start = micros();
        tft.fillRect(cx - i2, cy - i2, i, i, color1);
        t += micros() - start;
        // Outlines are not included in timing results
        tft.drawRect(cx - i2, cy - i2, i, i, color2);
    }

    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (x = radius; x < w; x += r2) {
        for (y = radius; y < h; y += r2) {
            tft.fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
    unsigned long start;
    int x, y, r2 = radius * 2,
            w = tft.width() + radius,
            h = tft.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for (x = 0; x < w; x += r2) {
        for (y = 0; y < h; y += r2) {
            tft.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles() {
    unsigned long start;
    int n, i, cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    n = min(cx, cy);
    start = micros();
    for (i = 0; i < n; i += 5) {
        tft.drawTriangle(
                cx, cy - i, // peak
                cx - i, cy + i, // bottom left
                cx + i, cy + i, // bottom right
                tft.Color565(0, 0, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles() {
    unsigned long start, t = 0;
    int i, cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (i = min(cx, cy); i > 10; i -= 5) {
        start = micros();
        tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                tft.Color565(0, i, i));
        t += micros() - start;
        tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                tft.Color565(i, i, 0));
    }

    return t;
}

unsigned long testRoundRects() {
    unsigned long start;
    int w, i, i2,
            cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    w = min(tft.width(), tft.height());
    start = micros();
    for (i = 0; i < w; i += 6) {
        i2 = i / 2;
        tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.Color565(i, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects() {
    unsigned long start;
    int i, i2,
            cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

    tft.fillScreen(ILI9341_BLACK);
    start = micros();
    for (i = min(tft.width(), tft.height()); i > 20; i -= 6) {
        i2 = i / 2;
        tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.Color565(0, i, 0));
    }

    return micros() - start;
}
