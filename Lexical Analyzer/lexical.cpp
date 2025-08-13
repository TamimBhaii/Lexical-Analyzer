#include <bits/stdc++.h>
using namespace std;

vector<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while", "bool"};

vector<string> predefinedidentifiers = {
    "printf", "scanf", "getchar", "putchar", "gets", "puts", "fopen", "fclose", "fread", "fwrite",
    "fprintf", "fscanf", "fseek", "ftell", "rewind", "fflush", "fgetc", "fputc", "fgets", "fputs",
    "perror", "remove", "rename", "tmpfile", "tmpnam", "EOF", "NULL", "SEEK_SET", "SEEK_CUR", "SEEK_END",
    "FILE", "size_t", "fpos_t",
    "malloc", "calloc", "realloc", "free", "exit", "abort", "system", "rand", "srand", "atof", "atoi",
    "atol", "strtod", "strtol", "strtoul", "EXIT_SUCCESS", "EXIT_FAILURE", "RAND_MAX", "div_t", "ldiv_t",
    "strlen", "strcpy", "strncpy", "strcat", "strncat", "strcmp", "strncmp", "strchr", "strrchr",
    "strstr", "strtok", "memset", "memcpy", "memmove", "memcmp",
    "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "sinh", "cosh", "tanh", "exp", "log", "log10",
    "pow", "sqrt", "ceil", "floor", "fabs", "fmod", "HUGE_VAL",
    "isalnum", "isalpha", "isdigit", "islower", "isupper", "isspace", "tolower", "toupper", "isprint", "ispunct"};

unordered_set<string> kwset, predefset;

unordered_set<string> typekeywords = {
    "int", "char", "float", "double", "long", "short", "signed", "unsigned", "void",
    "struct", "union", "enum", "typedef", "const", "volatile", "static", "extern", "register", "bool"};

bool iskeyword(const string &s) { return kwset.count(s); }
bool ispredefined(const string &s) { return predefset.count(s); }

bool ispunctuator(char c)
{
    string punct = "(){}[]:;,";
    return punct.find(c) != string::npos;
}
bool isoperatorstart(char c)
{
    string ops = "+-*/%=!<>&|";
    return ops.find(c) != string::npos;
}
bool isvalididentifierstart(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}
bool isvalididentifierchar(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}
bool isnumber(const string &s)
{
    if (s.empty())
    {
        return false;
    }
    int dot = 0;
    for (char c : s)
    {
        if (c == '.')
        {
            dot++;
        }
        else if (!isdigit((unsigned char)c))
        {
            return false;
        }
    }
    return dot <= 1;
}

unordered_set<string> collectdeclaredidentifiers(const string &code)
{
    unordered_set<string> declared;
    size_t n = code.size();
    size_t i = 0;
    bool typeseen = false;
    while (i < n)
    {
        if (isvalididentifierstart(code[i]))
        {
            string word;
            while (i < n && isvalididentifierchar(code[i]))
            {
                word.push_back(code[i]);
                i++;
            }
            if (typekeywords.count(word))
            {
                typeseen = true;
                continue;
            }
            if (typeseen)
            {
                declared.insert(word);
                while (i < n && isspace((unsigned char)code[i]))
                {
                    i++;
                }
                if (i < n && code[i] == '(')
                {
                    int depth = 0;
                    while (i < n)
                    {
                        if (code[i] == '(')
                        {
                            depth++;
                        }
                        else if (code[i] == ')')
                        {
                            depth--;
                            if (depth == 0)
                            {
                                i++;
                                break;
                            }
                        }
                        i++;
                    }
                }
                while (i < n)
                {
                    while (i < n && isspace((unsigned char)code[i]))
                    {
                        i++;
                    }
                    if (i < n && code[i] == ',')
                    {
                        i++;
                        while (i < n && isspace((unsigned char)code[i]))
                        {
                            i++;
                        }
                        while (i < n && code[i] == '*')
                        {
                            i++;
                        }
                        if (i < n && isvalididentifierstart(code[i]))
                        {
                            string name;
                            while (i < n && isvalididentifierchar(code[i]))
                            {
                                name.push_back(code[i]);
                                i++;
                            }
                            if (!name.empty())
                            {
                                declared.insert(name);
                            }
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
                typeseen = false;
            }
        }
        else
        {
            if (code[i] == ';' || code[i] == '\n')
            {
                typeseen = false;
            }
            i++;
        }
    }
    return declared;
}

void tokenize(const string &code, const unordered_set<string> &declared)
{
    istringstream input(code);
    string line;
    while (getline(input, line))
    {
        for (size_t i = 0; i < line.length();)
        {
            if (isspace((unsigned char)line[i]))
            {
                i++;
                continue;
            }
            if (line.substr(i, 8) == "#include")
            {
                cout << "Token: '#include'\tType: Preprocessor Directive\n";
                size_t start = line.find_first_of("<\"", i);
                size_t end = line.find_first_of(">\"", start == string::npos ? string::npos : start + 1);
                if (start != string::npos && end != string::npos)
                {
                    cout << "Token: '" << line.substr(start, end - start + 1) << "'\tType: Header File\n";
                }
                break;
            }
            if (line[i] == '"')
            {
                string str;
                str += line[i++];
                while (i < line.length() && line[i] != '"')
                {
                    str += line[i++];
                }
                if (i < line.length())
                {
                    str += line[i++];
                }
                cout << "Token: " << str << "\tType: String Literal\n";
                continue;
            }
            if (line[i] == '\'')
            {
                string ch;
                ch += line[i++];
                while (i < line.length() && line[i] != '\'')
                {
                    ch += line[i++];
                }
                if (i < line.length())
                {
                    ch += line[i++];
                }
                cout << "Token: " << ch << "\tType: Character Literal\n";
                continue;
            }
            string two = (i + 1 < line.size()) ? line.substr(i, 2) : string();
            vector<string> multi_ops = {"==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "%="};
            if (!two.empty() && find(multi_ops.begin(), multi_ops.end(), two) != multi_ops.end())
            {
                cout << "Token: '" << two << "'\tType: Operator\n";
                i += 2;
                continue;
            }
            if (isoperatorstart(line[i]))
            {
                cout << "Token: '" << line[i] << "'\tType: Operator\n";
                i++;
                continue;
            }
            if (ispunctuator(line[i]))
            {
                cout << "Token: '" << line[i] << "'\tType: Punctuator\n";
                i++;
                continue;
            }
            if (isdigit((unsigned char)line[i]))
            {
                string num;
                while (i < line.length() && (isdigit((unsigned char)line[i]) || line[i] == '.'))
                {
                    num += line[i++];
                }
                if (isnumber(num))
                {
                    cout << "Token: '" << num << "'\tType: Constant\n";
                }
                else
                {
                    cout << "Token: '" << num << "'\tType: Unknown\n";
                }
                continue;
            }
            if (isvalididentifierstart(line[i]))
            {
                string word;
                while (i < line.length() && isvalididentifierchar(line[i]))
                {
                    word += line[i++];
                }
                if (word == "true" || word == "false")
                {
                    cout << "Token: '" << word << "'\tType: BOOLEAN_LITERAL\n";
                }
                else if (iskeyword(word))
                {
                    cout << "Token: '" << word << "'\tType: Keyword\n";
                }
                else if (ispredefined(word))
                {
                    cout << "Token: '" << word << "'\tType: Predefined Identifier\n";
                }
                else if (declared.count(word))
                {
                    cout << "Token: '" << word << "'\tType: Identifier\n";
                }
                else
                {
                    cout << "Token: '" << word << "'\tType: Unknown\n";
                }
                continue;
            }
            cout << "Token: '" << line[i] << "'\tType: Unknown\n";
            i++;
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (auto &k : keywords)
    {
        kwset.insert(k);
    }
    for (auto &p : predefinedidentifiers)
    {
        predefset.insert(p);
    }

    freopen("input.c", "r", stdin);
    stringstream buffer;
    buffer << cin.rdbuf();
    string code = buffer.str();

    auto declared = collectdeclaredidentifiers(code);
    tokenize(code, declared);

    return 0;
}
