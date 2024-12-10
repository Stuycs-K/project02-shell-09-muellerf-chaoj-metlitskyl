#ifndef TOKEN_H
#define TOKEN_H

struct token_list {
    struct token *tokens;
    int size;
    int cap;
};

struct token {
    int type;
    char *value;
};

enum token_type {
    WORD,
    ASSIGNMENT_WORD,
    NAME,
    NEWLINE,
    IO_NUMBER,
    AND_IF,
    OR_IF,
    DSEMI,
    DLESS,
    DGREAT,
    LESSAND,
    GREATAND,
    LESSGREAT,
    DLESSDASH,
    CLOBBER,
    IF,
    THEN,
    ELSE,
    ELIF,
    FI,
    DO,
    DONE,
    CASE,
    ESAC,
    WHILE,
    UNTIL,
    FOR,
    LBRACE,
    RBRACE,
    BANG,
    IN,
    PIPE,
    LPAREN,
    RPAREN,
    LTHAN,
    GTHAN,
    AMP,
    SEMI,
    DOLLAR,
    SUBSHELL,
};

struct token_list *tokenize(char *prog);
void add_token(struct token_list *tokens, char *value, int type);
struct token *next(struct token_list *tokens);
void print_token(struct token *token);
void reset_back(struct token_list *tokens, int orig_size);
#endif
