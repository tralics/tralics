// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


#include "tralics.h"
const char* txtoken1_rcsid=
  "$Id: txtoken1.C,v 2.77 2015/08/03 16:12:51 grimm Exp $";

// We define functions that return the name of a token;

static Buffer make_name_buffer; // used for token names

namespace tralics_ns {
  String make_name(String x, int y);
  String strip_end(String s);
}

using tralics_ns::make_name;
using tralics_ns::strip_end;


// This converts `endfoo' to `foo'. It does not check that the
// string starts with `end', just that is is not zero.
String tralics_ns::strip_end(String s)
{
  if(!s) return 0;
  return s+3;
}


// This returns a temporary associated to \skip24
// (given the name skip and the integer 24)
String tralics_ns::make_name(String x, int y)
{
  make_name_buffer << bf_reset << x << y;
  return make_name_buffer.c_str();
}

String CmdChr::specchar_cmd_name() const
{
  make_name_buffer << bf_reset << "Character ";
  make_name_buffer.push_back16u(chr);
  return make_name_buffer.c_str();
  
}

String CmdChr::token_error_name() const
{
  switch (chr) {
  case classerror_code: return "ClassError";
  case classwarning_code: return "ClassWarning";
  case classwarningnoline_code: return "ClassWarningNoLine";
  case classinfo_code: return "ClassInfo";
  case packageerror_code: return "PackageError";
  case packagewarning_code: return "PackageWarning";
  case packagewarningnoline_code: return "PackageWarningNoLine";
  case packageinfo_code: return "PackageInfo";
  case genericerror_code: return "GenericError";
  case genericinfo_code: return "GenericInfo";
  case genericwarning_code: return "GenericWarning";
  case messagebreak_code: return "MessageBreak";
  case latexerror_code: return "@latex@error";
  case latexwarning_code: return "@latex@warning";
  case latexwarningnoline_code: return "@latex@warning@no@line";
  case latexinfo_code: return "@latex@info";
  case latexinfonoline_code: return "@latex@info@no@line";
  default: return 0;
  }
}

String CmdChr::token_monthday_name() const
{
  switch(chr) {
  case md_frenchm_code: return "@monthnamefrench";
  case md_frenchd_code: return "@daynamefrench";
  case md_spanishm_code: return "@monthnamespanish";
  case md_spanishd_code: return "@daynamespanis";
  case md_germanm_code: return "@monthnamegerman";
  case md_germand_code: return "@daynamegerman";
  case md_englishm_code: return "@monthnameenglish";
  case md_englishd_code: return "@daynameenglish";
  default: return 0;
  }
}


String CmdChr::token_xkeyval_name() const
{
  switch(chr) {
  case boot_keyval_code: return "tralics@boot@keyval";
  case definekey_code: return "define@key";
  case xdefinekey_code: return "define@key";
  case define_cmdkey_code: return "define@cmdkey";
  case define_cmdkeys_code: return "define@cmdkeys";
  case define_boolkey_code: return "define@boolkey";
  case define_boolkeys_code: return "define@boolkeys";
  case save_keys_code: return "savekeys";
  case gsave_keys_code: return "gsavekeys";
  case delsave_keys_code: return "delsavekeys";
  case gdelsave_keys_code: return "gdelsavekeys";
  case unsave_keys_code: return  "unsavekeys";
  case gunsave_keys_code: return  "gunsavekeys";
  case preset_keys_code: return "presetkeys";
  case gpreset_keys_code: return "gpresetkeys";
  case delpreset_keys_code: return  "delpresetkeys";
  case gdelpreset_keys_code: return  "gdelpresetkeys";
  case unpreset_keys_code: return "unpresetkeys";
  case gunpreset_keys_code: return "gunpresetkeys";
  case setkeys_code: return "setkeys";
  case setrmkeys_code: return "setrmkeys";
  case declare_optionsX_code: return "DeclareOptionX";
  case execute_optionsX_code: return "ExecuteOptionsX";
  case process_optionsX_code: return "ProcessOptionsX";
  default: return 0;
  }
}



String CmdChr::token_specialmath_name() const
{
  switch(chr) {
  case acute_code: return "acute"; 
  case grave_code: return "grave"; 
  case ddddot_code: return "ddddot"; 
  case mathring_code: return "mathring"; 
  case dddot_code: return "dddot"; 
  case ddot_code: return "ddot"; 
  case tilde_code: return "tilde"; 
  case widetilde_code:  return  "widetilde";
  case bar_code: return  "bar";
  case breve_code: return  "breve"; 
  case check_code: return  "check"; 
  case hat_code: return  "hat"; 
  case widehat_code:  return  "widehat"; 
  case vec_code: return  "vec"; 
  case overrightarrow_code: return  "overrightarrow"; 
  case overleftarrow_code: return  "overleftarrow"; 
  case overleftrightarrow_code: return  "overleftrightarrow"; 
  case underrightarrow_code: return  "underrightarrow"; 
  case underleftarrow_code: return  "underleftarrow"; 
  case underleftrightarrow_code: return  "underleftrightarrow"; 
  case xleftarrow_code: return  "xleftarrow"; 
  case xrightarrow_code: return  "xrightarrow"; 
  case phantom_code: return "phantom";
  case vphantom_code: return "vphantom";
  case hphantom_code: return "hphantom";
  case smash_code: return "smash";
  case atsmash_code: return "@smash";
  case accentset_code: return "accentset";
  case underaccent_code: return "underaccent";
  case undertilde_code: return "undertilde"; 
  case dot_code: return  "dot"; 
  case qopname_code: return  "qopname"; 
  case operatorname_code: return  "operatorname"; 
  case operatornamestar_code: return  "operatornamewithlimits"; 
  case overline_code: return  "overline"; 
  case underline_code: return  "underline"; 
  case stackrel_code: return  "stackrel"; 
  case underset_code: return  "underset"; 
  case overset_code: return  "overset"; 
  case mathchoice_code: return "mathchoice";
  case multicolumn_code: return "multicolumn";
  case frac_code: return  "frac"; 
  case overbrace_code: return  "overbrace"; 
  case underbrace_code: return  "underbrace"; 
  case dfrac_code: return  "dfrac";
  case tfrac_code: return  "tfrac";
  case cfrac_code: return  "cfrac";
  case binom_code: return  "binom";
  case dbinom_code: return  "dbinom";
  case tbinom_code: return  "tbinom";
  case boxed_code: return  "boxed";
  case sqrt_code: return  "sqrt";
  case root_code: return  "@root";
  case mathbox_code: return  "mathbox";
  case genfrac_code: return  "genfrac";
  case mathmi_code: return "mathmi";
  case mathmo_code: return "mathmo";
  case mathmn_code: return "mathmn";
  case mathci_code: return "mathco";
  case mathcn_code: return "mathcn";
  case mathcsymbol_code: return "mathcsymbol";
  case math_attribute_code: return "mathattribute"; 
  case multiscripts_code: return "mmlmultiscripts";
  case cell_attribute_code: return "cellattribute";
  case row_attribute_code: return "rowattribute";
  case thismath_attribute_code: return "thismathattribute";
  case formula_attribute_code: return "formulaattribute";
  case table_attribute_code: return "tableattribute";
  default: return 0;
  }
}

// For debug: returns the name of a command
String CmdChr::token_math_name() const
{
  switch (int (chr)) {
  case xml_zero_space_loc: return "zerospace";
  case xml_thinmu_space_loc: return ",";
  case xml_thickmu_space_loc: return ";";
  case xml_medmu_space_loc: return ":";
  case xml_thinmuneg_space_loc: return "!";
  case xml_quad_space_loc: return "~";
  case alpha_code: return "alpha";
  case beta_code: return "beta";
  case gamma_code: return "gamma";
  case delta_code: return "delta";
  case epsilon_code: return "epsilon";
  case varepsilon_code: return "varepsilon";
  case zeta_code: return "zeta";
  case eta_code: return "eta";
  case theta_code: return "theta";
  case iota_code: return "iota";
  case kappa_code: return "kappa";
  case lambda_code: return "lambda";
  case mu_code: return "mu";
  case nu_code: return "nu";
  case xi_code: return "xi";
  case pi_code: return "pi";
  case rho_code: return "rho";
  case sigma_code: return "sigma";
  case tau_code: return "tau";
  case upsilon_code: return "upsilon";
  case phi_code: return "phi";
  case chi_code: return "chi";
  case psi_code: return "psi";
  case omega_code: return "omega"; 
  case varpi_code: return "varpi";
  case varrho_code: return "varrho";
  case varsigma_code: return "varsigma";
  case varphi_code: return "varphi";
  case vartheta_code: return "vartheta";
  case varkappa_code: return "varkappa";
  case Gamma_code: return "Gamma";
  case Delta_code: return "Delta";
  case Theta_code: return "Theta";
  case Lambda_code: return "Lambda";
  case Xi_code: return "Xi";
  case Sigma_code: return "Sigma";
  case Upsilon_code: return "Upsilon";
  case Phi_code: return "Phi";
  case Pi_code: return "Pi";
  case Psi_code: return "Psi";
  case Omega_code: return "Omega";
  case hbar_code: return "hbar";
  case ell_code:  return "ell";
  case wp_code: return "wp";
  case Re_code: return "Re";
  case Im_code: return "Im";
  case partial_code: return "partial";
  case infty_code: return "infty";
  case emptyset_code: return "emptyset";
  case nabla_code: return "nabla";
  case surd_code: return "surd";
  case top_code: return "top";
  case bottom_code: return "bottom";
  case bot_code: return "bot";
  case angle_code: return "angle";
  case triangle_code: return "triangle";
  case triangleq_code: return "triangleq";
  case log_code: return "log";
  case lg_code: return "lg";
  case ln_code: return "ln";
  case lim_code: return "lim";
  case limsup_code: return "limsup";
  case liminf_code: return "liminf";
  case injlim_code: return "injlim";
  case projlim_code: return "projlim";
  case varlimsup_code: return "varlimsup";
  case varliminf_code: return "varliminf";
  case varinjlim_code: return "varinjlim";
  case varprojlim_code: return "varprojlim";
  case sin_code: return "sin";
  case in_code: return "in";
  case mod_code: return "@mod";
  case arcsin_code: return "arcsin";
  case sinh_code: return "sinh";
  case cos_code: return "cos";
  case arccos_code: return "arccos";
  case cosh_code: return "cosh";
  case tan_code: return "tan";
  case arctan_code: return "arctan";
  case tanh_code: return "tanh";
  case cot_code: return "cot";
  case coth_code: return "coth";
  case sec_code: return "sec";
  case csc_code: return "csc";
  case max_code: return "max";
  case min_code: return "min";
  case sup_code: return "sup";
  case inf_code: return "inf";
  case arg_code: return "arg";
  case ker_code: return "ker";
  case dim_code: return "dim";
  case hom_code: return "hom";
  case det_code: return "det";
  case exp_code: return "exp";
  case Pr_code: return "Pr";
  case gcd_code: return "gcd";
  case deg_code: return "deg";

  case forall_code: return "forall";
  case exists_code: return "exists";
  case coprod_code: return "coprod";
  case bigvee_code: return "bigvee";
  case bigwedge_code: return "bigwedge";
  case biguplus_code: return "biguplus";
  case bigcap_code: return "bigcap";
  case bigcup_code: return "bigcup";
  case int_code: return "int";
  case sum_code: return "sum";
  case prod_code: return "prod";
  case bigotimes_code: return "bigotimes";
  case bigoplus_code: return "bigoplus";
  case bigodot_code: return "bigodot";
  case oint_code: return "oint";
  case bigsqcup_code: return "bigsqcup";
  case smallint_code: return "smallint";
    // mathbin
  case triangleleft_code: return "triangleleft";
  case triangleright_code: return "triangleright";
  case bigtriangleup_code: return "bigtriangleup";
  case bigtriangledown_code: return "bigtriangledown";
  case wedge_code: return "wedge";
  case vee_code: return "vee";
  case cap_code: return "cap";
  case cup_code: return "cup";
  case Cup_code: return "Cup";
  case Cap_code: return "Cap";
  case multimap_code: return "multimap";
  case dagger_code: return "dagger";
  case ddagger_code: return "ddagger";
  case sqcap_code: return "sqcap";
  case sqcup_code: return "sqcup";
  case almalg_code: return "amalg";
  case diamand_code: return "diamond";
  case Diamond_code: return "Diamond";
  case bullet_code: return "bullet";
  case wr_code: return "wr";
  case div_code: return "div";
  case odot_code: return "odot";
  case oslash_code: return "oslash";
  case otimes_code: return "otimes";
  case ominus_code: return "ominus";
  case oplus_code: return "oplus";
  case uplus_code: return "uplus";
  case mp_code: return "mp";
  case pm_code: return "pm";
  case circ_code: return "circ";
  case bigcirc_code: return "bigcirc";
  case setminus_code: return "setminus";
  case cdot_code: return "cdot";
  case ast_code: return "ast";
  case times_code: return "times";
  case star_code: return "star";
    // Rel
  case propto_code: return "propto";
  case sqsubseteq_code: return "sqsubseteq";
  case sqsupseteq_code: return "sqsupseteq";
  case sqsubset_code: return "sqsubset";
  case sqsupset_code: return "sqsupset";
  case parallel_code: return "parallel";
  case bindnasrepma_code: return "bindnasrepma";
  case mid_code: return "mid";
  case dashv_code: return "dashv";
  case vdash_code: return "vdash";
  case Vdash_code: return "Vdash";
  case models_code: return "models";
  case nearrow_code: return "nearrow";
  case searrow_code: return "searrow";
  case nwarrow_code: return "nwarrow";
  case swarrow_code: return "swarrow";
  case Leftrightarrow_code: return "Leftrightarrow";
  case Leftarrow_code: return "Leftarrow";
  case Rightarrow_code: return "Rightarrow";
  case ne_code: return "ne";
  case le_code: return "le";
  case ge_code: return "ge";
  case succ_code: return "succ";
  case approx_code: return "approx";
  case succeq_code: return "succeq";
  case preceq_code: return "preceq";
  case prec_code: return "prec";
  case doteq_code: return "doteq";
  case supset_code: return "supset";
  case subset_code: return "subset";
  case supseteq_code: return "supseteq";
  case subseteq_code: return "subseteq";
  case ni_code: return "ni";
  case gg_code: return "gg";
  case ll_code: return "ll";
  case gtrless_code: return "gtrless";
  case geqslant_code: return "geqslant";
  case leqslant_code: return "leqslant";
  case notin_code: return "notin";
  case colon_code: return "colon";
  case starchar_code: return "*";
  case applyfct_code: return "ApplyFunction";
  case invisiblecomma_code: return "InvisibleComma";
  case leftrightarrow_code: return "leftrightarrow";
  case leftarrow_code: return "leftarrow";
  case rightarrow_code: return "rightarrow";
  case mapsto_code: return "mapsto";
  case sim_code: return "sim";
  case simeq_code: return "simeq";
  case perp_code: return "perp";
  case equiv_code: return "equiv";
  case asymp_code: return "asymp";
  case smile_code: return "smile";
  case frown_code: return "frown";
  case leftharpoonup_code: return "leftharpoonup";
  case leftharpoondown_code: return "leftharpoondown";
  case rightharpoonup_code: return "rightharpoonup";
  case rightharpoondown_code: return "rightharpoondown";
  case hookrightarrow_code: return "hookrightarrow";
  case hookleftarrow_code: return "hookleftarrow";
  case bowtie_code: return "bowtie";
  case Longrightarrow_code: return "Longrightarrow";
  case longrightarrow_code: return "longrightarrow";
  case longleftarrow_code: return "longleftarrow";
  case Longleftarrow_code: return "Longleftarrow";
  case longmapsto_code: return "longmapsto";
  case longleftrightarrow_code: return "longleftrightarrow";
  case Longleftrightarrow_code: return "Longleftrightarrow";
  case iff_code: return "iff";
  case cdots_code: return "cdots";
  case hdots_code: return "hdots";
  case vdots_code: return "vdots";
  case ddots_code: return "ddots";
  case imath_code: return "imath";
  case jmath_code: return "jmath";
  case Vertx_code: return "@Vert";
  case Vert_code: return "Vert";
  case vert_code: return "vert";
  case uparrow_code: return "uparrow";
  case downarrow_code: return "downarrow";
  case Uparrow_code: return "Uparrow";
  case Downarrow_code: return "Downarrow";
  case Updownarrow_code: return "Updownarrow";
  case updownarrow_code: return "updownarrow";
  case backslash_code: return "backslash";
  case Box_code: return "Box";
  case rangle_code: return "rangle";
  case langle_code: return "langle";
  case rmoustache_code: return "rmoustache";
  case lmoustache_code: return "lmoustache";
  case rgroup_code: return "rgroup";
  case lgroup_code: return "lgroup";
  case rbrace_code: return "rbrace";
  case lbrace_code: return "lbrace";
  case lceil_code: return "lceil";
  case rceil_code: return "rceil";
  case lfloor_code: return "lfloor";
  case rfloor_code: return "rfloor";
  case square_code: return "square";
  case cong_code: return "cong";
  case ncong_code: return "ncong";
  case mho_code: return "mho";
  case leadsto_code: return "leadsto";
  case lhd_code: return "lhd";
  case rhd_code: return "rhd";
  case unlhd_code: return "unlhd";
  case unrhd_code: return "unrhd";
  case clubsuit_code: return "clubsuit";
  case diamondsuit_code: return "diamondsuit";
  case heartsuit_code: return "heartsuit";
  case spadesuit_code: return "spadesuit";
  case aleph_code: return "aleph";
  case not_code: return "lnot";
  case prime_code: return "prime";
  case imaginaryi_code: return "ImaginaryI";
  case exponentiale_code: return "ExponentialE";
  case differentiald_code: return "DifferentialD";
  case beth_code: return "beth";
  case circledS_code: return "circledS";
  case complement_code: return "complement";
  case daleth_code: return "daleth";
  case eth_code: return "eth";
  case Finv_code: return "Finv";
  case Game_code: return "Game";
  case gimel_code: return "gimel";
  case mathparagraph_code: return "mathparagraph";
  case mathsection_code: return "mathsection";
  case backprime_code: return "backprime";
  case bigstar_code: return "bigstar";
  case blacktriangledown_code: return "blacktriangledown";
  case blacksquare_code: return "blacksquare";
  case blacklozenge_code: return "blacklozenge";
  case lozenge_code: return "lozenge";
  case blacktriangle_code: return "blacktriangle";
  case diagup_code: return "diagup";
  case diagdown_code: return "diagdown";
  case lightning_code: return "lightning";
  case measuredangle_code: return "measuredangle";
  case nexists_code: return "nexists";
  case sphericalangle_code: return "sphericalangle";
  case varnothing_code: return "varnothing";
  case baro_code: return "baro";
  case barwedge_code: return "barwedge";
  case curlyvee_code: return "curlyvee";
  case curlywedge_code: return "curlywedge";
  case divideontimes_code: return "divideontimes";
  case dotplus_code: return "dotplus";
  case gtrdot_code: return "gtrdot";
  case intercal_code: return "intercal";
  case interleave_code: return "interleave";
  case leftslice_code: return "leftslice";
  case rightslice_code: return "rightslice";
  case leftthreetimes_code: return "leftthreetimes";
  case rightthreetimes_code: return "rightthreetimes";
  case lessdot_code: return "lessdot";
  case ltimes_code: return "ltimes";
  case rtimes_code: return "rtimes";
  case merge_code: return "merge";
  case sslash_code: return "sslash";
  case talloblong_code: return "talloblong";
  case boxast_code: return "boxast";
  case boxplus_code: return "boxplus";
  case boxbox_code: return "boxbox";
  case boxbslash_code: return "boxbslash";
  case boxcircle_code: return "boxcircle";
  case boxdot_code: return "boxdot";
  case boxminus_code: return "boxminus";
  case boxslash_code: return "boxslash";
  case boxtimes_code: return "boxtimes";
  case centerdot_code: return "centerdot";
  case circledast_code: return "circledast";
  case circledcirc_code: return "circledcirc";
  case circleddash_code: return "circleddash";
  case obar_code: return "obar";
  case ogreaterthan_code: return "ogreaterthan";
  case olessthan_code: return "olessthan";
  case varocircle_code: return "varocircle";
  case llparenthesis_code: return "llparenthesis";
  case rrparenthesis_code: return "rrparenthesis";
    //  case bindnasrepma_code: return "bindnasrepma";
  case binampersand_code: return "binampersand";
  case llbracket_code: return "llbracket";
  case rrbracket_code: return "rrbracket";
  case backepsilon_code: return "backepsilon";
  case because_code: return "because";
  case between_code: return "between";
  case nmid_code: return "nmid";
  case nparallel_code: return "nparallel";
  case nVDash_code: return "nVDash";
  case nvDash_code: return "nvDash";
  case nVdash_code: return "nVdash";
  case nvdash_code: return "nvdash";
  case pitchfork_code: return "pitchfork";
  case therefore_code: return "therefore";
  case vDash_code: return "vDash";
  case VDash_code: return "VDash";
  case Vvdash_code: return "Vvdash";
  case nLeftarrow_code: return "nLeftarrow";
  case nLeftrightarrow_code: return "nLeftrightarrow";
  case nRightarrow_code: return "nRightarrow";
  case nleftarrow_code: return "nleftarrow";
  case nleftrightarrow_code: return "nleftrightarrow";
  case nrightarrow_code: return "nrightarrow";
  case digamma_code: return "digamma";
  case lnapprox_code: return "lnapprox";
  case Subset_code: return "Subset";
  case Supset_code: return "Supset";
  case blacktriangleleft_code: return "blacktriangleleft";
  case blacktriangleright_code: return "blacktriangleright";
  case triangledown_code: return "triangledown";
  case circlearrowleft_code: return "circlearrowleft";
  case circlearrowright_code: return "circlearrowright";
  case curvearrowleft_code: return "curvearrowleft";
  case curvearrowright_code: return "curvearrowright";
  case dasharrow_code: return "dasharrow";
  case dashleftarrow_code: return "dashleftarrow";
  case dashrightarrow_code: return "dashrightarrow";
  case fatsemi_code: return "fatsemi";
  case downdownarrows_code: return "downdownarrows";
  case downharpoonright_code: return "downharpoonright";
  case downharpoonleft_code: return "downharpoonleft";
  case leftarrowtail_code: return "leftarrowtail";
  case rightarrowtail_code: return "rightarrowtail";
  case leftarrowtriangle_code: return "leftarrowtriangle";
  case rightarrowtriangle_code: return "rightarrowtriangle";
  case leftrightarrowtriangle_code: return "leftrightarrowtriangle";
  case leftleftarrows_code: return "leftleftarrows";
  case upuparrows_code: return "upuparrows";
  case rightrightarrows_code: return "rightrightarrows";
  case leftrightarrows_code: return "leftrightarrows";
  case leftrightharpoons_code: return "leftrightharpoons";
  case leftrightsquigarrow_code: return "leftrightsquigarrow";
  case Lleftarrow_code: return "Lleftarrow";
  case Rrightarrow_code: return "Rrightarrow";
  case Longmapsfrom_code: return "Longmapsfrom";
  case Mapsfrom_code: return "Mapsfrom";
  case Longmapsto_code: return "Longmapsto";
  case Mapsto_code: return "Mapsto";
  case longmapsfrom_code: return "longmapsfrom";
  case mapsfrom_code: return "mapsfrom";
  case looparrowleft_code: return "looparrowleft";
  case looparrowright_code: return "looparrowright";
  case Lsh_code: return "Lsh";
  case Rsh_code: return "Rsh";
  case rightleftarrows_code: return "rightleftarrows";
  case rightleftharpoons_code: return "rightleftharpoons";
  case rightsquigarrow_code: return "rightsquigarrow";
  case twoheadleftarrow_code: return "twoheadleftarrow";
  case twoheadrightarrow_code: return "twoheadrightarrow";
  case upharpoonleft_code: return "upharpoonleft";
  case upharpoonright_code: return "upharpoonright";
  case varprime_code: return "varprime";
  case Prime_code: return "Prime";
  case tprime_code: return "tprime";
  case bprime_code: return "bprime";
  case qprime_code: return "qprime";
  case veebar_code: return "veebar";
  case backsimeq_code: return "backsimeq";
  case backsim_code: return "backsim";
  case Bumpeq_code: return "Bumpeq";
  case bumpeq_code: return "bumpeq";
  case circeq_code: return "circeq";
  case curlyeqprec_code: return "curlyeqprec";
  case curlyeqsucc_code: return "curlyeqsucc";
  case Doteq_code: return "Doteq";
  case doteqdot_code: return "doteqdot";
  case eqcirc_code: return "eqcirc";
  case eqsim_code: return "eqsim";
  case eqslantgtr_code: return "eqslantgtr";
  case eqslantless_code: return "eqslantless";
  case fallingdotseq_code: return "fallingdotseq";
  case risingdotseq_code: return "risingdotseq";
  case geqq_code: return "geqq";
  case leqq_code: return "leqq";
  case gggtr_code: return "gggtr";
  case ggg_code: return "ggg";
  case lll_code: return "lll";
  case llless_code: return "llless";
  case gtrapprox_code: return "gtrapprox";
  case lessapprox_code: return "lessapprox";
  case gtreqless_code: return "gtreqless";
  case gtreqqless_code: return "gtreqqless";
  case lesseqgtr_code: return "lesseqgtr";
  case lesseqqgtr_code: return "lesseqqgtr";
  case lessgtr_code: return "lessgtr";
  case lesssim_code: return "lesssim";
  case gtrsim_code: return "gtrsim";
  case precapprox_code: return "precapprox";
  case succapprox_code: return "succapprox";
  case succcurlyeq_code: return "succcurlyeq";
  case succsim_code: return "succsim";
  case preccurlyeq_code: return "preccurlyeq";
  case leftrightarroweq_code: return "leftrightarroweq";
  case gnapprox_code: return "gnapprox";
  case gneqq_code: return "gneqq";
  case lneqq_code: return "lneqq";
  case gneq_code: return "gneq";
  case lneq_code: return "lneq";
  case gnsim_code: return "gnsim";
  case lnsim_code: return "lnsim";
  case ngeq_code: return "ngeq";
  case ngtr_code: return "ngtr";
  case nleq_code: return "nleq";
  case nless_code: return "nless";
  case nprec_code: return "nprec";
  case nsim_code: return "nsim";
  case nsucc_code: return "nsucc";
  case precnapprox_code: return "precnapprox";
  case precneqq_code: return "precneqq";
  case precnsim_code: return "precnsim";
  case precsim_code: return "precsim";
  case succnapprox_code: return "succnapprox";
  case succneqq_code: return "succneqq";
  case succnsim_code: return "succnsim";
  case inplus_code: return "inplus";
  case niplus_code: return "niplus";
  case ntrianglerighteqslant_code: return "ntrianglerighteqslant";
  case ntrianglelefteqslant_code: return "ntrianglelefteqslant";
  case subseteqq_code: return "subseteqq";
  case supseteqq_code: return "supseteqq";
  case vartriangleleft_code: return "vartriangleleft";
  case vartriangleright_code: return "vartriangleright";
  case trianglerighteq_code: return "trianglerighteq";
  case trianglelefteq_code: return "trianglelefteq";
  case nsubseteq_code: return "nsubseteq";
  case nsupseteq_code: return "nsupseteq";
  case ntrianglelefteq_code: return "ntrianglelefteq";
  case ntrianglerighteq_code: return "ntrianglerighteq";
  case ntriangleleft_code: return "ntriangleleft";
  case ntriangleright_code: return "ntriangleright";
  case subsetneqq_code: return "subsetneqq";
  case supsetneqq_code: return "supsetneqq";
  case subsetneq_code: return "subsetneq";
  case supsetneq_code: return "supsetneq";
  case strut_code: return "strut";
  case mathstrut_code: return "mathstrut";
  case iint_code: return "iint";
  case iiint_code: return "iiint";
  case iiiint_code: return "iiiint";
  case xiint_code: return "xiint";
  case xiiint_code: return "xiiint";
  case xiiiint_code: return "xiiiint";
  case idotsint_code: return "idotsint";
  case mml_none_code: return "mmlnone";
  case mml_prescripts_code: return "mmlprescripts";
  default: return 0;
  }
}



// Aux functions for CmdChr::name() const
String CmdChr::token_fancy_name() const
{
  switch(chr) {
  case fancy_lhead_code: return "lhead";
  case fancy_chead_code: return "chead";
  case fancy_rhead_code: return "rhead";
  case fancy_lfoot_code: return "lfoot";
  case fancy_cfoot_code: return "cfoot";
  case fancy_rfoot_code: return "rfoot";
  case fancy_head_code: return "fancyhead";
  case fancy_foot_code: return "fancyfoot";
  case fancy_hf_code: return "fancyhf";
  default: return 0;
  }
}
String CmdChr::token_color_name()const
{
  switch(chr) {
  case definecolor_code: return "definecolor";
  case color_code: return "color";
  case normalcolor_code: return "normalcolor";
  case pagecolor_code: return "pagecolor";
  case colorbox_code: return "colorbox";
  case fcolorbox_code: return "fcolorbox";
  default: return 0;
  }
}

String CmdChr::token_fbox_name()const
{
  switch(chr) {
  case fbox_code: return "fbox";
  case framebox_code: return "framebox";
  case rotatebox_code: return "rotatebox";
  case scalebox_code: return "scalebox";
  case raisebox_code: return "raisebox";
  case dashbox_code: return "dashbox";
  default: return 0;
  }
}

String CmdChr::token_linebreak_name() const
{
  switch(chr) {
  case pagebreak_code: return "pagebreak";
  case nopagebreak_code: return "nopagebreak";
  case linebreak_code: return  "linebreak";
  case nolinebreak_code: return  "nolinebreak";
  default: return 0;
  }
}

String CmdChr::token_mark_name()  const
{
  switch(chr) {
  case topmark_code: return  "topmark";
  case firstmark_code: return  "firstmark";
  case botmark_code: return  "botmark";
  case splitfirstmark_code: return  "splitfirstmark";
  case splitbotmark_code: return  "splitbotmark";
  case topmarks_code: return  "topmarks";
  case firstmarks_code: return  "firstmarks";
  case botmarks_code: return  "botmarks";
  case splitfirstmarks_code: return  "splitfirstmarks";
  case splitbotmarks_code: return  "splitbotmarks";
  default: return 0;
  }
}

String CmdChr::token_fiorelse_name()  const
{
  switch(chr) {
  case fi_code: return "fi";
  case or_code: return "or";
  case else_code: return  "else";
  default: return 0;
  }
}


String CmdChr::token_iftest_name() const
{
  switch(chr) {
  case if_char_code: return  "if";
  case if_cat_code: return  "ifcat";
  case if_int_code: return  "ifnum";
  case if_dim_code: return  "ifdim";
  case if_odd_code: return  "ifodd";
  case if_vmode_code: return  "ifvmode";
  case if_hmode_code: return  "ifhmode";
  case if_mmode_code: return  "ifmmode";
  case if_inner_code: return  "ifinner";
  case if_void_code: return  "ifvoid";
  case if_hbox_code: return  "ifhbox";
  case if_vbox_code: return  "ifvbox";
  case ifx_code: return  "ifx";
  case if_eof_code: return  "ifeof";
  case if_true_code: return "iftrue";
  case if_false_code: return "iffalse";
  case if_case_code: return  "ifcase";
  case if_defined_code: return  "ifdefined";
  case if_csname_code: return  "ifcsname";
  case if_fontchar_code: return  "iffontchar";
  case if_leapyear_code: return "ifleapyear";
  default: return 0;
  }
}

String CmdChr::token_counter_name() const
{
  switch(chr) {
  case killcounter_code: return  "@stpelt";
  case stepcounter_code: return  "stepcounter";
  case addtocounter_code: return  "addtocounter";
  case setcounter_code: return  "setcounter";
  case value_code: return  "value";
  case addtoreset_code: return "@addtoreset";
  default: return 0;
  }
}

String CmdChr::token_accent_name() const
{
  switch(chr) {
  case '\'': return  "'";
  case '`': return  "`";
  case '"': return  "\"";
  case '^': return  "^";
  case '~': return  "~";
  case 'k': return  "k";
  case 'H': return  "H";
  case 'v': return  "v";
  case 'u': return  "u";
  case 'C': return  "C";
  case 'c': return  "c";
  case '.': return  ".";
  case '=': return  "=";
  case 'r': return  "r";
  case 'b': return  "b";
  case 'd': return  "d";
  case 'D': return  "D";
  case 'h': return  "H";
  default: return 0;
  }
}

String CmdChr::token_convert_name() const
{
  switch(chr) {
  case number_code: return  "number";
  case at_arabic_code: return  "@arabic";
  case romannumeral_code: return  "romannumeral";
  case Romannumeral_code: return "Romannumeral";
  case string_code: return  "string";
  case meaning_code: return  "meaning";
  case fontname_code: return  "fontname";
  case jobname_code: return  "jobname";
  case attributeval_code: return  "XMLgetatt";
  case ra_jobname_code: return  "ra@jobname";
  case tralicsversion_code: return  "tralicsversion";
  case etexrevision_code: return  "eTeXrevision";
  case sanitize_code: return "@onelevel@sanitize";
  case twodigits_code: return "two@digits";
  case rayear_code: return "ra@year";
  default: return 0;
  }
}

String CmdChr::token_for_name() const
{
  switch(chr) {
  case  xkv_for_n_code: return "tralics@for@n";
  case  xkv_for_en_code: return "tralics@for@en";
  case  xkv_for_o_code: return "tralics@for@o";
  case  xkv_for_i_code: return "tralics@for@i";
  case  xkv_for_eo_code: return "tralics@for@eo";
  case for_code : return "@for";
  case tfor_code : return "@tfor";
  case forloop_code : return "@forloop";
  case iforloop_code : return "@iforloop";
  case tforloop_code : return "@tforloop";
  case breaktfor_code: return "@break@tfor";
  case xkv_breaktfor_code: return "tralics@fro@break";
  default: return 0;
  }
}


String CmdChr::token_lastitem_name() const
{
  switch(chr) {
  case lastpenalty_code: return  "lastpenalty";
  case lastkern_code: return  "lastkern";
  case lastskip_code: return  "lastskip";
  case inputlineno_code: return  "inputlineno";
  case badness_code: return  "badness";
  case xmllastid_code: return "XMLlastid";
  case xmlAid_code: return "XML@A@id";
  case xmlBid_code: return "XML@B@id";
  case xmlAsize_code: return "XML@A@size";
  case XMLboxname_code: return "XMLbox@name";
  case xmlAname_code: return "XML@A@name";
  case xmlBname_code: return "XML@B@nale";
  case XMLboxid_code: return "XMLbox@id";
  case xmlcurrentid_code: return "XMLcurrentid";
  case xmlcurarray_code: return "XMLcurrentarrayid";
  case xmlcurcell_code: return "XMLcurrentcellid";
  case xmlcurrow_code: return "XMLcurrentrowid";
  case currentgrouplevel_code: return "currentgrouplevel";
  case currentgrouptype_code: return "currentgrouptype";
  case currentiflevel_code: return "currentiflevel";
  case currentiftype_code: return "currentiftype";
  case currentifbranch_code: return "currentifbranch";
  case etexversion_code: return "eTeXversion";
  case lastnodetype_code: return "lastnodetype";
  case fontcharwd_code: return "fontcharwd";
  case fontcharht_code: return "fontcharht";
  case fontchardp_code : return "fontchardp";
  case fontcharic_code: return "fontcharic";
  case parshapelength_code: return "parshapelength";
  case parshapeindent_code: return "parshapeindent";
  case parshapedimen_code: return "parshapedimen";
  case numexpr_code: return "numexpr";
  case dimexpr_code: return "dimexpr";
  case glueexpr_code: return "glueexpr";
  case muexpr_code: return "muexpr";
  case gluestretchorder_code: return "gluestretchorder";
  case glueshrinkorder_code: return "glueshrinkorder";
  case gluestretch_code: return "gluestretch";
  case glueshrink_code: return "glueshrink";
  case mutoglue_code: return "mutoglue";
  case gluetomu_code: return "gluetomu";
  default: return 0;
  }
}

String CmdChr::token_over_name() const
{
  switch(chr) {
  case above_code: return  "above";
  case over_code: return  "over";
  case atop_code: return  "atop";
  case abovewithdelims_code: return  "abovewithdelims";
  case overwithdelims_code: return  "overwithdelims";
  case atopwithdelims_code: return  "atopwithdelims";
  default: return 0;
  }
}


String CmdChr::token_limits_name() const
{
  switch(chr) {
  case 0: return  "displaylimits";
  case 2: return  "limits";
  case 1:return  "nolimits";
  default: return 0;
  }
}

String CmdChr::token_style_name() const
{
  switch(chr) {
  case displaystyle_code: return  "displaystyle";
  case textstyle_code: return  "textstyle";
  case scriptstyle_code: return  "scriptstyle"; 
  case scriptscriptstyle_code: return  "scriptscriptstyle";
  default: return 0;
  }
}

String CmdChr::token_big_name() const
{
  switch(chr) {
  case big_code: return  "big"; 
  case Big_code: return  "Big"; 
  case bigg_code: return  "bigg"; 
  case Bigg_code: return  "Bigg"; 
  case bigl_code: return  "bigl"; 
  case Bigl_code: return  "Bigl"; 
  case biggl_code: return  "biggl"; 
  case Biggl_code: return  "Biggl"; 
  case bigr_code: return  "bigr"; 
  case Bigr_code: return  "Bigr"; 
  case biggr_code: return  "biggr"; 
  case Biggr_code: return  "Biggr"; 
  case bigm_code: return  "bigm"; 
  case Bigm_code: return  "Bigm"; 
  case biggm_code: return  "biggm"; 
  case Biggm_code: return  "Biggm"; 
  default: return 0;
  }
}

String CmdChr::token_mathcomp_name() const
{
  switch(chr) {
  case ord_noad: return  "mathord";
  case op_noad: return  "mathop";
  case bin_noad: return  "mathbin";
  case rel_noad: return  "mathrel";
  case open_noad: return  "mathopen";
  case close_noad: return  "mathclose";
  case punct_noad: return  "mathpunct";
  case inner_noad: return  "mathinner";
  case between_noad: return  "mathbetween";
  default: return 0;
  }
}

String CmdChr::token_emath_name() const
{
  switch(chr) {
  case eqnarray_code: return  "endeqnarray";
  case Beqnarray_code: return  "Bendeqnarray";
  case multline_code: return  "endmultline";
  case eqnarray_star_code: return  "endeqnarray*";
  case Beqnarray_star_code: return  "Bendeqnarray*";
  case multline_star_code: return  "endmultline*";
  case align_code: return  "end@align";
  case align_star_code: return  "endalign*";
  case split_code: return  "endsplit";
  case aligned_code: return  "endaligned";
  case equation_code: return  "endequation";
  case equation_star_code: return  "endequation*";
  case math_code: return  "endmath";
  case displaymath_code: return  "enddisplaymath";
  default: return 0;
  }
}

String CmdChr::token_eignorec_name() const
{
  switch(chr) {
  case htmlonly_code: return  "endhtmlonly";
  case comment_code: return  "endcomment";
  case rawhtml_code: return  "endrawhtml";
  case LaTeXonly_code: return  "endLaTeXonly";
  default: return 0;
  }
}

String CmdChr::token_eignore_name() const
{
  switch(chr) {
  case body_code: return  "endbody";
  case abstract_code: return  "endabstract";
  case latexonly_code: return  "endlatexonly";
  case xmlonly_code: return  "endxmlonly";
  case subequations_code: return  "endsubequations";
  default: return 0;
  }
}


String CmdChr::token_efigure_name() const
{
  switch(chr) {
  case 0: return  "endfigure";
  case 1: return  "endfigure*";
  case 2: return  "endwrapfigure";
  default: return 0;
  }
}


String CmdChr::token_ecenter_name() const
{
  switch(chr) {
  case center_code: return  "endcenter";
  case quote_code: return  "endquote";
  case quotation_code: return  "endquotation";
  case flushleft_code: return  "endflushleft";
  case flushright_code: return  "endflushright";
  case verse_code: return  "endverse";
  default: return 0;
  }
}

// input and friends
String CmdChr::token_file_name() const
{
  switch(chr) {
  case ifexists_code: return  "IfFileExists";
  case inputifexists_code: return  "InputIfFileExists";
  case input_code: return  "input";
  case Input_code: return  "Input";
  case endinput_code: return  "endinput";
  case openin_code: return "openin";
  case closein_code: return "closein";
  case include_code: return  "include";
  case scantokens_code: return "scantokens";
  default: return 0;
  }
}

String CmdChr::token_dashline_name() const
{
  switch(chr) {
  case dashline_code: return  "dashline";
  case drawline_code: return  "drawline";
  case dottedline_code: return  "dottedline";
  case circle_code: return  "circle";
  default: return 0;
  }
}

String CmdChr::token_put_name() const
{
  switch(chr) {
  case put_code: return  "put";
  case line_code: return  "line";
  case vector_code: return  "vector";
  case oval_code: return  "oval";
  case multiput_code: return  "multiput";
  case scaleput_code: return  "scaleput";
  case frame_code: return  "frame";
  default: return 0;
  }
}

String CmdChr::token_caseshift_name() const
{
  switch(chr) {
  case 0: return  "lowercase";
  case 1: return  "uppercase";
  case 2: return  "MakeLowercase";
  case 3: return  "MakeUppercase";
  case 4: return  "MakeTextLowercase";
  case 5: return  "MakeTextUppercase";
  default: return 0;
  }
}

String CmdChr::token_centering_name() const
{
  switch(chr) {
  case center_code: return  "centering";
  case flushright_code: return  "raggedleft";
  case flushleft_code: return  "raggedright";
  default: return 0;
  }
}

String CmdChr::token_box_name() const
{
  switch(chr) {
  case mbox_code: return  "mbox";
  case makebox_code: return  "makebox";
  case text_code: return  "text";
  default: return 0;
  }
}

String CmdChr::token_section_name() const
{
  switch(chr) {
  case part_code: return  "part";
  case chapter_code: return  "chapter";
  case section_code: return  "section";
  case subsection_code: return  "subsection";
  case subsubsection_code: return  "subsubsection";
  case paragraph_code: return  "paragraph";
  case subparagraph_code: return  "subparagraph";
  case endsec_code: return  "endsec";
  case toplevel_sec_code: return  "toplevelsection";
  default: return 0;
  }
}

String CmdChr::token_cite_name() const
{
  switch(chr) {
  case cite_code: return  "cite";
  case footcite_code: return  "footcite";
  case nocite_code: return  "nocite";
  case refercite_code: return  "refercite";
  case yearcite_code: return  "yearcite";
  case natcite_code: return  "natcite";
  case natcite_e_code: return  "endnatcite";
  default: return 0;
  }
}

String CmdChr::token_latexctr_name() const
{
  switch(chr) {
  case arabic_code: return "arabic";
  case roman_code: return  "roman";
  case Roman_code: return"Roman";
  case alph_code: return  "alph";
  case Alph_code: return  "Alph";
  case fnsymbol_code: return  "fnsymbol";
  case at_alph_code: return "@alph";
  case at_Alph_code: return "@Alph";
  case at_fnsymbol_code: return "@fnsymbol";
  default: return 0;
  }
}

String CmdChr::token_mathfont2_name() const
{
  switch(chr) {
  case math_f_normal: return "mml@font@normal";
  case math_f_upright: return "mml@font@upright";
  case math_f_bold:  return "mml@font@bold";
  case math_f_italic:  return "mml@font@italic";
  case math_f_bold_italic: return "mml@font@bolditalic";
  case math_f_script: return "mml@font@script";
  case math_f_bold_script: return "mml@font@boldscript";
  case math_f_fraktur: return "mml@font@fraktur";
  case math_f_doublestruck: return "mml@font@doublestruck";
  case math_f_bold_fraktur: return "mml@font@boldfraktur";
  case math_f_sansserif: return "mml@font@sansserif";
  case math_f_bold_sansserif: return "mml@font@boldsansseif";
  case math_f_sansserif_italic: return "mml@font@sanserifitalic";
  case math_f_sansserif_bold_italic: return "mml@font@sanserifbolditalic";
  case math_f_monospace: return "mml@font@monospace";
  default: return 0;
  }
}

String CmdChr::token_mathfont1_name() const
{
  switch(chr) {
  case cal_code: return  "cal";
  case mathtt_code: return  "mathtt";
  case mathcal_code: return  "mathcal";
  case mathbf_code: return  "mathbf";
  case mathrm_code: return  "mathrm";
  case mathfrak_code: return  "mathfrak";
  case mathit_code: return  "mathit";
  case mathbb_code: return  "mathbb";
  case mathsf_code: return  "mathsf";
  case mathnormal_code: return  "mathnormal";
  default: return 0;
  }
}

String CmdChr::token_setinteraction_name() const
{
  switch(chr) {
  case batchmode_code: return  "batchmode";
  case nonstopmode_code: return  "nonstopmode";
  case scrollmode_code: return  "scrollmode";
  case errorstopmode_code: return  "errorstopmode";
  default: return 0;
  }
}

String CmdChr::token_shape_name() const
{
  switch(chr) {
  case parshape_code: return "parshape";
  case interlinepenalties_code: return "interlinepenalties";
  case clubpenalties_code: return "clubpenalties";
  case widowpenalties_code: return "widowpenalties";
  case displaywidowpenalties_code: return "displaywidowpenalties";
  default: return 0;
  }
}
String CmdChr::token_def_name() const
{
  switch(chr) {
  case xdef_code: return  "xdef"; 
  case def_code: return  "def"; 
  case gdef_code: return  "gdef"; 
  case edef_code: return  "edef"; 
  case newcommand_code: return  "newcommand"; 
  case newthm_code: return  "newtheorem"; 
  case newenv_code: return  "newenvironment"; 
  case renewenv_code: return  "renewenvironment"; 
  case renew_code: return  "renewcommand"; 
  case provide_code: return  "providecommand"; 
  case checkcommand_code: return  "CheckCommand"; 
  case declare_math_operator_code: return  "DeclareMathOperator"; 
  default: return 0;
  }
}

String CmdChr::token_shorthand_name() const
{
  switch(chr) {
  case char_def_code: return  "chardef";
  case math_char_def_code: return  "mathchardef";
  case count_def_code: return  "countdef";
  case dimen_def_code: return  "dimendef";
  case skip_def_code: return  "skipdef";
  case mu_skip_def_code: return  "muskipdef";
  case toks_def_code: return  "toksdef";
  default: return 0;
  }
}

String CmdChr::token_prefix_name() const
{
  switch(chr) {
  case global_code: return  "global";
  case long_code: return  "long";
  case outer_code: return  "outer";
  case protected_code: return  "protected";
  default: return 0;
  }
}

String CmdChr::token_register_name() const
{
  switch(chr) {
  case it_int: return  "count";
  case it_dimen: return  "dimen";
  case it_glue: return  "skip";
  case it_mu: return  "muskip";
  default: return 0;
  }
}

String CmdChr::token_deffamily_name() const
{
  switch(chr) {
  case textfont_code: return  "textfont";
  case scriptfont_code: return  "scriptfont";
  case scriptscriptfont_code: return  "scriptscriptfont";
  default: return 0;
  }
}

String CmdChr::token_defcode_name() const
{
  switch(chr) {
  case 0: return  "catcode"; 
  case math_code_offset: return  "mathcode"; 
  case lc_code_offset: return  "lccode"; 
  case uc_code_offset: return  "uccode"; 
  case sf_code_offset: return  "sfcode"; 
  case del_code_offset: return  "delcode"; 
  default: return 0;
  }
}

String CmdChr::token_setboxdimen_name() const
{
  switch(chr) {
  case wd_code: return  "wd";
  case ht_code: return  "ht";
  case dp_code: return  "dp";
  default: return 0;
  }
}

String CmdChr::token_setpagedimen_name() const
{
  switch(chr) {
  case pagegoal_code: return  "pagegoal";
  case pagetotal_code: return  "pagetotal";
  case pagestretch_code: return  "pagestretch";
  case pagefilstretch_code: return  "pagefilstretch";
  case pagefillstretch_code: return  "pagefillstretch";
  case pagefilllstretch_code: return  "pagefilllstretch";
  case pageshrink_code: return  "pageshrink";
  case pagedepth_code: return  "pagedepth";
  default: return 0;
  }
}

String CmdChr::token_assignmuglue_name() const
{
  return token_assignglue_name();
}

String CmdChr::token_assignglue_name() const
{
  switch(chr) {
  case lineskip_code: return  "lineskip";
  case baselineskip_code: return  "baselineskip";
  case parskip_code: return  "parskip";
  case abovedisplayskip_code: return  "abovedisplayskip";
  case belowdisplayskip_code: return  "belowdisplayskip";
  case abovedisplayshortskip_code: return  "abovedisplayshortskip";
  case belowdisplayshortskip_code: return  "belowdisplayshortskip";
  case leftskip_code: return  "leftskip";
  case rightskip_code: return  "rightskip";
  case topskip_code: return  "topskip";
  case splittopskip_code: return  "splittopskip";
  case tabskip_code: return  "tabskip";
  case spaceskip_code: return  "spaceskip";
  case xspaceskip_code: return  "xspaceskip";
  case parfillskip_code: return  "parfillskip";

  case thinmuskip_code: return  "thinmuskip";
  case medmuskip_code: return  "medmuskip";
  case thickmuskip_code: return  "thickmuskip";

  default:
    if(chr<256)
      return make_name("skip",chr);
    else
      return make_name("muskip",chr-muskip_reg_offset);
  }
}


String CmdChr::token_assigndimen_name() const
{
  switch(chr) {
  case parindent_code: return  "parindent";
  case mathsurround_code: return  "mathsurround";
  case lineskiplimit_code: return  "lineskiplimit";
  case hsize_code: return  "hsize";
  case vsize_code: return  "vsize";
  case maxdepth_code: return  "maxdepth";
  case splitmaxdepth_code: return  "splitmaxdepth";
  case boxmaxdepth_code: return  "boxmaxdepth";
  case hfuzz_code: return  "hfuzz";
  case vfuzz_code: return  "vfuzz";
  case delimitershortfall_code: return  "delimitershortfall";
  case nulldelimiterspace_code: return  "nulldelimiterspace";
  case scriptspace_code: return  "scriptspace";
  case predisplaysize_code: return  "predisplaysize";
  case displaywidth_code: return  "displaywidth";
  case displayindent_code: return  "displayindent";
  case overfullrule_code: return  "overfullrule";
  case hangindent_code: return  "hangindent";
  case hoffset_code: return  "hoffset";
  case voffset_code: return  "voffset";
  case emergencystretch_code: return  "emergencystretch";
  case unitlength_code: return  "unitlength";
  case textheight_code: return  "textheight";
  case textwidth_code: return  "textwidth";
  case linewidth_code: return  "linewidth";
  case columnwidth_code: return  "columnwidth";
  default: 
    return make_name("dimen",chr);
  }
}

String CmdChr::token_assignint_name() const
{
  switch(chr) {
  case pretolerance_code: return  "pretolerance";
  case tolerance_code: return  "tolerance";
  case linepenalty_code: return  "linepenalty";
  case hyphenpenalty_code: return  "hyphenpenalty";
  case exhyphenpenalty_code: return  "exhyphenpenalty";
  case clubpenalty_code: return  "clubpenalty";
  case widowpenalty_code: return  "widowpenalty";
  case displaywidowpenalty_code: return  "displaywidowpenalty";
  case brokenpenalty_code: return  "brokenpenalty";
  case binoppenalty_code: return  "binoppenalty";
  case relpenalty_code: return  "relpenalty";
  case predisplaypenalty_code: return  "predisplaypenalty";
  case postdisplaypenalty_code: return  "postdisplaypenalty";
  case interlinepenalty_code: return  "interlinepenalty";
  case outputpenalty_code: return  "outputpenalty";
  case doublehyphendemerits_code: return  "doublehyphendemerits";
  case finalhyphendemerits_code: return  "finalhyphendemerits";
  case adjdemerits_code: return  "adjdemerits";
  case mag_code: return  "mag";
  case delimiterfactor_code: return  "delimiterfactor";
  case looseness_code: return  "looseness";
  case time_code: return  "time";
  case day_code: return  "day";
  case month_code: return  "month";
  case year_code: return  "year";
  case showboxbreadth_code: return  "showboxbreadth";
  case nomath_code: return  "@nomathml";
  case notrivialmath_code: return  "notrivialmath";
  case showboxdepth_code: return  "showboxdepth";
  case hbadness_code: return  "hbadness";
  case vbadness_code: return  "vbadness";
  case pausing_code: return  "pausing";
  case tracingonline_code: return  "tracingonline";
  case tracingmacros_code: return  "tracingmacros";
  case tracingstats_code: return  "tracingstats";
  case tracingparagraphs_code: return  "tracingparagraphs";
  case tracingpages_code: return  "tracingpages";
  case tracingoutput_code: return  "tracingoutput";
  case tracinglostchars_code: return  "tracinglostchars";
  case tracingcommands_code: return  "tracingcommands";
  case tracingrestores_code: return  "tracingrestores";
  case tracingassigns_code: return "tracingassigns";
  case tracinggroups_code: return "tracinggroups";
  case tracingifs_code: return "tracingifs";
  case tracingmath_code: return "tracingmath";
  case tracingscantokens_code: return "tracingscantokens";
  case tracingnesting_code: return "tracingnesting";
  case predisplaydirection_code: return "predisplaydirection";
  case TeXXetstate_code: return "TeXXeTstate";
  case uchyph_code: return  "uchyph";
  case maxdeadcycles_code: return  "maxdeadcycles";
  case hangafter_code: return  "hangafter";
  case floatingpenalty_code: return  "floatingpenalty";
  case globaldefs_code: return  "globaldefs";
  case fam_code: return  "fam";
  case escapechar_code: return  "escapechar";
  case defaulthyphenchar_code: return  "defaulthyphenchar";
  case defaultskewchar_code: return  "defaultskewchar";
  case endlinechar_code: return  "endlinechar";
  case newlinechar_code: return  "newlinechar";
  case language_code: return  "language";
  case lefthyphenmin_code: return  "lefthyphenmin";
  case righthyphenmin_code: return  "righthyphenmin";
  case holdinginserts_code: return  "holdinginserts";
  case errorcontextlines_code: return  "errorcontextlines";
  case atmathversion_code: return "@mathversion";
  case incentering_code: return "incentering ";
  case fpseed_code: return "fpseed";
  case list_depth_code: return "@listdepth";
  case list_ctr_code: return "@listctr";
  case use_subfigure_code: return "tralics@use@subfigure";
  case math_font_pos: return "@curmathfont";
  case lastlinefit_code: return "lastlinefit";
  case savingdiscards_code: return "savingdiscards";
  case savinghyphcodes_code: return "savinghyphcodes";
  default: 
    if(chr<math_code_offset) return make_name("catcode",chr);
    if(chr<lc_code_offset) return make_name("mathcode",chr-math_code_offset);
    if(chr<uc_code_offset) return make_name("uccode",chr-lc_code_offset);
    if(chr<sf_code_offset) return make_name("uccode",chr-uc_code_offset);
    if(chr<del_code_offset) return make_name("sfcode",chr-sf_code_offset);
    if(chr<count_reg_offset) return make_name("delcode",chr-del_code_offset);
    if(chr<mu_reg_offset) return make_name("count",chr-count_reg_offset);
    if(chr<other_int_offset) return make_name("mu",chr-mu_reg_offset);
    return make_name("countref",chr);
  }
}

String CmdChr::token_assigntoks_name() const
{
  switch(chr) {
  case output_code: return  "output";
  case everypar_code: return  "everypar";
  case everymath_code: return  "everymath";
  case everybibitem_code: return  "everybibitem";
  case everydisplay_code: return  "everydisplay";
  case everyhbox_code: return  "everyhbox";
  case everyvbox_code: return  "everyvbox";
  case everyxbox_code: return  "everyxbox";
  case everyjob_code: return  "everyjob";
  case everycr_code: return  "everycr";
  case everyeof_code: return  "everyeof";
  case errhelp_code: return  "errhelp";
  default:
    return make_name("toks",chr);
  }
}


String CmdChr::token_leader_name() const
{
  switch(chr) {
  case shipout_code: return  "shipout";
  case leaders_code: return  "leaders";
  case cleaders_code: return  "cleaders";
  case xleaders_code: return  "xleaders";
  default: return 0;
  }
}
String CmdChr::token_index_name() const
{
  switch(chr) {
  case index_code: return "index";
  case glossary_code: return "glossary";
  case printindex_code: return "printindex";
  case printglossary_code: return "printglossary";
  case makeindex_code: return "makeindex";
  case makeglossary_code: return "makeglossary";
  case newindex_code: return "@newindex";
  default: return 0;
  }
}
String CmdChr::token_newcount_name() const
{
  switch(chr) {
  case newcount_code: return "newcount";
  case newtoks_code: return "newtoks";
  case newbox_code: return "newbox";
  case newdimen_code: return "newdimen";
  case newlength_code: return "newlength";
  case newmuskip_code: return "newmuskip";
  case newread_code: return "newread";
  case newwrite_code: return "newwrite";
  case newlanguage_code: return "newlanguage";
  default: return 0;
  }
}
  
String CmdChr::token_line_name() const
{
  switch(chr) {
  case leftline_code: return "leftline";
  case rightline_code: return  "rightline";
  case centerline_code: return  "centerline";
  case llap_code: return "llap";
  case rlap_code: return "rlap";
  default: return 0;
  }
}


String CmdChr::token_makebox_name() const
{
  switch(chr) {
  case box_code: return "box";
  case copy_code: return  "copy";
  case usebox_code: return  "usebox";
  case lastbox_code: return  "lastbox";
  case useboxA_code: return  "XML@copyA";
  case useboxB_code: return  "XML@copyB";
  case vsplit_code: return  "vsplit";
  case vtop_code: return  "vtop";
  case vbox_code: return  "vbox";
  case hbox_code: return  "hbox";
  case xbox_code: return  "xbox";
  case parbox_code: return "parbox";
  case marginpar_code: return "marginpar";
  default: return 0;
  }
}

String CmdChr::token_move_name() const
{
  switch(chr) {
  case moveleft_code: return "moveleft";
  case moveright_code: return "moveright";
  case raise_code: return "raise";
  case lower_code: return "lower";
  default: return 0;
  }
}

String CmdChr::token_xray_name() const
{
  switch(chr) {
  case show_code: return "show";
  case showbox_code: return "showbox";
  case show_xmlA_code: return "show@xmlA";
  case show_xmlB_code: return "show@xmlB";
  case showthe_code: return "showthe";
  case showlists_code: return "showlists";
  case showgroups_code: return "showgroups";
  case showifs_code: return "showifs";
  case showtokens_code: return "showtokens";
  default: return 0;
  }
}


String CmdChr::token_extension_name() const
{
  switch(chr) {
  case openout_code: return "openout";
  case write_code: return "write";
  case closeout_code: return "closeout";
  default: return 0;
  }
}

String CmdChr::token_unbox_name() const
{
  switch(chr) {
  case unhbox_code: return "unhbox";
  case unhcopy_code: return "unhcopy";
  case unvbox_code: return "unvbox";
  case unvcopy_code: return "unvcopy";
  default: return 0;
  }
}

String CmdChr::token_ltfont_name() const
{
  switch(chr) {
  case fontencoding_code: return "fontencoding";
  case fontfamily_code: return "fontfamily";
  case fontseries_code: return "fontseries";
  case fontshape_code: return "fontshape";
  default: return 0;
  }
}

String CmdChr::token_ign2_name() const
{
  switch(chr) {
  case HTMLset_code: return "HTMLset";
  case fontsize_code: return "fontsize";
  default: return 0;
  }
}

String CmdChr::token_ign1_name() const
{
  switch(chr) {
  case RAlabel_code: return "RAlabel";
  case hyphenation_code: return "hyphenation";
  case patterns_code: return "patterns";
  case special_code: return "special";
  case includeonly_code: return "includeonly";
  case showhyphens_code: return "showhyphens";

  default: return 0;
  }
}

String CmdChr::token_ignore_name() const
{
  switch(chr) {
  case protect_code: return  "protect";
  case sloppy_code: return  "sloppy";
  case fussy_code: return  "fussy";
  case subitem_code: return  "subitem";
  case htmlimage_code: return  "htmlimage";
  case immediate_code: return  "immediate";
  case unpenalty_code: return  "unpenalty";
  case unkern_code: return  "unkern";
  case unskip_code: return  "unskip";
  case addnl_code: return  "@addnl";
  case atsign_code: return  "@";
  case italiccorr_code: return  "/";
  case clearpage_code: return  "clearpage";
  case samepage_code: return  "samepage";
  case newpage_code: return  "newpage";
  case hrulefill_code: return  "hrulefill";
  case dotfill_code: return  "dotfill";
  case cleardoublepage_code: return "cleardoublepage";
  case break_code: return "break";
  case nobreak_code: return "nobreak";
  case fillbreak_code: return "filbreak";
  case goodbreak_code: return "goodbreak";
  case eject_code: return "eject";
  case smallbreak_code: return "smallbreak";
  case medbreak_code: return "medbreak";
  case bigbreak_code: return "bigbreak";
  case endL_code: return "endL";
  case endR_code: return "endR";
  case beginL_code: return "beginL";
  case beginR_code : return "beginR";
  case pagediscards_code : return "pagediscards";
  case nointerlineskip_code: return "nointerlineskip";
  case offinterlineskip_code: return "offinterlineskip";
  case frenchspacing_code: return "frenchspacing";
  case nonfrenchspacing_code: return "nonfrenchspacing";
  case loggingoutput_code: return "loggingoutput";
  case showoutput_code: return "showgoutput";
  case nofiles_code: return "nofiles";
  case raggedbottom_code: return "raggedbottom";
  case flushbottom_code: return "flushbottom";
  case onecolumn_code: return "onecolumn";
  case twocolumn_code: return "twocolumn";
  case reversemarginpar_code: return "reversemarginpar";
  case normalmarginpar_code: return "normalmarginpar";
  case normalbaselines_code: return "normalbaselines";
  case removelastskip_code: return "removelastskip";
  default: return 0;
  }
}

String CmdChr::token_fontsize_name() const
{
  switch(chr) {
  case tiny_code: return  "tiny";
  case scriptsize_code: return  "scriptsize";
  case footnotesize_code: return  "footnotesize";
  case small_code: return "small";
  case normalsize_code: return  "normalsize";
  case large_code: return  "large";
  case Large_code: return  "Large";
  case LARGE_code: return  "LARGE";
  case huge_code: return  "huge";
  case Huge_code: return  "Huge";
  default: return 0;
  }
}

String CmdChr::token_argfont_name() const
{
  switch(chr) {
  case rm_family_code:return  "textrm";
  case sf_family_code:return  "textsf";
  case tt_family_code:return "texttt";
  case md_series_code:return  "textmd";
  case bf_series_code:return  "textbf";
  case up_shape_code:return  "textup";
  case it_shape_code:return  "textit";
  case sl_shape_code:return  "textsl";
  case sc_shape_code:return  "textsc";
  case em_code:return  "emph";
  case normalfont_code:return  "textnormal";
  default: return 0;
  }
}

String CmdChr::token_soul_name() const
{
  switch(chr) {
  case soul_sup_code: return "textsuperscript";
  case soul_sub_code: return "textsubscript";
  case soul_oldstyle_code :   return "oldstylenums";
  case soul_caps_code: return "caps";
  case soul_hl_code: return "hl";
  case soul_so_code: return "so";
  case soul_st_code: return "st";
  case soul_ul_code: return "ul";
  default: return 0;
  }
}

String CmdChr::token_oldfont_name() const
{
  switch(chr) {
  case rm_family_code: return "rm";
  case sf_family_code: return "sf";
  case tt_family_code: return "tt";
  case bf_series_code: return "bf";
  case it_shape_code: return "it";
  case sc_shape_code: return "sc";
  case sl_shape_code: return "sl";
  default: return 0;
  }
}

String CmdChr::token_noargfont_name() const
{
  switch(chr) {
  case rm_family_code: return  "rmfamily";
  case sf_family_code: return  "sffamily";
  case tt_family_code: return  "ttfamily";
  case md_series_code: return  "mdseries";
  case bf_series_code: return  "bfseries";
  case up_shape_code: return  "upshape";
  case it_shape_code: return  "itshape";
  case sl_shape_code: return  "slshape";
  case sc_shape_code: return  "scshape";
  case em_code: return  "em";
  case normalfont_code: return  "normalfont";
  default: return 0;
  }
}

String CmdChr::token_trees_name() const
{
  switch(chr) {
  case node_code: return "node";
  case nodepoint_code: return "nodepoint";
  case nodeconnect_code: return "nodeconnect";
  case anodeconnect_code: return "anodeconnect";
  case nodecurve_code: return "nodecurve";
  case anodecurve_code: return "anodecurve";
  case nodebox_code: return "nodebox";
  case nodeoval_code: return "nodeoval";
  case nodetriangle_code: return "nodetriangle";
  case nodecircle_code: return "nodecircle";
  case barnodeconnect_code:  return "barnodeconnect";
  case abarnodeconnect_code:  return "abarnodeconnect";
  default: return 0;
  }
}

String CmdChr::token_unimp_font_name() const
{
  switch(chr) {
  case DeclareTextCommand_code: return "DeclareTextCommand";
  case ProvideTextCommand_code: return "ProvideTextCommand";
  case DeclareTextCompositeCommand_code: return "DeclareTextCompositeCommand";
  case DeclareTextComposite_code: return "DeclareTextComposite";
  case DeclareTextAccent_code: return "DeclareTextAccent";
  case UseTextAccent_code: return "UseTextAccent";
  case DeclareTextCommandDefault_code: return "DeclareTextCommandDefault";
  case ProvideTextCommandDefault_code: return "ProvideTextCommandDefault";
  case TextSymbolUnavailable_code: return "TextSymbolUnavailable";
  case DeclareTextSymbol_code: return "DeclareTextSymbol";
  case DeclareTextSymbolDefault_code: return "DeclareTextSymbolDefault";
  case UseTextSymbol_code: return "UseTextSymbol";
  case UndeclareTextCommand_code: return "UndeclareTextCommand";
  case DeclareFontShape_code: return "DeclareFontShape";
  case DeclareFixedFont_code: return "DeclareFixedFont";
  case DeclareFontFamily_code: return "DeclareFontFamily ";
  case DeclareFontEncoding_code: return "DeclareFontEncoding";
  case DeclareFontSubstitution_code: return "DeclareFontSubstitution";
  case DeclareFontEncodingDefaults_code: return "DeclareFontEncodingDefaults";
  case DeclarePreloadSizes_code: return "DeclarePreloadSizes";
  case DeclareMathSizes_code: return "DeclareMathSizes";
  case DeclareErrorFont_code: return "DeclareErrorFont";
  case DeclareSizeFunction_code: return "DeclareSizeFunction ";
  case DeclareMathVersion_code: return "DeclareMathVersion";
  case DeclareSymbolFont_code: return "DeclareSymbolFont";
  case SetSymbolFont_code: return "SetSymbolFont";
  case DeclareMathAlphabet_code: return "DeclareMathAlphabet";
  case SetMathAlphabet_code: return "SetMathAlphabet";
  case DeclareMathAccent_code: return "DeclareMathAccent";
  case DeclareMathSymbol_code: return "DeclareMathSymbol";
  case DeclareMathDelimiter_code: return "DeclareMathDelimiter";
  case DeclareMathRadical_code: return "DeclareMathRadical";
  case DeclareSymbolFontAlphabet_code: return "DeclareSymbolFontAlphabet";
  case DeclareTextFontCommand_code: return "DeclareTextFontCommand";
  case DeclareOldFontCommand_code: return "DeclareOldFontCommand";
  case setsize_code: return "@setsize";
  case setfontsize_code: return "@setfontsize";
  default: return 0;
  }
}

String CmdChr::token_unimp_name() const
{
  switch(chr) {
  case accent_code: return "accent";
  case delimiter_code: return "delimiter";
  case halign_code: return "halign";
  case ignorespaces_code: return "ignorespaces";
  case insert_code: return "insert";
  case mark_code: return "mark";
  case marks_code: return "marks";
  case mathaccent_code: return "mathaccent";
  case mathchar_code: return "mathchar";
  case noalign_code: return "noalign";
  case noboundary_code: return "noboundary";
  case omit_code: return "omit";
  case penalty_code: return "penalty";
  case radical_code: return "radical";
  case vadjust_code: return "vadjust";
  case valign_code: return "valign";
  case vcenter_code: return "vcenter";
  case span_code: return "span";
  case dump_code: return "dump";
  case atatend_code: return "@@end";
  case message_code: return "message";
  case errmessage_code: return "errmessage";
  case widthof_code: return "widthof";
  case heightof_code: return "heightof";
  case depthof_code: return "depthof";
  case discretionary_code: return "discretionary";
  case allowbreak_code: return "allowbreak";
  case abort_code: return "tralics@abort";
  case sleep_code: return "tralics@sleep";
  case prompt_code: return "tralics@prompt";
  default: return 0;
  }
}

String CmdChr::token_cst_name() const
{
  switch(chr) {
  case textasciicircum_code: return "textasciicircum";
  case guillemet_og_code: return "og";
  case guillemet_fg_code: return "fg";
  case i_code: return  "i";
  case j_code: return  "j";
  case dollar_code: return  "$";
  case comma_code: return  ",";
  case underscore_code: return  "_";
  case amp_code: return  "&";
  case lbrace_chr: return "{";
  case rbrace_chr: return "}";
  case percent_code: return  "%";
  case space_code: return  " ";
    //  case space_code: return  "\n";
    //  case space_code: return  "\r";
  case exclam_code: return  "!";
  case quad_code: return  "quad";
  case qquad_code: return  "qquad";
  case dots_code: return  "dots";
  case ldots_code: return  "ldots";
  case msharp_code: return  "sharp";
  case natural_code: return  "natural"; 
  case flat_code: return  "flat";
  case textasciigrave_code: return "textasciigrave";
  case textasciiacutex_code: return "textasciiacutex";
  case tdagger_code: return "dag";
  case tddagger_code: return "ddag";

  case sharp_code: return  "#";
  case dash_code: return  "-";
  case SS_code: return  "SS";
  case numero_code: return  "numero"; 
  case Numero_code: return  "Numero";
  case ier_code: return  "ier";
  case iere_code: return  "iere";
  case iers_code: return  "iers";
  case ieres_code: return  "ieres";
  case ieme_code: return  "ieme";
  case iemes_code: return  "iemes";
  case textvisiblespace_code: return "textvisiblespace";
  case textnospace_code: return  "textnospace";
  case LaTeX_code: return  "LaTeX";
  case TeX_code: return  "TeX";
  case ss_code: return  "ss";
  case slash_code: return  "slash";
  case textbackslash_code: return  "textbackslash";
  case null_code: return  "null";
  case thicklines_code: return  "thicklines";
  case thinlines_code: return  "thinlines";
  default:
    return make_name("other_constant_number",chr);
  }
}

String CmdChr::special_name() const
{
  switch(cmd) {
  case 1: return "begin-group character";
  case 2: return "end-group character";
  case 3: return "math shift character";
  case 4: return "alignment tab character";
  case 5: return "impossible (eol)";
  case 6: return "macro parameter character";
  case 7: return "superscript character";
  case 8: return "subscript character";
  case 9: return "impossible (ignored)";
  case 10: return "blank space";
  case 11: return "the letter";
  case 12: return "the character";
  case 13: return "active character";
  case 14: return "impossible (comment)";
  case 15: return "impossible (ignored)";
  default: return 0;
  }
}


// This returns the name of a CmdChr pair.
// The result is a UTF8 string
String CmdChr::name() const
{
  switch(cmd) {
  case mathbin_cmd:
  case mathrel_cmd:
  case mathinner_cmd:
  case mathbetween_cmd:
  case mathopen_cmd:
  case mathclose_cmd:
  case mathspace_cmd:
  case mathord_cmd:
  case mathordb_cmd:
  case mathop_cmd:
  case mathopn_cmd:
    return token_math_name();
  case section_cmd: return token_section_name();
  case label_cmd: return  chr==0 ?"label" : "anchor";
  case ref_cmd: return chr==0 ? "ref" : "pageref";
  case eqref_cmd: return  "XMLref";
  case par_cmd: return "par";
  case start_par_cmd: return  chr==0 ?  "noindent" :  "indent";
  case endcsname_cmd: return "endcsname";
  case multicolumn_cmd: return  "multicolumn";
  case atdocument_cmd: 
    return chr==0 ?  "AtBeginDocument" :   "AtEndDocument";
  case fvset_cmd: return "fvset";
  case calc_cmd: return "calc ";
  case biblio_cmd: return "bibliography";
  case aparaitre_cmd: return "aparaitre";
  case citation_cmd: return  "citation";
  case end_citation_cmd: return "endcitation";
  case thebibliography_cmd: return  "thebibliography";
  case end_thebibliography_cmd: return "endthebibliography";
  case bpers_cmd: return "bpers";
  case cititem_cmd: return "cititem";
  case bibitem_cmd: return "bibitem";
  case cite_one_cmd: return chr==0 ?  "cite@one" : "cite@simple";
  case cite_cmd: return token_cite_name();
  case solvecite_cmd: return "XMLsolvecite";
  case bib_cmd: return chr==0 ? "bauthors" : "beditors";
  case backslash_cmd: return chr==0 ? "\\" : "newline";
  case hline_cmd:
    if(chr==one_code) return "cline";
    if(chr==two_code) return "hlinee";
    return "hline";
  case skip_cmd:
    if(chr==smallskip_code) return "smallskip";
    if(chr==medskip_code) return"medskip";
    if(chr==bigskip_code) return "bigskip";
    return 0;
  case hfill_cmd: 
    if(chr==hfil_code) return "hfil";
    if(chr==hfill_code) return "hfill";
    if(chr==hfilneg_code) return "hfilneg";
    if(chr==hss_code) return "hss";
    return 0;
  case vfill_cmd:
    if(chr==vfil_code) return "vfil";
    if(chr==vfill_code) return "vfill";
    if(chr==vfilneg_code) return "vfilneg";
    if(chr==vss_code) return "vss";
    return 0;
  case soul_cmd:
  case sub_cmd:
    return token_soul_name();
  case oldfont_cmd: return token_oldfont_name();
  case argfont_cmd: return token_argfont_name();
  case noargfont_cmd: return token_noargfont_name();
  case fontsize_cmd: return token_fontsize_name();
  case item_cmd:  return chr==0 ? "item" : "@item";
  case usecounter_cmd: return  "usecounter";
  case get_config_cmd: 
    return chr==0 ? "tralics@find@config" : "tralics@getd@config";
  case kvo_family_cmd:
    switch(chr) {
    case kvo_fam_set_code: return "KVO@family@set";
    case kvo_fam_get_code: return "KVO@family";
    case kvo_pre_set_code: return "KVO@prefix@set";
    case kvo_pre_get_code: return "KVO@prefix"; 
    case kvo_bool_opt_code: return "DeclareBoolOption";
    case kvo_comp_opt_code: return "DeclareComplementaryOption";
    case kvo_boolkey_code: return "KVO@boolkey";
    case kvo_voidkey_code: return "KVO@voidkey";
    case kvo_string_opt_code: return "DeclareStringOption";
    case kvo_void_opt_code: return "DeclareVoidOption";
    case kvo_disable_code: return "DisableKeyvalOption";
    case kvo_decdef_code: return "DeclareDefaultOption";
    case kvo_process_code: return "ProcessKeyvalOptions";
    default: return 0;
    }
  case pers_cmd: 
    switch(chr) {
    case 3: return "@persB";
    case 2: return "persB";
    case 1: return "@persA";
    default: return "persA";
    }
  case doc_class_cmd: 
    return  chr == 0 ? "documentclass" : "LoadClass";
  case package_cmd: return  "usepackage";
  case inputclass_cmd: return  "InputClass";
  case needs_format_cmd: return "NeedsTeXFormat";
  case if_package_loaded_cmd:
    return chr== 0 ? "@ifclassloaded" : "@ifpackageloaded";
  case if_package_later_cmd:
    return chr== 0 ? "@ifclasslater" : "@ifpackagelater";
  case if_package_with_cmd:
    return chr== 0 ? "@ifclasswith" : "@ifpackagewith";
  case provides_package_cmd:
    return chr== 0 ? "ProvidesFile" :
      chr== 1 ? "ProvidesPackage" : "ProvidesClass";
  case pass_options_cmd:
    return chr== 0 ?"PassOptionsToClass" : "PassOptionsToPackage";
  case declare_options_cmd: return "DeclareOption";
  case options_not_used_cmd: return  "OptionNotUsed";
  case process_options_cmd: return "ProcessOptions";
  case execute_options_cmd: return "ExecuteOptions";
  case at_end_of_class_cmd:
    return chr== 0 ? "AtEndOfClass" : "AtEndOfPackage";
  case load_with_options_cmd:
    return chr== 0 ? "LoadClassWithOptions" : "RequirePackageWithOptions";
  case float_cmd:
    switch(chr) {
    case 0 : return "@float"; 
    case 1 : return "@dblfloat"; 
    case 2 : return "float@end"; 
    case 3 : return "float@dblend"; 
    default: return 0; 
    }
  case subfigure_cmd: return  "subfigure";
  case pop_stack_cmd: return  "pop@stack";
  case box_cmd: return token_box_name();
  case math_comp_cmd: return token_mathcomp_name();
  case big_cmd: return token_big_name();
  case style_cmd: return token_style_name();
  case limits_cmd: return token_limits_name();
  case ensuremath_cmd: return  "ensuremath";
  case nonumber_cmd: return  "nonumber";
  case nonscript_cmd: return  "nonscript";
  case eqno_cmd: switch(chr) {
    case eqno_code: return "eqno";
    case leqno_code: return "leqno";
    default: return 0;
    }
  case tag_cmd: switch(chr) {
    case 1: return "@xtag";
    case 2: return "@ytag";
    default: return "tag";
    }
  case line_cmd: return token_line_name();
  case linebreak_cmd: return token_linebreak_name();
  case fancy_cmd: return token_fancy_name();
  case xfancy_cmd: return "fancyinternal";
  case xthepage_cmd: return "inert@thepage";
  case url_cmd:
    return chr==0 ?  "url" :  "rrrt";
  case hanl_cmd: 
    return chr==0 ?  "htmladdnormallink" : 
    ( chr==1? "htmladdnormallinkfoot" : "href") ;
  case addatt_cmd: 
    switch(chr) {
    case addatt_to_cur_code: return "AddAttToCurrent";
    case addatt_to_last_code: return "AddAttToLast";
    case addatt_to_doc_code: return "addattributetodocument";
    case addatt_to_code: return "XMLaddatt";
    case addatt_to_index_code: return "addattributetoindex";
    default: return 0;
    }
  case over_cmd: return token_over_name();
  case begingroup_cmd: 
    return chr==0 ? "begingroup" : 
    chr==1 ? "endgroup" : 
    "endgroup (for env)";
  case startprojet_cmd: return  "RAstartprojet";
  case ra_startdoc_cmd: return  "rawebstartdocument";
  case declaretopics_cmd: return  "declaretopic";
  case footnote_cmd: return "footnote";
  case popmodule_cmd: return "tralics@pop@module";
  case pushmodule_cmd: return "tralics@push@module";
  case fnhack_cmd: return "tralics@fnhack";
  case interpret_rc_cmd: return "tralics@interpret@rc";
  case caption_cmd: return  "caption";
  case centering_cmd: return token_centering_name();
  case save_box_cmd: return chr==0? "sbox" : "savebox";
  case fbox_cmd: return token_fbox_name();
  case color_cmd: return token_color_name();
  case hspace_cmd: 
     switch(chr) {
     case 0: return "hspace" ;
     case 1: return "vspace" ;
     case 2: return "hspace*" ;
     default: return "vspace*" ;
     }
  case includegraphics_cmd: return  chr==0 ? "includegraphics" :  "psfig";
  case error_cmd: return  chr==1 ? "@notprerr" : "error";
  case glo_cmd: return  "glo";
  case reevaluate_cmd: return  "@reevaluate";
  case xmllatex_cmd: return "xmllatex";
  case specimp_cmd: 
  case unimp_cmd: 
    return token_unimp_name();
  case unimp_font_cmd: return token_unimp_font_name();
  case trees_cmd: return token_trees_name();
  case index_cmd: return token_index_name();
  case gloss_cmd: return chr==0 ? "cgloss@gll" : "cgloss@glll";
  case only_preamble_cmd: return "@onlypreamble";
  case toc_cmd: {
    if (chr == 1) return "listoftables";
    if (chr == 2) return "listoffigures";
    return "tableofcontents";
  }
  case random_cmd: return "tralics@random";
  case ot2enc_cmd: {
    if (chr==1) return "OT2-encoding";
    if (chr==2) return "OT1-encoding";
    return "T1-encoding";
  }
  case mathversion_cmd: return "mathversion";
  case matter_cmd:
    switch(chr) {
    case mainmatter_code: return "mainmatter";
    case frontmatter_code: return "frontmatter";
    case backmatter_code: return "backmatter";
    default:  return 0;
    }
  case cr_cmd:
    switch(chr) {
    case cr_code: return "cr";
    case mycr_code: return "cr";
    case crcr_code: return "crcr";
    case crwithargs_code: return "cr withargs";
    default: return 0;
    }
  case scan_glue_cmd:
    if(chr==hskip_code) return "hskip";
    if(chr==vskip_code) return "vskip";
    return "mskip";
  case case_shift_cmd: return token_caseshift_name();
  case kern_cmd: return chr==0 ? "kern" : "mkern";
  case make_box_cmd: return token_makebox_name();
  case XML_swap_cmd:
    if (chr==0) return "XML@moveAB";
    if (chr==1) return "XML@moveBA";
    return "XML@swapAB";
  case XML_fetch_cmd: return "XML@fetch";
  case XML_modify_cmd:
    switch(chr) {
    case xml_get_code: return "XML@get";
    case xml_ins_code: return "XML@insert";
    case xml_del_code: return "XML@delete";
    case xml_set_code: return "XML@set";
    case xml_parent_code: return "XML@parent";
    case xml_setA_code: return "XML@setA";
    case xml_setB_code: return "XML@setB";
    default: return 0;
    }
  case char_num_cmd: return "char";
  case cst1_cmd:
  case cst2_cmd:
  case cst_cmd: return token_cst_name();
  case specchar_cmd: return specchar_cmd_name();
  case nobreakspace_cmd: return "nobreakspace";
  case input_cmd:
  case file_cmd: return token_file_name();
  case xmlelt_cmd: return chr==0 ? "xmlelt" :
    (chr == 1 ? "xmlemptyelt" : "xmlcomment");
  case newcolumntype_cmd: return "newcolumntype";
  case arg_font_cmd: return chr==lsc_code ? "lsc" : "fsc";
  case rule_cmd: return chr== hrule_code ?  "hrule" : chr== vrule_code ?  "vrule" : "rule";
  case document_cmd: return  "document";
  case keywords_cmd: return  "motscle";
  case center_cmd: return strip_end(token_ecenter_name());
  case figure_cmd: return strip_end(token_efigure_name());
  case table_cmd:  return chr==0 ? "table" :  chr==2? "wraptable" : "table*";
  case glossaire_cmd: return  "glossaire";
  case itemize_cmd: return  "itemize";
  case list_cmd: return  "list";
  case enumerate_cmd: return  "enumerate";
  case description_cmd: return  "description";
  case listfiles_cmd: return "listfiles";
  case ignore_env_cmd: return strip_end(token_eignore_name());
  case ignore_content_cmd: return strip_end(token_eignorec_name());
  case raw_env_cmd: return  "rawxml";
  case math_env_cmd: return  strip_end(token_emath_name());
  case RAsection_env_cmd: return  "RAsection";
  case tabular_env_cmd: return  chr==0 ? "tabular":  "tabular*";
  case verbatim_env_cmd: 
     return chr==0 ? "verbatim" : chr==1 ? "Verbatim" : "@verbatim";
  case minipage_cmd: return  "minipage";
  case picture_env_cmd: return  "picture";
  case xmlelement_env_cmd: return chr==0 ? "xmlelement" : "xmlelement*";
  case filecontents_env_cmd: 
    return chr==0 ? "filecontents" : chr==1 ?"filecontents*" :
      chr==2 ? "filecontents+" : "filecontents";
  case end_document_cmd: return "enddocument";
  case end_keywords_cmd: return  "endmotscle";
  case end_center_cmd: return token_ecenter_name();
  case end_figure_cmd: return token_efigure_name();
  case end_table_cmd: return chr==0 ? "endtable" :  chr==2 ? "endwraptable" :   "endtable*";
  case end_glossaire_cmd: return  "endglossaire"; 
  case end_itemize_cmd: return  "enditemize"; 
  case end_list_cmd: return  "endlist"; 
  case end_enumerate_cmd: return  "endenumerate"; 
  case end_description_cmd: return  "enddescription"; 
  case end_ignore_env_cmd: return token_eignore_name();
  case end_ignore_content_cmd: return token_eignorec_name();
  case end_raw_env_cmd: return  "endrawxml";
  case end_math_env_cmd: return  token_emath_name();
  case end_RAsection_env_cmd: return  "endRAsection";
  case end_tabular_env_cmd: return chr==0 ? "endtabular": "endtabular*";
  case end_verbatim_env_cmd:
      return chr==0 ? "endverbatim" : chr==1 ? "endVerbatim" : "end@verbatim";
  case end_minipage_cmd: return  "endminipage"; 
  case end_picture_env_cmd: return  "endpicture"; 
  case end_xmlelement_env_cmd:
    return chr==0 ? "endxmlelement" : "endxmlelement*";
  case end_filecontents_env_cmd:
    return chr==0 ? "endfilecontents" : "endfilecontents*";
  case begin_cmd: return  "begin";
  case end_cmd: return  "end";
  case left_cmd: return "left"; 
  case right_cmd: return "right"; 
  case mathfont_cmd: return token_mathfont2_name();
  case math_font_cmd: return token_mathfont1_name();
  case special_math_cmd: 
  case specmath_cmd:
      return token_specialmath_name();
  case leave_v_mode_cmd: return "leavevmode";
  case epsfbox_cmd: return  "epsfbox";
  case put_cmd: return token_put_name();
  case bezier_cmd: return chr==0 ? "bezier" :   "qbezier";
  case dashline_cmd: return token_dashline_name();
  case ignore_cmd:
  case ignoreA_cmd:
  case ignorep_cmd:
    return token_ignore_name();
  case relax_cmd: return "relax";
  case ltfont_cmd: return token_ltfont_name();
  case ignore_one_argument_cmd: return token_ign1_name();
  case ignore_two_argument_cmd: return token_ign2_name();
  case defineverbatimenv_cmd: return "DefineVerbatimEnvironment";
  case saveverb_cmd: return "SaveVerb";
  case add_to_macro_cmd:
    return chr==1?"tralics@addtolist@o" : "tralics@addtolist@n";
  case makeatletter_cmd: return "makeatletter"; 
  case makeatother_cmd: return "makeatother";
  case numberedverbatim_cmd: return "numberedverbatim"; 
  case unnumberedverbatim_cmd: return "unnumberedverbatim";
  case after_assignment_cmd: return "afterassignment";
  case un_box_cmd: return token_unbox_name();
  case extension_cmd: return token_extension_name();
  case setlanguage_cmd: return "setlanguage";
  case xray_cmd: return token_xray_name();
  case move_cmd: return token_move_name();
  case leader_ship_cmd: return token_leader_name();
  case selectfont_cmd: return "selectfont";
  case usefont_cmd: return "usefont";
  case isin_cmd: return "in@";
  case tracingall_cmd: return  "tracingall";
  case newif_cmd: return  "newif";
  case newcount_cmd: return  token_newcount_name();
  case newboolean_cmd: return chr==0 ? "newboolean" :  "provideboolean";
  case setboolean_cmd: return  "setboolean";
  case ifthenelse_cmd: return  "ifthenelse";
  case whiledo_cmd: return  "whiledo";
  case expandtwoargs_cmd: return  "@expandtwoargs";
  case removeelement_cmd: return  "@removeelement";
  case shortverb_cmd:
    return chr==0 ? "DefineShortVerb" :  "UndefineShortVerb";
  case newcounter_cmd: return  "newcounter";
  case aftergroup_cmd: return  "aftergroup";
  case latex_ctr_cmd: return token_latexctr_name();
  case ifundefined_cmd: return chr==0? "@ifundefined" :  "tralics@ifundefined"; 
  case ifstar_cmd: return  "@ifstar";
  case ifnextchar_cmd: return  chr==0 ? "@ifnextchar" : "@ifnextcharacter";
  case ifempty_cmd: return  chr==0 ?   "@iftempty" : "@ifbempty";
  case typeout_cmd: return  chr==0 ? "typeout" : "wlog";
  case for_cmd: return token_for_name();
  case fp_cmd: return token_fp_names();
  case fpif_cmd: return token_fp_names();
  case fpi_cmd: return token_fpi_names();
  case verb_cmd: return chr==0 ? "verb": " verb";
  case last_item_cmd: return token_lastitem_name();
  case toks_register_cmd: return  "toks";
  case assign_toks_cmd: return token_assigntoks_name();
  case assign_int_cmd: return token_assignint_name();
  case assign_dimen_cmd: return token_assigndimen_name();
  case assign_glue_cmd: return token_assignglue_name();
  case assign_mu_glue_cmd: return token_assignmuglue_name();
  case assign_font_dimen_cmd: return "fontdimen";
  case assign_enc_char_cmd: return "input@encoding@val";
  case assign_font_int_cmd: return  chr==0 ? "hyphenchar" : "skewchar";
  case set_aux_cmd: 
    switch(chr) {
    case 0: return "spacefactor";
    case 1: return "prevdepth";
    case 2: return "input@encoding";
    case 4: return "@xfontsize";
    default: return "input@encoding@default";
    }
  case set_prev_graf_cmd: return "prevgraf";
  case set_page_dimen_cmd: return token_setpagedimen_name();
  case set_page_int_cmd: 
    if(chr== deadcycles_code) return "deadcycles";
    if(chr== interactionmode_code) return "interactionmode";
    return "insertpenalties";
  case set_box_dimen_cmd: return token_setboxdimen_name();
  case thm_aux_cmd:
    if(chr==theorem_style_code) return "theoremstyle";
    if(chr==theorem_bodyfont_code) return "theorembodyfont";
    if(chr==theorem_headerfont_code) return "theoremheaderfont";
    return 0;
  case start_thm_cmd:
    return chr==2 ?"@endtheorem" : "@begintheorem";
  case setmode_cmd: return  "@setmode";
  case set_shape_cmd: return token_shape_name(); 
  case def_code_cmd: return token_defcode_name();
  case def_family_cmd: return token_deffamily_name();
  case set_font_cmd: return  "nullfont";
  case def_font_cmd: return  "font";
  case set_mathprop_cmd: return  "mathfontproperty";
  case set_mathchar_cmd: return  "setmathchar";
  case register_cmd: return token_register_name();
  case advance_cmd: return "advance";
  case multiply_cmd: return  "multiply";
  case divide_cmd: return  "divide";
  case prefix_cmd: return token_prefix_name();
  case let_cmd: return chr==let_code?  "let" : "futurelet";
  case shorthand_def_cmd: return token_shorthand_name();
  case read_to_cs_cmd: return chr== 0 ? "read" : "readline";
  case def_cmd: return token_def_name();
  case set_box_cmd: return  "setbox";
  case set_interaction_cmd: return token_setinteraction_name();
  case xspace_cmd: return  "xspace";
  case strippt_cmd: return "strip@pt";
  case the_cmd: 
    if(chr==the_code) return  "the";
    else if(chr==unexpanded_code) return "unexpanded";
    else return "detokenize";
  case convert_cmd: return token_convert_name();
  case obracket_cmd: return  chr==0 ? "["  : "]";
  case oparen_cmd: return chr==0 ? "(" : ")";
  case csname_cmd: return  "csname"; 
  case expandafter_cmd: return chr==0? "expandafter" : "unless"; 
  case noexpand_cmd: return  "noexpand";
  case scanupdown_cmd: return "@scanupdown";
  case sideset_cmd: return "sideset";
  case split_cmd: return "tralics@split";
  case a_cmd: return  "a";
  case accent_cmd: return token_accent_name();
  case loop_cmd: return  "loop";
  case counter_cmd: return token_counter_name();
  case setlength_cmd: return chr==0 ?  "setlength" :  "addtolength";
  case useverb_cmd: return  "UseVerb";
  case first_of_one_cmd: return  "@firstofone";
  case first_of_two_cmd: return  chr==1 ? "@firstoftwo" : "@secondoftwo";
  case if_test_cmd: return token_iftest_name();
  case fi_or_else_cmd: return token_fiorelse_name();
  case top_bot_mark_cmd: return  token_mark_name();
  case char_given_cmd:
    return make_name("char",chr);
  case math_given_cmd:
    return make_name("mathchar",chr);
  case undef_cmd: return "undefined";
  case endv_cmd: return "endtemplate";
  case ding_cmd: return "ding";
  case grabenv_cmd: return "grabenv";
  case formatdate_cmd : return "formatdate";
  case thickness_cmd:
    if(chr==thicklines_code) return "thicklines";
    if(chr==thinlines_code) return "thinlines";
    if(chr==linethickness_code) return "linethickness";
    return 0;
  case bibliographystyle_cmd:
    return "bibliographystyle";
  case insertbibliohere_cmd: return "insertbibliohere";
  case inhibit_xml_cmd:
    return "syntaxonly";
  case footcitepre_cmd:
    return "footcitepre";
  case car_cmd: return chr==0 ? "@car" : "@cdr";
  case latex_error_cmd: return token_error_name();
  case xkeyval_cmd: return token_xkeyval_name();
  case curves_cmd:
    switch(chr) {
    case arc_code: return "arc";
    case bigcircle_code: return "bigcircle";
    case closecurve_code: return "closecurve"; 
    case curve_code: return "curve";
    case tagcurve_code: return "tagcurve";
    default: return 0;
    }
  case check_date_cmd: 
    switch(chr) {
    case 0: return  "checkdate";
    case 1: return "countdays";
    case 2: return "nextdate";
    case 3: return "prevdate";
    case 4: return "datebynumber";
    default: return 0;
    }
  case refstepcounter_cmd: return "refstepcounter";
  case month_day_cmd: return token_monthday_name();
  case change_element_name_cmd : return "ChangeElementName";
  case multispan_cmd : return "multispan";
  case stripprefix_cmd : return "strip@prefix";
  case hexnumber_cmd : return "hexnumber@";
  case zapspace_cmd : return chr==0 ?  "zap@space" : "zap@fl@space";
  case vglue_cmd : return chr==0 ? "vglue" : "hglue";
  case typein_cmd: return "typein";
  case cons_cmd: return "@cons";
  case afterfi_cmd: return "@afterfi";
  case afterelsefi_cmd: return "@afterelsefi";
  case testopt_cmd: return "@testopt";
  case gobble_cmd: return chr==1 ? "@gobble" : chr==2 ?"@gobbletwo" : "@gobblefour"; 
  case selective_sanitize_cmd: return "@selective@sanitize";
  case numberwithin_cmd: return "numberwithin";
  case ifdefinable_cmd: return "@ifdefinable";
  case dblarg_cmd: return "@dblarg";
  case makelabel_cmd: return "tralics@makelabel";
  default: return 0;
  }
}
