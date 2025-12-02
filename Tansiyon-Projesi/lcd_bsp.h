#ifndef LCD_BSP_H
#define LCD_BSP_H

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "lvgl.h"
#include "demos/lv_demos.h"
#include "esp_check.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* --- DIŞARIDAN ÇAĞIRILACAK FONKSİYONLAR --- */

// Ekranı ve LVGL'i başlatan fonksiyon
void lcd_lvgl_Init(void);

// Tansiyon değerini ekrana gönderen fonksiyon
void basinc_guncelle(int deger);

// LVGL Thread güvenliği için kilit fonksiyonları
bool example_lvgl_lock(int timeout_ms);
void example_lvgl_unlock(void);

// Diğer yardımcı fonksiyonlar (Gerekirse diye burada durabilirler ama static olmamalılar)
void example_lvgl_rounder_cb(struct _lv_disp_drv_t *disp_drv, lv_area_t *area);

#ifdef __cplusplus
}
#endif

#endif /* LCD_BSP_H */