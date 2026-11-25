 #include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>

using namespace std;

struct Composant {
    long long intervalle = 0;
    long long temps = 0;
    bool change = false;
    unordered_set<string> fautes;

    void touch() {
        change = true;

        if (fautes.count("DEPUIS_MODIF")) {
            temps = 0;
        }
    }

    bool tick(long long d) {
        if (fautes.count("ZERO_IGNORE") && d <= 0) {
            return false;
        }

        if (d > 0 || fautes.count("NEGATIF_COMPTE")) {
            temps += d;
        }

        bool modification = change || fautes.count("SANS_SALETE");

        if (fautes.count("STRICT")) {
            return modification && temps > intervalle;
        }

        return modification && temps >= intervalle;
    }

    void saved() {
        if (!fautes.count("SAVED_GARDE_SALETE")) {
            change = false;
        }

        if (!fautes.count("SAVED_GARDE_TEMPS")) {
            temps = 0;
        }
    }

    string secours(const string& document) {
        if (fautes.count("SECOURS_FIXE")) {
            return "secours.nkref.auto";
        }

        if (fautes.count("SECOURS_EST_DOC")) {
            return document;
        }

        if (fautes.count("SANS_NOM_VIDE") && document.empty()) {
            return "";
        }

        if (document.empty()) {
            return "planche-sans-nom.nkref.auto";
        }

        return document + ".auto";
    }
};

bool executerTicks(Composant& c, int nombre, long long d) {
    bool reponse = false;

    for (int i = 0; i < nombre; ++i) {
        if (c.tick(d)) {
            reponse = true;
        }
    }

    return reponse;
}

int main() {
    string ligne;

    vector<string> anglesMorts;
    int detectes = 0;
    int nonDetectes = 0;

    while (getline(cin, ligne)) {
        if (ligne.empty()) {
            continue;
        }

        stringstream ss(ligne);
        string nom, faute;

        ss >> nom;

        Composant modele;
        bool estFautive = false;

        while (ss >> faute) {
            if (faute != "AUCUN") {
                modele.fautes.insert(faute);
                estFautive = true;
            }
        }

        vector<int> echecs;

        // Test 1
        {
            Composant c = modele;
            c.intervalle = 60;

            if (executerTicks(c, 3600, 1)) {
                echecs.push_back(1);
            }
        }

        // Tests 2 et 3 : même composant
        {
            Composant c = modele;
            c.intervalle = 60;
            c.touch();

            if (executerTicks(c, 59, 1)) {
                echecs.push_back(2);
            }

            if (!c.tick(1)) {
                echecs.push_back(3);
            }
        }

        // Tests 4 et 5 : même composant
        {
            Composant c = modele;
            c.intervalle = 60;
            c.touch();

            executerTicks(c, 60, 1);
            c.saved();

            if (executerTicks(c, 120, 1)) {
                echecs.push_back(4);
            }

            c.touch();

            if (!c.tick(0)) {
                echecs.push_back(5);
            }
        }

        // Test 6
        {
            Composant c = modele;
            c.intervalle = 10;
            c.touch();
            c.saved();

            if (executerTicks(c, 100, 1)) {
                echecs.push_back(6);
            }
        }

        // Test 7
        {
            string document = "D:/planches/personnages.nkref";

            if (modele.secours(document) == document) {
                echecs.push_back(7);
            }
        }

        // Test 8
        {
            string document = "D:/planches/personnages.nkref";

            string resultat = modele.secours(document);

            if (resultat.rfind(document, 0) != 0) {
                echecs.push_back(8);
            }
        }

        // Test 9
        {
            if (modele.secours("") == "") {
                echecs.push_back(9);
            }
        }
 if (echecs.empty()) {
            cout << nom << " PASSE\n";

            if (estFautive) {
                ++nonDetectes;
                anglesMorts.push_back(nom);
            }
        } else {
            cout << nom << " ECHOUE";

            for (int numero : echecs) {
                cout << " " << numero;
            }

            cout << "\n";

            if (estFautive) {
                ++detectes;
            }
        }
    }

    cout << "DETECTES " << detectes << "\n";
    cout << "NON_DETECTES " << nonDetectes << "\n";

    cout << "ANGLE_MORT";

    if (anglesMorts.empty()) {
        cout << " AUCUN";
    } else {
        for (const string& nom : anglesMorts) {
            cout << " " << nom;
        }
    }

    cout << "\n";

    return 0;
}