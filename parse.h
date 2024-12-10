#ifndef PARSE_H
#define PARSE_H

#include "token.h"
struct program {
    struct complete_command **complete_commands;
};

struct complete_command {
    struct list *list;
    struct token *separator_op;
};

struct list {
    struct list *list;
    struct token *separator_op;
    struct and_or *and_or;
};

struct and_or {
    struct and_or *and_or;
    struct token *op;
    struct pipeline *pipeline;
};

struct pipeline {
    struct token *bang;
    struct pipe_sequence *pipe_sequence;
};

struct pipe_sequence {
    struct pipe_sequence *pipe_sequence;
    struct token *pipe;
    struct command *command;
};

struct command {
    struct simple_command *simple_command;
    struct compound_command *compound_command;
    struct redirect_list *redirect_list;
    struct function_definition *function_definition;
};

struct compound_command {
    struct brace_group *brace_group;
    struct subshell *subshell;
    struct for_clause *for_clause;
    struct case_clause *case_clause;
    struct if_clause *if_clause;
    struct while_clause *while_clause;
    struct until_clause *until_clause;
};

struct subshell {
    struct compound_list *compound_list;
};

struct compound_list {
    struct term *term;
    struct separator *separator;
};

struct term {
    struct term *term;
    struct separator *separator;
    struct and_or *and_or;
};

struct for_clause {
    struct token *name;
    struct do_group *do_group;
    struct sequential_sep *sequential_sep;
    struct wordlist *wordlist;
};

struct wordlist {
    struct wordlist *wordlist;
    char *word;
};

struct case_clause {
    struct token *word;
    struct case_item *case_list;
};

struct case_item {
    struct pattern *pattern;
    struct compound_list *compound_list;
};

struct pattern {
    char **words;
};

struct if_clause {
    struct if_part *parts;
};

struct if_part {
    struct compound_list *condition;
    struct compound_list *body;
};

struct while_clause {
    struct compound_list *condition;
    struct compound_list *body;
};

struct until_clause {
    struct compound_list *condition;
    struct compound_list *body;
};

struct function_definition {
    struct token *name;
    struct function_body *body;
};

struct function_body {
    struct compound_command *compound_command;
};

struct brace_group {
    struct compound_list *compound_list;
};

struct simple_command {
    struct cmd_prefix *cmd_prefix;
    char *cmd;
    struct cmd_suffix *cmd_suffix;
};

struct cmd_prefix {
    struct io_redirect *io_redirect;
    struct cmd_prefix *cmd_prefix;
    char *ASSIGNMENT_WORD;
};

struct cmd_suffix {
    struct io_redirect *io_redirect;
    struct cmd_suffix *cmd_suffix;
    char *WORD;
};

struct redirect_list {
    struct io_redirect *io_redirect;
    struct redirect_list *redirect_list;
};

struct io_redirect {
    char *io_file;
    char *IO_NUMBER;
    char *io_here;
};

struct io_file {
    char *filename;
    char *LESSAND;
    char *GREATAND;
    char *DGREAT;
    char *LESSGREAT;
    char *CLOBBER;
};

struct filename {
    char *WORD;
};

struct io_here {
    char *DLESS;
    char *DLESSDASH;
};

struct here_end {
    char *WORD;
};

struct separator {
    struct token *separator_op;
    struct linebreak *linebreak;
};

struct sequential_sep {
    char *SEMICOLON;
    struct linebreak *linebreak;
};

struct program *parse_program(struct token_list *tokens);
struct complete_command *_parse_complete_command(struct token_list *tokens);
struct list *_parse_list(struct token_list *tokens);
struct and_or *_parse_and_or(struct token_list *tokens);
struct pipeline *_parse_pipeline(struct token_list *tokens);
struct pipe_sequence *_parse_pipe_sequence(struct token_list *tokens);
struct command *_parse_command(struct token_list *tokens);
struct compound_command *_parse_compound_command(struct token_list *tokens);
struct subshell *_parse_subshell(struct token_list *tokens);
struct compound_list *_parse_compound_list(struct token_list *tokens);
struct term *_parse_term(struct token_list *tokens);
struct for_clause *_parse_for_clause(struct token_list *tokens);
struct case_clause *_parse_case_clause(struct token_list *tokens);
struct case_item *_parse_case_item(struct token_list *tokens);
struct pattern *_parse_pattern(struct token_list *tokens);
struct if_clause *_parse_if_clause(struct token_list *tokens);
struct if_part *_parse_if_part(struct token_list *tokens);
struct while_clause *_parse_while_clause(struct token_list *tokens);
struct until_clause *_parse_until_clause(struct token_list *tokens);
struct function_definition *
_parse_function_definition(struct token_list *tokens);
struct function_body *_parse_function_body(struct token_list *tokens);
struct brace_group *_parse_brace_group(struct token_list *tokens);
struct simple_command *_parse_simple_command(struct token_list *tokens);
struct cmd_prefix *_parse_cmd_prefix(struct token_list *tokens);
struct cmd_suffix *_parse_cmd_suffix(struct token_list *tokens);
struct redirect_list *_parse_redirect_list(struct token_list *tokens);
struct io_redirect *_parse_io_redirect(struct token_list *tokens);
struct io_file *_parse_io_file(struct token_list *tokens);
struct filename *_parse_filename(struct token_list *tokens);
struct io_here *_parse_io_here(struct token_list *tokens);
struct here_end *_parse_here_end(struct token_list *tokens);
struct token *_parse_separator_op(struct token_list *tokens);
struct separator *_parse_separator(struct token_list *tokens);
struct sequential_sep *_parse_sequential_sep(struct token_list *tokens);
struct token *_parse_single(struct token_list *tokens, enum token_type type);
struct wordlist *_parse_wordlist(struct token_list *tokens);
struct do_group *_parse_do_group(struct token_list *tokens);
#endif
