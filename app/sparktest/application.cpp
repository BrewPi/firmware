/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include "Adafruit_mfGFX.h"
#include "Adafruit_ILI9341.h"
#include "ScrollBox.h"
#include "OneWire.h"
#include "BrewPiTouch.h"
#include "DS2408.h"
#include "ValvesController.h"
#include "Platform.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// Define the pins we're going to call pinMode on
int act1 = A0;
int act2 = A1;
int act3 = A6;
int buzz = A2;
Adafruit_ILI9341 tft = Adafruit_ILI9341(D4, D5);
ScrollBox debugBox(&tft);
OneWire ow(0);
BrewPiTouch touch(D3, D2);

unsigned long testText();
void printOneWireAdresses(void);
void ballValvesSerialTest(void);

void setup() {
    pinMode(act1, OUTPUT);
    pinMode(act2, OUTPUT);
    pinMode(act3, OUTPUT);
    pinMode(buzz, OUTPUT);
    digitalWrite(buzz, LOW);
    delay(200);
    digitalWrite(buzz, HIGH);

    SPI.begin();
    //TODO, lgramatikov, core runs at 72MHz. 11 gives 6.5. But looks like Spark can do only predefined values - http://docs.spark.io/#/firmware/communication-spi 
    //16 looks like good start.
    SPI.setClockDivider(SPI_CLOCK_DIV16); //not quite full! speed! :)
    //SPI.setClockDivider(11); // 85MHz / 11 = 7.6 MHz (full! speed!)

    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);

    tft.begin();
    tft.setRotation(3);

    testText();

    Serial.begin(57600);
    
    //configure DS248X to use active pull-up instead of pull-up resistor 
    //configure returns 0 if it cannot find DS248X connected 
    if (!ow.init()) {
        const char error[] = "OneWire initialization failed\n";
        Serial.print(error);
        debugBox.println(error);
    }
    //touch.init();
    debugBox.println("BrewPi started");
    printOneWireAdresses();
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
    ballValvesSerialTest();
}

void printOneWireAdresses(void){
    uint8_t addr[8];
    if (!ow.search(addr)) {
        debugBox.println("No more addresses.");
        ow.reset_search();
        delay(250);
        return;
    } else {
        Serial.print("R=");
        for (uint8_t i = 0; i < 8; i++) {
            Serial.print(addr[i], HEX);
            debugBox.print(addr[i], HEX);
            Serial.print(" ");
            debugBox.print(" ");
        }
    }
    debugBox.println("");
}

void ballValvesSerialTest(){
    uint8_t addr[8];
    ow.search(addr); //get first address from bus
    ValvesController valves;
    valves.init(&ow, addr);
    
    while (1) {
        valves.update();
        
        if (Serial.available()) {
            char c = Serial.read();
            switch (c) {
                case '1':
                    valves.open(0);
                    break;
                case '2':
                    valves.close(0);
                    break;
                case '3':
                    valves.stop(0);
                    break;
                case '4':
                    valves.open(1);
                    break;
                case '5':
                    valves.close(1);
                    break;
                case '6':
                    valves.stop(1);
                    break;
            }    
        }
        delay(50);        
    }
}
/*
void touchCalibrateTest(){
    touch.calibrate(&tft);
    touch.update();
    if (touch.isTouched()) {
        touch.update();
        Serial.print(touch.getXRaw());
        Serial.print("\t");
        Serial.print(touch.getYRaw());
        delay(10);
        //digitalWrite(buzz, LOW);

    } else {
        digitalWrite(buzz, HIGH);
    }
}
*/

void loop(void) {
    //printOneWireAdresses();
    ballValvesSerialTest();
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
    tft.setCursor(0, 2);

    tft.setTextColor(tft.Color565(0xD4, 0x26, 0x50));
    tft.setTextSize(3);
    tft.print("BrewPi ");
    tft.setTextColor(tft.Color565(0x5F, 0xB1, 0x46));
    tft.setTextSize(3);
    tft.println("Spark");

    tft.setTextSize(1);
    tft.println();
    tft.setTextSize(2);

    tft.setTextColor(0x3333);
    tft.setCursorX(50);
    tft.println("Mashing display demo");
    tft.println();

    char const * name[6] = {"HLT in: ", "HLT out: ", "Mash in: ", "Mash out: ", "Boil in:", "Boil out:"};
    float value[6] = {68.6, 68.8, 68.7, 68.1, 25.1, 26.4};
    float setting[6] = {0x0, 69.0, 0x0, 68.5, 0x0, 0x0};

    tft.setTextColor(ILI9341_YELLOW);
//    tft.setTextSize(1);
    tft.setCursorX(135);
    tft.print("Actual");
    tft.setCursorX(225);
    tft.println("Target");

    tft.setTextSize(2);
    for (int i = 0; i < 6; i++) {
        tft.setTextColor(tft.Color565(0xAA, 0xAA, 0xAA));
        tft.setCursorX(15);
        tft.print(name[i]);

    tft.setTextColor(ILI9341_WHITE);
        tft.setCursorX(140);
        tft.print(value[i]);
        tft.setCursorX(230);
        if(setting[i] == 0x00){
            tft.println(" -.--");
        }
        else{
            tft.println(setting[i]);
        }
    }
    tft.setTextSize(1);
    tft.println();
    for (int i = 0; i < 12; i++) {
        int valve = i+1;
        tft.setCursor(i/3 * 80, i%3*8 + 200);
        tft.setTextColor(ILI9341_WHITE);
        tft.print(valve);
        tft.print(": ");
        if ((valve > 1 && valve < 6) || valve == 9) {
            tft.setTextColor(ILI9341_GREEN);
            tft.print("open");
        } else {
            tft.setTextColor(ILI9341_RED);
            tft.print("closed");
        }
        if ((i + 1) % 3 == 0) {
            tft.println();
        }      
    }


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
