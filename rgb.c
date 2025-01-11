#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"




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
    int r;  // resolution
    int l;  // longueure
    int h;  // hauteure
    int **img[3]; // matrice r= 0 g = 1 b = 2
} image;

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
float **creationMatricefloat(int r, int h, int l)
{
    float **k = (float **)malloc(sizeof(float *) * r * h);
    for (int i = 0; i < r * h; i++)
    {
        k[i] = (float *)malloc(sizeof(float) * r * l);
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

image initmatricefile(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    image g;

    // Lire les dimensions r, l, h
    if (fscanf(f, "%d,%d,%d ", &g.r, &g.l, &g.h) != 3) {   // fscanf retoune le nombre des elements scanne 
        fprintf(stderr, "Erreur : format incorrect .\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    printf("Dimensions : resolution=%d, longueur=%d, hauteur=%d\n", g.r, g.l, g.h);

    
    for (int i = 0; i < 3; i++) {
        g.img[i] = creationMatrice(g.r,g.h,g.l); 
        if (g.img[i] == NULL) {
            fprintf(stderr, "Erreur : echec de l'allocation mémoire pour img[%d].\n", i);
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

   
    for (int i = 0; i < g.h * g.r; i++) {
        for (int j = 0; j < g.l * g.r; j++) {
            if (fscanf(f, "(%d.%d.%d) ", &g.img[0][i][j], &g.img[1][i][j], &g.img[2][i][j]) != 3) {
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
void afficher2d(int **m, int r, int h, int l)
{
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

// Extraire une des composantes (rouge / vertes / blue ) de l image 
int **extraire(image m, int r, int h, int l, int c)
{
    int **a = creationMatrice(r, h, l);
    for (int i = 0; i < r * h; i++)
    {
        for (int j = 0; j < r * l; j++)
        {
            a[i][j] = m.img[c][i][j];
        }
    }
    return a;
}

// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************
// ***********************************      Q   U    4          *************************************
// **************************************************************************************************
// **************************************************************************************************
// **************************************************************************************************



// Filtre de flous 


int ** flou(int **m,int r, int h, int l){
    float somme=0.0;
    // intialisation de la matrice
   
    float kernel[3][3]={
        {1.0/16.0,1.0/16.0,1.0/16.0},
        {1.0/16.0,1.0/16.0,1.0/16.0},
        {1.0/16.0,1.0/16.0,1.0/16.0}
    };
   
    // nouvel matrice 
    int ** output=creationMatrice(r,h,l);
    for (int i=1;i<r*h-1;i++){
        for (int j=1;j<r*l-1;j++){
            somme=0;
            for (int ki=0;ki<3;ki++){
                for(int kj=0;kj<3;kj++){
                    somme+=kernel[ki][kj]*m[i+ki-1][j+kj-1];
                }
            }
            output[i][j]=(int)round(somme);
        }
    }

    // remplire les cases vides
    for (int i=0;i<r*h;i++){
       output[i][0]=0; // premiere colone 
       output[i][r*l-1]=0; // derniere colonne 
    }
    for (int j=0;j<r*l;j++){
        output[0][j]=0; // premiere ligne 
        output[r*h-1][j]=0; // derniere ligne 
    }
    
   return output;

}

// Filtre de detection par countours 



int **detection_de_contours(int **m, int r, int h, int l) {
    int somme_x = 0, somme_y = 0;
    // Initialisation des kernels de Sobel
    float sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    float sobel_y[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // Nouvelle matrice de sortie
    int **output = creationMatrice(r, h, l);

    for (int i = 1; i < r*h - 1; i++) {  // Parcours des pixels (sans les bords)
        for (int j = 1; j < l*r - 1; j++) {

            // Calcul des gradients pour Sobel X et Sobel Y
            somme_x = 0;
            somme_y = 0;

            for (int ki = 0; ki < 3; ki++) {
                for (int kj = 0; kj < 3; kj++) {
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
    for (int i = 0; i < r*h; i++) {
        output[i][0] = 0;  // Première colonne
        output[i][l - 1] = 0;  // Dernière colonne
    }

    for (int j = 0; j < l*r; j++) {
        output[0][j] = 0;  // Première ligne
        output[r - 1][j] = 0;  // Dernière ligne
    }

    return output;
}



// Creation de l image floue 
image floue_image(image m){
    image nv;
    nv.h=m.h;
    nv.l=m.l;
    nv.r=m.r;
    for(int i=0;i<3;i++){
        nv.img[i]=flou(m.img[i],m.r,m.h,m.l);
    }
    return nv;
};
// Creation de l image edge detect

image detect_image(image m){
    image nv;
    nv.h=m.h;
    nv.l=m.l;
    nv.r=m.r;
    for(int i=0;i<3;i++){
        nv.img[i]=detection_de_contours(m.img[i],m.r,m.h,m.l);
    }
    return nv;
};


// Creation de l image 
void saveImageAsPNG(image img, const char *filename) {
    int width = img.l * img.r;
    int height = img.h * img.r;
    unsigned char *data = malloc(3 * width * height); // RGB data

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = (i * width + j) * 3;
            data[index] = img.img[0][i][j];    // Red channel
            data[index + 1] = img.img[1][i][j]; // Green channel
            data[index + 2] = img.img[2][i][j]; // Blue channel
        }
    }

    stbi_write_png(filename, width, height, 3, data, width * 3);
    free(data);
}

int main()
{
    char filename[]="matrice.txt";
    image m;
    image m2=initmatricefile(filename);
    image resultat_flou,resultat_edge;
    m.h = 8;
    m.l = 8;
    m.r = 5;
    for (int i = 0; i < 3; i++)
    {
        m.img[i] = creationMatrice(m.r, m.h, m.l);
        initmatricerand(m.img[i], m.r, m.h, m.l);
    }
    printf("\n************** r a n d ****************\n");
    afficherimg(m, m.r, m.h, m.l);
    printf("\n************** f i l e ****************\n");
    afficherimg(m2,2,2,3);
    int **red = extraire(m, m.r, m.h, m.l, 0);
    int **green = extraire(m, m.r, m.h, m.l, 1);
    int **blue = extraire(m, m.r, m.h, m.l, 2);
    printf("\n************** r e d  ****************\n");
   
    afficher2d(red, m.r, m.h, m.l);
 
    printf("\n************** g r e e n  ****************\n");
    afficher2d(green, m.r, m.h, m.l);

     printf("\n************** b l u e   ****************\n");
     afficher2d(blue, m.r, m.h, m.l);
    printf("\n************** f l o u e   ****************\n");
  
    resultat_flou=floue_image(m);
    afficherimg(resultat_flou,resultat_flou.r,resultat_flou.h,resultat_flou.l);

    printf("\n************** Image Loaded from File ****************\n");
    afficherimg(m2, m2.r, m2.h, m2.l);

    // Save the loaded image as a PNG
    saveImageAsPNG(m2, "output.png");
    printf("\nImage saved as output.png\n");

    // Apply noise reduction
    printf("\n************** Applying Noise Reduction ****************\n");
      resultat_edge=detect_image(m2);
    resultat_flou = floue_image(m2);

    
    afficherimg(resultat_flou, resultat_flou.r, resultat_flou.h, resultat_flou.l);
    afficherimg(resultat_edge, resultat_edge.r, resultat_edge.h, resultat_edge.l);


    // Save the noise-reduced image as a PNG
    saveImageAsPNG(resultat_flou, "output_floue.png");
    saveImageAsPNG(resultat_edge, "edge_detecte.png");

    
}