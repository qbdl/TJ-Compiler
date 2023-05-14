#include"chibi.h"

int main(int argc,char **argv)
{
    if(argc!=2)
        error("%s: invalid number of arguments",argv[0]);

    // Tokenize and parse.
    user_input=argv[1];
    token=tokenize();
    //从根节点往下进行汇编代码生成
    Function *prog = program();//调用顶部（内部自动完成AST树的生成）

    // Assign offsets to local variables.(暂时是单函数)
    int offset = 0;
    for (Var *var = prog->locals; var; var = var->next) {
        offset += 8;
        var->offset = offset;
    }
    prog->stack_size = offset;
    
    // Traverse the AST to emit assembly.
    codegen(prog);

    return 0;
}