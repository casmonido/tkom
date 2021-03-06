#include <iostream>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include "Source.h"
using namespace std;


Source::Source(const char *fn) 
{ 
	file_name = new char[strlen(fn)+1];
	strcpy(file_name, fn);
	file_pointer = fopen(file_name, "r");
	if (!file_pointer)
	{ 
		cout << "FATAL ERROR - could not open the file \"" << file_name << "\"\n";
		exit(1);
	}
	cout << "Compilation report for a file: \"" << file_name << "\"\n\n";
}

Source::~Source()
{ 
	cout << "\nEnd of the report. Number of detected errors: " << total_error_number << '\n';
	fclose(file_pointer);
	delete [] file_name;
}

void Source::Error(int error_number, const TextPos& atom_position, const char* txt_atom, bool digraph)
{ 
	if (digraph)
		digraph_counter_in_line += 1;
	total_error_number++;
	cout << "error in line " << current_position_in_file.line_number  << ", character position " << (current_position_in_file.ascii_char_number - digraph_counter_in_line - 1)
	 << ": " << txt_atom << '\n';
	cout << current_line_buffer << "\n"; //character position: sa numerowane od 1
	number_of_errors_in_line += 1;
}

int Source::NextChar()
{ 
	int ret = 1;
	if (current_position_in_file.line_number == 0) 
		ret = NextLine(); // Pierwszy wiersz
	else 
		if (!current_line_buffer[current_position_in_file.ascii_char_number]) 
			ret = NextLine(); //koniec znakow w linii, sprobuj przejsc do nowej
	
	if (ret) //plik sie nie skonczyl
		return current_line_buffer[current_position_in_file.ascii_char_number++]; 
	else
		return EOF;
}

int Source::NextLine()
{ 
	if (!fgets(current_line_buffer, MAXLIN, file_pointer)) 
		return 0; // EOF
	current_position_in_file.line_number++; 
	current_position_in_file.ascii_char_number = 0; // Przed pierwszym znakiem
	while (current_line_buffer[current_position_in_file.ascii_char_number] == ' ') 
		current_position_in_file.ascii_char_number++;

	number_of_errors_in_line = 0; // 0 błędów w nowym wierszu
	digraph_counter_in_line = 0;
	return 1;
}