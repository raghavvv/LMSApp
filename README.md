# LMSApp

A simple C++14-based Library Management System (LMS) that uses SQLite for persistent storage. The application allows users to add, search, edit, and delete book records through a console interface.
Features
•	Add new books with title, author, publication year, availability, and ISBN
•	Search books by title, author, or publication year
•	View all available books
•	Edit book details (title, author, year, ISBN, availability)
•	Delete book records
•	View total number of books in the inventory
•	Data is stored in a local SQLite database (library.db)
Requirements
•	C++14 compatible compiler (e.g., MSVC, g++)
•	SQLite3 development libraries
•	Windows (uses winsqlite/winsqlite3.h)
Building
1.	Open the solution in Visual Studio.
2.	Make sure SQLite3 is available and properly referenced.
3.	Build the solution using Build Solution.
Usage
1.	Run the program (F5 or Debug > Start Debugging).
2.	Follow the on-screen menu to add, search, edit, or delete books.
3.	The database file library.db will be created in the working directory.
File Structure
•	LMSApp.cpp – Main application logic and menu
•	book.h / book.cpp – Book class and related database functions
Notes
•	When mixing cin and getline, the program uses cin.ignore() to handle input buffer issues.
•	All book records are stored in the BOOKS table in the SQLite database.
License
This project is for educational purposes.

