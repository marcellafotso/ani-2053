#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Commit {
    string hash;
    string action;
    vector<string> fonctions;
};

struct Annonce {
    string hash;
    string etat;
    string fonction;
};

int main() {

    int H;
    cin >> H;

    vector<Commit> commits(H);
    unordered_map<string, int> position;

    for (int i = 0; i < H; i++) {
        int k;

        cin >> commits[i].hash;
        cin >> commits[i].action;
        cin >> k;

        position[commits[i].hash] = i + 1;

        for (int j = 0; j < k; j++) {
            string f;
            cin >> f;
            commits[i].fonctions.push_back(f);
        }
    }

    int A;
    cin >> A;

    vector<Annonce> annonces(A);

    for (int i = 0; i < A; i++) {
        cin >> annonces[i].hash;
        cin >> annonces[i].etat;
        cin >> annonces[i].fonction;
    }

    // Calculer les fonctions présentes après chaque commit.

    vector<unordered_set<string>> historique(H + 1);

    for (int i = 1; i <= H; i++) {

        historique[i] = historique[i - 1];

        for (const string& f : commits[i - 1].fonctions) {

            if (commits[i - 1].action == "AJOUTE") {
                historique[i].insert(f);
            }
            else if (commits[i - 1].action == "RETIRE") {
                historique[i].erase(f);
            }
        }
    }

    int mensonges = 0;

    // Vérifier chaque annonce.

    for (int i = 0; i < A; i++) {

        int ecriture = position[annonces[i].hash];

        auto estVraie = [&](int commit) {

            bool presente =
                historique[commit].count(annonces[i].fonction) > 0;

            if (annonces[i].etat == "A_VENIR") {
                return !presente;
            }

            return presente;
        };

        cout << "ANNONCE " << i + 1 << " ";

        if (estVraie(H)) {
            cout << "VRAI\n";
        }
        else {

            int debut = H;

            while (debut > ecriture && !estVraie(debut - 1)) {
                debut--;
            }

            cout << "MENT " << commits[debut - 1].hash << "\n";

            mensonges++;
        }
    }

    cout << "MENSONGES " << mensonges << "\n";

    return 0;
}