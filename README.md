# SimClever Tansiyon Monitor

ESP32-S3 ve LVGL kullanılarak geliştirilmiş, yüksek çözünürlüklü AMOLED ekrana sahip dijital tansiyon (basınç) monitörü.

⚠️ **Durum:** Geliştirme aşamasındadır (Work in Progress)

## 🚀 Özellikler

- **Donanım:** Waveshare ESP32-S3-Touch-AMOLED-1.43
- **Sensör:** XGZP6847 Basınç Sensörü (0-40kPa / 0-300mmHg)
- **Arayüz:** LVGL kütüphanesi ile tasarlanmış hibrit kadran (Analog İbre + Dijital Gösterge)
- **Akıllı Renk Bölgeleri:**
  - 0 - 80 mmHg: 🟢 Yeşil (Güvenli)
  - 80 - 160 mmHg: 🔴 Kırmızı (Dikkat/Risk - Ekranda Mavi kodlanarak düzeltildi)
  - 160+ mmHg: ⚫ Siyah (Yüksek)

## 🛠️ Donanım Bağlantıları

Projede I2C pin çakışmasını önlemek amacıyla sensör çıkışı standart SDA pini yerine GPIO 7 üzerine alınmıştır.

| XGZP6847 Sensör | ESP32-S3 (Waveshare) | Açıklama |
|-----------------|----------------------|----------|
| VCC | 3.3V | I2C Soketinden alınabilir |
| GND | GND | I2C Soketinden alınabilir |
| OUT (Sinyal) | GPIO 7 | ⚠️ **ÖNEMLİ:** Kartın kenarındaki pad/delik kullanılmalı. (GPIO 6 dokunmatik ile çakışıyor) |

## 📂 Proje Yapısı
```
SimClever-Tansiyon-Monitor/
├── Tansiyon-Projesi/       # Ana Proje Dosyaları
│   ├── Tansiyon-Projesi.ino # Ana Arduino kodu
│   ├── lcd_bsp.c           # Ekran ve LVGL arayüz kodları
│   ├── lcd_bsp.h           # Başlık dosyaları
│   └── ...                 # Diğer sürücü dosyaları
├── Tools/
│   └── calibration/        # Sadece sensör kalibrasyonu için basit kod
└── README.md               # Proje dokümantasyonu
```

## ⚙️ Kritik Ayarlar (LVGL Config)

Bu projeyi derlemeden önce `lv_conf.h` dosyasında şu ayarların yapıldığından emin olun:

### 1. Renk Düzeltmesi
```c
LV_COLOR_16_SWAP -> 1  // Renklerin doğru görünmesi için şart
```

### 2. Fontlar
```c
LV_FONT_MONTSERRAT_14 -> 1  // Genel kullanım
LV_FONT_MONTSERRAT_20 -> 1  // Kadran sayıları için
LV_FONT_MONTSERRAT_48 -> 1  // Orta dijital gösterge için
```
