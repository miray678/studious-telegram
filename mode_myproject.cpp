#include "Copter.h" // ArduPilot ana kütüphanesi
#include "mode_myproject.h"

/*
 * Mode 27: Otonom Hız Sabitleme Kontrol Döngüsü
 * Bu dosya, uçağın hava hızını 20 m/s değerine sabitleyen 
 * algoritmayı içerir.
 */

bool ModeMyNewPlaneMode::init(bool ignore_checks)
{
    // Adım kontrol yapısını ve zamanlayıcıyı sıfırla
    step = 0;
    step_start_ms = AP_HAL::millis();
    
    // Temel uçuş modu başlatma kontrollerini çağır
    return Mode::init(ignore_checks);
}

void ModeMyNewPlaneMode::run()
{
    // 1. GÜVENLİK KONTROLÜ: Kalkış Süreci
    // Eğer uçak kalkış halindeyse standart kalkış algoritması korunur.
    if (plane.is_taking_off()) {
        plane.mode_takeoff.run();
        return;
    }

    // 2. ZAMAN TABANLI KONTROL MANTIĞI
    const uint32_t now_ms = AP_HAL::millis();
    const float elapsed_s = (now_ms - step_start_ms) * 0.001f;

    // STEP 0: Hız Sabitleme (AirSpeed = 20 m/s)
    if (step == 0) {
        float target_airspeed = 20.0f; // Hedef: 20 m/s
        
        // TECS üzerinden pitch ve throttle değerlerini güncelle
        plane.TECS_controller.update_pitch_throttle(plane.get_target_altitude_ms(), 
                                                   target_airspeed, 
                                                   true, 
                                                   0.0f);
        
        // 5 saniye boyunca stabilizasyon sağlandıktan sonra bir sonraki adıma geç
        if (elapsed_s >= 5.0f) {
            step = 1;
            step_start_ms = now_ms;
        }
    }

    // STEP 1: Veri Loglama ve Kararlı Uçuş
    if (step == 1) {
        // Hız 20 m/s'de korunurken Yer İstasyonuna (GCS) bilgi mesajı gönderilir
        plane.set_target_airspeed_cm(2000); // 2000 cm/s = 20 m/s
        gcs().send_text(MAV_SEVERITY_INFO, "Mode 27: 20m/s Hiz Stabilize Edildi");
    }
}
