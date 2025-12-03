#include "lcd_bsp.h" 

/* --- PIN TANIMLAMALARI --- */
const int sensorPin = 7;   // Basınç Sensörü (IO7)
const int bataryaPin = 4;  // Pil Voltajı (IO4 - Dahili Voltaj Bölücü)

/* --- KALİBRASYON DEĞERLERİ --- */
float voltageZero = 0.205; // Buraya kendi kalibrasyon değerini yaz!
float voltageMaxRef = 1.725;
float pressureMaxRef_mmHg = 200.0;
float slope; 

/* --- ZAMANLAYICILAR --- */
unsigned long sonPilOkuma = 0; // Pili sürekli okuyup işlemciyi yormayalım

void setup() {
  Serial.begin(115200);
  
  // Basınç Eğim Hesabı
  slope = pressureMaxRef_mmHg / (voltageMaxRef - voltageZero);
  
  // Ekranı Başlat
  lcd_lvgl_Init();
  
  // Pin Modları
  pinMode(sensorPin, INPUT);
  pinMode(bataryaPin, INPUT); // Pil okuma pini
}

void loop() {
  // ==========================================
  // 1. BASINÇ SENSÖRÜNÜ OKUMA (Hızlı olmalı)
  // ==========================================
  long sumADC = 0;
  int samples = 20; 
  
  for(int i=0; i<samples; i++){
    sumADC += analogRead(sensorPin);
  }
  float rawADC = sumADC / (float)samples;

  float voltage = (rawADC / 4095.0) * 3.3;
  float pressure_mmHg = (voltage - voltageZero) * slope;

  // Negatif ve parazit değerleri temizle
  if(pressure_mmHg < 2.0) {
    pressure_mmHg = 0.0;
  }

  // ==========================================
  // 2. PİL DURUMUNU OKUMA (Saniyede 1 kez yeterli)
  // ==========================================
  if(millis() - sonPilOkuma > 1000) {
    sonPilOkuma = millis();
    
    // Waveshare ESP32-S3-Touch-AMOLED-1.43 Şemasına göre:
    // Pil voltajı 200K ve 100K dirençler ile bölünür (1/3 Oranı).
    // Yani okunan değer x 3 = Gerçek Pil Voltajı.
    
    int rawBat = analogRead(bataryaPin);
    float batVolt = (rawBat / 4095.0) * 3.3 * 3.0; // Çarpı 3 (Voltaj Bölücü)
    
    // Lityum Polimer (LiPo) Pil Yüzde Hesabı
    // 3.00V = %0 (Boş)
    // 4.20V = %100 (Dolu)
    int yuzde = map(batVolt * 100, 300, 420, 0, 100);
    
    // Sınırlandırma (Yüzde 0-100 dışına çıkmasın)
    yuzde = constrain(yuzde, 0, 100);

    // Ekrana Gönder (Kilit mekanizması ile)
    if (example_lvgl_lock(-1)) {
      pil_guncelle(yuzde);
      example_lvgl_unlock();
    }
    
    // Debug için seri porta yazdıralım
    Serial.print("Pil Voltaj: ");
    Serial.print(batVolt);
    Serial.print("V | Yuzde: %");
    Serial.println(yuzde);
  }

  // ==========================================
  // 3. EKRANI GÜNCELLEME (İbre Akıcılığı için)
  // ==========================================
  if (example_lvgl_lock(-1)) {
    basinc_guncelle((int)pressure_mmHg);
    example_lvgl_unlock();
  }

  delay(5); // Çok kısa bekleme (Akıcılık için)
}