# Panduan Presentasi Personal Finance Tracker

Panduan ini mengikuti nama variabel dan fungsi di `src/main.c`, `src/finance.h`,
`src/finance.c`, dan `src/ui.c`. Bahasanya dibuat santai supaya mudah dipahami
dan bisa dibaca saat presentasi.

## 1. Gambaran Umum Aplikasi

Personal Finance Tracker adalah aplikasi desktop untuk mencatat uang masuk dan
uang keluar. Programnya ditulis dengan bahasa C, sedangkan Raylib dan Raygui
membantu membuat jendela, tombol, dan kolom input. Pengguna memasukkan nominal
dan keterangan, program memeriksa datanya, menyimpan transaksi ke array, lalu
menghitung saldo serta menampilkan ringkasan dan riwayat.

**Contoh pembukaan:**

> “Kami membuat aplikasi pencatat pemasukan dan pengeluaran. Tujuannya supaya
> pengguna bisa melihat jumlah uang masuk, uang keluar, saldo akhir, dan daftar
> transaksi. Kami memakai C untuk logika program dan Raylib serta Raygui untuk
> tampilannya.”

## 2. Analogi Sederhana Cara Kerja Data

Bayangkan ada buku catatan dengan tiga kolom dan 100 baris, bernomor 0 sampai 99.
Setiap baris adalah satu transaksi.

- `jenis[MAX]` adalah kolom jenis. Angka `1` berarti pemasukan dan `2` berarti
  pengeluaran.
- `nominal[MAX]` adalah kolom jumlah uang.
- `keterangan[MAX][100]` adalah kolom catatan, misalnya “Uang saku”. Maksimal
  99 karakter dipakai untuk tulisan; satu tempat tersisa untuk penanda akhir teks.
- `jumlah` adalah penghitung banyaknya baris yang sudah terisi. Jika nilainya 2,
  transaksi ada di indeks 0 dan 1; transaksi baru masuk di indeks 2.
- `MAX` adalah kapasitas buku, yaitu 100 transaksi.

Array disebut **array paralel** karena semua kolom menggunakan nomor baris yang
sama. Contohnya, `jenis[0]`, `nominal[0]`, dan `keterangan[0]` adalah tiga bagian
dari transaksi pertama. Ibarat buku catatan, ketiga kolom itu harus tetap berada
di baris yang sama.

Variabel `totalPemasukan`, `totalPengeluaran`, dan `saldoAkhir` adalah tempat
menyimpan hasil hitung ringkasan. Fungsi `hitungRingkasan` mengisi ulang nilainya
dengan membaca data transaksi.

Data disimpan di memori selama aplikasi berjalan. Saat aplikasi ditutup, isi
array tidak disimpan ke file, jadi ketika dibuka lagi daftar transaksi kembali
kosong.

**Contoh kalimat presentasi:**

> “Kami memakai array seperti buku catatan dengan tiga kolom. Nomor indeks yang
> sama menunjukkan satu transaksi, dan `jumlah` menjadi penghitung baris yang
> sudah terisi.”

## 3. Pembagian Peran dan Penjelasan Per File

Satu anggota kelompok dapat menjelaskan satu file. Kalau anggota lebih banyak,
bagian UI bisa dibagi menjadi tampilan halaman dan modal input.

### A. `src/finance.h` — Daftar data dan fungsi keuangan

Header bisa dianggap sebagai daftar nama data dan layanan yang boleh dipakai
bagian program lain. Tempat penyimpanan array yang sebenarnya dibuat di
`finance.c`.

#### `MAX`, array, `jumlah`, dan total ringkasan

- **Tugas utama:** Menyatakan data bersama yang dipakai aplikasi.
- **Alur:** `MAX` menetapkan kapasitas 100. `jenis`, `nominal`, dan `keterangan`
  menyimpan transaksi. `jumlah` menghitung isinya. Tiga variabel `long long`
  menyimpan total pemasukan, total pengeluaran, dan saldo.
- **Cara ngomongnya:**
  > “Di header kami menyebutkan data yang digunakan bersama. Seperti daftar
  isi, header memberi tahu nama dan bentuk datanya, sementara isi array dibuat
  di `finance.c`.”

#### `hitungRingkasan`, `tambahPemasukan`, `tambahPengeluaran`, `formatRupiah`

- **Tugas utama:** Menyediakan nama fungsi keuangan yang bisa dipanggil UI.
- **Alur:** `hitungRingkasan` menghitung total. Dua fungsi tambah memeriksa dan
  menyimpan transaksi. `formatRupiah` merapikan tampilan nominal.
- **Cara ngomongnya:**
  > “Daftar fungsi ini menjadi penghubung antara tampilan di `ui.c` dan aturan
  transaksi di `finance.c`.”

### B. `src/main.c` — Titik awal program

#### `jalankanAplikasi`

- **Tugas utama:** Nama fungsi yang disediakan oleh modul UI untuk menjalankan
  jendela aplikasi.
- **Alur:** `main.c` memberi tahu compiler bahwa fungsi ini ada di file lain.
- **Cara ngomongnya:**
  > “Fungsi tampilannya ada di `ui.c`. Di sini kami mengenalkan namanya supaya
  > fungsi utama bisa memanggilnya.”

#### `main`

- **Tugas utama:** Menjadi fungsi pertama yang dijalankan komputer.
- **Alur:** `main` memanggil `jalankanAplikasi`, lalu mengembalikan nilai hasil
  ke sistem operasi setelah aplikasi selesai.
- **Cara ngomongnya:**
  > “`main` adalah pintu masuk program. Tugasnya meneruskan jalannya aplikasi ke
  > `jalankanAplikasi`.”

### C. `src/finance.c` — Penyimpanan, pemeriksaan, dan perhitungan

#### Data transaksi

- **Tugas utama:** Membuat array transaksi dan penghitungnya.
- **Alur:** Array disediakan untuk 100 transaksi dan `jumlah` dimulai dari 0.
  Variabel total ringkasan juga dibuat dan dimulai dari 0.
- **Cara ngomongnya:**
  > “Di file ini tempat penyimpanan dibuat. Karena belum ada catatan saat
  > aplikasi baru dibuka, penghitung dan total uang dimulai dari nol.”

#### `hitungRingkasan`

- **Tugas utama:** Menghitung pemasukan, pengeluaran, dan saldo akhir.
- **Alur:** Total pemasukan dan pengeluaran dikosongkan dulu. Perulangan `for`
  membaca transaksi dari indeks 0 sampai `jumlah - 1`. Jenis 1 ditambahkan ke
  pemasukan, jenis 2 ke pengeluaran, lalu saldo dihitung dengan pengurangan.
- **Cara ngomongnya:**
  > “Fungsi ini membaca catatan satu baris demi satu baris. Setelah semua
  > pemasukan dan pengeluaran dijumlahkan, saldo didapat dari pemasukan
  > dikurangi pengeluaran.”

#### `parseNominal`

- **Tugas utama:** Mengubah teks nominal menjadi angka tanpa memakai `strtol`.
- **Alur:** Fungsi membaca karakter satu per satu. Kalau karakter bukan digit
  `0` sampai `9`, input ditolak. Nilai yang terlalu besar untuk tipe `int` juga
  ditolak. Hasil `-1` berarti input tidak valid.
- **Cara ngomongnya:**
  > “Nominal dibaca seperti mengeja angka per digit. Kalau ada huruf atau
  > angkanya terlalu besar, program menolak input tersebut.”

#### `keteranganTerisi`

- **Tugas utama:** Memastikan keterangan tidak kosong atau hanya spasi.
- **Alur:** Karakter diperiksa satu per satu. Jika ditemukan karakter selain
  spasi atau tab, keterangan dianggap terisi.
- **Cara ngomongnya:**
  > “Pemeriksaan ini mencegah pengguna menyimpan keterangan yang isinya hanya
  > spasi.”

#### `validasiUmum`

- **Tugas utama:** Memeriksa input umum yang diperlukan oleh pemasukan dan
  pengeluaran.
- **Alur:** Nominal harus angka dan lebih dari nol. Panjang keterangan dibatasi
  99 karakter dan isinya tidak boleh kosong.
- **Cara ngomongnya:**
  > “Aturan input yang sama dikumpulkan di satu fungsi supaya pemasukan dan
  > pengeluaran mengikuti pemeriksaan yang seragam.”

#### `tambahPemasukan`

- **Tugas utama:** Menyimpan pemasukan jika semua pemeriksaan berhasil.
- **Alur:** Kapasitas diperiksa terlebih dahulu, lalu nominal dan keterangan.
  Jika valid, data disimpan pada indeks `jumlah`, jenis diisi 1, lalu `jumlah`
  ditambah satu.
- **Cara ngomongnya:**
  > “Pemasukan baru dicatat hanya jika masih ada tempat dan inputnya benar.
  > Setelah disimpan, penghitung baris bertambah satu.”

#### `tambahPengeluaran`

- **Tugas utama:** Menyimpan pengeluaran jika saldo cukup dan input valid.
- **Alur:** `hitungRingkasan` dipanggil agar saldo terbaru tersedia. Program
  memeriksa kapasitas, nominal, kecukupan saldo, dan keterangan. Jika semuanya
  benar, transaksi disimpan dengan jenis 2.
- **Cara ngomongnya:**
  > “Sebelum mencatat pengeluaran, program menghitung saldo terlebih dahulu.
  > Kalau uang yang akan dikeluarkan lebih besar dari saldo, transaksi ditolak.”

#### `formatRupiah`

- **Tugas utama:** Menampilkan angka dengan awalan `Rp` dan titik pemisah ribuan.
- **Alur:** Digit angka diambil dari belakang, titik ditambahkan tiap tiga digit,
  lalu susunannya dibalik agar urutannya benar.
- **Cara ngomongnya:**
  > “Fungsi ini merapikan nominal supaya mudah dibaca, misalnya angka
  > `1234567` ditampilkan sebagai `Rp1.234.567`.”

### D. `src/ui.c` — Tampilan dan cara pengguna berinteraksi

UI mengatur jendela, warna, teks, tombol, halaman, dan modal. Perhitungan uang
tetap dilakukan oleh fungsi di `finance.c`.

#### Angka `#define` dan variabel tampilan

- **Tugas utama:** Menamai angka untuk kolom/halaman aktif dan mengingat keadaan
  tampilan.
- **Alur:** `FIELD_NOMINAL` dan nama sejenisnya adalah angka penanda fokus.
  `PAGE_SUMMARY` dan `PAGE_HISTORY` menandai halaman aktif. Variabel lain
  menyimpan isi input, pesan, scroll, dan animasi.
- **Cara ngomongnya:**
  > “Angka penanda ini menggantikan `enum`. Nama seperti `PAGE_HISTORY` membuat
  > kita lebih mudah membaca arti angka yang dipakai program.”

#### `drawText`, `measureText`, `drawTextCentered`, `drawTextRight`

- **Tugas utama:** Menggambar dan mengatur posisi teks.
- **Alur:** Satu fungsi menggambar teks, satu mengukur lebarnya, dan dua lainnya
  memakai ukuran itu untuk meratakan teks ke tengah atau ke kanan.
- **Cara ngomongnya:**
  > “Fungsi pembantu ini menjaga tulisan tetap rapi, jadi posisi teks tidak
  > perlu dihitung ulang di setiap bagian.”

#### `setStatus`, `drawRounded`, `lerpColor`, `getButtonHover`, `setButtonHover`,
`drawButton`

- **Tugas utama:** Menyiapkan pesan dan menggambar bentuk serta tombol.
- **Alur:** Status menyimpan pesan berhasil/gagal. `drawRounded` menggambar
  kartu bersudut bulat. `lerpColor` mencampur warna. Dua fungsi hover memilih
  nilai animasi tombol dari variabel biasa, lalu `drawButton` menggambar tombol
  dan memeriksa klik.
- **Cara ngomongnya:**
  > “Bagian ini menyiapkan tampilan dasar seperti tombol, kartu, dan pesan
  > status. Warna hover berubah bertahap agar transisinya tetap sama seperti
  > sebelumnya.”

#### `drawWalletBadge`, `drawHeader`, `drawSummaryIcon`, `drawHistoryIcon`,
`drawBottomNavigation`

- **Tugas utama:** Menggambar identitas dan navigasi aplikasi.
- **Alur:** Header menampilkan logo sederhana, judul, dan tombol Keluar. Ikon
  ringkasan dan riwayat digambar dari bentuk dasar. Navigasi bawah mengubah
  halaman aktif saat tab dipilih.
- **Cara ngomongnya:**
  > “Bagian ini mengurus header dan navigasi. Pilihan tab hanya mengubah halaman
  > yang terlihat, sementara pencatatan transaksi tetap memakai data yang sama.”

#### `drawSummaryCard`, `drawStatusBanner`, `drawTransactionList`

- **Tugas utama:** Menampilkan saldo, hasil aksi, dan daftar transaksi.
- **Alur:** Kartu saldo mengambil angka dari variabel ringkasan global. Banner
  muncul jika ada pesan. Daftar menampilkan placeholder saat kosong dan dapat
  digulir pada halaman Riwayat.
- **Cara ngomongnya:**
  > “Fungsi-fungsi ini menampilkan data yang sudah dihitung. Daftar riwayat
  > membaca array transaksi dan roda mouse dipakai untuk menggulir daftar.”

#### `openModal`, `hapusKarakterTerakhir`, `tambahKarakter`, `pasteClipboard`

- **Tugas utama:** Menyiapkan formulir dan mengatur teks yang diketik pengguna.
- **Alur:** Modal mengosongkan kolom lalu memfokuskan nominal. Karakter nominal
  dibatasi menjadi digit; keterangan menerima teks sampai kapasitas kolom.
  Backspace menghapus karakter dan paste memakai aturan input yang sama.
- **Cara ngomongnya:**
  > “Ketika formulir dibuka, kolom nominal langsung aktif. Program memeriksa
  > karakter sebelum memasukkannya supaya nominal hanya berisi angka.”

#### `simpanTransaksi`, `processModalInput`

- **Tugas utama:** Menghubungkan tombol/keyboard dengan fungsi penyimpanan.
- **Alur:** Simpan memilih fungsi pemasukan atau pengeluaran sesuai modal.
  Pengelola input membaca klik, tombol Tab, Enter, Backspace, dan paste. Enter
  dari kolom keterangan menjalankan penyimpanan.
- **Cara ngomongnya:**
  > “UI menyerahkan pemeriksaan dan penyimpanan ke `finance.c`. Di sini kami
  > hanya menentukan kapan fungsi itu dipanggil, misalnya saat tombol Simpan
  > diklik atau Enter ditekan.”

#### `drawTextBox`, `drawModal`

- **Tugas utama:** Menggambar kolom input dan kartu modal.
- **Alur:** Kotak teks digambar dengan Raygui dan kolom aktif diberi tanda
  kursor. Modal pengeluaran juga menunjukkan saldo tersedia. Tombol Batal
  menutup modal dan tombol Simpan mencatat transaksi.
- **Cara ngomongnya:**
  > “Modal menjadi tempat mengisi transaksi. Pada pengeluaran, saldo tersedia
  > ditampilkan agar pengguna bisa melihat batas uang yang dapat dipakai.”

#### `drawSummaryPage`, `drawHistoryPage`, `drawExitDialog`

- **Tugas utama:** Menyusun konten dua halaman dan dialog keluar.
- **Alur:** Ringkasan menampilkan kartu saldo dan transaksi terbaru. Riwayat
  menampilkan daftar lengkap yang dapat digulir. Dialog meminta konfirmasi
  sebelum aplikasi ditutup.
- **Cara ngomongnya:**
  > “Ringkasan untuk melihat gambaran cepat, Riwayat untuk memeriksa transaksi,
  > dan dialog keluar mencegah aplikasi tertutup tanpa konfirmasi.”

#### `configureRaygui`, `jalankanAplikasi`

- **Tugas utama:** Mengatur gaya kontrol dan menjalankan jendela utama.
- **Alur:** Raygui diberi font dan warna aplikasi. `jalankanAplikasi` membuat
  jendela, memuat font, menjalankan perulangan gambar dan input, lalu
  membersihkan resource setelah pengguna keluar.
- **Cara ngomongnya:**
  > “`jalankanAplikasi` adalah pengatur tampilan utama. Selama perulangannya
  > berjalan, program memperbarui ringkasan dan menggambar ulang jendela.”

## 4. Alur Demo Aplikasi Saat Presentasi

1. Buka aplikasi dan tunjukkan saldo awal Rp0 serta daftar transaksi yang masih
   kosong.
2. Pilih **+ Pemasukan**. Isi nominal `500000` dan keterangan `Uang saku`, lalu
   simpan. Tunjukkan pesan berhasil dan saldo yang berubah.
3. Pilih **- Pengeluaran**. Coba nominal `600000` dengan keterangan `Belanja`.
   Simpan dan tunjukkan pesan bahwa saldo tidak cukup.
4. Coba lagi pengeluaran `125000` dengan keterangan `Makan`. Tunjukkan saldo
   akhir Rp375.000.
5. Pilih tab **Riwayat**. Tunjukkan bahwa pemasukan dan pengeluaran yang berhasil
   tercatat, tetapi transaksi yang gagal tidak masuk daftar.
6. Klik **Keluar**, lalu pilih **Tutup** pada dialog konfirmasi.

## 5. Pertanyaan Dosen/Aslab dan Cara Menjawab

### 1. Kenapa memakai array terpisah, bukan satu `struct`?

> “Kami memakai array paralel karena tugas meminta data `jenis`, `nominal`, dan
> `keterangan` disimpan terpisah. Indeks yang sama menunjukkan satu transaksi.
> Kekurangannya, semua array harus selalu memakai indeks yang sama.”

### 2. Apa yang terjadi kalau transaksi sudah 100?

> “Sebelum menyimpan, program mengecek `jumlah` terhadap `MAX`. Kalau sudah
> penuh, transaksi baru ditolak dan pesan data penuh ditampilkan.”

### 3. Bagaimana cara menghitung saldo?

> “`hitungRingkasan` membaca semua transaksi. Pemasukan dijumlahkan, pengeluaran
> dijumlahkan, lalu `saldoAkhir` diisi dari pemasukan dikurangi pengeluaran.”

### 4. Bagaimana program memastikan pengeluaran tidak melebihi saldo?

> “`tambahPengeluaran` memanggil `hitungRingkasan` terlebih dahulu. Nominal
> pengeluaran dibandingkan dengan `saldoAkhir`; jika lebih besar, program tidak
> menyimpan transaksi.”

### 5. Apa fungsi `#define` untuk halaman dan kolom?

> “`#define` memberi nama pada angka penanda. Jadi kode lebih mudah dibaca:
> `PAGE_HISTORY` lebih jelas daripada angka 1 tanpa keterangan.”

### 6. Bagaimana empat file C digabung saat compile?

> “`build.bat` meminta GCC mengompilasi `main.c`, `finance.c`, dan `ui.c`
> bersama-sama. File `finance.h` disertakan oleh file C untuk mengenalkan data
> dan fungsi yang dipakai bersama.”

### 7. Kenapa teks nominal diperiksa karakter per karakter?

> “Supaya program hanya menerima digit `0` sampai `9` dan bisa menolak huruf
> atau angka yang terlalu besar untuk tipe `int`.”

### 8. Apakah transaksi tersimpan setelah aplikasi ditutup?

> “Belum. Transaksi hanya ada di array selama program berjalan. Untuk menyimpan
> data setelah aplikasi ditutup, program perlu ditambah fitur baca-tulis file.”
