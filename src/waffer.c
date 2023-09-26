/*
    waffer - An easy to use CLI tool that adds license(s) to your projects!
    Copyright (C) 2023  Unified

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to replace placeholders in the license text
void replacePlaceholders(char *licenseText, const char *year, const char *holder) {
    char *yearPlaceholder = "\%copyright_year\%";
    char *holderPlaceholder = "\%copyright_holder\%";
    char *pos = licenseText;

    while ((pos = strstr(pos, yearPlaceholder)) != NULL) {
        memmove(
            pos + strlen(year),
            pos + strlen(yearPlaceholder),
            strlen(pos + strlen(yearPlaceholder)) + 1
        );
        memcpy(pos, year, strlen(year));
    }

    pos = licenseText;
    while ((pos = strstr(pos, holderPlaceholder)) != NULL) {
        memmove(
            pos + strlen(holder),
            pos + strlen(holderPlaceholder),
            strlen(pos + strlen(holderPlaceholder)) + 1
        );
        memcpy(pos, holder, strlen(holder));
    }
}

// Function to write the license text to the LICENSE file
void writeLicenseToFile(const char *licenseText) {
    FILE *outputFile = fopen("LICENSE", "w");
    if (outputFile == NULL) {
        perror("Unable to create LICENSE file.");
        exit(EXIT_FAILURE);
    }

    fprintf(outputFile, "%s", licenseText);
    fclose(outputFile);
}

// Function to read the license text from a file
char *readLicenseFromFile(const char *licenseFileName) {
    FILE *licenseFile = fopen(licenseFileName, "r");
    if (licenseFile == NULL) {
        return NULL; // Return NULL to indicate the file was not found
    }

    fseek(licenseFile, 0, SEEK_END);
    long fileSize = ftell(licenseFile);
    fseek(licenseFile, 0, SEEK_SET);

    char *licenseText = (char *)malloc(fileSize + 1);
    if (licenseText == NULL) {
        perror("! Error >> Memory allocation error !");
        fclose(licenseFile);
        exit(EXIT_FAILURE);
    }

    fread(licenseText, 1, fileSize, licenseFile);
    fclose(licenseFile);

    licenseText[fileSize] = '\0';
    return licenseText;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: waffer -l <license> -y <copyright_year> -h <copyright_holder> [OR] -h <help_menu> [OR] -v [version]\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-v") == 0) {
        printf("Running waffer v1.0 by unium @ unified\n");
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-h") == 0) {
        printf("\n------------------------------------------------------------------------------------\n");
        printf("waffer v1.0 by unium @ unified\n");
        printf("Description : An easy to use CLI tool that adds license(s) to your projects!\n");
        printf("Usage: waffer -l <license> -y <copyright_year> -h <copyright_holder> [OR] [options]\n");
        printf("Options:\n");
        printf("-h                Displays this menu.\n");
        printf("-v                Displays the version of this command.\n");
        printf("------------------------------------------------------------------------------------\n\n");
        return EXIT_SUCCESS;
    }

    char *licenseName = NULL;
    char *year = NULL;
    char *holder = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            licenseName = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc) {
            year = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            holder = argv[i + 1];
            i++;
        }
    }

    if (licenseName == NULL || year == NULL || holder == NULL) {
        printf("Missing required arguments.\n");
        printf("Usage: waffer -l <license> -y <copyright_year> -h <copyright_holder> [OR] -h <help_menu> [OR] -v [version]\n");
        return EXIT_FAILURE;
    }

    char licenseFileName[256];
    snprintf(licenseFileName, sizeof(licenseFileName), "/etc/unified/waffer/licenses/%s.LICENSE", licenseName);

    char *licenseText = readLicenseFromFile(licenseFileName);
    if (licenseText != NULL) {
        // Replace placeholders in the license text
        replacePlaceholders(licenseText, year, holder);
        writeLicenseToFile(licenseText);
        free(licenseText);
        printf("%s license added to LICENSE file with year: [%s] and holder: [%s].\n", licenseName, year, holder);
    } else {
        printf("License file not found for %s.\n", licenseName);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
