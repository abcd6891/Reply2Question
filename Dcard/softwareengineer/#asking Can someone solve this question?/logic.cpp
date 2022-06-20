/*
https://ideone.com/Zf6TGF

#How to run
1. run it on ideone
2. or compile it with g++

#stdin
3
5, 6
1, 2, 4
xyz'+xy'z

#stdout
請輸入變數個數: 
請輸入 minterms: 
請輸入 don't care terms: 
請輸入小題確認是否為一組解: 
是

#stderr
numVar 3
min terms:
 5
 6
d terms:
 1
 2
 4
expression: xyz'+xy'z
normalizedExpr: x*y*z'+x*y'*z
postOrderExpr: xy*z'*xy'*z*+
arg 0, exprConst: 00*0'*00'*0*+
value 0

arg 3, exprConst: 01*1'*01'*1*+
value 0

arg 5, exprConst: 10*1'*10'*1*+
value 1

arg 6, exprConst: 11*0'*11'*0*+
value 1

arg 7, exprConst: 11*1'*11'*1*+
value 0


*/
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<set>
#include<sstream>
#include<vector>
#include<stack>

using namespace std;

#define MYDEBUG


// DIY: more than 3 vars? more than 31 vars?
static const string IDX2VAR = "xyz";
char getVar(int index, int numVar) {
    return IDX2VAR[numVar - 1 - index];
}
// DIY: inverse map<char, int> / speedup
int getVarIdx(char v, int numVar) {
    for (int i=0; i<IDX2VAR.size(); i++)
        if (IDX2VAR[i] == v)
            return numVar - 1 - i;
    return -1;
}

void commaSplitStringToNums(const string &str, vector<int> &nums) {
    istringstream in(str);
    int tmp;
    while (in >> tmp) {
        nums.push_back(tmp);
        while (in.peek() == ',' || in.peek() == ' ') {
            in.ignore();
        }
    }
}

void readLineToNums(vector<int> &nums) {
    string line;
    getline(cin, line);
    commaSplitStringToNums(line, nums);
}

int getOpPriority(const char c) {
    switch (c) {
        case '\'':
            return 2;
        case '*':
            return 1;
        case '+':
            return 0;
    }
    return -1;
}

string getPostOrder(const string &inOrder, int numVar) {// DIY: remove numVar
    ostringstream out;
    stack<char> S;

    for (const char c: inOrder) {
        if (getVarIdx(c, numVar) != -1) {
            out << c;
            continue;
        }
        if (c == '(') {
            S.push(c);
            continue;
        }
        if (c == ')') {
            while (S.top() != '(') {
                out << S.top();
                S.pop();
            }
            S.pop();
            continue;
        }

        while (S.size() && getOpPriority(S.top()) >= getOpPriority(c)) {
            out << S.top();
            S.pop();
        }
        S.push(c);
    }

    while(S.size()) {
        out << S.top();
        S.pop();
    }
    return out.str();
}

int evalPostOrder(const string &exprConst) {
    stack<int> S;
    
    for (const char &c: exprConst) {
        if (c == '0') {
            S.push(0);
            continue;
        }
        if (c == '1') {
            S.push(1);
        }
        if (c == '\'') {
            int oprand = S.top();
            S.pop();
            S.push(1-oprand);
            continue;
        }
        if (c == '*' || c == '+') {
            int oprand1 = S.top();
            S.pop();
            int oprand2 = S.top();
            S.pop();
            if (c == '*') {
                S.push(oprand1 & oprand2);
            } else {
                S.push(oprand1 | oprand2);
            }
            continue;
        }
    }
    return S.top();
}

class Qustion {
private:
    int numVar;
    set<int> minTerms;
    set<int> dontCares;
    string expr;

    void readVarNum() {
        cout << "請輸入變數個數: " << endl;
        cin >> numVar;
    }
    void readMinTerms() {
        // DIY: check input with numVar
        cout << "請輸入 minterms: " << endl;

        // DIY: flush stdin will be better?
        while(cin.peek() == '\n')
            cin.ignore();

        vector<int> nums;
        readLineToNums(nums);
        minTerms = set<int>(nums.begin(), nums.end());
    }
    void readDontCatreTerms() {
        cout << "請輸入 don't care terms: " << endl;
        vector<int> nums;
        readLineToNums(nums);
        dontCares = set<int>(nums.begin(), nums.end());
    }
    void readExpression() {
        cout << "請輸入小題確認是否為一組解: " << endl;
        // DIY: flush stdin will be better?
        while(cin.peek() == '\n')
            cin.ignore();
        getline(cin, expr);
    }
#ifdef MYDEBUG
    void printQuestion() {
        cerr << "numVar " << numVar << endl;
        cerr << "min terms:" << endl;
        for(const int &n: minTerms)
            cerr << " " << n << endl;
        cerr << "d terms:" << endl;
        for(const int &n: dontCares)
            cerr << " " << n << endl;
        cerr << "expression: " << expr << endl;
    }
#endif

    string getNormalizedExpr() {
        ostringstream out;
        // add * between 2 continues tokens
        // 1. vars
        // 2. var1' var2
        // 3. think () as a "var"

        bool lastIsVar = false;

        for (const char &c: expr) {
            if (c == ' ') { // DIY: skip other delm
                continue;
            }

            bool beginOfVar = false;
            bool newLastIsVar = false;

            if (getVarIdx(c, numVar) != -1) {
                beginOfVar = true;
                newLastIsVar = true;
            } else if (c == '(') {
                beginOfVar = true;
            } else if (c == '\'') {
                newLastIsVar = true;
            }

            if (lastIsVar && beginOfVar)
                out << '*';
            out << c;
            lastIsVar = newLastIsVar;
        }

        return out.str();
    }

    string fillVar(int arg, const string &expr) {
        ostringstream out;
        for (const char c: expr) {
            int idx = getVarIdx(c, numVar);
            if (-1 == idx) {
                out << c;
            } else {
                out << ((arg >> idx)&1);
            }
        }
        return out.str();
    }

public:
    Qustion() {
        numVar = 0;
    }

    void read() {
        readVarNum();
        readMinTerms();
        readDontCatreTerms();
        readExpression();
#ifdef MYDEBUG
        printQuestion();
#endif
    }

    bool solve() {
        string normalizedExpr = getNormalizedExpr(); // add *
        string postOrderExpr = getPostOrder(normalizedExpr, numVar);
#ifdef MYDEBUG
        cerr << "normalizedExpr: " << normalizedExpr << endl;
        cerr << "postOrderExpr: " << postOrderExpr << endl;
#endif

        // enum all combination
        // DIY: more than 31 vars
        const int UB = 1 << numVar; // pow of 2
        for (int arg=0; arg<UB; arg++) {
            if (dontCares.find(arg) != dontCares.end()) {
                // ignore don't care
                continue;
            }
            // DIY: get result without keep exprConst, speedup?
            string exprConst = fillVar(arg, postOrderExpr);
            int value = evalPostOrder(exprConst);
#ifdef MYDEBUG
            cerr << "arg " << arg << ", " << "exprConst: " << exprConst << endl;
            cerr << "value " << value << endl << endl;
#endif
            if (value) {
                if (minTerms.find(arg) == minTerms.end())
                    return false;
            } else {
                if (minTerms.find(arg) != minTerms.end())
                    return false;
            }
        }
        return true;
    }
};

int main() {
    Qustion q;
    q.read();
    if (q.solve()) {
        cout << "是" << endl;
    } else {
        cout << "否" << endl;
    }
    return 0;
}