#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 54000


typedef struct no{
              char* nome;
			  int cor;
			  int tam;
			  struct no *prox;
			  //struct no *list;
			  struct no *prod;
			  
             } No;

			 
void insNo (No* l, char *i);
No* buscaNo (No* l,char v[]);
No* iniNo (void);
void exibeNo (No* l);
void removeNo (No* l, char *i);	
void liberaNo (No* l);	
void lerEntrada(No* Dec, char *nome);	// para padrão .eqn normal
void blerEntrada(No* Dec, char *nome);  // para padrão .eqn de berkley
void alerEntrada(No* Dec, char *nome);  // para padrão .prs do atacs
void toEqn(No* Dec, char *nome);  // para padrão .prs do atacs

char* copia(char *aux); 
void stripFext(char *str);
void materializa(No *Dec, No *In, No *Out, No *Buffer, char *nome, int ini);
char* getWord(char *aux);

char *vet_ini;			 
			 
int main(int argc, char *argv[])
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
        for(x=0;argv[4][x]!='\0';x++) 
		{
			if(argv[4][x]!='0'&&argv[4][x]!='1')
			{
				printf("A inicialização só pode conter valores entre zero e um\n");
				return 1;
			}
		}	
		
	    ini=1;
		vet_ini=(char *) malloc(strlen(argv[4])*sizeof(char));
		vet_ini=copia(argv[4]);
	  
    }else
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
  materializa(Dec,In,Out,Buffer,argv[1],ini);
 return 0;
}

void lerEntrada(No* Dec, char *nome)
{
 FILE *pa;
 char linha[MAX],aux[MAX],*point,*vchar;
 No *l,*m,*n,In,Out,Buffer;
 int lin,tam,flag=0,h;

  pa = fopen (nome, "r");
  if (pa == NULL ) { /* verifica erro na abertura */
    printf(" Arquivo nao pode ser aberto .");
    exit(1);
   }
  
 
    while(!feof(pa)) 
   { 
     fgets(linha , MAX , pa );
	 lin++;
     tam=strlen(linha);
      if(tam>2 && linha[0]!=';' && linha[0]!='#') // não pega linha vazia ou linhas que são comentarios
       {
	        h=tam-1;;
	      while((linha[h]=='\0'||linha[h]==10||linha[h]=='\n')||linha[h]==' '||linha[h]=='	')
			{
			  linha[h]='\0';
			  h--;
			}
		  //printf("linha: %s \n\n",linha);
		  point=strtok(linha," ");
		  
	      if(flag==0)
		  {
		    
			//vchar=(char*)malloc(strlen(point)*sizeof(char));
			vchar=copia(point);
			insNo(Dec,vchar);
			l=buscaNo(Dec,vchar);
			if(l==NULL)
			{
			  printf("erro não encontrou %s\n",vchar);
			  exit(1);
			}
			
			l->prod=(No*) malloc(sizeof(No));
			l->prod->prox=iniNo();
			l->cor=1; 
			flag=1;
			//printf("\n entrou %s",l->nome);
			
			
			}else{
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
   
   
   ////////////////////////////
   for(l=Dec->prox;l!=NULL;l=l->prox)
   {
      
	  for(n=Dec->prox;n!=NULL;n=n->prox)
	  {
	    for(m=n->prod->prox;m!=NULL;m=m->prox)
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
			printf("\n	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
			printf("\n");
			if(strcmp(aux,l->nome)==0)
			{
				l->cor=2;
				m->cor=2;

			}
			printf("	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
		}
	  }
	  
	  printf("\n saidaK: %s  cor %d\n",l->nome,l->cor);
   }
   
  
   fclose(pa);
   
   ////////////////////////////////////
   //fprintf(fpout," | ");
  
   ///////////////////////////////////
   
}

void blerEntrada(No* Dec, char *nome)
{
 FILE *pa;
 char linha[MAX],aux[MAX],*point,*vchar,*ux,vvv[3];
 No *l,*m,*n,In,Out,Buffer;
 int lin=0,tam,flag=0,h,x=0,y=0,prod=0,lit=0,k,or=0;

  pa = fopen (nome, "r");
  if (pa == NULL ) { /* verifica erro na abertura */
    printf(" Arquivo nao pode ser aberto .");
    exit(1);
   }
  
 
    while(!feof(pa)) 
   { 
     fgets(linha , MAX , pa );
	 lin++;
     tam=strlen(linha);
      if(tam>2&&linha[0]!='#') // não pega linha vazia ou linhas que são comentarios
       {
	        h=tam-1;;
	      while(linha[h]=='\0'||linha[h]==10||linha[h]=='\n'||linha[h]==' '||linha[h]=='	')
			{
			  linha[h]='\0';
			  h--;
			}
		   // printf("linha %d tamanho %d: %s \n\n",lin,tam,linha);
		    //point=strtok(linha," ");
		    x=0;		  
	      if(flag==0)
		  {
		    //printf("\n point %s\n",point);
			ux=linha;
			
			while(linha[x]==' '&&linha[x]!='\0' && x<h)
			     {
				  //  aux[x]=linha[x];
					x++;	
				 }
				
			
			while(linha[x]!=' ')
			     {
				    aux[x]=linha[x];
					x++;	
				 }
				 aux[x]='\0';
				// printf("\n aux:%s\n",aux);
			while((linha[x] == ' ' && x && x <h)
			         x++;	 
			//vchar=(char*)malloc(strlen(point)*sizeof(char));
			vchar=copia(aux);
			insNo(Dec,vchar);
			l=buscaNo(Dec,vchar);
			if(l==NULL)
			{
			  printf("erro não encontrou %s\n",vchar);
			  exit(1);
			}
			
			l->prod=(No*) malloc(sizeof(No));
			l->prod->prox=iniNo();
			l->cor=1; 
			flag=1;
			k=0;
			//point=strtok(NULL," ");
			//printf("\n point %s\n",point);
			if(linha[x]!='=')
			  {
			    printf("error linha %d...",lin);
                exit(1);				
			  }
			  x++;
			  //x++;
			  //x++;
			//printf("\n entrou %s",l->nome);
			//printf("\nFoi...%s\n",point);
			//exit(1);
			//gcc E:\ITA\Dropbox\mscAG\geraVHD.c -o bgeraVHD.exe
			
			//getWord()
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
						//printf("\n aux:%s\n",aux);	
						//ux=linha;
						//ux+=x;
						// printf("\n Linha:");	
						// while(*ux!='\0'&&*ux!='\n')
						//      {
						//	    printf("%c",*ux);	
						//		ux++;
						//	  }
						//	  printf("\n");	
						//scanf("vvv:%s",vvv);
					    
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
   }
   
   //exit(1);
   
   ////////////////////////////
   for(l=Dec->prox;l!=NULL;l=l->prox)
   {
      
	  for(n=Dec->prox;n!=NULL;n=n->prox)
	  {
	    for(m=n->prod->prox;m!=NULL;m=m->prox)
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
			//printf("\n	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
			//printf("\n");
			if(strcmp(aux,l->nome)==0)
			{
				l->cor=2;
				m->cor=2;

			}
			//printf("	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
		}
	  }
	  
	  //printf("\n saidaK: %s  cor %d\n",l->nome,l->cor);
   }
   
  
   fclose(pa);
   
   ////////////////////////////////////
   //fprintf(fpout," | ");
  
   ///////////////////////////////////
   
}

void alerEntrada(No* Dec, char *nome)  // para padrão .prs do atacs
{
	FILE *pa;
	char linha[MAX],aux[MAX],*point,*vchar,*ux,vvv[3];
	No *l,*m,*n,In,Out,Buffer;
	int lin=0,tam,flag=0,h,x=0,y=0,flag2=0;

	pa = fopen (nome, "r");
	if (pa == NULL ) { /* verifica erro na abertura */
    printf(" Arquivo nao pode ser aberto .");
    exit(1);
    }
	
	while(!feof(pa)) 
    { 
     fgets(linha , MAX , pa );
	 lin++;
     tam=strlen(linha);
      if(tam>2&&linha[0]!='#'&&tam>2&&linha[0]!=';') // não pega linha vazia ou linhas que são comentarios
       {
	        h=tam-1;;
	      while((linha[h]=='\0'||linha[h]==10||linha[h]=='\n')||linha[h]==' '||linha[h]=='	')
			{
			  linha[h]='\0';
			  h--;
			}
		    //printf("linha %d tamanho %d: %s \n\n",lin,tam,linha);
		    //point=strtok(linha," ");
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
				printf("erro não encontrou %s\n",vchar);
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
            do{
				while(linha[x]=='&' || linha[x] == ' ' || linha[x] == '(')
			    {         x++;
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
			    
					
			}while(flag);
			//printf("\n");
			
		    }   // '!'
	    }
   }
   
   //printf("chegou aqui 1\n");
  
   //exit(1);
   
   ////////////////////////////
	for(l=Dec->prox;l!=NULL;l=l->prox)
	{
	  for(n=Dec->prox;n!=NULL;n=n->prox)
	  {
	    for(m=n->prod->prox;m!=NULL;m=m->prox)
		{
		    strcpy(aux,m->nome);
			//l->nome[strlen(l->nome)-2]='\0';
			if(aux[0]=='~')
			{
				ux=m->nome;
				ux++;
				strcpy(aux,ux);
			}
			//printf("\naux  %s ",aux);
			if(aux[strlen(aux)-1]==39)
			aux[strlen(aux)-1]='\0';
			//printf("\n	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
			//printf("\n");
			if(strcmp(aux,l->nome)==0)
			{
				l->cor=2;
				m->cor=2;
			}
			//printf("	l  %s  cor %d| aux %s cor %d \n",l->nome,l->cor,aux,m->cor);
		}
	  }
	  //printf("\n saidaK: %s  cor %d\n",l->nome,l->cor);
    }
	
	
	
	fclose(pa);
}

/****************************************************************************/        
/*     inserção no início: retorna a lista atualizada                       */
/*                                                                          */
/*                                                                          */
/****************************************************************************/ 
void insNo (No* l, char *i)
{
      No  *p,*novo = (No*) malloc(sizeof(No));
      novo->nome = i;
      novo->cor = 0;
	 //printf("\n inserindo o %s  tam %d \n\n ",novo->nome,strlen(novo->nome));
	  novo->prox=NULL;
	 if(l->prox==NULL)
       {  
         
         l->prox=novo;
         return;
        }
  
  for(p=l;p->prox!=NULL;p=p->prox);
   p->prox=novo;
}

/****************************************************************************/        
/*    função busca: busca um elemento na lista                              */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
No* buscaNo (No* l, char v[])
{
  No* p;
  //printf("call here \n");
   if(l->prox!=NULL)
   {
		for (p=l->prox; p!=NULL; p=p->prox)
			if (strcmp(p->nome, v)==0)
			   return p;
	}	
  // printf("\n nda");		
  return NULL; /* não achou o elemento */
}

/****************************************************************************/        
/*      função de inicialização: retorna uma lista vazia                    */
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
void exibeNo(No* l)
{
  int j;
  No* p; /* variável auxiliar para percorrer a lista */
   if(l->prox!=NULL)
	{  
		for (p = l->prox,j=0; p!=NULL; p = p->prox,j++)
			printf("    %s    ", p->nome);
			//printf("  n%d  ",j);
	}
}

/****************************************************************************/        
/*    função liberaNo: libera a memória da lista                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void liberaNo (No* l)
{
	No* p = l;
	while (p != NULL) {
		No* t = p->prox; /* guarda referência para o próximo elemento
		*/
		free(p); /* libera a memória apontada por p */
		p = t; /* faz p apontar para o próximo */
	}
}

char* copia(char *aux)
{
 char *obj;
 int x;
 // for(x=1;aux[x]!='\0';x++);
 // printf("\nx %d  aux %d\n",x,strlen(aux));
 obj=(char*) malloc((strlen(aux)+1)*sizeof(char));
 //if(obj[strlen(aux)]=='\0')
 //   printf("\na string e finalizada\n");
	for(x=0;x<strlen(aux)&&aux[x]!='\n'&&aux[x]!=' '&&aux[x]!='	'&&aux[x]!='\0';x++)
    obj[x]=aux[x];
    obj[x]='\0';
	
	return obj;
}

char* getWord(char *aux)
{
  char *obj=NULL, *ux;
  int x=0;
   // obj=(char*) malloc((strlen(aux)+1)*sizeof(char));
   
   printf("\nEntrou\n");
   ux=aux;
   
    if((*ux == ' '||*ux == '(')&& *ux!='\0'&& *ux!='\n')
			 ux++;
   
     printf("\n ux %s \n",ux);
   while (*ux != '\0' && *ux != ')' && *ux != ';' && *ux != '\n'&&*ux != ' ')
        {  
	        x++;
			ux++;	   
		}
		aux++;
		printf("\nx %d ux:%s aux:%s\n",x,ux,aux);
		//*aux= *ux++;
	    ux++;	
		*aux=*ux;
		//exit(1);
  return obj;
}

void stripFext(char *str)
{
  while (*str != '\0' && *str != '.')
    str++;
  if (*str == '.')
    *str = '\0';
}


void materializa(No *Dec, No *In, No *Out, No *Buffer, char *nome, int ini)
{
 FILE *fpout;
 int test,literais=0,produtos=0,cont,x,y,z,enb,v=0,set=0;
 No *l,*l2,*m,*m2,*n,*n2;
 char infn[MAX],var[MAX],*aux,na[MAX];
 strcpy(infn, nome);
 stripFext(infn);
 strcpy(na,infn);
 strcat(infn, ".vhd");
 
   
 // nda
   if ((fpout = fopen(infn, "w")) == NULL)
        { 
	      printf("cannot open output file \n"); 
	      exit(1);
	    }
		
	printf("\n Gerando o arquivo: %s  \n",infn);	
		
	for(l=Dec->prox;l!=NULL;l=l->prox)
    {
	    //exibeNo(l->prod);
		//printf("\n");
		
		if(l->cor==1)
			{
				//printf("\n cor 1 \n");
				//n=buscaNo(Out,var);
				//if(n==NULL)
				aux=copia(l->nome);
				insNo(Out,aux);
			}else if(l->cor==2)
			{
				//printf("\n cor 2 \n");
				aux=copia(l->nome);
				//n=buscaNo(Buffer,var);
				//if(n==NULL)
				insNo(Buffer,aux);
			}

		for(m=l->prod->prox;m!=NULL;m=m->prox)
		{
		  
		   strcpy(var,m->nome);
		   if(var[strlen(var)-1]==39)
			var[strlen(var)-1]='\0';
			
			if(var[0]=='!' || var[0]=='~')
			{
			  for(z=0;var[z]!='\0';z++)
			   var[z]=var[z+1];
			}
			
			if(m->cor==0&&m->nome[0]!='+'&&m->nome[0]!='|')
			{  
			   // printf("\n cor 0 \n");
				n=buscaNo(In,var);
				if(n==NULL)
				{
				   // printf("entrada\n");
				    aux=copia(var);
					insNo(In,aux);
				}
			}
			
			
			
        }	
    
    }
	
	fprintf(fpout,"----------------\n\n");
	fprintf(fpout,"-- Codigo Gerado Pela Ferramenta geraVHD --\n");
	fprintf(fpout,"-- Autor: Tiago da Silva Curtinhas (2013) --\n\n");
	fprintf(fpout,"library ieee;\n");
	fprintf(fpout,"use ieee.std_logic_1164.all;\n");

	
	fprintf(fpout,"ENTITY %s IS\n",na);
	fprintf(fpout,"PORT(\n");
	fprintf(fpout,"\t\t");
	for(l=In->prox;l!=NULL;l=l->prox)
	  {
	   fprintf(fpout,"%s",l->nome);
	   if(l->prox!=NULL)
	     fprintf(fpout,", ");
	  }
	  
	if(ini==1)  
	 fprintf(fpout,", rst");
	fprintf(fpout," : IN std_logic;\n");
	
	if(Out->prox!=NULL)
	{
	   fprintf(fpout,"\t\t");	
	   for(l=Out->prox;l!=NULL;l=l->prox)
	      {
			fprintf(fpout,"%s",l->nome);
			if(l->prox!=NULL)
	          fprintf(fpout,", ");
			 // literais++;
		  }
		  
		if(Buffer->prox==NULL)  
	   fprintf(fpout," : OUT std_logic);\n");
	   else
	    fprintf(fpout," : OUT std_logic;\n");
	   
	}
	
	if(Buffer->prox!=NULL)
	{
		fprintf(fpout,"\t\t");
	   for(l=Buffer->prox;l!=NULL;l=l->prox)
	      {
			fprintf(fpout,"%s",l->nome);
			if(l->prox!=NULL)
			  fprintf(fpout,", ");  
			 // literais++;
		  }
	   fprintf(fpout," : OUT std_logic);\n");
	   
	}
	
	fprintf(fpout,"END %s;\n\n",na);
	
	fprintf(fpout,"ARCHITECTURE bhv OF %s IS\n",na);
	
		if(Buffer->prox!=NULL)
		{
			fprintf(fpout,"\n SIGNAL ");
			for(l=Buffer->prox;l!=NULL;l=l->prox)
			{
				fprintf(fpout,"s_%s",l->nome);
				if(l->prox!=NULL)
				fprintf(fpout,", ");  
				// literais++;
			}
		    fprintf(fpout," : std_logic;\n\n");
		
		}
	
	
	fprintf(fpout,"BEGIN\n\n");
	
	
		if(Buffer->prox!=NULL)
		{
		   // fprintf(fpout,"\n");
			for(l=Buffer->prox;l!=NULL;l=l->prox)
			{
				fprintf(fpout," %s <= s_%s;\n",l->nome,l->nome);
				//if(l->prox!=NULL)
				//fprintf(fpout,", ");  
				// literais++;
			}
		    fprintf(fpout,"\n");
		    //fprintf(fpout," : std_logic);\n");
		
		}
		fprintf(fpout,"\n\n");
	
	   for(l=Dec->prox;l!=NULL;l=l->prox)
	   {
	     
		
		 test=0;
		 
		 for(m=l->prod->prox;m!=NULL;m=m->prox)// no caso de: s<=((a AND b) OR (c and d));
	         if(m->nome[0]=='+'||m->nome[0]=='|')
				test++;
				
		 if(test)
		 {	 
	       if(l->cor==2)
		   fprintf(fpout," s_%s <=((",l->nome);
	       else
		   fprintf(fpout," %s <=((",l->nome);
	       
		 } 
		 else  
		 {	 
	       if(l->cor==2)
           fprintf(fpout," s_%s <=(",l->nome);	
	       else
           fprintf(fpout," %s <=(",l->nome);	
         }		   
		 
		 enb=0;
	     for(m=l->prod->prox;m!=NULL;m=m->prox)
	      {  
			    if(m->nome[0]=='+'||m->nome[0]=='|')
				   {
						if(set)
						{
							//produtos++;
							if(ini == 1 &&vet_ini[v] == '0' )
							fprintf(fpout," AND rst");
							set=0;
						}
				     fprintf(fpout,") OR (");//m->nome
                     
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
						    for(z=0;m->nome[z]!='\0';z++)
							    m->nome[z]=m->nome[z+1];
						  }
						   if(m->cor==2)
						   fprintf(fpout,"NOT(s_%s)",m->nome);
					       else
						   fprintf(fpout,"NOT(%s)",m->nome);
					   
					}
					else
					{
						   if(m->cor==2)
 						   fprintf(fpout," s_%s",m->nome);
					       else
 						   fprintf(fpout," %s",m->nome);
					}
					if(m->prox!=NULL)
					{
					  if(m->prox->nome[0]!='+'&&m->prox->nome[0]!='|')
					  {
						fprintf(fpout," AND ");
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
				fprintf(fpout," AND rst");
				set=0;
			}
		   
		   if(test)
           {
			    if(ini == 1 &&vet_ini[v] == '1' )
					fprintf(fpout,") OR not(rst));\n\n");
				else
			   fprintf(fpout,"));\n\n");	
				
				
			   
		   }   
		   else
		   {
			   if(ini == 1 &&vet_ini[v] == '1' )
					fprintf(fpout,") OR not(rst);\n\n");
				else
					fprintf(fpout,");\n\n");
				
		   }	
				v++;
		   
		   
		  
	     
	     }
		 
		// literais++;
	     fprintf(fpout,"\n");
	
	fprintf(fpout,"END bhv;\n");
	
	printf(" Arquivo Gerado Com Sucesso!\n");
	printf(" Produtos: %d  \n Literais: %d \n",produtos,literais);
	
	fclose(fpout);
	
}

void toEqn(No* Dec, char *nome) // converte o padrão .prs do atacs para o .eqn  do 3D
{
 FILE *fpout;
 int test,literais=0,produtos=0,cont,x,y,z,enb,v=0,set=0;
 No *l,*l2,*m,*m2,*n,*n2;
 char infn[MAX],var[MAX],*aux,na[MAX];
 strcpy(infn, nome);
 stripFext(infn);
 strcpy(na,infn);
 strcat(infn, ".eqn");
 
   
 // nda
   if ((fpout = fopen(infn, "w")) == NULL)
        { 
	      printf("cannot open output file \n"); 
	      exit(1);
	    }
		
	printf("\n Gerando o arquivo: %s  \n",infn);
	
	for(l=Dec->prox;l!=NULL;l=l->prox)
    {
		printf("%s = \n",l->nome);
	    //exibeNo(l->prod);
		//printf("\n");
		fprintf(fpout,"%s = \n",l->nome);
		//scanf("&s",na);
		for(m=l->prod->prox;m!=NULL;m=m->prox)
		{
		   fprintf(fpout," ");
		   printf(" ");
		   strcpy(var,m->nome);
		    //if(var[strlen(var)-1]==39)
			//fprintf(fpout,"%s ",m->nome); //var[strlen(var)-1]='\0';
		   if(var[0]=='!' || var[0]=='~')
			{
			  for(z=0;var[z]!='\0';z++)
			   var[z]=var[z+1];
		       fprintf(fpout," %s'",var); 
		       printf(" %s'",var); 
			}
			else if(var[0]!='|' &&  var[0]!='+')
			{
				fprintf(fpout," %s",m->nome); //var[strlen(var)-1]='\0';	
				printf(" %s",m->nome); //var[strlen(var)-1]='\0';	
			}	
			
			if(m->nome[0]=='+' || m->nome[0]=='|')
			{  
			   // printf("\n cor 0 \n");
			   fprintf(fpout," +\n"); 
			   printf(" +\n"); 
				
			}
			//fprintf(fpout,"\n");
			
			if(m->prox==NULL && l->prox!=NULL)
			{  
			   // printf("\n cor 0 \n");
			   fprintf(fpout,"\n\n"); 
			   printf("\n\n"); 
				
			}
			
        }	
    
    }
	fprintf(fpout,"\n"); 
	printf("\n"); 
	
	
}