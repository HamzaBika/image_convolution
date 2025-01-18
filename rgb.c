#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define M_PI 3.14159265358979323846

// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// ***********************************      Q   U    1       *************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// Image struct
typedef struct
{
    int r;        // resolution
    int l;        // longueure
    int h;        // hauteure
    int **img[3]; // matrice r= 0 g = 1 b = 2
} image;
// Grey Image struct
typedef struct
{
    int r;
    int l;
    int h;
    int **img;
} grey_image;
// Creation de matrice d entier
int **creationMatrice(int r, int h, int l)
{
    int **k = (int **)malloc(sizeof(int *) * r * h);
    for (int i = 0; i < r * h; i++)
    {
        k[i] = (int *)malloc(sizeof(int) * r * l);
    }
    return k;
}

// Creation de matrice d entier
double **creationMatricedouble(int r, int h, int l)
{
    double **k = (double **)malloc(sizeof(double *) * r * h);
    for (int i = 0; i < r * h; i++)
    {
        k[i] = (double *)malloc(sizeof(double) * r * l);
    }
    return k;
}
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// ***********************************      Q   U    2         *************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************

// Initialisation par des valeurs aleatoires
void initmatricerand(int **m, int r, int h, int l)
{
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            m[i][j] = rand() % 256;
        }
    }
}

// Initialisation par  Saisie sur le clavier

void initmatricekey(int **m, int r, int h, int l)
{
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            printf("M[%d][%d]:", i, j);
            scanf("%d", *(m + i) + j);
        }
    }
}
// Initialisation par  fichier

image initmatricefile(char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    image g;

    // Lire les dimensions r, l, h
    if (fscanf(f, "%d,%d,%d ", &g.r, &g.l, &g.h) != 3)
    { // fscanf retoune le nombre des elements scanne
        fprintf(stderr, "Erreur : format incorrect .\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    printf("Dimensions : resolution=%d, longueur=%d, hauteur=%d\n", g.r, g.l, g.h);

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

// Affichage de l image en console sous forme (x,x,x) (x,x,x) (x,x,x) (x,x,x)
void afficherimg(image m, int r, int h, int l)
{
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            printf("(%d.%d.%d) ", m.img[0][i][j], m.img[1][i][j], m.img[2][i][j]);
        }
        printf("\n");
    }
}

// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// ***********************************      Q   U    3          *************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************

// Affichage d une matrice a 2 dimension
void afficher2d(double **m, int l, int c)
{
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
}

// Extraire une des composantes (rouge / vertes / blue ) de l image
grey_image extraire(image m, int c)
{
    grey_image g;
    g.img = creationMatrice(m.r, m.h, m.l);
    g.l = m.l;
    g.r = m.r;
    g.h = m.h;
    for (int i = 0; i < m.h * m.r; i++)
    {
        for (int j = 0; j < m.l * m.r; j++)
        {
            g.img[i][j] = m.img[c][i][j];
        }
    }
    return g;
}
// Fonction pour sauvegarder une image en niveaux de gris

// Fonction pour sauvegarder une image en niveaux de gris
void saveGreyImageAsPNG(grey_image g, const char *filename)
{
    int width = g.l * g.r;
    int height = g.h * g.r;
    unsigned char *data = malloc(width * height); // Un seul canal pour le gris

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Limiter les valeurs entre 0 et 255
            int pixelValue = g.img[i][j];
            if (pixelValue > 255)
                pixelValue = 255;
            if (pixelValue < 0)
                pixelValue = 0;

            data[i * width + j] = (unsigned char)pixelValue;
        }
    }

    // Le '1' indique un seul canal (niveau de gris)
    stbi_write_png(filename, width, height, 1, data, width);
    free(data);
}

// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// ***********************************      Q   U    4          *************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************

// Filtre de flous

// int ** flou(int **m,int r, int h, int l){
//     float somme=0.0;
//     // intialisation de la matrice

//     float kernel[3][3]={
//         {1.0/16.0,1.0/16.0,1.0/16.0},
//         {1.0/16.0,1.0/16.0,1.0/16.0},
//         {1.0/16.0,1.0/16.0,1.0/16.0}
//     };

//     // nouvel matrice
//     int ** output=creationMatrice(r,h,l);
//     for (int i=1;i<r*h-1;i++){
//         for (int j=1;j<r*l-1;j++){
//             somme=0;
//             for (int ki=0;ki<3;ki++){
//                 for(int kj=0;kj<3;kj++){
//                     somme+=kernel[ki][kj]*m[i+ki-1][j+kj-1];
//                 }
//             }
//             output[i][j]=(int)round(somme);
//         }
//     }

//     // remplire les cases vides
//     for (int i=0;i<r*h;i++){
//        output[i][0]=0; // premiere colone
//        output[i][r*l-1]=0; // derniere colonne
//     }
//     for (int j=0;j<r*l;j++){
//         output[0][j]=0; // premiere ligne
//         output[r*h-1][j]=0; // derniere ligne
//     }

//    return output;

// }

// Filtre de detection par countours
double **guasse_ker(float ecart, int *size_ker)
{
    int size = 2 * (int)ceil(3 * ecart) + 1;
    *size_ker = size;
    double **ker = creationMatricedouble(1, size, size);
    int offset = size / 2;
    int x, y;
    double somme = 0.0;
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

image convolution(image img, double **k, int size_ker)
{
    image result;
    result.h = img.h;
    result.l = img.l;
    result.r = img.r;
    for (int i = 0; i < 3; i++)
    {
        result.img[i] = creationMatrice(result.r, result.h, result.l);
        // Initialiser à zéro
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
    // Gestion des bords
    for (int c = 0; c < 3; c++)
    {
        // Bords horizontaux
        for (int i = 0; i < size_ker / 2; i++)
        {
            for (int j = 0; j < l; j++)
            {
                result.img[c][i][j] = img.img[c][i][j]; // Copier l'original au lieu de mettre à 0
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

int **detection_de_contours(int **m, int r, int h, int l)
{
    int somme_x = 0, somme_y = 0;
    // Initialisation des kernels de Sobel
    float sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    float sobel_y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    // Nouvelle matrice de sortie
    int **output = creationMatrice(r, h, l);

    for (int i = 1; i < r * h - 1; i++)
    { // Parcours des pixels (sans les bords)
        for (int j = 1; j < l * r - 1; j++)
        {

            // Calcul des gradients pour Sobel X et Sobel Y
            somme_x = 0;
            somme_y = 0;

            for (int ki = 0; ki < 3; ki++)
            {
                for (int kj = 0; kj < 3; kj++)
                {
                    somme_x += sobel_x[ki][kj] * m[i + ki - 1][j + kj - 1];
                    somme_y += sobel_y[ki][kj] * m[i + ki - 1][j + kj - 1];
                }
            }

            // Calcul de la magnitude du gradient
            int magnitude = (int)round(sqrt(somme_x * somme_x + somme_y * somme_y));

            // Stocker la magnitude dans la matrice de sortie
            output[i][j] = magnitude;
        }
    }

    // Remplir les bords de la matrice de sortie avec 0
    for (int i = 0; i < r * h; i++)
    {
        output[i][0] = 0;     // Première colonne
        output[i][l - 1] = 0; // Dernière colonne
    }

    for (int j = 0; j < l * r; j++)
    {
        output[0][j] = 0;     // Première ligne
        output[r - 1][j] = 0; // Dernière ligne
    }

    return output;
}

// Creation de l image floue
// image floue_image(image m){
//     image nv;
//     nv.h=m.h;
//     nv.l=m.l;
//     nv.r=m.r;
//     for(int i=0;i<3;i++){
//         nv.img[i]=flou(m.img[i],m.r,m.h,m.l);
//     }
//     return nv;
// };
// Creation de l image edge detect

grey_image detect_image(image m, int c)
{
    grey_image nv;
    nv.h = m.h;
    nv.l = m.l;
    nv.r = m.r;

    nv.img = detection_de_contours(m.img[c], m.r, m.h, m.l);

    return nv;
};

// Creation de l image
void saveImageAsPNG(image img, const char *filename)
{
    int width = img.l * img.r;
    int height = img.h * img.r;
    unsigned char *data = malloc(3 * width * height); // RGB data

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = (i * width + j) * 3;
            data[index] = img.img[0][i][j];     // Red channel
            data[index + 1] = img.img[1][i][j]; // Green channel
            data[index + 2] = img.img[2][i][j]; // Blue channel
        }
    }

    stbi_write_png(filename, width, height, 3, data, width * 3);
    free(data);
}
//                    for troubleshooting
// int main()
// {
//     // char filename[] = "matrice.txt";
//     // image m2 = initmatricefile(filename);
//     // int sizeker = 0;
//     // float ecart;
//     // image resultat_flou;
//     // // printf("Enter l ecrat type :");
//     // // scanf("%f",&ecart);
//     // double **ker = guasse_ker(2, &sizeker);
//     // printf("%d\n", sizeker);
//     // afficher2d(ker, sizeker, sizeker);

//     // printf("\n************** Image Loaded from File ****************\n");
//     // //afficherimg(m2, m2.r, m2.h, m2.l);
//     // grey_image grey = extraire(m2, 1);
//     // grey_image edge = detect_image(m2,0);  // 0 pour rouge

//     // saveGreyImageAsPNG(grey, "grey_component.png");
//     // saveGreyImageAsPNG(edge, "edge_detect.png");
//     // // Save the loaded image as a PNG
//     // saveImageAsPNG(m2, "output.png");
//     // printf("\nImage saved as output.png\n");

//     // // Apply noise reduction
//     // printf("\n************** Applying Noise Reduction ****************\n");
//     // resultat_flou = convolution(m2, ker, sizeker);
//     // //afficherimg(resultat_flou, resultat_flou.r, resultat_flou.h, resultat_flou.l);

//     // // Save the noise-reduced image as a PNG
//     // printf("Dimensions : resolution=%d, longueur=%d, hauteur=%d", resultat_flou.r, resultat_flou.l, resultat_flou.h);

//     // saveImageAsPNG(resultat_flou, "output_floue_new.png");
//     // printf("\nImage saved as output_floue_new.png\n");

// }
int main() {
    int choix, sous_choix;
    char nom_fichier[256] = "matrice.txt";
    
    // Variables pour l'image
    image img_originale;
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
        printf("5. Quitter\n");
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

            case 5: {
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