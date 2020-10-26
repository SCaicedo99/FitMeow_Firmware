const int xInput = A0;
const int yInput = A1;
const int zInput = A2;

// initialize minimum and maximum Raw Ranges for each axis
// 10-bit analog digit convert
int RawMin = 0;
int RawMax = 1023;

// Take multiple samples to reduce noise
const int sampleSize = 10;

void setup() {
  // put your setup code here, to run once:
  analogReference(EXTERNAL);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Read raw values
  // ReadAxis() takes 10 samples of ADC conversions and return the average
  int xRaw = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);

  // Convert raw values to 'milli-Gs"
  // converts the analog output voltages from the sensor to gravitational acceleration(G)
  // The values -3000 and 3000 represents the gravitational acceleration 
  // (in milli-g which is 1/1000 of a g) measured by the sensor 
  // i.e. ±3g (-3000 milli-g to 3000 milli-g).
  long xScaled = map(xRaw, RawMin, RawMax, -3000, 3000);
  long yScaled = map(yRaw, RawMin, RawMax, -3000, 3000);
  long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);

  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;

  Serial.print("X, Y, Z  :: ");
  Serial.print(xRaw);
  Serial.print(", ");
  Serial.print(yRaw);
  Serial.print(", ");
  Serial.print(zRaw);
  Serial.print(" :: ");
  Serial.print(xAccel,0);
  Serial.print("G, ");
  Serial.print(yAccel,0);
  Serial.print("G, ");
  Serial.print(zAccel,0);
  Serial.println("G");

  delay(10);

}

// Take samples and return the average
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
  reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}
