#ifndef DEBUGGING_H_
#define DEBUGGING_H_

#include <stdlib.h>
#include "logging.h"

#if NODEBUG
    #define ASSERT(_component, _expr, ...) ((void)(_expr))
#else
    // runtime checking
    #define ASSERT(_component, _expr, ...) do {                                        \
        if (!(_expr)) {                                                                \
            printf("\n");                                                              \
            FATAL_LOG(                                                                 \
                _component,                                                            \
                "\n" GREEN_COLOR_TEXT BOLD_TEXT "%s" RESET_TEXT DEFAULT_COLOR_TEXT ":" \
                GREEN_COLOR_TEXT BOLD_TEXT "%d" RESET_TEXT DEFAULT_COLOR_TEXT ": "     \
                GREEN_COLOR_TEXT BOLD_TEXT "%s" RESET_TEXT DEFAULT_COLOR_TEXT ": "     \
                "\x1b[91m" BOLD_TEXT "[Assertion `%s` failed.] %s" RESET_TEXT          \
                DEFAULT_COLOR_TEXT "\n\n",                                             \
                __FILE__,                                                              \
                __LINE__,                                                              \
                __func__,                                                              \
                #_expr,                                                                \
                " " #__VA_ARGS__);                                                     \
            exit(1);                                                                   \
        }                                                                              \
    } while(0)
#endif


#endif // DEBUGGING_H_


