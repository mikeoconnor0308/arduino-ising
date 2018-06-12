#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <gfxfont.h>
#include <Wire.h>

Adafruit_BicolorMatrix ledMatrix = Adafruit_BicolorMatrix();
int sensorPin = A0;    // select the input pin for the photoresistor
int sensorValue = 0;

bool mainLoop = false;

//dimensions of led matrix.
const int xLength = 8;
const int yLength = 8;
//current configuration of Ising model. 
int config[xLength][yLength];
//default temperature of model.
float temperature = 2.3;

//Generates a random initial state
void GenerateInitialConfiguration()
{
    for(int i=0; i < xLength; i++)
    {
        for(int j =0; j < xLength; j++)
        {
            //Generates either 0 or -1;
            config[i][j] = 2 * random(0, 2) - 1;
            
        }
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Ising Model Test");

    randomSeed(analogRead(0));
    ledMatrix.begin(0x70); // pass in the address
    GenerateInitialConfiguration();
}

//Update the LED matrix based on the Ising model.
void UpdateLEDMatrix()
{
    ledMatrix.clear();
    for(int x =0; x < xLength; x++)
    {
        for(int y =0; y < yLength; y++)
        {
            //If up, then color yellow, otherwise color red. Green is also an option!
            if(config[x][y] == 1) 
                ledMatrix.drawPixel(x,y, LED_YELLOW);
            else if(config[x][y] == -1)
                ledMatrix.drawPixel(x,y, LED_RED);
        }
        
    }
    ledMatrix.writeDisplay();
}

//Perform the monte carlo update of the simulation.
void MonteCarloMove(float beta)
{
    for(int i=0; i < xLength; i++)
    {
        for(int j=0; j < yLength; j++)
        {
            int x = random(0, xLength); 
            int y = random(0, yLength); 
            int state = config[x][y];
            int nb = (int) config[(x+1)%xLength][y] + (int) config[x][(y+1)%yLength] + (int) config[(x-1)%xLength][y] + (int) config[x][(y-1)%yLength];
            int cost = 2 * state * nb;
            if(cost < 0)
                state *= -1;
            else 
            {
                float prob = (float) random(0, 100) / (float) 100; 
                if(prob < exp(-cost * beta)) 
                    state *= -1; 
            }
            config[x][y] = state;
        }
    }
}

//Filter the photoresistor sensor input. 
float filter(int sensorValue)
{
    float minValue = 60.0;
    float maxValue = 400.0; 
    float scaling = 2.0;
    float bump = 0.25;
    float val = max(0, sensorValue - minValue);
    return (scaling * val / (maxValue - minValue)) + bump;
}

//Main loop
void loop()
{
    sensorValue = analogRead(sensorPin);

    //Displays some introductory text.
    if (!mainLoop)
    {
        ledMatrix.setTextWrap(false);
        ledMatrix.setTextSize(1);
        ledMatrix.setTextColor(LED_GREEN);
        for (int8_t x = 7; x >= -36; x--)
        {
            ledMatrix.clear();
            ledMatrix.setCursor(x, 0);
            ledMatrix.print("Ising");
            ledMatrix.writeDisplay();
            delay(30);
        }
        mainLoop = true;

    }

    //Update the temperature based on sensor.
    float temp = temperature * filter(sensorValue);
    Serial.println(sensorValue);
    //Perform monte carlo move.
    MonteCarloMove(1.0/temp);
    UpdateLEDMatrix();
    //Chill out for a few ms. 
    delay(50);

}
