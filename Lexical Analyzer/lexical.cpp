#include <bits/stdc++.h>
using namespace std;

// ---------- keyword + predefined lists ----------
vector<string> keywords = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while"
};

vector<string> predefinedIdentifiers = {
    // stdio.h (partial)
    "printf","scanf","getchar","putchar","gets","puts","fopen","fclose","fread","fwrite",
    "fprintf","fscanf","fseek","ftell","rewind","fflush","fgetc","fputc","fgets","fputs",
    "perror","remove","rename","tmpfile","tmpnam","EOF","NULL","SEEK_SET","SEEK_CUR","SEEK_END",
    "FILE","size_t","fpos_t",
    // stdlib.h (partial)
    "malloc","calloc","realloc","free","exit","abort","system","rand","srand","atof","atoi",
    "atol","strtod","strtol","strtoul","EXIT_SUCCESS","EXIT_FAILURE","RAND_MAX","div_t","ldiv_t",
    // string.h (partial)
    "strlen","strcpy","strncpy","strcat","strncat","strcmp","strncmp","strchr","strrchr",
    "strstr","strtok","memset","memcpy","memmove","memcmp",
    // math.h (partial)
    "sin","cos","tan","asin","acos","atan","atan2","sinh","cosh","tanh","exp","log","log10",
    "pow","sqrt","ceil","floor","fabs","fmod","HUGE_VAL",
    // ctype.h (partial)
    "isalnum","isalpha","isdigit","islower","isupper","isspace","tolower","toupper","isprint","ispunct",
    // add more as you like...
};

unordered_set<string> kwset, predefset;

// type-like words to detect declarations (heuristic)
unordered_set<string> typeKeywords = {
    "int","char","float","double","long","short","signed","unsigned","void",
    "struct","union","enum","typedef","const","volatile","static","extern","register"
};

// ---------- helpers ----------
bool isKeyword(const string &s){ return kwset.count(s); }
bool isPredefined(const string &s){ return predefset.count(s); }

bool isPunctuator(char c) {
    string punct="(){}[]:;,";
    return punct.find(c)!=string::npos;
}
bool isOperatorStart(char c) {
    string ops="+-*/%=!<>&|";
    return ops.find(c)!=string::npos;
}
bool isValidIdentifierStart(char c) {
    return isalpha((unsigned char)c) || c=='_';
}
bool isValidIdentifierChar(char c) {
    return isalnum((unsigned char)c) || c=='_';
}
bool isNumber(const string &s) {
    if(s.empty()) return false;
    int dot=0;
    for(char c:s){
        if(c=='.') dot++;
        else if(!isdigit((unsigned char)c)) return false;
    }
    return dot<=1;
}

// ---------- first pass: collect declared identifiers (simple heuristics) ----------
unordered_set<string> collectDeclaredIdentifiers(const string &code){
    unordered_set<string> declared;
    size_t n = code.size();
    size_t i = 0;
    bool typeSeen = false; // whether we've recently seen a type-like word
    while(i < n){
        if(isValidIdentifierStart(code[i])){
            string word;
            while(i<n && isValidIdentifierChar(code[i])) { word.push_back(code[i]); i++; }
            // lower-case? C is case-sensitive so keep as-is.
            if(typeKeywords.count(word)){
                // saw a type keyword; mark
                typeSeen = true;
                continue;
            }
            if(typeSeen){
                // this word is likely a declared name (var or function)
                declared.insert(word);
                // skip spaces
                while(i<n && isspace((unsigned char)code[i])) i++;
                // if we find '(' after name => likely function (still declared)
                if(i<n && code[i]=='('){
                    // consume until matching ')' (rudimentary)
                    int depth = 0;
                    while(i<n){
                        if(code[i]=='(') depth++;
                        else if(code[i]==')') { depth--; if(depth==0){ i++; break; } }
                        i++;
                    }
                }
                // handle comma separated declarations: , nextName ...
                while(i<n){
                    // skip spaces
                    while(i<n && isspace((unsigned char)code[i])) i++;
                    if(i<n && code[i]==','){
                        i++; while(i<n && isspace((unsigned char)code[i])) i++;
                        // skip pointer stars
                        while(i<n && code[i]=='*') i++;
                        // read next identifier
                        if(i<n && isValidIdentifierStart(code[i])){
                            string name;
                            while(i<n && isValidIdentifierChar(code[i])) { name.push_back(code[i]); i++; }
                            if(!name.empty()) declared.insert(name);
                            continue;
                        } else {
                            // no identifier after comma -> break
                            break;
                        }
                    }
                    break;
                }
                // when declaration ends (we saw a name), reset typeSeen
                typeSeen = false;
            }
            // else ordinary word (not type, not part of declaration) -> ignore
        } else {
            // non-word char
            // reset typeSeen on semicolon or newline (end of declaration)
            if(code[i]==';' || code[i]=='\n') typeSeen = false;
            i++;
        }
    }
    return declared;
}

// ---------- tokenizer (second pass) ----------
void tokenize(const string &code, const unordered_set<string> &declared){
    istringstream input(code);
    string line;
    while(getline(input, line)){
        for(size_t i=0;i<line.length();){
            if(isspace((unsigned char)line[i])) { i++; continue; }

            if(line.substr(i,8) == "#include"){
                cout<<"Token: '#include'\tType: Preprocessor Directive\n";
                size_t start=line.find_first_of("<\"",i);
                size_t end=line.find_first_of(">\"", start==string::npos?string::npos:start+1);
                if(start!=string::npos && end!=string::npos)
                    cout<<"Token: '"<<line.substr(start, end-start+1)<<"'\tType: Header File\n";
                break;
            }

            if(line[i]=='"'){
                string str; str += line[i++];
                while(i<line.length() && line[i]!='"') { str+=line[i++]; }
                if(i<line.length()) str+=line[i++];
                cout<<"Token: "<<str<<"\tType: String Literal\n";
                continue;
            }

            if(line[i]=='\''){
                string ch; ch += line[i++];
                while(i<line.length() && line[i]!='\'') { ch+=line[i++]; }
                if(i<line.length()) ch+=line[i++];
                cout<<"Token: "<<ch<<"\tType: Character Literal\n";
                continue;
            }

            string two = (i+1<line.size()) ? line.substr(i,2) : string();
            vector<string> multi_ops = {"==","!=","<=",">=","&&","||","++","--","+=","-=","*=","/=","%="};
            if(!two.empty() && find(multi_ops.begin(), multi_ops.end(), two) != multi_ops.end()){
                cout<<"Token: '"<<two<<"'\tType: Operator\n";
                i += 2;
                continue;
            }

            if(isOperatorStart(line[i])){
                cout<<"Token: '"<<line[i]<<"'\tType: Operator\n";
                i++;
                continue;
            }

            if(isPunctuator(line[i])){
                cout<<"Token: '"<<line[i]<<"'\tType: Punctuator\n";
                i++;
                continue;
            }

            if(isdigit((unsigned char)line[i])){
                string num;
                while(i<line.length() && (isdigit((unsigned char)line[i]) || line[i]=='.')) num += line[i++];
                if(isNumber(num)) cout<<"Token: '"<<num<<"'\tType: Constant\n";
                else cout<<"Token: '"<<num<<"'\tType: Unknown\n";
                continue;
            }

            if(isValidIdentifierStart(line[i])){
                string word;
                while(i<line.length() && isValidIdentifierChar(line[i])) word += line[i++];
                if(isKeyword(word)) cout<<"Token: '"<<word<<"'\tType: Keyword\n";
                else if(isPredefined(word)) cout<<"Token: '"<<word<<"'\tType: Predefined Identifier\n";
                else if(declared.count(word)) cout<<"Token: '"<<word<<"'\tType: Identifier\n";
                else cout<<"Token: '"<<word<<"'\tType: Unknown\n"; // <-- here unseen-gibberish becomes Unknown
                continue;
            }

            cout<<"Token: '"<<line[i]<<"'\tType: Unknown\n";
            i++;
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // prepare sets
    for(auto &k: keywords) kwset.insert(k);
    for(auto &p: predefinedIdentifiers) predefset.insert(p);

    // read whole file from stdin (input.c)
    freopen("input.c","r",stdin);
    stringstream buffer; buffer<<cin.rdbuf();
    string code = buffer.str();

    // PASS 1: collect declared identifiers (heuristic)
    auto declared = collectDeclaredIdentifiers(code);

    // PASS 2: tokenize and classify using declared + predefined + keywords
    tokenize(code, declared);

    // (optional) print collected declared identifiers to debug
    // cout << "---- Declared (collected) names ----\n";
    // for(auto &n: declared) cout<<n<<"\n";

    return 0;
}
