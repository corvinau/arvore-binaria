#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define NUM 30

FILE *arq;
int vet[NUM];

typedef struct NodoArvore
{
    int info;
    struct NodoArvore *dir, *esq;
}ArvBin;

ArvBin *cria_nodo()
{
    ArvBin *p;
    p = (ArvBin *) malloc(sizeof(ArvBin));
    if(!p)
    {
        printf("\nProblema de alocacao\n\n");
        exit(0);
    }
    return p;
}

void libera(ArvBin *arvore)
{
    if(arvore != NULL)
    {
        libera(arvore->esq);
        libera(arvore->dir);
        free(arvore);
        arvore = NULL;
    }
}

void gera_numeros()
{
    int i,j,r=0;

    srand((unsigned)time(NULL));

    if((arq = fopen("numeros.txt", "w+"))== NULL)
    {
        printf("O arquivo nao pode ser aberto!");
        return;
    }

    for(i=0; i<=NUM-1; i++)
    {
        r = 100+rand()%(1000-100);
        vet[i] = r;

        for(j=0; j<i; j++)
        {
            if(vet[j] == vet[i])
            {
                vet[i] = r++;
            }
        }
        fprintf(arq, "%d; ", r);
    }

    fclose(arq);
}

int define_raiz()
{
    int x, i, j, n=0, media=0, soma=0, raizarv, menor=999;
    int vetor[NUM], difmed[NUM], aux[NUM];

    if((arq = fopen("numeros.txt", "r"))== NULL)
    {
        printf("\nO arquivo nao pode ser aberto\n\n");
        return;
    }

    while(!feof(arq))
    {
        fscanf(arq, "%d; ", &x);
        for(i=0; i<=NUM-1; i++)
        {
            vetor[i] = x;
        }
        soma += x;
    }

    media = soma/NUM;
    printf("Media: %d\n\n", media);

    fclose(arq);

    if((arq = fopen("numeros.txt", "r"))== NULL)
    {
        printf("\nO arquivo nao pode ser aberto\n\n");
        return;
    }

    while(!feof(arq))
    {
        fscanf(arq, "%d; ", &j);
        for(i=0; i<=NUM-1; i++)
        {
            aux[i] = j;
            difmed[i] = aux[i]-media;

            if(difmed[i]<0)
            {
                difmed[i] = difmed[i]*-1;
            }

            if (difmed[i]<menor)
            {
                menor=difmed[i];
                raizarv=aux[i];
            }
        }
    }

    fclose(arq);

    return raizarv;
}

ArvBin *insere_no_arvore(ArvBin *pai, ArvBin *filho, int dado)
{
    if(filho == NULL)
    {
        filho = cria_nodo();
        filho->dir = NULL;
        filho->esq = NULL;
        filho->info = dado;
        if(pai == NULL) // insercao do primeiro nodo
            return filho;
        if(dado < pai->info) //verifica se insere no direita ou esquerda da sub-arvore
            pai->esq = filho;
        else
            pai->dir = filho;

        return filho;
    }

    if(dado < filho->info)
        insere_no_arvore(filho, filho->esq, dado);
    else
        insere_no_arvore(filho, filho->dir, dado);

    return filho;
}

int altura(ArvBin *r)
{
    int he, hd;
    if (r == NULL)
        return -1; //altura da arvore vazia
    else
    {
        he = altura (r->esq);
        hd = altura (r->dir);
        if (he < hd)
            return hd + 1;
        else
            return he + 1;
    }
}

void MostraFolhas(ArvBin *a)
{
    if(a != NULL)
    {
        if(a->esq == NULL &&  a->dir == NULL)
        {
            printf("%d\n", a->info);
            return;
        }
        MostraFolhas(a->esq);
        MostraFolhas(a->dir);
    }
}

ArvBin *busca(ArvBin *r, int valor)
{
    ArvBin *aux;
    if (r == NULL)
        return NULL;
    else if (valor == r->info)
        return r;
    else
    {
        aux = busca(r->esq, valor);
        if (aux == NULL)
            aux = busca(r->dir, valor);

        return aux;
    }
}

int nivel(ArvBin *pos, int x, int ni)
{
    if (pos ==NULL)
        return (-1);

    if (x== pos->info)
        return(ni);

    if (x < pos->info)
        return(nivel(pos->esq, x, ni+1));
    else
    {
        if(x> pos->info)
            return(nivel(pos->dir, x, ni+1));
    }
}

void imprime_preordem (ArvBin *arv)
{
    if(!arv)
        return;
    printf("%d\n", arv->info);
    imprime_preordem(arv->esq);
    imprime_preordem(arv->dir);
}

void imprime_posordem(ArvBin *arv)
{
    if(!arv)
        return;
    imprime_posordem(arv->esq);
    imprime_posordem(arv->dir);
    printf("%d\n", arv->info);
}

void imprime_inordem(ArvBin *arv)
{
    if(!arv)
        return;
    imprime_inordem(arv->esq);
    printf("%d\n", arv->info);
    imprime_inordem(arv->dir);
}

void show(ArvBin *x, int b)
{
    int i;

    for (i = 0; i < b; i++)
        printf("---");

    if (x == NULL)
    {
        printf("*\n");
        return;
    }

    printf("%d\n", x->info);

    show(x->dir, b+1);
    show(x->esq, b+1);
}

ArvBin *retira(ArvBin *r, int v)
{
    ArvBin *t, *f;

    if (r == NULL)
        return NULL;
    else if (r->info > v)
        r->esq = retira(r->esq, v);
    else if (r->info < v)
        r->dir = retira(r->dir, v);
    else /* achou o no a remover */
    {
        /* no sem filhos */
        if (r->esq == NULL && r->dir == NULL)
        {
            free (r);
            r = NULL;
        }
        /* no so tem filho a direita */
        else if (r->esq == NULL)
        {
            t = r;
            r = r->dir;
            free (t);
        }
        /* so tem filho a esquerda */
        else if (r->dir == NULL)
        {
            t = r;
            r = r->esq;
            free (t);
        }
        /* no tem os dois filhos */
        else
        {
            f = r->esq;
            while (f->dir != NULL)
            {
                f = f->dir;
            }
            r->info = f->info;/* troca as informacoes */
            f->info = v;
            r->esq = retira(r->esq, v);
        }
    }
    return r;
}

void main()
{
    int op, raiz, i, alt, buscar, niv = 0, resp, exc;
    ArvBin *b, *e, *p = NULL;

    do
    {
        printf("\n1. Gerar arquivo texto com 30 valores numericos OK");
        printf("\n2. Montar uma arvore binaria de busca em memoria OK");
        printf("\n3. Apresentar na tela a arvore binaria OK");
        printf("\n4. Excluir um elemento da arvore binaria");
        printf("\n5. Buscar um elemento da arvore binaria OK");
        printf("\n6. Imprimir os elementos da arvore no percurso In-Ordem OK");
        printf("\n7. Imprimir os elementos da arvore no percurso Pre-Ordem OK");
        printf("\n8. Imprimir os elementos da arvore no percurso Pos-Ordem OK");
        printf("\n9. Sair OK\n\n");

        scanf("%d", &op);
        switch(op)
        {
        case 1:
            system("cls");
            gera_numeros();
            for(i=0; i<=NUM-1; i++)
            {
                printf("%d\n", vet[i]);
            }
            break;
        case 2:
            if(vet[0] == 0)
            {
                system("cls");
                printf("\nGere o arquivo antes\n\n");
                break;
            }
            else
            {
                system("cls");
                printf("\nGerando arvore binaria\n\n");
                raiz = define_raiz();
                p = insere_no_arvore(p,p,raiz);

                for(i=0; i<=NUM-1; i++)
                {
                    if(vet[i] != raiz)
                    {
                        p = insere_no_arvore(p,p,vet[i]);
                    }
                }

                alt = altura(p);
                printf("Altura da arvore: %d\n", alt);
                printf("Raiz: %d\n", raiz);
                printf("Folhas:\n");
                MostraFolhas(p);
                printf("\n");
            }
            break;
        case 3:
            if(p == NULL)
            {
                system("cls");
                printf("\nMonte a arvore antes\n\n");
            }
            else
            {
                system("cls");
                show(p, 0);
                printf("\n");
            }
            break;
        case 4:
            if(p == NULL)
            {
                system("cls");
                printf("\nMonte a arvore antes\n\n");
            }
            else
            {
                system("cls");
                printf("\nDigite o numero a ser excluido da arvore: ");
                scanf("%d", &exc);
                e = retira(p, exc);
                if(e == NULL)
                {
                    printf("O numero nao existe na arvore");
                }
                else
                {
                    printf("Numero excluido");
                }
            }
            break;
        case 5:
            if(p == NULL)
            {
                system("cls");
                printf("\nMonte a arvore antes\n\n");
            }
            else
            {
                system("cls");
                printf("\nDigite o numero a ser pesquisado na arvore: ");
                scanf("%d", &buscar);

                b = busca(p, buscar);
                if(b == NULL)
                {
                    printf("\nO numero nao existe na arvore\n\n");
                }
                else
                {
                    resp = nivel(p, buscar, niv);
                    printf("\nNivel %d\n", resp);
                    if(resp == 0 && p->esq != NULL && p->dir != NULL)
                    {
                        printf("No raiz\n\n");
                    }
                    else if(resp != 0 && p->esq != NULL || p->dir != NULL)
                    {
                        printf("No pai\n\n");
                    }
                    else if(resp != 0 && p->esq == NULL && p->dir == NULL)
                    {
                        printf("No folha\n\n"); //BUG: Nao reconhece como sendo um na folha, mostra como se fosse um no pai
                    }
                }
            }
            break;
        case 6:
            if(p == NULL)
            {
                system("cls");
                printf("\nMonte a arvore antes\n\n");
            }
            else
            {
                system("cls");
                printf("\nIn-ordem\n\n");
                imprime_inordem(p);
            }
            break;
        case 7:
            if(p == NULL)
            {
                system("cls");
                printf("\nMonte a arvore antes\n\n");
            }
            else
            {
                system("cls");
                printf("\nPre-ordem\n\n");
                imprime_preordem(p);
            }
            break;
        case 8:
            if(p == NULL)
            {
                system("cls");
                printf("\nMonte a arvore antes\n\n");
            }
            else
            {
                system("cls");
                printf("\nPos-ordem\n\n");
                imprime_posordem(p);
            }
            break;
        case 9:
            system("cls");
            printf("\nSaindo do programa!\n\n");
            libera(p);
            remove("numeros.txt");
            break;
        default:
            printf("\nOpcao Invalida!\n\n");
        }
    }
    while(op != 9);
}
