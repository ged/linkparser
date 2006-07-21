

Dictionary dictionary_create(char *dict_name, char *post_process_file_name,
  char *constituent_knowledge_name, char *affix_name);
int dictionary_delete(Dictionary dict);
int dictionary_get_max_cost(Dictionary dict);

Parse_Options  parse_options_create();
int parse_options_delete(Parse_Options opts);
void parse_options_set_verbosity(Parse_Options opts, int verbosity);
int  parse_options_get_verbosity(Parse_Options opts);
void parse_options_set_linkage_limit(Parse_Options opts, int linkage_limit);
int  parse_options_get_linkage_limit(Parse_Options opts);
oid parse_options_set_disjunct_cost(Parse_Options opts, int disjunct_cost);
int  parse_options_get_disjunct_cost(Parse_Options opts);
void parse_options_set_min_null_count(Parse_Options opts, int null_count);
int  parse_options_get_min_null_count(Parse_Options opts);
void parse_options_set_max_null_count(Parse_Options opts, int null_count);
int  parse_options_get_max_null_count(Parse_Options opts);
void parse_options_set_null_block(Parse_Options opts, int null_block);
int  parse_options_get_null_block(Parse_Options opts);
void parse_options_set_short_length(Parse_Options opts, int short_length);
int  parse_options_get_short_length(Parse_Options opts);
void parse_options_set_islands_ok(Parse_Options opts, int islands_ok);
int  parse_options_get_islands_ok(Parse_Options opts);
void parse_options_set_max_parse_time(Parse_Options  opts, int secs);
int  parse_options_get_max_parse_time(Parse_Options opts);
void parse_options_set_max_memory(Parse_Options  opts, int mem);
int  parse_options_get_max_memory(Parse_Options opts);
int  parse_options_timer_expired(Parse_Options opts);
int  parse_options_memory_exhausted(Parse_Options opts);
int  parse_options_resources_exhausted(Parse_Options opts);
void parse_options_reset_resources(Parse_Options opts);
void parse_options_set_cost_model_type(Parse_Options opts, int cm);
int  parse_options_get_cost_model_type(Parse_Options opts);
void parse_options_set_screen_width(Parse_Options opts, int val);
int  parse_options_get_screen_width(Parse_Options opts);
void parse_options_set_allow_null(Parse_Options opts, int val);
int  parse_options_get_allow_null(Parse_Options opts);
void parse_options_set_display_walls(Parse_Options opts, int val);
int  parse_options_get_display_walls(Parse_Options opts);
void parse_options_set_all_short_connectors(Parse_Options opts, int val);
int  parse_options_get_all_short_connectors(Parse_Options opts);

Sentence sentence_create(char *input_string, Dictionary dict);
void sentence_delete(Sentence sent);
int sentence_parse(Sentence sent, Parse_Options opts);
int sentence_length(Sentence sent);
char * sentence_get_word(Sentence sent, int w);
int sentence_null_count(Sentence sent);
int sentence_num_linkages_found(Sentence sent);
int sentence_num_valid_linkages(Sentence sent);
int sentence_num_linkages_post_processed(Sentence sent);
int sentence_num_violations(Sentence sent, int i);
int sentence_disjunct_cost(Sentence sent, int i);

Linkage  linkage_create(int index, Sentence sent, Parse_Options opts);
int linkage_get_num_sublinkages(Linkage linkage);
int linkage_set_current_sublinkage(Linkage linkage, int index);
int linkage_compute_union(Linkage linkage);
int linkage_get_num_words(Linkage linkage);
int  linkage_get_num_links(Linkage linkage);
int linkage_get_link_length(Linkage linkage, int index);
int linkage_get_link_lword(Linkage linkage, int index);
int linkage_get_link_rword(Linkage linkage, int index);
char * linkage_print_diagram(Linkage linkage);
char * linkage_print_postscript(Linkage linkage, int mode);
char * linkage_print_links_and_domains(Linkage linkage);
char * linkage_get_link_label(Linkage linkage, int index);
char * linkage_get_link_llabel(Linkage linkage, int index);
char * linkage_get_link_rlabel(Linkage linkage, int index);
int     linkage_get_link_num_domains(Linkage linkage, int index);
char ** linkage_get_link_domain_names(Linkage linkage, int index);
char *  linkage_get_violation_name(Linkage linkage);
char ** linkage_get_words(Linkage linkage);
char *  linkage_get_word(Linkage linkage, int w);
int linkage_unused_word_cost(Linkage linkage);
int linkage_disjunct_cost(Linkage linkage);
int linkage_and_cost(Linkage linkage);
int linkage_link_cost(Linkage linkage);
void linkage_delete(Linkage linkage);

PostProcessor   post_process_open(char * name);
void            post_process_close(PostProcessor postprocessor);
void linkage_post_process(Linkage linkage, PostProcessor postprocessor);


typedef struct CNode_s CNode;
struct CNode_s {
  char  * label;
  CNode * child;
  CNode * next;
  int   start, end;
};

CNode * linkage_constituent_tree(Linkage linkage);
void    linkage_free_constituent_tree(CNode * n);
char *  linkage_print_constituent_tree(Linkage linkage, int mode);
