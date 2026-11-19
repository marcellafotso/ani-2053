#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    vector<string> pileUtilisateur;
    vector<int> traitsOutil;

    string ligne;
    int numeroTrait = 0;
    int numeroAnnulation = 0;

    int justes = 0;
    int riens = 0;
    int trahis = 0;

    int operations = 0;
    int annulables = 0;

    while (getline(cin, ligne)) {
        if (ligne.empty()) {
            continue;
        }

        stringstream ss(ligne);
        string commande;
        ss >> commande;

        if (commande == "ANNULE") {
            numeroAnnulation++;

            string attendu = "AUCUNE";
            string obtenu = "RIEN";

            // Annulation attendue par l'utilisateur
            if (!pileUtilisateur.empty()) {
                attendu = pileUtilisateur.back();
                pileUtilisateur.pop_back();
            }

            // Annulation effectuee par l'outil
            if (!traitsOutil.empty()) {
                int n = traitsOutil.back();
                traitsOutil.pop_back();
                obtenu = "TRAIT " + to_string(n);
            }

            string verdict;

            if (obtenu == "RIEN") {
                if (attendu == "AUCUNE") {
                    verdict = "JUSTE";
                } else {
                    verdict = "RIEN";
                }
            } else if (obtenu == attendu) {
                verdict = "JUSTE";
            } else {
                verdict = "TRAHI";
            }

            if (verdict == "JUSTE") justes++;
            else if (verdict == "RIEN") riens++;
            else trahis++;

            cout << "ANNULE " << numeroAnnulation
                 << " ATTENDU " << attendu
                 << " OBTENU " << obtenu
                 << " " << verdict << "\n";
        }
        else {
            operations++;

            if (commande == "TRAIT") {
                numeroTrait++;
                annulables++;

                string trait = "TRAIT " + to_string(numeroTrait);
                pileUtilisateur.push_back(trait);
                traitsOutil.push_back(numeroTrait);
            }
            else {
                pileUtilisateur.push_back(ligne);

                if (commande == "EFFACE") {
                    traitsOutil.clear();
                }
            }
        }
    }

    cout << "JUSTES " << justes << "\n";
    cout << "RIENS " << riens << "\n";
    cout << "TRAHIS " << trahis << "\n";
    cout << "ANNULABLES " << annulables
         << " SUR " << operations << "\n";

    return 0;
}