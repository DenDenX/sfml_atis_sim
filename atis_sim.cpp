#include <SFML/Graphics.hpp> // grafik kutuphanesi  
#include <iostream>         // terminalden bilgi almak icin gerekli kutuphane   
#include <sstream>          // stringi floata ceviren kutuphane
#include <cmath>

using namespace std;
using namespace sf;

const double PI = 3.14159; // daha dogru hesaplar icin burayi guncelle!
bool         CRTC_ERR = false;

// top atis verilerini tutmak icin ufak bir yapi
struct Hiz {
    double Vx = -1;
    double Vy = -1;
    double olcek = 0; // atisin ekrana sigmasi icin olceklendirme gerekiyor.
    double atisSuresi=0; // baslangic pozisyonu 400 den buyuk oldugu icin ekstra bir komut lzim.
};

string float2string(double deger, string ibare) {
    stringstream ss;
    ss << deger << ibare;
    return ss.str();
}

// sin ve cos ile islem ypabilmek icin dereceden radyana ceviren fonksiyon
double dgr2rad(double degree){
    return degree * PI / 180.0d;
}

// baslangic degerlerini aliyoruz
void getvalues(Hiz &baslangicHizlari){
    short int secenek = 0;

    double g = 10; // yercekimi ivmesini kendimiz belirliyoruz. Bunu siz gelistirebilirsiniz.
    cout << "Hiz : (1)Ayrik, (2) Bilesik: ";
    cin >> secenek;

    switch (secenek)
    {
    case 1: // ayrik olarak hizlari aliyoruz
        cout << "\nVx = ";
        cin >> baslangicHizlari.Vx;
        cout << "Vy = ";
        cin >> baslangicHizlari.Vy;

        break;
    case 2: // bilesik olarak hizlari aliyoruz
        double hiz, derece;
        cout << "\nAtis hizi  = ";
        cin >> hiz;
        cout <<   "Atis acisi = ";
        cin >> derece;
        derece = dgr2rad(derece);

        baslangicHizlari.Vx = cos(derece) * hiz;
        baslangicHizlari.Vy = sin(derece) * hiz;

        
        break;

    default:
        cerr << "\nHATA!: Hatali tercih.'n" << flush;
        CRTC_ERR = true; 
        break;
        
    }

    if (baslangicHizlari.Vy == 0){
            cerr << "\nHATA!: Dikey bilesendeki hiz 0m/s olamaz\nn" << flush;
            CRTC_ERR = true;    
        }
    if (baslangicHizlari.Vx < 0){
            cerr << "\nHATA!: Top yorungesi cercevenin disinda. \nYatay bilesendeki hizi minimum 0 olarak ayarlayin.\nYa da atis acisini 90 - 0 derece arasinda tutun.\n" << flush;
            CRTC_ERR = true;    
        }

    // olcegi ayarliyoruz.

    double t = baslangicHizlari.Vy / g * 2.0f;
    double Xmax = t * baslangicHizlari.Vx;
    double Hmax = baslangicHizlari.Vy * t / 2.0f;
    baslangicHizlari.atisSuresi = t / 2.0f;

    double sclw = Xmax / 900.0f; //cercevenin genisligini 900 olarak aldik
    double sclh = Hmax / 370.0f; // cercevenin boyunu 370 olarak aldik

    baslangicHizlari.olcek = max(sclh, sclw); // hangi ilcek daha buyukse onu almamiz gerekiyor.
    cout << "Olcek =  " << baslangicHizlari.olcek << "  (Otomatik)" << endl;
}

// Topun belirtilen sure icinde ne kadar yer degistirmesi gerektigini hesaplayan fonksiyon
// ayni zamanda dusey bilesendeki hizi da ayarliyor.
Vector2f adimHesapla(double &Vx, double &Vy, double &scl, double g, Time &firstTime, Time &lastTime){

    Time time_tmp = lastTime - firstTime;
    double time = time_tmp.asMilliseconds() / 1000.0f;
    double *tmp = new double(Vy); // dusey bilesendeki hizi ayarlamak icin...


    // formuller
    Vy = Vy - (g * time);
    double Xy = (Vy + *tmp) * time / 2.0f;
    double Xx = Vx * time;

    delete tmp; // bellekte yer kaplamasini istemiyoruz. bu satiri silerseniz bellek hatasi alabilirsiniz.
    return Vector2f(Xx / scl, -Xy / scl); // olceklendirilmis hareketi gonderiyoruz.

}

int main() {
    Hiz baslangicHizlari; // struct imizi olusturduk.
    getvalues(baslangicHizlari); // verilerimizi structimiza yerlestirdik.

    if (CRTC_ERR) {
        return -1;
    }

    Time firstTime = seconds(0); // zaman icindeki degisimi alacagiz.
    Time lastTime = seconds(0); // bu yuzden iki farkli zaman tutucu lazim.



    RenderWindow window(VideoMode(1000, 450), "Atis Simulasyonu"); // penceremizi olusturduk.
    window.setFramerateLimit(45); //fps limitimizi ayarliyoruz
    /* 
        NOT: FPS limitini cok yuksek ayarlamaniz durumunda program zamani daha az kesin olcer.
             FPS limitini cok dusuk (10 - 15) ayarlamaniz ise programin daha kesin calismasini saglar.
             Ancak bu durumda topun hareketinde aksamalar (yere dusmeme) gorulebilir.
             Optimum degeri bulmak icin cihazinizda birkac deneme yapmalisiniz.
             Bu deger genel olarak 40 - 80 fps civarindadir.
    */
    
    Event event; //program makinadaki olaylari dinliyor. Bu sadece RenderWindow'u acik tutmak icin kullaniliyor.
                // gelistirmek isterseniz pek cok sey ekleyebilirsiniz.
    
    //simulasyonda kullanilacak fontu belirliyoruz, Varsayilan olarak font yuklu degil
    Font mainFont; 
    mainFont.loadFromFile("./src/f.ttf");
    // Cerceve cizimi
    RectangleShape frame; 
    frame.setSize(Vector2f(940, 380));
    frame.setFillColor(Color::Transparent);
    frame.setOutlineThickness(1);
    frame.setPosition(Vector2f(35,30));
    // Top cizimi
    CircleShape ball;
    ball.setRadius(5);
    ball.setFillColor(Color::Cyan);
    ball.setPosition(Vector2f(45, 400));


    // Yatay bilesendeki hiz icin cubuk cizimi
    Vertex yatayBilesen[] = {
        Vertex(Vector2f(47.5, 425)),
        Vertex(Vector2f(47.5, 425))
    };
    // dikey bilesendeki hiz icin cubuk cizimi
    Vertex dikeyBilesen[] = {
        Vertex(Vector2f(20, 402.5)),
        Vertex(Vector2f(20, 402.5))
    };

    // Zaman degeri
    Text zamanBilgisi;
    zamanBilgisi.setFont(mainFont);
    zamanBilgisi.setFillColor(Color::White);
    zamanBilgisi.setPosition(Vector2f(30, 5));
    zamanBilgisi.setCharacterSize(15);

    Text HataPayi;
    HataPayi.setFont(mainFont);
    HataPayi.setFillColor(Color::White);
    HataPayi.setPosition(Vector2f(700, 5));
    HataPayi.setCharacterSize(15);
    HataPayi.setString("Hata Payi: %1 ( +- %0,5 )");


    Text MenzilBilgisi;
    MenzilBilgisi.setFont(mainFont);
    MenzilBilgisi.setFillColor(Color::White);
    MenzilBilgisi.setCharacterSize(10);
    MenzilBilgisi.setString("test");

    Text YukseklikBilgisi;
    YukseklikBilgisi.setFont(mainFont);
    YukseklikBilgisi.setFillColor(Color::White);
    YukseklikBilgisi.setCharacterSize(10);
    YukseklikBilgisi.setString("test");

    bool adimHesaplaON = true; // programi sonlandiracak deger
    Vector2f adim;  // topu ne kadar hareket ettirecegimizin kaydini tutan vektor
    Clock clock;    // saatimizi baslattik

// ___________________________________________________________________________________________________
// ___________________________________________________________________________________________________

    while (window.isOpen()){

        // pencereyi acik tutan event zimbirtilari
        while (window.pollEvent(event)){
            if (event.type == Event::Closed)
                window.close();
        }



        firstTime = lastTime; // zaman farki
        lastTime = clock.getElapsedTime(); // guncek zaman


        zamanBilgisi.setString(float2string(lastTime.asSeconds(), "s")); //zaman bilgisini duzenliyoruz


        // Program hala calisiyorsa..
        if (adimHesaplaON) {
            
            adim = adimHesapla(baslangicHizlari.Vx, baslangicHizlari.Vy, baslangicHizlari.olcek, 9.8, firstTime, lastTime);
        }
        //Top yere dusmemisse..
        if (ball.getPosition().y + adim.y + 2.5 <= 400 || lastTime.asSeconds() < baslangicHizlari.atisSuresi ){

                window.clear(); // sifirla


                ball.move(adim); // topu hareket ettir
                window.draw(ball); // topu ciz

                    // Topun pozisyonlarina gore dikey ve yatay cubuklari ayarliyoruz
                Vector2f ballPositions = ball.getPosition();

                double yty_blsn_tmp = ballPositions.x + 2.5f;
                double dky_blsn_tmp = ballPositions.y + 2.5f;

                yatayBilesen[1] = Vertex(Vector2f(yty_blsn_tmp, 425));
                dikeyBilesen[1] = Vertex(Vector2f(20, dky_blsn_tmp));

                MenzilBilgisi.setPosition(Vector2f((20 + yty_blsn_tmp)/2, 440));
                YukseklikBilgisi.setPosition(Vector2f(40, dky_blsn_tmp));

                MenzilBilgisi.setString(float2string((yty_blsn_tmp - 47.5)* baslangicHizlari.olcek, "m"));
                YukseklikBilgisi.setString(float2string((400 - dky_blsn_tmp) * baslangicHizlari.olcek, "m"));


                window.draw(zamanBilgisi); // zaman bilgisini ciz
                window.draw(HataPayi);
                window.draw(yatayBilesen, 2, Lines); //yatay bilesen cubugunu ciz
                window.draw(dikeyBilesen, 2, Lines); // dikey bilesen cubugunu ciz
                window.draw(YukseklikBilgisi);
                window.draw(MenzilBilgisi);

                window.draw(frame); // cerceveyi ciz
        // top yere dusmusse
        }else{
            adimHesaplaON = false;
        }

        //pencereyi goster
        window.display();
        
    }
}