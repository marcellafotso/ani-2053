#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "NkRef/NkRefFile.h"
#include "NKTime/NkChrono.h"

using namespace nkentseu;
using namespace nkref;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Utilisation : mesure.exe nombre_images\n";
        return 1;
    }

    int N = std::atoi(argv[1]);

    if (N <= 0) {
        std::cout << "Le nombre d'images doit etre positif.\n";
        return 1;
    }

    // Creation de la planche
    NkRefBoard board;

    // Donnees source inventees : 1000 octets
    uint8 source[1000];

    for (int i = 0; i < 1000; i++) {
        source[i] = static_cast<uint8>(i % 256);
    }

    // Tableau des images source
    NkRefFileImage* images = new NkRefFileImage[N];

    // Ajout des images
    for (int i = 0; i < N; i++) {
        NkVec2f position(
            static_cast<float32>(i * 120),
            static_cast<float32>(i * 90)
        );

        board.AddItem(position, 100, 80, NkString());

        images[i].data = source;
        images[i].size = 1000;
    }

    // Vue par defaut
    NkRefView view;

    // Resultat de la serialisation
    NkVector<uint8> donnees;

    // Mesure de 20 serialisations
    NkChrono chronoEcriture;

    for (int i = 0; i < 20; i++) {
        donnees.Clear();

        NkRefSerialize(
            donnees,
            board,
            images,
            view,
            false
        );
    }

    double tempsEcriture =
        chronoEcriture.Elapsed().ToMilliseconds() / 20.0;

    // Variables pour la deserialisation
    NkVector<NkRefLoadedItem> loadedItems;
    NkVector<NkRefStroke> loadedStrokes;

    NkRefView loadedView;
    bool darkTheme = false;
    bool showGrid = true;

    // Mesure de 20 deserialisations
    NkChrono chronoLecture;

    for (int i = 0; i < 20; i++) {
        loadedItems.Clear();
        loadedStrokes.Clear();

        NkRefDeserialize(
            donnees.Data(),
            donnees.Size(),
            loadedItems,
            loadedStrokes,
            loadedView,
            darkTheme,
            showGrid
        );
    }

    double tempsLecture =
        chronoLecture.Elapsed().ToMilliseconds() / 20.0;

    // Affichage des resultats
    std::printf(
        "images : %d, bytes : %zu, write : %.3f ms, read : %.3f ms\n",
        N,
        static_cast<size_t>(donnees.Size()),
        tempsEcriture,
        tempsLecture
    );

    delete[] images;

    return 0;
}