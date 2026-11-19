/* Daftar data dan fungsi keuangan yang dipakai oleh aplikasi. */
#ifndef FINANCE_H
#define FINANCE_H

#define MAX 100

/* Data transaksi disimpan dalam tiga array dengan nomor baris yang sama. */
extern int jenis[MAX];
extern int nominal[MAX];
extern char keterangan[MAX][100];
extern int jumlah;

/* Hasil ringkasan dihitung ulang dari seluruh transaksi. */
extern long long totalPemasukan;
extern long long totalPengeluaran;
extern long long saldoAkhir;

/* Menghitung total pemasukan, pengeluaran, dan saldo akhir. */
void hitungRingkasan(void);

/* Memeriksa input lalu menyimpan satu transaksi pemasukan. */
int tambahPemasukan(char nominalText[], char deskripsi[], char message[], int messageSize);

/* Memeriksa saldo dan input lalu menyimpan satu transaksi pengeluaran. */
int tambahPengeluaran(char nominalText[], char deskripsi[], char message[], int messageSize);

/* Mengubah angka menjadi teks Rupiah dengan pemisah ribuan. */
void formatRupiah(long long value, char output[], int outputSize);

#endif
