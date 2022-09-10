#include "Vector.h";
const int EQ_STR_MAX = 30;
const int POSTFIX_CHAR_MAX = 60;

char hexdigits[17]  = "0123456789ABCDEF"; 
char* rev(char *str, int len)
{
    int i;
    for (i = len/2-1 ; i >= 0 ; --i) {
        char c = str[i];
        str[i] = str[len-i-1];
        str[len-i-1] = c;
    }
    return str;
}
String toBinaryStr(int64_t n)
{
    String r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}
String toHexStr(int64_t num) {
    if (num == 0){return "0";}
    String test = "";
    while (num != 0) {
        test+=(hexdigits[num & 0xF]);
        num = num >> 4;
    }
	test+="x0";
    return rev((char*)test.c_str(),test.length());
}

int precedence(char op) {
    switch (op) {
        case '|':return -3;
        case 'o':return -2; //exclusive or
        case '&':return -1;
        case 'l':return 0; //shift left
        case 'r':return 0; //shift right
        case '-':return 2;
        case '+':return 2;
        case '/':return 3;
        case '*':return 3;
        case '~':return 5; //bitwise not
        case '^':return 5;
        case 'm': return 5; //unary minus
        case 'p': return 5; //unary plus
    }
    return -1;
}
char associativity(char op) {
    //L : left associativity R: right associativity
    switch (op) {
        case '|':return 'L';
        case 'o':return 'L';
        case '&':return 'L';
        case 'l':return 'L';
        case 'r':return 'L';
        case '-':return 'L';
        case '+':return 'L';
        case '/':return 'L';
        case '*':return 'L';
        case '~':return 'R';
        case '^':return 'R';
        case 'm':return 'R'; //unary minus
        case 'p':return 'R'; //unary plus
    }
    return '.';
}

bool isoperator(char op) {
    //used to identify unary operators. A unary operator appears just after the following op.
    switch (op) {
        case '|':return 1;
        case 'o':return 1;
        case '&':return 1;
        case 'l':return 1;
        case 'r':return 1;
        case '-':return 1;
        case '+':return 1;
        case '/':return 1;
        case '*':return 1;
        case '^':return 1;
        case '(':return 1;
    }
    //Note a minus/plus following ) is not a unary minus/plus
    return 0;
}

String modify(String s) {
    //Remove all spaces from s
    //Replace unary plus with p
    //Replace unary minus with m
    //add * before ( if there's no operator
    s.replace(" ", "");
    s.replace("<<", "l");
    s.replace(">>", "r");
    s.replace("o^", "o");
    //Replace unary plus with p
   //Replace unary minus with m
    for (int i = 1; i < s.length(); i++) {
        if (isoperator(s[i - 1]) && (s[i] == '-' || s[i] == 'm')) {
            s[i] = 'm';
        }
        if (isoperator(s[i - 1]) && (s[i] == '+' || s[i] == 'p')) {
            s[i] = 'p';
        }
    }
    if (s[0] == '-') { s[0] = 'm'; }
    if (s[0] == '+') { s[0] = 'p'; }

    //add * before ( if there's no operator
    String ans = "";
    for (int i = 0; i < s.length() - 1; i++) {
        if (s[i] >= '0' && s[i] <= '9' && s[i + 1] == '(') {
            ans += s[i]; ans += '*';
        }
        else {
            bool add = true;
            if (i > 0) {
                if (s[i] == ')' && s[i + 1] >= '0' && s[i + 1] <= '9') {
                    ans += ')';
                    ans += '*';
                    add = false;
                }
            }
            if(add){
                ans += s[i];
            }
        }
    }
    ans += s[s.length() - 1];
    return ans;
}

String to_postfix(String exp) {
    //Implementation of shunting yard algorithm.
    exp = modify(exp);
    char storage_array[POSTFIX_CHAR_MAX];
    Vector<char> operators(storage_array);
    String ans = "";
    String operand = "";
    char prevchar = '*';

    for (int i = 0; i < exp.length(); i++) {

        if (i > 0) { prevchar = exp[i - 1]; }
        char c = exp[i];

        if (isdigit(c) != 0 || c == '.') { // if c is a digit or a decimal point
            operand += c;
        }
        else {// c is an operator 

            //add operand to ans
            if (operand != "") {
                ans += operand + " ";
                operand = "";
            }
            if (operators.size() == 0 || c == '(') {
                operators.push_back(c);
            }
            else { //operator stack not empty
                char top = operators.back(); //operator at top of operator stack
                if (c == ')') {
                    while (top != '(') {
                        ans += top; ans += " ";
                        operators.pop_back();
                        top = operators.back();
                    }
                    operators.pop_back(); //pop the (
                }
                else {
                    if (top == '(' || (associativity(c) == 'R' && (precedence(c) >= precedence(top)))) {
                        operators.push_back(c);
                    }
                    else {
                        if (associativity(c) == 'L') {
                            while (precedence(c) <= precedence(top) && operators.size() != 0 && top != '(') {
                                ans += top; ans += " ";
                                operators.pop_back();
                                if (operators.size() == 0) { break; }
                                top = operators.back();
                            }
                            operators.push_back(c);
                        }
                    }
                }
            }

        }
    }

    if (operand != "") {
        ans += operand + " ";
    }
    if (operators.size() != 0) {
        for (int i = operators.size() - 1; i > -1; i--) {
            ans += operators[i]; ans += " ";
        }
    }

    return ans;
}

int64_t power (int64_t a, int64_t b) { 
    int64_t tm=b; 
    int64_t ans=1ll; 
    while (tm > 0){
      ans*=a;
      tm--;
    }
    return ans; 
    } 
 

int64_t result(int64_t a, int64_t b, String o) {
    //result(5.2, 1.1, "-") returns 4.1
    if (o == "+") { return a + b; }
    if (o == "-") { return a - b; }
    if (o == "*") { return a * b; }
    if (o == "/") { return a / b; }
    if (o == "^") { 
      return power(a, b); 
      }
    if (o == "|") { return (a | b); }
    if (o == "o") { return (a ^ b); }
    if (o == "&") { return (a & b); }
    if (o == "l") { return (a << b); }
    if (o == "r") { return (a >> b); }
    return 0;
}

void strTokenExtract(String str, Vector<String> &out) {
    String temp = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            out.push_back(temp);
            temp = "";
        }
        else {
            temp += str[i];
        }
    }
}

char * _print_base(char *buf, uint64_t n, int base) {
  char *str = &buf[(64+1) - 1]; // handle printing bits

  *str = '\0'; // work from least significant digit to most
  do {
    unsigned digit = n % base;
    *--str = digit < 10 ? digit + '0' : digit + 'A' - 10;
  } while ( (n /= base) );

  return (str);
};

char * printll(char *work, int64_t ldata, int base = DEC)
{
  char *ptr = work;
  if ( base == DEC && ldata < 0 ) {
    ldata = -ldata;
    ptr = _print_base(&ptr[1], (uint64_t)ldata, base);
    *--ptr = '-';
  }
  else {
    ptr = _print_base(ptr, (uint64_t)ldata, base);
  }
  return ptr;
}

int64_t atoll (const char *s, int64_t base)
{
    int64_t val = 0;
    bool neg = false;
    unsigned i = 0;
    if(s[0] == '-'){
      s = s + 1;
      neg=true;
    }
    for (; i < strlen (s); i++)
        val = base*val + s [i] - '0';
    return neg ? val*-1 : val;
}

int64_t EvaluateExpression(String exp) {
    String postfix = to_postfix(exp);
    String storage_array[EQ_STR_MAX];
    Vector<String> out(storage_array);
    strTokenExtract(postfix,out);
    //Note : unary plus is always ignored
    int64_t storage_array2[32];
    Vector<int64_t> Operand(storage_array2);
    for (auto i : out) {
        if (i == "m" || i == "~") {
            int64_t a = Operand.back();
            Operand.pop_back();
            if (i == "m") { //unary minus  
                int64_t b = a * -1ll;
                Operand.push_back(b);
            }
            else { //bitwise not
                Operand.pop_back();
                Operand.push_back(~a);
            }
        }
        else {
            if (i != "(*)" && isoperator(i[0])) {
                int64_t b = Operand.back();
                Operand.pop_back();
                int64_t a = Operand.back();
                Operand.pop_back();
                Operand.push_back(result(a, b, i));
            }
            else {
                int64_t tmp = atoll(i.c_str(),10);
                Operand.push_back(tmp);
            }
        }
    }
    return Operand.back();
}

