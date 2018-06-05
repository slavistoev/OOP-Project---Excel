#include "Excel.h"

void nl() {
	std::cout << std::endl;
}

void isSpace() {
	char c;
	if (std::cin.peek() != ' ')
		std::cout << "Leave space between '>'"
		" and command\n";
}

bool isCommand() {
	char c, input[MAX];
	do {
		std::cin.get(c);
	} while (c == ' ');
	if (c != '>') {
		std::cout << "Not a command. Every command " <<
			"begins with '>'!\n";
		if (c != '\n')
			std::cin.getline(input, MAX);
		return 0;
	}
	isSpace();
	return 1;
}

int excel()
{
	char input[MAX];
	do {
		while (!isCommand());
		std::cin.getline(input, MAX);
		moveInput(input);
		if (!strstr(input, "exit")
			&& !strstr(input, "open")) {
			std::cout << "Open a file first!\n";
			continue;
		}

		if (strstr(input, "exit")) {
			std::cout << "Exit!" << std::endl;
			continue;
		}

		char fileName[MAX];
		if (!moveInput(input))
			std::cout << "Leave space between "
			"command and command input!\n";
		strcpy(fileName, input);
		std::ifstream fin(fileName, std::ios::in);
		if (!fin) {
			std::cout << "No such file exists. Open another one.\n";
			continue;
		}

		std::cout << "Successfully opened " << fileName << "\n";
		Excel e;
		do {
			Row r;
			r.readRow(fin);
			e.addRow(r);
		} while (fin);
		fin.close();
		// read all the rows
		e.makeTable();
		do {
			while (!isCommand());
			std::cin.getline(input, MAX);
			moveInput(input);
			// switch (command)
			if (strstr(input, "exit")) {
				std::cout << "Exit!" << std::endl;
			}
			else if (strstr(input, "close")) {
				std::cout << "Successfully closed "
					<< fileName << std::endl;
			}
			else if (strstr(input, "print")) {
				e.printExcel();
			}
			else if (strstr(input, "save") && !strstr(input, "saveas")) {
				std::ofstream fout(fileName, std::ios::out);
				e.printFileExcel(fout);
				std::cout << "Successfully saved the changes to "
					<< fileName << std::endl;
			}
			else if (strstr(input, "saveas")) {
				moveInput(input);
				std::ofstream fout(input, std::ios::out);
				e.printFileExcel(fout);
				std::cout << "Successfully saved the changes to "
					<< input << std::endl;
			}
			else if (strstr(input, "edit")) {
				moveInput(input);
				e.editCell(input);
			}
		} while (!strstr(input, "exit")
			&& !strstr(input, "close"));
	} while (!strstr(input, "exit"));

	return 0;
}

int main()
{
	excel();

	return 0;
}
