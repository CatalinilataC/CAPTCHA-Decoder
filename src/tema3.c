//CATALIN BAJENARU 311 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"

#define N 111
#define RAZA 7
#pragma pack(1)

struct pix
{
    unsigned char B, G, R;
};


struct cifra
{
    short int x, y;
    unsigned char val;
};

 struct pct
{
    int x, y;
};

struct pixbon
{
    int B, G, R;
    int nr;
};
struct pctbon
{
    struct pct dot;//doar pt ca tema ar fi despre structuri
    unsigned char B, G, R;
};

unsigned char calcif(int v[N + 3][N + 3], int a, int b)
{
    int st = 0,en = 1, x, y;
   struct pct q[N];
    q[0].x = a;
    q[0].y = b;
    v[a][b] = 2;
    while(st != en)
    {
        x = q[st].x;
        y = q[st].y;
        st++;
        if(v[x + 1][y] == 1)
        {
            q[en].x = x + 1;
            q[en].y = y;
            v[x + 1][y] = 2;
            en++;
        }
        if(v[x - 1][y] == 1)
        {
            q[en].x = x - 1;
            q[en].y = y;
            v[x - 1][y] = 2;
            en++;
        }
        if(v[x][y + 1] == 1)
        {
            q[en].x = x;
            q[en].y = y + 1;
            v[x][y + 1] = 2;
            en++;
        }
        if(v[x][y - 1] == 1)
        {
            q[en].x = x;
            q[en].y = y - 1;
            v[x][y - 1] = 2;
            en++;
        }

    }
    if(en == 5)return 1;
    if(en == 9)return 7;
    if(en == 11)return 4;
    if(en == 16)return 0;
    if(en == 17)
    {
        if(v[a - 1][b] == 2)
        {
            if(v[a - 3][b] == 2)return 3;
            else return 5;
        }
        else return 2;
    }
    if(en == 18)
    {
        if(v[a - 3][b] == 2)return 9;
        return 6;
    }
    if(en == 19)return 8;
    return -1;
}

void ripleis(struct pix mp[N + 3][N + 3], int x,  int y, int a, int b)
{
    int i, j;
    for(i = 0; i < 5; i++)
        for(j = 0; j < 5; j++)
        {
            mp[a + i][b - j] = mp[x + i][y - j];
        }
}

void ripleis0(struct pix mp[N + 3][N + 3], int x, int y)
{
    int i, j;
    for(i = 0; i < 5; i++)
        for(j = 0; j < 5; j++)
        {
            mp[x + i][y - j] = (struct pix){255, 255, 255};
        }
}

void task1(FILE* im1, struct pix mp[N + 3][N + 3], struct bmp_fileheader bf, struct bmp_infoheader bi, struct pix cul)
{
    int i, j,
        padding = bi.width % 4;
    int zero[] = {0, 0, 0};
    fwrite(&bf, sizeof(struct bmp_fileheader), 1, im1);
    fwrite(&bi, sizeof(struct bmp_infoheader), 1, im1);
    fseek(im1, bf.imageDataOffset, 0);
    for(i = 0; i < bi.height; i++)
    {
        for(j = 0; j < bi.width; j++)
        {
            if(mp[i][j].B == 255 && mp[i][j].G == 255 && mp[i][j].R == 255)
                fwrite(&mp[i][j], 3, 1, im1);
            else fwrite(&cul, 3, 1, im1);

        }
        fwrite(zero, padding, 1, im1);
    }

}

void lire(FILE* z, struct pix mp[N + 3][N + 3],  struct bmp_fileheader *bf, struct bmp_infoheader *bi)
{//functia asta citeste datele continute intr o imagine de tip bmp
    int i, j;
    fread(&(*bf), sizeof(struct bmp_fileheader), 1, z);
    fread(&(*bi), sizeof(struct bmp_infoheader), 1, z);
    int padding = (*bi).width % 4;
 //   fseek(z, (*bf).imageDataOffset, 0);
    for(i=0; i< (*bi).height; i++)
    {
        for(j=0; j < (*bi).width; j++)
        {
            fread(&mp[i][j], 3, 1, z);
        }

        fseek(z, padding, 1);
    }
}

int task2(struct pix mp[N + 3][N + 3], struct cifra vecif[N + 3], struct bmp_infoheader bi)
{
    int co = 0, i, j;
    int v[N + 3][N + 3];
    for(i = bi.height - 1; i >= 0; i--)
    {
        for(j = 0; j < bi.width; j++)
        {
            if(mp[i][j].B == 255 && mp[i][j].G == 255 && mp[i][j].R == 255)
            {
                v[bi.height - i][j + 1] = 0;
            }
            else
            {
                v[bi.height - i][j + 1] = 1;
            }
        }
    }
    for(j = bi.width; j; j--)
    {
        for(i = bi.height; i; i--)
        {
            if(v[i][j] == 1)
            {
                vecif[co].x = i;
                vecif[co].y = j;
                vecif[co].val = calcif(v, i, j);
                co++;
            }
        }
    }

    return co;
}

void task3(FILE* t3, struct pix mp[N + 3][N + 3], struct cifra vecif[N + 3], struct cifra cival[N], struct bmp_fileheader bf, struct bmp_infoheader bi, int co, int t)
{
    int i, j;

    for(i = 0; i < t; i++)
    {
        ripleis(mp, bi.height - cival[i].x, cival[i].y - 1,bi.height - vecif[co - 1 - i].x, vecif[co - 1 - i].y - 1);
    }
    for(i = t; i < co; i++)
    {
        ripleis0(mp,bi.height - vecif[co - 1 - i].x, vecif[co - 1 - i].y - 1);
    }
    fwrite(&bf, sizeof(struct bmp_fileheader), 1, t3);
    fwrite(&bi, sizeof(struct bmp_infoheader), 1, t3);
    int zero[]= {0, 0, 0};
    int padding = bi.width % 4;
    for(i = 0; i < bi.height; i++)
    {
        for(j=0; j<bi.width; j++)
        {
            fwrite(&mp[i][j],3,1,t3);
        }
        fwrite(zero, padding, 1, t3);
    }


}

void bonus(FILE* inpt, struct pix mp[N + 3][N + 3])
{
    struct bmp_fileheader bf;
    struct bmp_infoheader bi;
    char sir4[50], capt[25];
    int i, j, k, ok;
     fgets(capt, 20, inpt);
      capt[strlen(capt) - 1] = 0;
     FILE* im2 = fopen(capt, "rb");
      capt[strlen(capt) - 4] = 0;
      strcpy(sir4, capt);
      strcat(sir4, "_bonus.bmp");
      FILE* bns = fopen(sir4, "wb");
      lire(im2, mp, &bf, &bi);
      struct pixbon harta[N + 3][N + 3];
      struct pctbon vecpct[N];
      int nrpct = 0;
      for(i = 0; i < bi.height; i++)
      {
          for(j = 0; j < bi.width; j++)
          {ok = 1;
              if(mp[i][j].B != 255 || mp[i][j].G != 255 || mp[i][j].R != 255)
              {//daca pixelul e diferit de alb
                    if((i && (mp[i - 1][j].B != 255 || mp[i - 1][j].G != 255 || mp[i - 1][j].R != 255)) ||
                       (j && (mp[i][j - 1].B != 255 || mp[i][j - 1].G != 255 || mp[i][j - 1].R != 255)) ||
                       ((i != bi.height - 1) && (mp[i + 1][j]. B != 255 || mp[i + 1][j].G != 255 || mp[i + 1][j].R != 255)) ||
                       ((j != bi.width - 1) && (mp[i][j + 1]. B != 255 || mp[i][j + 1].G != 255 || mp[i][j + 1].R != 255)))
                       ok = 0;
                    if(ok)
                 {//daca pixelul este un punct ce nu face parte dintr o cifra
                    vecpct[nrpct].dot.x = i;
                    vecpct[nrpct].dot.y = j;
                    vecpct[nrpct].B = mp[i][j].B;
                    vecpct[nrpct].G = mp[i][j].G;
                    vecpct[nrpct].R = mp[i][j].R;
                    nrpct++;
                    harta[i][j].nr = 0;
                 }
                 else
                 {//daca pixelul apartine unei cifre
                     harta[i][j].B = mp[i][j].B;
                     harta[i][j].G = mp[i][j].G;
                     harta[i][j].R = mp[i][j].R;
                     harta[i][j].nr = 1;
                 }
                }
                else harta[i][j].nr = 0;
          }
      }
      for(i = 0; i < bi.height; i++)
      {
          for(j = 0; j < bi.width; j++)
          {
              if(harta[i][j].nr)
              {
                  for(k = 0; k < nrpct; k++)
                  {
                      if((i - vecpct[k].dot.x) * (i - vecpct[k].dot.x) + (j - vecpct[k].dot.y) * (j - vecpct[k].dot.y) <= RAZA * RAZA)
                      {
                          harta[i][j].B += vecpct[k].B;
                          harta[i][j].G += vecpct[k].G;
                          harta[i][j].R += vecpct[k].R;
                          harta[i][j].nr++;

                      }
                  }
              }
          }
      }
    fwrite(&bf, sizeof(struct bmp_fileheader), 1, bns);
    fwrite(&bi, sizeof(struct bmp_infoheader), 1, bns);
    int zero[]= {0, 0, 0};
    int padding = bi.width % 4;
    for(i = 0; i < bi.height; i++)
    {
        for(j = 0; j < bi.width; j++)
        {
            if(harta[i][j].nr)
            {
                mp[i][j].B = harta[i][j].B / harta[i][j].nr;
                mp[i][j].G = harta[i][j].G / harta[i][j].nr;
                mp[i][j].R = harta[i][j].R / harta[i][j].nr;
            }
            fwrite(&mp[i][j], 3, 1, bns);
        }
        fwrite(zero, padding, 1, bns);
    }
}



int main()
{
    int i, j = 0, ok;
    struct bmp_fileheader bf;
    struct bmp_infoheader bi;
    char capt[25], sterge[25];
    char sirin[] = "input.txt";
    struct pix mp[N + 3][N + 3], cul;
    FILE* inpt = fopen(sirin, "rt");
    if(inpt == NULL)
	return 0;//ar trebui verificati toti pointerii de fisiere, dar problema nu mentioneaza ca s-ar putea intampla sa nu functioneze
    fgets(capt, 20, inpt);
    capt[strlen(capt) - 1] = 0;
    FILE* im = fopen(capt, "rb");
    capt[strlen(capt) - 4] = 0;
    fscanf(inpt,"%d%d%d", &i, &j, &ok);
    cul.B = i;
    cul.G = j;
    cul. R = ok;
    lire(im, mp, &bf, &bi);
    char sir1[N], sir2[N], sir3[N];
    strcpy(sir1, capt);
    strcpy(sir2, capt);
    strcpy(sir3, capt);
    strcat(sir1, "_task1.bmp");
    strcat(sir2, "_task2.txt");
    strcat(sir3, "_task3.bmp");
    FILE* im1 = fopen(sir1,"wb");
    FILE* t2 = fopen(sir2, "wt");
    FILE* t3 = fopen(sir3, "wb");
    task1(im1, mp, bf, bi, cul);
    struct cifra vecif[N + 3];
    int co = task2(mp, vecif, bi);
    int sht[10], p = 0, t = 0;
    fgets(sterge, 25, inpt);
    fgets(sterge, 25, inpt);
    for(i = 0; sterge[i + 1] != 0; i++)
    {
        if(sterge[i] != ' ')
        {
            sht[p++] = sterge[i] - '0';
        }
    }

    struct cifra cival[N];
    for(i = co - 1; i >= 0 ; i--)
    {
        ok = 0;
        fprintf(t2, "%d", vecif[i].val);//afiseaza cifrele pt tasku' 2

        for(j = 0; j < p; j++)
        {
            if(sht[j] == vecif[i].val)
            {
                ok = 1;
                break;
            }
        }
        if(!ok)
	   cival[t++] = vecif[i];
    }
    task3(t3, mp, vecif, cival, bf, bi, co, t); //rezolva tasku' 3
    bonus(inpt, mp); //rezolva bonusul
    return 0;
}

