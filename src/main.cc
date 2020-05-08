#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 7
#define BRIGHTNESS 127
#define SATURATION 255
#define AXIS 5
#define NUM_LEDS 125

#define MULTIPLIER 15
#define DELAY 20
#define COLOR_SHIFT 1

CRGB leds[NUM_LEDS];

// z modulo 2, y, x
uint8_t lookup_table[2][5][5] =
    {
        {
        {0, 1, 2, 3, 4},
        {9, 8, 7, 6, 5},
        {10, 11, 12, 13, 14},
        {19, 18, 17, 16, 15},
        {20, 21, 22, 23, 24}
        },
        {
        {24, 23, 22, 21, 20},
        {15, 16, 17, 18, 19},
        {14, 13, 12, 11, 10},
        {5, 6, 7, 8, 9},
        {4, 3, 2, 1, 0}
        }
};

uint8_t get_addr(int x, int y, int z)
{

    return lookup_table[z % 2][y][x] + z * 25;
}

void setup()
{
    delay(300);
    Serial.begin(115200);
    Serial.println("Cube starting up.");
    FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CHSV(i * 2, SATURATION, BRIGHTNESS);
        FastLED.show();
        delay(15);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(15);
    }

    for (uint8_t z = 0; z < AXIS; z++)
    {
        for (uint8_t y = 0; y < AXIS; y++)
        {
            for (uint8_t x = 0; x < AXIS; x++)
            {
                leds[get_addr(x, y, z)] = CRGB::White;
                FastLED.show();
                delay(50);
                leds[get_addr(x, y, z)] = CRGB::Black;
            }
        }
    }
}

uint8_t offs = 0;
void loop()
{

    for (uint8_t z = 0; z < AXIS; z++)
    {
        for (uint8_t y = 0; y < AXIS; y++)
        {
            for (uint8_t x = 0; x < AXIS; x++)
            {
                leds[get_addr(x, y, z)] = CHSV(
                    x * MULTIPLIER +
                        y * MULTIPLIER +
                        z * MULTIPLIER +
                        offs,
                    SATURATION, BRIGHTNESS);
            }
        }
    }
    FastLED.show();
    offs = offs + COLOR_SHIFT;
    delay(DELAY);
}