#pragma once
#include <string>
#include <winsqlite/winsqlite3.h>
#include <iostream>
#include <cstring>
using namespace std;


class Book {
public:
	string title;
	string author;
	string isbn;
	int publicationYear;
	string available;
	Book();
	void read_details(sqlite3* db);
	void save_details(sqlite3* db);
};

int count_callback(void* data, int argc, char** argv, char** colName); //
int callback(void* data, int argc, char** argv, char** colName);
int print_callback(void* data, int argc, char** argv, char** colName);

int is_in_db(sqlite3* db, string isbn);

string change_book_title(sqlite3* db, string isbn);
string change_book_author(sqlite3* db, string isbn);
int change_book_publication_year(sqlite3* db, string isbn);
string change_isbn(sqlite3* db, string isbn);
void change_availability(sqlite3* db, string isbn);
bool delete_book(sqlite3* db, string isbn);
void print_table(sqlite3* db);
void book_count(sqlite3* db);
void search_book_title(sqlite3* db);
void search_book_author(sqlite3* db);
void search_book_year(sqlite3* db);
void check_availability(sqlite3* db);

