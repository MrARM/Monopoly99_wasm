#ifndef MONOPOLY_WEB_COMPAT_IO_H
#define MONOPOLY_WEB_COMPAT_IO_H

#include <dirent.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef _O_RDONLY
#define _O_RDONLY O_RDONLY
#endif
#ifndef _O_WRONLY
#define _O_WRONLY O_WRONLY
#endif
#ifndef _O_RDWR
#define _O_RDWR O_RDWR
#endif
#ifndef _O_CREAT
#define _O_CREAT O_CREAT
#endif
#ifndef _O_TRUNC
#define _O_TRUNC O_TRUNC
#endif

struct _finddata_t {
    unsigned attrib;
    long time_create;
    long time_access;
    long time_write;
    long size;
    char name[260];
};

typedef struct MONOPOLY_WEB_IO_FIND {
    DIR *dir;
    char directory[512];
    char pattern[260];
} MONOPOLY_WEB_IO_FIND;

static inline void monopoly_web_io_normalize_path(const char *in, char *out, size_t outSize)
{
    if (!out || outSize == 0) return;
    size_t o = 0;
    if (!in) {
        out[0] = '\0';
        return;
    }
    if (((in[0] >= 'A' && in[0] <= 'Z') || (in[0] >= 'a' && in[0] <= 'z')) && in[1] == ':') {
        in += 2;
        if (*in == '\\' || *in == '/') in++;
    }
    while (*in && o + 1 < outSize) {
        out[o++] = (*in == '\\') ? '/' : *in;
        in++;
    }
    out[o] = '\0';
}

static inline int _open(const char *path, int flags, ...)
{
    char normalized[1024];
    monopoly_web_io_normalize_path(path, normalized, sizeof(normalized));
    return open(normalized, flags, 0666);
}

static inline int _close(int fd)
{
    return close(fd);
}

static inline int _read(int fd, void *buffer, unsigned int count)
{
    return (int)read(fd, buffer, count);
}

static inline int _write(int fd, const void *buffer, unsigned int count)
{
    return (int)write(fd, buffer, count);
}

static inline long _lseek(int fd, long offset, int origin)
{
    return (long)lseek(fd, offset, origin);
}

static inline long _filelength(int fd)
{
    struct stat st;
    return fstat(fd, &st) == 0 ? (long)st.st_size : -1L;
}

static inline char *_getcwd(char *buffer, int maxlen)
{
    return getcwd(buffer, (size_t)maxlen);
}

static inline int monopoly_web_io_fill_find_data(const MONOPOLY_WEB_IO_FIND *find, const char *name, struct _finddata_t *data)
{
    if (!find || !name || !data) return -1;
    memset(data, 0, sizeof(*data));
    strncpy(data->name, name, sizeof(data->name) - 1);
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", find->directory[0] ? find->directory : ".", name);
    struct stat st;
    if (stat(path, &st) == 0) {
        data->size = (long)st.st_size;
        data->time_create = data->time_access = data->time_write = (long)st.st_mtime;
        if (S_ISDIR(st.st_mode)) data->attrib |= 0x10;
    }
    return 0;
}

static inline int monopoly_web_io_next_match(MONOPOLY_WEB_IO_FIND *find, struct _finddata_t *data)
{
    if (!find || !find->dir || !data) return -1;
    struct dirent *entry;
    while ((entry = readdir(find->dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        if (fnmatch(find->pattern, entry->d_name, FNM_CASEFOLD) == 0)
            return monopoly_web_io_fill_find_data(find, entry->d_name, data);
    }
    return -1;
}

static inline long _findfirst(const char *pattern, struct _finddata_t *data)
{
    char normalized[1024];
    monopoly_web_io_normalize_path(pattern, normalized, sizeof(normalized));
    char *slash = strrchr(normalized, '/');
    MONOPOLY_WEB_IO_FIND *find = (MONOPOLY_WEB_IO_FIND *)calloc(1, sizeof(*find));
    if (!find) return -1L;
    if (slash) {
        size_t len = (size_t)(slash - normalized);
        if (len >= sizeof(find->directory)) len = sizeof(find->directory) - 1;
        memcpy(find->directory, normalized, len);
        find->directory[len] = '\0';
        strncpy(find->pattern, slash + 1, sizeof(find->pattern) - 1);
    } else {
        strcpy(find->directory, ".");
        strncpy(find->pattern, normalized, sizeof(find->pattern) - 1);
    }
    find->dir = opendir(find->directory[0] ? find->directory : ".");
    if (!find->dir) {
        free(find);
        return -1L;
    }
    if (monopoly_web_io_next_match(find, data) != 0) {
        closedir(find->dir);
        free(find);
        return -1L;
    }
    return (long)(intptr_t)find;
}

static inline int _findnext(long handle, struct _finddata_t *data)
{
    return monopoly_web_io_next_match((MONOPOLY_WEB_IO_FIND *)(intptr_t)handle, data);
}

static inline int _findclose(long handle)
{
    MONOPOLY_WEB_IO_FIND *find = (MONOPOLY_WEB_IO_FIND *)(intptr_t)handle;
    if (!find) return -1;
    if (find->dir) closedir(find->dir);
    free(find);
    return 0;
}

#endif
