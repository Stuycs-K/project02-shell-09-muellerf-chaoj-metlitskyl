#include "parse.h"
#include "token.h"
#include <stdlib.h>

struct program *parse_program(struct token_list *tokens) {
    struct program *program = malloc(sizeof(struct program));
    int cap = 10;
    int size = 0;
    struct complete_command **commands =
        malloc(sizeof(struct complete_command *) * cap);
    while (tokens->size > 0) {
        if (tokens->tokens[0].type == NEWLINE) {
            next(tokens);
        } else {
            if (size == cap) {
                cap *= 2;
                commands =
                    realloc(commands, sizeof(struct complete_command *) * cap);
            }
            struct complete_command *command = _parse_complete_command(tokens);
            if (command == NULL) {
                return NULL;
            }
            commands[size++] = command;
        }
        if (tokens->size > 0 && tokens->tokens[0].type != NEWLINE) {
            return NULL;
        }
    }
    if (size == cap) {
        cap++;
        commands = realloc(commands, sizeof(struct complete_command *) * cap);
    }
    commands[size] = NULL;
    program->complete_commands = commands;
    return program;
}

struct complete_command *_parse_complete_command(struct token_list *tokens) {
    struct complete_command *complete_command =
        malloc(sizeof(struct complete_command));
    int start = tokens->size;
    complete_command->list = _parse_list(tokens);
    if (complete_command->list == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    complete_command->separator_op = _parse_separator_op(tokens);
    return complete_command;
}

struct list *_parse_list(struct token_list *tokens) {
    struct list *list = malloc(sizeof(struct list));
    int start = tokens->size;
    list->list = _parse_list(tokens);
    list->separator_op = NULL;
    if (list->list != NULL) {
        list->separator_op = _parse_separator_op(tokens);
        if (list->separator_op == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
    }
    list->and_or = _parse_and_or(tokens);
    if (list->and_or == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return list;
}

struct and_or *_parse_and_or(struct token_list *tokens) {
    struct and_or *and_or = malloc(sizeof(struct and_or));
    int start = tokens->size;
    struct token *token;
    and_or->and_or = _parse_and_or(tokens);
    and_or->op = NULL;
    if (and_or->and_or == NULL) {
        reset_back(tokens, start);
        and_or->pipeline = _parse_pipeline(tokens);
        if (and_or->pipeline == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
        return and_or;
    }
    if (tokens->size == 0) {
        return NULL;
    }
    if ((token = _parse_single(tokens, AND_IF)) == NULL &&
        (token = _parse_single(tokens, OR_IF)) == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    and_or->op = token;
    if (_parse_single(tokens, NEWLINE) == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    and_or->pipeline = _parse_pipeline(tokens);
    if (and_or->pipeline == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return and_or;
}

struct pipeline *_parse_pipeline(struct token_list *tokens) {
    struct pipeline *pipeline = malloc(sizeof(struct pipeline));
    int start = tokens->size;
    pipeline->bang = _parse_single(tokens, BANG);
    pipeline->pipe_sequence = _parse_pipe_sequence(tokens);
    if (pipeline->pipe_sequence == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return pipeline;
}

struct pipe_sequence *_parse_pipe_sequence(struct token_list *tokens) {
    struct pipe_sequence *pipe_sequence = malloc(sizeof(struct pipe_sequence));
    int start = tokens->size;
    pipe_sequence->pipe_sequence = _parse_pipe_sequence(tokens);
    pipe_sequence->pipe = NULL;
    if (pipe_sequence->pipe_sequence != NULL) {
        if (_parse_single(tokens, PIPE) == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
        if (_parse_single(tokens, NEWLINE) == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
    } else {
        reset_back(tokens, start);
    }
    pipe_sequence->command = _parse_command(tokens);
    if (pipe_sequence->command == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return pipe_sequence;
}

struct command *_parse_command(struct token_list *tokens) {
    struct command *command = calloc(1, sizeof(struct command));
    int start = tokens->size;
    command->simple_command = _parse_simple_command(tokens);
    if (command->simple_command != NULL) {
        return command;
    }
    reset_back(tokens, start);
    command->compound_command = _parse_compound_command(tokens);
    if (command->compound_command != NULL) {
        command->redirect_list = _parse_redirect_list(tokens);
        return command;
    }
    reset_back(tokens, start);
    command->function_definition = _parse_function_definition(tokens);
    if (command->function_definition != NULL) {
        return command;
    }
    return NULL;
}

struct compound_command *_parse_compound_command(struct token_list *tokens) {
    struct compound_command *compound_command =
        calloc(1, sizeof(struct compound_command));
    int start = tokens->size;
    compound_command->brace_group = _parse_brace_group(tokens);
    if (compound_command->brace_group != NULL) {
        return compound_command;
    }
    reset_back(tokens, start);
    compound_command->subshell = _parse_subshell(tokens);
    if (compound_command->subshell != NULL) {
        return compound_command;
    }
    reset_back(tokens, start);
    compound_command->for_clause = _parse_for_clause(tokens);
    if (compound_command->for_clause != NULL) {
        return compound_command;
    }
    reset_back(tokens, start);
    compound_command->case_clause = _parse_case_clause(tokens);
    if (compound_command->case_clause != NULL) {
        return compound_command;
    }
    reset_back(tokens, start);
    compound_command->if_clause = _parse_if_clause(tokens);
    if (compound_command->if_clause != NULL) {
        return compound_command;
    }
    reset_back(tokens, start);
    compound_command->while_clause = _parse_while_clause(tokens);
    if (compound_command->while_clause != NULL) {
        return compound_command;
    }
    reset_back(tokens, start);
    compound_command->until_clause = _parse_until_clause(tokens);
    if (compound_command->until_clause != NULL) {
        return compound_command;
    }
    return NULL;
}

struct subshell *_parse_subshell(struct token_list *tokens) {
    struct subshell *subshell = malloc(sizeof(struct subshell));
    int start = tokens->size;
    if (_parse_single(tokens, LPAREN) == NULL) {
        return NULL;
    }
    subshell->compound_list = _parse_compound_list(tokens);
    if (subshell->compound_list == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    if (_parse_single(tokens, RPAREN) == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return subshell;
}

struct compound_list *_parse_compound_list(struct token_list *tokens) {
    struct compound_list *compound_list = malloc(sizeof(struct compound_list));
    int start = tokens->size;
    if (_parse_single(tokens, NEWLINE) == NULL) {
        return NULL;
    }
    compound_list->term = _parse_term(tokens);
    if (compound_list->term == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    compound_list->separator = _parse_separator(tokens);
    return compound_list;
}

struct term *_parse_term(struct token_list *tokens) {
    struct term *term = malloc(sizeof(struct term));
    int start = tokens->size;
    term->term = _parse_term(tokens);
    term->separator = NULL;
    if (term->term != NULL) {
        term->separator = _parse_separator(tokens);
        if (term->separator == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
    }
    term->and_or = _parse_and_or(tokens);
    if (term->and_or == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return term;
}

struct for_clause *_parse_for_clause(struct token_list *tokens) {
    struct for_clause *for_clause = malloc(sizeof(struct for_clause));
    int start = tokens->size;
    if (_parse_single(tokens, FOR) == NULL) {
        return NULL;
    }
    for_clause->name = _parse_single(tokens, NAME);
    if (for_clause->name == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    if (_parse_single(tokens, NEWLINE)) {
        if (_parse_single(tokens, IN) == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
        for_clause->wordlist = _parse_wordlist(tokens);
        for_clause->sequential_sep = _parse_sequential_sep(tokens);
        if (for_clause->sequential_sep == NULL) {
            reset_back(tokens, start);
            return NULL;
        }
    } else {
        for_clause->sequential_sep = _parse_sequential_sep(tokens);
    }
    for_clause->do_group = _parse_do_group(tokens);
    if (for_clause->do_group == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return for_clause;
}

struct wordlist *_parse_wordlist(struct token_list *tokens) {
    struct wordlist *wordlist = malloc(sizeof(struct wordlist));
    int start = tokens->size;
    struct token *token = _parse_single(tokens, WORD);
    if (token == NULL) {
        return NULL;
    }
    wordlist->word = token->value;
    wordlist->wordlist = _parse_wordlist(tokens);
    if (wordlist->wordlist == NULL) {
        reset_back(tokens, start);
        return NULL;
    }
    return wordlist;
}

struct token *_parse_single(struct token_list *tokens, enum token_type type) {
    if (tokens->size == 0) {
        return NULL;
    }
    struct token *token = next(tokens);
    if (token->type != type) {
        reset_back(tokens, tokens->size - 1);
        return NULL;
    }
    return token;
}
