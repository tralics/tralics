#pragma once
#include "EQTB.h"
#include "Token.h"
#include <unordered_map>
#include <vector>

// This is the main hash table.
class Hashtab : public std::vector<std::string> {
private:
    std::unordered_map<std::string, size_t> map;

public:
    std::array<Token, 15>                  my_mathfont_table;
    std::array<Token, 5>                   genfrac_mode;
    std::unordered_map<size_t, EqtbCmdChr> eqtb;

    int usage_normal{0}, usage_unhashed{0};

    Hashtab();

    auto locate(const std::string &s) -> Token;
    auto hash_find(const std::string &s) -> size_t;
    auto primitive(const std::string &s, symcodes c, subtypes v = zero_code) -> Token;
    auto nohash_primitive(const std::string &a, CmdChr b) -> Token;
    auto eval_let(const std::string &a, const std::string &b) -> Token;
    void eval_let_local(const std::string &a, const std::string &b);
    auto is_defined(const std::string &b) -> bool;

    void boot_fancyhdr();
    void boot_etex();
    void boot_latex3();
    void boot_keyval();

    Token par_token, OB_token, CB_token, dollar_token, verb_token, noindent_token, textvisiblespace_token, frozen_dont_expand, relax_token,
        frozen_relax_token, sendgroup_token, frozen_endcsname, eof_token, frozen_undef_token, equals_token, char_token, number_token,
        roman_numeral_token, Roman_numeral_token, advance_token, global_token, atalph_token, atAlph_token, fnsymbol_token, roman_token,
        Roman_token, alph_token, Alph_token, iftrue_token, iffalse_token, let_token, urlfont_token, fi_token, iterate_token, repeat_token,
        expandafter_token, endcsname_token, csname_token, xkv_cc_token, isodd_token, lengthtest_token, boolean_token, equal_token, or_token,
        and_token, not_token, ratio_token, real_token, calc_token, OR_token, AND_token, NOT_token, isundefined_token, frozen_protection,
        prime_token, elt_token, killcounter_token, real_end_token, dagger_token, ddagger_token, ensuremath_token, vbar_token, atbd_token,
        notprerr_token, hat_token, zero_token, comma_token, dot_token, star_token, plus_token, minus_token, tilda_token, space_token,
        newline_token, verbatim_number_font, verbatim_font, temp_token, unitlength_token, verb_prefix_token, verbatim_prefix_token,
        citeone_token, citesimple_token, end_natcite_token, at_one_token, at_zero_token, crwithargs_token, ifdim_token, ifnum_token,
        iwhilenum_token, iwhiledim_token, iwhilesw_token, do_token, bth_token, eth_token, arabic_token, gobble_token, gobbletwo_token,
        else_token, endv_token, cr_token, footcite_sep_token, footcite_pre_token, cite_punct_token, operatorname_token, itemlabel_token,
        xscale_token, yscale_token, xscaley_token, yscalex_token, refname_token, cite_type_token, cite_prenote_token, lbrace_token,
        rbrace_token, headercr_token, mathversion_token, xtag_token, ytag_token, xtag1_token, ytag1_token, apostrophe_token, cite_e_token,
        cite_i_token, ref_i_token, label_i_token, ensuremath_i_token, nocase_e_token, nocase_i_token, ref_token, label_token, infalse_token,
        intrue_token, nil_token, CurrentOption_token, xkv_for_i_token, CurrentOptionKey_token, CurrentOptionValue_token, forloop_token,
        doubleat_token, def_token, iforloop_token, tforloop_token, footnote_token, dblarg_token, mmlprescripts_token, mmlmultiscripts_token,
        mmlnone_token, mathop_token, limits_token, begingroup_token, endgroup_token, allowbreak_token, stepcounter_token, nomathsw0_token,
        nomathsw1_token, xkv_resa_token, xkv_warn_token, xkv_prefix_token, xkv_fams_token, xkv_na_token, xkv_rm_token, xkv_tfam_token,
        usevalue_token, xkv_header_token, xkv_tkey_token, gsavevalue_token, savevalue_token, empty_token, composite_token,
        ExplFileName_token, ExplFileDate_token, ExplFileVersion_token, ExplFileDescription_token, last_tok;
};

inline Hashtab hash_table;
