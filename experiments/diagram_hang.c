#include <locale.h>
#include <stdio.h>
#include "link-grammar/link-includes.h"

int main()
{
    Dictionary    dict;
    Parse_Options opts;
    Sentence      sent;
    Linkage       linkage;
    char *        string;
    char *        input_string = "The flag was wet.";

    setlocale(LC_ALL, "");
    opts = parse_options_create();
    dict = dictionary_create_lang("en");
    if (!dict) {
        printf ("Fatal error: Unable to open the dictionary\n");
        return 1;
    }

    sent = sentence_create(input_string, dict);
    if (sentence_split(sent, opts)) {
        printf ("Warning: Unable to tokenize the sentence\n");
        return 1;
    }
    if (sentence_parse(sent, opts)) {
        linkage = linkage_create(0, sent, opts);

        string = linkage_print_diagram(linkage, true, 12);
        printf("12 max width: %s", string);
        linkage_free_diagram(string);

        string = linkage_print_diagram(linkage, true, 11);
        printf("11 max width: %s", string);
        linkage_free_diagram(string);

        linkage_delete(linkage);
    }
    sentence_delete(sent);

    dictionary_delete(dict);
    parse_options_delete(opts);

    return 0;
}