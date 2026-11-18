/* Implementasi penyimpanan, validasi, dan perhitungan transaksi. */
#include "finance.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Data transaksi disimpan dalam array paralel sesuai program dasar. */
int jenis[MAX];
int nominal[MAX];
char keterangan[MAX][100];
int jumlah = 0;

/* Memformat nilai uang agar mudah dibaca pada kartu dan riwayat. */
void formatRupiah(long long value, char *output, size_t outputSize)
{
    char digits[32];
    char formatted[48];
    size_t digitCount = 0;
    size_t formattedCount = 0;
    size_t i;
    int negative = value < 0;
    unsigned long long magnitude = negative
        ? (unsigned long long)(-(value + 1)) + 1
        : (unsigned long long)value;

    do {
        digits[digitCount++] = (char)('0' + magnitude % 10);
        magnitude /= 10;
    } while (magnitude > 0 && digitCount < sizeof(digits));

    for (i = 0; i < digitCount; i++) {
        if (i > 0 && i % 3 == 0) formatted[formattedCount++] = '.';
        formatted[formattedCount++] = digits[i];
    }

    if (negative) formatted[formattedCount++] = '-';
    formatted[formattedCount] = '\0';

    for (i = 0; i < formattedCount / 2; i++) {
        char temp = formatted[i];
        formatted[i] = formatted[formattedCount - 1 - i];
        formatted[formattedCount - 1 - i] = temp;
    }

    snprintf(output, outputSize, "Rp%s", formatted);
}

/* Menghitung pemasukan, pengeluaran, dan saldo dari transaksi saat ini. */
void ringkasanKeuangan(RingkasanKeuangan *ringkasan)
{
    int i;
    ringkasan->pemasukan = 0;
    ringkasan->pengeluaran = 0;

    for (i = 0; i < jumlah; i++) {
        if (jenis[i] == 1) {
            ringkasan->pemasukan += nominal[i];
        } else if (jenis[i] == 2) {
            ringkasan->pengeluaran += nominal[i];
        }
    }

    ringkasan->saldo = ringkasan->pemasukan - ringkasan->pengeluaran;
}

/* Mengubah input desimal menjadi int tanpa menerima overflow. */
static int parseNominal(const char *input, int *value)
{
    char *end = NULL;
    long parsed;

    if (input[0] == '\0') return 0;
    errno = 0;
    parsed = strtol(input, &end, 10);
    if (errno == ERANGE || end == input || *end != '\0' || parsed > INT_MAX || parsed < INT_MIN) return 0;
    *value = (int)parsed;
    return 1;
}

/* Memastikan keterangan berisi setidaknya satu karakter non-spasi. */
static int keteranganTerisi(const char *text)
{
    size_t i;
    for (i = 0; text[i] != '\0'; i++) {
        if (!isspace((unsigned char)text[i])) return 1;
    }
    return 0;
}

/* Memeriksa nominal dan keterangan yang sama-sama dibutuhkan kedua transaksi. */
static int validasiUmum(const char *nominalText, const char *deskripsi,
                        int *nilai, char *message, size_t messageSize)
{
    if (!parseNominal(nominalText, nilai)) {
        snprintf(message, messageSize, "Input harus berupa angka!");
        return 0;
    }
    if (*nilai <= 0) {
        snprintf(message, messageSize, "Nominal harus lebih dari 0!");
        return 0;
    }
    if (strlen(deskripsi) > 99) {
        snprintf(message, messageSize, "Keterangan maksimal 99 karakter!");
        return 0;
    }
    if (!keteranganTerisi(deskripsi)) {
        snprintf(message, messageSize, "Keterangan tidak boleh kosong!");
        return 0;
    }
    return 1;
}

/* Menyimpan pemasukan setelah kapasitas dan seluruh input lolos validasi. */
int tambahPemasukan(const char *nominalText, const char *deskripsi, char *message, size_t messageSize)
{
    int nilai;

    if (jumlah >= MAX) {
        snprintf(message, messageSize, "Data transaksi sudah penuh!");
        return 0;
    }
    if (!validasiUmum(nominalText, deskripsi, &nilai, message, messageSize)) return 0;

    jenis[jumlah] = 1;
    nominal[jumlah] = nilai;
    snprintf(keterangan[jumlah], sizeof(keterangan[jumlah]), "%s", deskripsi);
    jumlah++;
    snprintf(message, messageSize, "Pemasukan berhasil ditambahkan!");
    return 1;
}

/* Memeriksa saldo tersedia sebelum menyimpan pengeluaran. */
int tambahPengeluaran(const char *nominalText, const char *deskripsi, char *message, size_t messageSize)
{
    RingkasanKeuangan ringkasan;
    int nilai;

    ringkasanKeuangan(&ringkasan);
    if (jumlah >= MAX) {
        snprintf(message, messageSize, "Data transaksi sudah penuh!");
        return 0;
    }
    if (!parseNominal(nominalText, &nilai)) {
        snprintf(message, messageSize, "Input harus berupa angka!");
        return 0;
    }
    if (nilai <= 0) {
        snprintf(message, messageSize, "Nominal harus lebih dari 0!");
        return 0;
    }
    if ((long long)nilai > ringkasan.saldo) {
        snprintf(message, messageSize, "Pengeluaran gagal! Saldo tidak mencukupi.");
        return 0;
    }
    if (strlen(deskripsi) > 99) {
        snprintf(message, messageSize, "Keterangan maksimal 99 karakter!");
        return 0;
    }
    if (!keteranganTerisi(deskripsi)) {
        snprintf(message, messageSize, "Keterangan tidak boleh kosong!");
        return 0;
    }

    jenis[jumlah] = 2;
    nominal[jumlah] = nilai;
    snprintf(keterangan[jumlah], sizeof(keterangan[jumlah]), "%s", deskripsi);
    jumlah++;
    snprintf(message, messageSize, "Pengeluaran berhasil ditambahkan!");
    return 1;
}
