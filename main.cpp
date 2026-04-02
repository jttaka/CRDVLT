#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <argon2.h>
#include <string>
#include <cstdlib>

#if defined(_WIN32)
#include <conio>
std::string getPsswrd(const char* prompt) {
	std::cout << prompt;
	std::string s; char c;
	while ((c=_getch())!='\r') s+=c;
	std::cout << std::endl;
	return s;
}
#else
#include <unistd.h>
#include <termios.h>
std::string getPsswrd(const char* prompt) {
	std::cout << prompt;
	termios t, t0;
	tcgetattr(0,&t0); t=t0; t.c_lflag &= ~ECHO; tcsetattr(0,TCSANOW,&t);
	std::string s; std::getline(std::cin,s);
	tcsetattr(0,TCSANOW,&t0);
	std::cout << std::endl;
	return s;
}
#endif

std::string hash(const std::string& psswrd) {
		std::string salt = "random_salt";

		char hash[32];

		if (argon2i_hash_raw(
					2,
					1 << 16,
					2,
					psswrd.c_str(),
					psswrd.size(),
					salt.c_str(),
					salt.size(),
					reinterpret_cast<unsigned char*>(hash),
					sizeof(hash)
				) != ARGON2_OK) {
			std::cerr << "Error hashing password\n";
			return "";
		}
		std::stringstream ss;
		for (int i = 0; i < 32; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
		}
		return ss.str();
}

void cmd() {
	while(true) {
		std::cout << "\n➜ ";
		std::string prompt;
		std::cin >> prompt;
		if (prompt == "exit") {
			exit(0);
		} else if (prompt == "help" || prompt == "h") {
			std::cout << "Available commands: help, h, exit.";
		} else {
			std::cout << "Invalid prompt. Try help or h";
		}
	}
}

int main() {
	if (std::filesystem::exists("psswrd")) {
		std::ifstream inFile("psswrd");
		std::string psswrd;

		std::stringstream buffer;
		buffer << inFile.rdbuf();
		psswrd = buffer.str();

		std::string psswrd_A = getPsswrd("Enter master password: ");

		if (psswrd == hash(psswrd_A)) {
			std::cout << "Correct";
			cmd();
		} else {
			std::cout << "Incorrect";
		}

	} else {
		std::cout << "First time running CRDVLT.\nCreating password file.\n";
		std::ofstream file("psswrd");
		std::cout << "Password file succesfully created!\n";

		std::string psswrd = getPsswrd("Create master password: ");

		std::ofstream outFile("psswrd");

		outFile << hash(psswrd);
		outFile.close();
		cmd();

	}
	return 0;
} 
