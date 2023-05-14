#define _GNU_SOURCE
#include<ctype.h>//用于测试和转换字符的函数
#include<stdarg.h>//处理可变数量的参数
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//
// tokenize.c 词法分析器
//

// Token
typedef enum
{
    TK_RESERVED, // Keywords or punctuators
    TK_IDENT,    // Identifiers
    TK_NUM,      // Integer literals
    TK_EOF,      // End-of-file markers
}TokenKind;

// Token type
typedef struct Token Token;
struct Token 
{
  TokenKind kind; // Token kind
  Token *next;    // Next token
  long val;       // If kind is TK_NUM, its value
  char *str;      // Token string
  int len;        // Token length
};


void error(char *fmt, ...);// Reports an error and exit.
void error_at(char *loc, char *fmt, ...);// Reports an error location and exit.
bool consume(char *op);// Consumes the current token if it matches `op`.
Token *consume_ident(void);
void expect(char *op); // Ensure that the current token is `op`.
long expect_number(void); // Ensure that the current token is TK_NUM.
bool at_eof(void); 
Token *tokenize(void);

// Input program
extern char *user_input;
//Global Current token
extern Token *token;



//
// parse.c 语法分析器
//

// Local variable
typedef struct Var Var;
struct Var
{
  Var *next;
  char *name;// Variable name
  int offset;// Offset from RBP
};


// AST node
typedef enum 
{
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_RETURN,    // "return"
  ND_EXPR_STMT, // Expression statement
  ND_VAR,       // Variable
  ND_NUM,       // Integer
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node 
{
  NodeKind kind; // Node kind
  Node *next;    // Next node
  Node *lhs;     // Left-hand side
  Node *rhs;     // Right-hand side
  Var *var;      // Used if kind == ND_VAR
  long val;      // Used if kind == ND_NUM
};

typedef struct Function Function;
struct Function
{
  Node *node;
  Var *locals;
  int stack_size;
};

Function *program(void);



//
// codegen.c 目标代码生成
//

void codegen(Function *prog);