#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mycat.h"

int main(int argc, char *argv[]) {
    if (argc == 1)
        display_help_message();

    int opt;
    size_t invisible_to_hex = 0;
    while ((opt = getopt_long(argc, argv, "Ah", long_options, NULL)) != -1) {
        switch (opt) {
            case 'A':
                invisible_to_hex++;
                break;
            case 'h':
            default:
                display_help_message();
        }
    }

    size_t passed_files = argc - 1 - invisible_to_hex;
    int *opened_files = (int *) malloc(sizeof(int) * passed_files);
    size_t j = 0;
    for (size_t i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-A", strlen(argv[i]))) {
            int fd;
            if ((fd = open(argv[i], O_RDONLY)) == -1) {
                error(open_error);
                for (size_t k = 0; k < j; k++)
                    close(opened_files[k]);
                free(opened_files);
                _exit(1);
            }
            opened_files[j++] = fd;
        }
    }

    for (size_t i = 0; i < passed_files; ++i) {
        cat(opened_files[i]);
        close(opened_files[i]);
    }
    free(opened_files);
    _exit(0);
}

int readbuff(int fd, char *buffer, ssize_t size) {
    ssize_t read_bytes = 0;
    while (read_bytes < size) {
        ssize_t just_read = read(fd, buffer + read_bytes, size - read_bytes);
        if (just_read <= 0) {
            if (errno == EINTR)
                continue;
            return just_read;
        }
        read_bytes += just_read;
    }
    return read_bytes;
}

int writebuff(int fd, const char *buffer, ssize_t size) {
    ssize_t written_bytes = 0;
    while (written_bytes < size) {
        ssize_t just_written = write(fd, buffer + written_bytes, size - written_bytes);
        if (just_written == -1) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        written_bytes += just_written;
    }
    return written_bytes;
}

void print(const char *string) {
    writebuff(1, string, strlen(string));
}

void error(const char *error_message) {
    writebuff(2, error_message, strlen(error_message));
}

void display_help_message() {
    print(help_message);
    _exit(0);
}

int cat(int fd) {
    char buffer[BUFFSIZE];
    int n;
    while ((n = readbuff(fd, buffer, BUFFSIZE)) > 0)
        if (writebuff(1, buffer, n) != n) {
            error(write_error);
            return -1;
        }
    if (n < 0) {
        error(read_error);
        return -1;
    }
    print("\n");
    return 0;
}
