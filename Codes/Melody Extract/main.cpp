#include <iostream>
#include "math.h"

// Old school xmas light controller song extractor.
// To use it, hook up an logic analyzer to the CH0 of yours 24MHz Logic analyzer. Other channels connect to GND.

#define FREQ_CHANGE_TOLERANCE 20

// Function define prototypes.
int skipLine(FILE *_f);
int readOneLine(FILE *_f, char *_buffer, int _bufferSize);
void printFrequency(FILE *_f, double _old, double _current, double _tolerance, double *_freqChangeTime, double _timestamp);

int main(int argc, char** argv)
{
	// Variables
	double lastTime = 0;
	double currentTime = 0;
	double lastFrequency = 0;
	double currentFrequency = 0;
	double frequencyChangeTime = 0;
	int channelData[8] = {0};
	
	// Frist open a file
	FILE *fp;
	FILE *fCsv;
	
	fp = fopen("songData.csv", "r");
	fCsv = fopen("output.csv", "w+");
	
	if (fp ==  NULL ||fCsv == NULL)
	{
		printf("Cannot open the file or create it, freezing the program!");
		return 0;
	}
	
	// File open successfully? Skip the header!
	if (skipLine(fp) != 1)
	{
		printf("Error reading the file! Stopping the program!");
		return 0;
	}
	
	// Make an buffer of one line.
	char oneLineBuffer[1000];
	
	// Now try to process all the rows of the data.
	while (readOneLine(fp, oneLineBuffer, sizeof(oneLineBuffer)))
	{
		// Get the data of the time when CH0 has changed the state.
		sscanf(oneLineBuffer, "%lf,%d,%d,%d,%d,%d,%d,%d,%d", &currentTime, &channelData[0], &channelData[1], &channelData[2], &channelData[3], &channelData[4], &channelData[5], &channelData[6], &channelData[7]);
		
		if (channelData[0] == 1)
		{
			// Calculate the freq.
			currentFrequency = 1 / (currentTime - lastTime);

			// Process it. Watchout for the frequency change. If the frequency has changed save the new frequency and save the timestamp when frequency has changed.
			printFrequency(fCsv, lastFrequency, currentFrequency, FREQ_CHANGE_TOLERANCE, &frequencyChangeTime, currentTime);
		}
		// Now current measurements are old measurements (needed for calculating the difference).
		lastFrequency = currentFrequency;
		lastTime = currentTime;
	}
	
	// Close the files.
	fclose(fp);
	fclose(fCsv);
	
	return 0;
}

int skipLine(FILE *_f)
{
	// Get the fisrt char from the line.
	char _c = fgetc(_f);
	
	// Go trough the file until you hit new line char or end of the file.
	while (_c != EOF && _c != '\n')
	{
		_c = fgetc(_f);
	}
	
	// If you hit the EOF (End of file), return an error, otherwise retrun 1 (success).
	if (_c != EOF)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
	// You should never-ever be here. If you somehow got here, throw an error.
	return 0;
}

int readOneLine(FILE *_f, char *_buffer, int _bufferSize)
{
	// Init all the variables.
	int _n = 0;
	
	// Get the first char from the line
	char _c = fgetc(_f);
	
	// Read all the chars until you hit now line or end of file, or you run out of the memory.
	while (_c != EOF && _c != '\n' && _bufferSize > 0)
	{
		_buffer[_n++] = _c;
		_c = fgetc(_f);
		_bufferSize--;
	}
	
	// Add null-trem char.	
	_buffer[_n] = '\0';
	
	// If the last chaar is EOF, return an error.
	if (_c == EOF) return 0;
	
	// Otherwise return an success!
	return 1;
}

void printFrequency(FILE *_f, double _old, double _current, double _tolerance, double *_freqChangeTime, double _timestamp)
{
	if (fabs(_current - _old) > _tolerance)
	{
		// Calulate the duration in milliseconds.
		double _duration = (_timestamp - (*_freqChangeTime)) * 1000.0;
		
		// Print all the data (for debug only)
		//printf("Frequency %lf, Duration: %lf\r\n", _current, _currentTime - (*_freqChangeTime));
		
		// Check the frequency and duration. In the duration is smaller than 100 ms or frequency is smaller than 50 Hz, ignore it.
		if (_old > 50 && _duration > 100)
		{
			// Print only frequency data.
			//printf("%d, ", (int)(_old));
		
			// Print only duration data in milliseconds.
			printf("%d, ", (int)(_duration));
			
			fprintf(_f, "%d; %d;\n", (int)(_old), (int)(_duration));
		}
		
		// If the frequency is smaller than 50 Hz, that means song has changed.
		if (_old < 50)
		{
			fprintf(_f, "\nNew song\n");
			printf("\nNew song\n");
		}
		
		*_freqChangeTime = _timestamp;
	}	
}
