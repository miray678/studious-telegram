#pragma once

#include "mode.h" // Temel uçuş modu sınıfı

class ModeMyNewPlaneMode : public Mode {
public:
    // Üst sınıftaki (Mode) özellikleri miras alıyoruz
    using Mode::Mode;

    // Mod ilk seçildiğinde çalışacak başlangıç fonksiyonu
    bool init(bool ignore_checks) override;

    // Modun ana çalışma döngüsü (Hız kontrolü burada yapılır)
    void run() override;

protected:
    // Yer istasyonunda ve loglarda görünecek isim tanımlamaları
    const char* name() const override { return "MyNewMode"; }
    const char* name4() const override { return "MYNM"; }

    // Uçuş modu numarasını sistemdeki karşılığı olan 27 olarak döndürür
    Number mode_number() const override { return Number(27); }

private:
    // Zaman tabanlı adım (step) kontrolü için gerekli değişkenler
    uint8_t step = 0;
    uint32_t step_start_ms = 0;
};
