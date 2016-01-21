/**************************************************************
 * *  Filename: main.cpp
 * *  Coded by: Dustin Dinh, Benjamin Fisher, Christina Nock
 * *  Purpose:  To read the input from text file, run the algorithm, record time, and write output to text file
 * * CREDIT: vector functions such as size, at(index), pushback found on http://www.dreamincode.net/forums/topic/33631-c-vector-tutorial/.
 * * C++ I/O functions fsuch as ifstream, ofstream, open, close found on http://www.cplusplus.com/doc/tutorial/files/ 
 * * Additional credits are in the code
 * ***************************************************************/
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <string>
#include <sstream>


#include "enumeration.h"
#include "betterEnumeration.h"
#include "divideandconquer.h"
#include "linearTime.h"

std::vector<std::string> readFile(std::ifstream& input);
void formatLine(std::string &rawString);
std::vector<int> convertToNumber(std::string &line);
std::vector<std::vector<int>> getNumbers(std::ifstream &input);
void writeResults(std::ofstream &output, std::vector<int>& results, int lo, int hi, int total);

int main()
{
    
	/* Stream class provided by C++ to read from files
	Credit: http://www.cplusplus.com/doc/tutorial/files/*/
	std::ifstream textfile;
	/* In order to open a file with a stream object we use its member function open */
    textfile.open( "MSS_Problems.txt" );
	/* To check if a file stream was successful opening a file, you can do it by calling to member is_open
	Credit: http://www.cplusplus.com/doc/tutorial/files/*/
    if( !textfile.is_open() )
    {
        textfile.close();
        std::cout << "Please add MSS_Problems.txt and try again." << std::endl;
        exit(1);
    }
	/* Stream class to write on files
	Credit: http://www.cplusplus.com/doc/tutorial/files/*/
    std::ofstream textfile2;
    textfile2.open("MSS_Results.txt");
	/* vector of vectors to store number set */
    std::vector<std::vector<int>> setOfNumbers = getNumbers(textfile);
		
	
	//look at input
	
	for( int i = 0; i < setOfNumbers.size(); i++ )
    {
		std::cout << std::endl << "Vectors of Vectors = " << i << std::endl;
		for(int x = 0; x < setOfNumbers.at(i).size(); ++x){			
			std::cout << setOfNumbers.at(i).at(x) << "\t";	
		}
        std::cout << std::endl << std::endl;
    }


    textfile.close();
	/* Label the results file */
    textfile2 << " Algorithm 1: Enumeration \n";
    std::cout << " Algorithm 1: Enumeration \n" << std::endl;	


	/* Run enumeration algorithm on input numbers */
	for( int i = 0; i < setOfNumbers.size(); i++ )
    {
        int start;
        int end;
        int total = bruteForceEnumeration(setOfNumbers.at(i), start, end);
        /* Call function to output is to be written to MSS_Results.txt */
        std::cout << total << std::endl;	
        writeResults( textfile2, setOfNumbers.at(i), start, end, total );
    }
	/* Label the results file */
    textfile2 << " Algorithm 2: Better Enumeration \n";
    std::cout << " Algorithm 2: Better Enumeration \n" << std::endl;
    /* Run better enumeration algorithm on input numbers */
	for( unsigned int i = 0; i < setOfNumbers.size(); i++ )
    {
        int start;
        int end;
        int total = betterBruteMaxSum(setOfNumbers.at(i), start, end);
	std::cout << total << std::endl;	//int start;
        //int end;
        //int total = betterBruteMaxSum(*( setOfNumbers->at(i)));
		/* Call function to output is to be written to MSS_Results.txt */
        writeResults( textfile2, setOfNumbers.at(i), start, end, total );
    }
	/* Label the results file */
    textfile2 << " Algorithm 3: Divide and Conquer \n";
    std::cout << " Algorithm 3: Divide and Conquer \n" << std::endl;
    /* Run divide and conquer algorithm on input numbers */
	for( unsigned int i = 0; i < setOfNumbers.size(); i++ )
    {
        int start;
        int end;
        int total = executeDivideAndConquer(setOfNumbers.at(i), start, end);
        std::cout << total << std::endl;
       // int total = executeDivideAndConquer(*(setOfNumbers->at(i)), start, end);
		/* Call function to output is to be written to MSS_Results.txt */
		writeResults( textfile2, setOfNumbers.at(i), start, end, total );
    }
    /* Label the results file */
    textfile2 << " Algorithm 4: Linear-time \n";
    std::cout << " Algorithm 4: Linear-time \n" << std::endl;
    for( unsigned int i = 0; i < setOfNumbers.size(); i++ )
    {
       int start;
       int end;
       int total = linearMaxSub(setOfNumbers.at(i), start, end);
       // int total = linearMaxSub(*( setOfNumbers->at(i)));
        std::cout << total << std::endl;
    /*     Call function to output is to be written to MSS_Results.txt */
		writeResults( textfile2, setOfNumbers.at(i), start, end, total);
    }
    textfile2.close();

    return 0;
}

/**************************************************************
 * *  Function name: readFile
 * *  Description: Function will format the lines by removing bracket
 * ***************************************************************/
std::vector<std::string> readFile(std::ifstream& input)
{
  	std::vector<std::string> stringVector = std::vector<std::string>();
    std::string line;
	/* Read in a line until a newline character is found using getline
	Credit http://stackoverflow.com/questions/18786575/using-getline-in-c */
    while(std::getline(input, line))
    {
        /* Use push back to insert the line into string vector */
		stringVector.push_back(line);
    }
    return stringVector;
}

/**************************************************************
 * *  Function name: formatLine
 * *  Description: Function will format the lines by removing bracket
 * ***************************************************************/
void formatLine(std::string &rawString)
{
    /* This is a temp string to hold non-bracket part of string*/
	std::string formattedString;

	/* Iterate through the string to look for bracket */
    for( unsigned int i = 0; i < rawString.length(); i++ )
    {
        /* Skip any bracket characters */
		if(rawString.at(i) == '[' || rawString.at(i) == ']' || rawString.at(i) == ',')
            continue;
        /* Only add the non bracket characters to the temp string */
		else
            formattedString += rawString.at(i);
    }
	/* Now the string is formatted */
    rawString = formattedString;
}

/**************************************************************
 * *  Function name: convertToNumber
 * *  Description: Function will read file input and put contents
 * *  into number vector
 * ***************************************************************/
std::vector<int> convertToNumber(std::string &line)
{
    std::vector<int> returnVector;
	/* Convert the string into a C style string or const char
	http://stackoverflow.com/questions/236129/split-a-string-in-c*/ 
	std::string buf; // Have a buffer string
    std::stringstream ss; // Insert the string into a stream

	ss << line;

    std::vector<std::string> tokens; // Create vector to hold our words
	while (ss >> buf){
        tokens.push_back(buf);
	}

	int tempNum = 0;

	//http://code-better.com/c/convert-string-integer-using-stringstream
	for(unsigned int i = 0; i < tokens.size(); ++i)
    {
		ss.clear();
        ss << tokens.at(i);	
		ss >> tempNum;
		if(ss.fail()){
			std::cout << "error" << std::endl;
			exit(2);
		}else{
			/* Use push back function to insert number as integer to vector */
			returnVector.push_back(tempNum);		
		}
    }

    return returnVector;
}

/**************************************************************
 * *  Function name: getNumbers
 * *  Description: Function will read file input and put contents
 * *  into number vector
 * ***************************************************************/
std::vector<std::vector<int>> getNumbers(std::ifstream &input)
{
    /* 2d pointers to int vectors 
	Credit http://stackoverflow.com/questions/21663256/how-to-initialize-a-vector-of-vectors*/
	std::vector<std::vector<int>> numbers;
    /* Call function to read numbers from file into a string */
	std::vector<std::string> numbersAsString = readFile(input);

	for( unsigned int i = 0; i < numbersAsString.size(); i++ )
    {
		if(numbersAsString.at(i).size() > 0){
			/* Call function to strip the brackets */
			formatLine(numbersAsString.at(i));

			std::vector<int> nums = convertToNumber(numbersAsString.at(i));
		
			/* Insert converted number into number vector */
			numbers.push_back(nums);
		}
    }

    return numbers;
}

/**************************************************************
 * *  Function name: writeResults
 * *  Description: Function will write results to MSSResults text file
 * ***************************************************************/
void writeResults(std::ofstream &output, std::vector<int>& results, int lo, int hi, int total)
{
    /* Use the << operator to write to a file and add back the bracket
	CREDIT http://www.cplusplus.com/forum/beginner/4442/ */
	output << "[";		
	/* For each number, write it to the file followed by comma and space*/
        for( unsigned int i = 0; i < results.size() - 1; i++ )
             output << results.at(i) << ", ";
	/* Once you get to last number, add a end bracket and newline */
        output << results.at(results.size() - 1) << "]" << std::endl;
	/* Now we will write the results from start to end index into file */
        if ((results.size() > 0) && (lo > 0) && (hi > 0))
        {
              output << "[";
              for( int j = lo; j < hi; j++ )
                   output << results.at(j) << ", ";
                   output << results.at(hi) << ']' << std::endl;
        }
        /* Now we want to write the sum of max array from start to end on the file */
        output << "Total: " << total ;
        output << std::endl;
        
}


