#ifndef MYCAT_MYCAT_H
#define MYCAT_MYCAT_H

#include <unistd.h>

#define BUFFSIZE 4096

const char help_message[] = "Usage: mycat [OPTION] ... [FILE] ...\n"
                            "Concatenate FILE(s) to standard output.\n\n"
                            "    -A          show hex values of invisible characters instead\n"
                            "    -h, --help  display this help and exit\n\n"
                            "Examples:\n"
                            "    mycat f g h     Output f's contents, then g's and h's.\n";

const char open_error[] = "mycat: open error\n";
const char read_error[] = "mycat: read error\n";
const char write_error[] = "mycat: write error\n";

struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                {NULL, 0,             NULL, 0}};


int readbuff(int fd, char *buffer, ssize_t size);

int writebuff(int fd, const char *buffer, ssize_t size);

void print(const char *string);

void error(const char *error_message);

void display_help_message();

int cat(int fd);

#endif //MYCAT_MYCAT_H
