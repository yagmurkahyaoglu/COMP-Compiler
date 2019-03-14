//cmpe230 assigment1
//this project is written by Elif CALISKAN, Yagmur KAHYAOGLU
//2016400183-2015400057
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <stack>
#include <string>

using namespace std;

void term(string expr,stack<string>& postf,int numLine);
void moreterms(string expr,stack<string>& postf,int numLine);
void morefactors(string expr,stack<string>& postf,int numLine);
void factor(string expr,stack<string>& postf,int numLine);
void addition(int counter);
void power(int& counter);
void multiplication(int numLine);
void assign();
//used for printing an error message. It takes an integer parameter which is the line number
void printError(int numLine){
    cout<<"code segment"<<endl;
    cout<<"MOV BX,MSG"<<endl;
    string x=""+to_string(numLine);
    int a=x.size()+19;
    cout<<"MOV CX,"<<a<<"d"<<endl;
    cout<<"MOV AH,02h"<<endl;
    cout<<"MORE:"<<endl;
    cout<<"MOV DL,[BX]"<<endl;
    cout<<"int 21h"<<endl;
    cout<<"inc BX"<<endl;
    cout<<"dec CX"<<endl;
    cout<<"JNZ MORE"<<endl;
    cout<<"int 20h"<<endl;
    cout<<"MSG:"<<endl;
    cout<<"DB 'Line "<<numLine<<": Syntax error'"<<endl;
    cout<<"code ends"<<endl;
}
//expression method is used in infixToPostfix method. It takes a string, a stack and an integer as parameters
//expr is used for the expression, postf is the main stack which keeps the posfix representation, numLine is the line number
//this method splits the expression into two sections: term and moreterms.
void expression(string expr,stack<string>& postf,int numLine){
    size_t pos = expr.find("+");
    int index=0;
    if(pos!=string::npos)
        index=pos;
    else{
        term(expr,postf,numLine);
        return;
    }
    bool test=true;
    int openpar = 0;
    for (int i = 0; i <= index; i++) { //hata verebilir bak
        if (expr[i] == ('(')) {
            openpar++;
        }
        if (expr[i] == (')')) {
            openpar--;
        }
    }
    while(test) {
        openpar = 0;
        for (int i = 0; i <= index; i++) {
            if (expr[i] == ('(')) {
                openpar++;
            }
            if (expr[i] == (')')) {
                openpar--;
            }
        }
        if(openpar==0) {
            term(expr.substr(0, index), postf,numLine);
            moreterms(expr.substr(index+1), postf,numLine);
            test=false;
        }
        else{
            string newstr=expr.substr(index + 1);
            size_t pos2 = newstr.find("+");
            if(pos2!=string::npos) {
                index += pos2+1;
            }
            else{
                term(expr,postf,numLine);
                test=false;
            }
        }
    }
}
//term method is used in infixToPostfix method. It takes a string, a stack and an integer as parameters
//expr is used for the expression, postf is the main stack which keeps the posfix representation, numLine is the line number
//this method splits the term into two sections: factor and morefactors.
void term(string expr,stack<string>& postf,int numLine){
    size_t pos = expr.find("*");
    int index=0;
    if(pos!=string::npos)
        index=pos;
    else{
        factor(expr,postf,numLine);
        return;
    }
    bool test=true;
    while(test) {
        int openpar = 0;
        for (int i = 0; i <= index; i++) {
            if (expr[i] == ('(')) {
                openpar++;
            }
            if (expr[i] == (')')) {
                openpar--;
            }
        }
        if(openpar==0) {
            factor(expr.substr(0, index), postf,numLine);
            morefactors(expr.substr(index + 1), postf,numLine);
            test=false;
        }
        else{
            string newstr=expr.substr(index + 1);
            size_t pos2 = newstr.find("*");
            if(pos2!=string::npos) {
                index += pos2+1;
            }
            else{
                factor(expr,postf,numLine);
                test=false;
            }
        }
    }
}
//moreterms method is used in infixToPostfix method. It takes a string, a stack and an integer as parameters
//expr is used for the expression, postf is the main stack which keeps the posfix representation, numLine is the line number
//this method splits the term into two sections: term and moreterms. if there is no + sign in expr,it calls term method
void moreterms(string expr,stack<string>& postf,int numLine){
    if(expr=="")
        return;
    size_t found = expr.find("+");
    if(found==string::npos){
        term(expr, postf,numLine);
        postf.push("+");
    }
    if(found!=string::npos){
        int index=found;
        bool test=true;
        while(test) {
            int openpar = 0;
            for (int i = 0; i <= index; i++) {
                if (expr[i] == ('(')) {
                    openpar++;
                }
                if (expr[i] == (')')) {
                    openpar--;
                }
            }
            if(openpar==0) {
                term(expr.substr(0, index), postf,numLine);
                postf.push("+");
                moreterms(expr.substr(index + 1), postf,numLine);
                test=false;
            }
            else{
                string newstr=expr.substr(index + 1);
                size_t pos2 = newstr.find("+");
                if(pos2!=string::npos) {
                    index += pos2+1;
                }
                else{
                    term(expr,postf,numLine);
                    postf.push("+");
                    test=false;
                }
            }
        }
    }
}
//morefactors method is used in infixToPostfix method. It takes a string, a stack and an integer as parameters
//expr is used for the expression, postf is the main stack which keeps the posfix representation, numLine is the line number
//this method splits the term into two sections: factor and morefactors. if there is no * sign in expr,it calls term method
void morefactors(string expr,stack<string>& postf,int numLine){
    if(expr==""){
        return;
    }
    size_t found = expr.find("*");
    if(found==string::npos){
        factor(expr, postf,numLine);
        postf.push("*");

    }
    if(found!=string::npos){
        int index=found;
        bool test=true;
        while(test) {
            int openpar = 0;
            for (int i = 0; i <= index; i++) {
                if (expr[i] == ('(')) {
                    openpar++;
                }
                if (expr[i] == (')')) {
                    openpar--;
                }
            }
            if(openpar==0) {
                factor(expr.substr(0, index), postf,numLine);
                postf.push("*");
                morefactors(expr.substr(index+1), postf,numLine);
                test=false;
            }
            else{
                string newstr=expr.substr(index+1);
                size_t pos2 = newstr.find("*");
                if(pos2!=string::npos) {
                    index += pos2+1;
                }
                else{
                    factor(expr,postf,numLine);
                    postf.push("*");
                    test=false;
                }
            }
        }
    }
}
//factor method is used in infixToPostfix method. It takes a string, a stack and an integer as parameters
//expr is used for the expression, postf is the main stack which keeps the posfix representation, numLine is the line number
//if expr has two paranthesis around it, factor calls the expression method
// else, it means expr is id, num or pow so it pushes expr to stack
void factor(string expr,stack<string>& postf,int numLine){
    if((expr[0]=='(')&&expr[expr.size()-1]==')'){
        expression(expr.substr(1,expr.size()-2),postf,numLine);
    }
    else {
        postf.push(expr);
    }
}
//this method is used for converting an infix expression into a postfix expression
//this method takes a string, a vector and an integer
//expr is used for the whole expression in line, vals is the vector of known variables, numLines is the line number
//this method converts the stack by calling the expression method and recursive process starts.
//after converting into postfix, if it starts with any char other than a letter, it pushes the real value,
// if str starts with a letter, the method adds 'a' at the end if the corresponding char is lowercase
//if the corresponding letter is uppercase, the method adds 'z' at the end because the code should be case sensitive,
//then it checks if the given variable is used before; if it is not used, it replaces the variable with "0"
stack<string> infixToPostfix(string expr,vector<string> vals,int numLine){ //boşluk kontrol error kontrol
    stack<string> postf = *new stack<string> ;
    expression(expr,postf,numLine);
    stack<string> postfine = *new stack<string> ;
    int size=postf.size();
    for(int i=0;i<size;i++){
        string str = postf.top();
        postf.pop();
        size_t po=str.find("pow");
        if(((str[0]<='z'&&str[0]>='a')||(str[0]<='Z'&&str[0]>='A'))&&po==string::npos){
            bool found=false;
            int size=str.size();
            for(int k=0;k<size;k++){
                if(str[k]<='z'&&str[k]>='a'){
                    str+="a";
                }
                else if(str[k]<='Z'&&str[k]>='A'){
                    str+="z";
                }
            }
            for(int a=0;a<vals.size();a++){
                if(vals[a]==str){
                    found=true;
                }
            }
            if(!found){
                str="0";
            }
        }
        postfine.push(str);
    }
    return postfine;
}
//this method is used for converting the postfix expression into assembly commands
//it takes a stack, a vector and three integers
//expr is used for postfix expression, vals is vector of used variable strings, numLine is the line number and
// m is a label for understanding if it is called from power or not, if we call from power m's value is 0
// we use m value in an if statement and if it is zero we don't call the assign function
//counter is used for eliminating the duplication of labels
void toAssembly(stack<string>& expr,vector<string>& vals,int numLine,int m,int& counter){
    while(expr.size()!=0){
        string str=expr.top();
        expr.pop();
        size_t pos1 = str.find("*");
        size_t pos2 = str.find("+");
        size_t pos3 = str.find("pow");
        if(pos1!=string::npos){
            multiplication(counter);
            counter++;
        }
        else if(pos2!=string::npos){
            addition(counter);
            counter++;
        }
        else if(pos3!=string::npos){
            str = str.substr(4,str.size()-5);
            bool test=true;
            int openpar=0;
            string str1;
            string str2;
            size_t pos=str.find(",");
            int index=pos;
            while(test) {
                openpar = 0;
                for (int i = 0; i <= index; i++) {
                    if (str[i] == ('(')) {
                        openpar++;
                    }
                    if (str[i] == (')')) {
                        openpar--;
                    }
                }
                if(openpar==0) {
                    str1=str.substr(0, index);
                    str2=str.substr(index+1);
                    test=false;
                }
                else{
                    string newstr=str.substr(index + 1);
                    size_t posk = newstr.find(",");
                    if(posk!=string::npos) {
                        index += posk+1;
                    }
                    else{
                        test=false;
                    }
                }
            }
            stack<string> news1=infixToPostfix(str1,vals,numLine);
            toAssembly(news1,vals,numLine,0,counter);
            counter++;
            stack<string> news2=infixToPostfix(str2,vals,numLine);
            toAssembly(news2,vals,numLine,0,counter);
            counter++;
            power(counter);
            counter++;
        }
        else{
            bool isvar=false;
            for(int i=0;i<vals.size();i++){
                if(vals[i]==str){
                    isvar=true;
                }
            }
            if(isvar){
                cout<<"PUSH "<<str<<"high"<<endl; //yer
                cout<<"PUSH "<<str<<"low"<<endl;  //değiştirdim
            }
            else{
                int size=str.size();
                for(int a=0;a<8-size;a++){
                    str="0"+str;
                }
                cout<<"PUSH "<<"0"<<str.substr(0,4)<<"h"<<endl;
                cout<<"PUSH "<<"0"<<str.substr(4)<<"h"<<endl;
            }
        }
    }
    if(m==1) {
        assign();
    }
    counter++;
}
//this method is used for addition
//it adds the two low values first and adds the carry to addition of high values
//it takes an integer as parameter
//counter is used for eliminating the duplication of labels
void addition(int counter){
    cout<<"POP CX"<<endl; //üst1
    cout<<"POP DX"<<endl; //alt1
    cout<<"POP AX"<<endl; //üst2
    cout<<"POP BX"<<endl; //alt2
    cout<<"ADD AX,CX"<<endl; //alt3
    cout<<"JC CARRY"<<counter<<endl;
    cout<<"DONECARRY"<<counter<<":"<<endl; //   EMİN DEĞİLİZ KONTROL!!!
    cout<<"ADD BX,DX"<<endl; //üst3
    cout<<"PUSH BX"<<endl; //alt3
    cout<<"PUSH AX"<<endl; //üst3
    cout<<"JMP CARRYON"<<counter<<endl;
    cout<<"CARRY"<<counter<<":"<<endl;
    cout<<"ADD BX,1"<<endl;
    cout<<"JMP DONECARRY"<<counter<<endl;
    cout<<endl;
    cout<<"CARRYON"<<counter<<":"<<endl;

}
//this method is used for multiplication
//it takes one integer parameter. numLine is used for changing the label names and eliminating the duplicates.
//this method multiplies the two low values first. It multiplies the first low value with the second high value and the first
//high value with the second low value. Then it adds the carry of the first multiplication to the high values.

void multiplication(int numLine){
    cout<<"POP AX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"POP DX"<<endl;
    cout<<"POP CX"<<endl;
    cout<<"PUSH BX"<<endl;
    cout<<"PUSH DX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"PUSH CX"<<endl;
    cout<<"MUL DX"<<endl;
    cout<<"PUSH DX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"POP CX"<<endl;
    cout<<"POP AX"<<endl;
    cout<<"POP DX"<<endl;
    cout<<"MUL DX"<<endl;
    cout<<"ADD CX,AX"<<endl;
    cout<<"POP AX"<<endl;
    cout<<"POP DX"<<endl;
    cout<<"MUL DX"<<endl;
    cout<<"ADD CX,AX"<<endl;
    cout<<"PUSH CX"<<endl;
    cout<<"PUSH BX"<<endl;
}
//this method is used to take the given power of an expression
//it checks if the power or the base part is 0 or 1 and pushes the right values accordingly
//else it multiplies the base part power times with a loop using the same algorithm used in the multiplication method
void power(int& counter){
    counter++;
    cout<<"POP CX"<<endl;
    cout<<"POP DX"<<endl;
    cout<<"POP AX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"CMP DX,0"<<endl;
    cout<<"JZ MIDDLE1"<<counter<<endl;
    cout<<"JMP MIDDLE2"<<counter<<endl;
    cout<<"MIDDLE1"<<counter<<":"<<endl;
    cout<<"CMP CX,0"<<endl;
    cout<<"JZ OUT2"<<counter<<endl;
    cout<<"CMP CX,1"<<endl;
    cout<<"JZ OUT3"<<counter<<endl;
    cout<<"JMP MIDDLE2"<<counter<<endl;
    cout<<"MIDDLE2"<<counter<<":"<<endl;
    cout<<"CMP AX,0"<<endl;
    cout<<"JZ OUT1"<<counter<<endl;
    cout<<"CMP AX,1"<<endl;
    cout<<"JZ OUT2"<<counter<<endl;
    cout<<"SUB CX,1"<<endl;
    cout<<"PUSH BX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"FOR"<<counter<<":"<<endl;
    cout<<"PUSH BX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"POP AX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"POP DX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"MUL DX"<<endl;
    cout<<"MOV DX,BX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"PUSH BX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"MOV AX,BX"<<endl;
    cout<<"MUL DX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"ADD DX,BX"<<endl;
    cout<<"ADD DX,BX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"PUSH DX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"MOV AX,BX"<<endl;
    cout<<"MOV BX,0"<<endl;
    cout<<"CONT"<<counter<<":"<<endl;
    cout<<"LOOP FOR"<<counter<<endl;
    cout<<"JMP DEVAM"<<counter<<endl;
    cout<<"OUT1"<<counter<<":"<<endl;
    cout<<"PUSH 0"<<endl;
    cout<<"PUSH 0"<<endl;
    cout<<"JMP DEVAM"<<counter<<endl;
    cout<<"OUT2"<<counter<<":"<<endl;
    cout<<"PUSH 0"<<endl;
    cout<<"PUSH 1"<<endl;
    cout<<"JMP DEVAM"<<counter<<endl;
    cout<<"OUT3"<<counter<<":"<<endl;
    cout<<"PUSH BX"<<endl;
    cout<<"PUSH AX"<<endl;
    cout<<"JMP DEVAM"<<counter<<endl;
    cout<<"DEVAM"<<counter<<":"<<endl;
    counter++;

}
//this method makes the assignment operation
//in this method the addresses of high and low values are assigned in bx register
void assign(){
    cout<<"POP AX"<<endl;
    cout<<"POP CX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"MOV [BX],AX"<<endl;
    cout<<"POP BX"<<endl;
    cout<<"MOV [BX],CX"<<endl;
}
//this method is used for printing the output
//it takes one string and two intergers
//val is the name of wanted variable, numLine is the line number, a is used for putting different names for high and low labels
void print(string val, int numLine,int a){
    cout<<"PUSH "<<val<<endl;
    cout<<"POP BX"<<endl;
    cout<<"MOV CX,4h"<<endl;
    cout<<"MOV AH,2h"<<endl;
    cout<<"LOOP1"<<numLine<<a<<":"<<endl;
    cout<<"MOV DX,0fh"<<endl;
    cout<<"ROL BX,4h"<<endl;
    cout<<"AND DX,BX"<<endl;
    cout<<"CMP DL,0ah"<<endl;
    cout<<"JAE HEXDIGIT"<<numLine<<a<<endl;
    cout<<"ADD DL,'0'"<<endl;
    cout<<"JMP OUTPUT"<<numLine<<a<<endl;
    cout<<"HEXDIGIT"<<numLine<<a<<":"<<endl;
    cout<<"ADD DL,'A'"<<endl;
    cout<<"SUB DL,0ah"<<endl;
    cout<<"OUTPUT"<<numLine<<a<<":"<<endl;
    cout<<"INT 21h"<<endl;
    cout<<"DEC CX"<<endl;
    cout<<"JNZ LOOP1"<<numLine<<a<<endl;
    cout<<"ENDLOOP2"<<numLine<<a<<":"<<endl;
    cout<<"MOV AX,0\n"<<endl;
}
//this method updates the epression string, finds if there is an error and returns a string
//it takes a string, an integer and a bool
//str is the expression which will be updated, numLine is the line number, error is the bool which will be true if there is an error
string updateStr(string& str,int numLine,bool& error){
    string newstr="";
    int lastOne=0;
    int now=0;
    int lastIndex=0;
    for(int i=0;i<str.size();i++){
        if(str[i]!=' '){
            if((str[i]=='+')||str[i]=='*'){
                now=2;
            }
            else if(str[i]=='('){
                now=3;
            }
            else if(str[i]==')'){
                now=4;
            }
            else if(str[i]==','){
                now=5;
            }
            else{
                now=1;
            }
            if(lastOne==0){
                if(now==2||now==4||now==5){
                    printError(numLine);
                    error=true;
                }
            }
            else if(lastOne==1){ //pow sorun
                if(now==3){
                    if(str[lastIndex]!='w'){
                        printError(numLine);
                        error=true;
                    }
                }
                else if(str[i-1]==' '&&now==1){
                    printError(numLine);
                    error=true;
                }
            }
            else if(lastOne==2){
                if(now==2||now==5){
                    printError(numLine);
                    error=true;
                }
            }
            else if(lastOne==3){
                if(now==2||now==4||now==5){
                    printError(numLine);
                    error=true;
                }
            }
            else if(lastOne==4){
                if(now==1||now==3){
                    printError(numLine);
                    error=true;
                }
            }
            else{
                if(now==2||now==4||now==5){
                    printError(numLine);
                    error=true;
                }
            }
            lastOne=now;
            lastIndex=i;
            newstr+=str[i];
        }
    }
    if(lastOne==2||lastOne==3||lastOne==5){
            error=true;
    }
    int openpar = 0;
    for (int i = 0; i <newstr.size(); i++) {
        if (newstr[i] == ('(')) {
            openpar++;
        }
        if (newstr[i] == (')')) {
            openpar--;
        }
    }
    if(openpar!=0){
        //printError(numLine);
        error=true;
    }
    return newstr;
}
//this is the main method; it takes 4 argv values, first one is the name of program,second one is the path of input file
//third one is the path of output file, fourth one is the path of temp file
//it starts reading the input file it looks for the “=” sign if there is an assignment it means it is not
// looking for an output value, so it needs to evaluate the expression
// it splits the line into two sections first one is the variable, second one is the expression, it updates those strings,
// if there is no assignment, it should print the output value. After writing outputhigh and outputlow values, it prints one empty line.
// if there is an assignment, it declares these variables by first adding 'a's or 'z's at the end of the variable name and
// then adding low and high at the end. then it pushes them to the stack in assembly code.
// it converts the expression into postfix and if there is no error, it calls toAssembly method to compute the variable’s value.
int main(int argc, char* argv[]) {
    ifstream infile(argv[1]);
    int numLine=0;
    string line;
    int counter=0;
    getline(infile, line);
    vector<string> vals;
    bool error = false;
    int errorNum;
    freopen(argv[3], "w+", stdout);
    while(line!=" ") {
        numLine++;
        if (line!= "") {
            string x1;
            string x2;
            size_t ass = line.find("=");
            if (ass != string::npos) {
                x1 = line.substr(0, ass);
                x2 = line.substr(ass + 1);
                x1 = updateStr(x1, numLine, error);
                x2 = updateStr(x2, numLine, error);
                if (error) {
                    errorNum=numLine;
                    break;
                }
            } else {
                x2 = updateStr(line, numLine, error);
                if (error) {
                    errorNum=numLine;
                    break;
                }
            }
            if (ass == string::npos) {
                updateStr(x2,numLine,error);
                stack<string> a = infixToPostfix(x2, vals, numLine);
                string first=a.top();
                bool found = false;
                size_t posp=first.find("pow");
                if (a.size() == 1&&posp==string::npos&&found) {

                    int size=x2.size();
                    for(int k=0;k<size;k++){
                        if(x2[k]<='z'&&x2[k]>='a'){
                            x2+="a";
                        }
                        else if(x2[k]<='Z'&&x2[k]>='A'){
                            x2+="z";
                        }
                    }
                    for (int i = 0; i < vals.size(); i++) {
                        if (vals[i] == x1)
                            found = true;
                    }
                    print(x2 + "high", numLine, 1);
                    print(x2 + "low", numLine, 2);
                    cout<<"MOV DL,10"<<endl;
                    cout << "MOV AH,02h" << endl;
                    cout << "int 21h" << endl;
                } else {
                    cout<<"PUSH offset " << "alph" <<counter<< "high"<<endl;
                    cout << "PUSH offset " <<"alph" <<counter<< "low" << endl;
                    string val ="alph"+to_string(counter);
                    vals.push_back(val);
                    toAssembly(a, vals, numLine,1,counter);
                    counter++;
                    string valh=val+"high";
                    string vall=val+"low";
                    print(valh, numLine, 1);
                    print(vall, numLine, 2);
                    cout << "MOV DL,10" << endl;
                    cout << "MOV AH,02h" << endl;
                    cout << "int 21h" << endl;
                }

            } else {
                stack<string> a = *new stack<string>;
                //a = infixToPostfix(x1, vals, numLine);
                expression(x1, a, numLine);
                if (a.size() != 1) {
                    errorNum=numLine;
                    error=true;
                    break;
                }
                if(x2==""){
                    errorNum=numLine;
                    error=true;
                    break;
                }
                bool found = false;
                int size=x1.size();
                for(int k=0;k<size;k++){
                    if(x1[k]<='z'&&x1[k]>='a'){
                        x1+="a";
                    }
                    else if(x1[k]<='Z'&&x1[k]>='A'){
                        x1+="z";
                    }
                }
                for (int i = 0; i < vals.size(); i++) {
                    if (vals[i] == x1)
                        found = true;
                }
                if (!found) {
                    int size=x1.size();
                    vals.push_back(x1);
                    cout<<"PUSH offset " << x1 << "high"<<endl;
                    cout << "PUSH offset " << x1 << "low" << endl;
                }
                else{
                    cout<<"PUSH offset " << x1 << "high"<<endl;
                    cout << "PUSH offset " << x1 << "low" << endl;
                }

                stack<string> ab = infixToPostfix(x2, vals, numLine);
                toAssembly(ab, vals, numLine,1,counter);
                counter++;
            }
        }
        line = " ";
        getline(infile, line);
    }
    cout << "JMP exit" << endl;
    cout << "exit:" << endl;
    cout << "MOV AH,4ch" << endl;
    cout << "MOV AL,00" << endl;
    cout << "int 21h" << endl;
    fclose (stdout);
    ofstream outfile(argv[2]);

    if(error) {
        freopen(argv[2],"w",stdout);
        printError(errorNum);
        fclose(stdout);
    }
    else {
        outfile << "DATA SEGMENT" << endl;

        for (int k = 0; k < vals.size(); k++) {
            cout << vals[k] << "low" << "\t" << "dw 0" << endl;
            cout << vals[k] << "high" << "\t" << "dw 0" << endl;
        }
        outfile << "DATA ENDS" << endl;
        line = "";
        ifstream inf(argv[3]);
        getline(inf, line);
        while (line != " ") {
            outfile << line << endl;
            line = " ";
            getline(inf, line);
        }
    }

    outfile.close();

    return 0;
}