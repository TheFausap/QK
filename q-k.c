//
//  main.c
//  q-k
//
//  Created by Fausto Saporito on 11/05/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define K0      0x1
#define K1      0x2
#define B0      0x3
#define B1      0x4
#define P0      0x5
#define P1      0x6
// matrices
#define I       0x10
#define X       0x11
#define Z       0x12
#define H       0x13
#define Y       0x14
#define S       0x15
#define SX      0x16
#define T       0x17
#define uCX     0x18
#define dCX     0x19
#define M       0x20
#define ECR     0x21
#define SWAP    0x22
#define PG      0x23
#define CCX     0x24
#define SQ2     0x25
#define ISQ2    0x26
// use parameter
#define Rx      0x30
#define Ry      0x31
#define Rz      0x32
#define Rn      0x33
#define CP      0x34
#define U       0x35
#define P       0x36
#define QFT     0x37
// functions
#define TS      0x40
#define SP      0x41
#define NZP     0x42
#define NZC     0x43
#define SZ      0x44
#define BASIS   0x45
#define KET     0x46
#define BRA     0x47
#define PPK     0x48
#define PPB     0x49
#define QPP     0x50
#define PST     0x51
#define MMU     0x52
#define APPLY   0x53
#define MKR     0x54
#define INV     0x55
#define MPOWI   0x56
#define ID      0x57
#define P2      0x58
#define PN      0x59
#define MKRN    0x60
#define OM      0x61
#define CNTRL   0x62
#define PROB    0x63
#define PPROB   0x64
#define SHF     0x65
#define CLK     0x66

#define R       return
#define SKMAX   4096
#define GNUMB   4       // 10^4 - 1

#define MA(x,y) y=calloc(x,sizeof(*#y))
#define GVAL    push(('G'<<16)|(gg-1))
#define O       printf

#define POPD    pop()*1.0

#define MAT2    "(%f %f; %f %f)"
#define MAT3    "(%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f;\
%f %f %f %f %f %f %f %f)"

#define SMAT2   "(%s %s; %s %s)"
#define SMAT3   "(%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s;\
%s %s %s %s %s %s %s %s)"

#define EMIT(x, y...)   printf("%s:"x"\n",y)

FILE* in;

unsigned* sk;
long skstrt;
long skend;
int gg = 0;
int sp = SKMAX-1;

unsigned ipow(unsigned base, unsigned exp)
{
    unsigned result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    return result;
}

void push(unsigned x){sk[sp--]=x;}

unsigned pop(void) {R sk[++sp];}

#define CS(x) case x:R #x;break;
char* pops(void)
{
    char* r;
    char* n;
    int v;
    MA(6,r);
    MA(5,n);
    
    switch(v=pop())
    {
        CS(K0)
        CS(K1)
        CS(B0)
        CS(B1)
        CS(P0)
        CS(P1)
        CS(I)
        CS(X)
        CS(Z)
        CS(H)
        CS(Y)
        CS(S)
        CS(T)
        CS(SQ2)
        CS(ISQ2)
        CS(uCX)
        CS(dCX)
        CS(M)
        CS(ECR)
        CS(SWAP)
        CS(PG)
        CS(CCX)
        CS(SX)
        default:
            r[0]=v>>16;
            sprintf(n,"%d",v&0x3fff);
            R strdup(strcat(r,n));
            break;
    }
    R NULL;
}
#undef CS

char* genv(void)
{
    char n[GNUMB+1];
    char r[GNUMB+2];
    strcpy(r,"G");
    if((gg+1)>ipow(10,GNUMB))
        printf("internal error: too many G\n");
    sprintf(n,"%d",gg++);
    
    R strdup(strcat(r,n));
}

#define IF(x,...)   if(!strcmp(b,x)){ __VA_ARGS__; }else
#define IFS(x,...)  if(strstr(b,x)){ __VA_ARGS__; }else
#define IFC(x,...)  if(b[0]==x){ __VA_ARGS__; }else
#define END {}

int main(int n, const char * a[]) {
    char* buf;
    char* bufp;
    char* b;
    char* c;
    char* d;
    
    MA(80,buf);
    MA(80,bufp);
    MA(80,b);
    MA(80,c);
    MA(80,d);
    sk = calloc(SKMAX,sizeof(unsigned));
    unsigned s,p,q,r;
    
    if(n>1) in=fopen(a[1],"r");
    else in=fopen("test.in","r");
    
    while(fgets(buf,80,in))
    {
        bufp=strdup(buf);
        s=0;p=0;q=0;r=0;
        while((b=strsep(&bufp,";")))
        {
            IF("\n")
            IF("H",push(H))
            IF("I",push(I))
            IF("S",push(S))
            IF("Z",push(Z))
            IF("Y",push(Y))
            IF("K0",push(K0))
            IF("K1",push(K1))
            IF("uCX",push(uCX))
            IF("dCX",push(dCX))
            IF("SWAP",push(SWAP))
            IF("SQ2",push(SQ2))
            IF("ISQ2",push(ISQ2))
            IF("P0",push(P0))
            IF("P1",push(P1))
            IF("PI",O("%s:PI",genv()))
            IF("B0",push(B0))
            IF("B1",push(B1))
            IF("M",push(M))
            IF("SX",push(SX))
            IF("T",push(T))
            IF("CCX",push(CCX))
            IF("QFT",O("%s:QFT %d\n",genv(),pop());GVAL)
            IF("Rx",O("%s:Rx %d\n",genv(),pop());GVAL)
            IF("Ry",O("%s:Ry %d\n",genv(),pop());GVAL)
            IF("Rz",O("%s:Rz %d\n",genv(),pop());GVAL)
            IF("Rn",O("%s:Rn %d\n",genv(),pop());GVAL)
            IF("U",O("%s:U[%d;%d;%d]\n",genv(),pop(),pop(),pop());GVAL)
            IF("CP",O("%s:CP %d\n",genv(),pop());GVAL)
            IF("P",O("%s:P %d\n",genv(),pop());GVAL)
            IF("CTRL",O("%s:cntrl[%d;%d;%d;%d]\n",genv(),pop(),pop(),pop(),pop());GVAL)
            IF("*K",O("%s:mkr[%s;%s]\n",genv(),pops(),pops());GVAL)
            IF("*V",O("%s:ts[%s;%s]\n",genv(),pops(),pops());GVAL)
            IF("APPLY",O("%s:apply[%s;%s]\n",genv(),pops(),pops());GVAL)
            IF("DUP",p = pop();push(p);push(p))
            IF("REV",p = pop();q = pop();push(p);push(q))
            IF("PERM",p = pop();q = pop();r=pop();push(p);push(r);push(q))
            IF("CAB",p = pop();q = pop();r = pop();push(q);push(p);push(r))
            IFS("SET",b+=3;push(atoi(b)))
            IF("DB",/* Dagwood Bumstead */EMIT("zround ((cos vDB*pi)*mkr[I;I])-(1a90)*(sin vDB*pi)*(mkr[X;X]+mkr[Y;Y])",genv());GVAL)
            IF("PROB",O("%s:nprob %s\n",genv(),pops());GVAL)
            IF("OP",O("%s:op[%s;%s]\n",genv(),pops(),pops());GVAL)
            IF("Q*",O("%s:%s*%s\n",genv(),pops(),pops());GVAL)
            IF("Q+",O("%s:%s+%s\n",genv(),pops(),pops());GVAL)
            IFS("QMAT",b+=4;p = atoi(b);
                switch(p)
                {
                    case 2:
                        EMIT(SMAT2,genv(),pops(),pops(),pops(),pops());
                        GVAL;
                        break;
                    case 3:
                        EMIT(SMAT3,genv(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops(),
                             pops(),pops(),pops(),pops(),pops(),pops(),pops(),pops());
                        GVAL;
                        break;
                    default:
                        break;
                }
            )
            IFS("MAT",b+=3;p = atoi(b);
                switch(p)
                {
                    case 2:
                        EMIT(MAT2,genv(),POPD,POPD,POPD,POPD);
                        GVAL;
                        break;
                    case 3:
                        EMIT(MAT3,genv(),POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD,
                             POPD,POPD,POPD,POPD,POPD,POPD,POPD,POPD);
                        GVAL;
                        break;
                    default:
                        break;
                }
            )
            IFC('=',b++;s=pop();bzero(c,80);c[0]=s>>16;sprintf(d,"%d",s&0x3fff);strcat(c,d);O("%s:%s\n",b,c);push(s))
            IFC('V',b++;push(('V'<<16)|atoi(b)))
            IFC('%',O("pprob %s\n",pops()))
            IFC('*',push(pop()*pop()))
            IFC('+',push(pop()+pop()))
            IFC('-',push(pop()-pop()))
            IFC('/',push(pop()/pop()))
            IFC('[',b++;
                if(isdigit(b[0]))
                {
                    while((c=strsep(&b,"/"))) push(atoi(c));
                }
                else
                {
                    // commutator
                    c = pops(); d = pops();
                    EMIT("mmu[%s;%s]-mmu[%s;%s]",genv(),d,c,c,d);
                    GVAL;
                }
            )
            IFC('{',b++;
                if(isdigit(b[0]))
                {
                    srandom((unsigned)time(NULL));
                    for(;s<atoi(b);s++)
                        push((unsigned)random()%1317);
                }
                else
                {
                    // anti-commutator
                    c = pops(); d = pops();
                    EMIT("mmu[%s;%s]+mmu[%s;%s]",genv(),d,c,c,d);
                    GVAL;
                }
            )
            END
        }
    }
    
    fclose(in);
    
    R 0;
}
