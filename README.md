This is a Lexical Analyzer web-based project built using Flask (Python), integrated with a C++ backend for core tokenization logic, and connected to a MySQL database for user management and analysis history tracking.

What is a Lexical Analyzer?
A Lexical Analyzer (also called a Lexer or Scanner) is the first phase of a compiler. It takes source code as input and breaks it into meaningful units called tokens like:

Keywords (int, if, return)
Identifiers (variable and function names)
Operators ( +, ==, &&)
Constants (numeric values)
String and character literals
Punctuators ( ;, {, })

This tool helps in analyzing C language code by identifying and categorizing all these elements.

Project Features:
User Authentication (Sign Up / Login system)
Paste or upload C code to analyze
Lexical Analysis Output with token classification
User History Tracking stored in MySQL
C++ backend for fast and accurate tokenization (lexical.cpp)
Clean, styled HTML interface for Login, Signup, Analysis, and History pages

Technologies Used:
Frontend: HTML5, CSS (Inline styling)
Backend: Python (Flask)
Lexical Engine: C++ (lexical.cpp compiled into lexical.exe)
Database: MySQL
Deployment: (e.g., Render, or planned)

How It Works:
A user logs in or signs up.
On the analysis page, the user pastes their C code.
That code is passed to a compiled lexical.exe (from lexical.cpp) for processing.
Tokens are generated and categorized by type.
The output is shown to the user and saved to the database for future access.

Output Example
Input:

#include <stdio.h>
int main() {
    char c = 'A';
    printf("Hello, world!\n");
    return 0;
}

Output Tokens:

Token: '#include'     Type: Preprocessor Directive
Token: '<stdio.h>'    Type: Header File
Token: 'int'          Type: Keyword
Token: 'main'         Type: Identifier
Token: '('            Type: Punctuator
Token: 'Hello, world!' Type: String Literal
Token: '0'             Type: Constant
Token: 'return'        Type: Keyword
...

This project was created as part of a compiler design lab project, showcasing how C code is tokenized before further compilation stages...
