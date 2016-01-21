#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <limits>
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


/**************************************************************
 * *  Filename: main.cpp
 * *  Coded by: Dustin Dinh, Benjamin Fisher, Christina Nock
 * *  Purpose:  To read the input from text file, run the algorithm, record time, and write output to text file
 * * CREDIT: vector functions such as size, at(index), pushback found on http://www.dreamincode.net/forums/topic/33631-c-vector-tutorial/.
 * * C++ I/O functions fsuch as ifstream, ofstream, open, close found on http://www.cplusplus.com/doc/tutorial/files/ 
 * * Additional credits are in the code
 * ***************************************************************/


std::vector<std::string> readFile(std::ifstream& input);
void formatLine(std::string &rawString);
std::vector<int> convertToNumber(std::string &line);
std::vector<std::vector<int>> getNumbers(std::ifstream &input);
void writeResults(std::ofstream &output, std::vector<int>& results, int lo, int hi, int total);


//Max Sum Sub-Array struct
struct MSS
{
	int sum;
	int start;
	int end;
	MSS(){sum = std::numeric_limits<int>::min(); start = 0; end = 0;}
} result;

/*
	Function will find the sub array with the max sum in the array arg. 
	Brute force, time complexity of n^3
	Iterate over all of the ones sub arrays and find the max ones subarray, 
	then iterate over all the subarrays of size two and find the maxSum, 
	repeat until there are no more subarrays to check of size <= to the size of the array
*/
void bruteMaxSumSub(std::vector<int> &array, MSS &result, int size){	
	result.sum = std::numeric_limits<int>::min();
	result.start = 0;
	result.end = 0;
	int sumOfCur = 0;
	int start = 0;
	int end = size - 1;	
	//iterate through all of the possible subarray sizes
	for ( int subArraySize = 0; subArraySize <= end; ++subArraySize){		
		start = 0;
		//iterate through each subarray of size subArraySize in the list
		while(start <= end){			
			if((start + subArraySize) <= end)	{		
				sumOfCur = 0;
				//x
				//calculate the sum of individual subarrays
				for( int x = 0; x <= subArraySize; ++x   ){					
					sumOfCur += array[start + x];	
				}	
				if(sumOfCur  > result.sum){	
					result.sum = sumOfCur;
					result.end = start + subArraySize;
					result.start = start;
				}
			}
			++start;		
		}		
	}
}

/*
Function will find the sub array with the max sum in the array arg.
Improved brute force, time complexity of n^2
Start at lowest index and then find the max sub array of all subarrays starting from lowest to the end. 
Repeat with the next index until the end of the array is reached
ie: MSS of size lowest, then MSS of size lowest + 1, then MMS of size lowest +2, …. , MSS of sizelowest + end.

*/
void betterBruteMaxSum(std::vector<int> &array,  MSS &result, int size){
	result.sum = std::numeric_limits<int>::min();
	result.start = 0;
	result.end = 0;
	int sumOfCur = 0;
	int start = 0;
	int end = size - 1;	
	//start at index 0 and iterate through until the end
	for ( int startIndex= 0;  startIndex <= end; ++startIndex){
		sumOfCur = 0;
		//each iteration will increase the sub array by one
		for(int subArraySize = 0; subArraySize <= size; ++subArraySize){			
			if((startIndex + subArraySize <= end)){
				sumOfCur += array[startIndex + subArraySize];
				if(sumOfCur  > result.sum){		
					result.sum = sumOfCur;
					result.end = startIndex + subArraySize;
					result.start = startIndex;
				}
			}
		}						
	}
}

/*
Function will find the sub array with the max sum.
Use divide and conquer approach.  Divide array into halves until a subarray of one is reached. 
Then calculate maxSum for three cases. Case 1: maxSum is in leftSubArray.  Case 2: maxSum is int rightSubArray.  
Case 3: maxSum is a subarray that spans the left and right subarrays.
Base case the subarray maxSum equals the value of the index.
*/
MSS* divideAndConquerMaxSub(std::vector<int> &array, int start, int end){
	MSS *result = NULL;
	MSS *leftMSS = NULL;
	MSS *rightMSS = NULL;

	int mid = (end + start)/2;

	//base case
	if(start >= end){
		result = new MSS;
		result->sum = array[start]; result->start = result->end = end;
	}else{
		leftMSS  = divideAndConquerMaxSub( array, start, mid);
		rightMSS = divideAndConquerMaxSub( array, mid+1, end);
	}	

	//find span on way up tree and find the max sum sub array by comparing
	// max of span, left and right max sums
	if(result == NULL){
					
		int leftSpan = std::numeric_limits<int>::min();
		int rightSpan = std::numeric_limits<int>::min();
		int tempSum = 0;
		int spanStart = 0;
		int spanEnd = 0;
		//calculate the left span
		for(int x = mid; x >= start; --x){
			tempSum += array[x];
			if(leftSpan < tempSum){spanStart = x;  leftSpan = tempSum;}
		}
		
		//calculate the right span
		tempSum = 0;
		for(int x = mid+1; x <= end; ++x){
			tempSum += array[x];
			if(rightSpan < tempSum){spanEnd=x; rightSpan = tempSum;}
		}
		//determine which is the largest sum: left, right or span
		if ((leftSpan + rightSpan) > leftMSS->sum && (leftSpan + rightSpan) > rightMSS->sum){			
			leftMSS->sum = leftSpan + rightSpan;
			leftMSS->start = spanStart; 
			leftMSS->end = spanEnd;
			result = leftMSS;
			delete rightMSS;
			rightMSS = 0;
		}else if(leftMSS->sum > rightMSS->sum){
			delete rightMSS;
			rightMSS = 0;
			result = leftMSS;				
		}else{
			delete leftMSS;
			leftMSS = 0;
			result = rightMSS;
		}
	}

	return result;
}

/*
Function will find the sub array with the max sum.
Iterate over the array once, keeping track of two sumSubArrays.  
The first sumSubArray will store the current sum of the subarray found during the search.  
If the sum of this subarray drops below zero then it will reset its self on the next index.  
The second sum will store the maximum sum subarray found so far and will only be updated 
if a higher value is found using the first sumSubArray.
*/
void linearMaxSub(std::vector<int> &array, int size,  MSS &result){
	MSS sumCur;

	result.sum = std::numeric_limits<int>::min();	

	for (int x = 0; x < size; ++x){
		//current sum is neg so discard the sub array and start again
		if(sumCur.sum <= 0){
			sumCur.sum = array[x];
			sumCur.start = x;
			sumCur.end = x;
		}else{
			//current sum is positive so keep adding elements
			sumCur.sum += array[x];
			sumCur.end = x;
		}
		//check is the result is less than the cur if so update result
		if(result.sum < sumCur.sum)	{
			result.sum = sumCur.sum;
			result.start = sumCur.start;
			result.end = sumCur.end;
		}			
	}
}

void createVector(std::vector<int> &result, unsigned int size){

	int isNeg = 0;
	int num = 0;
	for(int i = 0; i < size; ++i){
		num = rand() % 100 + 1;
		isNeg = rand() % 2;

		if(isNeg){
			num *= -1;
		}
		result.push_back(num);
	}	
}

int main(){
	MSS * myResult;

	/* initialize random seed: */
	srand (time(NULL));

	unsigned int sizeOfNum = 512;
	std::vector<int> num;

	createVector(num, sizeOfNum);
	    
	printf("Max Sum Sub-Array Algorithms\n\n");
	printf("size = %d\n\n", sizeOfNum);
	std::clock_t    start;
    
	printf("\n\nBrute Force Max Sum Sub-Array\n\n");
	
	start = std::clock();
	bruteMaxSumSub(num, result, sizeOfNum);
	std::cout << "Brute Force Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of bruteMaxSumSub, max sum = %d\nSubArray*************\n", result.sum);
	for(int i = result.start; i <= result.end; ++i){
		printf("%d\t", num[i]);
	}

	printf("\n\nImproved Brute Force Max Sum Sub-Array\n\n");

	start = std::clock();
	betterBruteMaxSum(num, result, sizeOfNum);
	std::cout << "Better Brute Force Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of betterBruteMaxSumSub, max sum = %d\nSubArray*************\n", result.sum);
	for(int i = result.start; i <= result.end; ++i){
		printf("%d\t", num[i]);
	}

	printf("\n\nDivide and Conquer Max Sum Sub-Array\n\n");
		
	start = std::clock();
	myResult = divideAndConquerMaxSub(num, 0, sizeOfNum - 1);
	std::cout << "Divide and Conquer Max Sum Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of divideAndConquerMaxSub, max sum = %d\nSubArray*************\n", myResult->sum);
	for(int i = myResult->start; i <= myResult->end; ++i){
		printf("%d\t", num[i]);
	}
	//clean up 
	delete myResult;
	myResult = NULL;

	printf("\n\nLinear Max Sum Sub-Array\n\n");
		
	start = std::clock();
	linearMaxSub(num, sizeOfNum, result);
	std::cout << "Linear Max Sum Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of linearMaxSub, max sum = %d\nSubArray*************\n", result.sum);
	for(int i = result.start; i <= result.end; ++i){
		printf("%d\t", num[i]);
	}

	std::cout << std::endl << std::endl;	

/*********************************************************************************************************************************
**TESTING FILES
**********************************************************************************************************************************/

    
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

    textfile.close();
	/* Label the results file */
    textfile2 << " Algorithm 1: Enumeration \n";
    std::cout << " Algorithm 1: Enumeration \n" << std::endl;	


	/* Run enumeration algorithm on input numbers */
	for( int i = 0; i < setOfNumbers.size(); i++ )
    {        
		bruteMaxSumSub(setOfNumbers.at(i), result, setOfNumbers.at(i).size());
		int total = result.sum;
		int start = result.start;
		int end = result.end;
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

        betterBruteMaxSum(setOfNumbers.at(i), result, setOfNumbers.at(i).size());
		int total = result.sum;
		int start = result.start;
		int end = result.end;
        /* Call function to output is to be written to MSS_Results.txt */
        std::cout << total << std::endl;	
        writeResults( textfile2, setOfNumbers.at(i), start, end, total );
    }
	/* Label the results file */
    textfile2 << " Algorithm 3: Divide and Conquer \n";
    std::cout << " Algorithm 3: Divide and Conquer \n" << std::endl;
    /* Run divide and conquer algorithm on input numbers */
	for( unsigned int i = 0; i < setOfNumbers.size(); i++ )
    {		
		myResult = divideAndConquerMaxSub(setOfNumbers.at(i), 0, setOfNumbers.at(i).size()-1);
		int total = myResult->sum;
		int start = myResult->start;
		int end = myResult->end;
        /* Call function to output is to be written to MSS_Results.txt */
        std::cout << total << std::endl;	
        writeResults( textfile2, setOfNumbers.at(i), start, end, total );
		//clean up 
		delete myResult;
		myResult = NULL;
    }

    /* Label the results file */
    textfile2 << " Algorithm 4: Linear-time \n";
    std::cout << " Algorithm 4: Linear-time \n" << std::endl;
    for( unsigned int i = 0; i < setOfNumbers.size(); i++ )
    {		
		linearMaxSub(setOfNumbers.at(i), setOfNumbers.at(i).size(), result);
		int total = result.sum;
		int start = result.start;
		int end = result.end;
        /* Call function to output is to be written to MSS_Results.txt */
        std::cout << total << std::endl;	
        writeResults( textfile2, setOfNumbers.at(i), start, end, total );
    }
    textfile2.close();


	//clean up 
	delete myResult;
	myResult = NULL;
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


