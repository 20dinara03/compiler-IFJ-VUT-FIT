#ifndef LOGGING
#define LOGGING

#include "memo.h"
#include <stdbool.h>

#define dec_memo_free(type) \
    void free_##type(type *target);

bool check_env_var(char *env_var);

#define declare_logging(component)                                      \
        typedef struct component##_logging {                            \
        bool active;                                                    \
        void (*fatal)(char *message, int exit_code);                    \
        void (*debug)(struct component##_logging *self, char *message); \
        void (*free)(struct component##_logging *self);                 \
    } component##_logging_t;                                            \
    component##_logging_t* init_##component##_logging();                \
    void free_##component##_logging(component##_logging_t *self);       \
    void component##_log_debug(component##_logging_t *self, char *message);  \

void log_fatal(char *message, int exit_code);

#define define_logging(component)                                               \
    component##_logging_t* init_##component##_logging() {                       \
        component##_logging_t *self = malloc(sizeof(component##_logging_t));    \
        self->fatal = log_fatal;                                                \
        self->debug = component##_log_debug;                                    \
        self->free = free_##component##_logging;                                \
        self->active = check_env_var(#component);                               \
        return self;                                                            \
    }                                                                           \
    void free_##component##_logging(component##_logging_t *self) {              \
        free(self);                                                             \
    }                                                                           \
    void component##_log_debug(component##_logging_t *self, char *message) {    \
        if (self->active) printf("%s", message);                                \
    }

#define get_logging(component) \
    init_##component##_logging();

#endif //LOGGING
