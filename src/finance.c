/* Menyimpan transaksi, memeriksa input, dan menghitung uang. */
#include "finance.h"

#include <limits.h>
#include <stdio.h>

/* Array ini menjadi buku catatan transaksi selama aplikasi berjalan. */
int jenis[MAX];
int nominal[MAX];
char keterangan[MAX][100];
int jumlah = 0;

/* Tiga nilai ringkasan yang ditampilkan oleh halaman aplikasi. */
long long totalPemasukan = 0;
long long totalPengeluaran = 0;
long long saldoAkhir = 0;

/* Menghitung ulang ringkasan dengan membaca transaksi satu per satu. */
void hitungRingkasan(void)
{
    int i;

    totalPemasukan = 0;
    totalPengeluaran = 0;

    for (i = 0; i < jumlah; i++) {
        if (jenis[i] == 1) {
            totalPemasukan = totalPemasukan + nominal[i];
        } else if (jenis[i] == 2) {
            totalPengeluaran = totalPengeluaran + nominal[i];
        }
    }

    saldoAkhir = totalPemasukan - totalPengeluaran;
}

/* Membaca digit satu per satu dan menolak huruf atau angka terlalu besar. */
static int parseNominal(char input[])
{
    long long nilai = 0;
    int i = 0;

    if (input[0] == '\0') {
        return -1;
    }

    while (input[i] != '\0') {
        int digit;

        if (input[i] < '0' || input[i] > '9') {
            return -1;
        }

        digit = input[i] - '0';
        if (nilai > (INT_MAX - digit) / 10) {
            return -1;
        }

        nilai = nilai * 10 + digit;
        i++;
    }

    return (int)nilai;
}

/* Memeriksa apakah keterangan punya isi selain spasi. */
static int keteranganTerisi(char text[])
{
    int i = 0;

    while (text[i] != '\0') {
        if (text[i] != ' ' && text[i] != '\t' && text[i] != '\n' &&
            text[i] != '\r' && text[i] != '\v' && text[i] != '\f') {
            return 1;
        }
        i++;
    }

    return 0;
}

/* Memeriksa input bersama dan mengembalikan nominal, atau -1 jika gagal. */
static int validasiUmum(char nominalText[], char deskripsi[],
                        char message[], int messageSize)
{
    int nilai;
    int panjang = 0;

    nilai = parseNominal(nominalText);
    if (nilai < 0) {
        snprintf(message, messageSize, "Input harus berupa angka!");
        return -1;
    }

    if (nilai == 0) {
        snprintf(message, messageSize, "Nominal harus lebih dari 0!");
        return -1;
    }

    while (deskripsi[panjang] != '\0') {
        panjang++;
    }

    if (panjang > 99) {
        snprintf(message, messageSize, "Keterangan maksimal 99 karakter!");
        return -1;
    }

    if (!keteranganTerisi(deskripsi)) {
        snprintf(message, messageSize, "Keterangan tidak boleh kosong!");
        return -1;
    }

    return nilai;
}

/* Menyimpan pemasukan jika kapasitas dan semua isian sudah benar. */
int tambahPemasukan(char nominalText[], char deskripsi[], char message[], int messageSize)
{
    int nilai;
    int i = 0;

    if (jumlah >= MAX) {
        snprintf(message, messageSize, "Data transaksi sudah penuh!");
        return 0;
    }

    nilai = validasiUmum(nominalText, deskripsi, message, messageSize);
    if (nilai < 0) {
        return 0;
    }

    jenis[jumlah] = 1;
    nominal[jumlah] = nilai;

    while (deskripsi[i] != '\0' && i < 99) {
        keterangan[jumlah][i] = deskripsi[i];
        i++;
    }
    keterangan[jumlah][i] = '\0';

    jumlah++;
    snprintf(message, messageSize, "Pemasukan berhasil ditambahkan!");
    return 1;
}

/* Memastikan saldo cukup sebelum menyimpan pengeluaran. */
int tambahPengeluaran(char nominalText[], char deskripsi[], char message[], int messageSize)
{
    int nilai;
    int i = 0;

    hitungRingkasan();

    if (jumlah >= MAX) {
        snprintf(message, messageSize, "Data transaksi sudah penuh!");
        return 0;
    }

    nilai = parseNominal(nominalText);
    if (nilai < 0) {
        snprintf(message, messageSize, "Input harus berupa angka!");
        return 0;
    }

    if (nilai == 0) {
        snprintf(message, messageSize, "Nominal harus lebih dari 0!");
        return 0;
    }

    if ((long long)nilai > saldoAkhir) {
        snprintf(message, messageSize, "Pengeluaran gagal! Saldo tidak mencukupi.");
        return 0;
    }

    while (deskripsi[i] != '\0') {
        i++;
    }
    if (i > 99) {
        snprintf(message, messageSize, "Keterangan maksimal 99 karakter!");
        return 0;
    }

    if (!keteranganTerisi(deskripsi)) {
        snprintf(message, messageSize, "Keterangan tidak boleh kosong!");
        return 0;
    }

    i = 0;
    jenis[jumlah] = 2;
    nominal[jumlah] = nilai;
    while (deskripsi[i] != '\0' && i < 99) {
        keterangan[jumlah][i] = deskripsi[i];
        i++;
    }
    keterangan[jumlah][i] = '\0';

    jumlah++;
    snprintf(message, messageSize, "Pengeluaran berhasil ditambahkan!");
    return 1;
}

/* Menulis angka sebagai Rupiah dan menambahkan titik tiap tiga digit. */
void formatRupiah(long long value, char output[], int outputSize)
{
    char digits[32];
    char formatted[48];
    unsigned long long nilai;
    int digitCount = 0;
    int formattedCount = 0;
    int i;
    int negatif = 0;

    if (value < 0) {
        negatif = 1;
        nilai = (unsigned long long)(-(value + 1)) + 1;
    } else {
        nilai = (unsigned long long)value;
    }

    do {
        digits[digitCount] = (char)('0' + nilai % 10);
        digitCount++;
        nilai = nilai / 10;
    } while (nilai > 0 && digitCount < 32);

    for (i = 0; i < digitCount; i++) {
        if (i > 0 && i % 3 == 0) {
            formatted[formattedCount] = '.';
            formattedCount++;
        }
        formatted[formattedCount] = digits[i];
        formattedCount++;
    }

    if (negatif == 1) {
        formatted[formattedCount] = '-';
        formattedCount++;
    }
    formatted[formattedCount] = '\0';

    for (i = 0; i < formattedCount / 2; i++) {
        char sementara = formatted[i];
        formatted[i] = formatted[formattedCount - 1 - i];
        formatted[formattedCount - 1 - i] = sementara;
    }

    snprintf(output, outputSize, "Rp%s", formatted);
}
