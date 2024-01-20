// Wilson Lopez
// Created on November 16, 2020
// Updated on December 8, 2023

// References: 
// https://www.tutorialspoint.com/cplusplus/cpp_data_structures.htm
// https://en.wikipedia.org/wiki/Comma-separated_values#Internet_W3C_tabular_data_standard
// https://www.tutorialspoint.com/data_structures_algorithms/algorithms_basics.htm
// https://www.youtube.com/watch?v=da8prmVYul8
// https://www.geeksforgeeks.org/csv-file-management-using-c/
// https://www.cplusplus.com/reference/string/string/
// https://www.delftstack.com/howto/cpp/how-to-convert-float-to-string-in-cpp/
// https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
// https://www.cplusplus.com/reference/vector/vector/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

struct contents
{
	std::string name;
	std::vector <std::string> date;
	std::vector<float> open, high, low, close, adj, percent_change;
	std::vector<int> volume;
};

// Finds the filename of the .csv file.
void Compute_1_Filename(std::string& filename)
{
	// Stores the name of the file with the .csv extension in the filename parameter
	// Make sure that the .csv file is in the same directory as the main .cpp file when you are making this project.

	WIN32_FIND_DATAA findfiledata;
	HANDLE hfind;
	std::string file_extension = "*.csv";

	std::string directory_file_ext = file_extension;
	hfind = FindFirstFileA(directory_file_ext.c_str(), &findfiledata);

	if (hfind != INVALID_HANDLE_VALUE)
		filename = findfiledata.cFileName;
	else
		std::cout << "No file was present" << std::endl;
}

// Stores the contents of the file inside a data structure.
void StoreContents(contents& csvFile, std::string f, unsigned int& trading_days)
{
	std::ifstream c;
	c.open(f);
	
	if (!c.is_open())
		exit(0);
	
	if (c.is_open())
	{
		std::string first_line;
		std::string temp1, temp2, temp3, temp4, temp5, temp6, temp7;
		getline(c, first_line, '\n');

		// A while loop that repeats if c is not at the end of the file.
		while (!c.eof())
		{
			getline(c, temp1, ',');					// Read until it hits a comma and store it inside temp1.
			csvFile.date.push_back(temp1);			// Push back the value of temp1 into csvFile.date.
			
			getline(c, temp2, ',');					// Read until it hits a comma and store it inside temp2.
			csvFile.open.push_back(stof(temp2));	// Push back the value inside of temp2 as float, into csvFile.open.

			getline(c, temp3, ',');					// Read until it hits a comma and store it inside temp3.
			csvFile.high.push_back(stof(temp3));	// Push back the value of temp3 as float into csvFile.high.
	
			getline(c, temp4, ',');					// Read until it hits a comma and store it inside temp4.
			csvFile.low.push_back(stof(temp4));		// Push back the value of temp4 as float into csvFile.low.

			getline(c, temp5, ',');					// Read until it hits a comma and store it inside temp5.
			csvFile.close.push_back(stof(temp5));	// Push back the value of temp5 as float into csvFile.close.

			getline(c, temp6, ',');					// Read until it hits a comma and store it inside temp6.
			csvFile.adj.push_back(stof(temp6));		// Push back the value of temp6 as float into csvFile.adj.

			getline(c, temp7, '\n');				// Read until it hits a newline and store it inside temp7.
			csvFile.volume.push_back(stoi(temp7));	// Push back the value of temp7 as an integer into csvFile.volume.
			
			trading_days++;							// Increment the number of trading days.
			
		}

		// Removes newline character from corresponding members of csvFile data structure.
		csvFile.date.pop_back();				
		csvFile.open.pop_back();
		csvFile.high.pop_back();
		csvFile.low.pop_back();
		csvFile.close.pop_back();
		csvFile.adj.pop_back();	
		csvFile.volume.pop_back();
	}
	
	
	c.close();
}

// Calculates the Percent Change for every adj and open value in the file.
void PercentChange(contents& csvFile, unsigned int t_days)
{
	for (unsigned int i = 0; i < t_days; i++)
		csvFile.percent_change.push_back((csvFile.adj[i] - csvFile.open[i]) / csvFile.open[i] * 100);
}

// This function sorts the percent change, date, open and adj values in descending order (based on the percent change order).
void SortValues(contents& csvFile)
{
	bool swap;
	std::string date_temp;
	float pc_temp, open_temp, adj_temp;
	int size_minus_one = (int(csvFile.percent_change.size()) - 1);	// For traversing the vector inside the csvFile data structure.

	do
	{
		swap = false;	// Assume that all values are in order.
		for (int count = 0; count < size_minus_one; count++)
		{
			// Compares percent change values inside vector.
			if (csvFile.percent_change[count] < csvFile.percent_change[count + 1])
			{
				// Sorts the Percent Change values that are inside the struct.
				pc_temp = csvFile.percent_change[count];
				csvFile.percent_change[count] = csvFile.percent_change[count + 1];
				csvFile.percent_change[count + 1] = pc_temp;
				swap = true;

				// Sorts the Dates inside the data structure, according to the Percent Change order.
				date_temp = csvFile.date[count];
				csvFile.date[count] = csvFile.date[count + 1];
				csvFile.date[count + 1] = date_temp;

				// Sorts the Open values that are inside the struct, according to the Percent Change order.
				open_temp = csvFile.open[count];
				csvFile.open[count] = csvFile.open[count + 1];
				csvFile.open[count + 1] = open_temp;

				// Sorts the Adj. Close values that are inside the struct, according to the Percent Change order.
				adj_temp = csvFile.adj[count];
				csvFile.adj[count] = csvFile.adj[count + 1];
				csvFile.adj[count + 1] = adj_temp;
			}
		}
	} while (swap);
}

int main()
{
	contents csvFile;
	std::string filename;
	unsigned int trading_days = 0, user_input;

	Compute_1_Filename(filename);

	// Used to show filename without file extension.
	//ticker_length = filename.length() - 4; // 4 is the length of ".csv"
	//ticker_name = filename.substr(0, ticker_length);
	
	StoreContents(csvFile, filename, trading_days);
	
	trading_days--;	// Substracts a number from the trading days, as one of the rows inside the file are the column titles.

	std::cout << "This program will analyze \"" << filename << "\"";	// Outputs the name of the filename to the console screen.

	std::cout << " from " << csvFile.date.front() << " to " << csvFile.date.back() << std::endl;	// Outputs the start and end date that was found in the file.

	std::cout << "There are " << trading_days << " trading day(s) in this file." << std::endl;	// Outputs how many trading day(s) exist in the file.
	
	// Input the number of records from the console screen.
	std::cout << "Enter the number of increase records you want to find: ";
	std::cin >> user_input;

	// Validates that number of records that user wants to find is within range.
	// (Cannot be more than number of traiding days or less than zero.)
	while (user_input > trading_days || user_input < 0)
	{
		std::cout << std::endl << "The file does not contain enough trading days." << std::endl;
		std::cout << "There are only " << trading_days << " trading days." << std::endl;
		std::cout << "Enter the number of increase record you want to find: ";
		std::cin >> user_input;
	}

	// Column heads for output values.
	std::cout << std::endl << "Date" << "\t \t" << "Open" << "\t \t" << "A.Close" << " \t" << "Percent Change" << std::endl;

	PercentChange(csvFile, trading_days);
	SortValues(csvFile);

	// Outputs the the date, open, adj, and percent change values from descending order, based on the number the user entered.
	for (unsigned int i = 0; i < user_input; i++)
		std::cout << std::fixed << csvFile.date.at(i) << '\t' << csvFile.open.at(i) << '\t' << csvFile.adj.at(i) << '\t' << csvFile.percent_change[i] << std::endl;

	return 0;
}