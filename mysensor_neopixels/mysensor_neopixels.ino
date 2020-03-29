#include <Adafruit_NeoPixel.h>

#define MY_RADIO_RF24

#include <MySensors.h>

#define NEO_PIN      4 // NeoPixels input pin
#define NUMPIXELS      16 // Number of nexpixels in ring/strip
#define MAX_INTESITY   20  // Intesity of leds (in percentage). Remeber more intesity requires more power.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

#define CHILD_ID_LIGHT 15

void setup() {
  Serial.begin(115200);
  Serial.println("Starting distance sensor");
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.println("Neopixels initialized");
}


void presentation()  {
  sendSketchInfo("lampy", "1.0");
  present(CHILD_ID_LIGHT, S_RGB_LIGHT, "tiny lamp", true);
}

bool light_should_be_on = false;
int brightness_should_be = 50;
long colour_should_be = -1;

void loop() {
  pixels.setBrightness(brightness_should_be);
  
  if (light_should_be_on) {
    pixels.fill(colour_should_be, 0, 16);
  }
  else {
    pixels.fill(pixels.Color(0,0,0), 0, 16);
  }
  
  pixels.show();
}


void receive(const MyMessage &message) {
  Serial.println("Message received: ");
  Serial.print("type= "); Serial.println(message.type);
  Serial.print("data = "); Serial.println(message.data);
  
  if (message.isAck())  {
    Serial.println("Got ack from gateway");
  }
  if (message.type == V_STATUS) {
    light_should_be_on = (atoi(message.data) ==1);
  }
  if(message.type == V_PERCENTAGE) {
    brightness_should_be = 255 / 100 * atoi(message.data);
  }
  if (message.type == V_RGBW) {
    light_should_be_on = true;
    String hexstring = message.getString();

    // If the white value is on the end, remove it
    if(hexstring.length() > 6) {
      hexstring[7] = '\0';
      hexstring[6] = '\0';
    }
    Serial.print("hex colour = "); Serial.println(hexstring);
    colour_should_be = strtol( &hexstring[0], NULL, 16);
    Serial.print("color = "); Serial.println(colour_should_be);
  }
  Serial.println();
}
