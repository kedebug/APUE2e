#include "db.h"
#include "../include/apue.h"

#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/uio.h>

#define IDXLEN_SIZE     4
#define SEP             ':'
#define SPACE           ' '
#define NEWLINE         '\n'

#define PTR_SIZE        6
#define PTR_MAX         999999
#define NHASH_DEF       137
#define FREE_OFF        0
#define HASH_OFF        PTR_SIZE

typedef unsigned long   DBHASH;
typedef unsigned long   COUNT;

typedef struct _DB {
    int     idxfd;
    int     datfd;
    char*   idxbuf;
    char*   datbuf;
    char*   name;

    off_t   idxoff;
    size_t  idxlen;
    off_t   datoff;
    size_t  datlen;

    off_t   ptrval;
    off_t   ptroff;
    off_t   chainoff;
    off_t   hashoff;
    DBHASH  nhash;
    COUNT   cnt_fetcherr;
    COUNT   cnt_fetchok;
    COUNT   cnt_delerr;
    COUNT   cnt_delok;
    COUNT   cnt_store1;     // DB_INSERT, append
    COUNT   cnt_store2;     // DB_INSERT, empty
    COUNT   cnt_store3;     // DB_REPLACE, diff len, append
    COUNT   cnt_store4;     // DB_REPLACE, same len, overwrite
} DB;


static DB*      _db_alloc(int);
static void     _db_dodelete(DB*);
static int      _db_find_and_lock(DB*, const char*, int);
static int      _db_findfree(DB*, int, int);
static int      _db_free(DB*);
static DBHASH   _db_hash(DB*, const char*);
static char*    _db_readdat(DB*);
static char*    _db_readidx(DB*, off_t);
static char*    _db_readptr(DB*, off_t);

DBHANDLE db_open(const char* pathname, int oflag, ...) {
    DB* db;
    int len, mode;

    len = strlen(pathname);
    if ((db = _db_alloc(len)) == NULL) {
        err_dump("db_open: _db_alloc error for DB");
    } 

    db->nhash = NHASH_DEF;
    db->hashoff = HASH_OFF;
    strcpy(db->name, pathname);
    strcat(db->name, ".idx");

    if (oflag & O_CREAT) {
        va_list ap;
        va_start(ap, oflag);
        mode = va_arg(ap, int);
        va_end(ap);

        db->idxfd = open(db->name, oflag, mode);
        strcpy(db->name + len, ".dat");
        db->datfd = open(db->name, oflag, mode);
    } else {
        db->idxfd = open(db->name, oflag);
        strcpy(db->name + len, ".dat");
        db->datfd = open(db->name, oflag);
    }

    if (db->idxfd < 0 || db->datfd < 0) {
        _db_free(db);
        return NULL;
    }

    if ((oflag & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC)) {
        if (writew_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
            err_dump("db_open: writew_lock failed");

        struct stat statbuf;
        char asci[PTR_SIZE + 1], hash[PTR_SIZE * (NHASH_DEF + 1) + 2];

        if (fstat(db->idxfd, &statbuf) < 0) 
            err_dump("db_open: fstat failed");

        if (statbuf.st_size == 0) {
            sprintf(asci, "%*d", PTR_SIZE, 0);
            hash[0] = 0;
            for (size_t i = 0; i < NHASH_DEF + 1; i++)
                strcat(hash, asci);
            strcat(hash, '\n');
            i = strlen(hash);
            if (write(db->idxfd, hash, i) != i)
                err_dump("db_open: write index file error");
        }
        if (un_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
            err_dump("db_open: un_lock failed");
    }
    db_rewind(db);
    return db;
}

static DB* _db_alloc(int namelen) {
    DB* db;

    if ((db == calloc(1, sizeof(DB))) == NULL)
        err_dump("_db_alloc: calloc error");

    db->idxfd = db->datfd = -1;

    if ((db->name = malloc(namelen + 5)) == NULL) 
        err_dump("_db_alloc: malloc for db->name error");


    if ((db->idxbuf = malloc(IDXLEN_MAX + 2)) == NULL) 
        err_dump("_db_alloc: malloc for db->idxbuf error");
    if ((db->datbuf = malloc(DATLEN_MAX + 2)) == NULL) 
        err_dump("_db_alloc: malloc for db->datbuf error");

    return db;
}

void db_close(DBHANDLE h) {
    _db_free((DB*)h);
}

static void _db_free(DB* db) {
    if (db->idxfd > 0)
        close(db->idxfd);
    if (db->datfd > 0)
        close(db->datfd);
    if (db->idxbuf != NULL)
        free(db->idxbuf);
    if (db->datbuf != NULL)
        free(db->datbuf);
    if (db->name != NULL)
        free(db->name);

    free(db);
}

char* db_fetch(DBHANDLE h, const char* key) {
    DB*     db = (DB*)h;
    char*   ptr;

    if (_db_find_and_lock(db, key, 0) < 0) {
        ptr = NULL;
        db->cnt_fetcherr += 1; 
    } else {
        ptr = _db_readdat(db);
        db->cnt_fetchok += 1;
    }

    if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
        err_dump("db_fetch: unlock error");
}
