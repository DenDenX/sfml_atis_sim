# 2 Boyutlu Atış Simülasyonu

Bu proje, SFML (Simple and Fast Multimedia Library) kullanılarak geliştirilmiş bir 2 boyutlu atış simülasyonudur. Kullanıcı, başlangıç hızını ve açısını belirleyerek topun hareketini gözlemleyebilir.

## Özellikler

- Kullanıcı, yatay ve dikey bileşenleri ayrı ayrı veya birleşik olarak hız ve açı ile girebilir.
- Topun hareketi, yer çekimi etkisi altında simüle edilir.
- Topun hareketi sırasında menzil ve yükseklik bilgileri ekranda gösterilir.
- Zaman bilgisi ve hata payı gösterimi.

## Gereksinimler

- C++ derleyicisi
- SFML kütüphanesi

## Kurulum

1. SFML kütüphanesini indirin ve kurun. [SFML İndir](https://www.sfml-dev.org/download.php)
2. Bu projeyi klonlayın veya indirin:
    ```sh
    git clone https://github.com/kullanici_adi/sfml-atis-sim.git
    ```
3. `main.cpp` dosyasını derleyin:
    ```sh
    g++ main.cpp -o atis_simulasyonu -lsfml-graphics -lsfml-window -lsfml-system
    ```

## Kullanım

1. Programı çalıştırın:
    ```sh
    ./atis_simulasyonu
    ```

2. Program açıldığında, kullanıcıdan hız ve açı bilgileri istenir:
    - Yatay ve dikey hız bileşenlerini ayrı ayrı girebilir veya
    - Birleşik hız ve açı girebilir.

3. Simülasyon başladıktan sonra, topun hareketini ekranda gözlemleyebilirsiniz. 

## Kod Açıklaması

### Ana Yapılar ve Fonksiyonlar

- `struct Hiz`: Başlangıç hızlarını ve ölçeklendirme faktörünü tutar.
- `float2string`: Double türündeki değeri stringe çevirir.
- `dgr2rad`: Dereceyi radyana çevirir.
- `getvalues`: Kullanıcıdan başlangıç hız ve açı bilgilerini alır.
- `adimHesapla`: Topun hareketini hesaplar ve yeni pozisyonunu döner.

### Ana Döngü

- SFML kullanarak bir pencere oluşturulur ve her karede topun yeni pozisyonu hesaplanarak ekrana çizilir.
- Zaman bilgisi, menzil ve yükseklik bilgileri ekranda gösterilir.
- Top yere düştüğünde simülasyon durur.

## Katkıda Bulunma

Katkıda bulunmak isterseniz, lütfen bir fork yapın ve pull request gönderin. Her türlü katkıya açığız!
