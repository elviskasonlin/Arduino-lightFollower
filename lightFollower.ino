/*
 * IE1 PROJECT
 * Light Follower
 *
 * BY Pyae Phyo Aung Elvis
 * WRITTEN IN February 2017
 *
 */

typedef int u_int32_t; // Turning all integers into unsigned 32 bit integer type
const int pins[5] = {0, 1, 2, 5, 6}; // PINS CONFIG – 0\Calibration 1\LDR Left 2\LDR Right 3\Motor Left 4\Motor Right
const int threshDifferenceMax = 300, masterDelayFactor = 2, masterDelayDuration = 100;
int threshAmbientLower = 0;

//
// GENERAL PURPOSE FUNCTIONS
//

int getDifference(int inA, int inB) {
  int short difference = inA - inB;
  if (difference < 0) {
    difference = -difference;
  }
  return difference;
}

int getAverage(int inA, int inB) {
  int average = (inA + inB) / 2;
  return average;
}

int getGreater(int inA, int inB) {
  if (inA > inB) {
    return 1;
  }
  else if (inA < inB) {
    return 2;
  }
  else {
    return 0;
  }
}

//
// LOGIC FUNCTIONS
//

void move(int direction, int duration) {
  switch (direction) {
    case 0:
      Serial.println("Straight");
      delay(1000);
      digitalWrite(pins[3], HIGH);
      digitalWrite(pins[4], HIGH);
      delay(duration);
      digitalWrite(pins[3], LOW);
      digitalWrite(pins[4], LOW);
      break;
    case 1:
      Serial.println("Left");
      delay(1000);
      digitalWrite(pins[4], HIGH);
      delay(duration);
      digitalWrite(pins[4], LOW);
      break;
    case 2:
      Serial.println("Right");
      delay(1000);
      digitalWrite(pins[3], HIGH);
      delay(duration);
      digitalWrite(pins[3], LOW);
      break;
    default:
      Serial.println("/!\\ ERROR in FUNCTION : Move(Direction, Duration)");
  }
}

void findLightSource(int lightValueL, int lightValueR, int lightDifference) {
  int greaterValue = getGreater(lightValueL, lightValueR);
  switch (greaterValue) {
    case 1:
      if (lightDifference <= threshDifferenceMax) {
        move(0, masterDelayDuration * masterDelayFactor);
      } else {
        move(1, masterDelayDuration * masterDelayFactor);
      }
      break;
    case 2:
      if (lightDifference <= threshDifferenceMax) {
        move(0, masterDelayDuration * masterDelayFactor);
      } else {
        move(2, masterDelayDuration * masterDelayFactor);
      }
      break;
    case 0:
      move(0, masterDelayDuration * masterDelayFactor);
      break;
    default:
      Serial.println("/!\\ ERROR in FUNCTION : FindLightSource(LLL, LLR, LDiff)");
      break;
  }
}

//
// DATA I/O & PROCESSING FUNCTIONS
//

/*
int *getLightData() {
  int lightValueL = analogRead(pins[1]);
  int lightValueR = analogRead(pins[2]);
  int threshCalibration = analogRead(pins[0]);
  int *results[3] = {lightValueL, lightValueR, threshCalibration};
  return *results;
}

int *processLightData(int valueL, int valueR) {
  int avgLightLevel = getAverage(valueL, valueR);
  int diffLightLevel = getDifference(valueL, valueR);
  int *results[2] = {avgLightLevel, diffLightLevel}; // Returns an integer array with [0] Average Light Level, & [1] Light Level Difference
  return *results;
}
*/

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
  /*
    int rawLightData = getLightData();
    int* processedLightData = processLightData(rawLightData[0], rawLightData[1]);
  */

  int rawLightData[3] = {analogRead(pins[1]), analogRead(pins[2]), analogRead(pins[0])};
  int processedLightData[2] = {getAverage(rawLightData[0], rawLightData[1]), getDifference(rawLightData[0], rawLightData[1])};
  threshAmbientLower = rawLightData[2];

  Serial.println("LDR Left" + (String)rawLightData[0] + " LDR Right" + (String)rawLightData[1] + " Calibrator" + (String)rawLightData[2] + " Average" + (String)processedLightData[0] + " Difference" + (String)processedLightData[1] + "\n");

  if (processedLightData[0] >= threshAmbientLower) {
    if (processedLightData[1] <= threshDifferenceMax) {
      move(0, masterDelayDuration * masterDelayFactor);
    } else {
      findLightSource(rawLightData[0], rawLightData[1], processedLightData[1]);
    }
  } else {
    findLightSource(rawLightData[0], rawLightData[1], processedLightData[1]);
  }
}
