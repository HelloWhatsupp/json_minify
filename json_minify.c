/* json_minify.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum { ascii_special_ch_end = 32 };

void reverse_str(char *str, int str_length) {
	int i;

	for (i = 0; i < str_length; i++, str_length--) {
		char tmp = str[i];
		str[i] = str[str_length];
		str[str_length] = tmp;
	}
}

void convert_i_to_a(char *str, int n) {
	char *p = str;

	while (n) {
		*p = (n % 10) + '0';
		p++;
		n /= 10;
	}
	*p = '\0';
	reverse_str(str, strlen(str) - 1);
}

int get_num_len(int n) {
	int len = 0;

	while (n) {
		len++;
		n /= 10;
	}

	return len;
}

void add_to_str(char *dst, const char *src) {
	for (; *src; src++, dst++) {
		*dst = *src; 
	}
	*dst = '\0';
}

char *get_random_name(const char *extension) {
	int n, n_len;
	char *name;

	srand(time(NULL));
	n = rand();	
	n_len = get_num_len(n);

	name = malloc((n_len + strlen(extension) + 1) * sizeof(char));
	convert_i_to_a(name, n);
	add_to_str(name + n_len, extension);
	return name;
}

int main(int argc, char **argv) {
	FILE *f_in, *f_out;
	int c;
	char *f_out_name;

	if (argc < 2) {
		fprintf(stderr, "Provide input argument(s).\n");
		return 1;
	}

	f_in = fopen(argv[1], "r");
	if (!f_in) {
		perror(argv[1]);
		return 2;
	}

	f_out_name = get_random_name(".json");
	/* printf("%s will be used for temp file name\n", f_out_name); */
	f_out = fopen(f_out_name, "w");
	if (!f_out) {
		perror(f_out_name);
		return 3;
	}

	while ((c = fgetc(f_in)) != EOF) {
		if (c > ascii_special_ch_end) {
			fputc(c, f_out);	
		}
	}
	fclose(f_in);
	fclose(f_out);

	rename(f_out_name, argv[argc == 3 ? 2 : 1]);
	free(f_out_name);

	return 0;
}
