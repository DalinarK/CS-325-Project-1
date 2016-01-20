#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;
using std::ifstream;
using std::istringstream;
using std::vector;



int main (int argc, const char * argv[])
{
	//modified from http://linuxconfig.org/c-code-on-how-to-read-characters-from-a-file
	ifstream fin;
	fin.open("MSS_TestProblems-1.txt", ios::in);

	char my_character ;
	int numberElements = 0; //assumes that all the arrays contain at least 1 number

	string numberInput;
	int convertedNumber;

	vector <vector<int>* > setOfNumbers;
	vector <int> vectorRow;
	
	vector <int> *rowPTR = new vector <int>;

	vector<vector<int>>::iterator rowIt;
	vector<int>::iterator colIt;


	int rowNumber = 0;


	while (!fin.eof() ) 
	{
		if (my_character == '[')
		{
			cout << "adding new row" << endl;
			vector <int> *rowPTR = new vector <int>;
			rowPTR->clear();
			
		}

		fin.get(my_character);
		if (my_character == '-')
		{
			numberInput += '-';
		}

		if (my_character >= '0' && my_character <= '9')
		{
			numberInput += my_character;
			// cout << numberInput << " ";
		}

		if (my_character == ',' || my_character == ']')
		{
			numberElements++;
			//Taken from http://www.cplusplus.com/articles/D9j2Nwbp/#s2n
			if ( ! (istringstream(numberInput) >> convertedNumber) )
			{
				convertedNumber = 0;
			} 
				
			//add to vector
			rowPTR->push_back(convertedNumber);
			cout << rowPTR->back() << endl;
			numberInput.clear();
		}

		if (my_character == ']')
		{
			setOfNumbers.clear();
			cout << "number of elements is: " << numberElements << endl;
			numberElements = 0;
			++rowNumber;	
			setOfNumbers.push_back(rowPTR);
		}

	}

	cout << setOfNumbers[0]->back();


	// cout << setOfNumbers[0][0] << endl;

	// for( rowIt = setOfNumbers.begin(); rowIt != setOfNumbers.end(); ++rowIt) {
	// 	for( colIt = rowIt->begin(); colIt != rowIt->end(); ++colIt)
	// 	{
	// 		std::cout << *colIt << " ";
	// 	}   
	// 	cout << endl << endl;
	// }
}
