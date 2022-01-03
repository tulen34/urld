#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <curl/curl.h>

#define WORDLIST_CURRENT_SIZE 2064

struct wordlist {
    char  *content;
    size_t length;
    size_t offset;
    char   current[WORDLIST_CURRENT_SIZE];
};

static struct wordlist *wordlist_read(FILE *stream) {
    if (stream == NULL) {
        errno = EINVAL;
        return NULL;
    }

    fseek(stream, 0L, SEEK_END);
    long n = ftell(stream);
    rewind(stream);

    static struct wordlist w = {};
    w.content = malloc((size_t)n);
    if (w.content == NULL)
        return NULL;

    while (fgets(w.current, WORDLIST_CURRENT_SIZE, stream) != NULL) {
        // Игнорирование комментариев
        char *m = strchr(w.current, '#');
        if (m != NULL) {
            m[0] = '\n';
            m[1] = '\0';
        }
        // Хранить если не пустая строка
        if (sscanf(w.current, "%*s") == 0)
            strcat(w.content, w.current);
    }

    // Удаление последнего \n
    w.length = strlen(w.content) - 1;
    w.content[w.length] = '\0';
    realloc(w.content, w.length);

    return &w;
}

char *wordlist_next(struct wordlist *w) {
    if (w->offset >= w->length)
        return NULL;

    int n;
    sscanf(w->content + w->offset, "%[^\n]\n%n", w->current, &n);
    w->offset += (size_t)n;

    return w->current;
}


int main(int argc, char *argv[]) { 
    FILE *fp = fopen("/usr/share/wordlists/common.txt", "r");
    static struct wordlist *w;
    w = wordlist_read(fp);
    while (wordlist_next(w) != NULL);
}
