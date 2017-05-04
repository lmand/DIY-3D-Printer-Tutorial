#include "thermistortables.h"

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

#define SDPOWER            -1

#define EXTRUDERS 3

#define TEMP_SENSOR_AD595_OFFSET 0.0
#define TEMP_SENSOR_AD595_GAIN   1.0

#define THERMISTORHEATER_0 1
#define THERMISTORHEATER_1 1
#define THERMISTORHEATER_2 1

#define HEATER_0_USES_THERMISTOR 1
#define HEATER_1_USES_THERMISTOR 1
#define HEATER_2_USES_THERMISTOR 1

  static void *heater_ttbl_map[EXTRUDERS] = { (void *)heater_0_temptable
#if EXTRUDERS > 1
                                            , (void *)heater_1_temptable
#endif
#if EXTRUDERS > 2
                                            , (void *)heater_2_temptable
#endif
#if EXTRUDERS > 3
  #error Unsupported number of extruders
#endif
  };
  
    static int heater_ttbllen_map[EXTRUDERS] = { heater_0_temptable_len
#if EXTRUDERS > 1
                                             , heater_1_temptable_len
#endif
#if EXTRUDERS > 2
                                             , heater_2_temptable_len
#endif
#if EXTRUDERS > 3
  #error Unsupported number of extruders
#endif
  };
  
  #define PGM_RD_W(x)   (short)pgm_read_word(&x)

#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9




#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10

#define HEATER_1_PIN       8
#define TEMP_0_PIN         15   // ANALOG NUMBERING
#define TEMP_1_PIN         14   // ANALOG NUMBERING
#define TEMP_2_PIN         13   // ANALOG NUMBERING

void setup() {
  
  pinMode(TEMP_0_PIN  , INPUT);
  pinMode(TEMP_1_PIN  , INPUT);
  pinMode(TEMP_2_PIN  , INPUT);
  
  pinMode(FAN_PIN , OUTPUT);
  pinMode(HEATER_0_PIN , OUTPUT);
  pinMode(HEATER_1_PIN , OUTPUT);
  pinMode(LED_PIN  , OUTPUT);
  
  pinMode(X_STEP_PIN  , OUTPUT);
  pinMode(X_DIR_PIN    , OUTPUT);
  pinMode(X_ENABLE_PIN    , OUTPUT);
  
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);
  
  pinMode(Z_STEP_PIN  , OUTPUT);
  pinMode(Z_DIR_PIN    , OUTPUT);
  pinMode(Z_ENABLE_PIN    , OUTPUT);
  
  pinMode(E_STEP_PIN  , OUTPUT);
  pinMode(E_DIR_PIN    , OUTPUT);
  pinMode(E_ENABLE_PIN    , OUTPUT);
  
  pinMode(Q_STEP_PIN  , OUTPUT);
  pinMode(Q_DIR_PIN    , OUTPUT);
  pinMode(Q_ENABLE_PIN    , OUTPUT);
  
   digitalWrite(X_ENABLE_PIN    , LOW);
    digitalWrite(Y_ENABLE_PIN    , LOW);
    digitalWrite(Z_ENABLE_PIN    , LOW);
    digitalWrite(E_ENABLE_PIN    , LOW);
    digitalWrite(Q_ENABLE_PIN    , LOW);
    Serial.begin(115200);
}



float analog2temp(int raw, uint8_t e) {

  #ifdef HEATER_0_USES_MAX6675
    if (e == 0)
    {
      return 0.25 * raw;
    }
  #endif

  if(heater_ttbl_map[e] != 0)
  {
    float celsius = 0;
    byte i;  
    short (*tt)[][2] = (short (*)[][2])(heater_ttbl_map[e]);

    raw = (1023 * OVERSAMPLENR) - raw;
    for (i=1; i<heater_ttbllen_map[e]; i++)
    {
      if ((PGM_RD_W((*tt)[i][0]) > raw) && ((float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i-1][0])) >0))
      {
        celsius = PGM_RD_W((*tt)[i-1][1]) + 
          (raw - PGM_RD_W((*tt)[i-1][0])) * 
          (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i-1][1])) /
          (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i-1][0]));
        break;
      }
    }

    // Overflow: Set to last value in the table
    if (i == heater_ttbllen_map[e]) celsius = PGM_RD_W((*tt)[i-1][1]);

    return celsius;
  }
  return ((raw * ((5.0 * 100.0) / 1024.0) / OVERSAMPLENR) * TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET;
}

unsigned long prevMillis;

void loop () {
  
  if (millis() %1000 <500) 
    digitalWrite(LED_PIN, HIGH);
  else
   digitalWrite(LED_PIN, LOW);
  
  if (millis() %1000 <300) {
    digitalWrite(HEATER_0_PIN, HIGH);
    digitalWrite(HEATER_1_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
  } else if (millis() %1000 <600) {
    digitalWrite(HEATER_0_PIN, LOW);
    digitalWrite(HEATER_1_PIN, HIGH);
    digitalWrite(FAN_PIN, LOW);
  } else  {
    digitalWrite(HEATER_0_PIN, LOW);
    digitalWrite(HEATER_1_PIN, LOW);
    digitalWrite(FAN_PIN, HIGH);
  }
  
  if (millis() %2000 <1000) {
    digitalWrite(X_DIR_PIN    , HIGH);
    digitalWrite(Y_DIR_PIN    , HIGH);
    digitalWrite(Z_DIR_PIN    , HIGH);
    digitalWrite(E_DIR_PIN    , HIGH);
    digitalWrite(Q_DIR_PIN    , HIGH);
  }
  else {
    digitalWrite(X_DIR_PIN    , LOW);
    digitalWrite(Y_DIR_PIN    , LOW);
    digitalWrite(Z_DIR_PIN    , LOW);
    digitalWrite(E_DIR_PIN    , LOW);
    digitalWrite(Q_DIR_PIN    , LOW);
  }
  
  
    digitalWrite(X_STEP_PIN    , HIGH);
    digitalWrite(Y_STEP_PIN    , HIGH);
    digitalWrite(Z_STEP_PIN    , HIGH);
    digitalWrite(E_STEP_PIN    , HIGH);
    digitalWrite(Q_STEP_PIN    , HIGH); 
  delay(1);
  
  
  
    
    digitalWrite(X_STEP_PIN    , LOW);
    digitalWrite(Y_STEP_PIN    , LOW);
    digitalWrite(Z_STEP_PIN    , LOW);
    digitalWrite(E_STEP_PIN    , LOW);
    digitalWrite(Q_STEP_PIN    , LOW); 
    
    if (millis() -prevMillis >500){
    prevMillis=millis();
    int t = analogRead( TEMP_0_PIN);
    Serial.print("T0 ");
    Serial.print(t);
    Serial.print("/");
    Serial.print(analog2temp(1024 - t,0),0);
    
    Serial.print(" T1 ");
    t = analogRead( TEMP_1_PIN);
    Serial.print(t);
    Serial.print("/");
    Serial.print(analog2temp(1024 - t,1),0);
    
    Serial.print(" T2 ");
    t = analogRead( TEMP_2_PIN);
    Serial.print(t);
    Serial.print("/");
    Serial.println(analog2temp(1024 - t,2),0);
    
  }
    
}
