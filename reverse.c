#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// write output file
void write(char **lines, char *outputName, size_t linesCount)    {
    FILE *outputFile = fopen(outputName, "w");
    if(outputFile == NULL)  {
        fprintf(stderr, "error: cannot open file '%s'\n", outputName);
        exit(1);
    }

    //write lines to a file
    fprintf(stdout, "Writing file: %s\n", outputName);
    for (int i = linesCount - 1; i >= 0; i--) {
        fprintf(outputFile, "%s", lines[i]); // print line to output file
        free(lines[i]);
    }
    fclose(outputFile);
    return;
}

// print function for printing to screen with stdout
void print(char **lines, size_t linesCount)    {
    for (int i = linesCount - 1; i >= 0; i--) {
        fprintf(stdout, "%s", lines[i]); // print line to stdout 
        free(lines[i]);
    }
    return;
}

// read file function
void readFile(FILE *file, char ***lines, size_t *linesCount)    {
    char *line = NULL;
    size_t len = 0;
    *lines = NULL;
    *linesCount = 0;

    // loop for read lines from input file
    while (getline(&line, &len, file) != -1) {
        
        size_t lineLen = strlen(line);
        // ensure the line ends with line change '\n'
        if(lineLen == 0 || line[lineLen - 1] != '\n')   {
            line = realloc(line, lineLen + 2); // realloc line if newline is missing
            if(line == NULL)    {
                fprintf(stderr, "realloc failed\n");
                exit(1);
            }
            line[lineLen] = '\n';
            line[lineLen + 1] = '\0';
            lineLen++;
        }
        // enlarge array
        char **tmp = realloc(*lines,(*linesCount +1) * sizeof(char *));
        if(tmp == NULL) {
            fprintf(stderr, "realloc failed\n");
            exit(1);
        }
        *lines = tmp;

        (*lines)[*linesCount] = malloc(lineLen + 1); // memory allocation for new line
        if((*lines)[*linesCount] == NULL)    {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy((*lines)[*linesCount], line); // copy line to array
        (*linesCount)++; // count lines
    }


    // for (int i = linesCount - 1; i >= 0; i--) {
    //    fprintf(stdout, "%s", lines[i]);
    //    free(lines[i]);
    // }
    //print(lines, linesCount); // call print function, pass array and counter
    
    free(line);
    return;
}

//main function
int main(int argc, char* argv[]) { 
    FILE *file;
    char *userInput = NULL;
    size_t len = 0;
    char **lines = NULL;
    size_t linesCount = 0;


    // inwoke whithout args

    if (argc == 1)  {
        fprintf(stdout, "%s", "Press ctrl + D to stop input\n");
        readFile(stdin, &lines, &linesCount);
        print(lines, linesCount); // print lines to screen
    }

    // invoke with one arg
    if (argc == 2)  {
        fprintf(stdout, "Opening file: %s\n\n", argv[1]); // tell user what filename what is trying to open
        file = fopen(argv[1], "r"); //open file
    	if (file == NULL)    {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]); // error while opening file
	        exit(1);
	    }
        else    {
            readFile(file, &lines, &linesCount); // if file opens without errors, call readfile function pass file
            print(lines, linesCount);
        }
        fclose(file); // close file
    }
    // invoke whith 2 args
    if (argc == 3)  {
        if (strcmp(argv[1], argv[2]) == 0)  {
            fprintf(stderr, "%s", "Input and output file must differ\n");
            exit(1);
        }
        // fprintf(stdout, "%s", "tähän 3 argumenttia\n");
        fprintf(stdout, "Opening file: %s\n\n", argv[1]); // tell user what filename what is trying to open
        file = fopen(argv[1], "r"); //open file
    	if (file == NULL)    {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]); // error while opening file, exit 1
	        exit(1);
	    }
        else    {
            readFile(file, &lines, &linesCount); // if file opens without errors, call readfile function pass file
            write(lines, argv[2], linesCount);
        }
        fclose(file); // close file
    }
    // invoke 2+ args, error and print usage instructions, exit 1
    if (argc > 3)    {
    	fprintf(stderr, "usage: reverse <input> <output>\n");
    	exit(1);
    }

    free(lines);
    return 0;
}
