[23/11/2026 21:53] Kris Wanko: #include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Action {
    long long n = 0;
    string a, b, c;
};

bool entier(const string& s, long long& v) {
    if (s.empty()) return false;

    size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i == s.size()) return false;

    for (; i < s.size(); ++i)
        if (s[i] < '0' || s[i] > '9') return false;

    try {
        size_t p;
        v = stoll(s, &p);
        return p == s.size();
    } catch (...) {
        return false;
    }
}

vector<string> split(const string& s, char sep) {
    vector<string> r;
    size_t debut = 0;

    while (true) {
        size_t p = s.find(sep, debut);
        if (p == string::npos) {
            r.push_back(s.substr(debut));
            break;
        }
        r.push_back(s.substr(debut, p - debut));
        debut = p + 1;
    }

    return r;
}

bool lireChamps(const string& s, int minimum, Action& a) {
    vector<string> f = split(s, ':');

    if ((int)f.size() < minimum) return false;
    if (!entier(f[0], a.n)) return false;

    a.a = f[1];
    if (minimum >= 3) a.b = f[2];
    if (minimum >= 4) a.c = f[3];

    return true;
}

bool lireDepot(const string& s, Action& a) {
    size_t p1 = s.find(':');
    if (p1 == string::npos) return false;

    size_t p2 = s.find(':', p1 + 1);
    if (p2 == string::npos) return false;

    size_t p3 = s.find(':', p2 + 1);
    if (p3 == string::npos) return false;

    if (!entier(s.substr(0, p1), a.n)) return false;

    a.a = s.substr(p1 + 1, p2 - p1 - 1);
    a.b = s.substr(p2 + 1, p3 - p2 - 1);
    a.c = s.substr(p3 + 1);

    return !a.a.empty() && !a.b.empty() && !a.c.empty();
}

int main() {
    vector<long long> captures;
    vector<Action> pans, zooms, drops, clicks;
    vector<Action> moves, packs, grids;

    long long sortie = 0;
    bool sortiePresente = false;
    int ignores = 0;

    string ligne;

    while (getline(cin, ligne)) {
        size_t egal = ligne.find('=');

        if (egal == string::npos) {
            ++ignores;
            continue;
        }

        string nom = ligne.substr(0, egal);
        string valeur = ligne.substr(egal + 1);

        if (nom == "NK_AGENT_SHOT") {
            vector<string> elements = split(valeur, ',');
            int gardes = 0;

            for (const string& e : elements) {
                long long n;

                if (!entier(e, n)) {
                    ++ignores;
                } else if (gardes < 8) {
                    captures.push_back(n);
                    ++gardes;
                } else {
                    ++ignores;
                }
            }
        }
        else if (nom == "NK_AGENT_EXIT") {
            if (!entier(valeur, sortie))
                ++ignores;
            else
                sortiePresente = true;
        }
        else if (nom == "NK_AGENT_PAN" ||
                 nom == "NK_AGENT_MOVE") {
            Action a;

            if (!lireChamps(valeur, 3, a)) {
                ++ignores;
            } else if (nom == "NK_AGENT_PAN") {
                pans.push_back(a);
            } else {
                moves.push_back(a);
            }
        }
        else if (nom == "NK_AGENT_ZOOM") {
            Action a;

            if (!lireChamps(valeur, 4, a))
                ++ignores;
            else
                zooms.push_back(a);
        }
        else if (nom == "NK_AGENT_GRID") {
            Action a;

            if (!lireChamps(valeur, 2, a))
                ++ignores;
            else
                grids.push_back(a);
        }
        else if (nom == "NK_AGENT_PACK") {
            Action a;

            if (!entier(valeur, a.n))
                ++ignores;
            else
                packs.push_back(a);
        }
        else if (nom == "NK_AGENT_DROP" ||
                 nom == "NK_AGENT_CLICK") {
            vector<string> elements = split(valeur, ';');
            int gardes = 0;

            for (const string& e : elements) {
                Action a;
                bool ok;
[23/11/2026 21:53] Kris Wanko: if (nom == "NK_AGENT_DROP")
                    ok = lireDepot(e, a);
                else
                    ok = lireChamps(e, 3, a);

                if (!ok) {
                    ++ignores;
                } else if (gardes < 8) {
                    if (nom == "NK_AGENT_DROP")
                        drops.push_back(a);
                    else
                        clicks.push_back(a);

                    ++gardes;
                } else {
                    ++ignores;
                }
            }
        }
        else {
            ++ignores;
        }
    }

    long long dernierTour = 0;

    auto chercherMax = [&](const vector<Action>& v) {
        for (const auto& a : v)
            if (a.n > dernierTour)
                dernierTour = a.n;
    };

    chercherMax(pans);
    chercherMax(zooms);
    chercherMax(drops);
    chercherMax(clicks);
    chercherMax(moves);
    chercherMax(packs);
    chercherMax(grids);

    for (long long n : captures)
        if (n > 1 && n - 1 > dernierTour)
            dernierTour = n - 1;

    if (sortiePresente && sortie > 0 &&
        sortie - 1 < dernierTour)
        dernierTour = sortie - 1;

    long long jamais = 0;

    auto compterActionsJamais = [&](const vector<Action>& v) {
        for (const auto& a : v) {
            if (a.n <= 0 ||
                (sortiePresente && sortie > 0 && a.n >= sortie))
                ++jamais;
        }
    };

    compterActionsJamais(pans);
    compterActionsJamais(zooms);
    compterActionsJamais(drops);
    compterActionsJamais(clicks);
    compterActionsJamais(moves);
    compterActionsJamais(packs);
    compterActionsJamais(grids);

    for (long long n : captures) {
        if (n <= 0 ||
            (sortiePresente && sortie > 0 && n > sortie))
            ++jamais;
    }

    for (long long t = 0; t <= dernierTour; ++t) {
        for (const auto& a : pans)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " PAN "
                     << a.a << " " << a.b << "\n";

        for (const auto& a : zooms)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " ZOOM "
                     << a.a << " " << a.b << " " << a.c << "\n";

        for (const auto& a : drops)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " DEPOT "
                     << a.c << " " << a.a << " " << a.b << "\n";

        for (const auto& a : clicks)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " CLIC "
                     << a.a << " " << a.b << "\n";

        for (const auto& a : moves)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " MOVE "
                     << a.a << " " << a.b << "\n";

        for (const auto& a : packs)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " RANGE\n";

        for (const auto& a : grids)
            if (a.n == t && t > 0)
                cout << "TOUR " << t << " GRILLE "
                     << a.a << "\n";

        long long frame = t + 1;

        for (long long n : captures)
            if (n == frame)
                cout << "TOUR " << frame << " CAPTURE\n";

        if (sortiePresente && sortie > 0 && frame >= sortie) {
            cout << "ARRET " << t << "\n";
            cout << "JAMAIS " << jamais << "\n";
            cout << "IGNORES " << ignores << "\n";
            return 0;
        }
    }

    cout << "SANS_ARRET\n";
    cout << "JAMAIS " << jamais << "\n";
    cout << "IGNORES " << ignores << "\n";

    return 0;
}