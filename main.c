#include<ctype.h>//用于测试和转换字符的函数
#include<stdarg.h>//处理可变数量的参数
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum
{
    TK_RESERVED, // Keywords or punctuators
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
};

//Global Current token
Token *token;

// Reports an error and exit.
void error(char *fmt, ...)//可以接受一个格式字符串fmt和随后的可变参数列表(...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Consumes the current token if it matches `op`.
bool consume(char op)//检查当前token是否是一个预期的字符（如加号或减号）
{
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// Ensure that the current token is `op`.
void expect(char op) 
{
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("expected '%c'", op);
  token = token->next;
}

// Ensure that the current token is TK_NUM.
long expect_number(void) 
{
  if (token->kind != TK_NUM)
    error("expected a number");
  long val = token->val;
  token = token->next;
  return val;
}

bool at_eof(void) 
{
  return token->kind == TK_EOF;
}

// Create a new token and add it as the next token of `cur`.
Token *new_token(TokenKind kind, Token *cur, char *str) 
{
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// Tokenize `p` and returns new tokens.
Token *tokenize(char *p) 
{
  Token head = {};
  Token *cur = &head;

  while (*p) {
    // Skip whitespace characters.
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Punctuator
    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    // Integer literal
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("invalid token");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}


int main(int argc,char **argv)
{
    if(argc!=2)
        error("%s: invalid number of arguments",argv[0]);

    // char *p=argv[1];//传入的参数（sh里的input)
    token=tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf("  .globl main\n");
    printf("main:\n");

    // The first token must be a number
    printf("  mov rax,%ld\n",expect_number());
    
    //subsequent "+ number" or "- number"
    while(!at_eof()){
        if(consume('+')){
            printf("  add rax, %ld\n",expect_number());
            continue;
        }
        expect('-');
        printf("  sub rax, %ld\n",expect_number());
    }

    printf("  ret\n");
    return 0;
}