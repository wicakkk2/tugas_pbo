#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Buku {
protected:

    string judul;
    string pengarang;
    int tahunTerbit;
    string ISBN;
    bool status; 

public:
    Buku(string j, string p, int t, string i)
        : judul(j), pengarang(p), tahunTerbit(t), ISBN(i), status(true) {}

    virtual void tampilkanInfoBuku() {
        cout << "Judul: " << judul << endl;
        cout << "Pengarang: " << pengarang << endl;
        cout << "Tahun Terbit: " << tahunTerbit << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Status: " << (status ? "Tersedia" : "Dipinjam") << endl;
    }

    bool isTersedia() const { return status; }
    void setStatus(bool s) { status = s; }
    string getJudul() const { return judul; }
};

class BukuDigital : public Buku {
private:
    string linkAkses;

public:
    BukuDigital(string j, string p, int t, string i, string link)
        : Buku(j, p, t, i), linkAkses(link) {}

    void tampilkanInfoBuku() override {
        Buku::tampilkanInfoBuku();
        cout << "Link Akses: " << linkAkses << endl;
    }
};

class Anggota {
private:
    string nama;
    string nomorAnggota;
    string alamat;
    vector<Buku*> daftarPinjaman;

public:
    Anggota(string n, string no, string a)
        : nama(n), nomorAnggota(no), alamat(a) {}

    void tampilkanInfoAnggota() {
        cout << "Nama: " << nama << endl;
        cout << "Nomor Anggota: " << nomorAnggota << endl;
        cout << "Alamat: " << alamat << endl;
        cout << "Daftar Pinjaman: " << endl;
        for (auto& buku : daftarPinjaman) {
            cout << "- " << buku->getJudul() << endl;
        }
    }

    void pinjamBuku(Buku* buku) {
        if (buku->isTersedia()) {
            daftarPinjaman.push_back(buku);
            buku->setStatus(false);
            cout << "Buku berhasil dipinjam: " << buku->getJudul() << endl;
        } else {
            cout << "Buku sedang dipinjam: " << buku->getJudul() << endl;
        }
    }

    void kembalikanBuku(Buku* buku) {
        for (auto it = daftarPinjaman.begin(); it != daftarPinjaman.end(); ++it) {
            if (*it == buku) {
                daftarPinjaman.erase(it);
                buku->setStatus(true);
                cout << "Buku dikembalikan: " << buku->getJudul() << endl;
                return;
            }
        }
        cout << "Buku tidak ditemukan dalam daftar pinjaman." << endl;
    }

    string getNomorAnggota() const { return nomorAnggota; }
};

class Perpustakaan {
private:
    vector<Buku*> daftarBuku;
    vector<Anggota> daftarAnggota;

public:
    ~Perpustakaan() {
        for (auto buku : daftarBuku) {
            delete buku;
        }
    }

    void tambahBuku(Buku* buku) {
        daftarBuku.push_back(buku);
        cout << "Buku berhasil ditambahkan: " << buku->getJudul() << endl;
    }

    void tambahAnggota(const Anggota& anggota) {
        daftarAnggota.push_back(anggota);
        cout << "Anggota berhasil ditambahkan." << endl;
    }

    void daftarBukuTersedia() {
        cout << "Daftar Buku Tersedia:" << endl;
        for (const auto& buku : daftarBuku) {
            if (buku->isTersedia()) {
                buku->tampilkanInfoBuku();
                cout << endl;
            }
        }
    }

    Anggota* cariAnggota(const string& nomorAnggota) {
        for (auto& anggota : daftarAnggota) {
            if (anggota.getNomorAnggota() == nomorAnggota) {
                return &anggota;
            }
        }
        return nullptr;
    }

    Buku* cariBuku(const string& judul) {
        for (auto& buku : daftarBuku) {
            if (buku->getJudul() == judul) {
                return buku;
            }
        }
        return nullptr;
    }

    void pinjamBuku(const string& nomorAnggota, const string& judulBuku) {
        Anggota* anggota = cariAnggota(nomorAnggota);
        Buku* buku = cariBuku(judulBuku);
        if (anggota && buku) {
            anggota->pinjamBuku(buku);
        } else {
            cout << "Anggota atau Buku tidak ditemukan." << endl;
        }
    }

    void kembalikanBuku(const string& nomorAnggota, const string& judulBuku) {
        Anggota* anggota = cariAnggota(nomorAnggota);
        Buku* buku = cariBuku(judulBuku);
        if (anggota && buku) {
            anggota->kembalikanBuku(buku);
        } else {
            cout << "Anggota atau Buku tidak ditemukan." << endl;
        }
    }
};

void tampilkanMenu() {
    cout << "\n===== Menu Perpustakaan =====\n";
    cout << "1. Tambah Buku\n";
    cout << "2. Tambah Anggota\n";
    cout << "3. Pinjam Buku\n";
    cout << "4. Kembalikan Buku\n";
    cout << "5. Daftar Buku Tersedia\n";
    cout << "6. Keluar\n";
    cout << "Pilih menu: ";
}

int main() {
    Perpustakaan perpustakaan;
    int pilihan;

    do {
        tampilkanMenu();
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: {
                string judul, pengarang, ISBN, linkAkses;
                int tahunTerbit, jenisBuku;
                cout << "Pilih jenis buku (1: Buku Fisik, 2: Buku Digital): ";
                cin >> jenisBuku;
                cin.ignore();

                cout << "Masukkan Judul: ";
                getline(cin, judul);
                cout << "Masukkan Pengarang: ";
                getline(cin, pengarang);
                cout << "Masukkan Tahun Terbit: ";
                cin >> tahunTerbit;
                cin.ignore();
                cout << "Masukkan ISBN: ";
                getline(cin, ISBN);

                if (jenisBuku == 2) {
                    cout << "Masukkan Link Akses: ";
                    getline(cin, linkAkses);
                    perpustakaan.tambahBuku(new BukuDigital(judul, pengarang, tahunTerbit, ISBN, linkAkses));
                } else {
                    perpustakaan.tambahBuku(new Buku(judul, pengarang, tahunTerbit, ISBN));
                }
                break;
            }
            case 2: {
                string nama, nomorAnggota, alamat;
                cout << "Masukkan Nama Anggota: ";
                getline(cin, nama);
                cout << "Masukkan Nomor Anggota: ";
                getline(cin, nomorAnggota);
                cout << "Masukkan Alamat: ";
                getline(cin, alamat);
                perpustakaan.tambahAnggota(Anggota(nama, nomorAnggota, alamat));
                break;
            }
            case 3: {
                string nomorAnggota, judulBuku;
                cout << "Masukkan Nomor Anggota: ";
                getline(cin, nomorAnggota);
                cout << "Masukkan Judul Buku: ";
                getline(cin, judulBuku);
                perpustakaan.pinjamBuku(nomorAnggota, judulBuku);
                break;
            }
            case 4: {
                string nomorAnggota, judulBuku;
                cout << "Masukkan Nomor Anggota: ";
                getline(cin, nomorAnggota);
                cout << "Masukkan Judul Buku: ";
                getline(cin, judulBuku);
                perpustakaan.kembalikanBuku(nomorAnggota, judulBuku);
                break;
            }
            case 5: {
                perpustakaan.daftarBukuTersedia();
                break;
            }
            case 6: {
                cout << "Keluar dari program.\n";
                break;
            }
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 6);

    return 0;
}
