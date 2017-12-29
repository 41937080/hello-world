

#ifndef CONSOLE_T1_SERVICE_H_
#define CONSOLE_T1_SERVICE_H_

#define MAX_LIST 20

typedef int32_t (*CB_COMMAND)(int32_t argc, char *argv[]);


void show_hint();
int32_t test1 (int32_t argc, char *argv[]);
static int32_t split( char *str, const char *delim, char *outlist[] );
void console_t1_init();

#endif
