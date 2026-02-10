#include "tralics/Buffer.h"
#include "tralics/Dispatcher.h"
#include "tralics/Symcode.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

// We define functions that return the name of a token;

namespace tralics_ns {
    auto math_env_name(subtypes c) -> std::optional<std::string_view>;
} // namespace tralics_ns

namespace {
    auto make_name(const std::string &x, unsigned y) -> std::string { return fmt::format("{}{}", x, y); }
} // namespace

auto CmdChr::specchar_cmd_name() const -> std::string { return fmt::format("Character U+{:04X}", unsigned(chr)); }

auto CmdChr::token_error_name() const -> std::optional<std::string_view> {
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
    default: return std::nullopt;
    }
}

auto CmdChr::token_monthday_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case md_frenchm_code: return "@monthnamefrench";
    case md_frenchd_code: return "@daynamefrench";
    case md_spanishm_code: return "@monthnamespanish";
    case md_spanishd_code: return "@daynamespanis";
    case md_germanm_code: return "@monthnamegerman";
    case md_germand_code: return "@daynamegerman";
    case md_englishm_code: return "@monthnameenglish";
    case md_englishd_code: return "@daynameenglish";
    default: return std::nullopt;
    }
}

auto token_math_name(subtypes c) -> std::string {
    switch (unsigned(c)) {
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
    case ell_code: return "ell";
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
    default: return "";
    }
}

// Aux functions for CmdChr::name() const
auto CmdChr::token_fancy_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case fancy_lhead_code: return "lhead";
    case fancy_chead_code: return "chead";
    case fancy_rhead_code: return "rhead";
    case fancy_lfoot_code: return "lfoot";
    case fancy_cfoot_code: return "cfoot";
    case fancy_rfoot_code: return "rfoot";
    case fancy_head_code: return "fancyhead";
    case fancy_foot_code: return "fancyfoot";
    case fancy_hf_code: return "fancyhf";
    default: return std::nullopt;
    }
}
auto CmdChr::token_color_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case definecolor_code: return "definecolor";
    case color_code: return "color";
    case normalcolor_code: return "normalcolor";
    case pagecolor_code: return "pagecolor";
    case colorbox_code: return "colorbox";
    case fcolorbox_code: return "fcolorbox";
    default: return std::nullopt;
    }
}

auto CmdChr::token_fbox_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case fbox_code: return "fbox";
    case framebox_code: return "framebox";
    case rotatebox_code: return "rotatebox";
    case scalebox_code: return "scalebox";
    case raisebox_code: return "raisebox";
    case dashbox_code: return "dashbox";
    default: return std::nullopt;
    }
}

auto CmdChr::token_linebreak_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case pagebreak_code: return "pagebreak";
    case nopagebreak_code: return "nopagebreak";
    case linebreak_code: return "linebreak";
    case nolinebreak_code: return "nolinebreak";
    default: return std::nullopt;
    }
}

auto CmdChr::token_mark_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case topmark_code: return "topmark";
    case firstmark_code: return "firstmark";
    case botmark_code: return "botmark";
    case splitfirstmark_code: return "splitfirstmark";
    case splitbotmark_code: return "splitbotmark";
    case topmarks_code: return "topmarks";
    case firstmarks_code: return "firstmarks";
    case botmarks_code: return "botmarks";
    case splitfirstmarks_code: return "splitfirstmarks";
    case splitbotmarks_code: return "splitbotmarks";
    default: return std::nullopt;
    }
}

auto CmdChr::token_fiorelse_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case fi_code: return "fi";
    case or_code: return "or";
    case else_code: return "else";
    default: return std::nullopt;
    }
}

auto CmdChr::token_iftest_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case if_char_code: return "if";
    case if_cat_code: return "ifcat";
    case if_int_code: return "ifnum";
    case if_dim_code: return "ifdim";
    case if_odd_code: return "ifodd";
    case if_vmode_code: return "ifvmode";
    case if_hmode_code: return "ifhmode";
    case if_mmode_code: return "ifmmode";
    case if_inner_code: return "ifinner";
    case if_void_code: return "ifvoid";
    case if_hbox_code: return "ifhbox";
    case if_vbox_code: return "ifvbox";
    case ifx_code: return "ifx";
    case if_eof_code: return "ifeof";
    case if_true_code: return "iftrue";
    case if_false_code: return "iffalse";
    case if_case_code: return "ifcase";
    case if_defined_code: return "ifdefined";
    case if_csname_code: return "ifcsname";
    case if_fontchar_code: return "iffontchar";
    case if_leapyear_code: return "ifleapyear";
    default: return std::nullopt;
    }
}

auto CmdChr::token_counter_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case killcounter_code: return "@stpelt";
    case stepcounter_code: return "stepcounter";
    case addtocounter_code: return "addtocounter";
    case setcounter_code: return "setcounter";
    case value_code: return "value";
    case addtoreset_code: return "@addtoreset";
    default: return std::nullopt;
    }
}

auto CmdChr::token_accent_name() const -> std::string {
    switch (chr) {
    case '\'': return "'";
    case '`': return "`";
    case '"': return "\"";
    case '^': return "^";
    case '~': return "~";
    case 'k': return "k";
    case 'H': return "H";
    case 'v': return "v";
    case 'u': return "u";
    case 'C': return "C";
    case 'c': return "c";
    case '.': return ".";
    case '=': return "=";
    case 'r': return "r";
    case 'b': return "b";
    case 'd': return "d";
    case 'D': return "D";
    case 'h': return "H";
    default: return {};
    }
}

auto CmdChr::token_convert_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case number_code: return "number";
    case at_arabic_code: return "@arabic";
    case romannumeral_code: return "romannumeral";
    case Romannumeral_code: return "Romannumeral";
    case string_code: return "string";
    case meaning_code: return "meaning";
    case meaning_c_code: return "cs_meaning:c";
    case fontname_code: return "fontname";
    case jobname_code: return "jobname";
    case attributeval_code: return "XMLgetatt";
    case tralicsversion_code: return "tralicsversion";
    case etexrevision_code: return "eTeXrevision";
    case sanitize_code: return "@onelevel@sanitize";
    case twodigits_code: return "two@digits";
    default: return std::nullopt;
    }
}

auto CmdChr::token_for_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case xkv_for_n_code: return "tralics@for@n";
    case xkv_for_en_code: return "tralics@for@en";
    case xkv_for_o_code: return "tralics@for@o";
    case xkv_for_i_code: return "tralics@for@i";
    case xkv_for_eo_code: return "tralics@for@eo";
    case for_code: return "@for";
    case tfor_code: return "@tfor";
    case forloop_code: return "@forloop";
    case iforloop_code: return "@iforloop";
    case tforloop_code: return "@tforloop";
    case breaktfor_code: return "@break@tfor";
    case xkv_breaktfor_code: return "tralics@for@break";
    default: return std::nullopt;
    }
}

auto CmdChr::token_lastitem_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case lastpenalty_code: return "lastpenalty";
    case lastkern_code: return "lastkern";
    case lastskip_code: return "lastskip";
    case inputlineno_code: return "inputlineno";
    case badness_code: return "badness";
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
    case fontchardp_code: return "fontchardp";
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
    default: return std::nullopt;
    }
}

auto CmdChr::token_over_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case above_code: return "above";
    case over_code: return "over";
    case atop_code: return "atop";
    case abovewithdelims_code: return "abovewithdelims";
    case overwithdelims_code: return "overwithdelims";
    case atopwithdelims_code: return "atopwithdelims";
    default: return std::nullopt;
    }
}

auto CmdChr::token_limits_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case 0: return "displaylimits";
    case 2: return "limits";
    case 1: return "nolimits";
    default: return std::nullopt;
    }
}

auto CmdChr::token_style_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case displaystyle_code: return "displaystyle";
    case textstyle_code: return "textstyle";
    case scriptstyle_code: return "scriptstyle";
    case scriptscriptstyle_code: return "scriptscriptstyle";
    default: return std::nullopt;
    }
}

auto CmdChr::token_big_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case big_code: return "big";
    case Big_code: return "Big";
    case bigg_code: return "bigg";
    case Bigg_code: return "Bigg";
    case bigl_code: return "bigl";
    case Bigl_code: return "Bigl";
    case biggl_code: return "biggl";
    case Biggl_code: return "Biggl";
    case bigr_code: return "bigr";
    case Bigr_code: return "Bigr";
    case biggr_code: return "biggr";
    case Biggr_code: return "Biggr";
    case bigm_code: return "bigm";
    case Bigm_code: return "Bigm";
    case biggm_code: return "biggm";
    case Biggm_code: return "Biggm";
    default: return std::nullopt;
    }
}

auto CmdChr::token_mathcomp_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case ord_noad: return "mathord";
    case op_noad: return "mathop";
    case bin_noad: return "mathbin";
    case rel_noad: return "mathrel";
    case open_noad: return "mathopen";
    case close_noad: return "mathclose";
    case punct_noad: return "mathpunct";
    case inner_noad: return "mathinner";
    case between_noad: return "mathbetween";
    default: return std::nullopt;
    }
}

auto CmdChr::token_eignorec_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case htmlonly_code: return "endhtmlonly";
    case comment_code: return "endcomment";
    case rawhtml_code: return "endrawhtml";
    case LaTeXonly_code: return "endLaTeXonly";
    default: return std::nullopt;
    }
}

auto CmdChr::token_eignore_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case body_code: return "endbody";
    case abstract_code: return "endabstract";
    case latexonly_code: return "endlatexonly";
    case xmlonly_code: return "endxmlonly";
    default: return std::nullopt;
    }
}

auto CmdChr::token_efigure_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case 0: return "endfigure";
    case 1: return "endfigure*";
    case 2: return "endwrapfigure";
    default: return std::nullopt;
    }
}

auto CmdChr::token_ecenter_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case center_code: return "endcenter";
    case quote_code: return "endquote";
    case quotation_code: return "endquotation";
    case flushleft_code: return "endflushleft";
    case flushright_code: return "endflushright";
    case verse_code: return "endverse";
    default: return std::nullopt;
    }
}

// input and friends
auto CmdChr::token_file_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case ifexists_code: return "IfFileExists";
    case inputifexists_code: return "InputIfFileExists";
    case input_code: return "input";
    case Input_code: return "Input";
    case endinput_code: return "endinput";
    case openin_code: return "openin";
    case closein_code: return "closein";
    case include_code: return "include";
    case scantokens_code: return "scantokens";
    case readxml_code: return "readxml";
    default: return std::nullopt;
    }
}

auto CmdChr::token_dashline_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case dashline_code: return "dashline";
    case drawline_code: return "drawline";
    case dottedline_code: return "dottedline";
    case circle_code: return "circle";
    default: return std::nullopt;
    }
}

auto CmdChr::token_put_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case put_code: return "put";
    case line_code: return "line";
    case vector_code: return "vector";
    case oval_code: return "oval";
    case multiput_code: return "multiput";
    case scaleput_code: return "scaleput";
    case frame_code: return "frame";
    default: return std::nullopt;
    }
}

auto CmdChr::token_caseshift_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case 0: return "lowercase";
    case 1: return "uppercase";
    case 2: return "MakeLowercase";
    case 3: return "MakeUppercase";
    case 4: return "MakeTextLowercase";
    case 5: return "MakeTextUppercase";
    case 6: return "tl_to_lowercase";
    case 7: return "tl_to_uppercase";
    default: return std::nullopt;
    }
}

auto CmdChr::token_centering_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case center_code: return "centering";
    case flushright_code: return "raggedleft";
    case flushleft_code: return "raggedright";
    default: return std::nullopt;
    }
}

auto CmdChr::token_box_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case mbox_code: return "mbox";
    case makebox_code: return "makebox";
    case text_code: return "text";
    default: return std::nullopt;
    }
}

auto CmdChr::token_section_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case part_code: return "part";
    case chapter_code: return "chapter";
    case section_code: return "section";
    case subsection_code: return "subsection";
    case subsubsection_code: return "subsubsection";
    case paragraph_code: return "paragraph";
    case subparagraph_code: return "subparagraph";
    case endsec_code: return "endsec";
    case toplevel_sec_code: return "toplevelsection";
    default: return std::nullopt;
    }
}

auto CmdChr::token_cite_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case cite_code: return "cite";
    case nocite_code: return "nocite";
    case natcite_code: return "natcite";
    case natcite_e_code: return "endnatcite";
    default: return std::nullopt;
    }
}

auto CmdChr::token_latexctr_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case arabic_code: return "arabic";
    case roman_code: return "roman";
    case Roman_code: return "Roman";
    case alph_code: return "alph";
    case Alph_code: return "Alph";
    case fnsymbol_code: return "fnsymbol";
    case at_alph_code: return "@alph";
    case at_Alph_code: return "@Alph";
    case at_fnsymbol_code: return "@fnsymbol";
    default: return std::nullopt;
    }
}

auto CmdChr::token_setinteraction_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case batchmode_code: return "batchmode";
    case nonstopmode_code: return "nonstopmode";
    case scrollmode_code: return "scrollmode";
    case errorstopmode_code: return "errorstopmode";
    default: return std::nullopt;
    }
}

auto CmdChr::token_shape_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case parshape_code: return "parshape";
    case interlinepenalties_code: return "interlinepenalties";
    case clubpenalties_code: return "clubpenalties";
    case widowpenalties_code: return "widowpenalties";
    case displaywidowpenalties_code: return "displaywidowpenalties";
    default: return std::nullopt;
    }
}
auto CmdChr::token_def_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case xdef_code: return "xdef";
    case def_code: return "def";
    case gdef_code: return "gdef";
    case edef_code: return "edef";
    case lgdef_code: return "cs_gset:Npn"; // next 12 latex3
    case lxdef_code: return "cs_gset:Npx";
    case ldef_code: return "cs_set:Npn";
    case ledef_code: return "cs_set:Npx";
    case pdef_code: return "cs_set_protected_nopar:Npn";
    case pedef_code: return "cs_set_protected_nopar:Npx";
    case pldef_code: return "cs_set_protected:Npn";
    case pledef_code: return "cs_set_protected:Npx";
    case pgdef_code: return "cs_gset_protected_nopar:Npn";
    case pxdef_code: return "cs_gset_protected_nopar:Npx";
    case plgdef_code: return "cs_gset_protected:Npn";
    case plxdef_code: return "cs_gset_protected:Npx";

    case gdefn_code: return "cs_new_nopar:Npn";
    case xdefn_code: return "cs_new_nopar:Npx";
    case lgdefn_code: return "cs_new:Npn";
    case lxdefn_code: return "cs_new:Npx";
    case pgdefn_code: return "cs_new_protected_nopar:Npn";
    case pxdefn_code: return "cs_new_protected_nopar:Npx";
    case plgdefn_code: return "cs_new_protected:Npn";
    case plxdefn_code: return "cs_new_protected:Npx";
    case cdef_code: return "cs_set_nopar:cpn";
    case cedef_code: return "cs_set_nopar:cpx";
    case cgdef_code: return "cs_gset_nopar:cpn";
    case cxdef_code: return "cs_gset_nopar:cpx";
    case cgdefn_code: return "cs_new_nopar:cpn";
    case cxdefn_code: return "cs_new_nopar:cpx";
    case cldef_code: return "cs_set:cpn";
    case cledef_code: return "cs_set:cpx";
    case clgdef_code: return "cs_gset:cpn";
    case clxdef_code: return "cs_gset:cpx";
    case clgdefn_code: return "cs_new:cpn";
    case clxdefn_code: return "cs_new:cpx";
    case cpdef_code: return "cs_set_protected_nopar:cpn";
    case cpedef_code: return "cs_set_protected_nopar:cpx";
    case cpgdef_code: return "cs_gset_protected_nopar:cpn";
    case cpxdef_code: return "cs_gset_protected_nopar:cpx";
    case cpgdefn_code: return "cs_new_protected_nopar:cpn";
    case cpxdefn_code: return "cs_new_protected_nopar:cpx";
    case cpldef_code: return "cs_set_protected:cpn";
    case cpledef_code: return "cs_set_protected:cpx";
    case cplgdef_code: return "cs_gset_protected:cpn";
    case cplxdef_code: return "cs_gset_protected:cpx";
    case cplgdefn_code: return "cs_new_protected:cpn";
    case cplxdefn_code: return "cs_new_protected:cpx";

    case newcommand_code: return "newcommand";
    case newthm_code: return "newtheorem";
    case newenv_code: return "newenvironment";
    case renewenv_code: return "renewenvironment";
    case renew_code: return "renewcommand";
    case provide_code: return "providecommand";
    case checkcommand_code: return "CheckCommand";
    case declare_math_operator_code: return "DeclareMathOperator";
    default: return std::nullopt;
    }
}

auto CmdChr::token_shorthand_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case char_def_code: return "chardef";
    case math_char_def_code: return "mathchardef";
    case count_def_code: return "countdef";
    case dimen_def_code: return "dimendef";
    case skip_def_code: return "skipdef";
    case mu_skip_def_code: return "muskipdef";
    case toks_def_code: return "toksdef";
    default: return std::nullopt;
    }
}

auto CmdChr::token_prefix_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case global_code: return "global";
    case long_code: return "long";
    case outer_code: return "outer";
    case protected_code: return "protected";
    default: return std::nullopt;
    }
}

auto CmdChr::token_register_name() const -> std::optional<std::string_view> {
    switch (static_cast<internal_type>(chr)) {
    case it_int: return "count";
    case it_dimen: return "dimen";
    case it_glue: return "skip";
    case it_mu: return "muskip";
    default: return std::nullopt;
    }
}

auto CmdChr::token_deffamily_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case textfont_code: return "textfont";
    case scriptfont_code: return "scriptfont";
    case scriptscriptfont_code: return "scriptscriptfont";
    default: return std::nullopt;
    }
}

auto CmdChr::token_defcode_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case 0: return "catcode";
    case math_code_offset: return "mathcode";
    case lc_code_offset: return "lccode";
    case uc_code_offset: return "uccode";
    case sf_code_offset: return "sfcode";
    case del_code_offset: return "delcode";
    default: return std::nullopt;
    }
}

auto CmdChr::token_setboxdimen_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case wd_code: return "wd";
    case ht_code: return "ht";
    case dp_code: return "dp";
    default: return std::nullopt;
    }
}

auto CmdChr::token_setpagedimen_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case pagegoal_code: return "pagegoal";
    case pagetotal_code: return "pagetotal";
    case pagestretch_code: return "pagestretch";
    case pagefilstretch_code: return "pagefilstretch";
    case pagefillstretch_code: return "pagefillstretch";
    case pagefilllstretch_code: return "pagefilllstretch";
    case pageshrink_code: return "pageshrink";
    case pagedepth_code: return "pagedepth";
    default: return std::nullopt;
    }
}

auto CmdChr::token_assignmuglue_name() const -> std::string { return token_assignglue_name(); }

auto CmdChr::token_assignglue_name() const -> std::string {
    switch (chr) {
    case lineskip_code: return "lineskip";
    case baselineskip_code: return "baselineskip";
    case parskip_code: return "parskip";
    case abovedisplayskip_code: return "abovedisplayskip";
    case belowdisplayskip_code: return "belowdisplayskip";
    case abovedisplayshortskip_code: return "abovedisplayshortskip";
    case belowdisplayshortskip_code: return "belowdisplayshortskip";
    case leftskip_code: return "leftskip";
    case rightskip_code: return "rightskip";
    case topskip_code: return "topskip";
    case splittopskip_code: return "splittopskip";
    case tabskip_code: return "tabskip";
    case spaceskip_code: return "spaceskip";
    case xspaceskip_code: return "xspaceskip";
    case parfillskip_code: return "parfillskip";

    case thinmuskip_code: return "thinmuskip";
    case medmuskip_code: return "medmuskip";
    case thickmuskip_code: return "thickmuskip";

    default:
        if (chr < 256)
            return make_name("skip", chr);
        else
            return make_name("muskip", chr - muskip_reg_offset);
    }
}

auto CmdChr::token_assigndimen_name() const -> std::string {
    switch (chr) {
    case parindent_code: return "parindent";
    case mathsurround_code: return "mathsurround";
    case lineskiplimit_code: return "lineskiplimit";
    case hsize_code: return "hsize";
    case vsize_code: return "vsize";
    case maxdepth_code: return "maxdepth";
    case splitmaxdepth_code: return "splitmaxdepth";
    case boxmaxdepth_code: return "boxmaxdepth";
    case hfuzz_code: return "hfuzz";
    case vfuzz_code: return "vfuzz";
    case delimitershortfall_code: return "delimitershortfall";
    case nulldelimiterspace_code: return "nulldelimiterspace";
    case scriptspace_code: return "scriptspace";
    case predisplaysize_code: return "predisplaysize";
    case displaywidth_code: return "displaywidth";
    case displayindent_code: return "displayindent";
    case overfullrule_code: return "overfullrule";
    case hangindent_code: return "hangindent";
    case hoffset_code: return "hoffset";
    case voffset_code: return "voffset";
    case emergencystretch_code: return "emergencystretch";
    case unitlength_code: return "unitlength";
    case textheight_code: return "textheight";
    case textwidth_code: return "textwidth";
    case linewidth_code: return "linewidth";
    case columnwidth_code: return "columnwidth";
    default: return make_name("dimen", chr);
    }
}

auto CmdChr::token_assignint_name() const -> std::string {
    switch (chr) {
    case pretolerance_code: return "pretolerance";
    case tolerance_code: return "tolerance";
    case linepenalty_code: return "linepenalty";
    case hyphenpenalty_code: return "hyphenpenalty";
    case exhyphenpenalty_code: return "exhyphenpenalty";
    case clubpenalty_code: return "clubpenalty";
    case widowpenalty_code: return "widowpenalty";
    case displaywidowpenalty_code: return "displaywidowpenalty";
    case brokenpenalty_code: return "brokenpenalty";
    case binoppenalty_code: return "binoppenalty";
    case relpenalty_code: return "relpenalty";
    case predisplaypenalty_code: return "predisplaypenalty";
    case postdisplaypenalty_code: return "postdisplaypenalty";
    case interlinepenalty_code: return "interlinepenalty";
    case outputpenalty_code: return "outputpenalty";
    case doublehyphendemerits_code: return "doublehyphendemerits";
    case finalhyphendemerits_code: return "finalhyphendemerits";
    case adjdemerits_code: return "adjdemerits";
    case mag_code: return "mag";
    case delimiterfactor_code: return "delimiterfactor";
    case looseness_code: return "looseness";
    case time_code: return "time";
    case day_code: return "day";
    case month_code: return "month";
    case year_code: return "year";
    case showboxbreadth_code: return "showboxbreadth";
    case nomath_code: return "@nomathml";
    case multimlabel_code: return "multi@math@label";
    case notrivialmath_code: return "notrivialmath";
    case showboxdepth_code: return "showboxdepth";
    case hbadness_code: return "hbadness";
    case vbadness_code: return "vbadness";
    case pausing_code: return "pausing";
    case tracingonline_code: return "tracingonline";
    case tracingmacros_code: return "tracingmacros";
    case tracingstats_code: return "tracingstats";
    case tracingparagraphs_code: return "tracingparagraphs";
    case tracingpages_code: return "tracingpages";
    case tracingoutput_code: return "tracingoutput";
    case tracinglostchars_code: return "tracinglostchars";
    case tracingcommands_code: return "tracingcommands";
    case tracingrestores_code: return "tracingrestores";
    case tracingassigns_code: return "tracingassigns";
    case tracinggroups_code: return "tracinggroups";
    case tracingifs_code: return "tracingifs";
    case tracingmath_code: return "tracingmath";
    case tracingscantokens_code: return "tracingscantokens";
    case tracingnesting_code: return "tracingnesting";
    case predisplaydirection_code: return "predisplaydirection";
    case TeXXetstate_code: return "TeXXeTstate";
    case uchyph_code: return "uchyph";
    case maxdeadcycles_code: return "maxdeadcycles";
    case hangafter_code: return "hangafter";
    case floatingpenalty_code: return "floatingpenalty";
    case globaldefs_code: return "globaldefs";
    case fam_code: return "fam";
    case escapechar_code: return "escapechar";
    case defaulthyphenchar_code: return "defaulthyphenchar";
    case defaultskewchar_code: return "defaultskewchar";
    case endlinechar_code: return "endlinechar";
    case newlinechar_code: return "newlinechar";
    case language_code: return "language";
    case lefthyphenmin_code: return "lefthyphenmin";
    case righthyphenmin_code: return "righthyphenmin";
    case holdinginserts_code: return "holdinginserts";
    case errorcontextlines_code: return "errorcontextlines";
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
        if (chr < math_code_offset) return make_name("catcode", chr);
        if (chr < lc_code_offset) return make_name("mathcode", chr - math_code_offset);
        if (chr < uc_code_offset) return make_name("lccode", chr - lc_code_offset);
        if (chr < sf_code_offset) return make_name("uccode", chr - uc_code_offset);
        if (chr < del_code_offset) return make_name("sfcode", chr - sf_code_offset);
        if (chr < count_reg_offset) return make_name("delcode", chr - del_code_offset);
        if (chr < mu_reg_offset) return make_name("count", chr - count_reg_offset);
        if (chr < other_int_offset) return make_name("mu", chr - mu_reg_offset);
        return make_name("countref", chr);
    }
}

auto CmdChr::token_assigntoks_name() const -> std::string {
    switch (chr) {
    case output_code: return "output";
    case everypar_code: return "everypar";
    case everymath_code: return "everymath";
    case everybibitem_code: return "everybibitem";
    case everydisplay_code: return "everydisplay";
    case everyhbox_code: return "everyhbox";
    case everyvbox_code: return "everyvbox";
    case everyxbox_code: return "everyxbox";
    case everyjob_code: return "everyjob";
    case everycr_code: return "everycr";
    case everyeof_code: return "everyeof";
    case errhelp_code: return "errhelp";
    case theorem_style_code: return "theorem style";
    case theorem_bodyfont_code: return "theorem bodyfont";
    case theorem_headerfont_code: return "theorem headerfont";
    default: return make_name("toks", chr);
    }
}

auto CmdChr::token_leader_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case shipout_code: return "shipout";
    case leaders_code: return "leaders";
    case cleaders_code: return "cleaders";
    case xleaders_code: return "xleaders";
    default: return std::nullopt;
    }
}
auto CmdChr::token_index_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case index_code: return "index";
    case glossary_code: return "glossary";
    case printindex_code: return "printindex";
    case printglossary_code: return "printglossary";
    case makeindex_code: return "makeindex";
    case makeglossary_code: return "makeglossary";
    case newindex_code: return "@newindex";
    default: return std::nullopt;
    }
}
auto CmdChr::token_newcount_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case newcount_code: return "newcount";
    case newtoks_code: return "newtoks";
    case newbox_code: return "newbox";
    case newdimen_code: return "newdimen";
    case newlength_code: return "newlength";
    case newmuskip_code: return "newmuskip";
    case newread_code: return "newread";
    case newwrite_code: return "newwrite";
    case newlanguage_code: return "newlanguage";
    default: return std::nullopt;
    }
}

auto CmdChr::token_line_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case leftline_code: return "leftline";
    case rightline_code: return "rightline";
    case centerline_code: return "centerline";
    case llap_code: return "llap";
    case rlap_code: return "rlap";
    default: return std::nullopt;
    }
}

auto CmdChr::token_makebox_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case box_code: return "box";
    case copy_code: return "copy";
    case usebox_code: return "usebox";
    case lastbox_code: return "lastbox";
    case useboxA_code: return "XML@copyA";
    case useboxB_code: return "XML@copyB";
    case vsplit_code: return "vsplit";
    case vtop_code: return "vtop";
    case vbox_code: return "vbox";
    case hbox_code: return "hbox";
    case xbox_code: return "xbox";
    case parbox_code: return "parbox";
    case marginpar_code: return "marginpar";
    default: return std::nullopt;
    }
}

auto CmdChr::token_move_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case moveleft_code: return "moveleft";
    case moveright_code: return "moveright";
    case raise_code: return "raise";
    case lower_code: return "lower";
    default: return std::nullopt;
    }
}

auto CmdChr::token_xray_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case show_code: return "show";
    case showbox_code: return "showbox";
    case show_xmlA_code: return "show@xmlA";
    case show_xmlB_code: return "show@xmlB";
    case showthe_code: return "showthe";
    case showlists_code: return "showlists";
    case showgroups_code: return "showgroups";
    case showifs_code: return "showifs";
    case showtokens_code: return "showtokens";
    case register_show_code: return "__kernel_register_show:N";
    case registerc_show_code: return "__kernel_register_show:c";
    default: return std::nullopt;
    }
}

auto CmdChr::token_extension_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case openout_code: return "openout";
    case write_code: return "write";
    case closeout_code: return "closeout";
    case write_log_code: return "iow_log:x";
    case write_term_code: return "iow_term:x";
    case typeout_code: return "typeout";
    case wlog_code: return "wlog";
    default: return std::nullopt;
    }
}

auto CmdChr::token_unbox_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case unhbox_code: return "unhbox";
    case unhcopy_code: return "unhcopy";
    case unvbox_code: return "unvbox";
    case unvcopy_code: return "unvcopy";
    default: return std::nullopt;
    }
}

auto CmdChr::token_ltfont_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case fontencoding_code: return "fontencoding";
    case fontfamily_code: return "fontfamily";
    case fontseries_code: return "fontseries";
    case fontshape_code: return "fontshape";
    default: return std::nullopt;
    }
}

auto CmdChr::token_ign2_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case HTMLset_code: return "HTMLset";
    case fontsize_code: return "fontsize";
    default: return std::nullopt;
    }
}

auto CmdChr::token_ign1_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case RAlabel_code: return "RAlabel";
    case hyphenation_code: return "hyphenation";
    case patterns_code: return "patterns";
    case special_code: return "special";
    case includeonly_code: return "includeonly";
    case showhyphens_code: return "showhyphens";

    default: return std::nullopt;
    }
}

auto CmdChr::token_ignore_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case protect_code: return "protect";
    case sloppy_code: return "sloppy";
    case fussy_code: return "fussy";
    case subitem_code: return "subitem";
    case htmlimage_code: return "htmlimage";
    case immediate_code: return "immediate";
    case unpenalty_code: return "unpenalty";
    case unkern_code: return "unkern";
    case unskip_code: return "unskip";
    case addnl_code: return "@addnl";
    case atsign_code: return "@";
    case italiccorr_code: return "/";
    case clearpage_code: return "clearpage";
    case samepage_code: return "samepage";
    case newpage_code: return "newpage";
    case hrulefill_code: return "hrulefill";
    case dotfill_code: return "dotfill";
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
    case beginR_code: return "beginR";
    case pagediscards_code: return "pagediscards";
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
    default: return "";
    }
}

auto CmdChr::token_fontsize_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case tiny_code: return "tiny";
    case scriptsize_code: return "scriptsize";
    case footnotesize_code: return "footnotesize";
    case small_code: return "small";
    case normalsize_code: return "normalsize";
    case large_code: return "large";
    case Large_code: return "Large";
    case LARGE_code: return "LARGE";
    case huge_code: return "huge";
    case Huge_code: return "Huge";
    default: return std::nullopt;
    }
}

auto CmdChr::token_argfont_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case rm_family_code: return "textrm";
    case sf_family_code: return "textsf";
    case tt_family_code: return "texttt";
    case md_series_code: return "textmd";
    case bf_series_code: return "textbf";
    case up_shape_code: return "textup";
    case it_shape_code: return "textit";
    case sl_shape_code: return "textsl";
    case sc_shape_code: return "textsc";
    case em_code: return "emph";
    case normalfont_code: return "textnormal";
    default: return std::nullopt;
    }
}

auto CmdChr::token_soul_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case soul_sup_code: return "textsuperscript";
    case soul_sub_code: return "textsubscript";
    case soul_oldstyle_code: return "oldstylenums";
    case soul_caps_code: return "caps";
    case soul_hl_code: return "hl";
    case soul_so_code: return "so";
    case soul_st_code: return "st";
    case soul_ul_code: return "ul";
    default: return std::nullopt;
    }
}

auto CmdChr::token_oldfont_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case rm_family_code: return "rm";
    case sf_family_code: return "sf";
    case tt_family_code: return "tt";
    case bf_series_code: return "bf";
    case it_shape_code: return "it";
    case sc_shape_code: return "sc";
    case sl_shape_code: return "sl";
    default: return std::nullopt;
    }
}

auto CmdChr::token_noargfont_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case rm_family_code: return "rmfamily";
    case sf_family_code: return "sffamily";
    case tt_family_code: return "ttfamily";
    case md_series_code: return "mdseries";
    case bf_series_code: return "bfseries";
    case up_shape_code: return "upshape";
    case it_shape_code: return "itshape";
    case sl_shape_code: return "slshape";
    case sc_shape_code: return "scshape";
    case em_code: return "em";
    case normalfont_code: return "normalfont";
    default: return std::nullopt;
    }
}

auto CmdChr::token_trees_name() const -> std::optional<std::string_view> {
    switch (chr) {
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
    case barnodeconnect_code: return "barnodeconnect";
    case abarnodeconnect_code: return "abarnodeconnect";
    default: return std::nullopt;
    }
}

auto CmdChr::token_unimp_font_name() const -> std::optional<std::string_view> {
    switch (chr) {
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
    default: return std::nullopt;
    }
}

auto CmdChr::token_unimp_name() const -> std::optional<std::string_view> {
    switch (chr) {
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
    default: return std::nullopt;
    }
}

auto CmdChr::token_cst_name() const -> std::string {
    switch (chr) {
    case textasciicircum_code: return "textasciicircum";
    case guillemet_og_code: return "og";
    case guillemet_fg_code: return "fg";
    case i_code: return "i";
    case j_code: return "j";
    case dollar_code: return "$";
    case comma_code: return ",";
    case underscore_code: return "_";
    case amp_code: return "&";
    case lbrace_chr: return "{";
    case rbrace_chr: return "}";
    case percent_code: return "%";
    case space_code:
        return " ";
        //  case space_code: return  "\n";
        //  case space_code: return  "\r";
    case exclam_code: return "!";
    case quad_code: return "quad";
    case qquad_code: return "qquad";
    case dots_code: return "dots";
    case ldots_code: return "ldots";
    case msharp_code: return "sharp";
    case natural_code: return "natural";
    case flat_code: return "flat";
    case textasciigrave_code: return "textasciigrave";
    case textasciiacutex_code: return "textasciiacutex";
    case tdagger_code: return "dag";
    case tddagger_code: return "ddag";

    case sharp_code: return "#";
    case dash_code: return "-";
    case SS_code: return "SS";
    case numero_code: return "numero";
    case Numero_code: return "Numero";
    case ier_code: return "ier";
    case iere_code: return "iere";
    case iers_code: return "iers";
    case ieres_code: return "ieres";
    case ieme_code: return "ieme";
    case iemes_code: return "iemes";
    case textvisiblespace_code: return "textvisiblespace";
    case textnospace_code: return "textnospace";
    case LaTeX_code: return "LaTeX";
    case TeX_code: return "TeX";
    case ss_code: return "ss";
    case slash_code: return "slash";
    case textbackslash_code: return "textbackslash";
    case null_code: return "null";
    case thicklines_code: return "thicklines";
    case thinlines_code: return "thinlines";
    default: return make_name("other_constant_number", chr);
    }
}

auto CmdChr::l3_ifx_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3_p_NN_code: return "cs_if_eq_p:NN";
    case l3_TF_NN_code: return "cs_if_eq:NNTF";
    case l3_T_NN_code: return "cs_if_eq:NNT";
    case l3_F_NN_code: return "cs_if_eq:NNF";
    case l3_p_Nc_code: return "cs_if_eq_p:Nc";
    case l3_TF_Nc_code: return "cs_if_eq:NcTF";
    case l3_T_Nc_code: return "cs_if_eq:NcT";
    case l3_F_Nc_code: return "cs_if_eq:NcF";
    case l3_p_cN_code: return "cs_if_eq_p:cN";
    case l3_TF_cN_code: return "cs_if_eq:cNTF";
    case l3_T_cN_code: return "cs_if_eq:cNT";
    case l3_F_cN_code: return "cs_if_eq:cNF";
    case l3_p_cc_code: return "cs_if_eq_p:cc";
    case l3_TF_cc_code: return "cs_if_eq:ccTF";
    case l3_T_cc_code: return "cs_if_eq:ccT";
    case l3_F_cc_code: return "cs_if_eq:ccF";
    default: return std::nullopt;
    }
}

auto CmdChr::l3_expand_aux_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3expn_code: return "::n";
    case l3expN_code: return "::N";
    case l3expp_code: return "::p";
    case l3expc_code: return "::c";
    case l3expo_code: return "::o";
    case l3expf_code: return "::f";
    case l3expx_code: return "::x";
    case l3expV_code: return "::V";
    case l3expv_code: return "::v";
    case l3expfu_code: return "::f_unbraced";
    case l3expou_code: return "::o_unbraced";
    case l3expvu_code: return "::v_unbraced";
    case l3expVu_code: return "::V_unbraced";
    case l3expxu_code: return "::x_unbraced";
    default: return std::nullopt;
    }
}

auto CmdChr::l3_expand_base_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3exp_No_code: return "exp_args:No";
    case l3exp_NNo_code: return "exp_args:NNo";
    case l3exp_NNNo_code: return "exp_args:NNNo";
    case l3exp_NNc_code: return "exp_args:NNc";
    case l3exp_Ncc_code: return "exp_args:Ncc";
    case l3exp_Nccc_code: return "exp_args:Nccc";
    case l3exp_Nf_code: return "exp_args:Nf";
    case l3exp_NNf_code: return "exp_args:NNf";
    case l3exp_Nv_code: return "exp_args:Nv";
    case l3exp_NNv_code: return "exp_args:NNv";
    case l3exp_NV_code: return "exp_args:NV";
    case l3exp_NNV_code: return "exp_args:NNV";
    case l3exp_NNNV_code: return "exp_args:NNNV";
    case l3exp_Nco_code: return "exp_args:Nco";
    case l3exp_Ncf_code: return "exp_args:Ncf";
    case l3exp_NVV_code: return "exp_args:NVV";
    case l3exp_NcNc_code: return "exp_args:NcNc";
    case l3exp_Ncco_code: return "exp_args:Ncco";
    case l3exp_Nx_code: return "exp_args:Nx";
    case l3exp_Nnc_code: return "exp_args:Nnc";
    case l3exp_Nfo_code: return "exp_args:Nfo";
    case l3exp_Nff_code: return "exp_args:Nff";
    case l3exp_Nnf_code: return "exp_args:Nnf";
    case l3exp_Nno_code: return "exp_args:Nno";
    case l3exp_NnV_code: return "exp_args:NnV";
    case l3exp_Noo_code: return "exp_args:Noo";
    case l3exp_Nof_code: return "exp_args:Nof";
    case l3exp_Noc_code: return "exp_args:Noc";
    case l3exp_NNx_code: return "exp_args:NNx";
    case l3exp_Ncx_code: return "exp_args:Ncx";
    case l3exp_Nnx_code: return "exp_args:Nnx";
    case l3exp_Nox_code: return "exp_args:Nox";
    case l3exp_Nxo_code: return "exp_args:Nxo";
    case l3exp_Nxx_code: return "exp_args:Nxx";
    case l3exp_NNno_code: return "exp_args:NNno";
    case l3exp_NNoo_code: return "exp_args:NNoo";
    case l3exp_Nnnc_code: return "exp_args:Nnnc";
    case l3exp_Nnno_code: return "exp_args:Nnno";
    case l3exp_Nooo_code: return "exp_args:Nooo";
    case l3exp_NNnx_code: return "exp_args:NNnx";
    case l3exp_NNox_code: return "exp_args:NNox";
    case l3exp_Nnnx_code: return "exp_args:Nnnx";
    case l3exp_Nnox_code: return "exp_args:Nnox";
    case l3exp_Nccx_code: return "exp_args:Nccx";
    case l3exp_Ncnx_code: return "exp_args:Ncnx";
    case l3exp_Noox_code: return "exp_args:Noox";
    case l3exp_NVu_code: return "exp_last_unbraced:NV";
    case l3exp_Nvu_code: return "exp_last_unbraced:Nv";
    case l3exp_Nou_code: return "exp_last_unbraced:No";
    case l3exp_Nfu_code: return "exp_last_unbraced:Nf";
    case l3exp_Ncou_code: return "exp_last_unbraced:Nco";
    case l3exp_NcVu_code: return "exp_last_unbraced:NcV";
    case l3exp_NNVu_code: return "exp_last_unbraced:NNV";
    case l3exp_NNou_code: return "exp_last_unbraced:NNo";
    case l3exp_NNNVu_code: return "exp_last_unbraced:NNNV";
    case l3exp_NNNou_code: return "exp_last_unbraced:NNNo";
    case l3exp_Nnou_code: return "exp_last_unbraced:Nno";
    case l3exp_Noou_code: return "exp_last_unbraced:Noo";
    case l3exp_Nfou_code: return "exp_last_unbraced:Nfo";
    case l3exp_NnNou_code: return "exp_last_unbraced:NnNo";
    case l3exp_Nxu_code: return "exp_last_unbraced:Nx";
    case l3exp_Nouou_code: return "exp_last_two_unbraced:Noo";
    default: return std::nullopt;
    }
}

auto CmdChr::l3str_ifeq_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3_p_code: return "str_if_eq_p:nn";
    case l3_TF_code: return "str_if_eq:nnTF";
    case l3_T_code: return "str_if_eq:nnT";
    case l3_F_code: return "str_if_eq:nnF";
    case l3_p_code + 4: return "str_if_eq_x_p:nn";
    case l3_TF_code + 4: return "str_if_eq_x:nnTF";
    case l3_T_code + 4: return "str_if_eq_x:nnT";
    case l3_F_code + 4: return "str_if_eq_x:nnF";
    case l3_p_code + 8: return "str_if_eq_p:Vn";
    case l3_TF_code + 8: return "str_if_eq:VnTF";
    case l3_T_code + 8: return "str_if_eq:VnT";
    case l3_F_code + 8: return "str_if_eq:VnF";
    case l3_p_code + 12: return "str_if_eq_p:on";
    case l3_TF_code + 12: return "str_if_eq:onTF";
    case l3_T_code + 12: return "str_if_eq:onT";
    case l3_F_code + 12: return "str_if_eq:onF";
    case l3_p_code + 16: return "str_if_eq_p:no";
    case l3_TF_code + 16: return "str_if_eq:noTF";
    case l3_T_code + 16: return "str_if_eq:noT";
    case l3_F_code + 16: return "str_if_eq:noF";
    case l3_p_code + 20: return "str_if_eq_p:nV";
    case l3_TF_code + 20: return "str_if_eq:nVTF";
    case l3_T_code + 20: return "str_if_eq:nVT";
    case l3_F_code + 20: return "str_if_eq:nVF";
    case l3_p_code + 24: return "str_if_eq_p:VV";
    case l3_TF_code + 24: return "str_if_eq:VVTF";
    case l3_T_code + 24: return "str_if_eq:VVT";
    case l3_F_code + 24: return "str_if_eq:VVF";
    default: return std::nullopt;
    }
}

auto CmdChr::l3_set_cat_name() const -> std::optional<std::string_view> {
    switch (static_cast<symcodes>(chr)) {
    case escape_catcode: return "char_set_catcode_escape:n";
    case open_catcode: return "char_set_catcode_group_begin:n";
    case close_catcode: return "char_set_catcode_group_end:n";
    case dollar_catcode: return "char_set_catcode_math_toggle:n";
    case alignment_catcode: return "char_set_catcode_alignment:n";
    case eol_catcode: return "char_set_catcode_end_line:n";
    case parameter_catcode: return "char_set_catcode_parameter:n";
    case hat_catcode: return "char_set_catcode_math_superscript:n";
    case underscore_catcode: return "char_set_catcode_math_subscript:n";
    case ignored_catcode: return "char_set_catcode_ignore:n";
    case space_catcode: return "char_set_catcode_space:n";
    case letter_catcode: return "char_set_catcode_letter:n";
    case other_catcode: return "char_set_catcode_other:n";
    case active_catcode: return "char_set_catcode_active:n";
    case invalid_catcode: return "char_set_catcode_invalid:n";
    case comment_catcode: return "char_set_catcode_comment:n";
    case escape_catcode + 16: return "char_set_catcode_escape:N";
    case open_catcode + 16: return "char_set_catcode_group_begin:N";
    case close_catcode + 16: return "char_set_catcode_group_end:N";
    case dollar_catcode + 16: return "char_set_catcode_math_toggle:N";
    case alignment_catcode + 16: return "char_set_catcode_alignment:N";
    case eol_catcode + 16: return "char_set_catcode_end_line:N";
    case parameter_catcode + 16: return "char_set_catcode_parameter:N";
    case hat_catcode + 16: return "char_set_catcode_math_superscript:N";
    case underscore_catcode + 16: return "char_set_catcode_math_subscript:N";
    case ignored_catcode + 16: return "char_set_catcode_ignore:N";
    case space_catcode + 16: return "char_set_catcode_space:N";
    case letter_catcode + 16: return "char_set_catcode_letter:N";
    case other_catcode + 16: return "char_set_catcode_other:N";
    case active_catcode + 16: return "char_set_catcode_active:N";
    case invalid_catcode + 16: return "char_set_catcode_invalid:N";
    case comment_catcode + 16: return "char_set_catcode_comment:N";
    default: return std::nullopt;
    }
}

auto CmdChr::cat_ifeq_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case open_catcode * 4 + l3_p_code: return "token_if_group_begin_p:N";
    case open_catcode * 4 + l3_TF_code: return "token_if_group_begin:NTF";
    case open_catcode * 4 + l3_T_code: return "token_if_group_begin:NT";
    case open_catcode * 4 + l3_F_code: return "token_if_group_begin:NF";
    case close_catcode * 4 + l3_p_code: return "token_if_group_end_p:N";
    case close_catcode * 4 + l3_TF_code: return "token_if_group_end:NTF";
    case close_catcode * 4 + l3_T_code: return "token_if_group_end:NT";
    case close_catcode * 4 + l3_F_code: return "token_if_group_end:NF";
    case dollar_catcode * 4 + l3_p_code: return "token_if_math_toggle_p:N";
    case dollar_catcode * 4 + l3_TF_code: return "token_if_math_toggle:NTF";
    case dollar_catcode * 4 + l3_T_code: return "token_if_math_toggle:NT";
    case dollar_catcode * 4 + l3_F_code: return "token_if_math_toggle:NF";
    case alignment_catcode * 4 + l3_p_code: return "token_if_alignment_p:N";
    case alignment_catcode * 4 + l3_TF_code: return "token_if_alignment:NTF";
    case alignment_catcode * 4 + l3_T_code: return "token_if_alignment:NT";
    case alignment_catcode * 4 + l3_F_code: return "token_if_alignment:NF";
    case parameter_catcode * 4 + l3_p_code: return "token_if_parameter_p:N";
    case parameter_catcode * 4 + l3_TF_code: return "token_if_parameter:NTF";
    case parameter_catcode * 4 + l3_T_code: return "token_if_parameter:NT";
    case parameter_catcode * 4 + l3_F_code: return "token_if_parameter:NF";
    case hat_catcode * 4 + l3_p_code: return "token_if_math_superscript_p:N";
    case hat_catcode * 4 + l3_TF_code: return "token_if_math_superscript:NTF";
    case hat_catcode * 4 + l3_T_code: return "token_if_math_superscript:NT";
    case hat_catcode * 4 + l3_F_code: return "token_if_math_superscript:NF";
    case underscore_catcode * 4 + l3_p_code: return "token_if_math_subscript_p:N";
    case underscore_catcode * 4 + l3_TF_code: return "token_if_math_subscript:NTF";
    case underscore_catcode * 4 + l3_T_code: return "token_if_math_subscript:NT";
    case underscore_catcode * 4 + l3_F_code: return "token_if_math_subscript:NF";
    case space_catcode * 4 + l3_p_code: return "token_if_space_p:N";
    case space_catcode * 4 + l3_TF_code: return "token_if_space:NTF";
    case space_catcode * 4 + l3_T_code: return "token_if_space:NT";
    case space_catcode * 4 + l3_F_code: return "token_if_space:NF";
    case other_catcode * 4 + l3_p_code: return "token_if_other_p:N";
    case other_catcode * 4 + l3_TF_code: return "token_if_other:NTF";
    case other_catcode * 4 + l3_T_code: return "token_if_other:NT";
    case other_catcode * 4 + l3_F_code: return "token_if_other:NF";
    case letter_catcode * 4 + l3_p_code: return "token_if_letter_p:N";
    case letter_catcode * 4 + l3_TF_code: return "token_if_letter:NTF";
    case letter_catcode * 4 + l3_T_code: return "token_if_letter:NT";
    case letter_catcode * 4 + l3_F_code: return "token_if_letter:NF";
    case active_catcode * 4 + l3_p_code: return "token_if_active_p:N";
    case active_catcode * 4 + l3_TF_code: return "token_if_active:NTF";
    case active_catcode * 4 + l3_T_code: return "token_if_active:NT";
    case active_catcode * 4 + l3_F_code: return "token_active:NF";
    default: return std::nullopt;
    }
}

auto CmdChr::token_if_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case tok_eq_cat_code * 4 + l3_p_code: return "token_if_eq_catcode_p:NN";
    case tok_eq_char_code * 4 + l3_p_code: return "token_if_eq_charcode_p:NN";
    case tok_eq_meaning_code * 4 + l3_p_code: return "token_if_eq_meaning_p:NN";
    case tok_if_macro_code * 4 + l3_p_code: return "token_if_macro_p:N";
    case tok_if_cs_code * 4 + l3_p_code: return "token_if_cs_p:N";
    case tok_if_expandable_code * 4 + l3_p_code: return "token_if_expandable_p:N";
    case tok_if_long_code * 4 + l3_p_code: return "token_if_long_macro_p:N";
    case tok_if_prot_code * 4 + l3_p_code: return "token_if_protected_macro_p:N";
    case tok_if_longprot_code * 4 + l3_p_code: return "token_if_protected_long_macro_p:N";
    case tok_if_chardef_code * 4 + l3_p_code: return "token_if_chardef_p:N";
    case tok_if_mathchardef_code * 4 + l3_p_code: return "token_if_mathchardef_p:N";
    case tok_if_dim_code * 4 + l3_p_code: return "token_if_dim_register_p:N";
    case tok_if_int_code * 4 + l3_p_code: return "token_if_int_register_p:N";
    case tok_if_muskip_code * 4 + l3_p_code: return "token_if_muskip_register_p:N";
    case tok_if_skip_code * 4 + l3_p_code: return "token_if_skip_register_p:N";
    case tok_if_toks_code * 4 + l3_p_code: return "token_if_toks_register_p:N";
    case tok_if_primitive_code * 4 + l3_p_code: return "token_if_primitive_p:N";
    case tok_eq_cat_code * 4 + l3_TF_code: return "token_if_eq_catcode:NNTF";
    case tok_eq_char_code * 4 + l3_TF_code: return "token_if_eq_charcode:NNTF";
    case tok_eq_meaning_code * 4 + l3_TF_code: return "token_if_eq_meaning:NNTF";
    case tok_if_macro_code * 4 + l3_TF_code: return "token_if_macro:NTF";
    case tok_if_cs_code * 4 + l3_TF_code: return "token_if_cs:NTF";
    case tok_if_expandable_code * 4 + l3_TF_code: return "token_if_expandable:NTF";
    case tok_if_long_code * 4 + l3_TF_code: return "token_if_long_macro:NTF";
    case tok_if_prot_code * 4 + l3_TF_code: return "token_if_protected_macro:NTF";
    case tok_if_longprot_code * 4 + l3_TF_code: return "token_if_protected_long_macro:NTF";
    case tok_if_chardef_code * 4 + l3_TF_code: return "token_if_chardef:NTF";
    case tok_if_mathchardef_code * 4 + l3_TF_code: return "token_if_mathchardef:NTF";
    case tok_if_dim_code * 4 + l3_TF_code: return "token_if_dim_register:NTF";
    case tok_if_int_code * 4 + l3_TF_code: return "token_if_int_register:NTF";
    case tok_if_muskip_code * 4 + l3_TF_code: return "token_if_muskip_register:NTF";
    case tok_if_skip_code * 4 + l3_TF_code: return "token_if_skip_register:NTF";
    case tok_if_toks_code * 4 + l3_TF_code: return "token_if_toks_register:NTF";
    case tok_if_primitive_code * 4 + l3_TF_code: return "token_if_primitive:NTF";
    case tok_eq_cat_code * 4 + l3_T_code: return "token_if_eq_catcode:NNT";
    case tok_eq_char_code * 4 + l3_T_code: return "token_if_eq_charcode:NNT";
    case tok_eq_meaning_code * 4 + l3_T_code: return "token_if_eq_meaning:NNT";
    case tok_if_macro_code * 4 + l3_T_code: return "token_if_macro:NT";
    case tok_if_cs_code * 4 + l3_T_code: return "token_if_cs:NT";
    case tok_if_expandable_code * 4 + l3_T_code: return "token_if_expandable:NT";
    case tok_if_long_code * 4 + l3_T_code: return "token_if_long_macro:NT";
    case tok_if_prot_code * 4 + l3_T_code: return "token_if_protected_macro:NT";
    case tok_if_longprot_code * 4 + l3_T_code: return "token_if_protected_long_macro:NT";
    case tok_if_chardef_code * 4 + l3_T_code: return "token_if_chardef:NT";
    case tok_if_mathchardef_code * 4 + l3_T_code: return "token_if_mathchardef:NT";
    case tok_if_dim_code * 4 + l3_T_code: return "token_if_dim_register:NT";
    case tok_if_int_code * 4 + l3_T_code: return "token_if_int_register:NT";
    case tok_if_muskip_code * 4 + l3_T_code: return "token_if_muskip_register:NT";
    case tok_if_skip_code * 4 + l3_T_code: return "token_if_skip_register:NT";
    case tok_if_toks_code * 4 + l3_T_code: return "token_if_toks_register:NT";
    case tok_if_primitive_code * 4 + l3_T_code: return "token_if_primitive:NT";
    case tok_eq_cat_code * 4 + l3_F_code: return "token_if_eq_catcode:NNF";
    case tok_eq_char_code * 4 + l3_F_code: return "token_if_eq_charcode:NNF";
    case tok_eq_meaning_code * 4 + l3_F_code: return "token_if_eq_meaning:NNF";
    case tok_if_macro_code * 4 + l3_F_code: return "token_if_macro:NF";
    case tok_if_cs_code * 4 + l3_F_code: return "token_if_cs:NF";
    case tok_if_expandable_code * 4 + l3_F_code: return "token_if_expandable:NF";
    case tok_if_long_code * 4 + l3_F_code: return "token_if_long_macro:NF";
    case tok_if_prot_code * 4 + l3_F_code: return "token_if_protected_macro:NF";
    case tok_if_longprot_code * 4 + l3_F_code: return "token_if_protected_long_macro:NF";
    case tok_if_chardef_code * 4 + l3_F_code: return "token_if_chardef:NF";
    case tok_if_mathchardef_code * 4 + l3_F_code: return "token_if_mathchardef:NF";
    case tok_if_dim_code * 4 + l3_F_code: return "token_if_dim_register:NF";
    case tok_if_int_code * 4 + l3_F_code: return "token_if_int_register:NF";
    case tok_if_muskip_code * 4 + l3_F_code: return "token_if_muskip_register:NF";
    case tok_if_skip_code * 4 + l3_F_code: return "token_if_skip_register:NF";
    case tok_if_toks_code * 4 + l3_F_code: return "token_if_toks_register:NF";
    case tok_if_primitive_code * 4 + l3_F_code: return "token_if_primitive:NF";

    default: return std::nullopt;
    }
}

auto CmdChr::l3_set_num_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case setcat_code: return "char_set_catcode:nn";
    case thecat_code: return "char_value_catcode:n";
    case showcat_code: return "char_show_value_catcode:n";
    case setmath_code: return "char_set_mathcode:nn";
    case themath_code: return "char_value_mathcode:n";
    case showmath_code: return "char_show_value_mathcode:n";
    case setlc_code: return "char_set_lccode:nn";
    case thelc_code: return "char_value_lccode:n";
    case showlc_code: return "char_show_value_lccode:n";
    case setuc_code: return "char_set_uccode:nn";
    case theuc_code: return "char_value_uccode:n";
    case showuc_code: return "char_show_value_uccode:n";
    case setsf_code: return "char_set_sfcode:nn";
    case thesf_code: return "char_value_sfcode:n";
    case showsf_code: return "char_show_value_sfcode:n";
    default: return std::nullopt;
    }
}

auto CmdChr::l3str_case_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3_p_code: return "str_case_p:nn";
    case l3_TF_code: return "str_case:nnTF";
    case l3_T_code: return "str_case:nnT";
    case l3_F_code: return "str_case:nnF";
    case l3_p_code + 4: return "str_case_x_p:nn";
    case l3_TF_code + 4: return "str_case_x:nnTF";
    case l3_T_code + 4: return "str_case_x:nnT";
    case l3_F_code + 4: return "str_case_x:nnF";
    case l3_p_code + 8: return "str_case_p:on";
    case l3_TF_code + 8: return "str_case:onTF";
    case l3_T_code + 8: return "str_case:onT";
    case l3_F_code + 8: return "str_case:onF";
    default: return std::nullopt;
    }
}

auto CmdChr::l3_tl_basic_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3_tl_new_code: return "tl_new:N";
    case l3_tl_clear_code: return "tl_clear:N";
    case l3_tl_gclear_code: return "tl_gclear:N";
    case l3_tl_const_code: return "tl_const:Nn";
    case l3_tl_xconst_code: return "tl_const:Nx";
    case l3_tl_clearnew_code: return "tl_clear_new:N";
    case l3_tl_gclearnew_code: return "tl_gclear_new:N";
    case l3_tlx_new_code: return "tl_new:c";
    case l3_tlx_clear_code: return "tl_clear:c";
    case l3_tlx_gclear_code: return "tl_gclear:c";
    case l3_tlx_const_code: return "tl_const:cn";
    case l3_tlx_xconst_code: return "tl_const:cx";
    case l3_tlx_clearnew_code: return "tl_clear_new:c";
    case l3_tlx_gclearnew_code: return "tl_gclear_new:c";
    default: return std::nullopt;
    }
}

auto CmdChr::tl_concat_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case 0: return "tl_concat:NNN";
    case 1: return "tl_concat:ccc";
    case 2: return "tl_gconcat:NNN";
    case 3: return "tl_gconcat:ccc";
    default: return std::nullopt;
    }
}

auto CmdChr::tl_set_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3expn_code: return "tl_set:Nn";
    case l3expo_code: return "tl_set:No";
    case l3expf_code: return "tl_set:Nf";
    case l3expx_code: return "tl_set:Nx";
    case l3expV_code: return "tl_set:NV";
    case l3expv_code: return "tl_set:Nv";
    case l3expn_code + 9: return "tl_set:cn";
    case l3expo_code + 9: return "tl_set:co";
    case l3expf_code + 9: return "tl_set:cf";
    case l3expx_code + 9: return "tl_set:cx";
    case l3expV_code + 9: return "tl_set:cV";
    case l3expv_code + 9: return "tl_set:cv";
    case l3expn_code + 18: return "tl_gset:Nn";
    case l3expo_code + 18: return "tl_gset:No";
    case l3expf_code + 18: return "tl_gset:Nf";
    case l3expx_code + 18: return "tl_gset:Nx";
    case l3expV_code + 18: return "tl_gset:NV";
    case l3expv_code + 18: return "tl_gset:Nv";
    case l3expn_code + 9 + 18: return "tl_gset:cn";
    case l3expo_code + 9 + 18: return "tl_gset:co";
    case l3expf_code + 9 + 18: return "tl_gset:cf";
    case l3expx_code + 9 + 18: return "tl_gset:cx";
    case l3expV_code + 9 + 18: return "tl_gset:cV";
    case l3expv_code + 9 + 18: return "tl_gset:cv";
    default: return std::nullopt;
    }
}

auto CmdChr::tl_put_left_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case l3expn_code: return "tl_put_left:Nn";
    case l3expo_code: return "tl_put_left:No";
    case l3expx_code: return "tl_put_left:Nx";
    case l3expV_code: return "tl_put_left:NV";
    case l3expn_code + 9: return "tl_put_left:cn";
    case l3expo_code + 9: return "tl_put_left:co";
    case l3expx_code + 9: return "tl_put_left:cx";
    case l3expV_code + 9: return "tl_put_left:cV";
    case l3expn_code + 18: return "tl_gput_left:Nn";
    case l3expo_code + 18: return "tl_gput_left:No";
    case l3expx_code + 18: return "tl_gput_left:Nx";
    case l3expV_code + 18: return "tl_gput_left:NV";
    case l3expn_code + 9 + 18: return "tl_gput_left:cn";
    case l3expo_code + 9 + 18: return "tl_gput_left:co";
    case l3expx_code + 9 + 18: return "tl_gput_left:cx";
    case l3expV_code + 9 + 18: return "tl_gput_left:cV";
    case l3expn_code + 36: return "tl_put_right:Nn";
    case l3expo_code + 36: return "tl_put_right:No";
    case l3expx_code + 36: return "tl_put_right:Nx";
    case l3expV_code + 36: return "tl_put_right:NV";
    case l3expn_code + 9 + 36: return "tl_put_right:cn";
    case l3expo_code + 9 + 36: return "tl_put_right:co";
    case l3expx_code + 9 + 36: return "tl_put_right:cx";
    case l3expV_code + 9 + 36: return "tl_put_right:cV";
    case l3expn_code + 18 + 36: return "tl_gput_right:Nn";
    case l3expo_code + 18 + 36: return "tl_gput_right:No";
    case l3expx_code + 18 + 36: return "tl_gput_right:Nx";
    case l3expV_code + 18 + 36: return "tl_gput_right:NV";
    case l3expn_code + 9 + 18 + 36: return "tl_gput_right:cn";
    case l3expo_code + 9 + 18 + 36: return "tl_gput_right:co";
    case l3expx_code + 9 + 18 + 36: return "tl_gput_right:cx";
    case l3expV_code + 9 + 18 + 36: return "tl_gput_right:cV";
    default: return std::nullopt;
    }
}

auto CmdChr::l3_rescan_name() const -> std::optional<std::string_view> {
    switch (chr) {
    case 0: return "tl_set_rescan:Nnn";
    case 1: return "tl_set_rescan:Nno";
    case 2: return "tl_set_rescan:Nnx";
    case 3: return "tl_set_rescan:cnn";
    case 4: return "tl_set_rescan:cno";
    case 5: return "tl_set_rescan:cnx";
    case 6: return "tl_gset_rescan:Nnn";
    case 7: return "tl_gset_rescan:Nno";
    case 8: return "tl_gset_rescan:Nnx";
    case 9: return "tl_gset_rescan:cnn";
    case 10: return "tl_gset_rescan:cno";
    case 11: return "tl_gset_rescan:cnx";
    case 12: return "tl_gset_rescan:nn";
    default: return std::nullopt;
    }
}

auto CmdChr::special_name() const -> std::optional<std::string_view> {
    switch (cmd) {
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
    case 13: return "impossible (active character)";
    case 14: return "impossible (comment)";
    case 15: return "impossible (ignored)";
    default: return std::nullopt;
    }
}
