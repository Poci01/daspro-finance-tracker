/* Kontrak data transaksi dan fungsi perhitungan keuangan aplikasi. */
#ifndef FINANCE_H
#define FINANCE_H

#include <stddef.h>

#define MAX 100

/* Array paralel yang menyimpan tipe, nominal, dan keterangan transaksi. */
extern int jenis[MAX];
extern int nominal[MAX];
extern char keterangan[MAX][100];
extern int jumlah;

/* Hasil perhitungan pemasukan, pengeluaran, dan saldo akhir. */
typedef struct {
    long long pemasukan;
    long long pengeluaran;
    long long saldo;
} RingkasanKeuangan;

/* Menghitung total keuangan berdasarkan seluruh transaksi yang tersimpan. */
void ringkasanKeuangan(RingkasanKeuangan *ringkasan);

/* Memvalidasi lalu menambahkan satu transaksi pemasukan. */
int tambahPemasukan(const char *nominalText, const char *deskripsi, char *message, size_t messageSize);

/* Memvalidasi saldo lalu menambahkan satu transaksi pengeluaran. */
int tambahPengeluaran(const char *nominalText, const char *deskripsi, char *message, size_t messageSize);

/* Mengubah angka menjadi teks Rupiah dengan pemisah ribuan. */
void formatRupiah(long long value, char *output, size_t outputSize);

#endif
