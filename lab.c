#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


void replace_text(const char *filename, const char *old_text, const char *new_text) {
    FILE *file = fopen(filename, "r+"); 
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char line[1024];
    long pos;
    while ((pos = ftell(file)) != -1 && fgets(line, sizeof(line), file)) {
        char *temp = NULL;
        while ((temp = strstr(line, old_text)) != NULL) {
            fseek(file, pos, SEEK_SET);
            *temp = '\0';
            fprintf(file, "%s%s%s", line, new_text, temp + strlen(old_text));
            fflush(file);
            break;
        }
    }

    fclose(file);
}


void delete_lines(const char *filename, const char *regex_pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    regex_t regex;
    if (regcomp(&regex, regex_pattern, REG_EXTENDED) != 0) {
        perror("Ошибка компиляции регулярного выражения");
        fclose(file);
        return;
    }

    FILE *temp_file = tmpfile();
    if (!temp_file) {
        perror("Ошибка создания временного файла");
        fclose(file);
        regfree(&regex);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (regexec(&regex, line, 0, NULL, 0) != 0) {
            fputs(line, temp_file);
        }
    }

    regfree(&regex);
    rewind(file);
    rewind(temp_file);

    while (fgets(line, sizeof(line), temp_file)) {
        fputs(line, file);
    }

    fclose(file);
    fclose(temp_file);
}


void insert_text(const char *filename, const char *text, int is_front) {
    FILE *file = fopen(filename, "r+");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    FILE *temp_file = tmpfile();
    if (!temp_file) {
        perror("Ошибка создания временного файла");
        fclose(file);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (is_front) {
            fprintf(temp_file, "%s%s\n", text, line);
        } else {
            fprintf(temp_file, "%s%s\n", line, text);
        }
    }

    rewind(file);
    rewind(temp_file);

    while (fgets(line, sizeof(line), temp_file)) {
        fputs(line, file);
    }

    fclose(file);
    fclose(temp_file);
}
