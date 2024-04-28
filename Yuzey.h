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