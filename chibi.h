#define _GNU_SOURCE
#include <assert.h>
#include<ctype.h>//用于测试和转换字符的函数
#include <errno.h>
#include<stdarg.h>//处理可变数量的参数
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Type Type;
typedef struct Member Member;

//
// tokenize.c 词法分析器
//

// Token
typedef enum
{
    TK_RESERVED, // Keywords or punctuators
    TK_IDENT,    // Identifiers
    TK_STR,      // String literals
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

  char *contents; // String literal contents including terminating '\0'
  char cont_len;  // string literal length
};


void error(char *fmt, ...);// Reports an error and exit.
void error_at(char *loc, char *fmt, ...);// Reports an error location and exit.
void error_tok(Token *tok, char *fmt, ...);
Token *peek(char *s);
Token *consume(char *op);
Token *consume_ident(void);
void expect(char *op); // Ensure that the current token is `op`.
long expect_number(void); // Ensure that the current token is TK_NUM.
char *expect_ident(void);
bool at_eof(void); 
Token *tokenize(void);

//File name
extern char *filename;
// Input program
extern char *user_input;
//Global Current token
extern Token *token;



//
// parse.c 语法分析器
//

// Variable
typedef struct Var Var;
struct Var
{
  char *name;     // Variable name
  Type *ty;       // Type
  bool is_local;  //local or global

  //local variable
  int offset;     // Offset from RBP

  // Global variable
  char *contents;
  int cont_len;
};

typedef struct VarList VarList;
struct VarList 
{
  VarList *next;
  Var *var;
};

// AST node
typedef enum 
{
  ND_ADD,       // num + num
  ND_PTR_ADD,   // ptr + num or num + ptr
  ND_SUB,       // num - num
  ND_PTR_SUB,   // ptr - num
  ND_PTR_DIFF,  // ptr - ptr
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_MEMBER,    // . (struct member access)
  ND_ADDR,      // unary &
  ND_DEREF,     // unary *
  ND_RETURN,    // "return"
  ND_IF,        // "if"
  ND_WHILE,     // "while"
  ND_FOR,       // "for"
  ND_BLOCK,     // { ... }
  ND_FUNCALL,   // Function call
  ND_EXPR_STMT, // Expression statement
  ND_STMT_EXPR, // Statement expression
  ND_VAR,       // Variable
  ND_NUM,       // Integer
  ND_NULL,      // Empty statement
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node 
{
  NodeKind kind; // Node kind
  Node *next;    // Next node
  Type *ty;      // Type, e.g. int or pointer to int
  Token *tok;    // Representative token

  Node *lhs;     // Left-hand side
  Node *rhs;     // Right-hand side
  
  // "if, "while" or "for" statement
  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  // Block or statement expression
  Node *body;

  // Struct member access
  Member *member;

  // Function call
  char *funcname;
  Node *args;

  Var *var;      // Used if kind == ND_VAR
  long val;      // Used if kind == ND_NUM
};

typedef struct Function Function;
struct Function
{
  Function *next;
  char *name;
  VarList *params;

  Node *node;
  VarList *locals;
  int stack_size;
};

typedef struct{
  VarList *globals;
  Function *fns;
}Program;

Program *program(void);

//
// typing.c
//

typedef enum
{ 
  TY_CHAR,
  TY_INT,
  TY_PTR,
  TY_ARRAY,
  TY_STRUCT,
} TypeKind;

struct Type 
{
  TypeKind kind;
  int size;        // sizeof() value
  Type *base;      // pointer or array
  int array_len;   // array
  Member *members; // struct
};

// Struct member
struct Member 
{
  Member *next;
  Type *ty;
  char *name;
  int offset;
};

extern Type *char_type;
extern Type *int_type;

bool is_integer(Type *ty);
Type *pointer_to(Type *base);
Type *array_of(Type *base, int size);
void add_type(Node *node);


//
// codegen.c 目标代码生成
//

void codegen(Program *prog);