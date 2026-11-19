/* Modul antarmuka Raylib/Raygui untuk navigasi, formulir, dan riwayat. */
#include "finance.h"

#include <stdbool.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma GCC diagnostic pop

/* Ukuran jendela aplikasi. */
#define WINDOW_WIDTH 450
#define WINDOW_HEIGHT 800

/* Angka pengenal untuk kolom input yang sedang aktif. */
#define FIELD_NONE 0
#define FIELD_NOMINAL 1
#define FIELD_KETERANGAN 2

/* Angka pengenal untuk halaman yang sedang dibuka. */
#define PAGE_SUMMARY 0
#define PAGE_HISTORY 1

/* Nomor yang dipakai tombol untuk memilih nilai animasi masing-masing. */
#define HOVER_EXIT 1
#define HOVER_INCOME 2
#define HOVER_EXPENSE 3
#define HOVER_MODAL_CANCEL 4
#define HOVER_MODAL_SAVE 5
#define HOVER_EXIT_BACK 6
#define HOVER_EXIT_CONFIRM 7

/* Menyimpan warna utama agar konsisten dengan tema Moniyers. */
static const Color COLOR_PAGE = {247, 249, 248, 255};
static const Color COLOR_SURFACE = {255, 255, 255, 255};
static const Color COLOR_TEXT = {23, 35, 33, 255};
static const Color COLOR_MUTED = {120, 133, 129, 255};
static const Color COLOR_BORDER = {231, 236, 233, 255};
static const Color COLOR_PRIMARY = {15, 157, 138, 255};
static const Color COLOR_PRIMARY_DARK = {8, 124, 110, 255};
static const Color COLOR_INCOME = {30, 158, 90, 255};
static const Color COLOR_EXPENSE = {229, 72, 77, 255};

static Font appFont;
static char statusMessage[128] = "";
static int statusKind;
static bool statusVisible;
static char modalMessage[128];
static int modalKind;
static char nominalInput[20];
static char keteranganInput[100];
static int modalMode;
static int focusedField;
static int activePage;
static float historyScroll;
static bool shouldClose;
static bool showExitDialog;
static bool windowCloseRequested;
static float overlayAlpha;
static float hoverExit;
static float hoverIncome;
static float hoverExpense;
static float hoverModalCancel;
static float hoverModalSave;
static float hoverExitBack;
static float hoverExitConfirm;
static float hoverSummaryTab;
static float hoverHistoryTab;

/* Menggambar teks dengan font Inter agar hasilnya halus pada ukuran kecil. */
static void drawText(const char text[], float x, float y, float size, Color color)
{
    DrawTextEx(appFont, text, (Vector2){x, y}, size, 0.4f, color);
}

/* Mengukur lebar teks untuk perataan dan posisi nilai. */
static float measureText(const char text[], float size)
{
    return MeasureTextEx(appFont, text, size, 0.4f).x;
}

/* Menggambar teks tepat di tengah area yang disediakan. */
static void drawTextCentered(const char text[], float x, float y, float width, float size, Color color)
{
    drawText(text, x + (width - measureText(text, size)) / 2.0f, y, size, color);
}

/* Menggambar teks dengan sisi kanan sejajar pada koordinat tertentu. */
static void drawTextRight(const char text[], float right, float y, float size, Color color)
{
    drawText(text, right - measureText(text, size), y, size, color);
}

/* Menyimpan pesan validasi terakhir dan jenis warnanya. */
static void setStatus(const char message[], int kind)
{
    snprintf(statusMessage, sizeof(statusMessage), "%s", message);
    statusKind = kind;
    statusVisible = true;
}

/* Membuat sudut halus dan border dengan dua lapis bidang rounded. */
static void drawRounded(Rectangle bounds, float roundness, Color fill, Color border, float borderSize)
{
    Rectangle inner;

    DrawRectangleRounded(bounds, roundness, 24, border);
    inner = (Rectangle){
        bounds.x + borderSize,
        bounds.y + borderSize,
        bounds.width - borderSize * 2,
        bounds.height - borderSize * 2
    };
    if (inner.width > 0 && inner.height > 0) {
        DrawRectangleRounded(inner, roundness, 24, fill);
    }
}

/* Menginterpolasi warna tiap kanal untuk animasi hover tombol. */
static Color lerpColor(Color start, Color end, float amount)
{
    return (Color){
        (unsigned char)Lerp((float)start.r, (float)end.r, amount),
        (unsigned char)Lerp((float)start.g, (float)end.g, amount),
        (unsigned char)Lerp((float)start.b, (float)end.b, amount),
        (unsigned char)Lerp((float)start.a, (float)end.a, amount)
    };
}

/* Mengambil nilai animasi hover tombol berdasarkan nomor tombolnya. */
static float getButtonHover(int buttonId)
{
    if (buttonId == HOVER_EXIT) return hoverExit;
    if (buttonId == HOVER_INCOME) return hoverIncome;
    if (buttonId == HOVER_EXPENSE) return hoverExpense;
    if (buttonId == HOVER_MODAL_CANCEL) return hoverModalCancel;
    if (buttonId == HOVER_MODAL_SAVE) return hoverModalSave;
    if (buttonId == HOVER_EXIT_BACK) return hoverExitBack;
    return hoverExitConfirm;
}

/* Menyimpan nilai animasi hover tombol tanpa memakai pointer. */
static void setButtonHover(int buttonId, float value)
{
    if (buttonId == HOVER_EXIT) hoverExit = value;
    else if (buttonId == HOVER_INCOME) hoverIncome = value;
    else if (buttonId == HOVER_EXPENSE) hoverExpense = value;
    else if (buttonId == HOVER_MODAL_CANCEL) hoverModalCancel = value;
    else if (buttonId == HOVER_MODAL_SAVE) hoverModalSave = value;
    else if (buttonId == HOVER_EXIT_BACK) hoverExitBack = value;
    else hoverExitConfirm = value;
}

/* Menggambar tombol dengan transisi hover dan klik berbasis koordinat Raylib. */
static int drawButton(Rectangle bounds, const char label[], Color fill, Color textColor,
                      float fontSize, int buttonId)
{
    bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    Color hoverColor;
    float hoverProgress = getButtonHover(buttonId);
    float lerpAmount = Clamp(GetFrameTime() * 12.0f, 0.0f, 1.0f);

    if (fill.r > 240) hoverColor.r = 255;
    else hoverColor.r = (unsigned char)(fill.r + 15);
    if (fill.g > 240) hoverColor.g = 255;
    else hoverColor.g = (unsigned char)(fill.g + 15);
    if (fill.b > 240) hoverColor.b = 255;
    else hoverColor.b = (unsigned char)(fill.b + 15);
    hoverColor.a = fill.a;

    if (hovered) {
        hoverProgress = Lerp(hoverProgress, 1.0f, lerpAmount);
    } else {
        hoverProgress = Lerp(hoverProgress, 0.0f, lerpAmount);
    }

    setButtonHover(buttonId, hoverProgress);
    drawRounded(bounds, 0.27f, lerpColor(fill, hoverColor, hoverProgress), fill, 1);
    drawTextCentered(label, bounds.x, bounds.y + (bounds.height - fontSize) / 2.0f - 1.0f,
                     bounds.width, fontSize, textColor);
    return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

/* Menggambar badge dompet pada sisi kiri header. */
static void drawWalletBadge(void)
{
    drawRounded((Rectangle){18, 19, 42, 42}, 0.32f, COLOR_PRIMARY, COLOR_PRIMARY_DARK, 1);
    drawRounded((Rectangle){27, 30, 24, 17}, 0.2f, WHITE, WHITE, 0.5f);
    drawRounded((Rectangle){30, 27, 20, 7}, 0.3f, (Color){199, 245, 234, 255},
                (Color){199, 245, 234, 255}, 0.5f);
    DrawCircle(45, 38, 2.3f, COLOR_PRIMARY_DARK);
}

/* Menggambar identitas aplikasi dan tombol untuk membuka dialog keluar. */
static void drawHeader(void)
{
    drawWalletBadge();
    drawText("Personal Finance Tracker", 70, 24, 16, COLOR_TEXT);
    drawText("Catatan Keuangan Harian", 70, 46, 12, COLOR_MUTED);
    if (modalMode == 0 && !showExitDialog &&
        drawButton((Rectangle){376, 23, 57, 34}, "Keluar", COLOR_SURFACE, COLOR_MUTED, 11,
                   HOVER_EXIT)) {
        showExitDialog = true;
    }
}

/* Menggambar ikon grafik sederhana untuk tab Ringkasan. */
static void drawSummaryIcon(float centerX, float top, Color color)
{
    DrawLineEx((Vector2){centerX - 10, top + 18}, (Vector2){centerX + 10, top + 18}, 2, color);
    DrawLineEx((Vector2){centerX - 7, top + 15}, (Vector2){centerX - 7, top + 10}, 3, color);
    DrawLineEx((Vector2){centerX, top + 15}, (Vector2){centerX, top + 5}, 3, color);
    DrawLineEx((Vector2){centerX + 7, top + 15}, (Vector2){centerX + 7, top + 1}, 3, color);
}

/* Menggambar ikon daftar sederhana untuk tab Riwayat. */
static void drawHistoryIcon(float centerX, float top, Color color)
{
    int row;
    for (row = 0; row < 3; row++) {
        float y = top + 4 + (float)row * 7;
        DrawCircle((int)(centerX - 9), (int)y, 1.7f, color);
        DrawLineEx((Vector2){centerX - 4, y}, (Vector2){centerX + 9, y}, 2, color);
    }
}

/* Menyediakan navigasi bawah dua tab dengan label dan animasi hover. */
static void drawBottomNavigation(bool interactive)
{
    Rectangle leftHitbox = {0, 730, 225, 70};
    Rectangle rightHitbox = {225, 730, 225, 70};
    Rectangle leftPill = {33, 733, 159, 61};
    Rectangle rightPill = {258, 733, 159, 61};
    Vector2 mouse = GetMousePosition();
    float lerpAmount = Clamp(GetFrameTime() * 12.0f, 0.0f, 1.0f);
    Color activeFill = {230, 246, 242, 255};
    Color inactiveFill = COLOR_SURFACE;
    Color leftBase = inactiveFill;
    Color rightBase = inactiveFill;
    Color leftInk = COLOR_MUTED;
    Color rightInk = COLOR_MUTED;
    bool leftHovered = CheckCollisionPointRec(mouse, leftHitbox);
    bool rightHovered = CheckCollisionPointRec(mouse, rightHitbox);

    if (activePage == PAGE_SUMMARY) {
        leftBase = activeFill;
        leftInk = COLOR_PRIMARY_DARK;
    }
    if (activePage == PAGE_HISTORY) {
        rightBase = activeFill;
        rightInk = COLOR_PRIMARY_DARK;
    }

    DrawRectangle(0, 730, WINDOW_WIDTH, 70, COLOR_SURFACE);
    DrawLine(0, 730, WINDOW_WIDTH, 730, COLOR_BORDER);

    if (leftHovered) hoverSummaryTab = Lerp(hoverSummaryTab, 1.0f, lerpAmount);
    else hoverSummaryTab = Lerp(hoverSummaryTab, 0.0f, lerpAmount);
    if (rightHovered) hoverHistoryTab = Lerp(hoverHistoryTab, 1.0f, lerpAmount);
    else hoverHistoryTab = Lerp(hoverHistoryTab, 0.0f, lerpAmount);
    drawRounded(leftPill, 0.22f, lerpColor(leftBase, activeFill, hoverSummaryTab), leftBase, 0.5f);
    drawRounded(rightPill, 0.22f, lerpColor(rightBase, activeFill, hoverHistoryTab), rightBase, 0.5f);

    drawSummaryIcon(112.5f, 738, leftInk);
    drawHistoryIcon(337.5f, 741, rightInk);
    drawTextCentered("Ringkasan", leftHitbox.x, 771, leftHitbox.width, 11, leftInk);
    drawTextCentered("Riwayat", rightHitbox.x, 771, rightHitbox.width, 11, rightInk);

    if (interactive && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (leftHovered) {
            activePage = PAGE_SUMMARY;
        } else if (rightHovered) {
            activePage = PAGE_HISTORY;
            historyScroll = 0;
        }
    }
}

/* Menggambar saldo akhir, total masuk, total keluar, dan kapasitas data. */
static void drawSummaryCard(float y)
{
    Rectangle card = {18, y, 414, 202};
    char balanceText[64];
    char incomeText[64];
    char expenseText[64];
    char countText[48];

    formatRupiah(saldoAkhir, balanceText, sizeof(balanceText));
    formatRupiah(totalPemasukan, incomeText, sizeof(incomeText));
    formatRupiah(totalPengeluaran, expenseText, sizeof(expenseText));
    snprintf(countText, sizeof(countText), "%d / %d transaksi", jumlah, MAX);

    DrawRectangleRounded((Rectangle){card.x, card.y + 3, card.width, card.height},
                         0.12f, 24, Fade(BLACK, 0.04f));
    drawRounded(card, 0.12f, COLOR_PRIMARY, COLOR_PRIMARY_DARK, 1.5f);

    drawText("Saldo akhir", 36, y + 18, 13, (Color){226, 249, 244, 255});
    drawRounded((Rectangle){318, y + 16, 98, 24}, 0.45f, (Color){255, 255, 255, 33},
                (Color){255, 255, 255, 33}, 0.5f);
    drawTextCentered(countText, 320, y + 22, 94, 9.5f, (Color){235, 255, 250, 255});
    drawText(balanceText, 36, y + 45, 32, WHITE);

    DrawLine(36, (int)y + 92, 414, (int)y + 92, (Color){255, 255, 255, 75});
    drawRounded((Rectangle){30, y + 104, 191, 81}, 0.17f, (Color){255, 255, 255, 25},
                (Color){255, 255, 255, 25}, 0.5f);
    drawRounded((Rectangle){227, y + 104, 191, 81}, 0.17f, (Color){255, 255, 255, 25},
                (Color){255, 255, 255, 25}, 0.5f);
    DrawCircle(48, (int)y + 124, 5, (Color){167, 243, 220, 255});
    drawText("Total Pemasukan", 60, y + 117, 12, (Color){220, 250, 238, 255});
    drawText(incomeText, 42, y + 145, 16, WHITE);
    DrawCircle(245, (int)y + 124, 5, (Color){255, 180, 174, 255});
    drawText("Total Pengeluaran", 257, y + 117, 12, (Color){255, 228, 225, 255});
    drawText(expenseText, 239, y + 145, 16, WHITE);
}

/* Menampilkan status transaksi dengan warna sukses, gagal, atau netral. */
static void drawStatusBanner(float y)
{
    Color tint = {233, 245, 242, 255};
    Color accent = COLOR_PRIMARY_DARK;

    if (!statusVisible) return;

    if (statusKind > 0) {
        tint = (Color){226, 246, 235, 255};
        accent = COLOR_INCOME;
    } else if (statusKind < 0) {
        tint = (Color){255, 235, 235, 255};
        accent = COLOR_EXPENSE;
    }

    drawRounded((Rectangle){18, y, 414, 43}, 0.25f, tint, tint, 0.5f);
    DrawCircle(36, (int)y + 21, 4, accent);
    drawText(statusMessage, 49, y + 12, 12, accent);
}

/* Menggambar seluruh riwayat atau transaksi terbaru di area yang dibatasi. */
static void drawTransactionList(Rectangle area, int firstIndex, int itemCount, bool scrollable)
{
    int i;
    float contentHeight = (float)itemCount * 72.0f;
    float maxScroll = 0.0f;
    Vector2 mouse = GetMousePosition();

    if (contentHeight > area.height - 12) {
        maxScroll = contentHeight - area.height + 12;
    }

    if (scrollable && CheckCollisionPointRec(mouse, area)) {
        historyScroll -= GetMouseWheelMove() * 48.0f;
    } else {
        GetMouseWheelMove();
    }
    if (historyScroll < 0) historyScroll = 0;
    if (historyScroll > maxScroll) historyScroll = maxScroll;

    DrawRectangleRounded((Rectangle){area.x, area.y + 3, area.width, area.height},
                         0.07f, 24, Fade(BLACK, 0.04f));
    drawRounded(area, 0.07f, COLOR_SURFACE, COLOR_BORDER, 1);

    BeginScissorMode((int)area.x + 1, (int)area.y + 1, (int)area.width - 2, (int)area.height - 2);
    if (itemCount == 0) {
        drawTextCentered("Belum ada transaksi.", area.x, area.y + area.height / 2 - 9,
                         area.width, 14, COLOR_MUTED);
    } else {
        for (i = 0; i < itemCount; i++) {
            int transactionIndex = firstIndex + i;
            float itemY = area.y + 8.0f + (float)i * 72.0f;
            Rectangle item = {area.x + 8, itemY, area.width - 16, 64};
            Color kindColor = COLOR_EXPENSE;
            Color kindTint = {255, 237, 236, 255};
            const char *kindLabel = "Pengeluaran";
            const char *sign = "-";
            char amountText[64];
            char signedAmount[72];
            char indexLabel[16];
            int amountWidth;

            if (scrollable) {
                itemY = itemY - historyScroll;
                item.y = itemY;
            }
            if (jenis[transactionIndex] == 1) {
                kindColor = COLOR_INCOME;
                kindTint = (Color){232, 247, 238, 255};
                kindLabel = "Pemasukan";
                sign = "+";
            }

            if (itemY + item.height < area.y || itemY > area.y + area.height) continue;
            DrawRectangleRounded((Rectangle){item.x, item.y + 2, item.width, item.height},
                                 0.12f, 24, Fade(BLACK, 0.025f));
            drawRounded(item, 0.12f, (Color){250, 252, 251, 255}, COLOR_SURFACE, 1);
            DrawCircle((int)(item.x + 23), (int)(item.y + 23), 15, kindTint);
            snprintf(indexLabel, sizeof(indexLabel), "#%d", transactionIndex + 1);
            drawTextCentered(indexLabel, item.x + 8, item.y + 17, 30, 11, kindColor);

            drawText(kindLabel, item.x + 47, item.y + 9, 12, kindColor);
            drawText(keterangan[transactionIndex], item.x + 47, item.y + 34, 12, COLOR_MUTED);
            formatRupiah(nominal[transactionIndex], amountText, sizeof(amountText));
            snprintf(signedAmount, sizeof(signedAmount), "%s%s", sign, amountText);
            amountWidth = (int)measureText(signedAmount, 12);
            drawText(signedAmount, item.x + item.width - amountWidth - 10, item.y + 12, 12, kindColor);
        }
    }
    EndScissorMode();
}

/* Membuka formulir dan langsung memfokuskan kolom nominal. */
static void openModal(int mode)
{
    modalMode = mode;
    statusVisible = false;
    modalMessage[0] = '\0';
    modalKind = 0;
    nominalInput[0] = '\0';
    keteranganInput[0] = '\0';
    focusedField = FIELD_NOMINAL;
}

/* Menghapus satu karakter terakhir, termasuk satu karakter UTF-8 utuh. */
static void hapusKarakterTerakhir(char text[])
{
    int length = 0;

    while (text[length] != '\0') {
        length++;
    }
    if (length == 0) return;

    length--;
    while (length > 0 && ((unsigned char)text[length] & 0xC0) == 0x80) length--;
    text[length] = '\0';
}

/* Menambahkan karakter yang sesuai dengan kolom aktif dan kapasitasnya. */
static void tambahKarakter(int codepoint)
{
    const char *encoded;
    int length;
    int i;
    int encodedLength;

    if (codepoint < 32 || codepoint == 127) return;
    if (focusedField == FIELD_NOMINAL) {
        if (codepoint < '0' || codepoint > '9') return;
    } else if (focusedField != FIELD_KETERANGAN) {
        return;
    }

    encoded = CodepointToUTF8(codepoint, &encodedLength);
    if (focusedField == FIELD_NOMINAL) {
        length = 0;
        while (nominalInput[length] != '\0') length++;
        if (encodedLength > 0 && length + encodedLength < (int)sizeof(nominalInput)) {
            for (i = 0; i < encodedLength; i++) {
                nominalInput[length + i] = encoded[i];
            }
            nominalInput[length + encodedLength] = '\0';
        }
    } else {
        length = 0;
        while (keteranganInput[length] != '\0') length++;
        if (encodedLength > 0 && length + encodedLength < (int)sizeof(keteranganInput)) {
            for (i = 0; i < encodedLength; i++) {
                keteranganInput[length + i] = encoded[i];
            }
            keteranganInput[length + encodedLength] = '\0';
        }
    }
}

/* Menempelkan teks clipboard melalui filter kolom yang sedang aktif. */
static void pasteClipboard(void)
{
    const char *clipboard = GetClipboardText();
    int i;
    if (clipboard == NULL) return;

    for (i = 0; clipboard[i] != '\0'; i++) {
        int byteLength = 0;
        int codepoint = GetCodepoint(clipboard + i, &byteLength);
        if (byteLength <= 0) break;
        tambahKarakter(codepoint);
        i += byteLength - 1;
    }
}

/* Memvalidasi lalu menyimpan transaksi melalui modul keuangan. */
static void simpanTransaksi(void)
{
    char message[128];
    int success;

    if (modalMode == 1) {
        success = tambahPemasukan(nominalInput, keteranganInput, message, sizeof(message));
    } else {
        success = tambahPengeluaran(nominalInput, keteranganInput, message, sizeof(message));
    }

    if (success) setStatus(message, 1);
    else setStatus(message, -1);

    snprintf(modalMessage, sizeof(modalMessage), "%s", message);
    if (success) modalKind = 1;
    else modalKind = -1;
    if (success) {
        modalMode = 0;
        focusedField = FIELD_NONE;
        nominalInput[0] = '\0';
        keteranganInput[0] = '\0';
        historyScroll = 100000.0f;
    }
}

/* Mengelola fokus, ketik, hapus, paste, Tab, dan Enter pada formulir. */
static void processModalInput(void)
{
    Rectangle nominalBox;
    Rectangle descriptionBox;
    Vector2 mouse;
    int codepoint;
    bool pasted = false;

    if (modalMode == 0) return;
    if (modalMode == 2) {
        nominalBox = (Rectangle){54, 341, 342, 42};
        descriptionBox = (Rectangle){54, 411, 342, 42};
    } else {
        nominalBox = (Rectangle){54, 310, 342, 42};
        descriptionBox = (Rectangle){54, 380, 342, 42};
    }
    mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, nominalBox)) {
            focusedField = FIELD_NOMINAL;
        } else if (CheckCollisionPointRec(mouse, descriptionBox)) {
            focusedField = FIELD_KETERANGAN;
        } else {
            focusedField = FIELD_NONE;
        }
    }

    if (focusedField != FIELD_NONE &&
        (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
        IsKeyPressed(KEY_V)) {
        pasteClipboard();
        pasted = true;
    }
    if (focusedField != FIELD_NONE && IsKeyPressed(KEY_BACKSPACE)) {
        if (focusedField == FIELD_NOMINAL) {
            hapusKarakterTerakhir(nominalInput);
        } else {
            hapusKarakterTerakhir(keteranganInput);
        }
    }

    while ((codepoint = GetCharPressed()) > 0) {
        if (!pasted) tambahKarakter(codepoint);
    }
    if (focusedField != FIELD_NONE && IsKeyPressed(KEY_TAB)) {
        if (focusedField == FIELD_NOMINAL) {
            focusedField = FIELD_KETERANGAN;
        } else {
            focusedField = FIELD_NOMINAL;
        }
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (focusedField == FIELD_NOMINAL) {
            focusedField = FIELD_KETERANGAN;
        } else if (focusedField == FIELD_KETERANGAN) {
            simpanTransaksi();
        }
    }
}

/* Menggambar kotak input Raygui dan indikator fokus kursor. */
static void drawTextBox(Rectangle bounds, char text[], int field)
{
    float cursorX;
    int characterLimit;

    if (field == FIELD_NOMINAL) {
        characterLimit = (int)sizeof(nominalInput);
    } else {
        characterLimit = (int)sizeof(keteranganInput);
    }

    GuiTextBox(bounds, text, characterLimit, false);
    if (focusedField != field) return;

    DrawRectangleLinesEx(bounds, 2, COLOR_PRIMARY);
    cursorX = bounds.x + 10 + measureText(text, 15);
    if (cursorX > bounds.x + bounds.width - 8) cursorX = bounds.x + bounds.width - 8;
    DrawLineEx((Vector2){cursorX, bounds.y + 11}, (Vector2){cursorX, bounds.y + bounds.height - 11},
               1.5f, COLOR_PRIMARY);
}

/* Menampilkan modal input sesuai jenis transaksi dan saldo tersedia. */
static void drawModal(void)
{
    Rectangle modal = {30, 216, 390, 370};
    int nominalLabelY;
    int descriptionLabelY;
    Rectangle nominalBox = {54, 310, 342, 42};
    Rectangle descriptionBox = {54, 380, 342, 42};
    Color submitColor;
    const char *title;

    if (modalMode == 2) {
        nominalLabelY = 322;
        descriptionLabelY = 392;
        submitColor = COLOR_EXPENSE;
        title = "Tambah Pengeluaran";
    } else {
        nominalLabelY = 291;
        descriptionLabelY = 361;
        submitColor = COLOR_PRIMARY;
        title = "Tambah Pemasukan";
    }
    nominalBox.y = (float)(nominalLabelY + 19);
    descriptionBox.y = (float)(descriptionLabelY + 19);

    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, overlayAlpha));
    drawRounded(modal, 0.07f, COLOR_SURFACE, COLOR_BORDER, 1);
    drawText(title, 54, 236, 22, COLOR_TEXT);
    drawText("Catat transaksi keuanganmu", 54, 267, 12, COLOR_MUTED);

    if (modalMode == 2) {
        char availableText[64];
        formatRupiah(saldoAkhir, availableText, sizeof(availableText));
        drawRounded((Rectangle){54, 286, 342, 27}, 0.25f, (Color){232, 247, 244, 255},
                    (Color){232, 247, 244, 255}, 0.5f);
        drawText(TextFormat("Saldo tersedia: %s", availableText), 65, 291, 12, COLOR_PRIMARY_DARK);
    }

    drawText("Nominal", 54, nominalLabelY, 13, COLOR_TEXT);
    drawTextBox(nominalBox, nominalInput, FIELD_NOMINAL);
    drawText("Keterangan", 54, descriptionLabelY, 13, COLOR_TEXT);
    drawTextBox(descriptionBox, keteranganInput, FIELD_KETERANGAN);

    if (modalMessage[0] != '\0') {
        Color messageColor;
        if (modalKind > 0) messageColor = COLOR_INCOME;
        else messageColor = COLOR_EXPENSE;
        drawText(modalMessage, 54, 466, 11, messageColor);
    } else {
        drawText("Ketik nominal dan keterangan, lalu pilih Simpan.", 54, 466, 11, COLOR_MUTED);
    }

    if (drawButton((Rectangle){54, 518, 157, 43}, "Batal", (Color){241, 245, 243, 255},
                   COLOR_MUTED, 14, HOVER_MODAL_CANCEL)) {
        modalMode = 0;
        focusedField = FIELD_NONE;
    }
    if (drawButton((Rectangle){239, 518, 157, 43}, "Simpan", submitColor, WHITE, 14,
                   HOVER_MODAL_SAVE)) {
        simpanTransaksi();
    }
}

/* Menampilkan ringkasan dan tiga transaksi terakhir. */
static void drawSummaryPage(void)
{
    int itemCount;
    int firstIndex;
    char countText[32];

    if (jumlah < 3) itemCount = jumlah;
    else itemCount = 3;
    firstIndex = jumlah - itemCount;
    drawStatusBanner(137);
    drawSummaryCard(190);
    drawText("Transaksi terbaru", 22, 412, 17, COLOR_TEXT);
    snprintf(countText, sizeof(countText), "%d transaksi", jumlah);
    drawTextRight(countText, 426, 417, 11, COLOR_MUTED);
    drawTransactionList((Rectangle){18, 442, 414, 276}, firstIndex, itemCount, false);
}

/* Menampilkan seluruh transaksi dengan area yang bisa di-scroll. */
static void drawHistoryPage(void)
{
    char countText[32];

    drawStatusBanner(137);
    drawText("Riwayat Transaksi", 22, 197, 17, COLOR_TEXT);
    snprintf(countText, sizeof(countText), "%d / %d transaksi", jumlah, MAX);
    drawTextRight(countText, 426, 202, 11, COLOR_MUTED);
    drawTransactionList((Rectangle){18, 228, 414, 490}, 0, jumlah, true);
}

/* Meminta konfirmasi akhir sebelum aplikasi ditutup. */
static void drawExitDialog(void)
{
    Rectangle dialog = {55, 302, 340, 196};
    float closeButtonX;

    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, overlayAlpha));
    drawRounded(dialog, 0.08f, COLOR_SURFACE, COLOR_BORDER, 1);
    drawTextCentered("Terima kasih!", dialog.x, dialog.y + 27, dialog.width, 22, COLOR_TEXT);
    drawTextCentered("Terima kasih telah menggunakan program!", dialog.x + 12,
                     dialog.y + 73, dialog.width - 24, 12, COLOR_MUTED);
    if (!windowCloseRequested &&
        drawButton((Rectangle){dialog.x + 22, dialog.y + 124, 138, 42},
                   "Kembali", (Color){241, 245, 243, 255}, COLOR_MUTED, 13,
                   HOVER_EXIT_BACK)) {
        showExitDialog = false;
    }
    if (windowCloseRequested) closeButtonX = dialog.x + 101;
    else closeButtonX = dialog.x + 180;
    if (drawButton((Rectangle){closeButtonX, dialog.y + 124, 138, 42},
                   "Tutup", COLOR_PRIMARY, WHITE, 13, HOVER_EXIT_CONFIRM)) {
        shouldClose = true;
    }
}

/* Menerapkan font dan warna khusus pada kontrol Raygui. */
static void configureRaygui(void)
{
    GuiSetFont(appFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, (int)ColorToInt(COLOR_TEXT));
    GuiSetStyle(TEXTBOX, TEXT_SIZE, 15);
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 10);
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 1);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, (int)ColorToInt(COLOR_TEXT));
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, (int)ColorToInt(COLOR_SURFACE));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, (int)ColorToInt(COLOR_BORDER));
}

/* Membuat jendela, memuat font, memperbarui tampilan, lalu membersihkan resource. */
int jalankanAplikasi(void)
{
    char fontPath[512];

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Personal Finance Tracker");
    if (!IsWindowReady()) {
        TraceLog(LOG_ERROR, "Jendela aplikasi gagal dibuat.");
        return 1;
    }

    snprintf(fontPath, sizeof(fontPath), "%sassets\\Inter.ttf", GetApplicationDirectory());
    if (!FileExists(fontPath)) {
        TraceLog(LOG_ERROR, "Font tidak ditemukan: %s", fontPath);
        CloseWindow();
        return 1;
    }
    appFont = LoadFontEx(fontPath, 40, NULL, 0);
    if (appFont.texture.id == 0) {
        TraceLog(LOG_ERROR, "Font Inter gagal dimuat: %s", fontPath);
        CloseWindow();
        return 1;
    }
    SetTextureFilter(appFont.texture, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);
    configureRaygui();

    while (!shouldClose) {
        float targetOverlay;
        if (WindowShouldClose()) {
            windowCloseRequested = true;
            showExitDialog = true;
        }
        if (modalMode != 0 || showExitDialog) targetOverlay = 0.62f;
        else targetOverlay = 0.0f;
        overlayAlpha = Lerp(overlayAlpha, targetOverlay,
                            Clamp(GetFrameTime() * 10.0f, 0.0f, 1.0f));
        hitungRingkasan();
        if (modalMode != 0 && !windowCloseRequested) processModalInput();

        BeginDrawing();
        ClearBackground(COLOR_PAGE);
        drawHeader();

        if (modalMode == 0 && !showExitDialog) {
            if (drawButton((Rectangle){18, 82, 200, 46}, "+ Pemasukan",
                           COLOR_PRIMARY, WHITE, 15, HOVER_INCOME)) {
                openModal(1);
            }
            if (drawButton((Rectangle){232, 82, 200, 46}, "- Pengeluaran",
                           COLOR_EXPENSE, WHITE, 15, HOVER_EXPENSE)) {
                openModal(2);
            }
        }

        if (activePage == PAGE_SUMMARY) {
            drawSummaryPage();
        } else {
            drawHistoryPage();
        }
        drawBottomNavigation(modalMode == 0 && !showExitDialog);
        if (modalMode != 0) drawModal();
        if (showExitDialog) drawExitDialog();

        EndDrawing();
    }

    UnloadFont(appFont);
    CloseWindow();
    return 0;
}
