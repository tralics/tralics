#pragma once
#include "../txeqtb.h"
#include "Buffer.h"
#include <unordered_map>

// This is the main hash table.
class Hashtab : public std::vector<std::optional<std::string>> {
private:
    std::unordered_map<std::string, size_t> map;

public:
    std::array<Token, 15>             my_mathfont_table;
    std::array<Token, 5>              genfrac_mode;
    std::array<Equivalent, eqtb_size> eqtb;

    int usage_normal{0}, usage_unhashed{0};

    Hashtab();

    auto locate(const std::string &s) -> Token; // used by primitive, etc
    auto locate(const Buffer &b) -> Token;      // used by primitive, etc
    auto hash_find(const std::string &s) -> size_t;
    auto primitive(String s, symcodes c, subtypes v = zero_code) -> Token;
    auto nohash_primitive(const std::string &a, CmdChr b) -> Token;
    void eval_let(String a, String b);
    auto eval_letv(String a, String b) -> Token;
    void eval_let_local(String a, String b);
    auto is_defined(const Buffer &b) -> bool;

    void boot_base();
    void boot_fancyhdr();
    void boot_etex();
    void boot_latex3();
    void boot_keyval();

    Token par_token, OB_token, CB_token, dollar_token;
    Token verb_token, noindent_token, small_token, textvisiblespace_token;
    Token end_token, frozen_dont_expand, relax_token, frozen_relax_token, sendgroup_token, textbf_token, frozen_endcsname, begin_token,
        eof_token;
    Token frozen_undef_token, equals_token, char_token;
    Token number_token, roman_numeral_token, Roman_numeral_token;
    Token advance_token, global_token, atalph_token, atAlph_token, fnsymbol_token;
    Token roman_token, Roman_token, alph_token, Alph_token;
    Token iftrue_token, iffalse_token, let_token, urlfont_token;
    Token fi_token, iterate_token, repeat_token, expandafter_token, endcsname_token;
    Token csname_token, xkv_cc_token;
    Token isodd_token, lengthtest_token, boolean_token, equal_token;
    Token or_token, and_token, not_token, ratio_token, real_token, calc_token;
    Token OR_token, AND_token, NOT_token, isundefined_token;
    Token frozen_protection, prime_token, elt_token, killcounter_token;
    Token math_OB_token, math_CB_token, text_OB_token, text_CB_token;
    Token real_end_token, error_token, tt_token, dagger_token, ddagger_token;
    Token ensuremath_token, vbar_token, atbd_token, notprerr_token;
    Token hat_token, zero_token, one_token, comma_token, dot_token, star_token;
    Token plus_token, minus_token, tilda_token, space_token, newline_token;
    Token verbatim_number_font, verbatim_font, temp_token, unitlength_token;
    Token verb_prefix_token, verbatim_prefix_token, citeone_token, citesimple_token;
    Token end_natcite_token, at_one_token, at_zero_token, at_align_token;
    Token alt_rapers_token, alt_pers_token, crwithargs_token, of_token;
    Token ifdim_token, ifnum_token, iwhilenum_token, iwhiledim_token, iwhilesw_token, do_token, bth_token, eth_token, arabic_token,
        gobble_token, gobbletwo_token, else_token, endv_token, cr_token, refstepcounter_token, hspace_token, footcite_sep_token,
        footcite_pre_token, cite_punct_token, operatorname_token, itemlabel_token, vspace_token;
    Token xscale_token, yscale_token, xscaley_token, yscalex_token, refname_token;
    Token cite_type_token, cite_prenote_token, lbrace_token, rbrace_token;
    Token backslash_token, headercr_token, mathversion_token, tag_token, qquad_token, xtag_token, ytag_token, xtag1_token, ytag1_token,
        apostrophe_token, cite_e_token, cite_i_token, ref_i_token, label_i_token, ensuremath_i_token, nocase_e_token, nocase_i_token,
        ref_token, label_token, infalse_token, intrue_token, nil_token, CurrentOption_token, xkv_for_i_token, CurrentOptionKey_token,
        CurrentOptionValue_token, forloop_token, doubleat_token, def_token, iforloop_token, tforloop_token;
    Token footnote_token, dblarg_token;
    Token mmlprescripts_token, mmlmultiscripts_token, mmlnone_token, mathop_token, limits_token, begingroup_token, endgroup_token,
        allowbreak_token;
    Token increqnum_token, makelabel_token, stepcounter_token;
    Token nomathsw0_token, nomathsw1_token;
    Token xkv_resa_token, xkv_warn_token, xkv_prefix_token, xkv_fams_token, xkv_na_token, xkv_rm_token, xkv_tfam_token, usevalue_token,
        xkv_header_token, xkv_tkey_token, gsavevalue_token, savevalue_token, empty_token, composite_token;
    Token ExplFileName_token, ExplFileDate_token, ExplFileVersion_token, ExplFileDescription_token;
    Token last_tok;
};
