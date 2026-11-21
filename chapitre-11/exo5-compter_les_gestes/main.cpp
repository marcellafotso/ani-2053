#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    string ligne;
    bool enCours = false;

    string nom;
    long long debut = 0;
    long long minimum = 0;
    long long parJour = 0;
    long long gestes = 0;
    long long attente = 0;
    long long totalJour = 0;

    while (getline(cin, ligne)) {
        istringstream iss(ligne);

        long long t;
        string commande;

        if (!(iss >> t >> commande)) {
            continue;
        }

        if (commande == "DEBUT") {
            iss >> nom >> minimum >> parJour;

            debut = t;
            gestes = 0;
            attente = 0;
            enCours = true;
        }
        else if (commande == "FIN") {
            string nomFin;
            iss >> nomFin;

            if (enCours) {
                long long duree = t - debut;
                long long enTrop = max(0LL, gestes - minimum);
                long long jourSecondes = (attente * parJour) / 1000;

                totalJour += jourSecondes;

                cout << nom
                     << " GESTES " << gestes
                     << " EN_TROP " << enTrop
                     << " DUREE " << duree
                     << " ATTENTE " << attente
                     << " JOUR " << jourSecondes / 60
                     << ":" << setw(2) << setfill('0')
                     << jourSecondes % 60 << '\n';

                enCours = false;
            }
        }
        else if (commande == "ATTENTE") {
            long long d;
            iss >> d;

            if (enCours) {
                attente += d;
            }
        }
        else if (commande == "CLIC" ||
                 commande == "TOUCHE" ||
                 commande == "MOLETTE" ||
                 commande == "GLISSE") {

            if (enCours) {
                gestes++;
            }
        }
    }

    if (enCours) {
        cout << nom << " INACHEVEE\n";
    }

    cout << "TOTAL_JOUR "
         << totalJour / 60
         << ":" << setw(2) << setfill('0')
         << totalJour % 60 << '\n';

    return 0;
}