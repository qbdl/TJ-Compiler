#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv){
    if(argc!=2){
        fprintf(stderr,"%s:invalid number of arguments\n",argv[0]);
        return 1;
    }

    char *p=argv[1];//传入的参数（sh里的input)

    printf(".intel_syntax noprefix\n");
    printf("  .globl main\n");
    printf("main:\n");
    printf("  mov rax,%ld\n",strtol(p,&p,10));//strtol将当前指向字符转为某个基数的整数（并移动指针），直至遇到非数值/其他非法字符
    
    while(*p){
        if(*p=='+'){
            p++;//指针手动下移
            printf("  add rax,%ld\n",strtol(p,&p,10));
            continue;
        }
        else if(*p=='-'){
            p++;
            printf("  sub rax,%ld\n",strtol(p,&p,10));
            continue;
        }

        fprintf(stderr, "unexpected character: '%c'\n", *p);
        return 1;
    }
    
    
    printf("  ret\n");
    return 0;
}