#pragma once
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cmath>

const int MAX = 100, commandSize = 10;

class Cell {
public:
	Cell(const char* _data = "", int _length = 0);
	Cell(Cell const& _cell);
	Cell& operator=(Cell const& _cell);
	~Cell();

	char const* getData() const;
	void setData(char const* _data);
	int getLen() const { return len; }
	void setLen(int _length) { len = _length; }

	void print() const;
	void printFile(std::ostream& out) const;
private:
	char* data;
	int len;
};

class Row {
public:
	Row();
	Row(Row const& _row);
	Row& operator=(Row const& _row);
	~Row();

	void printRow() const;
	void printRowFile(std::ostream& out) const;
	void addCell(Cell const& _cell);
	void readRow(std::istream& in);
	int getCellNumber() const { return cellNumber; }
	Cell* getRow() const { return row; }
private:
	Cell* row;
	int cellNumber;
	int maxNumber;

	void copyRow(Row const& _row);
	void delRow();
};

class Excel {
public:
	Excel();
	Excel(Excel const& _excel);
	Excel& operator=(Excel const& _excel);
	~Excel();

	void printExcel() const;
	void printFileExcel(std::ostream& out) const;
	void addRow(Row const& _row);
	void makeTable();
	void editCell(char* _input);
	bool isCellIn(int row, int col) const;
	void readEditNumber(char* _input, double& _number, 
		int& col, int& row);
private:
	Row* excel;
	int rowNumber;
	int maxNumber;
	int maxCellsRow;

	void copyExcel(Excel const& _excel);
	void delExcel();
};

bool correctData(char const* _data);
bool moveInput(char* input);
bool readCellCoordinates(char* _input, 
	int& row, int& col);