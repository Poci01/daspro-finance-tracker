#include "finance.h"

#include <stdio.h>
#include <string.h>

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "GAGAL baris %d: %s\n", __LINE__, #condition); \
            return 1; \
        } \
    } while (0)

int main(void)
{
    char message[128];
    char longDescription[101];
    char formatted[64];
    RingkasanKeuangan ringkasan;

    CHECK(jumlah == 0);
    CHECK(!tambahPemasukan("", "Gaji", message, sizeof(message)));
    CHECK(strcmp(message, "Input harus berupa angka!") == 0);
    CHECK(!tambahPemasukan("abc", "Gaji", message, sizeof(message)));
    CHECK(strcmp(message, "Input harus berupa angka!") == 0);
    CHECK(!tambahPemasukan("0", "Gaji", message, sizeof(message)));
    CHECK(strcmp(message, "Nominal harus lebih dari 0!") == 0);
    CHECK(!tambahPemasukan("10", "   ", message, sizeof(message)));
    CHECK(strcmp(message, "Keterangan tidak boleh kosong!") == 0);
    CHECK(!tambahPemasukan("2147483648", "Gaji", message, sizeof(message)));
    CHECK(strcmp(message, "Input harus berupa angka!") == 0);

    memset(longDescription, 'x', 100);
    longDescription[100] = '\0';
    CHECK(!tambahPemasukan("10", longDescription, message, sizeof(message)));
    CHECK(strcmp(message, "Keterangan maksimal 99 karakter!") == 0);
    longDescription[99] = '\0';
    CHECK(tambahPemasukan("1", longDescription, message, sizeof(message)));
    CHECK(strlen(keterangan[0]) == 99);

    CHECK(!tambahPengeluaran("2", "Belanja", message, sizeof(message)));
    CHECK(strcmp(message, "Pengeluaran gagal! Saldo tidak mencukupi.") == 0);
    CHECK(tambahPemasukan("1000", "Gaji", message, sizeof(message)));
    CHECK(tambahPengeluaran("400", "Belanja", message, sizeof(message)));
    CHECK(jenis[1] == 1 && nominal[1] == 1000);
    CHECK(jenis[2] == 2 && nominal[2] == 400);

    ringkasanKeuangan(&ringkasan);
    CHECK(ringkasan.pemasukan == 1001);
    CHECK(ringkasan.pengeluaran == 400);
    CHECK(ringkasan.saldo == 601);
    formatRupiah(1234567, formatted, sizeof(formatted));
    CHECK(strcmp(formatted, "Rp1.234.567") == 0);

    while (jumlah < MAX) {
        CHECK(tambahPemasukan("1", "Transaksi uji", message, sizeof(message)));
    }
    CHECK(!tambahPemasukan("1", "Penuh", message, sizeof(message)));
    CHECK(strcmp(message, "Data transaksi sudah penuh!") == 0);
    CHECK(!tambahPengeluaran("1", "Penuh", message, sizeof(message)));
    CHECK(strcmp(message, "Data transaksi sudah penuh!") == 0);

    puts("Semua tes logika transaksi lulus.");
    return 0;
}
