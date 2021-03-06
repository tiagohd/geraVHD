#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 54000

typedef struct no {
    char* nome;
    int cor;
    int tam;
    struct no *prox;
    //struct no *list;
    struct no *prod;
} No;

typedef enum { false, true } bool;

void insNo (No* l, char *i);
No* buscaNo (No* l,char v[]);
No* iniNo (void);
void exibeNo (No* l);
void removeNo (No* l, char *i);
void liberaNo (No* l);
void lerEntrada (No* Dec, char *nome);	// para padr�o .eqn normal
void blerEntrada (No* Dec, char *nome);  // para padr�o .eqn de berkley
void alerEntrada (No* Dec, char *nome);  // para padr�o .prs do atacs
void toEqn(No* Dec, char *nome);  // para padr�o .prs do atacs
char* copia (char *aux);
void stripFext (char *str);
void materializa (No *Dec, No *In, No *Out, No *Buffer, char *nome, int ini);
void cl_materializa (No *Dec, No *In, No *Out, No *Buffer, char *nome, int ini);
void checkFile (FILE *file, char *name);
bool emptyChar (char c);
int not (int x);
bool fora (char *nome);
bool valida_nome (char *nome);
void addItem (No *list, FILE *arq);
void addStO (No *list, FILE *arq);
void insInOutBuffer(No *Dec, No *In, No *Out, No *Buffer, char *nome);
void addPortMap (No *list, FILE *arq);

char *vet_ini;

int main (int argc, char *argv[])
{

    No *Dec=(No*) malloc(sizeof(No));
    No *In=(No*) malloc(sizeof(No));
    No *Out=(No*) malloc(sizeof(No));
    No *Buffer=(No*) malloc(sizeof(No));

    char *aux;
    Dec->prox=iniNo();
    In->prox=iniNo();
    Out->prox=iniNo();
    Buffer->prox=iniNo();
    int flag=0,x,ini=0;

    if(argc!=3&&argc!=5)
    {
        printf(" Parametros incorreto!!! \n digite o nome do programa e o nome do arquivo de entrada. \n\n Ex for hfmin: Name arquivo.txt -h\n\n\n OR \n\n Ex for Berkley: Name arquivo.txt -b\n\n\n");
        return 1;
    }
    if(strcmp("-h",argv[2])==0)
        flag=0;
    else if(strcmp("-b",argv[2])==0)
        flag=1;
    else if(strcmp("-a",argv[2])==0)
        flag=2;
    else
    {
        printf(" Parametros incorreto!!! \n digite o nome do programa e o nome do arquivo de entrada. \n\n Ex for hfmin: Name arquivo.txt -h\n\n\n OR \n\n Ex for Berkley: Name arquivo.txt -b\n\n\n");
        return 1;
    }
    if(argc==5)
    {
        if(strcmp("-i",argv[3])==0)
        {
            for(x=0; argv[4][x]!='\0'; x++)
            {
                if(argv[4][x]!='0'&&argv[4][x]!='1')
                {
                    printf("A inicializa��o s� pode conter valores entre zero e um\n");
                    return 1;
                }
            }
            ini=1;
            vet_ini=(char *) malloc(strlen(argv[4])*sizeof(char));
            vet_ini=copia(argv[4]);
        } else
        {
            printf(" Parametros incorreto!!! \n digite o nome do programa e o nome do arquivo de entrada. \n\n Ex for hfmin: Name arquivo.txt -h\n\n\n OR \n\n Ex for Berkley: Name arquivo.txt -b\n\n\n");
            return 1;
        }
    }

    if(flag==0)
    {
        lerEntrada(Dec, argv[1]);
    } else if(flag==1)
    {
        blerEntrada(Dec, argv[1]);
    } else if(flag==2)
    {
        alerEntrada(Dec, argv[1]);
        toEqn(Dec,argv[1]);
    }

    //if(flag==1)
    insInOutBuffer(Dec,In,Out,Buffer,argv[1]);
    cl_materializa(Dec,In,Out,Buffer,argv[1],ini);
    //else
    //materializa(Dec,In,Out,Buffer,argv[1],ini);
    return 0;
}

/****************************************************************************/
/*    ler uma entrada de dado no formato .eqn                               */
/*     retorna uma lista de tokens                                          */
/*                                                                          */
/****************************************************************************/
void lerEntrada (No* Dec, char *nome)
{
    FILE *pa;
    char linha[MAX],aux[MAX],*point,*vchar;
    No *l,*m,*n,In,Out,Buffer;
    int lin,tam,flag=0,h;
    pa = fopen (nome, "r");
    checkFile(pa, nome);

    while(!feof(pa))
    {
        fgets(linha , MAX , pa );
        lin++;
        tam=strlen(linha);

        if(tam>2 && linha[0]!=';' && linha[0]!='#') // n�o pega linha vazia ou linhas que s�o comentarios
        {
            h=tam-1;;
            while((linha[h]=='\0'||linha[h]==10||linha[h]=='\n')||linha[h]==' '||linha[h]=='	')
            {
                linha[h]='\0';
                h--;
            }

            point=strtok(linha," ");

            if(flag==0)
            {
                vchar=copia(point);
                insNo(Dec,vchar);
                l=buscaNo(Dec,vchar);
                if(l==NULL)
                {
                    printf("erro n�o encontrou %s\n",vchar);
                    exit(1);
                }
                l->prod=(No*) malloc(sizeof(No));
                l->prod->prox=iniNo();
                l->cor=1;
                flag=1;
            } else
            {
                while(point!=NULL)
                {
                    vchar=copia(point);
                    insNo(l->prod,vchar);
                    point=strtok(NULL," ");
                }
                if(vchar[0]!='+'&&point==NULL)
                    flag=0;
            }

        }

    }

    for(l=Dec->prox; l!=NULL; l=l->prox)
    {
        for(n=Dec->prox; n!=NULL; n=n->prox)
        {
            for(m=n->prod->prox; m!=NULL; m=m->prox)
            {
                h=strlen(m->nome)-1;
                while(h>0 && (m->nome[h]=='\0' || m->nome[h]==10 || m->nome[h]=='\n' || m->nome[h]==' ' || m->nome[h]=='	' ))
                {
                    m->nome[h]='\0';
                    h--;
                }
                strcpy(aux,m->nome);
                printf("\naux  %s ",aux);
                if(aux[h]==39)
                    aux[h]='\0';
                if(strcmp(aux,l->nome)==0)
                {
                    l->cor=2;
                    m->cor=2;
                }
            }
        }
    }
    fclose(pa);
}

/****************************************************************************/
/*    erifica se o char lido � nulo                                         */
/*     retorna true caso verdadeiro                                         */
/*                                                                          */
/****************************************************************************/
bool emptyChar (char c)
{
    if ((c=='\0'||c==10||c=='\n')||c==' '||c=='	'||c==']' || c == '	')
        return true;
    return false;
}
// ler a saida minimizada no formato .kiss pla
//# pode ter coment�rios tb....
//.i 7
//.o 1
//.ilb ok rin fain cntgt1 aout frout ss00
//.ob fgr
//.p 5
//-0--10- 1
//--1--1- 1
//0-----0 1
//1----01 1
//-10-0-- 1
//.e
//void readB(No* Dec, char *nome)
//{
//  char *word, *pch, *sh, linha[MAXSTR], var[MAXSTR], s[50],aux[3];
//  int pos=0,tam,n,linha=0,opcao=0;
//  FILE *pa = fopen(nome,"r");
//
//  checkFile(pa, nome);
//
//  while(!feof(pa))
// {
//
//  fgets(linha , MAXSTR , pa );
//  pos++;
//
//  tam = strlen(linha);
//  strcpy(var,linha);
//  pch = var;
//   while (*pch != '\r' && *pch != '\n' && *pch != '\0')
//    pch++;
//   *pch='\0';
//  // printf("linha %2d tam: %d: %s \n",pos,tam,var);
//  // remove os coment�rios
//   pch = strstr (var,"#");
//   if(pch != NULL)
//   {
//	*pch = '\0';
//    tam = strlen(var);
//   }
//
//  if(tam > 1)
//  {
//
//
//   //n=0;
//   pch = var;
//
//   sh = s;
//   pch=getWord2(pch, sh, pos);
//   while (pch != NULL)
//   {
//	//n++;
//	//printf("%2d: %s ",n,s);
//	word=copia(s);
//	if(valida_nome(word))
//	  insNo(ListToken,word);
//    else exception(linha, word);
//
//	sh = s;
//	pch=getWord2(pch, sh, pos);
//	//scanf("%s",aux);
//   }
//   // if(n)
//   // printf("\n");
//  }
//
// }
//
//}
//
///****************************************************************************/
///*     estrai o token vhdl de s e atualiza a string                         */
///*     retorna atualizada ou null                                           */
///*                                                                          */
///****************************************************************************/
//char* getWord2(char *s, char *t, int linha)
//{
//  /* First skip over leading white spaces */
//  while (*s == ' ' || *s == '\t')
//    s++;
//
//   if (*s == '\0' || *s == '\n' || *s == '\r')
//    return(NULL);
//
//  if (*s == '/' || *s == '*' || *s == '.' || *s == '+' || *s == '-' || *s == '=' || *s == '(' || *s == ')' || *s == ',' || *s == ';' || *s == '>' || *s == '<' || *s == ':' || *s == '"' || *s == 0x27 || *s == '|')
//  {
//	*t++ = *s++;
//	if(*s == '*' || *s == '=' || *s == '>')
//	 *t++ = *s++;
//
//
//
//	*t = '\0';
//	return s;
//  }
//
//  while ( *s != ' ' && *s != '\t' && *s != '\n' && *s != '\0' && *s != ';' && *s != ',' && *s != 0x27 && *s != '"' && *s != '/' && *s != '*' && *s != '+' && *s != '-' && *s != ':' && *s != '.' && *s != '<' && *s != '>' && *s != '=' && *s != '(' && *s != ')' && *s != '#' && *s != '&' && *s != '|' )
//  {
//	if ( *s == '!' || *s == '#' || *s == '$' || *s == '%' || *s == '?' || *s == '@' || *s == '[' || *s == 0x5C || *s == ']' || *s == 0x5E || *s == 0x60 || *s == '{' || *s == '}' || *s == '~' || *s > 0x7E)
//	{
//     printf("Erro, linha %d: Caracter Ivalido! %c\n",linha,*s);
//     exit(1);
//    }
//	*t++ = *s++;
//
//  }
//
// *t = '\0';
// return s;
//
//}

/****************************************************************************/
/*    ler uma entrada de dado no formato .eqn de berkley                    */
/*     retorna uma lista de tokens                                          */
/*                                                                          */
/****************************************************************************/
void blerEntrada (No* Dec, char *nome)
{
    FILE *pa;
    char linha[MAX],aux[MAX],*point,*vchar,*ux,vvv[3];
    No *l,*m,*n,In,Out,Buffer;
    int lin=0,tam,flag=0,h,x=0,y=0,prod=0,lit=0,i,k,or=0;
    pa = fopen (nome, "r");
    checkFile(pa, nome);


    while(!feof(pa))
    {
        fgets(linha , MAX , pa );
        lin++;
        for (i=0 ;  i < strlen(linha) ; i++)
            if(linha[i] < 32 || linha[i] =='#' )
                linha[i]='\0';
        tam=strlen(linha);
        if(tam>2&&linha[0]!='#') // n�o pega linha vazia ou linhas que s�o comentarios
        {
            h=tam-1;
            x=0;
            if(flag==0)
            {
                ux=linha;
                while(linha[x]==' '&&linha[x]!='\0' && x<h)
                    x++;

                while(linha[x]!=' ')
                {
                    aux[x]=linha[x];
                    x++;
                }
                aux[x]='\0';

                while(linha[x] == ' ' && x && x <h)
                    x++;

                vchar=copia(aux);
                insNo(Dec,vchar);
                l=buscaNo(Dec,vchar);
                if(l==NULL)
                {
                    printf("erro n�o encontrou %s\n",vchar);
                    exit(1);
                }

                l->prod=(No*) malloc(sizeof(No));
                l->prod->prox=iniNo();
                l->cor=1;
                flag=1;
                k=0;

                if(linha[x]!='=')
                {
                    printf("error linha %d...",lin);
                    exit(1);
                }
                x++;

            }
            if(flag==1)
            {
                while(flag==1&&linha[x] !='\0'&& linha[x] !='\n')
                {
                    while((linha[x] == '&'||linha[x] == ' '|| linha[x] == '(')&& linha[x] !='\0'&& linha[x] !='\n')
                    {
                        if(linha[x]=='&' && k == 0)
                        {
                            k=1;
                            prod++;
                            printf("Prod: %d \n",prod);
                        }
                        x++;
                    }

                    if(linha[x]=='|')
                    {
                        k = 0;
                        or++;
                        printf("Or gate: %d\n",or);
                    }
                    y=0;
                    while(linha[x]!=';'&&linha[x]!='&'&&linha[x]!=' '&&linha[x] != '('&&linha[x] != ')'&& linha[x] !='\0'&& linha[x] !='\n')
                    {
                        aux[y]=linha[x];
                        if(linha[x]=='|')
                            k = 0;

                        if(linha[x]=='&' && k == 0)
                        {
                            k=1;
                            prod++;
                            printf("Prod: %d \n",prod);
                        }
                        x++;
                        y++;

                    }
                    aux[y]='\0';

                    vchar=copia(aux);
                    insNo(l->prod,vchar);  // insere aqui
                    lit++;
                    printf("Lit: %d    %s \n",lit,vchar);
                    while((linha[x] == ' '|| linha[x] == '('||linha[x] == ')')&& linha[x] !='\0'&& linha[x] !='\n')
                        x++;
                    if(linha[x]==';')
                    {
                        flag=0;
                        k=0;

                    }
                }

            }

        }
        //exit(1);
    }
    ////////////////////////////
    for(l=Dec->prox; l!=NULL; l=l->prox)
    {
        for(n=Dec->prox; n!=NULL; n=n->prox)
        {
            for(m=n->prod->prox; m!=NULL; m=m->prox)
            {
                strcpy(aux,m->nome);
                //l->nome[strlen(l->nome)-2]='\0';
                if(aux[0]=='!')
                {
                    ux=m->nome;
                    ux++;
                    strcpy(aux,ux);
                }
                //printf("\naux  %s ",aux);
                if(aux[strlen(aux)-1]==39)
                    aux[strlen(aux)-1]='\0';
                if(strcmp(aux,l->nome)==0)
                {
                    l->cor=2;
                    m->cor=2;
                }
                //printf("	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
            }
        }
    }
    fclose(pa);
}

/****************************************************************************/
/*    ler uma entrada de dado no formato .eqn da ferramenta atacs           */
/*     retorna uma lista de tokens                                          */
/*                                                                          */
/****************************************************************************/
void alerEntrada (No* Dec, char *nome)  // para padr�o .prs do atacs
{
    FILE *pa;
    char linha[MAX],aux[MAX],*point,*vchar,*ux,vvv[3];
    No *l,*m,*n,In,Out,Buffer;
    int lin=0,tam,flag=0,h,x=0,y=0,flag2=0;
    pa = fopen (nome, "r");
    checkFile(pa, nome);

    while(!feof(pa))
    {
        fgets(linha , MAX , pa );
        lin++;
        tam=strlen(linha);
        if(tam>2&&linha[0]!='#'&&tam>2&&linha[0]!=';') // n�o pega linha vazia ou linhas que s�o comentarios
        {
            h=tam-1;;
            while((linha[h]=='\0'||linha[h]==10||linha[h]=='\n')||linha[h]==' '||linha[h]=='	')
            {
                linha[h]='\0';
                h--;
            }

            x=0;
            if(linha[0]=='[')
            {
                x+=2;
                y=0;
                //printf("\n point %s\n",point);
                ux=linha;

                while(linha[x]!=':')
                {
                    aux[y]=linha[x];
                    x++;
                    y++;
                }
                aux[y]='\0';
                // printf("\n aux:%s \n",aux);
                //while((linha[x] == ' '|| linha[x] == '[') && linha[x]!='\0'&& linha[x]!='\n')
                l=buscaNo(Dec,aux);

                //printf("chegou aqui 000\n");
                flag2=1;
                if(l==NULL)
                {
                    //printf("chegou aqui 00\n");
                    vchar=(char*)malloc(strlen(aux)*sizeof(char));
                    vchar=copia(aux);
                    insNo(Dec,vchar);
                    l=buscaNo(Dec,vchar);
                    if(l==NULL)
                    {
                        printf("erro n�o encontrou %s\n",vchar);
                        exit(1);
                    }

                    l->prod=(No*) malloc(sizeof(No));
                    l->prod->prox=iniNo();
                    l->cor=1;

                    if(linha[x]!=':')
                    {
                        printf("error linha %d...",lin);
                        exit(1);
                    }

                    //printf("chegou aqui 0\n");
                    flag2=0;
                }
                x++;


                flag=1;
                do {
                    while(linha[x]=='&' || linha[x] == ' ' || linha[x] == '(')
                    {   x++;
                        if(linha[x] == '(' && flag2==1 )
                        {
                            aux[0]='|';
                            aux[1]='\0';
                            vchar=copia(aux);
                            insNo(l->prod,vchar);
                        }

                    }

                    y=0;
                    while(linha[x]!=' '&&linha[x] != ')')
                    {
                        aux[y]=linha[x];
                        x++;
                        y++;
                    }
                    aux[y]='\0';
                    // printf(" %s ",aux);
                    vchar=copia(aux);
                    insNo(l->prod,vchar);


                    if(linha[x]==')')
                        flag=0;


                } while(flag);
                //printf("\n");

            }   // '!'
        }
    }
    ////////////////////////////
    for(l=Dec->prox; l!=NULL; l=l->prox)
    {
        for(n=Dec->prox; n!=NULL; n=n->prox)
        {
            for(m=n->prod->prox; m!=NULL; m=m->prox)
            {
                strcpy(aux,m->nome);
                if(aux[0]=='~')
                {
                    ux=m->nome;
                    ux++;
                    strcpy(aux,ux);
                }
                if(aux[strlen(aux)-1]==39)
                    aux[strlen(aux)-1]='\0';
                if(strcmp(aux,l->nome)==0)
                {
                    l->cor=2;
                    m->cor=2;
                }

            }
        }

    }
    fclose(pa);
}

/****************************************************************************/
/*     inser��o no in�cio: retorna a lista atualizada                       */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void insNo (No* l, char *i)
{
    No  *p,*novo = (No*) malloc(sizeof(No));
    novo->nome = i;
    novo->cor = 0;
    novo->prox=NULL;
    if(l->prox==NULL)
    {
        l->prox=novo;
        return;
    }

    for(p=l; p->prox!=NULL; p=p->prox);
    p->prox=novo;
}

/****************************************************************************/
/*    fun��o busca: busca um elemento na lista                              */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
No* buscaNo (No* l, char v[])
{
    No* p;
    if(l->prox!=NULL)
    {
        for (p=l->prox; p!=NULL; p=p->prox)
            if (strcmp(p->nome, v)==0)
                return p;
    }
    return NULL; /* n�o achou o elemento */
}

/****************************************************************************/
/*      fun��o de inicializa��o: retorna uma lista vazia                    */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
No* iniNo (void)
{
    return NULL;
}

/****************************************************************************/
/*       exibeNo:                                                           */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void exibeNo (No* l)
{
    int j;
    No* p; /* vari�vel auxiliar para percorrer a lista */
    if(l->prox!=NULL)
    {
        for (p = l->prox,j=0; p!=NULL; p = p->prox,j++)
            printf("    %s    ", p->nome);
    }
}

/****************************************************************************/
/*    fun��o liberaNo: libera a mem�ria da lista                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void liberaNo (No* l)
{
    No* p = l;
    while (p != NULL) {
        No* t = p->prox; /* guarda refer�ncia para o pr�ximo elemento
		*/
        free(p); /* libera a mem�ria apontada por p */
        p = t; /* faz p apontar para o pr�ximo */
    }
}

/****************************************************************************/
/*    fun��o copia: retorna um poteiro da string copiada                    */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
char* copia(char *aux)
{
    char *obj;
    int x;
    // for(x=1;aux[x]!='\0';x++);
    // printf("\nx %d  aux %d\n",x,strlen(aux));
    obj=(char*) malloc((strlen(aux)+1)*sizeof(char));
    //if(obj[strlen(aux)]=='\0')
    //   printf("\na string e finalizada\n");
    for(x=0; x<strlen(aux)&&aux[x]!='\n'&&aux[x]!=' '&&aux[x]!='	'&&aux[x]!='\0'; x++)
        obj[x]=aux[x];
    obj[x]='\0';

    return obj;
}

bool valida_nome(char * nome)
{
    int x, l;
    l=strlen(nome);
    //                  0 - 9
    if( nome[0] < 64)
        return false;
    for(x=1; x< l; x++)
        if(fora(&nome[x])==1)
            return false;
    return true;

}
bool fora (char * nome)
{
    if(*nome == 45) // troca o - pelo _
    {
        *nome = 95;
        return false;
    }
    //          0 - 9                        A  - Z                         a - z                    _               -
    if((*nome > 47 && *nome < 58) || (*nome > 96 && *nome < 123 ) || (*nome > 65 && *nome < 91 ) ||(*nome == 95)||(*nome == 45))
        return false;

    return true;
}

/****************************************************************************/
/*    fun��o copia: retorna um poteiro da string copiada                    */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void stripFext (char *str)
{
    while (*str != '\0' && *str != '.')
    {
        if(*str == '-')
            *str='_';
        str++;
    }
    if (*str == '.')
        *str = '\0';
}

/****************************************************************************/
/*    fun��o checkFile: Notifica��o de arquivo n�o encontrado               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void checkFile (FILE *file, char *name)
{
    if (file == NULL)  {
        printf("%s not Found.\n", name);
        exit (1);
    }
}

/****************************************************************************/
/*    fun��o cl_materializa: gera os arquivos .vhd para  arquitetura        */
/*                           de clock local do sicarelo                     */
/*                                                                          */
/****************************************************************************/
void cl_materializa (No *Dec, No *In, No *Out, No *Buffer, char *nome, int ini)
{
    FILE *spec_a, *conf_a, *sinais_a, *tb_a;;
    int test,literais=0,produtos=0,cont,x,y,z,enb,v=0,set=0;
    No *l,*l2,*m,*m2,*n,*n2;
    char na[MAX];
    char spec_n[MAX], conf_n[MAX], sinais_n[MAX], tb_n[MAX];
    strcpy(na, nome);
    stripFext(na);

    sprintf(spec_n, "%s.vhd",na);         // ok
    sprintf(conf_n, "conf_%s_post.vhd",na);    // ok
    sprintf(sinais_n, "sinais_%s.vhd",na);// ok
    sprintf(tb_n, "tb_%s.vhd",na);        // ok
    // abrindo os arquivos:
    // name.vhd              -> arquivo de s�ntese
    // conf_name_post.vhd    -> arquivo de configura��o
    // sinais_name.vhd       -> arquivo de estimulos
    // tb_name.vhd           -> entidade de testa da especifica��o a ser s�ntetizada

    spec_a = fopen(spec_n, "w");
    checkFile(spec_a, spec_n);
    conf_a = fopen(conf_n, "w");
    checkFile(conf_a, conf_n);
    sinais_a = fopen(sinais_n, "w");
    checkFile(sinais_a, sinais_n);
    tb_a = fopen(tb_n, "w");
    checkFile(tb_a, tb_n);
    printf("\n Gerando o arquivo: %s  \n",spec_n);

    stripFext(spec_n);
    stripFext(conf_n);
    stripFext(sinais_n);
    stripFext(tb_n);

    fprintf(spec_a,"----------------\n\n");
    fprintf(spec_a,"-- Codigo Gerado Pela Ferramenta geraVHD --\n");
    fprintf(spec_a,"-- Autor: Tiago da Silva Curtinhas (2013) --\n\n");
    fprintf(spec_a,"library ieee;\n");
    fprintf(spec_a,"use ieee.std_logic_1164.all;\n");
    fprintf(spec_a,"LIBRARY altera_mf;\n");
    fprintf(spec_a,"USE altera_mf.altera_mf_components.all;\n");
    fprintf(spec_a,"ENTITY %s IS\n",na);
    fprintf(spec_a,"PORT(\n");
    fprintf(spec_a,"\t\t");
    addItem(In,spec_a);
    fprintf(spec_a,", rst");
    fprintf(spec_a," : IN std_logic;\n");
    addItem(Out,spec_a);
    fprintf(spec_a,"\t\t");
    if(Buffer->prox!=NULL)
    {
        fprintf(spec_a," : OUT std_logic;\n");
        addItem(Out,spec_a);
        fprintf(spec_a,"\t\t");
        fprintf(spec_a," : OUT std_logic);\n");
    }
    else
        fprintf(spec_a," : OUT std_logic);\n");
    fprintf(spec_a,"END %s;\n\n",na);

    fprintf(spec_a,"ARCHITECTURE bhv OF %s IS\n\n",na);

    fprintf(spec_a,"  component lcell\n");
    fprintf(spec_a,"   port ( a_in : in std_logic;  a_out : out std_logic);\n");
    fprintf(spec_a,"  end component;\n\n");

    if(Out->prox!=NULL)
    {
        fprintf(spec_a,"\n SIGNAL CK, ");
        for(l=Out->prox; l!=NULL; l=l->prox)
        {
            fprintf(spec_a,"s_%s",l->nome);
            if(l->prox!=NULL)
                fprintf(spec_a,", ");
        }
        fprintf(spec_a," : std_logic;\n");
    }
    if(Buffer->prox!=NULL)
    {
        fprintf(spec_a," SIGNAL ");
        for(l=Buffer->prox; l!=NULL; l=l->prox)
        {
            fprintf(spec_a,"s_%s",l->nome);
            if(l->prox!=NULL)
                fprintf(spec_a,", ");
        }
        fprintf(spec_a," : std_logic;\n");
    }

    if(Out->prox!=NULL)
    {
        fprintf(spec_a," SIGNAL ");
        for(l=Out->prox; l!=NULL; l=l->prox)
        {
            fprintf(spec_a,"d_%s",l->nome);
            if(l->prox!=NULL)
                fprintf(spec_a,", ");
        }
        fprintf(spec_a," : std_logic;\n");
    }
    if(Buffer->prox!=NULL)
    {
        fprintf(spec_a," SIGNAL ");
        for(l=Buffer->prox; l!=NULL; l=l->prox)
        {
            fprintf(spec_a,"d_%s",l->nome);
            if(l->prox!=NULL)
                fprintf(spec_a,", ");
        }
        fprintf(spec_a," : std_logic;\n\n");
    }

    fprintf(spec_a,"BEGIN\n\n");

    fprintf(spec_a,"  buff: lcell port map (s_fgr, CK);\n");

    if(Out->prox!=NULL)
    {
        for(l=Out->prox; l!=NULL; l=l->prox)
            fprintf(spec_a,"  %s <= s_%s;\n",l->nome,l->nome);
    }

    if(Buffer->prox!=NULL)
    {
        for(l=Buffer->prox; l!=NULL; l=l->prox)
            fprintf(spec_a,"  %s <= s_%s;\n",l->nome,l->nome);
    }
    fprintf(spec_a,"\n\n");

    fprintf(spec_a," abc: process (rst, CK)\n");
    fprintf(spec_a," BEGIN\n\n");
    fprintf(spec_a,"  if rst = '0' then\n\n");
    if(Out->prox!=NULL)
    {
        for(l=Out->prox; l!=NULL; l=l->prox)
            if(strcmp(l->nome,"fgr")!=0)
                fprintf(spec_a,"   s_%s <= '0';\n",l->nome);
    }
    if(Buffer->prox!=NULL)
    {
        for(l=Buffer->prox; l!=NULL; l=l->prox)
            fprintf(spec_a,"   s_%s <= '0';\n",l->nome);
        //fprintf(spec_a,"\n");
    }
    fprintf(spec_a,"\n  elsif rising_edge(CK) then\n\n");

    if(Out->prox!=NULL)
    {
        for(l=Out->prox; l!=NULL; l=l->prox)
            fprintf(spec_a,"   s_%s <= d_%s;\n",l->nome,l->nome);
    }
    if(Buffer->prox!=NULL)
    {
        for(l=Buffer->prox; l!=NULL; l=l->prox)
            fprintf(spec_a,"   s_%s <= d_%s;\n",l->nome,l->nome);
        //fprintf(spec_a,"\n");
    }

    fprintf(spec_a,"\n  end if;\n");
    fprintf(spec_a," END process abc;\n");

    for(l=Dec->prox; l!=NULL; l=l->prox)
    {
        test=0;

        for(m=l->prod->prox; m!=NULL; m=m->prox) // no caso de: s<=((a AND b) OR (c and d));
            if(m->nome[0]=='+'||m->nome[0]=='|')
                test++;

        if(test)
        {
            if(l->cor==2)
                fprintf(spec_a,"   s_%s <=((",l->nome);
            else
                fprintf(spec_a,"   %s <=((",l->nome);
        }
        else
        {
            if(l->cor==2)
                fprintf(spec_a,"   s_%s <=(",l->nome);
            else
                fprintf(spec_a,"   %s <=(",l->nome);
        }

        enb=0;
        for(m=l->prod->prox; m!=NULL; m=m->prox)
        {
            if(m->nome[0]=='+'||m->nome[0]=='|')
            {
                if(set)
                {
                    //produtos++;
                    if(ini == 1 &&vet_ini[v] == '0' )
                        fprintf(spec_a," AND rst");
                    set=0;
                }
                fprintf(spec_a,") OR (");//m->nome

                if(enb)
                    produtos++;
                enb=0;
            }
            else
            {
                if(m->nome[strlen(m->nome)-1]==39||m->nome[0]=='!'||m->nome[0]=='~')
                {
                    if(m->nome[strlen(m->nome)-1]==39)
                        m->nome[strlen(m->nome)-1]='\0';

                    if(m->nome[0]=='!' || m->nome[0]=='~')
                    {
                        for(z=0; m->nome[z]!='\0'; z++)
                            m->nome[z]=m->nome[z+1];
                    }
                    if(m->cor==2)
                        fprintf(spec_a,"NOT(s_%s)",m->nome);
                    else
                        fprintf(spec_a,"NOT(%s)",m->nome);
                }
                else
                {
                    if(m->cor==2)
                        fprintf(spec_a," s_%s",m->nome);
                    else
                        fprintf(spec_a," %s",m->nome);
                }
                if(m->prox!=NULL)
                {
                    if(m->prox->nome[0]!='+'&&m->prox->nome[0]!='|')
                    {
                        fprintf(spec_a," AND ");
                        enb++;
                        set=1;
                    }
                }
                literais++;
            }
        }

        if(set)
        {
            //produtos++;
            if(ini == 1 &&vet_ini[v] == '0' )
                fprintf(spec_a," AND rst");
            set=0;
        }

        if(test)
        {
            if(ini == 1 &&vet_ini[v] == '1' )
                fprintf(spec_a,") OR not(rst));\n");
            else
                fprintf(spec_a,"));\n");
        }
        else
        {
            if(ini == 1 &&vet_ini[v] == '1' )
                fprintf(spec_a,") OR not(rst);\n");
            else
                fprintf(spec_a,");\n");
        }
        v++;
    }

    fprintf(spec_a,"\n");
    fprintf(spec_a,"END bhv;\n");
    //------------------------- Gerando arq_conf -------------------------

    fprintf(conf_a,"----------------\n");
    fprintf(conf_a,"-- Codigo Gerado Pela Ferramenta geraVHD --\n");
    fprintf(conf_a,"-- Autor: Tiago da Silva Curtinhas (2013) --\n\n");
    fprintf(conf_a,"configuration %s of %s is\n",conf_n,tb_n);
    fprintf(conf_a," for tb\n");
    fprintf(conf_a,"  for comp : %s use entity work.%s(structure);\n",spec_n,spec_n);
    fprintf(conf_a,"  end for;\n");
    fprintf(conf_a,"  for suv : %s use entity work.%s(bhv_sinais);\n",sinais_n,sinais_n);
    fprintf(conf_a,"  end for;\n");
    fprintf(conf_a," end for;\n");
    fprintf(conf_a,"end %s;\n",conf_n);

    //------------------------- Gerando arq_sinais -----------------------

    fprintf(sinais_a,"----------------\n\n");
    fprintf(sinais_a,"-- Codigo Gerado Pela Ferramenta geraVHD --\n");
    fprintf(sinais_a,"-- Autor: Tiago da Silva Curtinhas (2013) --\n\n");
    fprintf(sinais_a,"library ieee;\n");
    fprintf(sinais_a,"use ieee.std_logic_1164.all;\n");
    fprintf(sinais_a,"ENTITY %s IS\n",sinais_n);
    fprintf(sinais_a,"PORT(\n");
    addItem (In, sinais_a);
    fprintf(sinais_a,"rst : OUT std_logic);\n");
    fprintf(sinais_a,"END %s;\n\n",sinais_n);

    fprintf(sinais_a,"ARCHITECTURE bhv_sinais OF %s IS\n\n",sinais_n);

    fprintf(sinais_a,"  constant half_clock_period : time := 10ns;\n\n");
    fprintf(sinais_a,"BEGIN\n\n");
    fprintf(sinais_a," sinais: process\n");
    fprintf(sinais_a,"  BEGIN\n\n");
    for(l=In->prox; l!=NULL; l=l->prox)
        fprintf(sinais_a,"   %s <='0';\n",l->nome);
    fprintf(sinais_a,"   rst <='0';\n");
    fprintf(sinais_a,"   wait for half_clock_period;\n");
    fprintf(sinais_a,"   wait for half_clock_period;\n");
    fprintf(sinais_a,"   rst <='1';\n");
    fprintf(sinais_a,"   wait for half_clock_period;\n");
    fprintf(sinais_a,"   wait for half_clock_period;\n");
    fprintf(sinais_a,"   wait for half_clock_period;\n");
    fprintf(sinais_a,"end process sinais;\n\n");
    fprintf(sinais_a,"END bhv_sinais;\n");

    //------------------------- Gerando arq_tb ---------------------------

    fprintf(tb_a,"----------------\n\n");
    fprintf(tb_a,"-- Codigo Gerado Pela Ferramenta geraVHD --\n");
    fprintf(tb_a,"-- Autor: Tiago da Silva Curtinhas (2013) --\n\n");
    fprintf(tb_a,"library ieee;\n");
    fprintf(tb_a,"use ieee.std_logic_1164.all;\n");
    fprintf(tb_a,"ENTITY %s IS\n",tb_n);
    fprintf(tb_a,"END %s;\n\n",tb_n);

    fprintf(tb_a,"ARCHITECTURE tb OF %s IS\n\n",tb_n);

    fprintf(tb_a,"signal ");
    addItem (In, tb_a);
    fprintf(tb_a,", rst : std_logic;\n");

    fprintf(tb_a,"signal ");
    addItem (Out, tb_a);
    fprintf(tb_a," : std_logic;\n");

    fprintf(tb_a,"signal ");
    addItem (Buffer, tb_a);
    fprintf(tb_a," : std_logic;\n\n");

    fprintf(tb_a,"-- declara��o de components\n");
    fprintf(tb_a,"component %s is\n",spec_n);
    fprintf(tb_a,"PORT (\n");
    addItem (In, tb_a);
    fprintf(tb_a,", rst : IN std_logic;\n");
    addItem (Out, tb_a);
    fprintf(tb_a," : OUT std_logic;\n");
    addItem (Buffer, tb_a);
    fprintf(tb_a," : OUT std_logic);\n");
    fprintf(tb_a,"end component;\n\n");

    fprintf(tb_a,"-- declara��o de components\n");
    fprintf(tb_a,"component %s is\n",sinais_n);
    fprintf(tb_a,"PORT (");
    addItem (In, tb_a);
    fprintf(tb_a,", rst : OUT std_logic);");
    fprintf(tb_a,"end component;\n\n");
    fprintf(tb_a,"BEGIN\n\n");

    fprintf(tb_a,"-- port map declaration for\n");
    fprintf(tb_a," comp : %s\n",spec_n);
    fprintf(tb_a,"  PORT MAP (\n");
    addPortMap (In, tb_a);
    fprintf(tb_a,"rst=>rst,\n");
    addPortMap (Out, tb_a);
    fprintf(tb_a,", ");
    addPortMap (Buffer, tb_a);
    fprintf(tb_a,");\n\n");

    fprintf(tb_a,"-- port map declaration for\n");
    fprintf(tb_a,"suv : %s\n",sinais_n);
    fprintf(tb_a,"PORT MAP (\n");
    addPortMap (In, tb_a);
    fprintf(tb_a,"rst=>rst);\n\n");
    fprintf(tb_a,"END architecture tb;\n");

    //--------------------------------------------------------------------

    printf(" Arquivos Gerados Com Sucesso!\n");
    printf(" Produtos: %d  \n Literais: %d \n",produtos,literais);

    fclose(spec_a);
    fclose(conf_a);
    fclose(sinais_a);
    fclose(tb_a);

}


/****************************************************************************/
/*    fun��o toEqn: converte .eqn atacs para .eqn 3D                        */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void toEqn (No* Dec, char *nome) // converte o padr�o .prs do atacs para o .eqn  do 3D
{
    FILE *spec_a;
    int test,literais=0,produtos=0,cont,x,y,z,enb,v=0,set=0;
    No *l,*l2,*m,*m2,*n,*n2;
    char spec_n[MAX],var[MAX],*aux,na[MAX];
    strcpy(spec_n, nome);
    stripFext(spec_n);
    strcpy(na,spec_n);
    strcat(spec_n, ".eqn");

    if ((spec_a = fopen(spec_n, "w")) == NULL)
    {
        printf("cannot open output file \n");
        exit(1);
    }

    printf("\n Gerando o arquivo: %s  \n",spec_n);

    for(l=Dec->prox; l!=NULL; l=l->prox)
    {
        printf("%s = \n",l->nome);
        fprintf(spec_a,"%s = \n",l->nome);

        for(m=l->prod->prox; m!=NULL; m=m->prox)
        {
            fprintf(spec_a," ");
            printf(" ");
            strcpy(var,m->nome);

            if(var[0]=='!' || var[0]=='~')
            {
                for(z=0; var[z]!='\0'; z++)
                    var[z]=var[z+1];
                fprintf(spec_a," %s'",var);
                printf(" %s'",var);
            }
            else if(var[0]!='|' &&  var[0]!='+')
            {
                fprintf(spec_a," %s",m->nome);
                printf(" %s",m->nome);
            }

            if(m->nome[0]=='+' || m->nome[0]=='|')
            {
                fprintf(spec_a," +\n");
                printf(" +\n");

            }
            if(m->prox==NULL && l->prox!=NULL)
            {
                fprintf(spec_a,"\n\n");
                printf("\n\n");
            }
        }
    }
    fprintf(spec_a,"\n");
    printf("\n");


}

/****************************************************************************/
/*    fun��o not: Inverte o n�vel digital do sinal                         */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
int not(int x)
{
    if (x == 1) return 0;
    if (x == 0) return 1;
    if (x == 2) return 2;
    return 1;
}

/****************************************************************************/
/*    fun��o addStO: atribui um sinal interno para um sa�da                  */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void addStO (No *list, FILE *arq)
{
    No *l;
    for(l=list->prox; l!=NULL; l=l->prox)
        fprintf(arq,"  %s <= s_%s;\n",l->nome,l->nome);
}

/****************************************************************************/
/*    fun��o addStO: atribui um sinal interno para um sa�da                  */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void addPortMap (No *list, FILE *arq)
{
    No *l;
    for(l=list->prox; l!=NULL; l=l->prox)
        fprintf(arq,"  %s => %s;\n",l->nome,l->nome);
}

/****************************************************************************/
/*    fun��o addItem: Adiciona uma lista de item no arquivo                 */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void addItem (No *list, FILE *arq)
{
    No *l;
    for(l=list->prox; l!=NULL; l=l->prox)
    {
        fprintf(arq,"%s",l->nome);
        if(l->prox!=NULL)
            fprintf(arq,", ");
    }
}

/****************************************************************************/
/*    fun��o insInOutBuffer: Adiciona as In Out e Buffer a partir da lista  */
/*                           de tokens                                      */
/*                                                                          */
/****************************************************************************/
void insInOutBuffer(No *Dec, No *In, No *Out, No *Buffer, char *nome)
{
    int z;
    No *l,*m,*n;
    char var[MAX],*aux,na[MAX];
    char spec_n[MAX], conf_n[MAX], sinais_n[MAX], tb_n[MAX];
    strcpy(na, nome);
    stripFext(na);

    for(l=Dec->prox; l!=NULL; l=l->prox)
    {
        if(l->cor==1)
        {
            aux=copia(l->nome);
            insNo(Out,aux);
        } else if(l->cor==2)
        {
            aux=copia(l->nome);
            insNo(Buffer,aux);
        }
        for(m=l->prod->prox; m!=NULL; m=m->prox)
        {
            strcpy(var,m->nome);
            if(var[strlen(var)-1]==39)
                var[strlen(var)-1]='\0';

            if(var[0]=='!' || var[0]=='~')
            {
                for(z=0; var[z]!='\0'; z++)
                    var[z]=var[z+1];
            }

            if(m->cor==0&&m->nome[0]!='+'&&m->nome[0]!='|')
            {
                n=buscaNo(In,var);
                if(n==NULL)
                {
                    aux=copia(var);
                    insNo(In,aux);
                }
            }
        }

    }

}