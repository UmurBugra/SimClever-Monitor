#include <Arduino.h>

/*
  XGZP6847A 
  Hassasiyet: ~0.1 mmHg
*/

const int sensorPin = 36;

// --- GÜNCELLENMİŞ KALİBRASYON DEĞERLERİ(TAM KALIBRE DEGIL) ---

// 1. ADIM: Sıfır Noktası (Boşta okunan voltaj)

float voltageZero = 0.186; 

// 2. ADIM: Tepe Noktası (200 mmHg basıldığında okunan voltaj)

float voltageMaxRef = 1.725;
float pressureMaxRef_mmHg = 200.0;

// EĞİM HESABI (Otomatik)
// y = mx + b
float slope = pressureMaxRef_mmHg / (voltageMaxRef - voltageZero);

void setup() {
  Serial.begin(115200);
  Serial.println("XGZP6847A Baslatiliyor...");
}

void loop() {
  // Gürültü Filtreleme (20 okuma ortalaması)
  long sumADC = 0;
  int samples = 20;
  
  for(int i=0; i<samples; i++){
    sumADC += analogRead(sensorPin);
    delay(2);
  }
  float rawADC = sumADC / (float)samples;

  // 1. Voltaj Hesabı
  float voltage = (rawADC / 4095.0) * 3.3;

  // 2. Basınç Hesabı (mmHg)
  float pressure_mmHg = (voltage - voltageZero) * slope;

  // 3. Basınç Hesabı (kPa)
  float pressure_kPa = pressure_mmHg * 0.133322;

  // Küçük negatif sapmaları ve 0.3 mmHg altındaki gürültüyü sıfırla
  if(pressure_mmHg < 0.3) {
    pressure_mmHg = 0.0;
    pressure_kPa = 0.0;
  }

  // Sonuçları Yazdır
  Serial.print("V: ");
  Serial.print(voltage, 3);
  Serial.print(" | mmHg: ");
  Serial.print(pressure_mmHg, 1);
  Serial.print(" | kPa: ");
  Serial.println(pressure_kPa, 2);

  delay(200); 
}