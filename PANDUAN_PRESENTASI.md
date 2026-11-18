# Panduan Presentasi Personal Finance Tracker

Panduan ini dibuat dari kode di `src/main.c`, `src/finance.h`, `src/finance.c`, dan `src/ui.c`. Bahasanya sengaja dibuat sederhana supaya bisa dipakai belajar sekaligus dibaca saat presentasi.

## 1. Gambaran Umum Aplikasi

Personal Finance Tracker adalah aplikasi desktop sederhana untuk mencatat uang masuk dan uang keluar. Program ini dibuat dengan bahasa C untuk melatih dasar pemrograman seperti variabel, array, fungsi, percabangan, dan perulangan; Raylib dan Raygui dipakai untuk membuat jendela dan komponen tampilannya. Pengguna mengisi nominal serta keterangan transaksi, lalu aplikasi memeriksa input, menyimpan transaksi sementara di memori, menghitung saldo, dan menampilkan ringkasan serta riwayatnya.

**Contoh pembukaan presentasi:**

> “Kami membuat Personal Finance Tracker, yaitu aplikasi sederhana untuk mencatat pemasukan dan pengeluaran. Pengguna bisa menambahkan transaksi, melihat saldo dan riwayatnya, serta mencoba validasi seperti nominal yang harus positif dan saldo yang harus cukup sebelum pengeluaran.”

## 2. Analogi Sederhana Cara Kerja Data

Bayangkan ada **buku catatan dengan tiga kolom**: jenis transaksi, nominal, dan keterangan. Buku ini punya 100 baris, dari baris nomor 0 sampai 99.

- `jenis[MAX]` adalah kolom pertama. Angka `1` berarti pemasukan dan `2` berarti pengeluaran.
- `nominal[MAX]` adalah kolom kedua, untuk menyimpan jumlah uang pada baris yang sama.
- `keterangan[MAX][100]` adalah kolom ketiga, untuk menyimpan catatan transaksi, misalnya “Uang saku”. Satu catatan dapat berisi sampai 99 karakter, karena satu ruang tambahan dipakai untuk tanda akhir teks.
- `jumlah` adalah penghitung berapa baris yang sudah terisi. Jika nilainya 2, data yang dipakai berada di indeks 0 dan 1; baris berikutnya akan disimpan di indeks 2.
- `MAX` bernilai 100, jadi jumlah transaksi tidak boleh melebihi 100.

Ketiga kolom itu disebut **array paralel**. Anggap saja seperti buku catatan yang kolom-kolomnya selalu merujuk ke baris yang sama: `jenis[0]`, `nominal[0]`, dan `keterangan[0]` adalah satu transaksi yang sama. Kalau salah satu kolom bergeser, catatannya jadi tidak cocok.

Data aplikasi ini disimpan di memori komputer saat program berjalan. Kode yang dibaca untuk panduan ini belum menyimpan transaksi ke file, jadi transaksi akan mulai kosong lagi ketika aplikasi ditutup lalu dibuka kembali.

**Contoh kalimat presentasi:**

> “Data kami simpan seperti buku catatan dengan tiga kolom. Indeks yang sama menunjukkan satu transaksi, sedangkan variabel `jumlah` memberi tahu berapa baris yang sudah terisi.”

## 3. Pembagian Peran dan Penjelasan Per File

Satu anggota kelompok bisa membawakan satu file. Jika jumlah anggota lebih banyak, bagian `ui.c` bisa dibagi lagi menjadi tampilan utama dan modal/input.

### A. `src/finance.h` — Kesepakatan antarbagian program

File ini seperti **daftar isi dan kesepakatan bersama**. File lain dapat melihat nama data dan fungsi keuangan yang tersedia tanpa perlu menyalin isi perhitungannya.

#### `MAX`, array, dan `jumlah`

- **Tugas utama:** Menyatakan batas transaksi dan data yang digunakan bersama.
- **Alur:** `MAX` menetapkan kapasitas 100. Kata `extern` memberi tahu file lain bahwa data tersebut ada, tetapi tempat penyimpanan aslinya dibuat di `finance.c`.
- **Cara ngomongnya:**
  > “Di header ini kami menyatakan data yang dipakai bersama. `extern` ibarat memberi tahu bagian lain lokasi buku catatannya tanpa membuat buku baru.”

#### `RingkasanKeuangan`

- **Tugas utama:** Menjadi satu wadah untuk tiga hasil hitung: `pemasukan`, `pengeluaran`, dan `saldo`.
- **Alur:** Hasil hitung disimpan pada tiga bagian bernama. Ketiganya memakai `long long` agar total dapat menampung angka lebih besar daripada satu nominal transaksi bertipe `int`.
- **Cara ngomongnya:**
  > “`RingkasanKeuangan` seperti satu kotak yang punya tiga ruang: total pemasukan, total pengeluaran, dan saldo akhir.”

#### Prototipe fungsi

- **Tugas utama:** Memberi tahu nama fungsi, masukan yang dibutuhkan, dan hasil yang dikembalikan.
- **Alur:** `ringkasanKeuangan` menghitung total; `tambahPemasukan` dan `tambahPengeluaran` memvalidasi lalu mencoba menyimpan transaksi; `formatRupiah` mengubah angka menjadi teks Rupiah.
- **Cara ngomongnya:**
  > “Prototipe ini seperti daftar layanan yang tersedia. Isi masing-masing layanan ada di `finance.c`, sedangkan `ui.c` bisa memanggilnya melalui nama yang tercantum di sini.”

### B. `src/main.c` — Pintu masuk program

#### `main`

- **Tugas utama:** Menjadi fungsi pertama yang dijalankan sistem operasi.
- **Alur:** `main` memanggil `jalankanAplikasi`, lalu mengembalikan hasilnya ketika aplikasi selesai.
- **Cara ngomongnya:**
  > “`main` adalah pintu masuk program. Tugasnya sederhana: menyerahkan jalannya aplikasi ke `jalankanAplikasi` yang ada di modul UI.”

#### Deklarasi `jalankanAplikasi`

- **Tugas utama:** Memberi tahu compiler bahwa fungsi tersebut dibuat di file lain.
- **Alur:** Deklarasi menyebutkan bahwa fungsi tidak menerima masukan dan mengembalikan angka hasil. Isi fungsinya berada di `ui.c`.
- **Cara ngomongnya:**
  > “Di sini kami memberi tahu bahwa `jalankanAplikasi` tersedia di bagian UI, jadi `main` dapat memanggilnya.”

### C. `src/finance.c` — Aturan dan perhitungan uang

#### Data transaksi

- **Tugas utama:** Menyediakan tempat penyimpanan sebenarnya untuk array dan penghitung transaksi.
- **Alur:** Semua array dibuat untuk maksimal 100 transaksi; `jumlah` dimulai dari 0 karena belum ada transaksi.
- **Cara ngomongnya:**
  > “Kalau header hanya memberi tahu letak datanya, di file ini array benar-benar dibuat dan penghitung transaksi dimulai dari nol.”

#### `formatRupiah`

- **Tugas utama:** Mengubah angka menjadi tulisan Rupiah dengan titik pemisah ribuan.
- **Alur:** Angka dibaca dari digit paling belakang, titik ditambahkan tiap tiga digit, lalu susunannya dibalik agar urutannya kembali normal. Hasilnya ditulis sebagai teks seperti `Rp1.250.000`.
- **Cara ngomongnya:**
  > “Fungsi ini merapikan angka sebelum ditampilkan, supaya nominal lebih mudah dibaca, misalnya menjadi `Rp1.250.000`.”

#### `ringkasanKeuangan`

- **Tugas utama:** Menghitung total pemasukan, total pengeluaran, dan saldo.
- **Alur:** Kedua total dimulai dari nol. Perulangan `for` memeriksa transaksi satu per satu sampai `jumlah`; jenis 1 ditambahkan ke pemasukan, jenis 2 ke pengeluaran. Saldo adalah pemasukan dikurangi pengeluaran.
- **Cara ngomongnya:**
  > “Fungsi ini membaca semua baris yang sudah terisi. Uang masuk dijumlahkan, uang keluar dijumlahkan, lalu saldo dihitung dari selisih keduanya.”

#### `parseNominal` — fungsi pembantu internal

- **Tugas utama:** Memeriksa apakah teks nominal benar-benar angka yang bisa masuk ke tipe `int`.
- **Alur:** Teks kosong ditolak. Teks diubah menjadi angka; jika ada karakter yang bukan bagian angka, nilainya melewati batas, atau terjadi overflow, fungsi mengembalikan gagal.
- **Cara ngomongnya:**
  > “Fungsi ini seperti pemeriksa tulisan di kolom nominal: isinya harus angka yang utuh dan tidak boleh terlalu besar untuk tempat penyimpanannya.”

#### `keteranganTerisi` — fungsi pembantu internal

- **Tugas utama:** Memastikan keterangan tidak kosong atau hanya berisi spasi.
- **Alur:** Karakter diperiksa satu per satu. Jika ditemukan karakter selain spasi, keterangan dianggap terisi.
- **Cara ngomongnya:**
  > “Pemeriksaan ini membuat input seperti beberapa spasi saja tetap dianggap kosong.”

#### `validasiUmum` — fungsi pembantu internal

- **Tugas utama:** Menjalankan pemeriksaan input yang sama untuk transaksi pemasukan dan pengeluaran.
- **Alur:** Nominal harus angka, nilainya harus lebih dari nol, panjang keterangan maksimal 99 karakter, dan keterangan wajib memiliki isi.
- **Cara ngomongnya:**
  > “Daripada menulis pemeriksaan yang sama berkali-kali, kami mengumpulkannya di `validasiUmum` agar aturan input tetap seragam.”

#### `tambahPemasukan`

- **Tugas utama:** Menambahkan pemasukan jika kapasitas dan inputnya memenuhi aturan.
- **Alur:** Pertama, kapasitas diperiksa. Lalu nominal dan keterangan divalidasi. Jika berhasil, data ditulis pada indeks `jumlah`, jenisnya diisi 1, kemudian `jumlah` dinaikkan satu. Pesan hasil dikirim kembali ke UI.
- **Cara ngomongnya:**
  > “Pemasukan baru disimpan setelah kapasitas dan semua input lolos pemeriksaan. Setelah disimpan, penghitung baris bertambah satu.”

#### `tambahPengeluaran`

- **Tugas utama:** Menambahkan pengeluaran hanya jika saldo mencukupi dan input valid.
- **Alur:** Ringkasan dihitung lebih dulu agar saldo tersedia diketahui. Setelah cek kapasitas dan nominal, nominal dibandingkan dengan saldo. Keterangan lalu diperiksa. Jika semuanya lolos, transaksi disimpan dengan jenis 2 dan `jumlah` bertambah.
- **Cara ngomongnya:**
  > “Sebelum mencatat pengeluaran, program menghitung saldo dari riwayat. Kalau nominal pengeluaran lebih besar dari saldo, transaksi ditolak dan data tidak ditambahkan.”

### D. `src/ui.c` — Tampilan dan interaksi pengguna

File ini mengatur jendela Raylib, warna, teks, kartu, navigasi, modal, dan cara tombol/keyboard merespons. **Logika hitung dan validasi uang tetap dipanggil dari `finance.c`**, sehingga tampilan dan aturan keuangan tidak dicampur.

#### Variabel tampilan dan tema

- **Tugas utama:** Menyimpan warna, teks input, halaman aktif, status, fokus, dan animasi UI.
- **Alur:** Warna disimpan sebagai kumpulan nilai merah-hijau-biru. Variabel seperti `modalMode`, `activePage`, dan `focusedField` mengingat bagian UI yang sedang aktif.
- **Cara ngomongnya:**
  > “Bagian atas `ui.c` menyimpan keadaan tampilan, seperti modal sedang terbuka atau halaman yang dipilih, serta warna yang dipakai aplikasi.”

#### Pembantu gambar: `drawText`, `measureText`, `drawTextCentered`, `drawTextRight`

- **Tugas utama:** Menampilkan dan menata teks menggunakan font aplikasi.
- **Alur:** `drawText` menulis teks; `measureText` mengukur lebarnya; dua fungsi lainnya memakai ukuran itu untuk meratakan teks ke tengah atau ke kanan.
- **Cara ngomongnya:**
  > “Fungsi-fungsi ini membantu menempatkan teks dengan rapi, jadi bagian lain tidak perlu menghitung lebar tulisan sendiri.”

#### `setStatus`

- **Tugas utama:** Menyimpan pesan hasil transaksi untuk ditampilkan sebagai banner.
- **Alur:** Pesan dan jenis hasil—sukses atau gagal—disalin ke variabel UI, lalu banner ditandai agar terlihat.
- **Cara ngomongnya:**
  > “Setelah validasi selesai, fungsi ini menyiapkan pesan yang akan dilihat pengguna, misalnya transaksi berhasil atau saldo tidak cukup.”

#### `drawRounded`, `lerpColor`, dan `drawButton`

- **Tugas utama:** Membuat bentuk bersudut halus dan tombol yang memberi respons visual saat diarahkan kursor.
- **Alur:** `drawRounded` menumpuk dua bidang rounded untuk isi dan tepi. `lerpColor` mencampur warna awal dan warna hover secara bertahap. `drawButton` menggambar tombol, memperbarui animasinya, lalu memberi tahu apakah tombol diklik.
- **Cara ngomongnya:**
  > “Tombol tidak langsung berubah warna secara kasar. Warnanya bergerak bertahap memakai `Lerp`, seperti menggeser warna dari keadaan awal ke keadaan hover.”

#### `drawWalletBadge`, `drawHeader`, `drawSummaryIcon`, dan `drawHistoryIcon`

- **Tugas utama:** Menggambar identitas aplikasi, judul, tombol Keluar, dan ikon untuk navigasi bawah.
- **Alur:** Header menampilkan badge dompet dan judul. Ikon ringkasan dibuat dari garis grafik, sementara ikon riwayat dibuat dari titik dan garis.
- **Cara ngomongnya:**
  > “Bagian ini menangani elemen identitas aplikasi dan ikon. Tombol Keluar membuka dialog konfirmasi, bukan langsung menutup jendela.”

#### `drawBottomNavigation`

- **Tugas utama:** Menggambar tab Ringkasan dan Riwayat di bagian bawah serta menangani kliknya.
- **Alur:** Dua area dengan lebar yang sama digambar. Tab yang aktif diberi warna berbeda; saat diklik, halaman aktif diubah. Pilihan klik dimatikan ketika modal atau dialog keluar sedang terbuka.
- **Cara ngomongnya:**
  > “Navigasi bawah memiliki dua pilihan. Saat Riwayat dipilih, halaman berubah dan posisi gulir riwayat dikembalikan ke awal.”

#### `drawSummaryCard`, `drawStatusBanner`, dan `drawTransactionList`

- **Tugas utama:** Menampilkan ringkasan saldo, pesan hasil, serta kartu transaksi.
- **Alur:** Kartu ringkasan memakai `formatRupiah` untuk menampilkan total. Banner hanya digambar jika ada aksi yang menghasilkan pesan. Daftar menampilkan placeholder saat kosong; halaman Riwayat menerima gulir mouse dan membatasi gambar agar tetap di dalam panel.
- **Cara ngomongnya:**
  > “Tampilan ringkasan dan riwayat mengambil data yang sama dari array transaksi. Perbedaannya, daftar di halaman Riwayat dapat digulir untuk melihat seluruh transaksi.”

#### `openModal`

- **Tugas utama:** Membuka formulir pemasukan atau pengeluaran dan mengatur kondisi awalnya.
- **Alur:** Mode transaksi dipilih, pesan lama dan input dikosongkan, banner status disembunyikan, lalu fokus awal diarahkan ke kolom nominal.
- **Cara ngomongnya:**
  > “Saat formulir dibuka, aplikasi menyiapkan input baru dan langsung memfokuskan kolom nominal agar pengguna bisa mulai mengetik.”

#### `hapusKarakterTerakhir`, `tambahKarakter`, dan `pasteClipboard`

- **Tugas utama:** Mengatur penghapusan, pengetikan, dan tempel teks pada kolom modal.
- **Alur:** Hapus karakter memastikan satu huruf UTF-8 terhapus utuh. Penambahan karakter membatasi nominal hanya ke digit dan menjaga batas ukuran kolom. Tempel clipboard dibaca karakter demi karakter melalui aturan yang sama.
- **Cara ngomongnya:**
  > “Input nominal difilter supaya hanya menerima angka. Kolom keterangan tetap bisa berisi teks, tetapi panjangnya dibatasi kapasitas kolom.”

#### `simpanTransaksi`

- **Tugas utama:** Mengirim isi modal ke fungsi keuangan yang sesuai dan menampilkan hasilnya.
- **Alur:** Jika mode pemasukan aktif, fungsi memanggil `tambahPemasukan`; jika mode pengeluaran, memanggil `tambahPengeluaran`. Pesan hasil menjadi banner. Jika sukses, modal ditutup dan daftar diarahkan ke transaksi terbaru.
- **Cara ngomongnya:**
  > “UI tidak menghitung saldo sendiri. Tombol Simpan meneruskan input ke fungsi keuangan, lalu menampilkan pesan yang dikembalikan fungsi tersebut.”

#### `processModalInput`

- **Tugas utama:** Mengelola keyboard dan fokus selama modal terbuka.
- **Alur:** Klik mouse memilih kolom. Ctrl+V menempel teks, Backspace menghapus, dan karakter yang diketik melewati filter. Tab berpindah kolom; Enter dari nominal berpindah ke keterangan, sedangkan Enter di keterangan menjalankan penyimpanan.
- **Cara ngomongnya:**
  > “Fungsi ini mengatur cara pengguna mengisi modal. Nominal hanya menerima angka, dan tombol Tab atau Enter membantu berpindah tanpa harus selalu memakai mouse.”

#### `drawTextBox` dan `drawModal`

- **Tugas utama:** Menggambar kolom input dan jendela formulir.
- **Alur:** `drawTextBox` memakai `GuiTextBox` untuk tampilan kolom serta menggambar tanda fokus. `drawModal` menggambar lapisan gelap dan kartu putih; untuk pengeluaran, saldo tersedia ikut ditampilkan. Tombol Batal menutup modal, sedangkan Simpan memanggil `simpanTransaksi`.
- **Cara ngomongnya:**
  > “Modal pengeluaran menampilkan saldo sebelum pengguna menyimpan. Dengan begitu pengguna bisa membandingkan nominalnya dengan uang yang tersedia.”

#### `drawSummaryPage` dan `drawHistoryPage`

- **Tugas utama:** Menyusun isi masing-masing halaman.
- **Alur:** Ringkasan menampilkan kartu saldo dan tiga transaksi terbaru. Riwayat menampilkan semua transaksi di panel yang dapat digulir. Keduanya menampilkan jumlah transaksi dan banner jika ada.
- **Cara ngomongnya:**
  > “Halaman Ringkasan memberi gambaran cepat, sedangkan halaman Riwayat dipakai untuk melihat daftar transaksi secara lengkap.”

#### `drawExitDialog`

- **Tugas utama:** Meminta konfirmasi sebelum aplikasi ditutup.
- **Alur:** Dialog menampilkan ucapan terima kasih. Tombol Kembali membatalkan keluar jika dialog dibuka dari tombol aplikasi; tombol Tutup mengakhiri loop aplikasi.
- **Cara ngomongnya:**
  > “Sebelum keluar, program meminta konfirmasi agar pengguna tidak menutup aplikasi tanpa sengaja.”

#### `configureRaygui`

- **Tugas utama:** Menyetel font, ukuran teks, warna, dan garis tepi kontrol Raygui.
- **Alur:** Font aplikasi dipasang, lalu gaya umum dan gaya kotak teks diberi warna serta ukuran yang cocok dengan tema terang.
- **Cara ngomongnya:**
  > “Pengaturan ini membuat kotak input Raygui memiliki tampilan yang konsisten dengan warna aplikasi.”

#### `jalankanAplikasi`

- **Tugas utama:** Menyiapkan jendela dan menjalankan aplikasi sampai pengguna memilih keluar.
- **Alur:** MSAA diaktifkan sebelum jendela dibuat. Font dimuat dan Raygui diatur. Di dalam loop, aplikasi memeriksa permintaan tutup, menghitung ringkasan, memproses input, lalu menggambar halaman dan modal. Setelah loop selesai, font dan jendela dibersihkan.
- **Cara ngomongnya:**
  > “Ini pengatur utama aplikasi. Selama loop berjalan, program terus membaca keadaan transaksi dan menggambar ulang tampilan sampai pengguna mengonfirmasi keluar.”

## 4. Alur Demo Aplikasi Saat Presentasi

1. **Buka aplikasi.** Tunjukkan halaman Ringkasan, saldo awal Rp0, jumlah transaksi 0/100, dan tulisan “Belum ada transaksi.”
2. **Tambah pemasukan.** Klik `+ Pemasukan`, masukkan nominal `500000`, isi keterangan `Uang saku`, lalu pilih Simpan. Tunjukkan pesan berhasil dan saldo yang berubah.
3. **Coba pengeluaran yang gagal.** Klik `- Pengeluaran`, masukkan `600000` dengan keterangan `Belanja`, lalu Simpan. Karena saldo hanya Rp500.000, program menolak transaksi dan menampilkan pesan saldo tidak mencukupi.
4. **Tambah pengeluaran yang berhasil.** Ulangi dengan nominal `125000` dan keterangan `Makan`. Tunjukkan bahwa saldo tersisa Rp375.000.
5. **Coba validasi input (opsional).** Buka pemasukan dan coba nominal kosong atau `0`; tunjukkan bahwa program meminta angka yang valid dan lebih dari nol. Kolom nominal juga menolak huruf.
6. **Lihat riwayat.** Pilih tab Riwayat di navigasi bawah. Tunjukkan transaksi yang berhasil tercatat dan bahwa transaksi gagal tidak masuk daftar. Jika transaksi sudah banyak, coba gulir dengan roda mouse.
7. **Keluar.** Klik Keluar, tunjukkan dialog konfirmasi, lalu pilih Tutup.

## 5. Bocoran Pertanyaan Dosen/Aslab dan Cara Menjawabnya

### 1. Kenapa memakai array terpisah, bukan `struct` untuk setiap transaksi?

> “Program mengikuti bentuk data dasar yang diminta: tiga array paralel. Cara ini membuat hubungan indeks mudah ditunjukkan, misalnya semua data indeks 0 adalah transaksi pertama. Kekurangannya, semua array harus selalu memakai indeks yang sama.”

### 2. Apa yang terjadi kalau data sudah 100?

> “Sebelum menyimpan transaksi, fungsi memeriksa apakah `jumlah` sudah mencapai `MAX`. Kalau sudah 100, transaksi baru ditolak dan pesan ‘Data transaksi sudah penuh!’ ditampilkan.”

### 3. Bagaimana program memastikan saldo cukup sebelum pengeluaran?

> “`tambahPengeluaran` memanggil `ringkasanKeuangan` untuk menghitung saldo dari semua transaksi. Nominal pengeluaran dibandingkan dengan saldo; kalau lebih besar, data tidak disimpan.”

### 4. Mengapa `jumlah` dimulai dari 0 dan kapan nilainya berubah?

> “Saat program mulai belum ada transaksi, jadi `jumlah` bernilai 0. Setelah transaksi berhasil disimpan, `jumlah` bertambah satu. Kalau input gagal, nilainya tidak berubah.”

### 5. Apa fungsi `finance.h` dan kenapa dibutuhkan?

> “Header adalah kesepakatan nama data dan fungsi yang dipakai beberapa file. Dengan begitu `ui.c` dapat memanggil perhitungan di `finance.c` tanpa menyalin kode perhitungannya.”

### 6. Bagaimana empat file ini digabung saat dijalankan?

> “`build.bat` meminta GCC mengompilasi `main.c`, `finance.c`, dan `ui.c` bersama-sama. `finance.h` disertakan oleh file C sebagai daftar deklarasi, bukan dikompilasi sebagai program tersendiri. Hasilnya adalah `finance_tracker.exe`.”

### 7. Apakah transaksi tetap ada setelah aplikasi ditutup?

> “Belum. Transaksi hanya disimpan di array selama aplikasi berjalan. Untuk menyimpannya setelah program ditutup, perlu ditambahkan fitur baca-tulis file, tetapi itu belum termasuk fungsi pada versi ini.”
