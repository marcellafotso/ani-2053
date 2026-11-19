#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

int main() {

    int n, c, a;
    string mot, etiquette;

    cin >> etiquette >> n;

    vector<string> avant(n);
    unordered_set<string> ensembleAvant;

    for (int i = 0; i < n; i++) {
        cin >> avant[i];
        ensembleAvant.insert(avant[i]);
    }

    cin >> etiquette >> c;

    unordered_set<string> corrige;

    for (int i = 0; i < c; i++) {
        cin >> mot;
        corrige.insert(mot);
    }

    cin >> etiquette >> a;

    unordered_set<string> apres;

    for (int i = 0; i < a; i++) {
        cin >> mot;
        apres.insert(mot);
    }

    int vraies = 0;
    int fausses = 0;
    int oubliees = 0;
    int nouvelles = 0;

    for (int i = 0; i < n; i++) {

        if (corrige.count(avant[i])) {
            cout << i + 1 << " " << avant[i]
                 << " DEVENUE_FAUSSE\n";
            fausses++;
        }
        else if (apres.count(avant[i])) {
            cout << i + 1 << " " << avant[i]
                 << " ENCORE_VRAIE\n";
            vraies++;
        }
        else {
            cout << i + 1 << " " << avant[i]
                 << " OUBLIEE\n";
            oubliees++;
        }
    }

    for (const string& x : apres) {
        if (!ensembleAvant.count(x)) {
            nouvelles++;
        }
    }

    int partOubliee = 0;

    if (vraies + oubliees != 0) {
        partOubliee = 100LL * oubliees / (vraies + oubliees);
    }

    cout << "VRAIES " << vraies << "\n";
    cout << "FAUSSES " << fausses << "\n";
    cout << "OUBLIEES " << oubliees << "\n";
    cout << "NOUVELLES " << nouvelles << "\n";
    cout << "PART_OUBLIEE " << partOubliee << "\n";

    return 0;
}