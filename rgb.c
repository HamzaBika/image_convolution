/*******************************************************************************
 * Programme de traitement d'image
 * 
 * Ce programme permet de réaliser différentes opérations de traitement d'image:
 * - Affichage et sauvegarde d'images
 * - Extraction de composantes couleur (RGB)
 * - Détection de contours (filtre de Sobel)
 * - Application de flou gaussien
 * 
 * Format d'entrée: fichier texte contenant les dimensions et valeurs RGB
 * Format de sortie: fichiers PNG
 * 
 * *********** Réalisé par: ***************

 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define M_PI 3.14159265358979323846

// Image struct
typedef struct
{
    int r;        // Resolution
    int l;        // Longueur (width)
    int h;        // Hauteur (height)
    int **img[3]; // Matrice : r = 0 (rouge), g = 1 (vert), b = 2 (bleu)
} image;

// Grey Image struct
typedef struct
{
    int r;        // Resolution
    int l;        // Longueur (width)
    int h;        // Hauteur (height)
    int **img;    // Matrice de niveaux de gris
} grey_image;

// Création de matrice d'entiers
int **creationMatrice(int r, int h, int l)
{
    // Allocation de mémoire pour la matrice
    int **k = (int **)malloc(sizeof(int *) * r * h);
    for (int i = 0; i < r * h; i++)
    {
        k[i] = (int *)malloc(sizeof(int) * r * l);
    }
    return k;
}

// Création de matrice de doubles
double **creationMatricedouble(int r, int h, int l)
{
    // Allocation de mémoire pour la matrice de doubles
    double **k = (double **)malloc(sizeof(double *) * r * h);
    for (int i = 0; i < r * h; i++)
    {
        k[i] = (double *)malloc(sizeof(double) * r * l);
    }
    return k;
}

// Initialisation par des valeurs aléatoires
void initmatricerand(int **m, int r, int h, int l)
{
    // Génère des valeurs aléatoires entre 0 et 255
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            m[i][j] = rand() % 256;
        }
    }
}

// Initialisation par saisie au clavier
void initmatricekey(int **m, int r, int h, int l)
{
    // Saisie des valeurs par l'utilisateur
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            printf("M[%d][%d]:", i, j);
            scanf("%d", *(m + i) + j);
        }
    }
}

// Initialisation à partir d'un fichier
image initmatricefile(char *filename)
{
    // Ouverture du fichier
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    image g;
    // Lecture des dimensions r, l, h
    if (fscanf(f, "%d,%d,%d ", &g.r, &g.l, &g.h) != 3)
    {
        fprintf(stderr, "Erreur : format incorrect.\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    printf("Dimensions : resolution=%d, longueur=%d, hauteur=%d\n", g.r, g.l, g.h);

    // Allocation et lecture des matrices RGB
    for (int i = 0; i < 3; i++)
    {
        g.img[i] = creationMatrice(g.r, g.h, g.l);
        if (g.img[i] == NULL)
        {
            fprintf(stderr, "Erreur : echec de l'allocation mémoire pour img[%d].\n", i);
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

    // Lecture des valeurs RGB
    for (int i = 0; i < g.h * g.r; i++)
    {
        for (int j = 0; j < g.l * g.r; j++)
        {
            if (fscanf(f, "(%d.%d.%d) ", &g.img[0][i][j], &g.img[1][i][j], &g.img[2][i][j]) != 3)
            {
                fprintf(stderr, "Erreur : données RGB mal formatees a (%d, %d).\n", i, j);
                fclose(f);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(f);
    return g;
}
// enregistrer l image dans un fichier 
void save_image_file(char *filename, image g)
{
    // Ouverture du fichier
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Écriture des dimensions
    fprintf(f, "%d %d %d\n", g.r, g.l, g.h);

    // Écriture des valeurs RGB des pixels
    for (int i = 0; i < g.h * g.r; i++)
    {
        for (int j = 0; j < g.l * g.r; j++)
        {
            fprintf(f, "(%d.%d.%d) ", 
                    g.img[0][i][j], // Rouge
                    g.img[1][i][j], // Vert
                    g.img[2][i][j]  // Bleu
            );
        }
        fprintf(f, "\n");
    }

    fclose(f);
}


// Affichage de l'image en console
void afficherimg(image m, int r, int h, int l)
{
    // Format d'affichage : (R.G.B)
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            printf("(%d.%d.%d) ", m.img[0][i][j], m.img[1][i][j], m.img[2][i][j]);
        }
        printf("\n");
    }
}

// Affichage d'une matrice 2D de doubles
void afficher2d(double **m, int l, int c)
{
    // Affiche chaque élément de la matrice
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
}

// Extraction d'une composante couleur
grey_image extraire(image m, int c)
{
    // Initialisation de l'image en niveaux de gris
    grey_image g;
    g.img = creationMatrice(m.r, m.h, m.l);
    g.l = m.l;
    g.r = m.r;
    g.h = m.h;
    
    // Copie de la composante sélectionnée
    for (int i = 0; i < m.h * m.r; i++)
    {
        for (int j = 0; j < m.l * m.r; j++)
        {
            g.img[i][j] = m.img[c][i][j];
        }
    }
    return g;
}

// Sauvegarde d'une image en niveaux de gris au format PNG
void saveGreyImageAsPNG(grey_image g, const char *filename)
{
    int width = g.l * g.r;
    int height = g.h * g.r;
    // Allocation du buffer pour l'image
    unsigned char *data = malloc(width * height);

    // Conversion et limitation des valeurs entre 0 et 255
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int pixelValue = g.img[i][j];
            if (pixelValue > 255) pixelValue = 255;
            if (pixelValue < 0) pixelValue = 0;
            data[i * width + j] = (unsigned char)pixelValue;
        }
    }

    stbi_write_png(filename, width, height, 1, data, width);
    free(data);
}

// Création du noyau gaussien pour le flou
double **guasse_ker(float ecart, int *size_ker)
{
    // Calcul de la taille du noyau
    int size = 2 * (int)ceil(3 * ecart) + 1;
    *size_ker = size;
    double **ker = creationMatricedouble(1, size, size);
    int offset = size / 2;
    int x, y;
    double somme = 0.0;

    // Calcul des coefficients gaussiens
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            x = i - offset;
            y = j - offset;
            ker[i][j] = ((1.0 / (2.0 * M_PI * ecart * ecart)) * exp(-((x * x + y * y) / (2 * ecart * ecart))));
            somme += ker[i][j];
        }
    }
    return ker;
}

// Application de la convolution (flou gaussien)
image convolution(image img, double **k, int size_ker)
{
    // Initialisation de l'image résultat
    image result;
    result.h = img.h;
    result.l = img.l;
    result.r = img.r;
    
    // Allocation et initialisation des matrices
    for (int i = 0; i < 3; i++)
    {
        result.img[i] = creationMatrice(result.r, result.h, result.l);
        for (int x = 0; x < result.h * result.r; x++)
        {
            for (int y = 0; y < result.l * result.r; y++)
            {
                result.img[i][x][y] = 0;
            }
        }
    }

    int l = img.l * img.r;
    int h = img.h * img.r;

    // Application de la convolution
    for (int c = 0; c < 3; c++)
    {
        for (int i = size_ker / 2; i < h - size_ker / 2; i++)
        {
            for (int j = size_ker / 2; j < l - size_ker / 2; j++)
            {
                for (int ik = 0; ik < size_ker; ik++)
                {
                    for (int jk = 0; jk < size_ker; jk++)
                    {
                        result.img[c][i][j] += (int)(k[ik][jk] * img.img[c][i + ik - size_ker / 2][j + jk - size_ker / 2]);
                    }
                }
            }
        }
    }

    // Gestion des bords de l'image
    for (int c = 0; c < 3; c++)
    {
        // Bords horizontaux
        for (int i = 0; i < size_ker / 2; i++)
        {
            for (int j = 0; j < l; j++)
            {
                result.img[c][i][j] = img.img[c][i][j];
                result.img[c][h - 1 - i][j] = img.img[c][h - 1 - i][j];
            }
        }
        // Bords verticaux
        for (int i = size_ker / 2; i < h - size_ker / 2; i++)
        {
            for (int j = 0; j < size_ker / 2; j++)
            {
                result.img[c][i][j] = img.img[c][i][j];
                result.img[c][i][l - 1 - j] = img.img[c][i][l - 1 - j];
            }
        }
    }

    return result;
}

// Détection de contours avec l'opérateur de Sobel
int **detection_de_contours(int **m, int r, int h, int l)
{
    int somme_x = 0, somme_y = 0;
    
    // Définition des kernels de Sobel
    float sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    float sobel_y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    // Création de la matrice de sortie
    int **output = creationMatrice(r, h, l);

    // Application des filtres de Sobel
    for (int i = 1; i < r * h - 1; i++)
    {
        for (int j = 1; j < l * r - 1; j++)
        {
            somme_x = 0;
            somme_y = 0;

            // Calcul des gradients
            for (int ki = 0; ki < 3; ki++)
            {
                for (int kj = 0; kj < 3; kj++)
                {
                    somme_x += sobel_x[ki][kj] * m[i + ki - 1][j + kj - 1];
                    somme_y += sobel_y[ki][kj] * m[i + ki - 1][j + kj - 1];
                }
            }

            // Calcul de la magnitude du gradient
            output[i][j] = (int)round(sqrt(somme_x * somme_x + somme_y * somme_y));
        }
    }

    // Initialisation des bords à 0
    for (int i = 0; i < r * h; i++)
    {
        output[i][0] = 0;
        output[i][l - 1] = 0;
    }
    for (int j = 0; j < l * r; j++)
    {
        output[0][j] = 0;
        output[r - 1][j] = 0;
    }

    return output;
}

// Création de l'image après détection des contours
grey_image detect_image(image m, int c)
{
    // Initialisation de la nouvelle image
    grey_image nv;
    nv.h = m.h;
    nv.l = m.l;
    nv.r = m.r;

    // Application de la détection de contours
    nv.img = detection_de_contours(m.img[c], m.r, m.h, m.l);

    return nv;
}

// Sauvegarde d'une image couleur au format PNG
void saveImageAsPNG(image img, const char *filename)
{
    int width = img.l * img.r;
    int height = img.h * img.r;
    // Allocation du buffer pour l'image RGB
    unsigned char *data = malloc(3 * width * height);

    // Copie des données RGB
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = (i * width + j) * 3;
            data[index] = img.img[0][i][j];     // Canal rouge
            data[index + 1] = img.img[1][i][j]; // Canal vert
            data[index + 2] = img.img[2][i][j]; // Canal bleu
        }
    }

    stbi_write_png(filename, width, height, 3, data, width * 3);
    free(data);
}

int main() {
    int choix, sous_choix;
    char nom_fichier[256] = "matrice.txt";
    
    // Variables pour l'image
    image img_originale;
    image randimg;
    grey_image img_grise = {0};
    grey_image img_contours = {0};
    double **noyau_gaussien = NULL;
    int taille_noyau = 0;
    float ecart_type = 2.0;
    
    printf("Chargement de l'image depuis %s...\n", nom_fichier);
    img_originale = initmatricefile(nom_fichier);
    printf("Image chargee avec succes.\n");
    
    while (1) {
        printf("\n=============== Menu de Traitement d'Image ===============\n");
        printf("1. Afficher et enregistrer l'image\n");
        printf("2. Extraire composante(s) couleur\n");
        printf("3. Detection des contours\n");
        printf("4. Appliquer flou gaussien\n");
        printf("0. Quitter\n");
        printf("\n=============== Partie independante de l image precedente ===============\n");
        printf("6. Creation d image aleatoire\n");
        printf("0. Quitter\n");
        printf("\nVotre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            
            case 1: {
                char nom_sauvegarde[] = "image_originale.png";
                printf("\nAffichage de l'image :\n");
                afficherimg(img_originale, img_originale.r, img_originale.h, img_originale.l);
                saveImageAsPNG(img_originale, nom_sauvegarde);
                printf("\n>>> Image enregistree avec succes dans le fichier : %s\n", nom_sauvegarde);
                break;
            }

            case 2: {
                printf("\nExtraction de composante(s) :\n");
                printf("0. Rouge uniquement\n");
                printf("1. Vert uniquement\n");
                printf("2. Bleu uniquement\n");
                printf("3. Toutes les composantes\n");
                printf("Votre choix : ");
                scanf("%d", &sous_choix);
                
                if (sous_choix >= 0 && sous_choix <= 3) {
                    if (sous_choix == 3) {
                        // Extraire toutes les composantes
                        char noms_fichiers[3][50] = {
                            "composante_rouge.png",
                            "composante_verte.png",
                            "composante_bleue.png"
                        };
                        for (int i = 0; i < 3; i++) {
                            img_grise = extraire(img_originale, i);
                            saveGreyImageAsPNG(img_grise, noms_fichiers[i]);
                            printf(">>> Image enregistree avec succes dans le fichier : %s\n", noms_fichiers[i]);
                        }
                    } else {
                        // Extraire une seule composante
                        char nom_sauvegarde[50];
                        const char *noms_composantes[] = {"rouge", "verte", "bleue"};
                        snprintf(nom_sauvegarde, 50, "composante_%s.png", noms_composantes[sous_choix]);
                        img_grise = extraire(img_originale, sous_choix);
                        saveGreyImageAsPNG(img_grise, nom_sauvegarde);
                        printf(">>> Image enregistree avec succes dans le fichier : %s\n", nom_sauvegarde);
                    }
                } else {
                    printf("Choix invalide!\n");
                }
                break;
            }

            case 3: {
                printf("\nDetection des contours :\n");
                printf("0. Rouge uniquement\n");
                printf("1. Vert uniquement (recommande)\n");
                printf("2. Bleu uniquement\n");
                printf("3. Toutes les composantes\n");
                printf("Votre choix : ");
                scanf("%d", &sous_choix);
                
                if (sous_choix >= 0 && sous_choix <= 3) {
                    if (sous_choix == 3) {
                        // Détecter les contours sur toutes les composantes
                        char noms_fichiers[3][50] = {
                            "contours_rouge.png",
                            "contours_vert.png",
                            "contours_bleu.png"
                        };
                        for (int i = 0; i < 3; i++) {
                            img_contours = detect_image(img_originale, i);
                            saveGreyImageAsPNG(img_contours, noms_fichiers[i]);
                            printf(">>> Image enregistree avec succes dans le fichier : %s\n", noms_fichiers[i]);
                        }
                    } else {
                        // Détecter les contours sur une seule composante
                        char nom_sauvegarde[50];
                        const char *noms_composantes[] = {"rouge", "vert", "bleu"};
                        snprintf(nom_sauvegarde, 50, "contours_%s.png", noms_composantes[sous_choix]);
                        img_contours = detect_image(img_originale, sous_choix);
                        saveGreyImageAsPNG(img_contours, nom_sauvegarde);
                        printf(">>> Image enregistree avec succes dans le fichier : %s\n", nom_sauvegarde);
                    }
                } else {
                    printf("Choix invalide!\n");
                }
                break;
            }

            case 4: {
                printf("\nFlou Gaussien :\n");
                printf("1. Utiliser l'ecart-type par defaut (%.1f)\n", ecart_type);
                printf("2. Choisir un nouvel ecart-type\n");
                printf("Votre choix : ");
                scanf("%d", &sous_choix);

                if (sous_choix == 2) {
                    printf("Entrez l'ecart-type (valeur recommandee entre 1.0 et 3.0) : ");
                    scanf("%f", &ecart_type);
                }

                // Libérer l'ancien noyau si existant
                if (noyau_gaussien != NULL) {
                    for (int i = 0; i < taille_noyau; i++) {
                        free(noyau_gaussien[i]);
                    }
                    free(noyau_gaussien);
                }

                // Créer et afficher le nouveau noyau
                noyau_gaussien = guasse_ker(ecart_type, &taille_noyau);
                printf("\nNoyau Gaussien genere (taille %dx%d) :\n", taille_noyau, taille_noyau);
                afficher2d(noyau_gaussien, taille_noyau, taille_noyau);

                char nom_sauvegarde[50];
                snprintf(nom_sauvegarde, 50, "flou_gaussien_%.1f.png", ecart_type);
                printf("\nApplication du flou gaussien...\n");
                image img_floue = convolution(img_originale, noyau_gaussien, taille_noyau);
                saveImageAsPNG(img_floue, nom_sauvegarde);
                printf(">>> Image enregistree avec succes dans le fichier : %s\n", nom_sauvegarde);
                break;
            }
            case 6: {
                int r,l,h;
                char nom_sauvegarde[] = "image_aleatoire.png";
                char nom_sauvegarde2[] = "image_aleatoire.txt";
                printf("\nResolution :");
                scanf("%d",&r);
                printf("\nLongueure :");
                scanf("%d",&l);
                printf("\nHauteure :");
                scanf("%d",&h);
                randimg.r=r;
                randimg.l=l;
                randimg.h=h;
                for (int i= 0 ; i< 3 ; i++ ){
                    randimg.img[i]=creationMatrice(r,h,l);
                    initmatricerand(randimg.img[i],r,h,l);
                }
                afficherimg(randimg, randimg.r, randimg.h, randimg.l);
                save_image_file(nom_sauvegarde2,randimg);
                saveImageAsPNG(randimg, nom_sauvegarde);
                printf("\n>>> Image enregistree avec succes dans le fichier : %s %s\n", nom_sauvegarde,nom_sauvegarde2);

                break;
            }

            case 0: {
                printf("Programme termine.\n");
                // Nettoyage de la mémoire
                if (noyau_gaussien != NULL) {
                    for (int i = 0; i < taille_noyau; i++) {
                        free(noyau_gaussien[i]);
                    }
                    free(noyau_gaussien);
                }
                exit(0);
            }

            default:
                printf("Choix invalide! Veuillez reessayer.\n");
        }
    }
    return 0;
}