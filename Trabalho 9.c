#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

int intensidade(unsigned int r, unsigned int g, unsigned int b);

int media(unsigned int r, unsigned int g, unsigned int b);

int olho(unsigned int r, unsigned int g, unsigned int b);

typedef struct _RGB{
    unsigned int r, g, b;
}RGB;

typedef struct _PPM{
    char magic_number[2];
    int w, h;
    int max_level;
    RGB *pixels;
} PPM;


PPM * load_PPM(FILE *in);

int main(){

    int max_forca = 255;
    int max_media = 255;
    int max_olho = 255;
    int num;
    char filename[4096];
    char namefile[4096];

    printf("---------------------------------------------\n");
    printf("\n");
    printf("1 - Conversao baseada em liminar de luminosidade\n");
    printf("2 - Conversao baseada na media\n");
    printf("3 - Conversao baseada nos comprimentos de onda\n");
    printf("\n");
    printf("---------------------------------------------\n");
    printf("\n");
    printf("Digite o numero de uma opcao para a conversao da imagem: ");
    scanf("%d", &num);
    printf("\n");
    printf("---------------------------------------------\n");
    printf("\n");
    printf("Voce escolheu %d\n", num);
    printf("\n");
    printf("---------------------------------------------\n");
    printf("\n");

    while(num != 1 && num != 2 && num != 3){
        printf("%d nao e uma escolha valida, escolha entre 1, 2 ou 3\n");
        printf("\n");
        printf("---------------------------------------------\n");
        printf("\n");
        printf("1 - Conversao baseada em liminar de luminosidade\n");
        printf("2 - Conversao baseada na media\n");
        printf("3 - Conversao baseada nos comprimentos de onda\n");
        printf("\n");
        printf("---------------------------------------------\n");
        printf("\n");
        printf("Digite o numero de uma opcao para a conversao da imagem: ");
        scanf("%d", &num);
        printf("\n");
        printf("---------------------------------------------\n");
        printf("\n");
        printf("Voce escolheu %d\n", num);
        printf("\n");
        printf("---------------------------------------------\n");
        printf("\n");
    }




    printf("Insira o nome do arquivo a ser convertido: ");
    scanf("%s", filename);
    printf("\n");

    FILE *in = fopen(filename, "r");

    if(in == NULL){
        printf("Nao foi possivel abrir o arquivo %s\n", filename);
        return -1;
    }else{
        printf("%s aberto\n\n", filename);
    }

    PPM *ppm = load_PPM(in);

    if (ppm == NULL)
    {
        printf("Nao foi possível carregar a imagem %s\n", filename);
        fclose(in);
        return -2;
    }

    PPM *termos = ppm;
    int mult = termos->w * termos->h;


    printf("Insira o nome do arquivo de saida com \".pgm\" no final: ");
    scanf("%s", namefile);

    FILE *fp = fopen(namefile , "w");
    fprintf(fp, "P2\n");
    fprintf(fp, "%d %d\n", ppm->w, ppm->h);


    if(num == 1){
        fprintf(fp, "%d\n", max_forca);
        for(int i = 0; i < mult; ++i){
            fprintf(fp, "%d\n", intensidade(termos->pixels[i].r, termos->pixels[i].g, termos->pixels[i].b));
        }
        printf("\nArquivo em escala cinza criado\n");
    } else {

        if(num == 2){
            fprintf(fp, "%d\n", max_media);
            for(int i = 0; i < mult; ++i){
                fprintf(fp, "%d\n", media(termos->pixels[i].r, termos->pixels[i].g, termos->pixels[i].b));
            }
        printf("\nArquivo em escala cinza criado\n");

    }else
            if(num == 3){
            fprintf(fp, "%d\n", max_olho);
            for(int i = 0; i < mult; ++i){
                fprintf(fp, "%d\n", olho(termos->pixels[i].r, termos->pixels[i].g, termos->pixels[i].b));

            }
            printf("\nArquivo em escala cinza criado\n");
        }
    }

    fclose(in);
    fclose(fp);

    return 0;
}

int olho(unsigned int r, unsigned int g, unsigned int b){
    int Y;
    Y = 0.2126*r + 0.7152*g + 0.0722*b;
    return r;
}

int media(unsigned int r, unsigned int g, unsigned int b){
    int Y;
    Y = (r+g+b)/3;
    return g;
}

int intensidade(unsigned int r, unsigned int g, unsigned int b){
    int Y;
    Y = 0.299*r + 0.587*g + 0.114*b;
    return b;
}

PPM * load_PPM(FILE *in)
{
    PPM *ppm = (PPM *) malloc(sizeof(PPM));

    if (ppm == NULL){
        printf("Sem memoria suficiente\n");
        return NULL;
    }

    // leitura do numero magico
    char line[4096];
    fgets(line, 4096, in);
    strncpy(ppm->magic_number, line, 2);



    // Lê a próxima linha que não é comentário
    fgets(line, 4096, in);

    // Ignorando comentários de linha
    while (strlen(line) > 0 && line[0] == '#')
        fgets(line, 4096, in);


    sscanf(line, "%d %d", &ppm->w, &ppm->h); //dentro da linha lida vai extrair o *w* e o *h*

    //ler o nível máximo do RGB, ignorando linhas de comentários
    fgets(line, 4096, in);
    sscanf(line, "%d", &ppm->max_level);

    ppm->pixels = (RGB *) calloc(ppm->w*ppm->h, sizeof(RGB));


    unsigned long long int mult = 1;
    mult = ppm->h * ppm->w;

    for(long long i = 0; i < mult; ++i){ //ler todos os rgb
        fgets(line, 4096, in);
        sscanf(line, "%d", &ppm->pixels[i].r);

        fgets(line, 4096, in);
        sscanf(line, "%d", &ppm->pixels[i].g);

        fgets(line, 4096, in);
        sscanf(line, "%d", &ppm->pixels[i].b);
    }

    return ppm;
}
