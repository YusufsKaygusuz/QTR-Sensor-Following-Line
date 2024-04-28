#include <QTRSensors.h>  // QTR sensör kütüphanesi ekleniyor

#include r2y.h // Hız sınırlarını 
#include PID.h // PID Sensör
#include calibration.h // Kalibrasyon 
#include YUZEY.h // Yüzey Analizi | Siyah-Beyaz Tespiti

#define NUM_SENSORS             6  // Kullanılan sensör sayısı
#define NUM_SAMPLES_PER_SENSOR  4  // Her sensör için örnekleme sayısı
#define EMITTER_PIN QTR_NO_EMITTER_PIN  // Emitter pini tanımlanıyor

boolean cizgi = 1;  // İzlenen çizgi rengi (1=beyaz, 0=siyah)
// bool cizgi = 1;

QTRSensorsAnalog qtra((unsigned char[]) {  // QTR sensörleri tanımlanıyor
  A0, A1, A2, A3, A4, A5
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];  // Sensör değerlerini tutacak dizi

#define M_AIN1 9   // Sağ motorun ileri yön pinleri
#define M_AIN2 8   // Sağ motorun geri yön pinleri
#define M_APWM 3   // Sağ motorun PWM pini

#define M_BIN1 10  // Sol motorun ileri yön pinleri
#define M_BIN2 11  // Sol motorun geri yön pinleri
#define M_BPWM 5   // Sol motorun PWM pini

double sagMax = 255;  // Sağ motorun maksimum hızı
double solMax = 255;  // Sol motorun maksimum hızı
double sagBase ;  // Sağ motorun çizgi üzerindeki varsayılan hızı
double solBase ;  // Sol motorun çizgi üzerindeki varsayılan hızı
double motorHiz = 0;  // Motor hızı
double sagmotorHiz = 0;  // Sağ motor hızı
double solmotorHiz = 0;  // Sol motor hızı

int hata;  // Çizgi izleme hatası
double setP = 2500;  // Hedef çizgi konumu
double Kpigment = 0.075;  // P-katsayısı
double Kdeniz = 0.952;  // D-katsayısı
uint16_t say;  // Zaman sayacı
int SonHata = 0;  // Son hata değeri

void setup() {
  KALIBRASYON();  // Sensör kalibrasyonu yapılıyor
}

void loop() {
  YUZEY();  // Yüzey durumu kontrol ediliyor
  PID();  // PID hesaplamaları yapılıyor
}

void sag_geri (int h) {
  analogWrite(M_APWM, h);  // Sağ motorun PWM sinyali ayarlanıyor
  digitalWrite(M_AIN1, 1);  // Sağ motorun geri yönü aktif ediliyor
  digitalWrite(M_AIN2, 0);  // Sağ motorun ileri yönü pasif ediliyor
}

void sol_geri (int h) {
  analogWrite(M_BPWM, h);  // Sol motorun PWM sinyali ayarlanıyor
  digitalWrite(M_BIN1, 0);  // Sol motorun ileri yönü pasif ediliyor
  digitalWrite(M_BIN2, 1);  // Sol motorun geri yönü aktif ediliyor
}

void sag_ileri(int h) {
  analogWrite(M_APWM, h);  // Sağ motorun PWM sinyali ayarlanıyor
  digitalWrite(M_AIN1, 0);  // Sağ motorun ileri yönü aktif ediliyor
  digitalWrite(M_AIN2, 1);  // Sağ motorun geri yönü pasif ediliyor
}

void sol_ileri (int h) {
  analogWrite(M_BPWM, h);  // Sol motorun PWM sinyali ayarlanıyor
  digitalWrite(M_BIN1, 1);  // Sol motorun ileri yönü aktif ediliyor
  digitalWrite(M_BIN2, 0);  // Sol motorun geri yönü pasif ediliyor
}

KALIBRASYON();

PID();

YUZEY();

r2y();


/*
void KALIBRASYON() {
  for (int i = 0; i < 200; i++) {
    qtra.calibrate();  // Sensörlerin kalibrasyonu yapılıyor
  }
}
*/

/*
void PID() {
  unsigned int position = qtra.readLine(sensorValues, 1, cizgi);  // Sensörlerden okuma yapılıyor
  hata = position - setP;  // Hata hesaplanıyor

  // P ve D katsayıları belirleniyor
  if (abs(hata) >= 2500) {
    Kpigment = 1;
    Kdeniz = 0;
    sagBase = 0;
    solBase = 0;
  } else if (hata >= -2500 && hata <= 2500) {
    Kpigment = 0.075;
    Kdeniz = 0.952;
    sagBase = 200;
    solBase = 200;
  }

  if (micros() - say > 10000) {
    motorHiz = Kpigment * hata + Kdeniz * (hata - SonHata);  // Motor hızı hesaplanıyor
    SonHata = hata;  // Son hata güncelleniyor
    sagmotorHiz = sagBase - motorHiz;  // Sağ motor hızı belirleniyor
    solmotorHiz = solBase + motorHiz;  // Sol motor hızı belirleniyor
    r2y();  // Motor hızlarına göre hareket ettiriliyor
    say = micros();  // Zaman sayacı güncelleniyor
  }
}
*/

/*
void YUZEY() {
  // Eğer tüm sensörler çizgiyi görmüyorsa izlenen yüzey beyazdır
  if (sensorValues[0] >= 700 && sensorValues[1] >= 700 && sensorValues[4] >= 700 && sensorValues[5] > 700) {
    cizgi = 1;
  }
  // Eğer tüm sensörler çizgiyi görmüyorsa izlenen yüzey siyahtır
  if (sensorValues[0] <= 50 && sensorValues[1] <= 50 && sensorValues[4] <= 50 && sensorValues[5] <= 50) {
    cizgi = 0;
  }
}
*/

/*
void r2y() {
  // Sağ motor hızı sınırları kontrol ediliyor
  if (sagmotorHiz > sagMax) {
    sagmotorHiz = sagMax;
  }
  // Sağ motor geri hareket ediyorsa pozitif değere dönüştürülüyor
  if (sagmotorHiz < 0) {
    sag_geri(abs(sagmotorHiz));
  } else {
    sag_ileri(sagmotorHiz);
  }
  // Sol motor hızı sınırları kontrol ediliyor
  if (solmotorHiz > solMax) {
    solmotorHiz = solMax;
  }
  // Sol motor geri hareket ediyorsa pozitif değere dönüştürülüyor
  if (solmotorHiz < 0) {
    sol_geri(abs(solmotorHiz));
  } else {
    sol_ileri(solmotorHiz);
  }
}
*/
