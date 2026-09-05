/* Anonymous reference solution for Assignment 1.
 *
 * This file demonstrates one complete implementation. Students should
 * understand every function and complete the declaration in their official
 * skeleton according to their actual use of assistance.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define DEBUG 0
#define MAX_LINES 999
#define MAX_CHARS 80
#define TOP_SCORES 5
#define MAX_MATCH 10

typedef char line_t[MAX_CHARS + 1];
typedef line_t lines_t[MAX_LINES];
typedef int score_array_t[MAX_LINES];
typedef int index_array_t[MAX_LINES];

int read_one_line(line_t line, int max);
int read_lines(lines_t lines);
double average_length(lines_t lines, int nlines);
void print_stage(int stage);
void print_line_length(lines_t lines, int index);
int prefix_match(const char *text, const char *term);
int position_match(const char *text, int argc, char *argv[]);
int match_points(int match_len);
int line_score(const char *line, int argc, char *argv[]);
void score_all_lines(lines_t lines, score_array_t scores, int nlines,
        int argc, char *argv[]);
void print_line_score(lines_t lines, score_array_t scores, int index);
int comes_before(int left, int right, score_array_t scores);
void sort_indices(index_array_t order, score_array_t scores, int nlines);
void print_stage_one(lines_t lines, int nlines);
void print_stage_two(lines_t lines, score_array_t scores, int nlines);
void print_stage_three(lines_t lines, score_array_t scores,
        index_array_t order, int nlines);
void tadaa(void);

int
main(int argc, char *argv[]) {
    lines_t lines;
    score_array_t scores;
    index_array_t order;
    int nlines;

    nlines = read_lines(lines);

    print_stage(1);
    print_stage_one(lines, nlines);

    score_all_lines(lines, scores, nlines, argc, argv);
    print_stage(2);
    print_stage_two(lines, scores, nlines);

    sort_indices(order, scores, nlines);
    print_stage(3);
    print_stage_three(lines, scores, order, nlines);

    tadaa();
    return 0;
}

int
read_lines(lines_t lines) {
    int nlines = 0;
    line_t discard;

    while (nlines < MAX_LINES &&
            read_one_line(lines[nlines], MAX_CHARS) != EOF) {
        nlines++;
    }
    while (read_one_line(discard, MAX_CHARS) != EOF) {
        /* Input beyond MAX_LINES is allowed to be discarded. */
    }
    return nlines;
}

double
average_length(lines_t lines, int nlines) {
    int total = 0;

    for (int i = 0; i < nlines; i++) {
        total += (int)strlen(lines[i]);
    }
    if (nlines == 0) {
        return 0.0;
    }
    return (double)total / nlines;
}

void
print_line_length(lines_t lines, int index) {
    printf("line %3d:\n", index);
    printf("-> %s\n", lines[index]);
    printf("-> length = %3d\n", (int)strlen(lines[index]));
}

int
prefix_match(const char *text, const char *term) {
    int len = 0;

    while (text[len] != '\0' && term[len] != '\0' &&
            tolower((unsigned char)text[len]) ==
            tolower((unsigned char)term[len])) {
        len++;
    }
    return len;
}

int
position_match(const char *text, int argc, char *argv[]) {
    int best = 0;

    for (int argnum = 1; argnum < argc; argnum++) {
        int len = prefix_match(text, argv[argnum]);
        if (len > best) {
            best = len;
        }
    }
    return best;
}

int
match_points(int match_len) {
    if (match_len > MAX_MATCH) {
        match_len = MAX_MATCH;
    }
    return (1 << match_len) - 1;
}

int
line_score(const char *line, int argc, char *argv[]) {
    int score = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        int match_len = position_match(line + i, argc, argv);
        score += match_points(match_len);
    }
    return score;
}

void
score_all_lines(lines_t lines, score_array_t scores, int nlines,
        int argc, char *argv[]) {
    for (int i = 0; i < nlines; i++) {
        scores[i] = line_score(lines[i], argc, argv);
    }
}

void
print_line_score(lines_t lines, score_array_t scores, int index) {
    printf("line %3d:\n", index);
    printf("-> %s\n", lines[index]);
    printf("-> score = %3d\n", scores[index]);
}

int
comes_before(int left, int right, score_array_t scores) {
    if (scores[left] != scores[right]) {
        return scores[left] > scores[right];
    }
    return left < right;
}

void
sort_indices(index_array_t order, score_array_t scores, int nlines) {
    for (int i = 0; i < nlines; i++) {
        order[i] = i;
    }

    for (int i = 1; i < nlines; i++) {
        int item = order[i];
        int j = i;

        while (j > 0 &&
                comes_before(item, order[j - 1], scores)) {
            order[j] = order[j - 1];
            j--;
        }
        order[j] = item;
    }
}

void
print_stage_one(lines_t lines, int nlines) {
    printf("average line = %.2f characters\n",
            average_length(lines, nlines));
    if (nlines > 0) {
        print_line_length(lines, 0);
        print_line_length(lines, nlines - 1);
    }
}

void
print_stage_two(lines_t lines, score_array_t scores, int nlines) {
    if (nlines > 0) {
        print_line_score(lines, scores, 0);
        print_line_score(lines, scores, nlines - 1);
    }
}

void
print_stage_three(lines_t lines, score_array_t scores,
        index_array_t order, int nlines) {
    int nshow = nlines < TOP_SCORES ? nlines : TOP_SCORES;

    for (int rank = 0; rank < nshow; rank++) {
        print_line_score(lines, scores, order[rank]);
    }
}

void
print_stage(int stage) {
    printf("\n");
    printf("Stage %d\n", stage);
    printf("-------\n");
}

void
tadaa(void) {
    printf("\n");
    printf("tadaa!\n");
}

int
read_one_line(line_t line, int max) {
    int len = 0;
    int c;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            line[len] = '\0';
            return len;
        }
        if (len < max) {
            line[len] = c;
            len++;
        }
    }

    if (len > 0) {
        line[len] = '\0';
        return len;
    }
    return EOF;
}
