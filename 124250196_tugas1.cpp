#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCT 100
#define FILE_NAME "lelele.csv"

typedef struct {
    char nama[50];
    float harga;
    char kategori[30];
    int stok;
} Produk;

Produk daftarProduk[MAX_PRODUCT];
int jumlahProduk = 0;

void loadData() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("File %s tidak ditemukan. Data baru akan dibuat.\n", FILE_NAME);
        return;
    }

    jumlahProduk = 0;
    while (fscanf(file, "%[^,],%f,%[^,],%d\n",
                  daftarProduk[jumlahProduk].nama,
                  &daftarProduk[jumlahProduk].harga,
                  daftarProduk[jumlahProduk].kategori,
                  &daftarProduk[jumlahProduk].stok) != EOF) {
        jumlahProduk++;
        if (jumlahProduk >= MAX_PRODUCT) break;
    }
    fclose(file);
    printf("Data berhasil dimuat dari %s (%d produk).\n", FILE_NAME, jumlahProduk);
}

void saveData() {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error: Gagal menyimpan ke file %s\n", FILE_NAME);
        return;
    }

    for (int i = 0; i < jumlahProduk; i++) {
        fprintf(file, "%s,%.2f,%s,%d\n",
                daftarProduk[i].nama,
                daftarProduk[i].harga,
                daftarProduk[i].kategori,
                daftarProduk[i].stok);
    }
    fclose(file);
    printf("Data berhasil disimpan ke %s\n", FILE_NAME);
}

void tambahProduk() {
    if (jumlahProduk >= MAX_PRODUCT) {
        printf("Penyimpanan penuh! Tidak dapat menambah produk baru.\n");
        return;
    }

    Produk baru;
    printf("Masukkan nama produk: ");
    getchar(); 
    fgets(baru.nama, sizeof(baru.nama), stdin);
    baru.nama[strcspn(baru.nama, "\n")] = 0;

    printf("Masukkan harga: Rp ");
    scanf("%f", &baru.harga);

    do {
        printf("Masukkan kategori (makanan/minuman): ");
        getchar();
        fgets(baru.kategori, sizeof(baru.kategori), stdin);
        baru.kategori[strcspn(baru.kategori, "\n")] = 0;

        if (strcmp(baru.kategori, "makanan") != 0 && strcmp(baru.kategori, "minuman") != 0) {
            printf("Error: Kategori harus 'makanan' atau 'minuman'.\n");
        }
    } while (strcmp(baru.kategori, "makanan") != 0 && strcmp(baru.kategori, "minuman") != 0);

    printf("Masukkan stok: ");
    scanf("%d", &baru.stok);

    daftarProduk[jumlahProduk] = baru;
    jumlahProduk++;

    saveData();
    printf("Produk '%s' berhasil ditambahkan!\n", baru.nama);
}


void tampilkanSemua() {
    if (jumlahProduk == 0) {
        printf("\nBelum ada data produk.\n");
        return;
    }

    printf("\n================================================================================\n");
    printf("| %-30s | %-10s | %-12s | %-6s |\n", "Nama Produk", "Harga (Rp)", "Kategori", "Stok");
    printf("================================================================================\n");
    for (int i = 0; i < jumlahProduk; i++) {
        printf("| %-30s | Rp %-7.2f | %-12s | %-6d |\n",
               daftarProduk[i].nama,
               daftarProduk[i].harga,
               daftarProduk[i].kategori,
               daftarProduk[i].stok);
    }
    printf("================================================================================\n");
}


void bubbleSortByName() {
    for (int i = 0; i < jumlahProduk - 1; i++) {
        for (int j = 0; j < jumlahProduk - i - 1; j++) {
            if (strcmp(daftarProduk[j].nama, daftarProduk[j + 1].nama) > 0) {
                Produk temp = daftarProduk[j];
                daftarProduk[j] = daftarProduk[j + 1];
                daftarProduk[j + 1] = temp;
            }
        }
    }
    printf("Produk berhasil diurutkan berdasarkan NAMA (Ascending) menggunakan Bubble Sort.\n");
    tampilkanSemua();
    saveData();
}


int partitionByPrice(Produk arr[], int low, int high) {
    float pivot = arr[high].harga;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].harga > pivot) { 
            i++;
            Produk temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Produk temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

void quickSortByPrice(Produk arr[], int low, int high) {
    if (low < high) {
        int pi = partitionByPrice(arr, low, high);
        quickSortByPrice(arr, low, pi - 1);
        quickSortByPrice(arr, pi + 1, high);
    }
}

void urutkanHargaDescending() {
    if (jumlahProduk <= 1) {
        printf("Data terlalu sedikit untuk diurutkan.\n");
        tampilkanSemua();
        return;
    }
    quickSortByPrice(daftarProduk, 0, jumlahProduk - 1);
    printf("Produk berhasil diurutkan berdasarkan HARGA (Descending) menggunakan Quick Sort.\n");
    tampilkanSemua();
    saveData();
}


void sequentialSearch(char *cariNama) {
    int found = 0;
    for (int i = 0; i < jumlahProduk; i++) {
        if (strcmp(daftarProduk[i].nama, cariNama) == 0) {
            printf("\nProduk ditemukan (Sequential Search):\n");
            printf("Nama     : %s\n", daftarProduk[i].nama);
            printf("Harga    : Rp %.2f\n", daftarProduk[i].harga);
            printf("Kategori : %s\n", daftarProduk[i].kategori);
            printf("Stok     : %d\n", daftarProduk[i].stok);
            found = 1;
            break;
        }
    }
    if (!found) printf("Produk '%s' tidak ditemukan!\n", cariNama);
}

int binarySearch(char *cariNama, int low, int high) {
    if (low > high) return -1;
    int mid = (low + high) / 2;
    int cmp = strcmp(daftarProduk[mid].nama, cariNama);
    if (cmp == 0) return mid;
    else if (cmp > 0) return binarySearch(cariNama, low, mid - 1);
    else return binarySearch(cariNama, mid + 1, high);
}

void cariProduk() {
    if (jumlahProduk == 0) {
        printf("Belum ada data. Tambahkan produk terlebih dahulu.\n");
        return;
    }

    char namaCari[50];
    printf("Masukkan nama produk yang dicari: ");
    getchar();
    fgets(namaCari, sizeof(namaCari), stdin);
    namaCari[strcspn(namaCari, "\n")] = 0;

    int pilihan;
    printf("Pilih metode pencarian:\n");
    printf("1. Sequential Search\n");
    printf("2. Binary Search (produk harus sudah diurutkan berdasarkan nama)\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        sequentialSearch(namaCari);
    } else if (pilihan == 2) {
       
        bubbleSortByName(); 
        int index = binarySearch(namaCari, 0, jumlahProduk - 1);
        if (index != -1) {
            printf("\nProduk ditemukan (Binary Search):\n");
            printf("Nama     : %s\n", daftarProduk[index].nama);
            printf("Harga    : Rp %.2f\n", daftarProduk[index].harga);
            printf("Kategori : %s\n", daftarProduk[index].kategori);
            printf("Stok     : %d\n", daftarProduk[index].stok);
        } else {
            printf("Produk '%s' tidak ditemukan!\n", namaCari);
        }
    } else {
        printf("Pilihan tidak valid.\n");
    }
}


void menu() {
    int pilihan;
    do {
        printf("\n========= LE LELE MENU MANAGER =========\n");
        printf("1. Tambah Produk Baru\n");
        printf("2. Urutkan Produk (Bubble Sort - Nama A-Z)\n");
        printf("3. Urutkan Produk (Quick Sort - Harga Tertinggi ke Terendah)\n");
        printf("4. Cari Produk\n");
        printf("5. Tampilkan Semua Produk\n");
        printf("0. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: tambahProduk(); break;
            case 2: bubbleSortByName(); break;
            case 3: urutkanHargaDescending(); break;
            case 4: cariProduk(); break;
            case 5: tampilkanSemua(); break;
            case 0: printf("Terima kasih telah menggunakan program ini.\n"); break;
            default: printf("Pilihan tidak valid! Silakan coba lagi.\n");
        }
    } while (pilihan != 0);
}

int main() {
    loadData();
    menu();
    return 0;
}
