#pragma once
#include "Condition.h"
#include "EQTB.h"
#include "FontInfo.h"
#include "Glue.h"
#include "Hashtab.h"
#include "Image.h"
#include "LabelInfo.h"
#include "LineList.h"
#include "Mactab.h"
#include "SaveState.h"
#include "Stack.h"
#include "SthInternal.h"

struct RealNumber;
struct FpNum;
struct OneIndex;
class Math;
class SaveAuxBase;
class SaveAuxEnv;

// This file holds the definition of the Parser class, which is the main
// data structure of the Tralics translator.
// \todo this should be broken up

struct Parser {
    Mactab                                    mac_table;          // the table of macros
    std::array<EqtbString, 10>                eqtb_string_table;  // eqtb strings
    std::array<EqtbDim, dimension_table_size> eqtb_dim_table;     // EQTB, dimensions
    std::array<EqtbToken, toks_register_size> toks_registers;     // EQTB, token lists
    std::array<EqtbBox, nb_registers>         box_table;          // EQTB, boxes
    std::array<EqtbGlue, glue_table_size>     glue_table;         // EQTB, glue
    std::array<Token, nb_characters>          verbatim_chars;     // value of a character in verbatim mode
    std::array<long, nb_shortverb_values>     old_catcode{};      // catcodes for undefineshortverb
    std::array<size_t, 9>                     allocation_table{}; // values for \newcount etc
    std::array<bool, nb_xspace_values>        ok_for_xspace{};    // status of char w.r.t. \xspace
    std::array<Token, 22>                     uclc_list;          // upper, lowert case equivalent of \ij etc
    FontInfo                                  cur_font;           // info for the current font
    std::vector<Image>                        the_images;         // file data for images
    std::vector<Xml *>                        all_heads;
    Token                                     err_tok; // in case of error

    bool                  unexpected_seen_hi{false};             // check for wrongly placed font changes
    bool                  calc_loaded;                           // did we see \usepackage{calc} ?
    bool                  numbered_verbatim{};                   // has this verbatim line numbers ?
    bool                  restricted;                            // are we in restricted mode ?
    bool                  force_eof{false};                      // did we see \endinput ?
    bool                  no_new_file{false};                    // can we pop the input stack ?
    bool                  file_ended{};                          //
    bool                  chapter_has_star{false};               // true in frontmatter, backmatter
    bool                  list_files_p;                          // Should we list the files at the end ?
    bool                  tok_is_defined{};                      // use by \ifcsname
    int                   old_nberrs{};                          // previous number of errors
    int                   cur_line{};                            // current input line number
    int                   begin_env_line{0};                     // input line number of
    int                   default_language_num{0};               // default language
    int                   cur_level;                             // current level on the execution stack
    size_t                equation_ctr_pos{};                    // position in the table of the counter equation
    states                state;                                 // current state of the scanner
    Token                 after_assignment_token;                // token for \afterassignment
    subtypes              sectionning_offset;                    // what is the main section, part, chapter ?
    l_state               long_state;                            // Error recovery handling (\long)
    scan_stat             scanner_status;                        // Error recovery handling (\outer)
    size_t                cur_in_chan;                           // if get_token call get_a_new_line
    long                  cur_file_pos{0};                       // pos of file in the package list (0= none)
    std::string           cur_env_name;                          // name of current environment
    std::string           job_name;                              // the name, without extensions
    Buffer                input_buffer;                          // input buffer
    Buffer                mac_buffer;                            // buffer the current macro
    Buffer                group_buffer;                          // buffer for arg of \begin{...} \end(...)
    Buffer                unprocessed_xml;                       // chars to be converted into an XML element
    Buffer                fetch_name_res;                        // used by fetch_name
    LineList              lines;                                 // the lines to  be read
    TokenList             TL;                                    // list of tokens to be read again
    Condition             conditions;                            // condition stack for current \if
    SthInternal           cur_val;                               // result of scan_something internal
    TokenList             document_hook;                         // the document-hook
    TokenList             end_document_hook;                     // the end-document-hook
    char32_t              verb_saved_char{};                     // Char to use for verb by ShortVewrb
    std::vector<char32_t> input_line;                            // input line converted to chars
    size_t                input_line_pos{0};                     // position in input_line
    Xml *                 the_xmlA{nullptr}, *the_xmlB{nullptr}; // for XML tree manipulations

    [[nodiscard]] auto at_eol() const -> bool { return input_line_pos >= input_line.size(); }
    auto               get_next_char() -> char32_t { return input_line[input_line_pos++]; }
    auto               get_after_ass_tok() -> Token {
        Token x = after_assignment_token;
        after_assignment_token.kill();
        return x;
    }
    [[nodiscard]] auto get_def_language_num() const -> int { return default_language_num; }
    void               kill_line() { input_line.clear(); }
    void               see_cs_token() { cur_cmd_chr = Hashtab::the_eqtb()[cur_tok.eqtb_loc()].val; }
    void               see_cs_token(Token T) {
        cur_tok = T;
        see_cs_token();
    }
    void see_char_token(Token t) {
        cur_cmd_chr.cmd = t.cmd_val();
        cur_cmd_chr.chr = t.chr_val();
    }
    void set_after_ass_tok(Token x) { after_assignment_token = x; }
    void set_def_language_num(int x) { default_language_num = x; }

    auto               get_cur_env_name() -> std::string & { return cur_env_name; }
    void               back_input() { TL.push_front(cur_tok); }
    void               back_input(Token t) { TL.push_front(t); }
    void               back_input(TokenList &L) { TL.splice(TL.begin(), L); }
    [[nodiscard]] auto cur_line_to_istring() const -> std::string;
    void               decr_cur_level() { cur_level--; }
    [[nodiscard]] auto get_cur_filename() const -> std::string { return lines.file_name; }
    [[nodiscard]] auto get_cur_file_pos() const -> long { return cur_file_pos; } // \todo remove
    [[nodiscard]] auto get_cur_level() const -> int { return cur_level; }
    [[nodiscard]] auto get_cur_line() const -> int { return cur_line; }
    auto               get_cur_val() -> SthInternal & { return cur_val; }
    [[nodiscard]] auto get_job_name() const -> std::string { return job_name; }
    [[nodiscard]] auto get_list_files() const -> bool { return list_files_p; }
    void               init(LineList x) { lines = std::move(x); }
    void               set_cur_line(int x) { cur_line = x; }
    void               set_cur_file_pos(long k) { cur_file_pos = k; }
    void               set_cur_env_name(std::string s) {
        cur_env_name   = std::move(s);
        begin_env_line = cur_line;
    }
    void set_cur_env_name(std::string s, int x) {
        cur_env_name   = std::move(s);
        begin_env_line = x;
    }
    void set_job_name(std::string s) { job_name = std::move(s); }
    void unexpected_font() { unexpected_seen_hi = true; }
    void L3_load(bool preload);

    Parser();

    void add_buffer_to_document_hook(Buffer &b, const std::string &name);
    void add_language_att() const;
    void after_main_text();
    void boot();
    void box_end(Xml *res, size_t pos);
    auto list_to_string0(Buffer &b) -> bool;
    auto list_to_string(TokenList &L, Buffer &b) -> bool;
    auto list_to_string_cv(TokenList &L, Buffer &b) -> bool;
    void list_to_string_c(TokenList &x, const std::string &s1, const std::string &s2, const std::string &msg, Buffer &B);
    auto list_to_string_c(TokenList &x, String s1, String s2, String msg) -> Token;
    auto list_to_string_c(TokenList &x, String msg) -> std::string;
    auto csname_aux(String s1, String s2, TokenList &L, bool cs, Buffer &b) -> bool;
    auto csname_aux(TokenList &L, bool cs, Buffer &b) -> bool;
    auto csname_ctr(TokenList &L, Buffer &b) -> bool;
    void eq_define(size_t a, CmdChr bc, bool gbl);
    void titlepage_evaluate(const std::string &s, const std::string &cmd);
    void final_checks();
    void finish_images();
    void flush_buffer();
    void font_has_changed1();
    auto fp_read_value() -> FpNum;
    void fp_send_one_arg(TokenList &res);
    void init_all(const std::string &doc_elt);
    void load_latex();
    auto read_arg() -> TokenList;
    auto read_arg_nopar() -> TokenList;
    auto nct_aux(Token T, TokenList &body) -> std::optional<size_t>;
    auto nE_arg_nopar() -> std::string;
    auto nT_arg_nopar() -> std::string;
    auto nT_optarg_nopar() -> std::optional<std::string>;
    void parse_error(Token T, const std::string &s, TokenList &L);
    void parse_error(Token T, const std::string &s);
    void parse_error(const std::string &s);
    void parse_error(Token T, const std::string &s1, const std::string &s2);
    void parse_error(Token T, const std::string &s1, Token s2, const std::string &s3, const std::string &s4);
    void parse_error(Token T, const std::string &s1, const std::string &s2, const std::string &s3);
    void remove_junk();
    void scan_eqno(math_list_type type);
    void scan_glue(internal_type level);
    void scan_glue(internal_type level, Token T);
    void scan_glue(internal_type level, Token T, bool opt);
    void list_to_glue(internal_type level, Token t, TokenList &L);
    void set_default_language(int v);
    void signal_error();
    void signal_error(const std::string &s);
    void signal_error(Token T, const std::string &s);
    void signal_ovf(Token T, String h, long cur, long max);
    auto special_expand(TokenList *args) -> TokenList;
    auto special_tpa_arg(const std::string &name, const std::string &y, bool par, bool env, bool has_q) -> Xml *;
    void T_titlepage_finish(size_t v);
    auto tpa_exec(const std::string &cmd) -> Xml *;
    void M_tracingall();
    void translate0();
    void translate_all();
    void word_define(size_t a, long c, bool gbl);
    auto find_a_save_key(const std::string &mykey) -> bool;
    void mu_error(String s, int i);
    void expand_nct(TokenList &L);
    void token_for_show(const CmdChr &val);
    void create_label(const std::string &X, const std::string &S);
    void LC();

    auto               before_mac_arg() -> bool;
    auto               check_brace(int &b) const -> bool;
    auto               check_builtin_pack(const std::string &pack) -> bool;
    auto               check_if_redef(const std::string &s) -> bool;
    auto               counter_aux(const std::string &name, String opt, Token T) -> bool;
    auto               counter_check(Buffer &b, bool def) -> bool;
    auto               counter_read_opt(String s) -> int;
    auto               cs_from_input() -> Token;
    auto               delimiter_for_saveverb() -> char32_t;
    auto               delimiter_for_verb(bool &special_space) -> char32_t;
    auto               dimen_from_list(Token T, TokenList &L) -> ScaledInt;
    auto               do_register_arg(symcodes q, unsigned &p, Token &tfe) -> size_t;
    auto               E_ifx() -> bool;
    auto               E_the(subtypes c) -> TokenList;
    auto               edef_aux(TokenList &L) -> bool;
    auto               env_helper(const std::string &s) -> SaveAuxEnv *;
    auto               eval_condition(subtypes test) -> bool;
    auto               false_end_tabular(const std::string &s) -> bool;
    auto               fetch_csname(bool exp) -> Token;
    auto               fetch_name_opt() -> std::string;
    auto               fetch_name0_nopar() -> std::string;
    auto               fetch_name0() -> std::string;
    auto               fetch_name1(TokenList &L) -> std::string;
    auto               find_env_token(const std::string &name, bool beg) -> Token;
    auto               fp_read_int() -> int;
    auto               get_a_new_line() -> bool;
    auto               get_attval() -> std::string;
    auto               get_c_val(Token X) -> std::string;
    auto               get_counter(Token T) -> long;
    auto               get_ctb_opt() -> std::optional<std::string>;
    auto               get_index_value() -> size_t;
    auto               get_itoken() -> bool;
    auto               get_mac_value(const std::string &s) -> TokenList;
    auto               get_mac_value(Token T) -> TokenList;
    auto               get_opt_dim(Token T) -> std::string;
    auto               get_r_token(bool br = false) -> Token;
    auto               get_token_o() -> bool;
    auto               get_trees_opt() -> std::optional<std::string>;
    auto               get_x_token() -> bool;
    auto               grab_env_comma(TokenList &v) -> bool;
    auto               group_to_string_spec(bool) -> std::string;
    auto               group_to_string() -> std::string;
    auto               index_aux(TokenList &L, std::optional<size_t> father, OneIndex &g) -> size_t;
    auto               internal_makebox() -> Xml *;
    auto               is_delimiter(const TokenList &L) -> bool;
    auto               is_input_open() -> bool;
    auto               is_not_a_math_env(String s) -> bool;
    auto               is_verbatim_end() -> bool;
    auto               kvo_getfam() -> std::string;
    auto               latex_input(subtypes q) -> std::string;
    auto               M_counter(bool def) -> bool;
    auto               make_label_inner(const std::string &name) -> std::string;
    auto               math_argument(int w, Token T) -> subtypes;
    auto               math_dimen_attrib(Token C, String s) -> int;
    auto               math_lr_value() -> del_pos;
    auto               my_csname(String s1, String s2, TokenList &L, String s) -> bool;
    auto               new_line_for_read(bool spec) -> bool;
    [[nodiscard]] auto new_math_list(size_t cur_math, math_list_type c, subtypes s) -> std::optional<subtypes>;
    auto               next_from_line() -> bool;
    auto               next_from_line0() -> bool;
    auto               ok_to_define(Token a, rd_flag redef) -> bool;
    auto               optional_enumerate(TokenList &L, const std::string &ctr) -> bool;
    auto               push_par() -> Xid;
    auto               read_delimited(const TokenList &L) -> TokenList;
    auto               read_elt_id(Token T) -> size_t;
    auto               read_for_variable() -> Token;
    auto               read_from_file(long ch, bool rl_sw) -> TokenList;
    auto               read_latex_macro() -> Macro *;
    auto               read_mac_body(bool exp) -> TokenList;
    auto               read_mac_nbargs() -> size_t;
    auto               read_optarg_nopar(TokenList &L) -> bool;
    [[nodiscard]] auto read_optarg() -> std::optional<TokenList>;
    auto               read_token_arg(int cl) -> bool;
    auto               read_token_arg(Token t) -> bool;
    auto               read_unit() -> int;
    auto               read_until_nopar(Token x) -> TokenList;
    auto               read_until(Token x) -> TokenList;
    auto               remove_initial_plus(bool plus) -> bool;
    auto               remove_initial_star() -> bool;
    auto               rT_arg_nopar() -> std::string;
    auto               scan_27bit_int() -> size_t;
    auto               scan_alpha() -> size_t;
    auto               scan_anchor(bool &h) -> std::string;
    auto               scan_braced_int(Token T) -> long;
    auto               scan_char_num() -> size_t;
    auto               scan_color(const std::string &opt, const std::string &name) -> std::string;
    auto               scan_date_ctrs() -> bool;
    auto               scan_dim_helper(bool mu, bool allow_int) -> bool;
    auto               scan_dim2(RealNumber &R, bool mu) -> bool;
    auto               scan_dimen1(bool mu, bool inf, glue_spec &co, bool shortcut) -> bool;
    auto               scan_double_hat(char32_t cc) -> bool;
    auto               scan_expr_next(Token T, bool stack_empty) -> scan_expr_t;
    auto               scan_expr(Token T, internal_type et) -> bool;
    auto               scan_fifteen_bit_int() -> size_t;
    auto               scan_file_name() -> std::string;
    auto               scan_font_ident() -> long;
    auto               scan_for_eval(Buffer &B, bool in_env) -> bool;
    auto               scan_general_text() -> TokenList;
    auto               scan_group_del(TokenList &res, const TokenList &L) -> bool;
    auto               scan_group_del1(TokenList &res, Token x) -> bool;
    auto               scan_group_opt(TokenList &L, bool &have_arg) -> bool;
    auto               scan_group0(TokenList &res, int cl) -> bool;
    auto               scan_group1(TokenList &res, int &b, int cl) -> bool;
    auto               scan_group2(TokenList &L) -> bool;
    auto               scan_int_digs() -> long;
    auto               scan_int_internal() -> long;
    auto               scan_int(Token T, int n, String s) -> size_t;
    auto               scan_int(Token T) -> long;
    auto               scan_int(TokenList &L, Token T) -> long;
    auto               scan_keyword(String s) -> bool;
    auto               scan_math_dollar(size_t res, math_list_type type) -> bool;
    auto               scan_math_endcell(Token t) -> bool;
    auto               scan_math_env(size_t res, math_list_type type) -> bool;
    auto               scan_math_kern(symcodes T, subtypes &c) -> ScaledInt;
    auto               scan_math1(size_t res) -> int;
    auto               scan_mathfont_ident() -> size_t;
    auto               scan_pair_ints(Token T, TokenList &L) -> bool;
    auto               scan_reg_num() -> size_t;
    auto               scan_sign() -> bool;
    auto               scan_special_int_d(Token T, long d) -> long;
    auto               scan_style() -> Token;
    auto               scan_twenty_seven_bit_int() -> int;
    auto               sE_arg_nopar() -> std::string;
    auto               sE_arg() -> std::string;
    auto               sE_optarg_nopar() -> std::string;
    auto               shorthand_gdefine(subtypes cmd, String sh, unsigned k) -> Token;
    auto               skip_prefix(const TokenList &L) -> bool;
    auto               special_next_arg() -> std::string;
    auto               sT_arg_nopar() -> std::string;
    auto               sT_optarg_nopar() -> std::string;
    auto               sT_translate(TokenList &L) -> std::string;
    auto               start_scan_math(Math &u, subtypes type) -> bool;
    auto               string_to_write(long chan) -> std::string;
    auto               T_hanl_text() -> Xml *;
    auto               T_hanl_url() -> Xml *;
    auto               T_hline_parse(subtypes c) -> int;
    auto               T_ifthenelse_inner(Token T) -> bool;
    auto               T_ifthenelse(TokenList &A) -> bool;
    auto               T_item_label(unsigned c) -> std::string;
    auto               T_optarg_nopar() -> std::string;
    auto               T_raw_env(bool want_result) -> std::string;
    auto               T_xmllatex() -> std::string;
    auto               to_stringE(TokenList &L) -> std::string;
    auto               token_list_to_att(TokenList &a, Token C, bool spec) -> std::string;
    auto               token_list_to_dim(TokenList &a, Token C, bool spec) -> ScaledInt;
    auto               tokenize_buffer(const std::string &b, const std::string &name) -> TokenList;
    auto               translate_list(TokenList &L) -> Xml *;
    auto               vb_tokens(char32_t test, TokenList &L, bool before) -> bool;
    auto               XKV_parse_filename() -> TokenList;
    auto               xkv_save_keys_aux(bool c, int c2) -> bool;
    auto               xT_arg_nopar() -> Xml *;
    auto               xT_optarg_nopar() -> Xml *;
    void               accent_err1();
    void               accent_err2(Token Y);
    void               accent_err3();
    void               accent_err4();
    void               add_math_label(Xml *res);
    void               add_vspace(Token T, ScaledInt dimen, Xid X);
    void               after_math(bool is_inline);
    void               after_parameter(bool exp, size_t N);
    void               append_glue(Token T, ScaledInt dimen, bool vert);
    void               arg_font(subtypes c);
    void               assign_def_something(bool gbl);
    void               assign_toks(Token T, size_t p, bool gbl);
    void               back_input_braced(TokenList &L);
    void               back_input_pt(bool spec);
    void               back_input_unless_space();
    void               bad_counter0();
    void               bad_counter1(const Buffer &B, EqtbCmdChr &E);
    void               bad_csname(bool orig);
    void               bad_definition(Token name, size_t nb);
    void               bad_delimited(int cl, Token x);
    void               bad_end_env(int cl);
    void               bad_group_char();
    void               bad_macro_prefix(Token X, Token c);
    void               bad_nbargs(int k);
    void               bad_number();
    void               bad_number1(Buffer &B);
    void               bad_redefinition(int rd, Token T);
    void               begin_box(size_t src, subtypes c);
    void               boot_fp();
    void               boot_time();
    void               boot_uclc();
    void               boot_verbatim();
    void               boot_xspace();
    void               box_define(size_t a, Xml *c, bool gbl);
    void               calc_aux(SthInternal &A);
    void               calc_main(internal_type type, SthInternal &res, TokenList &B);
    void               calc_mul_aux(SthInternal &res);
    void               calc_primitive(SthInternal &A);
    void               calc_ratio_eval(long num, long den, SthInternal &res);
    void               calc_spec_mul(RealNumber val, SthInternal &res);
    void               call_define_key(TokenList &L, Token cmd, const std::string &arg, const std::string &fam);
    void               check_builtin_class();
    void               check_language();
    void               check_module_title(TokenList &L);
    void               check_outer_validity();
    void               close_all();
    void               count_days();
    void               counter_boot(const std::string &s, String aux);
    void               counter_overflow(Token T, long n, int nmax);
    void               csname_arg();
    void               date_commands(subtypes c);
    void               datebynumber();
    void               dbl_arg();
    void               declaretopics();
    void               default_bp(Buffer &B, Token T, TokenList &val);
    void               define_something(subtypes chr, bool gbl, symcodes w);
    void               dim_define(size_t a, ScaledInt c, bool gbl);
    void               dimen_from_list0(Token T, TokenList &L);
    void               disable_keys();
    void               do_register_command(bool gbl);
    void               dump_save_stack() const;
    void               E_accent_a();
    void               E_accent();
    void               E_addtoreset();
    void               E_afterelsefi();
    void               E_afterfi();
    void               E_all_of_one(Token T, subtypes c);
    void               E_car(bool first);
    void               E_convert();
    void               E_counter(subtypes c);
    void               E_csname();
    void               E_ensuremath();
    void               E_expandafter();
    void               E_fi_or_else();
    void               E_first_of_four(bool vb, subtypes c);
    void               E_first_of_three(bool vb, subtypes c);
    void               E_first_of_two(bool vb, subtypes c);
    void               E_get_config(unsigned c);
    void               E_if_test(subtypes test, bool negate);
    void               E_ifempty();
    void               E_ifundefined(bool c);
    void               E_ignore_n_args(bool vb, subtypes c);
    void               E_input(subtypes q);
    void               E_iwhile(subtypes cc);
    void               E_latex_ctr();
    void               E_loop();
    void               E_mathversion();
    void               E_multispan();
    void               E_parse_encoding(bool vb, subtypes what);
    void               E_random();
    void               E_scan_up_down();
    void               E_scan_up_down(TokenList &A, TokenList &B, TokenList &c, TokenList &res);
    void               E_setlength(subtypes c);
    void               E_sideset();
    void               E_split();
    void               E_the_traced(Token T, subtypes c);
    void               E_unless();
    void               E_usename(subtypes c, bool vb);
    void               E_user(bool vb, subtypes c, symcodes C);
    void               E_useverb();
    void               E_while(subtypes cc);
    void               E_xspace();
    void               E_zapspace();
    void               english_quotes(CmdChr X);
    void               enter_file_in_table(const std::string &nm, bool ok);
    void               err_one_arg(const TokenList &L);
    void               examine_token(Token T);
    void               exec_calc();
    void               exec_fp_cmd(subtypes i);
    void               exec_fpi_cmd(subtypes i);
    void               expand_first(TokenList &L);
    void               expand_mac(Macro &X);
    void               expand_no_arg(const std::string &s);
    void               expand_no_arg0(Token);
    void               expand_spaces();
    void               expand_twoargs();
    void               expand_verb(unsigned char t);
    void               expand_verb1(TokenList &);
    void               expand_when_ok(bool allow_undef);
    void               expand();
    void               extra_close_brace(int cl);
    void               extra_fi_or_else();
    void               extract_keys(TokenList, std::vector<std::string> &);
    void               fast_new_macro(TokenList &L, Token name);
    void               fetch_box_id(Xml *X);
    void               fetch_name2();
    void               finish_a_cell(Token T, const std::string &a);
    void               finish_counter_cmd(Token first, TokenList &L);
    void               finish_csname(Buffer &b, const std::string &s);
    void               finish_csname(const std::string &b);
    void               finish_iwhile(TokenList &A, Token D);
    void               finish_kvo_bool(Token T, const std::string &fam, const std::string &arg);
    void               finish_no_mathml(bool is_inline, size_t vp);
    void               finish_par_cmd(bool noindent, const std::string &xs);
    void               finish_pers();
    void               finish_trivial_math(Xml *res);
    void               flush_buffer0();
    void               flush_buffer1();
    void               fnhack();
    void               font_has_changed();
    void               formatdate();
    void               fp_boolean(bool res);
    void               fp_calla(Token);
    void               fp_callb(Token);
    void               fp_callc(Token);
    void               fp_calld(Token);
    void               fp_calle(Token);
    void               fp_e_arcsin(subtypes i);
    void               fp_e_csolve();
    void               fp_e_eval();
    void               fp_e_lsolve();
    void               fp_e_pascal();
    void               fp_e_qqsolve();
    void               fp_e_qsolve();
    void               fp_e_upn();
    void               fp_eval_lt(subtypes w);
    void               fp_eval_unarytest(subtypes w);
    void               fp_finish(FpNum X);
    void               fp_parse_error(Token a, Token b);
    void               fp_prepare();
    void               fp_print();
    void               fp_set();
    void               fp_setseed();
    void               fp_special_expand(TokenList &B);
    void               french_punctuation(CmdChr X);
    void               get_date_ctrs(long &year, size_t &month, size_t &day);
    void               get_def_nbargs(Macro *X, Token name);
    void               get_new_command_aux(const TokenList &a);
    void               get_x_token_or_active_char(symcodes &a, subtypes &b);
    void               glue_define(size_t a, Glue c, bool gbl);
    void               grab_env(TokenList &v);
    void               ignore_arg();
    void               ignore_optarg();
    void               implicit_par(subtypes c);
    void               improper_alpha();
    void               includegraphics(subtypes c);
    void               initialise_font();
    void               insert_endline_char();
    void               insert_every_bib();
    void               insert_hook(long n);
    void               insert_relax();
    void               internal_choice_key();
    void               internal_define_key_default(Token T, TokenList &L);
    void               internal_define_key(Token T);
    void               interpret_genfrac_cmd(size_t res, subtypes k, CmdChr W);
    void               interpret_math_cmd(size_t res, subtypes c);
    void               interpret_mathchoice_cmd(size_t res, subtypes k, CmdChr W);
    void               invalid_key(Token T, const std::string &key, const TokenList &val);
    void               is_date_valid();
    void               key_ifundefined();
    void               kvo_bool_key();
    void               kvo_bool_opt();
    void               kvo_comp_opt();
    void               kvo_family_etc(subtypes k);
    void               kvo_family(subtypes k);
    void               kvo_process();
    void               kvo_string_opt();
    void               kvo_void_key();
    void               kvo_void_opt();
    void               leave_h_mode();
    void               leave_v_mode();
    void               lost_if(Token T, int L);
    void               M_cons();
    void               M_cons(Token cmd, TokenList &L);
    void               M_declare_math_operator();
    void               M_def(bool edef, bool gbl, symcodes what, rd_flag fl);
    void               M_extension(subtypes cc);
    void               M_future_let(bool gbl);
    void               M_let_fast(Token a, Token b, bool gbl);
    void               M_let(bool gbl);
    void               M_let(subtypes chr, bool gbl);
    void               M_let(Token A, bool global, bool redef);
    void               M_let(Token a, Token b, bool gbl, bool redef);
    void               M_new_env(rd_flag redef);
    void               M_new_thm();
    void               M_newboolean(subtypes c);
    void               M_newcommand(rd_flag redef);
    void               M_newif_aux(Token T, const std::string &s, bool b);
    void               M_newif();
    void               M_prefixed_aux(bool gbl);
    void               M_prefixed();
    void               M_shorthand_define(subtypes cmd, bool gbl);
    void               M_shortverb(subtypes x);
    void               M_xray(subtypes c);
    void               mac_define(Token a, Macro *b, bool gbl, rd_flag redef, symcodes what);
    void               make_catcodes();
    void               make_token(String s);
    void               makelabel();
    void               minus_sign(CmdChr X);
    void               missing_argument();
    void               missing_close_brace(int cl);
    void               missing_equals(Token T);
    void               missing_flush();
    void               missing_number();
    void               missing_open_brace();
    void               month_day(subtypes c);
    void               more_bootstrap();
    void               multiple_label(const std::string &name, int L, const std::string &f);
    void               multiply_dim(RealNumber val, long v);
    void               need_array_mode();
    void               need_bib_mode();
    void               new_constant(String name, size_t max_val, subtypes alloc_pos, symcodes c);
    void               new_constant(subtypes c);
    void               new_font();
    void               new_macro(const std::string &s, Token name);
    void               new_macro(TokenList &L, Token name, bool gbl);
    void               new_macro(TokenList &L, Token name);
    void               new_prim(String a, String b);
    void               new_prim(String a, TokenList &b);
    void               new_prim(Token name, TokenList &L);
    void               new_prim(Token, Token);
    void               new_primx(String a, String b);
    void               new_xref(Xml *val, std::string v, bool err);
    void               next_date();
    void               next_day();
    void               next_from_list();
    void               no_arg_font();
    void               no_extension(AttList &AL, const std::string &s);
    void               numberwithin();
    void               old_font();
    void               one_of_two(bool which);
    void               one_of_two(TokenList &A, TokenList &B, bool which);
    void               open_tex_file(const std::string &f, bool seen_star);
    void               opt_to_mandatory();
    void               out_warning(Buffer &B, msg_type what);
    void               parshape_aux(subtypes m);
    void               pass_text(Token Tfe);
    void               pop_all_levels();
    void               pop_input_stack(bool vb);
    void               pop_level(boundary_type v);
    void               prefix_error(bool b_global, symcodes K);
    void               prev_date();
    void               print_token(std::ostream &fp, Token x);
    void               process_char(char32_t c);
    void               process_char(int s);
    void               process_char(size_t c);
    void               process_char(uchar c);
    void               process_string(String s);
    void               push_input_stack(const std::string &name, bool restore_at, bool re);
    void               push_level(boundary_type v);
    void               push_module();
    void               push_module(const std::string &aux);
    void               push_tpa();
    void               ratio_evaluate(TokenList &A, TokenList &B, SthInternal &res);
    void               read_into(TokenList &X);
    void               read_mac_body(TokenList &L, bool exp, size_t N);
    void               read_one_space();
    void               read_toks_edef(TokenList &L);
    void               refstepcounter_inner(TokenList &L, bool star);
    void               refstepcounter();
    void               refstepcounter(const std::string &s, bool star);
    void               refstepcounter(TokenList &L, bool star);
    void               remove_element(TokenList &A, TokenList &B, Token C);
    void               remove_initial_space_and_back_input();
    void               remove_initial_space_relax();
    void               remove_initial_space();
    void               runaway(int cl);
    void               save_font();
    void               scan_box(size_t bc);
    void               scan_dimen(bool mu, bool inf, glue_spec &co, bool shortcut);
    void               scan_dimen(bool mu, Token T);
    void               scan_double(RealNumber &res);
    void               scan_expr_arg(Token T, internal_type X);
    void               scan_expr(subtypes m);
    void               scan_group3(TokenList &res, int cl, bool exp, size_t N);
    void               scan_group4(TokenList &res, int cl);
    void               scan_hbox(size_t ptr, subtypes c);
    void               scan_ignore_group();
    void               scan_left_brace_and_back_input();
    void               scan_left_brace();
    void               scan_math_endcell_ok(size_t res);
    void               scan_math_hbox(size_t res, subtypes c);
    void               scan_math_mi(size_t res, subtypes c, subtypes k, CmdChr W);
    void               scan_math_rel(subtypes c, size_t res);
    void               scan_math_tag(subtypes c);
    auto               scan_math(size_t res, math_list_type type) -> bool;
    void               scan_math2(int, math_list_type, boundary_type);
    [[nodiscard]] auto scan_math3(size_t X, math_list_type T, int m) -> bool;
    void               scan_optional_equals();
    void               scan_prime();
    void               scan_rule(subtypes c);
    void               scan_something_internal(internal_type level, bool negative);
    void               scan_something_internal(internal_type level);
    void               scan_toks_absorb_expand();
    void               scan_toks_absorb();
    void               scan_unit(RealNumber R);
    void               see_font_change(subtypes c);
    void               see_new_id_spec(bool);
    void               select_math_font();
    void               selective_sanitize();
    void               set_boolean();
    void               set_counter(Token T, long c);
    void               set_date_ctrs(long year, size_t month, size_t day);
    void               setkeys(bool c);
    void               short_verb_error(Token T, Token t, unsigned x);
    void               skip_group(TokenList &);
    void               skip_group0(TokenList &L);
    void               skip_initial_space_and_back_input();
    void               skip_initial_space();
    void               skip_over_parens();
    void               solve_cite(bool user);
    void               special_fvset();
    void               start_a_cell(bool started);
    void               start_a_row(long a, const std::string &s = "");
    void               start_paras(int y, const std::string &Y, bool star);
    void               store_new_line(int n, bool vb);
    void               string_define(size_t a, const std::string &c, bool gbl);
    void               strip_pt();
    void               T_addtomacro(bool exp);
    void               T_aftergroup();
    void               T_ampersand();
    void               T_arg_local();
    void               T_arg();
    void               T_arg1(const std::string &y);
    void               T_at_end_of_class();
    void               T_atdocument(subtypes c);
    void               T_backslash();
    void               T_barnodeconnect(const std::string &W);
    void               T_bauteursediteurs(subtypes c);
    [[nodiscard]] auto T_begin(const std::string &s) -> bool;
    void               T_begindocument();
    [[nodiscard]] auto T_beginend(symcodes x) -> bool;
    void               T_bezier(subtypes c);
    void               T_bibitem();
    void               T_biblio();
    void               T_bibliostyle();
    void               T_bpers();
    void               T_cap_or_note(bool cap);
    void               T_case_shift(subtypes c);
    void               T_catperso();
    void               T_change_element_name();
    void               T_citation();
    void               T_cite_one();
    void               T_cite_type();
    void               T_cite(subtypes sw, TokenList &prenote, std::string &type);
    void               T_cite(subtypes sw);
    void               T_cititem();
    void               T_class_error(subtypes c);
    void               T_color(subtypes c);
    void               T_cr();
    void               T_cst1(subtypes c);
    void               T_cst2(subtypes c);
    void               T_curves(subtypes c);
    void               T_dashline(subtypes c);
    void               T_declare_option_star();
    void               T_declare_options();
    void               T_define_verbatim_env();
    void               T_ding();
    void               T_documentclass(bool bad);
    void               T_empty_bibitem();
    void               T_end_tabular(subtypes c);
    void               T_end_the_biblio();
    void               T_end_theorem();
    [[nodiscard]] auto T_end(const std::string &s) -> bool;
    void               T_enddocument(subtypes c);
    void               T_endv();
    void               T_epsfbox();
    void               T_error();
    void               T_etex();
    void               T_execute_options();
    void               T_fancy();
    void               T_fancy(String s, TokenList &L);
    void               T_fbox_dash_box();
    void               T_fbox_rotate_box();
    void               T_fbox(subtypes cc);
    void               T_figure_table_end(bool is_fig);
    void               T_figure_table(symcodes x, subtypes c);
    void               T_filecontents(subtypes spec);
    void               T_float(subtypes c);
    void               T_fonts(subtypes c);
    void               T_glo();
    void               T_gloss(bool c);
    void               T_glossaire_end();
    void               T_glossaire();
    void               T_grabenv();
    void               T_hanl(subtypes c);
    void               T_hline(subtypes c);
    void               T_hspace(subtypes c);
    void               T_if_package_later(bool c);
    void               T_if_package_loaded(bool type);
    void               T_if_package_with(bool c);
    void               T_ifdefinable();
    void               T_ifnextchar(bool c);
    void               T_ifstar();
    void               T_ifthenelse();
    void               T_ignoreA();
    void               T_index(subtypes c);
    void               T_input(subtypes q);
    void               T_inputclass();
    void               T_isin();
    void               T_item(subtypes c);
    void               T_keywords();
    void               T_label(subtypes c);
    void               T_line(subtypes c);
    void               T_linethickness(subtypes c);
    void               T_listenv_end();
    void               T_listenv(symcodes x);
    void               T_load_with_options(bool c);
    void               T_makebox(bool framed, Token C);
    void               T_mark(subtypes c);
    auto               T_math(subtypes type) -> bool;
    void               T_matter(subtypes c);
    void               T_mbox(subtypes c);
    void               T_minipage();
    void               T_moreinfo_end();
    void               T_multicolumn();
    void               T_multiput();
    void               T_newcolumn_type();
    void               T_newline();
    void               T_newthheorem();
    void               T_node();
    void               T_nodebox(const std::string &W);
    void               T_nodecircle(const std::string &W);
    void               T_nodeconnect(const std::string &W);
    void               T_nodecurve(const std::string &W);
    void               T_nodepoint();
    void               T_nodetriangle(const std::string &W);
    void               T_omitcite();
    void               T_optarg();
    void               T_option_not_used();
    void               T_par1();
    void               T_par1(const std::string &u);
    void               T_paras(subtypes x);
    void               T_participants_end();
    void               T_participants(subtypes x);
    void               T_pass_options(bool c);
    void               T_pers_ra();
    void               T_pers();
    void               T_picture();
    void               T_process_options_aux(TokenList &action);
    void               T_process_options();
    void               T_provides_package(bool c);
    void               T_put(subtypes c);
    void               T_ra_startdoc();
    void               T_raisebox();
    void               T_reevaluate();
    void               T_ref(bool is_ref);
    void               T_remove_element();
    void               T_save_box(bool simple);
    void               T_saveverb();
    void               T_scan_glue(subtypes c);
    void               T_setmode();
    void               T_specimp(subtypes c);
    void               T_start_tabular(subtypes c);
    void               T_start_the_biblio();
    void               T_start_theorem(subtypes c);
    void               T_startprojet(String proj, String loc);
    void               T_subequations(bool start);
    void               T_subfigure();
    void               T_testopt();
    void               T_translate(TokenList &X);
    void               T_trees(subtypes c);
    void               T_twodims(std::string &A, std::string &B, Token C);
    void               T_twoints(std::string &A, std::string &B, Token C);
    void               T_twoints(TokenList &A, TokenList &B);
    void               T_typein();
    void               T_un_box(subtypes c);
    void               T_unimp(subtypes c);
    void               T_unimplemented_font(subtypes c);
    void               T_url();
    void               T_use_counter();
    void               T_use_counter(const std::string &s);
    void               T_usefont();
    void               T_usepackage();
    void               T_verb(char32_t t);
    void               T_verbatim();
    void               T_verbatim(int my_number, Token style, Token pre, Token post);
    void               T_whiledo();
    void               T_xfancy();
    void               T_xkv_for(subtypes c);
    void               T_xmladdatt(subtypes c);
    void               T_xmlelt(subtypes w);
    void               T_xmlenv_end(subtypes c);
    void               T_xmlenv(subtypes c);
    void               TM_fonts();
    void               TM_math_fonts(Math &x) const;
    void               TM_tabular_arg(Xid id);
    void               token_for_show(bool lg, const CmdChr &val, Buffer &B);
    void               token_from_list(Token t);
    void               token_list_define(size_t p, TokenList &c, bool gbl);
    void               token_show(int what, Buffer &B);
    void               trace_if(int k) const;
    void               translate_char(CmdChr X);
    void               translate_char(uchar c1, uchar c2);
    void               translate_font_size();
    void               translate02() const;
    [[nodiscard]] auto translate03() -> bool;
    void               translate1();
    void               umlaut_bad();
    void               umlaut();
    void               undefined_env(const std::string &s);
    void               undefined_mac();
    void               unexpected_close_brace();
    void               unskip_group(String s);
    void               upn_eval(TokenList &l);
    void               use_a_package(const std::string &name, bool type, const std::string &date, bool builtin);
    void               user_XML_fetch();
    void               user_XML_modify(subtypes c);
    void               user_XML_swap(subtypes c);
    void               verb_error(Token T, int msg);
    void               wrong_mode(const std::string &s);
    void               wrong_pop(Token T, const std::string &a, const std::string &b);
    void               xgetfontsize();
    void               xkv_class_prefix();
    void               xkv_declare_option();
    void               xkv_execute_options();
    void               xkv_fetch_prefix_family();
    void               xkv_fetch_prefix();
    void               xkv_makehd();
    void               xkv_makehd(TokenList &L);
    void               xkv_merge(bool gbl, int type, TokenList &L, bool mg);
    void               xkv_pass_options(bool c);
    void               XKV_pox();
    void               XKV_process_execute(bool psw);
    void               xkv_process_options();
    void               xkv_testopta();
    void               xkv_unsave_keys(bool c);
    void               xml_name(Xml *x, internal_type level);
    void               xsetfontsize();

    auto l3_get_cat(symcodes &a, subtypes &b, Token caller) -> bool;
    auto l3_get_name(Token T) -> bool;
    auto l3_parms_from_ac(long n, Token T, bool s) -> TokenList;
    auto l3_read_int(Token T) -> long;
    auto L3_split_next_name() -> bool;
    auto l3_to_string(subtypes c, TokenList &L) -> std::string;
    void define_definer(String base, String nsig, String osig);
    void define_definer(String base);
    void E_cat_ifeq(subtypes c);
    void E_l3_ifx(subtypes c);
    void E_l3expand_aux(subtypes c);
    void E_l3expand_base(subtypes c);
    void E_l3noexpand(subtypes c);
    void E_l3str_case(subtypes c);
    void E_l3str_ifeq(subtypes c);
    void E_pdfstrcmp();
    void E_prg_return(unsigned c);
    void generate_from_sig();
    void l3_after_cond(Token T, bool test, subtypes c);
    void L3_check_cmd(subtypes c);
    void L3_eq_conditional(subtypes s);
    void l3_expand_f(TokenList &L);
    void l3_expand_N(TokenList &L);
    void l3_expand_o(TokenList &L);
    void l3_expand_Vv(TokenList &L, bool spec);
    void l3_expand_x(TokenList &L);
    void L3_generate_form(subtypes c, TokenList parms, TokenList body, subtypes s);
    void l3_generate_variant();
    void l3_generate_variant(const std::string &var, bool prot, Token orig);
    void l3_generate_variant(String orig, String var);
    void L3_getid();
    void L3_logid();
    void L3_new_conditional_aux(TokenList &arg_spec, subtypes s);
    void L3_new_conditional_parm(subtypes s);
    void L3_new_conditional(subtypes s);
    void l3_new_token_list(subtypes c);
    void l3_reexpand_o(TokenList &L);
    void L3_set_cat_code(subtypes c);
    void L3_set_num_code(subtypes c);
    void l3_tl_concat(subtypes c);
    void l3_tl_put_left(subtypes c);
    void l3_tl_set(subtypes c);
    void l3_token_check(subtypes c);
    void L3_user_split_next_name(bool base);
    void T_scantokens(TokenList &L);
    void tl_set_rescan(subtypes c);
    void Tl3_gen_from_ac(subtypes c);

    // general methods and variables used from packages and boot helpers
    CmdChr cur_cmd_chr;
    Token  cur_tok;
    void   extended_chars(size_t c);
    auto   get_token() -> bool;
    void   T_fonts(const std::string &x);

    // \todo specific methods used in packages, belong there
    // \todo static methods that would fit better elsewhere
    static auto ileave_v_mode() -> Xid;
    static auto last_att_list() -> AttList &;
    static void add_bib_marker(bool force);
    static void push_save_stack(SaveAuxBase *v);
    static void T_titlepage(size_t v);
};

inline Parser the_parser;
