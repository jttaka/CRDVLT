#include <iostream>
#include <string>

#if defined(_WIN32)
#include <conio.h>
std::string getPassword(const char* prompt) {
    std::cout << prompt;
    std::string s; char c;
    while ((c=_getch())!='\r') s+=c;
    std::cout << std::endl;
    return s;
}

#else

#include <termios.h>
#include <unistd.h>
std::string getPassword(const char* prompt) {
    std::cout << prompt;
    termios t, t0;
    tcgetattr(0,&t0); t=t0; t.c_lflag &= ~ECHO; tcsetattr(0,TCSANOW,&t);
    std::string s; std::getline(std::cin,s);
    tcsetattr(0,TCSANOW,&t0);
    std::cout << std::endl;
    return s;
}
#endif

int main() {
    std::string pw = getPassword("Create master password: ");
    std::cout << "Password length: " << pw.length() << std::endl;
}
