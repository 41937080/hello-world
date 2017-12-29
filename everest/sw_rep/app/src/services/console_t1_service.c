

#include "xuartps.h"
#include "stdio.h"


#include "console_t1_service.h"

typedef struct _COMMAND
{
    const char *cmd;
    CB_COMMAND cb;
} COMMAND;


COMMAND COMMAND_LIST[] = {
    {"test1",  test1},
    {NULL, NULL},
};


void show_hint()
{
    printf("Supported commands:\r\n");
    for(int32_t i = 0; i < 100; i++){
        if(COMMAND_LIST[i].cmd == NULL)break;
        printf("%s\r\n", COMMAND_LIST[i].cmd);
    }
}

int32_t test1 (int32_t argc, char *argv[])
{
    printf("test1\r\n");
    if(argc > 1) {
        printf("%s\r\n", argv[1]);
    }
    return 1;
}




static int32_t split( char *str, const char *delim, char *outlist[] ) {
    char    *tk;
    int32_t     cnt = 0;

    tk = strtok( str, delim );
    while( tk != NULL && cnt < MAX_LIST ) {
        outlist[cnt++] = tk;
        tk = strtok( NULL, delim );
    }
    return cnt;
}



void console_t1_init()
{
    char buf[512];
    char *argv[MAX_LIST];
    int32_t  argc;

    for(int32_t i = 0; i < 100; i++){
        char c = getchar();
        putchar(c);	// echo back
        if(c == '\r' || c == '\n' || c == '\0') {
            buf[i] = '\0';
            break;
        }
        buf[i] = c;
    }

    argc = split( buf, " " , argv );
    for(int32_t i = 0; i < 100; i++){
        if(COMMAND_LIST[i].cmd == NULL) {
            show_hint();
            break;
        }
        if( strcmp(COMMAND_LIST[i].cmd, argv[0]) == 0 ){
            COMMAND_LIST[i].cb(argc, argv);
            break;
        }
    }
}
