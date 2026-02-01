#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compress(FILE *inputFile, FILE *outputFile);
void decompress(FILE *inputFile, FILE *outputFile);
void print_usage(char *programName);

int main(int argc, char *argv[]) {
    // 1. Validate Command Line Arguments
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *inputPath = argv[2];
    char *outputPath = argv[3];

    // 2. Open Files
    FILE *in = fopen(inputPath, "r");
    if (in == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE *out = fopen(outputPath, "w");
    if (out == NULL) {
        perror("Error creating output file");
        fclose(in);
        return 1;
    }

    // 3. Execute Mode
    if (strcmp(mode, "-c") == 0) {
        printf("Compressing '%s' to '%s'...\n", inputPath, outputPath);
        compress(in, out);
    } 
    else if (strcmp(mode, "-d") == 0) {
        printf("Decompressing '%s' to '%s'...\n", inputPath, outputPath);
        decompress(in, out);
    } 
    else {
        printf("Invalid mode selected.\n");
        print_usage(argv[0]);
        fclose(in);
        fclose(out);
        return 1;
    }

    // 4. Cleanup
    printf("Operation complete.\n");
    fclose(in);
    fclose(out);
    return 0;
}

/**
 * Compression Logic:
 * Reads character by character. If the next character matches the current one,
 * it increments a counter. Once a mismatch is found, it writes [Char][Count].
 */
void compress(FILE *in, FILE *out) {
    int current_char, next_char;
    int count;

    current_char = fgetc(in);
    while (current_char != EOF) {
        count = 1;
        
        // Peek at next characters to count runs
        while ((next_char = fgetc(in)) == current_char) {
            count++;
        }

        // Write the character and its frequency
        fprintf(out, "%c%d", current_char, count);

        // Move to the next sequence
        current_char = next_char;
    }
}

/**
 * Decompression Logic:
 * Reads a character, then reads the integer immediately following it.
 * Prints the character 'count' times.
 */
void decompress(FILE *in, FILE *out) {
    char ch;
    int count;

    // fscanf returns the number of items successfully read.
    // We expect 2 items: a char and an int (e.g., "A5").
    while (fscanf(in, "%c%d", &ch, &count) == 2) {
        for (int i = 0; i < count; i++) {
            fputc(ch, out);
        }
    }
}

void print_usage(char *programName) {
    printf("Usage: %s <mode> <input_file> <output_file>\n", programName);
    printf("Modes:\n");
    printf("  -c   Compress\n");
    printf("  -d   Decompress\n");
}