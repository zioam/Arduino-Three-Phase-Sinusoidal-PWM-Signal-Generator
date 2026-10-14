#define analog1 9
#define analog2 10
#define analog3 11
#define freq_controller A0
#define led 12

float Vmax;           //0.7368;
float Vcentral;       //0.7349;
float Vmin = -1;           //0.7330;
float wave_period;  // Wave period time in seconds
float var_updT;
float Vcontrol1;  // Wave 1 0°
float Vcontrol2;  // Wave 2 90°
float Vcontrol3;  // Wave 3 -90° / 270°
float delta;      // Wave amplitude
float angle90;    // Amount of updates required to reach 90° degrees.
float OPERATOR;   // Value in which will decrease or increase in each update
float updT_MAX;
int updates;
int i = 0;
int i_caseOrder = 1;
int FcontrollerVal;
int FcontrollerVal_map;


// Convert voltage to PWM value
int VoltageToPWM(float voltage) {
  return float((voltage / 5) * 255);
}

void signal_GEN() {
  do {  // 90°
    // Set the analog outputs
    analogWrite(analog1, VoltageToPWM(Vcontrol1));
    analogWrite(analog2, VoltageToPWM(Vcontrol2));
    analogWrite(analog3, VoltageToPWM(Vcontrol3));

    Vcontrol1 += OPERATOR;
    Vcontrol2 -= OPERATOR;
    Vcontrol3 += OPERATOR;

    i++;
    delay(var_updT);
  } while (i < angle90);



  do {  // 180°
    // Set the analog outputs
    analogWrite(analog1, VoltageToPWM(Vcontrol1));
    analogWrite(analog2, VoltageToPWM(Vcontrol2));
    analogWrite(analog3, VoltageToPWM(Vcontrol3));

    Vcontrol1 -= OPERATOR;
    Vcontrol2 -= OPERATOR;
    Vcontrol3 += OPERATOR;

    i -= 1;
    delay(var_updT);
  } while (i > 0);



  do {  // 270°
    // Set the analog outputs
    analogWrite(analog1, VoltageToPWM(Vcontrol1));
    analogWrite(analog2, VoltageToPWM(Vcontrol2));
    analogWrite(analog3, VoltageToPWM(Vcontrol3));

    Vcontrol1 -= OPERATOR;
    Vcontrol2 += OPERATOR;
    Vcontrol3 -= OPERATOR;

    i -= 1;
    delay(var_updT);
  } while (i > -angle90);



  do {  // 360°
    // Set the analog outputs
    analogWrite(analog1, VoltageToPWM(Vcontrol1));
    analogWrite(analog2, VoltageToPWM(Vcontrol2));
    analogWrite(analog3, VoltageToPWM(Vcontrol3));

    Vcontrol1 += OPERATOR;
    Vcontrol2 += OPERATOR;
    Vcontrol3 -= OPERATOR;

    i++;
    delay(var_updT);
  } while (i < 0);
}

void setup() {
  Serial.begin(9600);

  // Analog output pins
  pinMode(analog1, OUTPUT);
  pinMode(analog2, OUTPUT);
  pinMode(analog3, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    float receivedFloat = Serial.parseFloat();

    switch (i_caseOrder) {
      case 1:
        wave_period = receivedFloat;
        i_caseOrder = 2;
        break;
      case 2:
        Vmax = receivedFloat;
        i_caseOrder = 3;
        break;
      case 3:
        Vmin = receivedFloat;
        i_caseOrder = 1;
        break;
      default:
        Serial.println("Error: Unexpected case, resetting order...");
        i_caseOrder = 1;
        break;
    }
    // Clear the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  Vcentral = (Vmax + Vmin) / 2;
  Vcontrol1 = Vcentral;
  Vcontrol2 = Vmax;
  Vcontrol3 = Vmin;
  delta = Vmax - Vcentral;
  updT_MAX = wave_period * 1000 / 4;
  FcontrollerVal = analogRead(freq_controller);
  FcontrollerVal_map = map(FcontrollerVal, 0, 4064, 30, updT_MAX);  //0-4064 cus depends on the pot.
  var_updT = FcontrollerVal_map;
  updates = wave_period / var_updT * 1000;
  angle90 = updates / 4;
  OPERATOR = delta / angle90;

  if(wave_period > 0 && Vmax > 0 && Vmin != -1){ //Check if the user has successfully set all values for the waves
    signal_GEN(); //If yes, it will start generating the waves
  }
}