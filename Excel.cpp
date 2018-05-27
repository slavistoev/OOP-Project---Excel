#include "Excel.h"

void Cell::setData(char const* _data) {
	delete[] data;
	len = strlen(_data);
	data = new char[len + 1];
	strcpy(data, _data);
}

Cell::Cell(const char* _data, int _length) 
	: data(nullptr), len(_length) {
	setData(_data);
}

char const* Cell::getData() const {
	return data;
}

Cell::Cell(Cell const& _cell) 
	: Cell(_cell.getData(), _cell.getLen()) {}

Cell& Cell::operator=(Cell const& _cell) {
	if (this != &_cell) {
		setData(_cell.getData());
		setLen(_cell.getLen());
	}
	return *this;
}

Cell::~Cell() {
	delete[] data;
}

void Cell::print() const {
	std::cout << std::setw(len);
	printFile(std::cout);
}

void Cell::printFile(std::ostream& out) const {
	out << getData();
}

Row::Row() : maxNumber(1), 
	cellNumber(0), row(nullptr) {
	row = new Cell[maxNumber];
}

void Row::copyRow(Row const& _row) {
	maxNumber = _row.maxNumber;
	cellNumber = _row.cellNumber;
	row = new Cell[maxNumber];
	for (int i = 0; i < cellNumber; i++)
		row[i] = _row.row[i];
}

void Row::delRow() {
	delete[] row;
}

Row::Row(Row const& _row) : Row() {
	delRow();
	copyRow(_row);
}

Row& Row::operator=(Row const& _row) {
	if (this != &_row) {
		delRow();
		copyRow(_row);
	}
	return *this;
}

Row::~Row() {
	delRow();
}

void Row::printRow() const {
	for (int i = 0; i < cellNumber; i++) {
		std::cout << " ";
		row[i].print();
		std::cout << " |";
	}
}

void Row::printRowFile(std::ostream& out) const {
	for (int i = 0; i < cellNumber; i++) {
		row[i].printFile(out);
		out << ", ";
	}
}

void Row::addCell(Cell const& _cell) {
	if (cellNumber >= maxNumber) {
		maxNumber *= 2;
		Cell* temp = new Cell[maxNumber];
		for (int i = 0; i < cellNumber; i++)
			temp[i] = row[i];
		delRow();
		row = temp;
	}
	row[cellNumber++] = _cell;
}

bool correctData(char const* _data) {
	int len = strlen(_data);
	if (len == 0)
		return true;
	else if (_data[0] == '"' && _data[len - 1] == '"')
		return true;
	else {
		bool dot = false, scientificNotation = false;
		do {
			if (_data[0] <= '9' && _data[0] >= '0')
				continue;
			else if (_data[0] == '.' && !dot) {
				dot = true;
				continue;
			}
			else if (strlen(_data) > 2 && _data[0] == 'e'
				&& (_data[1] == '+' || _data[1] == '-') &&
				!scientificNotation && dot) {
				scientificNotation = true;
				_data++;
				continue;
			}
			return false;
		} while (_data++ && strlen(_data) > 0);
	}
	return true;
}

void Row::readRow(std::istream& in) {
	char buffer[MAX];
	in.getline(buffer, MAX);
	int startIndex = -1, currIndex = 0, endIndex = -1;
	bool start = false;
	Cell c;
	do {
		if (!buffer[currIndex] || buffer[currIndex] == ',') {
			char temp[MAX] = "";
			if (startIndex != -1 && start) {
				for (int i = startIndex; i <= endIndex; i++)
					temp[i - startIndex] = buffer[i];
			}
			c.setData(temp);
			this->addCell(c);
			start = false;
		}
		else if (buffer[currIndex] == ' ');
		else {
			if (!start) {
				start = true;
				startIndex = currIndex;
			}
			endIndex = currIndex;
		}
	} while (buffer[currIndex++]);
	return;
}

Excel::Excel() : rowNumber(0), maxCellsRow(0),
	maxNumber(1), excel(nullptr) {
	excel = new Row[maxNumber];
}

void Excel::copyExcel(Excel const& _excel) {
	rowNumber = _excel.rowNumber;
	maxNumber = _excel.maxNumber;
	maxCellsRow = _excel.maxCellsRow;
	excel = new Row[maxNumber];
	for (int i = 0; i < rowNumber; i++)
		excel[i] = _excel.excel[i];
}

void Excel::delExcel() {
	delete[] excel;
}

Excel::Excel(Excel const& _excel) : Excel() {
	delExcel();
	copyExcel(_excel);
}

Excel& Excel::operator=(Excel const& _excel) {
	if (this != &_excel) {
		delExcel();
		copyExcel(_excel);
	}
	return *this;
}

Excel::~Excel() {
	delExcel();
}

void Excel::printExcel() const {
	for (int i = 0; i < rowNumber; i++) {
		excel[i].printRow();
		std::cout << std::endl;
	}
}

void Excel::printFileExcel(std::ostream& out) const {
	for (int i = 0; i < rowNumber; i++) {
		excel[i].printRowFile(out);
		out << std::endl;
	}
}

void Excel::addRow(Row const& _row) {
	if (rowNumber >= maxNumber) {
		maxNumber *= 2;
		Row* temp = new Row[maxNumber];
		for (int i = 0; i < rowNumber; i++)
			temp[i] = excel[i];
		delExcel();
		excel = temp;
	}
	excel[rowNumber++] = _row;
	if (_row.getCellNumber() > maxCellsRow)
		maxCellsRow = _row.getCellNumber();
}

void Excel::makeTable() {
	for (int i = 0; i < rowNumber; i++)
		for (int j = excel[i].getCellNumber();
			j < maxCellsRow; j++)
			excel[i].addCell(Cell());
	for (int i = 0; i < maxCellsRow; i++) {
		int maxLen = 0;
		for (int j = 0; j < rowNumber; j++) {
			if (!correctData(excel[j].getRow()[i].getData())) {
				std::cout << "Error : row " << j << ", col "
					<< i << " : " << excel[j].getRow()[i].getData() <<
					" is unknown data type! Cell is empty!" << std::endl;
				excel[j].getRow()[i].setData("");
				excel[j].getRow()[i].setLen(0);
			}
			int currLen = excel[j].getRow()[i].getLen();
			if (currLen > maxLen)
				maxLen = currLen;
		}
		for (int j = 0; j < rowNumber; j++)
			excel[j].getRow()[i].setLen(maxLen);
	}
}

bool moveInput(char* input) {
	if (strchr(input, ' ')) {
		strcpy(input, strchr(input, ' ') + 1);
		while (strlen(input) > 0 && input[0] == ' ')
			strcpy(input, strchr(input, ' ') + 1);
		return true;
	}
	return false;
}

void Excel::editCell(char* _input) {
	int row = 0, col = 0;
	readCellCoordinates(_input, row, col);
	if (!isCellIn(row, col)) return;

	Cell* temp = &excel[--row].getRow()[--col];
	moveInput(_input); _input++;
	moveInput(_input);

	double firstNumber = 0, secondNumber = 0;
	readEditNumber(_input, firstNumber, row, col);
	moveInput(_input);
	char op = _input[0]; moveInput(_input);
	readEditNumber(_input, secondNumber, row, col);

	double result = 0;
	bool error = false;
	switch (op) {
	case '+': result = firstNumber + secondNumber; break;
	case '-': result = firstNumber - secondNumber; break;
	case '*': result = firstNumber * secondNumber; break;
	case '^': result = pow(firstNumber, secondNumber); break;
	case '/': secondNumber == 0 ? error = true
		: result = firstNumber / secondNumber; break;
	default: error = true;
	}
	if (error) {
		temp->setData("\"ERROR\"");
		std::cout << "Could not modify cell! Check your input.\n";
	}
	else {

		// code implemented from 
		// Stack Overflow double to char* google search 
		char buffer[MAX];
		sprintf(buffer, "%g", result);
		// end of implemented code

		temp->setData(buffer);
		std::cout << "Successfully edited cell!\n";
	}
	makeTable();
}

bool Excel::isCellIn(int row, int col) const {
	if (row > rowNumber || col > maxCellsRow ||
		row < 0 || col < 0) {
		std::cout << "Cell (" << row << ", "
			<< col << ") does not exist! Enter another"
			" cell within range (0, 0) - (" << rowNumber
			<< ", " << maxCellsRow << ")\n";
		return false;
	}
	return true;
}

bool readCellCoordinates(char* _input,
	int& row, int& col) {
	if (_input[0] != 'R')
		return false;
	_input++;
	row = atoi(_input);
	while (_input[0] >= '0' && _input[0] <= '9'
		&& strlen(_input) > 0)
		_input++;
	_input++;
	col = atoi(_input);
	return true;
}

void Excel::readEditNumber(char* _input, double& _number, 
	int& row, int& col) {
	if (readCellCoordinates(_input, row, col)) {
		if (isCellIn(row, col)) {
			_number = atof(excel[--row].getRow()[--col].getData());
		}
	}
	else
		_number = atof(_input);
}