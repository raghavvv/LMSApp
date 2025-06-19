#include "book.h"


Book::Book() {
	title = " ";
	author = " ";
	isbn = " ";
	publicationYear = 0;
	available = " ";
}

void Book::read_details(sqlite3* db) {
	string availability[2] = { "Yes", "No" };
	int option;
	cout << "Enter book details please:\n";
	cout << "Title:";
	getline(cin, title);
	cout << "Author:";
	getline(cin, author);
	cout << "ISBN:";
	getline(cin, isbn);

	while(is_in_db(db, isbn)) {
		cout << "This book already exists in the database. Please enter a different ISBN.\n";
		cout << "ISBN:";
		getline(cin, isbn);
	}

	cout << "Publication Year:";
	cin >> publicationYear;
	cin.ignore(); // Clear the newline character from the input buffer
	cout << "Is the book available? (1 for Yes, 2 for No): ";
	cin >> option;

	available = availability[option - 1];

}

void Book::save_details(sqlite3* db) {
	char* errMsg;
	int rc;
	char* sql;
	sql = sqlite3_mprintf("INSERT INTO BOOKS(title, author, publication, available, isbn) VALUES ('%q', '%q', '%d', '%q', '%q')", title.c_str(), author.c_str(), publicationYear, available.c_str(), isbn.c_str());
	cout << sql << endl;

	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cout << rc << endl;
		fprintf(stderr, "sql err : % s\n", errMsg);
		sqlite3_free(errMsg);

	}
	else {
		printf("REcord created successfully \n");
	}

	sqlite3_free(sql);

}


int callback(void* data, int argc, char** argv, char** colName) {
	int* count = (int*)data;
	(*count)++;
	return 0;
}

int print_callback(void* data, int argc, char** argv, char** colName) {
	for (int i = 0; i < argc; i++) {
		cout << colName[i] << "=" << endl;
	}
	cout << "=================================\n";

	return 0;
}

int is_in_db(sqlite3* db, string isbn) {
	char sql[100];
	snprintf(sql, 100, "SELECT * FROM BOOKS WHERE isbn='%s'", isbn.c_str());
	int count = 0;
	int rc = sqlite3_exec(db, sql, callback, &count, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "error: %s \n", sqlite3_errmsg(db));
		return -1;
	}
	return count > 0;
}

string change_book_title(sqlite3* db, string isbn) {
	string new_title;
	cout << "Enter new title:";
	getline(cin, new_title);

	char* errMsg = 0;
	int rc;

	char* sql = sqlite3_mprintf("UPDATE BOOKS SET title = '%q' HWERE isbn='%q';", new_title.c_str(), isbn.c_str());

	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s \n", errMsg);
		sqlite3_free(errMsg);
	}

	else {
		cout << "title changed! \n";
	}
	sqlite3_free(sql);
	return new_title;
}

string change_book_author(sqlite3* db, string isbn) {
	string new_author;
	cout << "Enter new author's name:";
	getline(cin, new_author);

	char* errMsg = 0;
	int rc;
	char* sql = sqlite3_mprintf("UPDATE BOOKS SET author ='%q' WHERE isbn='%q';", new_author.c_str(), isbn.c_str());

	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error:%s \n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		cout << "author updated successfully";
	}
	return new_author;
}

int change_book_publication_year(sqlite3* db, string isbn) {
	int new_pub_year;
	cout << "Enter updated publication year:";
	cin >> new_pub_year;
	
	cin.ignore();

	char* errMsg = 0;
	int rc;
	char* sql = sqlite3_mprintf("UPDATE BOOK SET publication='%q' WHERE isbn='%q';", new_pub_year, isbn.c_str());

	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s \n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		cout << "publication year updated successfully";
	}
	return new_pub_year;
}

string change_isbn(sqlite3* db, string isbn) {
	string new_isbn;

	cout << "Enter new ISBN: ";
	getline(cin, new_isbn);

	int rc;
	char* errMsg = 0;
	char* sql = sqlite3_mprintf("UPDATE BOOKS SET isbn='%q' WHERE isbn='%q';", new_isbn.c_str(), isbn.c_str());
	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s \n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		cout << "isbn update success!";
	}

	return new_isbn;
}

bool delete_book(sqlite3* db, string isbn) {
	string sql = "DELETE FROM BOOKS WHERE isbn = ?;";
	sqlite3_stmt* stmt;

	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, isbn.c_str(), -1, SQLITE_STATIC);
	int result = sqlite3_step(stmt);

	if (result == SQLITE_DONE) {
		cout << "book record deleted";
		return true;
	}
	else {
		cout << "error deleeting book :(";
		return false;
	}
}


void print_table(sqlite3* db) {
	string sql = "SELECT * FROM BOOKS;";

	char* errMsg = NULL;
	int result = sqlite3_exec(db, sql.c_str(), print_callback, 0, &errMsg);

	if (result != SQLITE_OK) {
		cerr << "Error using SELECT statement:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
}

int count_callback(void* data, int argc, char** argv, char** colName) {
	int* count = static_cast<int*>(data);
	*count = stoi(argv[0]);
	return 0;
}

void book_count(sqlite3* db) {
	int count = 0;
	int rc;
	char* errMsg;

	string sql = "SELECT COUNT(*) FROM BOOKS;";
	rc = sqlite3_exec(db, sql.c_str(), count_callback, &count, &errMsg);

	if (rc != SQLITE_OK) {
		cerr << "error from sql:" << errMsg << endl;
		sqlite3_free(errMsg);
	}
	else {
		cout << "Number of books in the table:" << count << endl;
	}

}

void search_book_title(sqlite3* db) {
	int count = 0;
	string keyword;

	cout << "enter a keyword to search for the book:";
	getline(cin, keyword);

	string sql = "SELECT title, author, publication, available, isbn FROM BOOKS WHERE title LIKE ?'";
	sqlite3_stmt* stmt;

	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	string search_term = "%" + keyword + "%";

	sqlite3_bind_text(stmt, 1, search_term.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(stmt);

	while (result == SQLITE_ROW) {
		cout << "Title:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << endl;
		cout << "Author:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << endl;
		cout << "Published in:" << (sqlite3_column_int(stmt, 2)) << endl;
		cout << "Availability:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << endl;
		cout << "ISBN:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << endl;
		cout << "=========================================================\n";

		result = sqlite3_step(stmt);
		count++;
	}
	if (result == SQLITE_DONE) {
		cout << "error searching for book:\n";
	}
	if (count == 0) {
		cout << "book does not exist in db :(";
	}

	sqlite3_finalize(stmt);
}

void search_book_author(sqlite3* db) {
	int count = 0;
	string keyword;

	cout << "enter a keyword to search for the book:";
	getline(cin, keyword);

	string sql = "SELECT title, author, publication, available, isbn FROM BOOKS WHERE author LIKE ?'";
	sqlite3_stmt* stmt;

	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	string search_term = "%" + keyword + "%";

	sqlite3_bind_text(stmt, 1, search_term.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(stmt);

	while (result == SQLITE_ROW) {
		cout << "Title:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << endl;
		cout << "Author:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << endl;
		cout << "Published in:" << (sqlite3_column_int(stmt, 2)) << endl;
		cout << "Availability:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << endl;
		cout << "ISBN:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << endl;
		cout << "=========================================================\n";

		result = sqlite3_step(stmt);
		count++;
	}
	if (result == SQLITE_DONE) {
		cout << "error searching for book:\n";
	}
	if (count == 0) {
		cout << "book does not exist in db :(";
	}

	sqlite3_finalize(stmt);
}

void check_availability(sqlite3* db) {
	int count = 0;
	string sql = "SELECT title, author, publication, available, isbn FROM BOOKS WHERE available = 'Yes';";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	int result = sqlite3_step(stmt);
	while (result == SQLITE_ROW) {
		cout << "Title:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << endl;
		cout << "Author:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << endl;
		cout << "Published in:" << (sqlite3_column_int(stmt, 2)) << endl;
		cout << "Availability:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << endl;
		cout << "ISBN:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << endl;
		cout << "=========================================================\n";
		result = sqlite3_step(stmt);
		count++;
	}

	if (result == SQLITE_DONE) {
		cout << "error checking availability:\n";
	}
	if (count == 0) {
		cout << "No books available at the moment.\n";
	}

	sqlite3_finalize(stmt);
}


void search_book_year(sqlite3* db) {

	int count = 0, year;
	cout << "Enter publication year:";

	cin >> year;
	cin.ignore(); // Clear the newline character from the input buffer
	string sql = "SELECT title, author, publication, available, isbn FROM BOOKS WHERE publication = ?";

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, year);
	int result = sqlite3_step(stmt);

	while (result == SQLITE_ROW) {
		cout << "Title:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << endl;
		cout << "Author:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << endl;
		cout << "Published in:" << (sqlite3_column_int(stmt, 2)) << endl;
		cout << "Availability:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << endl;
		cout << "ISBN:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << endl;
		cout << "=========================================================\n";
		result = sqlite3_step(stmt);
		count++;
	}
	if (result == SQLITE_DONE) {
		cout << "error searching for book:\n";
	}
	if (count == 0) {
		cout << "book does not exist in db :(";
	}

	sqlite3_finalize(stmt);

}

void change_availability(sqlite3* db, string isbn) {
	string availability[2] = { "Yes", "No" };

	int option;
	cout << "please choose availability (1 for yes, 2 for no)";
	cin >> option;
	cin.ignore(); // Clear the newline character from the input buffer

	char* errMsg = 0;
	int rc;
	char* sql = sqlite3_mprintf("UPDATE BOOKS SET available = '%q' WHERE isbn='%q';", availability[option - 1].c_str(), isbn.c_str());
	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s \n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		cout << "availability updated successfully! \n";
	}

}


