// LMSApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "book.h"

int main()
{
	sqlite3* database;
	char* errorMessage = 0;
	int rc, option;
	const char* sql;
	const char* data = "Callback function called";

	rc = sqlite3_open("library.db", &database);
	if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(database) << endl;
		sqlite3_close(database);
		return 1;
	}
	else {
		cerr << "Opened database successfully" << endl;
	}

	sql = "CREATE TABLE IF NOT EXISTS BOOKS(" \
		"title TEX NOT NULL," \
		"author TEXT NOT NULL, " \
		"publication INT NOT NULL," \
		"available TEXT NOT NULL, " \
		"isbn TEXT NOT NULL);";

	rc = sqlite3_exec(database, sql, callback, (void*)data, &errorMessage);

	if (rc != SQLITE_OK) {
		cerr << "sql error:" << errorMessage << endl;
		sqlite3_free(errorMessage);
	}
	else {
		cout << "connected db successfully \n ";
	}

	//user menu
	do
	{
		cout << "welcome to the abc library system \n";
		cout << "1. Add a book \n";
		cout << "2. Search a book \n";
		cout << "3. Print the total books in the inventory \n";
		cout << "4. Edit book details \n";
		cout << "5. Exit system \n";
		cin >> option;
		
		cin.ignore(); // Clear the newline character from the input buffer

		Book book;

		switch (option) {
			case 1:
				book.read_details(database);
				book.save_details(database);
				break;
			case 2:
				int search_option;
				do
				{
					cout << "1. Search by Publication Year \n";
					cout << "2. Search by Author \n";
					cout << "3. Search by Book Title \n";
					cout << "4. See all available books \n";
					cout << "5. Go back \n";
					cin >> search_option;
					cin.ignore();

					switch (search_option) {
						case 1: search_book_year(database); break;
						case 2: search_book_author(database); break;
						case 3: search_book_title(database); break;
						case 4: check_availability(database); break;
						case 5: break;
						default: cout << "Invalid option. Please try again.\n"; break;
					}
				} while (search_option != 5);
				break;

			case 3:
				print_table(database);
				book_count(database);
				break;
			
			case 4:
				int edit_option;
				do
				{
					cout << "1. Change author name \n";
					cout << "2. Change year of publication \n";
					cout << "3. Change book title \n";
					cout << "4. Change ISBN \n";
					cout << "5. Change Availability status \n";
					cout << "6. Delete book record \n";
					cout << "7. go back \n";
					cin >> edit_option;
					cin.ignore(); // Clear the newline character from the input buffer

					if (edit_option >= 1 && edit_option <= 6) {
						string isbn;
						cout << "Enter ISBN of the book you want to modify or delete \n";
						getline(cin, isbn);

						switch (edit_option) {
							case 1: change_book_author(database, isbn); break;
							case 2: change_book_publication_year(database, isbn); break;
							case 3: change_book_title(database, isbn); break;
							case 4: change_isbn(database, isbn); break;
							case 5: change_availability(database, isbn); break;
							case 6: delete_book(database, isbn); break;
						}
					}
					else if (edit_option != 7) {
						cout << "Invalid input. Please try again.\n";
					}

				} while (edit_option != 7);
				break;

			case 5:
				cout << "exiting system";
				break;

			default:
				cout<< "Invalid option. Please try again.\n";

		}

	} while (option != 5);
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

