/**************************************************************
 * *  Filename: linearTime.cpp
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


#include "enumeration.h"
#include "betterEnumeration.h"
#include "divideandconquer.h"
#include "linearTime.h"

std::vector<std::string>* readFile(std::ifstream& input);
void formatLine(std::string &rawString);
std::vector<int>* convertToNumber(std::string &line);
std::vector<std::vector<int>*>* getNumbers(std::ifstream &input);
void writeResults(std::ofstream &output, std::vector<int>& results, int lo, int hi, int total);

int main()
{
    
	/* Stream class provided by C++ to read from files
	Credit: http://www.cplusplus.com/doc/tutorial/files/*/
	std::ifstream textfile;
	/* In order to open a file with a stream object we use its member function open */
    textfile.open( "MSS_TestProblems.txt" );
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
    std::vector<std::vector<int>*>* setOfNumbers = getNumbers(textfile);
    textfile.close();
	/* Label the results file */
    textfile2 << " Algorithm 1: Enumeration \n";
    std::cout << " Algorithm 1: Enumeration \n" << std::endl;
	/* Run enumeration algorithm on input numbers */
    for( int i = 0; i < setOfNumbers->size(); i++ )
    {
        int start;
        int end;
        int total = bruteForceEnumeration(*( setOfNumbers->at(i)), start, end);
        /* Call function to output is to be written to MSS_Results.txt */
    //    std::cout << "Start: " << start << std::endl;
      //  std::cout << "End: " << end << std::endl;
        //std::cout << "Total: " <<  total << std::endl;	
        writeResults( textfile2, *setOfNumbers->at(i), start, end, total );
    }
	/* Label the results file */
    textfile2 << " Algorithm 2: Better Enumeration \n";
    std::cout << " Algorithm 2: Better Enumeration \n" << std::endl;
    /* Run better enumeration algorithm on input numbers */
	for( int i = 0; i < setOfNumbers->size(); i++ )
    {
        int start;
        int end;
        int total = betterBruteMaxSum(*( setOfNumbers->at(i)), start, end);
//	std::cout << total << std::endl;	//int start;
        //int end;
        //int total = betterBruteMaxSum(*( setOfNumbers->at(i)));
		/* Call function to output is to be written to MSS_Results.txt */
        writeResults( textfile2, *setOfNumbers->at(i), start, end, total );
    }
	/* Label the results file */
    textfile2 << " Algorithm 3: Divide and Conquer \n";
    std::cout << " Algorithm 3: Divide and Conquer \n" << std::endl;
    /* Run divide and conquer algorithm on input numbers */
	for( int i = 0; i < setOfNumbers->size(); i++ )
    {
        int start;
        int end;
        int total = executeDivideAndConquer(*(setOfNumbers->at(i)), start, end);
  //      std::cout << total << std::endl;
       // int total = executeDivideAndConquer(*(setOfNumbers->at(i)), start, end);
		/* Call function to output is to be written to MSS_Results.txt */
		writeResults( textfile2, *setOfNumbers->at(i), start, end, total );
    }
    /* Label the results file */
    textfile2 << " Algorithm 4: Linear-time \n";
    std::cout << " Algorithm 4: Linear-time \n" << std::endl;
    for( int i = 0; i < setOfNumbers->size(); i++ )
    {
       int start;
       int end;
       int total = linearMaxSub(*( setOfNumbers->at(i)), start, end);
       // int total = linearMaxSub(*( setOfNumbers->at(i)));
    //    std::cout << total << std::endl;
    /*     Call function to output is to be written to MSS_Results.txt */
		writeResults( textfile2, *setOfNumbers->at(i), start, end, total);
    }
    textfile2.close();
    /* Delete the space taken by vector */
    for( int i = 0; i < setOfNumbers->size(); i++ )
        delete setOfNumbers->at(i);
    delete setOfNumbers;

    return 0;
}

/**************************************************************
 * *  Function name: readFile
 * *  Description: Function will format the lines by removing bracket
 * ***************************************************************/
std::vector<std::string>* readFile(std::ifstream& input)
{
  	std::vector<std::string>* stringVector = new std::vector<std::string>();
    std::string line;
	/* Read in a line until a newline character is found using getline
	Credit http://stackoverflow.com/questions/18786575/using-getline-in-c */
    while(std::getline(input, line))
    {
        /* Use push back to insert the line into string vector */
		stringVector->push_back(line);
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
    for( int i = 0; i < rawString.length(); i++ )
    {
        /* Skip any bracket characters */
		if(rawString.at(i) == '[' || rawString.at(i) == ']')
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
std::vector<int>* convertToNumber(std::string &line)
{
    std::vector<int>* returnVector = new std::vector<int>();
	/* Convert the string into a C style string or const char * 
	Credit: http://stackoverflow.com/questions/10958437/how-to-convert-an-stdstring-to-c-style-string */
    const char *lineCStyle = line.c_str();
	/* In order to split the line into numbers need to delimit on commas using strtok
	Credit: http://stackoverflow.com/questions/10513841/using-strtok-to-split-the-string */
    char* tokenizedString = strtok((char*)lineCStyle, " ,");
    while(tokenizedString != NULL)
    {
        /* For the tokenized, for each number use atoi to convert to a number 
		Credit http://stackoverflow.com/questions/7663709/convert-string-to-int-c */
		int tempNum = atoi(tokenizedString);
		/* Use push back function to insert number as integer to vector */
        returnVector->push_back(tempNum);
		/* The second time you call it you pass it NULL as the first parameter to tell function to resume from the last spot in the string
		Credit https://bytes.com/topic/c/answers/710341-null-second-call-strtok */
        tokenizedString = strtok(NULL, " ,");
    }
    return returnVector;
}

/**************************************************************
 * *  Function name: getNumbers
 * *  Description: Function will read file input and put contents
 * *  into number vector
 * ***************************************************************/
std::vector<std::vector<int>*>* getNumbers(std::ifstream &input)
{
    /* 2d pointers to int vectors 
	Credit http://stackoverflow.com/questions/21663256/how-to-initialize-a-vector-of-vectors*/
	std::vector<std::vector<int>*>* numbers = new std::vector<std::vector<int>*>();
    /* Call function to read numbers from file into a string */
	std::vector<std::string>* numbersAsString = readFile(input);
	for( int i = 0; i < numbersAsString->size(); i++ )
    {
        /* Call function to strip the brackets */
		formatLine(numbersAsString->at(i));
		/* Insert converted number into number vector */
        numbers->push_back(convertToNumber(numbersAsString->at(i)));
    }
	/* Clean the memory taken by the temp string */
    delete numbersAsString;
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
        for( int i = 0; i < results.size() - 1; i++ )
             output << results.at(i) << ", ";
	/* Once you get to last number, add a end bracket and newline */
        output << results.at(results.size() - 1) << "]" << std::endl;
	/* Now we will write the results from start to end index into file */
        //iif ((results.size() > 0) && (lo > 0) && (hi > 0))
        //{
              output << "[";
              for( int j = lo; j < hi; j++ )
                   output << results.at(j) << ", ";
                   output << results.at(hi) << ']' << std::endl;
       // }
        /* Now we want to write the sum of max array from start to end on the file */
        output << total;
        output << std::endl;
        
}


