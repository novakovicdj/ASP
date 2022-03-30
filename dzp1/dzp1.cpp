#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

constexpr int MAX = 20;

class Cvor {
private:
	static int id;
	int br;
public:
	vector<pair <int,int> > ivice;
	Cvor* sl;
	Cvor() {
		br = ++id;
		sl = nullptr;
	}
	~Cvor() {
		ivice.clear();
	}
	int getBr() {
		return br;
	}
	static void resetId() {
		id = 0;
	}
};

int Cvor::id = 0;

class Graf {
	int n;
public:
	set<int> s;
	Cvor* head = nullptr;
	Graf(int N) : n(N) {
		Cvor* posl = head;
		for (int i = 0; i < N; i++) {
			Cvor* novi = new Cvor;
			s.insert(novi->getBr());
			if (!head) {
				head = novi;
				posl = novi;
			}
			else {
				posl->sl = novi;
				posl = novi;
			}
		}
	}
	~Graf() {
		brisi();
	}

	int getN() {
		return n;
	}

	void uvecajN() {
		n++;
	}

	void umanjiN() {
		n--;
	}

	 void brisi() {
		while (head) {
			Cvor* pom = head;
			head = head->sl;
			delete pom;
		}
		s.clear();
		n = 0;
		head = nullptr;
	}
};

set<vector<int>> skupGrana;

Graf* prazanGraf(int n) { // stvaranje praznog grafa
	try {
		if (n > MAX || n < 0) throw false; // graf moze da ima maximalno 20 cvorova
		return new Graf(n);
	}
	catch(bool) {
		cout << "Ne mozete da stvorite graf unete velicine, probajte manje velicine." << endl;
		return nullptr;
	}
	catch (...) {
		cout << "Nepoznata greska!";
		exit(-1);
	}
}

void dodajCvor(Graf* gr) { // dodavanje cvora n+1
	try {
		if (!gr) throw gr;
		if (gr->getN() >= MAX) throw gr->getN();
		Cvor* tmp = gr->head;
		if (tmp) {
			while (tmp->sl) {
				tmp = tmp->sl;
			}
			Cvor* novi = new Cvor;
			tmp->sl = novi;
			gr->s.insert(novi->getBr()); // skup cuva sve cvorove u grafu
		}
		else {
			gr->head = new Cvor;
			gr->s.insert(gr->head->getBr()); // skup cuva sve cvorove u grafu
		}
		gr->uvecajN();
	}
	catch (Graf*) {
		cout << "Morate da stvorite graf da biste dodavali dodatne cvorove u njega." << endl;
	}
	catch (int) {
		cout << "Vec ste uneli maksimalan broj cvorova!" << endl;
	}
}

void ukloniCvor(Graf* g, int n) { // brisanje cvora n iz grafa
	
	try {
		if (!g) throw g;
		if (g->s.count(n) == 0) throw g->s.count(n);
		Cvor* tmp = g->head;
		Cvor* posl = nullptr;
		Cvor* pom;
		while (tmp) {
			for (auto i = tmp->ivice.begin(); i != tmp->ivice.end(); ++i) {
				if ((*i).first == n) {
					tmp->ivice.erase(i);
				}
			}
			if (tmp->getBr() == n) {
				pom = tmp;
				if (!posl) {
					tmp = tmp->sl;
					g->head = g->head->sl;
				}
				else {
					posl->sl = tmp->sl;
					tmp = tmp->sl;
				}
				delete pom;
			}
			else {
				posl = tmp;
				tmp = tmp->sl;
			}
		}
		g->umanjiN();
		g->s.erase(n);
	}
	catch (Graf*) {
		cout << "Nije moguce obrisati cvor, jer jos nije kreiran graf." << endl;
	}
	catch (unsigned int) {
		cout << "Cvor sa rednim brojem " << n << " se ne nalazi u grafu." << endl;
	}
	catch (...) {
		cout << "Nepoznata greska!";
		exit(-1);
	}
}

void dodajGranu(Graf* gr, int c1, int c2, int w = -1) {
	try {
		if (c1 == c2) throw (c1 == c2);
		if (!gr) throw gr;
		if (gr->s.count(c1) == 0 || gr->s.count(c2) == 0) throw gr->s.count(c1);
		vector<int> grana;
		vector<int> grana1;
		grana.push_back(c1);
		grana.push_back(c2);
		grana1.push_back(c2);
		grana1.push_back(c1);
		if (skupGrana.count(grana) == 0) {
			skupGrana.insert(grana);
			skupGrana.insert(grana1);
		}
		else {
			throw grana;
		} // u skupu su sve grane da ne bi bilo moguce 2 puta dodati istu granu
		Cvor* tmp = gr->head;
		int br = 0;
		while (tmp) {
			if (tmp->getBr() == c1) {
				tmp->ivice.push_back(make_pair(c2,w));
				++br;
			}
			else {
				if (tmp->getBr() == c2) {
					int n[] = { c1, w };
					tmp->ivice.push_back(make_pair(c1,w));
					++br;
				}
			}
			if (br == 2)
				break;
			tmp = tmp->sl;
		}
	}
	catch(bool) {
		cout << "Podrazumeva se da grana ima putanju do same sebe." << endl;
	}
	catch(Graf*) {
		cout << "Morate da stvorite graf da biste mogli da dodate grane u njega." << endl;
	}
	catch (unsigned int) {
		cout << "Ne mozete dodati granu izmedju ova dva cvora jer jedan od njih se ne nalazi u grafu." << endl;
	}
	catch (vector<int>) {
		cout << "Ova grana je vec dodata." << endl;
	}
	catch (...) {
		cout << "Nepoznata greska!";
		exit(-1);
	}
}

void ukloniGranu(Graf* gr, int c1, int c2) {
	try {
		if (!gr) throw gr;
		vector<int> g1;
		g1.push_back(c1);
		g1.push_back(c2);
		if (skupGrana.count(g1) == 0) throw g1; // ako se unese grana koja ne postoji u grafu
		vector<int> g2;
		g2.push_back(c2);
		g2.push_back(c1);
		skupGrana.erase(g2);
		skupGrana.erase(g1);
		Cvor* tmp = gr->head;
		while (tmp) {
			if (tmp->getBr() == c1)
				for (auto i = tmp->ivice.begin(); i != tmp->ivice.end(); i++) {
					if ((*i).first == c2) {
						tmp->ivice.erase(i);
						break;
					}
				}
			else if (tmp->getBr() == c2) {
				for (auto i = tmp->ivice.begin(); i != tmp->ivice.end(); i++) {
					if ((*i).first == c1) {
						tmp->ivice.erase(i);
						break;
					}
				}
			}
			tmp = tmp->sl;
		}
	}
	catch (Graf*) {
		cout << " Morate napraviti graf, pre nego sto pozelite da obrisete granu iz njega." << endl;
	}
	catch (vector<int>) {
		cout << "Ova grana ne postoji u grafu." << endl;
	}
	catch (...) {
		cout << "Nepoznata greska!";
		exit(-1);
	}
}

void pisiGraf(Graf* gr) {
	try {
		if (!gr) throw gr;
		Cvor* tmp = gr->head;
		while (tmp) {
			cout << tmp->getBr() << " -> ";
			for (unsigned int i = 0; i < tmp->ivice.size(); i++) {
				cout << tmp->ivice[i].first << " -> ";
			}
			cout << endl;
			tmp = tmp->sl;
		}
		cout << endl;
	}
	catch (Graf*) {
		cout << "Ne mozete da ispisete graf dok ga ne kreirate." << endl;
	}
	catch (...) {
		cout << "Nepoznata greska!";
		exit(-1);
	}
}

int citaj() {
	string unos;
	getline(cin, unos);
	int n;
	try {
		n = stoi(unos);
		return n;
	}
	catch (...) {
		return -1;
	}
}

int citajVise() {
	string unos;
	char c = ' ';
	getline(cin, unos, c);
	int n;
	try {
		n = stoi(unos);
		return n;
	}
	catch (...) {
		return -1;
	}
}

int main() {
	int broj, broj1;
	cout << "---Ulancana rezprezentacija grafa koriscenjem lista susednosti.---" << endl << endl;
	Graf* gr = nullptr;

	while (true) {
		
		cout << "Izaberite redni broj operacije:\n";
		cout << "\t1. Stvorite prazan graf sa proizvoljnim brojem cvorova ( bez grana)\n" <<
			"\t2. Dodajte cvor u graf.\n" <<
			"\t3. Uklonite cvor iz grafa.\n" <<
			"\t4. Dodajte granu u graf.\n" <<
			"\t5. Uklonite granu iz grafa.\n" <<
			"\t6. Ispisite graf.\n" <<
			"\t7. Izbrisite graf.\n" <<
			"\t0. Kraj programa\n";
		cout << "\nKomanda broj: ";
		int n = citaj();

		cout << endl << endl;
		
		switch (n) {
		case 0:
			delete gr;
			exit(0);
		case 1:
			cout << "Unesite broj cvorova koje zelite da uneste u prazan graf: ";
			broj = citaj();
			gr = prazanGraf(broj);
			break;
		case 2:
			dodajCvor(gr);
			break;
		case 3:
			cout << "Koji redni broj cvora zelite da izbrisete? ";
			broj = citaj();
			ukloniCvor(gr, broj);
			break;
		case 4:
			cout << "Izmedju kojih cvorova zelite da ubacite granu? ";
			broj = citajVise();
			broj1 = citaj();
			dodajGranu(gr, broj, broj1);
			break;
		case 5:
			cout << "Koju granu zelite da izbrisete iz grafa? ";
			broj = citajVise();
			broj1 = citaj();
			ukloniGranu(gr, broj, broj1);
			break;
		case 6:
			pisiGraf(gr);
			break;
		case 7:
			delete gr;
			gr = nullptr;
			Cvor().resetId();
			break;
		default:
			cout << "Nepoznata komanda, pokusajte ponovo!" << endl;
			break;
		}
	}

	return 0;
}