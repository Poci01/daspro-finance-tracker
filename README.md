# Personal Finance Tracker

Aplikasi desktop untuk mencatat pemasukan dan pengeluaran, dibuat dengan C, Raylib,
dan Raygui.

## Menjalankan aplikasi

Di Windows, jalankan `finance_tracker.exe` (bisa dengan klik dua kali atau melalui
PowerShell):

```powershell
.\finance_tracker.exe
```

## Compile ulang

Pastikan GCC MinGW tersedia di `PATH`, lalu jalankan:

```bat
build.bat
```

## Isi proyek

- `src/`: kode aplikasi dan logika keuangan.
- `include/` dan `lib/`: header Raylib/Raygui serta library Raylib.
- `assets/`: font Inter dan lisensinya.
- `PANDUAN_PRESENTASI.md`: bahan belajar dan contekan presentasi.
- `tests/`: tes fungsi keuangan; jalankan dengan `test.bat`.
