#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>

struct token_list *tokenize(char *prog) {
    struct token_list *tokens = malloc(sizeof(struct token_list));
    tokens->tokens = malloc(10 * sizeof(struct token));
    tokens->size = 0;
    tokens->cap = 10;

    char c;
    while ((c = prog++[0])) {
        switch (c) {
            int num;
            int num_brace;
            char *start;
        case '\n':
            add_token(tokens, "\n", NEWLINE);
            break;
        case ' ':
            continue;
        case '&':
            if (prog[0] == '&') {
                add_token(tokens, NULL, AND_IF);
                prog++;
            } else {
                add_token(tokens, NULL, AMP);
            }
            break;
        case '|':
            if (prog[0] == '|') {
                add_token(tokens, NULL, OR_IF);
                prog++;
            } else {
                add_token(tokens, NULL, PIPE);
            }
            break;
        case ';':
            if (prog[0] == ';') {
                add_token(tokens, NULL, DSEMI);
                prog++;
            } else {
                add_token(tokens, NULL, SEMI);
            }
        case '<':
            if (prog[0] == '<') {
                if (prog[1] == '-') {
                    add_token(tokens, NULL, DLESSDASH);
                    prog += 2;
                } else {
                    add_token(tokens, NULL, DLESS);
                    prog++;
                }
            } else if (prog[0] == '>') {
                add_token(tokens, NULL, LESSGREAT);
                prog++;
            } else if (prog[0] == '&') {
                add_token(tokens, NULL, LESSAND);
                prog++;
            } else {
                add_token(tokens, "<", LTHAN);
            }
            break;
        case '>':
            if (prog[0] == '>') {
                add_token(tokens, NULL, DGREAT);
                prog++;
            } else if (prog[0] == '&') {
                add_token(tokens, NULL, GREATAND);
                prog++;
            } else if (prog[0] == '|') {
                add_token(tokens, NULL, CLOBBER);
                prog++;
            } else {
                add_token(tokens, ">", GTHAN);
            }
            break;
        case '{':
            add_token(tokens, NULL, LBRACE);
            break;
        case '}':
            add_token(tokens, NULL, RBRACE);
            break;
        case '!':
            add_token(tokens, NULL, BANG);
            break;
        case '(':
            add_token(tokens, NULL, LPAREN);
            break;
        case ')':
            add_token(tokens, NULL, RPAREN);
            break;
        case '0' ... '9':
            num = c - '0';
            while (prog[0] >= '0' && prog[0] <= '9') {
                num = num * 10 + (prog[0] - '0');
                prog++;
            }
            add_token(tokens, (char *)num, IO_NUMBER);
            break;
        case '#':
            while (prog[0] && prog[0] != '\n') {
                prog++;
            }
            break;
        case '$':
            if (prog[0] == '(') {
                num_brace = 1;
                start = ++prog;
                while (num_brace > 0 && prog[0]) {
                    if (prog[0] == '(') {
                        num_brace++;
                    } else if (prog[0] == ')') {
                        num_brace--;
                    }
                    prog++;
                }
                prog[-1] = '\0';
                add_token(tokens, start, SUBSHELL);
                break;
            }
        default:
            prog--;
            start = prog;
            num_brace = 0;
            while (prog[0] &&
                   ((prog[0] != ' ' && prog[0] != '\n') || num_brace > 0)) {
                if (prog[0] == '$' && prog[1] == '(') {
                    break;
                }
                if (prog[0] == '"') {
                    while (prog[0] != '"') {
                        if (prog[0] == '\\') {
                            prog++;
                        }
                        prog++;
                    }
                } else if (prog[0] == '{') {
                    num_brace++;
                } else if (prog[0] == '}') {
                    num_brace--;
                }
                prog++;
            }
            prog[0] = '\0';
            prog++;

            if (strcmp(start, "if") == 0) {
                add_token(tokens, NULL, IF);
            } else if (strcmp(start, "then") == 0) {
                add_token(tokens, NULL, THEN);
            } else if (strcmp(start, "else") == 0) {
                add_token(tokens, NULL, ELSE);
            } else if (strcmp(start, "elif") == 0) {
                add_token(tokens, NULL, ELIF);
            } else if (strcmp(start, "fi") == 0) {
                add_token(tokens, NULL, FI);
            } else if (strcmp(start, "do") == 0) {
                add_token(tokens, NULL, DO);
            } else if (strcmp(start, "done") == 0) {
                add_token(tokens, NULL, DONE);
            } else if (strcmp(start, "case") == 0) {
                add_token(tokens, NULL, CASE);
            } else if (strcmp(start, "esac") == 0) {
                add_token(tokens, NULL, ESAC);
            } else if (strcmp(start, "while") == 0) {
                add_token(tokens, NULL, WHILE);
            } else if (strcmp(start, "until") == 0) {
                add_token(tokens, NULL, UNTIL);
            } else if (strcmp(start, "for") == 0) {
                add_token(tokens, NULL, FOR);
            } else if (strcmp(start, "in") == 0) {
                add_token(tokens, NULL, IN);
            } else {
                add_token(tokens, start, WORD);
            }

            wordexp_t p;
            char **w;

            if (wordexp(start, &p, 0)) {
                fprintf(stderr, "wordexp failed\n");
            } else {
                w = p.we_wordv;
                start = malloc(strlen(w[0]) + 1);
                strcpy(start, w[0]);
                add_token(tokens, start, WORD);
                wordfree(&p);
            }
        }
    }

    return tokens;
}

void add_token(struct token_list *tokens, char *value, int type) {
    if (tokens->size == tokens->cap) {
        tokens->cap *= 2;
        tokens->tokens =
            realloc(tokens->tokens, tokens->cap * sizeof(struct token));
    }

    struct token new_token;
    new_token.value = value;
    new_token.type = type;

    tokens->tokens[tokens->size++] = new_token;
}

struct token *next(struct token_list *tokens) {
    if (tokens->size == 0) {
        return NULL;
    }

    return &tokens->tokens-- [--tokens->size];
}

void print_token(struct token *token) {
    switch (token->type) {
    case WORD:
        printf("WORD: %s\n", token->value);
        break;
    case ASSIGNMENT_WORD:
        printf("ASSIGNMENT_WORD: %s\n", token->value);
        break;
    case NAME:
        printf("NAME: %s\n", token->value);
        break;
    case NEWLINE:
        printf("NEWLINE\n");
        break;
    case IO_NUMBER:
        printf("IO_NUMBER: %s\n", token->value);
        break;
    case AND_IF:
        printf("AND_IF\n");
        break;
    case OR_IF:
        printf("OR_IF\n");
        break;
    case DSEMI:
        printf("DSEMI\n");
        break;
    case DLESS:
        printf("DLESS\n");
        break;
    case DGREAT:
        printf("DGREAT\n");
        break;
    case LESSAND:
        printf("LESSAND\n");
        break;
    case GREATAND:
        printf("GREATAND\n");
        break;
    case LESSGREAT:
        printf("LESSGREAT\n");
        break;
    case DLESSDASH:
        printf("DLESSDASH\n");
        break;
    case CLOBBER:
        printf("CLOBBER\n");
        break;
    case IF:
        printf("IF\n");
        break;
    case THEN:
        printf("THEN\n");
        break;
    case ELSE:
        printf("ELSE\n");
        break;
    case ELIF:
        printf("ELIF\n");
        break;
    case FI:
        printf("FI\n");
        break;
    case DO:
        printf("DO\n");
        break;
    case DONE:
        printf("DONE\n");
        break;
    case CASE:
        printf("CASE\n");
        break;
    case ESAC:
        printf("ESAC\n");
        break;
    case WHILE:
        printf("WHILE\n");
        break;
    case UNTIL:
        printf("UNTIL\n");
        break;
    case FOR:
        printf("FOR\n");
        break;
    case LBRACE:
        printf("LBRACE\n");
        break;
    case RBRACE:
        printf("RBRACE\n");
        break;
    case BANG:
        printf("BANG\n");
        break;
    case IN:
        printf("IN\n");
        break;
    case PIPE:
        printf("PIPE\n");
        break;
    case LPAREN:
        printf("LPAREN\n");
        break;
    case RPAREN:
        printf("RPAREN\n");
        break;
    case LTHAN:
        printf("LTHAN\n");
        break;
    case GTHAN:
        printf("GTHAN\n");
        break;
    case AMP:
        printf("AMP\n");
        break;
    case SEMI:
        printf("SEMI\n");
        break;
    case DOLLAR:
        printf("DOLLAR\n");
        break;
    case SUBSHELL:
        printf("SUBSHELL: %s\n", token->value);
        break;
    }
}

void reset_back(struct token_list *tokens, int orig_size) {
    tokens->tokens += tokens->size - orig_size;
    tokens->size = orig_size;
}
