#include <bits/stdc++.h>
using namespace std;

bool isKeyword(const string& s) {
    vector<string> keywords = {
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
        "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
        "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
        "union", "unsigned", "void", "volatile", "while"
    };
    return find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool isPunctuator(char c) {
    string punct = "(){}[]:;,";
    return punct.find(c) != string::npos;
}

bool isOperatorStart(char c) {
    string ops = "+-*/%=!<>&|";
    return ops.find(c) != string::npos;
}

bool isValidIdentifierStart(char c) {
    return isalpha(c) || c == '_';
}

bool isValidIdentifierChar(char c) {
    return isalnum(c) || c == '_';
}

bool isHeaderLine(const string& line) {
    return line.substr(0, 8) == "#include";
}

bool isNumber(const string& s) {
    if (s.empty()) return false;
    int dot = 0;
    for (char c : s) {
        if (c == '.') dot++;
        else if (!isdigit(c)) return false;
    }
    return dot <= 1;
}

void tokenize(string code) {
    istringstream input(code);
    string line;

    while (getline(input, line)) {
        // Remove comments
        size_t pos = line.find("//");
        if (pos != string::npos) line = line.substr(0, pos);

        // Handle multiline comments
        static bool in_block_comment = false;
        if (in_block_comment) {
            size_t end_pos = line.find("*/");
            if (end_pos != string::npos) {
                in_block_comment = false;
                line = line.substr(end_pos + 2);
            } else continue;
        }
        pos = line.find("/*");
        if (pos != string::npos) {
            in_block_comment = true;
            line = line.substr(0, pos);
        }

        for (size_t i = 0; i < line.length();) {
            if (isspace(line[i])) {
                i++;
                continue;
            }

            // Header files
            if (line.substr(i, 8) == "#include") {
                cout << "Token: '#include'\tType: Preprocessor Directive" << endl;
                size_t start = line.find_first_of("<\"", i);
                size_t end = line.find_first_of(">\"", start + 1);
                if (start != string::npos && end != string::npos)
                    cout << "Token: '" << line.substr(start, end - start + 1) << "'\tType: Header File" << endl;
                break;
            }

            // String literals
            if (line[i] == '"') {
                string str;
                str += line[i++];
                while (i < line.length() && line[i] != '"') str += line[i++];
                if (i < line.length()) str += line[i++];
                cout << "Token: " << str << "\tType: String Literal" << endl;
                continue;
            }

            // Char literals
            if (line[i] == '\'') {
                string ch;
                ch += line[i++];
                while (i < line.length() && line[i] != '\'') ch += line[i++];
                if (i < line.length()) ch += line[i++];
                cout << "Token: " << ch << "\tType: Character Literal" << endl;
                continue;
            }

            // Operators (multi-char)
            string two = line.substr(i, 2);
            vector<string> multi_ops = {"==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "%="};
            if (find(multi_ops.begin(), multi_ops.end(), two) != multi_ops.end()) {
                cout << "Token: '" << two << "'\tType: Operator" << endl;
                i += 2;
                continue;
            }

            // Single-char operator
            if (isOperatorStart(line[i])) {
                cout << "Token: '" << line[i] << "'\tType: Operator" << endl;
                i++;
                continue;
            }

            // Punctuators
            if (isPunctuator(line[i])) {
                cout << "Token: '" << line[i] << "'\tType: Punctuator" << endl;
                i++;
                continue;
            }

            // Numbers
            if (isdigit(line[i])) {
                string num;
                while (i < line.length() && (isdigit(line[i]) || line[i] == '.')) num += line[i++];
                if (isNumber(num))
                    cout << "Token: '" << num << "'\tType: Constant" << endl;
                else
                    cout << "Token: '" << num << "'\tType: Unknown" << endl;
                continue;
            }

            // Identifiers or Keywords
            if (isValidIdentifierStart(line[i])) {
                string word;
                while (i < line.length() && isValidIdentifierChar(line[i])) word += line[i++];
                if (isKeyword(word))
                    cout << "Token: '" << word << "'\tType: Keyword" << endl;
                else
                    cout << "Token: '" << word << "'\tType: Identifier" << endl;
                continue;
            }

            // Unknown
            cout << "Token: '" << line[i] << "'\tType: Unknown" << endl;
            i++;
        }
    }
}

int main() {
    freopen("input.c", "r", stdin);
    stringstream buffer;
    buffer << cin.rdbuf();
    tokenize(buffer.str());
    return 0;
}
