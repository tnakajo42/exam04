#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to swap two characters
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Function to reverse a part of the string
void reverse(char *str, int start, int end) {
    while (start < end) {
        swap(&str[start], &str[end]);
        start++;
        end--;
    }
}

// Function to find the next lexicographical permutation
int next_permutation(char *str, int len) {
    int i = len - 2;

    // Step 1: Find the largest index `i` such that str[i] < str[i+1]
    while (i >= 0 && str[i] >= str[i + 1]) {
        i--;
    }

    // If no such index exists, the permutation is the last one (sorted in descending order)
    if (i < 0) {
        return 0; // No more permutations
    }

    // Step 2: Find the largest index `j` such that str[i] < str[j]
    int j = len - 1;
    while (str[j] <= str[i]) {
        j--;
    }

    // Step 3: Swap the values of str[i] and str[j]
    swap(&str[i], &str[j]);

    // Step 4: Reverse the sequence from str[i+1] to the end of the string
    reverse(str, i + 1, len - 1);

    return 1;
}

// Comparison function for qsort to compare individual characters
int char_compare(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }

    int len = strlen(argv[1]);

    // Sort the input string first using correct character comparison
    qsort(argv[1], len, sizeof(char), char_compare);

    // Print the first (sorted) permutation
    printf("%s\n", argv[1]);

    // Generate and print all subsequent permutations in lexicographical order
    while (next_permutation(argv[1], len)) {
        printf("%s\n", argv[1]);
    }

    return 0;
}

/*
#include <stdio.h>
#include <string.h>

void swap(char *x, char *y) {
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(char *str, int start, int end) {
    if (start == end) {
        printf("%s\n", str);
    } else {
        for (int i = start; i <= end; i++) {
            swap((str + start), (str + i));
            permute(str, start + 1, end);
            swap((str + start), (str + i)); // backtrack
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }

    int len = strlen(argv[1]);
    permute(argv[1], 0, len - 1);

    return 0;
}
*/
