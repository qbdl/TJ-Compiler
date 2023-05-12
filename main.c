#include"chibi.h"

int main(int argc,char **argv)
{
    if(argc!=2)
        error("%s: invalid number of arguments",argv[0]);

    // Tokenize and parse.
    user_input=argv[1];
    token=tokenize();
    //从根节点往下进行汇编代码生成
    Node *node=expr();
    // Traverse the AST to emit assembly.
    codegen(node);

    return 0;
}