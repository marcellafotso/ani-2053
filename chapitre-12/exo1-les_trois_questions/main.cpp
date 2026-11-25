#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

struct Composant {
    string nom;
    string lieu;
    string touche;
    string filet;
    string partieIntouchable;
};

int main() {
    int k;
    string mot;

    if (!(cin >> mot >> k)) {
        return 0;
    }

    unordered_set<string> intouchables;

    for (int i = 0; i < k; ++i) {
        cin >> mot;
        intouchables.insert(mot);
    }

    int n;
    cin >> n;

    vector<Composant> composants;
    vector<string> prets;

    int purs = 0;
    int aRevoir = 0;
    int filetsAPoser = 0;

    for (int i = 0; i < n; ++i) {
        Composant c;

        int b, t, e;
        bool besoinGlue = false;
        bool besoinDecouper = false;

        cin >> c.nom >> b;

        for (int j = 0; j < b; ++j) {
            cin >> mot;

            if (mot == "FENETRE" ||
                mot == "SOURIS" ||
                mot == "CLAVIER" ||
                mot == "GPU") {
                besoinGlue = true;
            }

            if (mot == "DISQUE" ||
                mot == "HORLOGE" ||
                mot == "RESEAU") {
                besoinDecouper = true;
            }
        }

        if (besoinGlue) {
            c.lieu = "GLUE";
        } else if (besoinDecouper) {
            c.lieu = "DECOUPER";
        } else {
            c.lieu = "FICHIER_PUR";
            ++purs;
        }

        cin >> t;

        c.touche = "INTACT";
        c.partieIntouchable = "";

        for (int j = 0; j < t; ++j) {
            cin >> mot;

            if (c.touche == "INTACT" &&
                intouchables.count(mot)) {
                c.touche = "REVOIR";
                c.partieIntouchable = mot;
            }
        }

        if (c.touche == "REVOIR") {
            ++aRevoir;
        }

        cin >> e;

        if (e == 0) {
            c.filet = "FILET_D_ABORD";
            ++filetsAPoser;
        } else {
            c.filet = "FILET_EXISTANT";
        }

        if (c.touche == "INTACT" &&
            c.filet == "FILET_EXISTANT") {
            prets.push_back(c.nom);
        }

        composants.push_back(c);
    }

    for (const Composant& c : composants) {
        cout << c.nom << " " << c.lieu << " "
             << c.touche;

        if (c.touche == "REVOIR") {
            cout << " " << c.partieIntouchable;
        }

        cout << " " << c.filet << "\n";
    }

    cout << "PURS " << purs << "\n";
    cout << "A_REVOIR " << aRevoir << "\n";
    cout << "FILETS_A_POSER " << filetsAPoser << "\n";

    cout << "PRETS";

    if (prets.empty()) {
        cout << " AUCUN";
    } else {
        for (const string& nom : prets) {
            cout << " " << nom;
        }
    }

    cout << "\n";

    return 0;
}