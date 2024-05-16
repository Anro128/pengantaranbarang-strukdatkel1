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


void saveBarang(vector<Barang> v){
    std::ofstream outFile("Barang.txt");
    for (const auto &e : v) outFile << e.id <<" | "<<e.nama <<" | "<<e.stok << "\n";
}

void savePengemudi(vector<Pengemudi> v){
    std::ofstream outFile("Pengemudi.txt");
    for (const auto &e : v) outFile << e.id <<" | "<<e.nama <<" | "<<e.status << "\n";
}

bool cmp(const Pesanan& a, const Pesanan& b) {
    return a.id < b.id;
}

vector <Barang> barang;
vector <Pengemudi> pengemudi;

class Notif{
    private:
        stack<Notifikasi> notif;
    public:
        Notif(){}

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
                cout<<tmp<<endl;
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

class Pengantaran{
    private:
        deque<Pesanan> pesanan;
        Notif notif;
        int idPesanan = 0;
    public:
        void lihatDaftarBarang(){
            cout<<"Daftar barang yang tersedia untuk diantar:"<< endl;
            if(barang.size()==0){
                cout<<"Tidak ada barang yang tersedia"<<endl;
                return;
            }

            cout<<"ID | Nama | Stok"<<endl;
            for(auto b: barang){
                cout<<b.id<<" | "<<b.nama<<" | "<<b.stok<<endl;
            }
            return;
        }

        void lihatDaftarPengemudi(){
            cout<<"Daftar pengemudi:"<< endl;
            if(pengemudi.size()==0){
                cout<<"Tidak ada pengemudi yang tersedia"<<endl;
                return;
            }

            cout<<"ID | Nama"<<endl;
            for(auto p: pengemudi){
                if(p.status == 0)cout<<p.id<<" | "<<p.nama<<endl;
            }
            return;
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
            notif.addNotif(ntf);
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
            for(auto p: pesanan){
                if(p.status == "Diverifikasi"){
                    printf("Id pesanan: %d,Nama Barang: %s,Tujuan: %s\n",p.id, barang[p.idBarang], p.tujuan);
                }
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
            notif.addNotif(ntf);
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
                string ntf="Pesanan "+to_string(pesanan[idx].id)+" Sampai di Tujuan akhir"+pesanan[idx].tujuan;
                notif.addNotif(ntf);
            }else{
                string lokTerakhir;
                cin>>lokTerakhir;
                pesanan[idx].lokTerakhir = lokTerakhir;
                string ntf="Pesanan "+to_string(pesanan[idx].id)+" Sampai di "+pesanan[idx].lokTerakhir;
                notif.addNotif(ntf);
            }
            cout<<"Berhasil diupdate"<<endl;
            return;
        }

        void lihatStatus(int id){
            int idx = findIDx(pesanan, id);
            Pesanan P=pesanan[idx];

            if(P.status == "Diverifikasi")
                cout<<"Pesanan sedang diverifikasi oleh admin"<<endl;
            else if(P.status == "Dikirim") 
                printf("Pesanan sudah sampai di %s, diantarkan oleh %s\n", P.lokTerakhir, pengemudi[P.idPengemudi].nama);
            else if(P.status == "Sampai")
                printf("Pesanan sudah terkirim ketujuan akhir : %s\n",P.tujuan);
            
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
    Pengantaran pengantaran;
    Notif notif;
    cout<<"Ketik 1 untuk login sebagain admin, jika bukan ketik selain 1"<<endl;
    int iniadmin=0;
    cin>>iniadmin;
    string pw;
    if(iniadmin == 1) {
        cout<<"Masukan Password: ";
        cin>>pw;
        bool isadmin=cek_admin(pw);
        if(isadmin){
            cout<<"Password benar! Welcome admin!!!"<<endl;
        }else{
            cout<<"Password salah!, anda login sebagai user"<<endl;
        }
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
            int id;
            cout<<"ID pengantaran: "; cin>>id;
            pengantaran.update(id);
        }else if(p == 113 && cek_admin(pw)){
            pengantaran.verif();
        }else if(p == 114 && cek_admin(pw)){
            int id=pengemudi.size();
            string nama;
            cout<<"Nama Pengemudi: "; cin>>nama;
            barang.push_back({id,nama,0});
        }else if(p == 1){
            pengantaran.buat();
        }else if(p==2){
            int id;
            cout<<"ID pengantaran: "; cin>>id;
            pengantaran.lihatStatus(id);
        }else if(p==3){
            notif.lihatNotif();
        }else{
            saveBarang(barang);
            break;
        }
    }
    return 0;
}