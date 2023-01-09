#include <TFT_eSPI.h>
#include "Latin_Hiragana_24.h"
#include "NotoSansBold15.h"
#include "NotoSansMonoSCB20.h"
#include <WiFi.h>
#include <time.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

const char* ssid = "xxxxxxxx";
const char* pass = "xxxxxxxx";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = (-6 * 3600);  //time zone * 3600
const int   daylightOffset_sec = 3600;

#define latin Latin_Hiragana_24
#define small NotoSansBold15
#define digits NotoSansMonoSCB20

#define color_white 0xE73C //white
#define color_grey  0x18C3 //gray
#define color_red   0x9986 //red
#define color_green 0x2CAB //green
#define color_gold  0xBDEF //gold

int fromTop = 34;
int left = 40;
int width = 240;
int heigth = 320; // was 80

uint16_t grays[60] = {0};
uint16_t lines[11] = {0};
int sec = 0;
int pos = 0;

int digit1 = 0;
int digit2 = 0;
int digit3 = 0;
int digit4 = 0;

long start = 0;
long end = 0;
int counter = 0;
double fps = 0;

// stores time as strings
char timeHour[3];
char timeMin[3];
char timeSec[3];

// setup LED control
#define LED_RED    4
#define LED_BLUE  17
#define LED_GREEN 16
// control red LED w/ PWM
// see https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/ledc.html
#define LED_CHANNEL 0         // PWM 0 to control red LED
const int freq = 2000;        // frequency
const uint8_t resolution = 8; // Resolution

void setup()
{
  Serial.begin(115200);

  //Initialize GPIO for LED
  pinMode(LED_RED, OUTPUT);   // red
  pinMode(LED_GREEN, OUTPUT);  // green
  pinMode(LED_BLUE, OUTPUT);  // blue
  // turn ON tricolor light (low = on)
  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_BLUE, 0);
  ledcSetup(LED_CHANNEL, freq, resolution); // set LED_CHANNEL

// Initialize TFP display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  for(int i = 0; i < 50; i++)
    grays[i] = tft.color565(i * 5, i * 5, i * 5);

  lines[0] = grays[5]; lines[1] = grays[10]; lines[2] = grays[20]; lines[3] = grays[30]; lines[4] = grays[40]; lines[5] = grays[49];
  lines[6] = grays[40]; lines[7] = grays[30]; lines[8] = grays[20]; lines[9] = grays[10]; lines[10] = grays[5];

  sprite.createSprite(320, 170);
  sprite.setTextDatum(4);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void draw()
{
  if(counter == 0)
    start = millis();
  counter++;

  if(counter == 100)
  {
    end = millis();
    fps = 100 / ((end - start) / 1000.00);
    counter = 0;
  }
  sprite.fillSprite(color_white);

  sprite.fillSmoothRoundRect(left + 30, fromTop - 20, width - 60, heigth, 8, color_grey, color_white);
  sprite.fillSmoothRoundRect(left + 32, fromTop - 18, width - 64, heigth, 8, color_white, color_grey);
  sprite.fillRect(left + 30, fromTop - 3, width - 60, 3, color_white);
  sprite.fillRect(left + 80, fromTop - 21, width - 160, 4, color_white);

  sprite.fillSmoothRoundRect(left, fromTop, width, heigth, 8, color_grey, color_white);

  sprite.fillSmoothRoundRect(220, fromTop, 60, heigth, 8, color_red, color_white);
  sprite.fillRect(219, fromTop, 4, heigth, color_white);

  sprite.fillSmoothCircle(220, fromTop + heigth, 9, color_white);
  sprite.fillSmoothCircle(220, fromTop + heigth, 5, color_grey);

  for(int i = 0; i < 4; i++)
  {
    sprite.fillRectHGradient(left + (20) + (i * 36), fromTop + 12, 15, 44, TFT_BLACK, grays[2]);
    sprite.fillRectHGradient(left + (35) + (i * 36), fromTop + 12, 15, 44, grays[2], TFT_BLACK);

    for(int j = 0; j < 11; j++)
      if(j == 5)
        sprite.drawLine(left + (40) + (i * 36), fromTop + 14 + (j * 4), left + (47) + (i * 36), fromTop + 14 + (j * 4), lines[j]);
      else
        sprite.drawLine(left + (43) + (i * 36), fromTop + 14 + (j * 4), left + (47) + (i * 36), fromTop + 14 + (j * 4), lines[j]);
  }

  sprite.fillRectHGradient(231, fromTop + 12, 20, 44, TFT_BLACK, grays[2]);
  sprite.fillRectHGradient(251, fromTop + 12, 20, 44, grays[2], TFT_BLACK);

  for(int j = 0; j < 11; j++)
    if(j == 5)
      sprite.drawLine(262, fromTop + 14 + (j * 4), 269, fromTop + 14 + (j * 4), lines[j]);
    else
      sprite.drawLine(265, fromTop + 14 + (j * 4), 269, fromTop + 14 + (j * 4), lines[j]);

  sprite.drawLine(left + 28, fromTop + 60, left + 28, fromTop + 66, color_white);
  sprite.drawLine(left + 28, fromTop + 66, left + 32, fromTop + 66, color_white);

  sprite.drawLine(left + 78, fromTop + 66, left + 78, fromTop + 60, color_white);
  sprite.drawLine(left + 74, fromTop + 66, left + 78, fromTop + 66, color_white);

  sprite.drawLine(left + 102, fromTop + 66, left + 106, fromTop + 66, color_white);
  sprite.drawLine(left + 102, fromTop + 66, left + 102, fromTop + 60, color_white);

  sprite.drawLine(left + 144, fromTop + 66, left + 148, fromTop + 66, color_white);
  sprite.drawLine(left + 148, fromTop + 66, left + 148, fromTop + 60, color_white);

  sprite.loadFont(latin);
  sprite.setTextColor(color_grey, color_white);
  sprite.drawString("TIME", 160, 18);
  sprite.unloadFont();

  sprite.loadFont(small);
  sprite.setTextColor(color_white, color_red);
  sprite.drawString("sec", 250, fromTop + 68);
  sprite.setTextColor(color_white, color_grey);
  sprite.drawString("hour", 94, fromTop + 68);
  sprite.drawString("min", 164, fromTop + 68);
  sprite.setTextColor(color_grey, color_white);
  sprite.drawString("POWER", 160, fromTop + 96);
  sprite.setTextColor(color_red, color_white);
  sprite.drawString("0", 62, fromTop + 104);

  sprite.setTextColor(color_green, color_white);
  sprite.drawString("100%", 272, fromTop + 104);
  sprite.drawString(String(fps), 20, 164);
  sprite.unloadFont();

  sprite.loadFont(digits);
  sprite.setTextColor(color_white, TFT_BLACK);
  sprite.drawString(String(timeHour[1]), 106, fromTop + 35);
  sprite.drawString(String(timeHour[0]), 70, fromTop + 35);
  sprite.drawString(String(timeMin[0]), 142, fromTop + 35);
  sprite.drawString(String(timeMin[1]), 178, fromTop + 35);
  sprite.drawString(timeSec, 249, fromTop + 35);
  sprite.unloadFont();

  sprite.fillRect(70, fromTop + 102, 180, 3, color_green);
  sprite.fillRect(190, fromTop + 102, 60, 3, color_grey);

  sprite.fillRect(60, fromTop + 120, 200, 2, color_red);
  sprite.fillTriangle(265, fromTop + 120, 255, fromTop + 120, 252, fromTop + 116, color_red);
  sprite.fillTriangle(265, fromTop + 120, 255, fromTop + 120, 252, fromTop + 124, color_red);

  sprite.fillSmoothRoundRect(80, fromTop + 112, 160, 17, 10, color_grey, color_white);
  sprite.drawRect(100, fromTop + 116, 120, 8, TFT_BLACK);
  sprite.fillRectHGradient(101, fromTop + 117, 59, 6, grays[1], grays[44]);
  sprite.fillRectHGradient(160, fromTop + 117, 59, 6, grays[44], grays[1]);

  for(int i = 0; i < 11; i++)
  {
    sprite.fillRect(102 + (i * 10) + pos, fromTop + 118, 2, 4, color_grey);
  }

  sprite.pushSprite(0, 0);
}

void loop()
{
  draw(); // fps is calculated by the draw() function.

  // the display updates ~30 times per second.
  // ideally we would use fsp to call the time check, but fsp bounces around,
  // so the update rate would be inconsistent.
  if(++sec >= 30)
  {
    sec = 0;
    printLocalTime();
  }

  pos++;
  if(pos == 10)
    pos = 0;

  // control LED output
  runLED();
}

// Get local time and print to strings.
void printLocalTime()
{
  struct tm timeinfo;

  // this can take up to 5,000 mS to time out.
  // see https://forum.arduino.cc/t/simple-time-not-so-simple/941214/2
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  else
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  // strftime() is comparable to sprintf() but for time.
  // https://www.mkssoftware.com/docs/man3/strftime.3.asp
  strftime(timeHour, 3, "%H", &timeinfo);
  strftime(timeMin, 3, "%M", &timeinfo);
  strftime(timeSec, 3, "%S", &timeinfo);

}

// the main loop runs ~30 times/sec.
// so the static states run 30 times before advancing.
#define STATIC_DELAY  30
void runLED(void)
{
  static int stateVar = 0;
  static int dutyCycle = 255;
  static int fixedLedDelay = 0;

  switch (stateVar)
  {
  case 0: // red
    ledcDetachPin(LED_RED);
    if (fixedLedDelay == 0)
      Serial.println("RED");
    digitalWrite(LED_RED, 0);
    digitalWrite(LED_GREEN, 1);
    digitalWrite(LED_BLUE, 1);
    if (++fixedLedDelay >= STATIC_DELAY)
    {
      stateVar++;
      fixedLedDelay = 0;
    }
    break;
  case 1: // green
    if (fixedLedDelay == 0)
      Serial.println("GREEN");
    digitalWrite(LED_RED, 1);
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 1);
    if (++fixedLedDelay >= STATIC_DELAY)
    {
      stateVar++;
      fixedLedDelay = 0;
    }
    break;
  case 2: // blue
    if (fixedLedDelay == 0)
      Serial.println("BLUE");
    digitalWrite(LED_RED, 1);
    digitalWrite(LED_GREEN, 1);
    digitalWrite(LED_BLUE, 0);
    if (++fixedLedDelay >= STATIC_DELAY)
    {
      stateVar++;
      fixedLedDelay = 0;
    }
    break;
  case 3: // all off (low = on)
    if (fixedLedDelay == 0)
      Serial.println("OFF");
    digitalWrite(LED_RED, 1);
    digitalWrite(LED_GREEN, 1);
    digitalWrite(LED_BLUE, 1);
    if (++fixedLedDelay >= STATIC_DELAY)
    {
      stateVar++;
      fixedLedDelay = 0;
    }
    break;

  // Note: PWM only controls pin 4 (red LED), not all LED pins.
  case 4:
    // set the others to off so we can tell which is PWM'ed
    digitalWrite(LED_GREEN, 1);
    digitalWrite(LED_BLUE, 1);
    ledcAttachPin(LED_RED, LED_CHANNEL);
    // gradually brighten
    if (dutyCycle >= 0)
    {
      ledcWrite(LED_CHANNEL, dutyCycle);  // output PWM
      dutyCycle -= 5;
    }
    else {
      stateVar++;
      dutyCycle = 0;
    }
    Serial.print("duty cycle: ");
    Serial.println(dutyCycle);
    break;
  case 5:
    // gradually darken
    if (dutyCycle <= 255)
    {
      ledcWrite(LED_CHANNEL, dutyCycle);  // output PWM
      dutyCycle += 5;
    }
    else {
      dutyCycle = 255;
      stateVar++;
    }
    Serial.print("duty cycle: ");
    Serial.println(dutyCycle);
    break;
  case 6: // unused.
  default:
    stateVar = 0;
  }

} // end runLED()
