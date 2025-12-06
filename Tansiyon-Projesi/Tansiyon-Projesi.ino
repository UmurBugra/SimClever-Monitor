#include "lcd_bsp.h" 

/* --- PIN TANIMLAMALARI --- */
const int sensorPin = 7;   // Basınç Sensörü
const int bataryaPin = 4;  // Pil Voltajı (IO4)

/* --- BATARYA AYARLARI (GÜNCELLENDİ) --- */
// Formül: Mevcut_Offset * (Seri_Volt / Multimetre_Volt)
#define Measurement_offset 0.990476 

#define MIN_VOLTAGE 3.40  
#define MAX_VOLTAGE 4.20  

/* --- BASINÇ KALİBRASYON --- */
float voltageZero = 0.205; 
float voltageMaxRef = 1.725;
float pressureMaxRef_mmHg = 200.0;
float slope; 

unsigned long sonPilOkuma = 0; 

/* --- PİL OKUMA FONKSİYONU --- */
float BAT_Get_Volts(void)
{
  int Volts = analogReadMilliVolts(bataryaPin); 
  float BAT_analogVolts = (float)(Volts * 3.0 / 1000.0) / Measurement_offset;
  return BAT_analogVolts;
}

/* --- YÜZDE HESAPLAMA --- */
float getBatteryPercentage(float voltage) {
    if (voltage > MAX_VOLTAGE) voltage = MAX_VOLTAGE;
    if (voltage < MIN_VOLTAGE) voltage = MIN_VOLTAGE;

    float percentage = ((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0;
    return percentage;
}

void setup() {
  Serial.begin(115200);
  slope = pressureMaxRef_mmHg / (voltageMaxRef - voltageZero);
  
  lcd_lvgl_Init();
  
  pinMode(sensorPin, INPUT);
}

void loop() {
  // --- BASINÇ OKUMA ---
  long sumADC = 0;
  int samples = 20; 
  for(int i=0; i<samples; i++) sumADC += analogRead(sensorPin);
  float rawADC = sumADC / 20.0;
  float voltage = (rawADC / 4095.0) * 3.3;
  float pressure_mmHg = (voltage - voltageZero) * slope;
  if(pressure_mmHg < 2.0) pressure_mmHg = 0.0;

  // --- PİL OKUMA ---
  if(millis() - sonPilOkuma > 1000) {
    sonPilOkuma = millis();
    
    // Voltajı al
    float batVolt = BAT_Get_Volts();
    
    // Yüzdeyi hesapla
    float percentFloat = getBatteryPercentage(batVolt);
    int yuzde = (int)percentFloat; 

    // Ekrana Gönder
    if (example_lvgl_lock(-1)) {
      pil_guncelle(yuzde);
      example_lvgl_unlock();
    }
    Serial.print("Pil Voltaj: "); Serial.print(batVolt, 3);
    Serial.print("V | Yuzde: %"); Serial.println(yuzde);
  }

  // --- EKRAN GÜNCELLEME ---
  if (example_lvgl_lock(-1)) {
    basinc_guncelle((int)pressure_mmHg);
    example_lvgl_unlock();
  }
  delay(5); 
}