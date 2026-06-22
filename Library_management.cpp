#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
//  STRUCTURES
// ============================================================

struct Book {
    int     id;
    string  title;
    string  author;
    bool    isIssued;
    int     issuedToMemberId; // 0 = not issued
};

struct Member {
    int    id;
    string name;
    string contact;
};

struct BorrowRecord {
    int memberId;
    int bookId;
    string issueDate;
    string returnDate; // empty = not returned yet
};

// ============================================================
//  GLOBAL DATA
// ============================================================

vector<Book>         books;
vector<Member>       members;
vector<BorrowRecord> records;

int nextBookId   = 1;
int nextMemberId = 1;

// ============================================================
//  BOOK FUNCTIONS
// ============================================================

void addBook() {
    Book b;
    b.id            = nextBookId++;
    b.isIssued      = false;
    b.issuedToMemberId = 0;

    cout << "\n--- Add New Book ---\n";
    cout << "Enter book title  : "; cin.ignore(); getline(cin, b.title);
    cout << "Enter author name : "; getline(cin, b.author);

    books.push_back(b);
    cout << "Book added successfully! Book ID = " << b.id << "\n";
}

void displayAllBooks() {
    if (books.empty()) {
        cout << "\nNo books in library.\n";
        return;
    }
    cout << "\n========== ALL BOOKS ==========\n";
    for (auto& b : books) {
        cout << "ID     : " << b.id     << "\n"
             << "Title  : " << b.title  << "\n"
             << "Author : " << b.author << "\n"
             << "Status : " << (b.isIssued ? "Issued" : "Available") << "\n"
             << "--------------------------------\n";
    }
}

// Search by title OR author (case-sensitive simple match)
void searchBook() {
    cout << "\nEnter title or author to search: ";
    string keyword;
    cin.ignore();
    getline(cin, keyword);

    bool found = false;
    for (auto& b : books) {
        if (b.title.find(keyword) != string::npos ||
            b.author.find(keyword) != string::npos) {
            cout << "\nID     : " << b.id     << "\n"
                 << "Title  : " << b.title  << "\n"
                 << "Author : " << b.author << "\n"
                 << "Status : " << (b.isIssued ? "Issued" : "Available") << "\n";
            found = true;
        }
    }
    if (!found) cout << "No matching book found.\n";
}

// ============================================================
//  MEMBER FUNCTIONS
// ============================================================

void addMember() {
    Member m;
    m.id = nextMemberId++;

    cout << "\n--- Add New Member ---\n";
    cout << "Enter member name    : "; cin.ignore(); getline(cin, m.name);
    cout << "Enter contact number : "; getline(cin, m.contact);

    members.push_back(m);
    cout << "Member added! Member ID = " << m.id << "\n";
}

void displayAllMembers() {
    if (members.empty()) {
        cout << "\nNo members registered.\n";
        return;
    }
    cout << "\n========== ALL MEMBERS ==========\n";
    for (auto& m : members) {
        cout << "ID      : " << m.id      << "\n"
             << "Name    : " << m.name    << "\n"
             << "Contact : " << m.contact << "\n"
             << "----------------------------------\n";
    }
}

// ============================================================
//  ISSUE & RETURN
// ============================================================

void issueBook() {
    int memberId, bookId;
    cout << "\n--- Issue Book ---\n";
    cout << "Enter Member ID : "; cin >> memberId;
    cout << "Enter Book ID   : "; cin >> bookId;

    // Find member
    Member* foundMember = nullptr;
    for (auto& m : members)
        if (m.id == memberId) { foundMember = &m; break; }

    if (!foundMember) { cout << "Member not found!\n"; return; }

    // Find book
    Book* foundBook = nullptr;
    for (auto& b : books)
        if (b.id == bookId) { foundBook = &b; break; }

    if (!foundBook)       { cout << "Book not found!\n";    return; }
    if (foundBook->isIssued) { cout << "Book is already issued to someone else.\n"; return; }

    // Issue
    foundBook->isIssued         = true;
    foundBook->issuedToMemberId = memberId;

    BorrowRecord r;
    r.memberId   = memberId;
    r.bookId     = bookId;
    r.issueDate  = "15-Jun-2026"; // In real project, use date library
    r.returnDate = "";
    records.push_back(r);

    cout << "Book \"" << foundBook->title << "\" issued to "
         << foundMember->name << " successfully!\n";
}

void returnBook() {
    int bookId;
    cout << "\n--- Return Book ---\n";
    cout << "Enter Book ID to return: "; cin >> bookId;

    Book* foundBook = nullptr;
    for (auto& b : books)
        if (b.id == bookId) { foundBook = &b; break; }

    if (!foundBook)          { cout << "Book not found!\n";           return; }
    if (!foundBook->isIssued) { cout << "This book was not issued.\n"; return; }

    // Update borrow record
    for (auto& r : records) {
        if (r.bookId == bookId && r.returnDate.empty()) {
            r.returnDate = "15-Jun-2026";
            break;
        }
    }

    foundBook->isIssued         = false;
    foundBook->issuedToMemberId = 0;
    cout << "Book \"" << foundBook->title << "\" returned successfully!\n";
}

// ============================================================
//  BORROW RECORDS
// ============================================================

void displayBorrowRecords() {
    if (records.empty()) { cout << "\nNo borrow records found.\n"; return; }

    cout << "\n========== BORROW RECORDS ==========\n";
    for (auto& r : records) {
        // Get names for display
        string memberName = "Unknown", bookTitle = "Unknown";
        for (auto& m : members) if (m.id == r.memberId) memberName = m.name;
        for (auto& b : books)   if (b.id == r.bookId)   bookTitle  = b.title;

        cout << "Member : " << memberName   << " (ID:" << r.memberId << ")\n"
             << "Book   : " << bookTitle    << " (ID:" << r.bookId   << ")\n"
             << "Issued : " << r.issueDate  << "\n"
             << "Return : " << (r.returnDate.empty() ? "Not returned yet" : r.returnDate) << "\n"
             << "-------------------------------------\n";
    }
}

// ============================================================
//  MAIN MENU
// ============================================================

int main() {
    cout << "========================================\n";
    cout << "   LIBRARY MANAGEMENT SYSTEM - C++     \n";
    cout << "========================================\n";

    int choice;
    do {
        cout << "\n====== MAIN MENU ======\n"
             << "1. Add Book\n"
             << "2. Add Member\n"
             << "3. Issue Book\n"
             << "4. Return Book\n"
             << "5. Search Book\n"
             << "6. View All Books\n"
             << "7. View All Members\n"
             << "8. View Borrow Records\n"
             << "0. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook();              break;
            case 2: addMember();            break;
            case 3: issueBook();            break;
            case 4: returnBook();           break;
            case 5: searchBook();           break;
            case 6: displayAllBooks();      break;
            case 7: displayAllMembers();    break;
            case 8: displayBorrowRecords(); break;
            case 0: cout << "Goodbye!\n";  break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}