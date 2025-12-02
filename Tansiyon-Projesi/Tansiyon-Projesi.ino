#include "lcd_bsp.h" 

const int sensorPin = 7; 

// Buraya kendi kalibrasyon değerini yazmayı unutma!
float voltageZero = 0.205; 

float voltageMaxRef = 1.725;
float pressureMaxRef_mmHg = 200.0;
float slope; 

void setup() {
  Serial.begin(115200);
  slope = pressureMaxRef_mmHg / (voltageMaxRef - voltageZero);
  lcd_lvgl_Init();
  pinMode(sensorPin, INPUT);
}

void loop() {
  // --- 1. SENSÖR OKUMA (HIZLANDIRILMIŞ) ---
  long sumADC = 0;
  // Örnek sayısını düşürdük ama okuma hızını artırdık
  // Delay(1) komutunu kaldırdık çünkü ESP32 çok hızlı okuyabilir.
  int samples = 20; 
  
  for(int i=0; i<samples; i++){
    sumADC += analogRead(sensorPin);
    // Buradaki delay(1)'i kaldırdık!
  }
  float rawADC = sumADC / (float)samples;

  // --- 2. HESAPLAMALAR ---
  float voltage = (rawADC / 4095.0) * 3.3;
  float pressure_mmHg = (voltage - voltageZero) * slope;

  if(pressure_mmHg < 2.0) pressure_mmHg = 0.0;

  // --- 3. EKRANI GÜNCELLEME ---
  if (example_lvgl_lock(-1)) {
    basinc_guncelle((int)pressure_mmHg);
    example_lvgl_unlock();
  }

  // --- 4. BEKLEME SÜRESİNİ KISALTTIK ---
  // Eskiden 50ms idi, şimdi 5ms yaptık.
  // Bu sayede ibre saniyede ~100 kez güncellenebilir (Çok akıcı olur).
  delay(5); 
}