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
