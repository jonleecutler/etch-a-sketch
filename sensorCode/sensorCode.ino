#define SENSORPINX A5 // x axis
#define SENSORPINY A3 // y axis

#define ACCELEROMETERPINX A2 // x
#define ACCELEROMETERPINY A1 // x
#define ACCELEROMETERPINZ A0 // x

#define BUTTONPIN 5

unsigned long targetTime = 0;
const unsigned long interval = 100;
unsigned long targetCount = 0;
const unsigned long count = 15;
const unsigned long change = 50;
int prevXAccel = 0;
int prevYAccel = 0;
int prevZAccel = 0;
bool pressed = false;

const String reset = "rst";
const String color = "clr";

void setup(){
  // initialize the button pin as an input:
  pinMode(BUTTONPIN, INPUT);
  
  Serial.begin(115200);
}


void loop(){
	if(millis()>=targetTime){

    // Check the button state to flip states if needed
    int buttonState = digitalRead(BUTTONPIN);
    if (buttonState == HIGH) {
      if (!pressed) {
        Serial.println(color.c_str());
      }
      
      pressed = true;
    }
    else {
      pressed = false;
    }

    // Get data from accelerometer
    int xAccel = analogRead(ACCELEROMETERPINX);
    int yAccel = analogRead(ACCELEROMETERPINY);
    int zAccel = analogRead(ACCELEROMETERPINZ);

    // Use data from the accelerometer to, calculate current change, reset previous stored values
    int currentChange = abs(xAccel - prevXAccel) + abs(yAccel - prevYAccel) + abs(zAccel - prevZAccel);
    prevXAccel = xAccel;
    prevYAccel = yAccel;
    prevZAccel = zAccel;

    // Update the current change counter
    if (currentChange >= change) {
      targetCount++;
      
      // If the current change count has hit the target send a reset instead of data
      if (targetCount >= count) {
        Serial.println(reset.c_str());
        targetCount = 0;
      }
    }
    else {
      targetCount = 0;
    }

    // Get data from sensors
    int xSensorPos = analogRead(SENSORPINX);
    int ySensorPos = analogRead(SENSORPINY);
    
    String data = String(xSensorPos) + "," + String(ySensorPos) + "\r\n";
		Serial.println(data.c_str());

    targetTime= millis()+interval;
	}
 
	// TODO: Detect if you want to reset the screen(shake the etch-a-sketch)
  // TODO: write the reset message(see server.js) to the serial port
}
