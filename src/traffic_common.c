#include "traffic_common.h"

int read_line(char *s, int n)
{
    if (!fgets(s, n, stdin))
        return 0;
    s[strcspn(s, "\r\n")] = 0;
    return 1;
}

int read_number(const char *prompt, int *value)
{
    char s[80], extra;
    printf("%s", prompt);
    return read_line(s, sizeof(s)) && sscanf(s, " %d %c", value, &extra) == 1;
}

int read_vertex(const char *prompt)
{
    char s[80], c;
    printf("%s", prompt);
    if (!read_line(s, sizeof(s)) || sscanf(s, " %c", &c) != 1)
        return -1;
    c = (char)toupper((unsigned char)c);
    return (c >= 'A' && c < 'A' + N) ? c - 'A' : -1;
}
