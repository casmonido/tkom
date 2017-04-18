#ifndef SOURCE
#define SOURCE


#define MAXLIN 200 //how much chars can be in one line  

struct TextPos
{ 
	int line_number, ascii_char_number; // Numer wiersza, znaku (od 1)
	TextPos(int l=0, int c=0): line_number(l), ascii_char_number(c) {}
};

class Source
{ 
	char *file_name; 
	FILE *file_pointer;
	TextPos current_position_in_file; 
	int total_error_number, number_of_errors_in_line; 
	char current_line_buffer[MAXLIN+1]; 
	void PrntLine(); // Druk bieżącego wiersza
	int NextLine(); // Następny wiersz do bufora
public:
	Source(const char *file);
	~Source();
	void Error(int error_number, const TextPos& atom_position, const char* txt_atom);
	int NextChar();
	const TextPos& GetPos() const { return current_position_in_file; }
};

#endif