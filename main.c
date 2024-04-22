#include <stdio.h>
#include "sqlite3.h"

int main(int argc, char** argv) {
    sqlite3* db;
    char* errMessage = 0;
    sqlite3_stmt* stmt;
    int res = sqlite3_open("test.db", &db);

    if (res) {
        fprintf(stderr, "Open DB Fail: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stderr, "Open DB Success\n");
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS Contacts(Id INT, Name TEXT);";
    res = sqlite3_exec(db, sql, 0, 0, &errMessage);
    
    if (res != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", errMessage);
        sqlite3_free(errMessage);
    } else {
        fprintf(stdout, "Table create\n");
    }

    sqlite3_enable_load_extension(db, 1);
    res = sqlite3_load_extension(db, "/home/xubbbb/Code/sqlite-vector/sqlite-vss/build/vector0.so", 0, &errMessage);

    if (res != SQLITE_OK) {
        fprintf(stderr, "Load extension 1 fail: %s\n", errMessage);
        sqlite3_free(errMessage);
    } else {
        fprintf(stdout, "Load extension 1 success\n");
    }

    res = sqlite3_load_extension(db, "/home/xubbbb/Code/sqlite-vector/sqlite-vss/build/vss0.so", 0, &errMessage);

    if (res != SQLITE_OK) {
        fprintf(stderr, "load extension 2 fail: %s\n", errMessage);
        sqlite3_free(errMessage);
    } else {
        fprintf(stdout, "load extension 2 success\n");
    }

    res = sqlite3_prepare_v2(db, "SELECT vss_version(), vector_to_json(?)", -1, &stmt, NULL);

    if (res != SQLITE_OK) {
        fprintf(stderr, "Prepare fail\n");
    } else {
        fprintf(stdout, "Prepare success\n");
    }

    unsigned char blob[] = {0x00, 0x0, 0x28, 0x42};
    sqlite3_bind_blob(stmt, 1, blob, sizeof(blob), SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        fprintf(stdout, "vss_version: %s\n", sqlite3_column_text(stmt, 0));
        fprintf(stdout, "vector_to_json: %s\n", sqlite3_column_text(stmt, 1));
    }

    sqlite3_finalize(stmt);

    res = sqlite3_exec(db,
    "CREATE VIRTUAL TABLE vss_demo USING vss0(a(2)); \
    INSERT INTO vss_demo(rowid, a) \
      VALUES \
        (1, '[1.0, 2.0]'), \
        (2, '[2.0, 2.0]'), \
        (3, '[1.5, 2.0]')  \
    "
  , NULL, NULL, &errMessage);

    if (res != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", errMessage);
        sqlite3_free(errMessage);
    } else {
        fprintf(stdout, "Table create\n");
    }

    res = sqlite3_prepare_v2(db, "\
    SELECT \
      rowid, \
      distance \
    FROM vss_demo \
    WHERE vss_search(a, '[1.0, 2.0]') \
    LIMIT 10;\
    ", -1, &stmt, NULL);

    if (res != SQLITE_OK) {
        fprintf(stderr, "Prepare fail\n");
    } else {
        fprintf(stdout, "Prepare success\n");
    }

    while (SQLITE_ROW == (res = sqlite3_step(stmt))) {
        long rowid = sqlite3_column_int64(stmt, 0);
        double distance = sqlite3_column_double(stmt, 1);
        printf("rowid=%ld distance=%f\n", rowid, distance);
    }

    if(res != SQLITE_DONE) {
        fprintf(stderr, "Query fail\n");
    } else {
        fprintf(stdout, "Query success\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}