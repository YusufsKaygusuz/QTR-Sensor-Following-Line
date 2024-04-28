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