//
// PROJECT Follow the Beacon of Light
//
// BY Pyae Phyo Aung Elvis
// WRITTEN IN February 2018
//

// MASTER CONSTANT DEFINITIONS
typedef int u_int32_t; // Turning all integers into unsigned 32 bit integer type
const int pins[] = [A0, A2, A4, 6, 7]; // PINS CONFIG – 0\Calibration 1\LDR Left 2\LDR Right 3\Motor Left 4\Motor Right
const int threshAmbientLower, threshDifferenceMax = 100, masterDelayFactor = 2, masterDelayDuration = 100;

//
// GENERAL PURPOSE FUNCTIONS
//

int getDifference(int inA, int inB) {
  int short difference = inA - inB;
  if (difference < 0) { difference = -difference; }
  return difference;
}

int getAverage(int inA, int inB) {
  int average = (inA + inB)/2;
  return average;
}

int getGreater(int inA, int inB) {
  if (inA > inB) { return 1; } 
  else if (inA < inB) { return 2; }
  else { return 0; }
}

//
// LOGIC FUNCTIONS
//

void move(string direction, int duration) {
  switch (direction) {
    case "straight":
      //Incorporate Trapezoidal Velocity Profile
      //Ramp up both Motors
      //Kill off after duration ends
      break;
    case "left":
      //Incorporate Trapezoidal Velocity Profile
      //Ramp up Right Motor
      //Kill off after duration ends
      break;
    case "right":
      //Incorporate Trapezoidal Velocity Profile
      //Ramp up Left Motor
      //Kill off after duration ends
      break;
    default:
      Serial.println("/!\\ ERROR in FUNCTION : Move(Direction, Duration)")
  }
}

void findLightSource(int lightValueL, int lightValueR, int lightDifference) {
  int greaterValue = getGreater(lightValueL, lightValueR);
  switch (greaterValue) {
    case 1:
      if (lightDifference <= threshDifferenceMax) {
        move("straight", masterDelayDuration * masterDelayFactor);
      } else {
        move("left", masterDelayDuration * masterDelayFactor);
      }
      break;
    case 2:
      if (lightDifference <= threshDifferenceMax) {
        move("straight", masterDelayDuration * masterDelayFactor);
      } else {
        move("right", masterDelayDuration * masterDelayFactor);
      }
      break;
    case 0:
      move("straight", masterDelayDuration * masterDelayFactor);
      break;
    default:
      Serial.println("/!\\ ERROR in FUNCTION : FindLightSource(LLL, LLR, LDiff)");
      break;
    } 
}

//
// DATA I/O & PROCESSING FUNCTIONS
//

int getLightData() {
  int lightValueL = analogRead(pins[1]);
  int lightValueR = analogRead(pins[2]);
  int threshCalibration = analogRead(pins[0]);
  return int results[lightValueL, lightValueR, threshCalibration];
}

int processLightData(int valueL, int valueR) {
  int avgLightLevel = getAverage(valueL, valueR);
  int diffLightLevel = getDifference(valueL, valueR);
  return int result[2] = [avgLightLevel, diffLightLevel]; // Returns an integer array with [0] Average Light Level, & [1] Light Level Difference
}

//
// CORE FUNCTIONS
//

void setup() {
  Serial.begin(9600);
  pinMode(pins[0], INPUT); //Light Threshold Calibrator
  pinMode(pins[1], INPUT); //LDR Left
  pinMode(pins[2], INPUT); //LDR Right
  pinMode(pins[3], OUTPUT); //Motor Left
  pinMode(pins[4], OUTPUT); //Motor Right
}

void loop() {
  int rawLightData[3] = getLightData();
  int processedLightData[2] = processLightData(rawLightData[0], rawLightData[1]);
  threshAmbientLower = rawLightData[2];

  if (processedLightData[0] >= threshAmbientLower) {
    if (processedLightData[1] <= threshDifferenceMax) {
      move("straight", masterDelayDuration * masterDelayFactor);
    } else {
      findLightSource(rawLightData[0], rawLightData[1], processedLightData[1]);
    }
  } else {
    findLightSource(rawLightData[0], rawLightData[1], processedLightData[1]);
  }
}
