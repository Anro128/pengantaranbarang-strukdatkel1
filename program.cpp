#include<bits/stdc++.h>
using namespace std;

struct Barang{
    int id;
    string nama;
    int stok;
};

struct Pengemudi{
    int id;
    string nama;
    bool status;
};

struct Pesanan{
    int id;
    int idBarang;
    int idPengemudi;
    int jumlah;
    string status;
    string lokTerakhir;
    string tujuan;
};

struct Notifikasi{
    string isi;
};

void readBarang(vector<Barang>& brg, const string& filename) {
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Gagal membuka file!" << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        size_t pos1 = line.find('|');
        if (pos1 != string::npos) {
            int id = stoi(line.substr(0, pos1));
            size_t pos2 = line.find('|', pos1+1);
            string nama = line.substr(pos1 + 1, pos2-(pos1+1));
            int stok=stoi(line.substr(pos2+1));
            Barang b = {id, nama, stok};
            brg.push_back(b);
        }
    }

    infile.close();
    return;
}

void readPengemudi(vector<Pengemudi>& png, const string& filename) {
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Gagal membuka file!" << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        size_t pos1 = line.find('|');
        if (pos1 != string::npos) {
            int id = stoi(line.substr(0, pos1));
            size_t pos2 = line.find('|', pos1+1);
            string nama = line.substr(pos1 + 1, pos2-(pos1+1));
            int status=stoi(line.substr(pos2+1));
            Pengemudi b = {id, nama, status};
            png.push_back(b);
        }
    }

    infile.close();
    return;
}

void readPesanan(deque<Pesanan>& psn, const string& filename){
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Gagal membuka file!" << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string item;
        vector<string> tmp;

        while (std::getline(ss, item, '|')) {
            tmp.push_back(item);
        }
        
        Pesanan p;
        p.id = stoi(tmp[0]);
        p.idBarang = stoi(tmp[1]);
        p.idPengemudi = stoi(tmp[2]);
        p.jumlah = stoi(tmp[3]);
        p.status = tmp[4];
        p.lokTerakhir = tmp[5];
        p.tujuan = tmp[6];

        psn.push_back(p);
    }

    infile.close();
    return;
}

void saveBarang(vector<Barang> v){
    ofstream barang("Barang.txt");
    for (const auto &e : v) barang << e.id <<"|"<<e.nama <<"|"<<e.stok << "\n";
}

void savePengemudi(vector<Pengemudi> v){
    ofstream pengemudi("Pengemudi.txt");
    for (const auto &f : v) pengemudi << f.id <<"|"<<f.nama <<"|"<<f.status << "\n";
}

bool cmp(const Pesanan& a, const Pesanan& b) {
    return a.id < b.id;
}

vector <Barang> barang;
vector <Pengemudi> pengemudi;
stack<Notifikasi> notif;


class Pengantaran{
    private:
        deque<Pesanan> pesanan;
        int idPesanan = 0;
    public:
        
        Pengantaran(){
            readPesanan(pesanan, "Pesanan.txt");
            idPesanan= pesanan.size();
        }

        void lihatDaftarBarang(){
            cout<<"Daftar barang yang tersedia untuk diantar:"<< endl;
            if(barang.size()==0){
                cout<<"Tidak ada barang yang tersedia"<<endl;
                return;
            }

            cout << left << setw(5) << "ID" << " | "<< left << setw(25) << "Nama" <<" | "<< left << setw(5) << "Stok" << endl;
            cout << string(36, '-') << endl;
            for(auto b: barang){
                cout << left << setw(5) << b.id << " | "<< left << setw(25) << b.nama <<" | "<< left << setw(5) << b.stok << endl;
            }
            return;
        }

        void lihatDaftarPengemudi(){
            cout<<"Daftar pengemudi:"<< endl;
            if(pengemudi.size()==0){
                cout<<"Tidak ada pengemudi yang tersedia"<<endl;
                return;
            }

            cout << left << setw(5) << "ID" << " | "<< left << setw(25) << "Nama" << endl;
            cout << string(31, '-') << endl;
            for(auto p: pengemudi){
                if(p.status == 0)
                    cout << left << setw(5) << p.id << " | "<< left << setw(25) << p.nama << endl;
            }
            return;
        }

        void lihatDaftarPesanan(){
            if(pesanan.empty()){
                cout<<"Tidak ada pesanan yang sedang diproses"<<endl;
                return;
            }

            cout<<"Daftar Pesanan"<<endl;
            cout << left << setw(5) << "ID" << "|" << left << setw(10) << "ID Barang" << "|" << left << setw(12) << "ID Pengemudi" << "|" << left << setw(8) << "Jumlah" << "|" << left << setw(15) << "Status" << "|"
                << left << setw(20) << "Lok. Terakhir" << "|"
                << left << setw(20) << "Tujuan" << endl;
            cout << string( 76, '-') << endl;

            // Data pesanan
            for (auto p : pesanan) {
                cout << left << setw(5) << p.id << "|"
                    << left << setw(10) << p.idBarang << "|"
                    << left << setw(12) << p.idPengemudi << "|"
                    << left << setw(8) << p.jumlah << "|"
                    << left << setw(15) << p.status << "|"
                    << left << setw(20) << p.lokTerakhir << "|"
                    << left << setw(20) << p.tujuan << endl;
            }
        }

        void buat(){
            lihatDaftarBarang();

            int idBarang, jum;
            string tujuan;
            cout<<"ID barang yang ingin di antar: "; cin>>idBarang;
            cout<<"Jumlah yang ingin diantar: "; cin>>jum;
            cout<<"Tujuan: "; cin>>tujuan;
            idPesanan++;

            if(jum > barang[idBarang].stok){
                cout<<"Stok tidak mencukupi"<<endl;
                return;
            }

            Pesanan tmp={idPesanan, idBarang, -1, jum, "Diverifikasi", "gudang", tujuan};
            string ntf="Pesanan "+to_string(idPesanan)+" berhasil ditambahkan";
            addNotif(ntf);
            cout<<ntf<<endl;
            pesanan.push_back(tmp);
            return;
        }

        int findIDx(const deque<Pesanan>& dq, int id) {
            Pesanan target = {id, 0, 0, 0, "", "", ""};
            auto it = lower_bound(dq.begin(), dq.end(), target, cmp);
            if (it != dq.end()) {
                return distance(dq.begin(), it);
            } else {
                return -1;
            }
        }
        
        void verif(){
            cout<<"Daftar pesanan yang belum diverifikasi: "<<endl;
            cout<<string(50,'-')<<endl;
            if(pesanan.empty()){
                cout<<"Tidak ada pesanan"<<endl;
                return;
            }
            
            int hit=0;
            for(auto p: pesanan){
                if(p.status == "Diverifikasi"){
                    hit++;
                    cout << "Id Pesananan: "<< (p.id) << "| Nama Barang: " << barang[p.idBarang].nama << "| Tujuan: "<<p.tujuan<<endl;
                }
            }

            if(hit==0){
                cout<<"Tidak ada pesanan yang harus diverifikasi"<<endl;
                return;
            }

            int id;
            cout<<"Masukan ID pesanan yang ingin diverifikasi: "; cin>>id;
            int idx = findIDx(pesanan, id);
            pesanan[idx].status = "Dikirim";
            lihatDaftarPengemudi();
            int idP;
            cout<<"Masukan ID pengemudi untuk ditugaskan dipengantaran ini: "; cin>>idP;
            pesanan[idx].idPengemudi=idP;
            pengemudi[idP].status=1;

            string ntf="Pesanan berhasil diverifikasi";
            addNotif(ntf);
            cout<<ntf<<endl;
            return;
        }

        void update(int id){
            int idx = findIDx(pesanan, id);

            cout<<"Pilih opsi update: "<<endl;
            cout<<"1. Status"<<endl;
            cout<<"2. Lokasi"<<endl;
            int o; cin>>o;

            if(o==1){
                pesanan[idx].status = "Sampai";
                pesanan[idx].lokTerakhir = pesanan[idx].tujuan;
                pengemudi[pesanan[idx].idPengemudi].status=0;
                barang[pesanan[idx].idBarang].stok-=pesanan[idx].jumlah;
                string ntf="Pesanan "+to_string(pesanan[idx].id)+" Sampai di Tujuan akhir"+pesanan[idx].tujuan;
                addNotif(ntf);
            }else if(o==2){
                string lokTerakhir;
                cout<<"Masukan lokasi terakhir: ";
                cin>>lokTerakhir;
                pesanan[idx].lokTerakhir = lokTerakhir;
                string ntf="Pesanan "+to_string(pesanan[idx].id)+" Sampai di "+pesanan[idx].lokTerakhir;
                addNotif(ntf);
            }
            cout<<"Berhasil diupdate"<<endl;
            return;
        }

        void lihatStatus(int id){
            if(id > pesanan.size()){
                cout<<"Pesanan Tidak ditemukan"<<endl;
                return;
            }

            int idx = findIDx(pesanan, id);
            Pesanan P=pesanan[idx];

            if(P.status == "Diverifikasi")
                cout<<"Pesanan sedang diverifikasi oleh admin"<<endl;
            else if(P.status == "Dikirim") 
                cout<<"Pesanan sudah sampai di "<< P.lokTerakhir <<", diantarkan oleh "<< pengemudi[P.idPengemudi].nama <<endl;
            else if(P.status == "Sampai")
                cout<<"Pesanan sudah terkirim ketujuan akhir : "<<P.tujuan<<endl;
            
            return;
        }

        void savePesanan(){
            ofstream psn("Pesanan.txt");
            for (const auto &e : pesanan) psn << e.id <<"|"<<e.idBarang <<"|"<<e.idPengemudi <<"|"<<e.jumlah <<"|"<<e.status <<"|"<<e.lokTerakhir <<"|"<<e.tujuan << "\n";
        }
        
        void addNotif(string s){
            Notifikasi newnotif;
            newnotif.isi=s;
            notif.push(newnotif);
        }

        void lihatNotif(){
            int x=5;
            queue<string> tmpnotif;
            cout<<"NOTIFIKASI"<<endl;
            while(x>0 && !notif.empty()){
                string tmp=notif.top().isi;
                cout<<" - "<<tmp<<endl;
                notif.pop();
                tmpnotif.push(tmp);
                x--;
            }
            
            while(!tmpnotif.empty()){
                Notifikasi newnotif;
                newnotif.isi=tmpnotif.front();
                notif.push(newnotif);
                tmpnotif.pop();
            }
            return;
        }
};

bool cek_admin(string pw){
    int enc[9]={1210, 1287, 1397, 1331, 1287, 1320, 1320, 1199, 1276};
    bool ret=1;
    for(int i=0;i<9;i++){
        int x=pw[i];
        x+=12;
        x*=11;
        if(x!=enc[i]) ret=0;
    }
    return ret;
}

int main(){
    readBarang(barang, "Barang.txt");
    readPengemudi(pengemudi, "Pengemudi.txt");
    Pengantaran pengantaran;

    cout<<"SELAMAT DATANG DI APLIKASI ANTARBARANG"<<endl;
    cout<<"======================================="<<endl;
    cout<<"Ketikan password untuk login sebagai admin: ";
    string pw;
    cin>>pw;
    if(cek_admin(pw)){
        cout<<"Password benar! Welcome admin!!!"<<endl;
    }else{
        cout<<"Password salah!, anda login sebagai user"<<endl;
    }
    
    while(1){
        cout <<"Haii... Kamu mau apa?"<< endl;
        cout <<"1. Antar Barang"<<endl;
        cout <<"2. Cek Status"<<endl;
        cout <<"3. Lihat Notifikasi"<<endl;

        if(cek_admin(pw)){
            cout <<"111. Tambahkan barang"<<endl;
            cout <<"112. Update data pengiriman"<<endl;
            cout <<"113. Verifikasi pengiriman dan kelola pengemudi"<<endl;
            cout <<"114. Tambahkan pengemudi"<<endl;
        }

        cout<<"99. Keluar"<<endl;

        int p;
        cin>>p;
        if(p == 111 && cek_admin(pw)){
            int id=barang.size();
            string nama;
            int stok;
            cout<<"Nama barang: "; cin>>nama;
            cout<<"Stok: "; cin>>stok;
            barang.push_back({id,nama,stok});
        }else if(p == 112  && cek_admin(pw)){
            pengantaran.lihatDaftarPesanan();

            int id;
            cout<<"ID pengantaran: "; cin>>id;
            pengantaran.update(id);
        }else if(p == 113 && cek_admin(pw)){
            pengantaran.verif();
        }else if(p == 114 && cek_admin(pw)){
            int id=pengemudi.size();
            string nama;
            cout<<"Nama Pengemudi: "; cin>>nama;
            pengemudi.push_back({id,nama,0});
        }else if(p == 1){
            pengantaran.buat();
        }else if(p==2){
            int id;
            cout<<"ID pengantaran: "; cin>>id;
            pengantaran.lihatStatus(id);
        }else if(p==3){
            pengantaran.lihatNotif();
        }else{
            saveBarang(barang);
            savePengemudi(pengemudi);
            pengantaran.savePesanan();
            break;
        }
    }
    return 0;
}