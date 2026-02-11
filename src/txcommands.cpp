#include "tralics/Bibtex.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/SaveAux.h"
#include "tralics/TitlePage.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

using namespace std::string_literals;

void show_unused_options();

// This file contains the big switch that interprets (most) commands

namespace {
    bool quote_started = false;
    Xml *glo_xml;
} // namespace

// Initialises the translator. The argument is the name of
// the document element.
void Parser::init_all(const std::string &doc_elt) {
    the_page_xml                     = new Xml(std::string("thepage"), nullptr);
    glo_xml                          = new Xml(the_names["glo"]);
    eqtb_int_table[endlinechar_code] = {'\r', 1};
    eqtb_int_table[newlinechar_code] = {'\n', 1};
    TL.clear();
    the_stack.init_all(doc_elt);
    the_stack.add_nl();
    onlypreamble.push_back(hash_table.let_token);
    onlypreamble.push_back(hash_table.atbd_token);
    onlypreamble.push_back(hash_table.notprerr_token);
}

// This assumes that the string is plain ASCII, no special XML characters
// Note: never use process_char('&'),
void Parser::process_string(std::string_view s) { unprocessed_xml.append(s); }

// This is useful for German unlaut. It translates two normal characters.
void Parser::translate_char(uchar c1, uchar c2) {
    translate_char(CmdChr(letter_catcode, subtypes(c1)));
    translate_char(CmdChr(letter_catcode, subtypes(c2)));
}

// This is used form German escape character, or maybe English or french quotes
//
void Parser::umlaut() {
    get_token();
    if (!cur_lang_german()) {
        if (!the_main.use_quotes) {
            umlaut_bad();
            return;
        }
        if (cur_cmd_chr.is_letter())
            quote_started = true;
        else if (quote_started)
            quote_started = false;
        else {
            umlaut_bad();
            return;
        }
        if (cur_tok.is_valid()) back_input();
        Token T = Token(other_t_offset, quote_started ? '`' : '\'');
        if (cur_lang_fr()) T = Token(other_t_offset, quote_started ? '<' : '>');
        back_input(T);
        back_input(T);
        return;
    }
    if ((!cur_cmd_chr.is_letter() && !cur_cmd_chr.is_other()) || cur_tok.char_val() > nb_characters) { //
        if (cur_tok.eqtb_loc() == '~') {                                                               // "~ -> dash , strange
            translate_char(CmdChr(uchar('-')));
            return;
        }
        umlaut_bad();
        return;
    }
    switch (cur_tok.char_val()) {
    case 'a': translate_char(CmdChr(uchar('\344'))); break;
    case 'o': translate_char(CmdChr(uchar('\366'))); break;
    case 'u': translate_char(CmdChr(uchar('\374'))); break;
    case 'e': translate_char(CmdChr(uchar('\353'))); break;
    case 'i': translate_char(CmdChr(uchar('\357'))); break;
    case 'A': translate_char(CmdChr(uchar('\304'))); break;
    case 'O': translate_char(CmdChr(uchar('\326'))); break;
    case 'U': translate_char(CmdChr(uchar('\334'))); break;
    case 'I': translate_char(CmdChr(uchar('\317'))); break;
    case 'E': translate_char(CmdChr(uchar('\313'))); break;
    case 's':
    case 'z': back_input(uclc_list[14]); return;
    case 'S': back_input(uclc_list[15]); return;
    case 'Z': translate_char('S', 'Z'); return;
    case 'c': translate_char('c', 'k'); return;
    case 'C': translate_char('C', 'K'); return;
    case 'f': translate_char('f', 'f'); return;
    case 'F': translate_char('F', 'F'); return;
    case 'l': translate_char('l', 'l'); return;
    case 'L': translate_char('L', 'L'); return;
    case 'm': translate_char('m', 'm'); return;
    case 'M': translate_char('M', 'M'); return;
    case 'n': translate_char('n', 'n'); return;
    case 'N': translate_char('N', 'N'); return;
    case 'p': translate_char('p', 'p'); return;
    case 'P': translate_char('P', 'P'); return;
    case 'r': translate_char('r', 'r'); return;
    case 'R': translate_char('R', 'R'); return;
    case 't': translate_char('t', 't'); return;
    case 'T': translate_char('T', 'T'); return;
    case '-':
    case '~': translate_char(CmdChr(uchar('-'))); return;
    case '|':
    case '"':
    case '=': return;
    case '`':
        LC();
        process_string("&#x0201E;");
        return;
    case '\'':
        LC();
        process_string("&#x0201D;");
        return;
    case '<': translate_char(CmdChr(uchar(0xAB))); return;
    case '>': translate_char(CmdChr(uchar(0xBB))); return;
    default: umlaut_bad();
    }
}

// Case of double-quote plus maybe some other character, result is
// a double quote
void Parser::umlaut_bad() {
    if (cur_tok.is_valid()) back_input();
    translate_char(CmdChr(letter_catcode, subtypes(uchar('"'))));
}

// This translates a character, that could be a not 8bit character.
// The result might be &#xFFF; or UTF8
void Parser::extended_chars(size_t c) {
    LC();
    if (c < 1 << 16)
        process_char(char32_t(c));
    else { unprocessed_xml.format("&#x{:X};", c); }
}

// Translate current character.
void Parser::translate_char(CmdChr X) {
    if (!the_stack.in_h_mode()) LC();
    char32_t c = X.char_val();
    if (c == 0) return; // we do not want null chars in trace or Xml
    if (tracing_commands()) spdlog::trace("{}", to_utf8(c));
    if (c >= 256) {
        process_char(c);
        return;
    }
    switch (c) {
    case '-': minus_sign(X); return;
    case '\'':
    case '`':
    case '<':
    case '>': english_quotes(X); return;
    case '"':
        if (X.is_letter() || the_parser.global_in_url || the_parser.global_in_load)
            process_char(c);
        else
            umlaut();
        return;
    case ';':
    case ':':
    case '!':
    case '?':
    case 0xBB: // <<
    case 0xAB: // >>
        french_punctuation(X);
        return;
    default: process_char(c);
    }
}

// This translates `'<>
// In some case ``, '', << and >> are translated as 0xAB and 0xBB
void Parser::english_quotes(CmdChr X) {
    auto c = X.char_val(); // Should be a small int
    if (the_parser.global_in_url || the_parser.global_in_load) {
        if (c == '<')
            process_string("&lt;");
        else if (c == '>')
            process_string("&gt;");
        else
            unprocessed_xml.push_back(static_cast<char>(c));
    } else {
        if (X.is_other() && (c == '\'' || c == '`' || cur_lang_fr())) {
            get_x_token();
            if (cur_cmd_chr.is_other() && cur_cmd_chr.char_val() == char32_t(c)) {
                if (c == '\'')
                    process_char(0x201d);
                else if (c == '`')
                    process_char(0x201c);
                else if (c == '<')
                    french_punctuation(CmdChr(uchar(0xAB)));
                else
                    french_punctuation(CmdChr(uchar(0xBB)));
                return;
            }
            if (cur_tok.is_valid()) back_input();
        }
        if (c == '<')
            process_string("&lt;");
        else if (c == '>')
            process_string("&gt;");
        else if (c == '\'' && X.is_other())
            process_char(the_main.rightquote_val);
        else if (c == '`' && X.is_other())
            process_char(the_main.leftquote_val);
        else
            unprocessed_xml.push_back(static_cast<char>(c));
    }
    if (X.is_letter() && !the_main.no_zerowidthspace) process_string(the_main.no_zerowidthelt ? "&#x200B;" : "<zws/>");
}

// This translates -, --, or ---.
void Parser::minus_sign(CmdChr X) {
    if (the_parser.global_in_url || the_parser.global_in_load)
        process_char('-');
    else if (X.is_letter()) {
        process_char('-');
        if (!the_main.no_zerowidthspace) process_string(the_main.no_zerowidthelt ? "&#x200B;" : "<zws/>");
    } else {
        get_x_token();
        if (cur_cmd_chr.is_minus_sign()) {
            get_x_token();
            if (cur_cmd_chr.is_minus_sign()) {
                process_char(0x2014);
            } else {
                if (cur_tok.is_valid()) back_input();
                process_char(0x2013);
            }
        } else {
            if (cur_tok.is_valid()) back_input();
            process_char('-');
        }
    }
}

// This handles :;!? 0xAB 0xBB. Especially in French.
void Parser::french_punctuation(CmdChr X) {
    auto c = X.char_val();
    if (the_parser.global_in_url || the_parser.global_in_load || X.is_letter() || !cur_lang_fr()) {
        extended_chars(c);
        return;
    }
    if (c == 0xAB) {
        for (;;) {
            get_x_token();
            if (cur_cmd_chr.is_space()) continue;
            if (cur_cmd_chr.cmd == cst_cmd && cur_cmd_chr.chr == nobreakspace_code) break;
            if (cur_cmd_chr.cmd == cst1_cmd && cur_cmd_chr.chr == comma_code) break;
            if (cur_tok.is_valid()) back_input();
            break;
        }
        extended_chars(c);
        process_char(0xA0);
        return;
    }
    unprocessed_xml.remove_last_space();
    process_char(0xA0);
    extended_chars(c);
}

void Parser::T_cst2(subtypes c) {
    LC();
    if (c == numero_code) process_string("n");
    if (c == Numero_code) process_string("N");
    flush_buffer();
    {
        Xml   *res = fonts1("sup");
        std::string_view s   = "o";
        if (c == ier_code)
            s = "er";
        else if (c == iers_code)
            s = "ers";
        else if (c == iere_code)
            s = "re";
        else if (c == ieres_code)
            s = "res";
        else if (c == ieme_code)
            s = "e";
        else if (c == iemes_code)
            s = "es";
        res->push_back_unless_nullptr(new Xml(std::string(s)));
        the_stack.add_last(res);
    }
    E_xspace();
}

void Parser::T_cst1(subtypes c) {
    LC();
    switch (c) {
    case underscore_code:
        process_char('_');
        return;
        ;
    case sharp_code: process_char('#'); return;
    case dash_code: return; // \- is: \discretionary{-}{}{}
    case slash_code:        // \slash is /\penalty \exhyphenpenalty
        process_char('/');
        return;
    case textbackslash_code: process_char('\\'); return;
    case null_code: return; // unused
    case LaTeX_code: process_string("<LaTeX/>"); return;
    case TeX_code:
        process_string("<TeX/>");
        return;
        ;
    case SS_code: process_string("SS"); return;
    case dots_code: // process_string("..."); return;
    // TODO: in the softcover fork, the next line becomes
    // case ldots_code: process_string("&#x85;"); return;
    case ldots_code: process_string("..."); return;
    case space_code: process_string(" "); return;
    case dollar_code: process_char('$'); return;
    case percent_code: process_char('%'); return;
    case exclam_code: return;
    case lbrace_chr: process_char('{'); return;
    case rbrace_chr:
        process_char('}');
        return;
        // case textasciigrave_code : process_char(0x2035); return;
        // case textasciicircum_code : process_char(0x2303); return;
        //  case textasciiacutey_code : process_char(0x2019); return;
    case textasciiacutex_code: process_char(0x2032); return;
    case textbullet_code: process_char(0x2022); return;
    case tdagger_code: process_char(0x2020); return;
    case tddagger_code:
        process_char(0x2021);
        return;
        // case textnospace_code : process_char(0x200B); return;
    case ss_code: process_char(0xDF); return;
    case i_code: process_char(0x131); return;
    case msharp_code: process_char(0x266F); return;
    case natural_code: process_char(0x266E); return;
    case flat_code: process_char(0x266D); return;
    case quad_code:
        process_char(0xA0);
        process_char(0xA0);
        process_char(0xA0);
        return;
    case qquad_code:
        process_char(0xA0);
        process_char(0xA0);
        process_char(0xA0);
        process_char(0xA0);
        process_char(0xA0);
        process_char(0xA0);
        return;
    case amp_code: process_string("&amp;"); return;
    case comma_code:
    case nbsp_code: process_char(0xA0); return;
    case j_code:
        process_char('j');
        return;
        // case textvisiblespace_code :  process_char(0x2423); return;
    case guillemet_og_code: // opening guillemet
        process_char(0xAB);
        process_char(0xA0);
        remove_initial_space_and_back_input();
        return;
    case guillemet_fg_code: // closing guillemet
        unprocessed_xml.remove_last_space();
        process_char(0xA0);
        process_char(0xBB);
        E_xspace();
        return;
        //  case emdash_code : process_char(0x2014); return;
        // case endash_code : process_char(0x2013); return;
    default: process_string("<Unknownelement/>"); return;
    }
}

// This translates \ding{N}, a reference to Zapf Dingbats
// In latex, this calls \char. Here we call scan_braced_int
void Parser::T_ding() {
    auto c = scan_braced_int(cur_tok);
    int  r = 0;
    switch (c) {
    case 33: r = 0x2701; break;
    case 34: r = 0x2702; break;
    case 35: r = 0x2703; break;
    case 36: r = 0x2704; break;
    case 37: r = 0x260E; break; // G
    case 38: r = 0x2706; break;
    case 39: r = 0x2707; break;
    case 40: r = 0x2708; break;
    case 41: r = 0x2709; break;
    case 42: r = 0x261B; break; // G
    case 43: r = 0x261E; break; // G
    case 44: r = 0x270C; break;
    case 45: r = 0x270D; break;
    case 46: r = 0x270E; break;
    case 47: r = 0x270F; break;
    case 48: r = 0x2710; break;
    case 49: r = 0x2711; break;
    case 50: r = 0x2712; break;
    case 51: r = 0x2713; break;
    case 52: r = 0x2714; break;
    case 53: r = 0x2715; break;
    case 54: r = 0x2716; break;
    case 55: r = 0x2717; break;
    case 56: r = 0x2718; break;
    case 57: r = 0x2719; break;
    case 58: r = 0x271A; break;
    case 59: r = 0x271B; break;
    case 60: r = 0x271C; break;
    case 61: r = 0x271D; break;
    case 62: r = 0x271E; break;
    case 63: r = 0x271F; break;
    case 64: r = 0x2720; break;
    case 65: r = 0x2721; break;
    case 66: r = 0x2722; break;
    case 67: r = 0x2723; break;
    case 68: r = 0x2724; break;
    case 69: r = 0x2725; break;
    case 70: r = 0x2726; break;
    case 71: r = 0x2727; break;
    case 72: r = 0x2605; break; // G
    case 73: r = 0x2729; break;
    case 74: r = 0x272A; break;
    case 75: r = 0x272B; break;
    case 76: r = 0x272C; break;
    case 77: r = 0x272D; break;
    case 78: r = 0x272E; break;
    case 79: r = 0x272F; break;
    case 80: r = 0x2730; break;
    case 81: r = 0x2731; break;
    case 82: r = 0x2732; break;
    case 83: r = 0x2733; break;
    case 84: r = 0x2734; break;
    case 85: r = 0x2735; break;
    case 86: r = 0x2736; break;
    case 87: r = 0x2737; break;
    case 88: r = 0x2738; break;
    case 89: r = 0x2739; break;
    case 90: r = 0x273A; break;
    case 91: r = 0x273B; break;
    case 92: r = 0x273C; break;
    case 93: r = 0x273D; break;
    case 94: r = 0x273E; break;
    case 95: r = 0x273F; break;
    case 96: r = 0x2740; break;
    case 97: r = 0x2741; break;
    case 98: r = 0x2742; break;
    case 99: r = 0x2743; break;
    case 100: r = 0x2744; break;
    case 101: r = 0x2745; break;
    case 102: r = 0x2746; break;
    case 103: r = 0x2747; break;
    case 104: r = 0x2748; break;
    case 105: r = 0x2749; break;
    case 106: r = 0x274A; break;
    case 107: r = 0x274B; break;
    case 108: r = 0x25CF; break; // G
    case 109: r = 0x274D; break;
    case 110: r = 0x25A0; break; // G
    case 111: r = 0x274F; break;
    case 112: r = 0x2750; break;
    case 113: r = 0x2751; break;
    case 114: r = 0x2752; break;
    case 115: r = 0x25B2; break; // g
    case 116: r = 0x25BC; break; // g
    case 117: r = 0x25C6; break; // g
    case 118: r = 0x2756; break;
    case 119: r = 0x25D7; break;
    case 120: r = 0x2758; break;
    case 121: r = 0x2759; break;
    case 122: r = 0x275A; break;
    case 123: r = 0x275B; break;
    case 124: r = 0x275C; break;
    case 125: r = 0x275D; break;
    case 126: r = 0x275E; break;
    case 161: r = 0x2761; break;
    case 162: r = 0x2762; break;
    case 163: r = 0x2763; break;
    case 164: r = 0x2764; break;
    case 165: r = 0x2765; break;
    case 166: r = 0x2766; break;
    case 167: r = 0x2767; break;
    case 168: r = 0x2663; break; // g
    case 169: r = 0x2666; break; //
    case 170: r = 0x2665; break; //
    case 171: r = 0x2660; break; //
    case 172: r = 0x2460; break;
    case 173: r = 0x2461; break;
    case 174: r = 0x2462; break;
    case 175: r = 0x2463; break;
    case 176: r = 0x2464; break;
    case 177: r = 0x2465; break;
    case 178: r = 0x2466; break;
    case 179: r = 0x2467; break;
    case 180: r = 0x2468; break;
    case 181: r = 0x2469; break;
    case 182: r = 0x2776; break;
    case 183: r = 0x2777; break;
    case 184: r = 0x2778; break;
    case 185: r = 0x2779; break;
    case 186: r = 0x277A; break;
    case 187: r = 0x277B; break;
    case 188: r = 0x277C; break;
    case 189: r = 0x277D; break;
    case 190: r = 0x277E; break;
    case 191: r = 0x277F; break;
    case 192: r = 0x2780; break;
    case 193: r = 0x2781; break;
    case 194: r = 0x2782; break;
    case 195: r = 0x2783; break;
    case 196: r = 0x2784; break;
    case 197: r = 0x2785; break;
    case 198: r = 0x2786; break;
    case 199: r = 0x2787; break;
    case 200: r = 0x2788; break;
    case 201: r = 0x2789; break;
    case 202: r = 0x278A; break;
    case 203: r = 0x278B; break;
    case 204: r = 0x278C; break;
    case 205: r = 0x278D; break;
    case 206: r = 0x278E; break;
    case 207: r = 0x278F; break;
    case 208: r = 0x2790; break;
    case 209: r = 0x2791; break;
    case 210: r = 0x2792; break;
    case 211: r = 0x2793; break;
    case 212: r = 0x2794; break;
    case 213: r = 0x2192; break;
    case 214: r = 0x2194; break;
    case 215: r = 0x2195; break;
    case 216: r = 0x2798; break;
    case 217: r = 0x2799; break;
    case 218: r = 0x279A; break;
    case 219: r = 0x279B; break;
    case 220: r = 0x279C; break;
    case 221: r = 0x279D; break;
    case 222: r = 0x279E; break;
    case 223: r = 0x279F; break;
    case 224: r = 0x27A0; break;
    case 225: r = 0x27A1; break;
    case 226: r = 0x27A2; break;
    case 227: r = 0x27A3; break;
    case 228: r = 0x27A4; break;
    case 229: r = 0x27A5; break;
    case 230: r = 0x27A6; break;
    case 231: r = 0x27A7; break;
    case 232: r = 0x27A8; break;
    case 233: r = 0x27A9; break;
    case 234: r = 0x27AA; break;
    case 235: r = 0x27AB; break;
    case 236: r = 0x27AC; break;
    case 237: r = 0x27AD; break;
    case 238: r = 0x27AE; break;
    case 239: r = 0x27AF; break;
    case 241: r = 0x27B1; break;
    case 242: r = 0x27B2; break;
    case 243: r = 0x27B3; break;
    case 244: r = 0x27B4; break;
    case 245: r = 0x27B5; break;
    case 246: r = 0x27B6; break;
    case 247: r = 0x27B7; break;
    case 248: r = 0x27B8; break;
    case 249: r = 0x27B9; break;
    case 250: r = 0x27BA; break;
    case 251: r = 0x27BB; break;
    case 252: r = 0x27BC; break;
    case 253: r = 0x27BD; break;
    case 254: r = 0x27BE; break;
    }
    LC();
    if (r == 0) {
        process_string("bad!");
        return;
    }
    process_char((r));
}

// \begin{minipage}
void Parser::T_minipage() {
    Token                      T = cur_tok;
    auto                       x = get_ctb_opt();
    std::optional<std::string> pos;
    if (x) pos = the_names[*x];
    ignore_optarg();
    auto                       y = get_ctb_opt();
    std::optional<std::string> ipos;
    if (y) ipos = the_names[*y];
    ignore_optarg(); // really needed ?
    scan_glue(it_dimen, T, false);
    std::string w = std::string(cur_val.get_dim_val());
    if (the_stack.in_v_mode()) leave_v_mode();
    the_stack.push1(the_names["minipage"]);
    the_stack.set_v_mode();
    state = state_S;
    the_stack.add_att_to_last(the_names["minipage_width"], w);
    if (pos) the_stack.add_att_to_last(the_names["pos"], *pos);
    if (ipos) the_stack.add_att_to_last(the_names["inner_pos"], *ipos);
    word_define(incentering_code, 0, false);
}

// \begin{xmlelement}
void Parser::T_xmlenv(subtypes c) {
    flush_buffer();
    std::string a = sT_arg_nopar();
    if (c == 0 && the_stack.in_v_mode()) {
        leave_v_mode();
    } else if (c == 1)
        leave_h_mode();
    else if (c == 2) {
        if (the_stack.in_v_mode()) leave_v_mode();
    }
    the_stack.push(the_names["ELT"], new Xml(std::string(a), nullptr));
    if (c == 2) the_stack.set_v_mode();
    remove_initial_space_and_back_input();
}

// \end{xmlelement}
void Parser::T_xmlenv_end(subtypes c) {
    flush_buffer();
    if (c == 1)
        leave_h_mode();
    else if (c == 2) {}
    the_stack.pop_if_frame(the_names["cst_p"]);
    the_stack.pop(the_names["ELT"]);
}

// \end{itemize},  \end{description}, \end{enumerate},
void Parser::T_listenv_end() {
    leave_h_mode();
    the_stack.pop_if_frame(the_names["item"]);
    the_stack.pop(the_names["list"]);
    the_stack.add_nl();
}

void Parser::T_atdocument(subtypes c) {
    TokenList L = read_arg();
    if (c == 0)
        document_hook.splice(document_hook.end(), L);
    else
        end_document_hook.splice(end_document_hook.end(), L);
}

// Translates `\begin{glossaire}`
void Parser::T_glossaire() {
    leave_h_mode();
    the_stack.push1(the_names["gloss_type"], the_names["list"]);
    the_stack.add_att_to_last(the_names["type"], the_names["gloss_type"]);
    the_stack.add_last(new Xml(the_names["head"], glo_xml));
    the_stack.set_no_mode();
}

// Translates `\end{glossaire}`
void Parser::T_glossaire_end() {
    auto n = the_stack.top_stack()->size();
    the_stack.pop(the_names["gloss_type"]);
    if (n == 1) parse_error("empty glossaire");
}

// case \begin{figure}\begin{table}
// c=2 is wrapfigure
bool Parser::T_figure_table(symcodes x, subtypes c) {
    auto                       opt = nT_optarg_nopar();
    std::optional<std::string> overhang;
    std::string                place, width;
    if (c == 2) {
        place    = nT_arg_nopar();
        overhang = nT_optarg_nopar();
        width    = nT_arg_nopar();
    }
    word_define(incentering_code, 1, false);
    leave_h_mode();
    the_stack.push1(the_names[x == figure_cmd ? "figure_env" : "table_env"]);
    if (c == 2) {
        if (opt && !opt->empty()) the_stack.add_att_to_last(std::string("narrow"), *opt);
        the_stack.add_att_to_last(the_names["place"], place);
        if (overhang && !overhang->empty()) the_stack.add_att_to_last(std::string("overhang"), *overhang);
        the_stack.add_att_to_last(the_names["width"], width);
    } else {
        the_stack.add_att_to_last(the_names["rend"], the_names[x == figure_cmd ? "figure" : "table"]);
        if (opt && !opt->empty()) the_stack.add_att_to_last(the_names["place"], *opt);
        if (c == 1) the_stack.add_att_to_last(the_names["starred"], the_names["true"]);
    }
    if (!refstepcounter(x == figure_cmd ? "figure" : "table", true)) return false;
    the_stack.set_v_mode();
    return true;
}

// Translates \enf{figure} or \end{table}
void Parser::T_figure_table_end(bool is_fig) {
    std::string name = the_names[is_fig ? "figure_env" : "table_env"];
    leave_h_mode();
    Xml *aux = the_stack.top_stack();
    if (!aux->has_name(name))
        parse_error("no figure/table on stack");
    else if (!the_main.nofloat_hack)
        aux->postprocess_fig_table(is_fig);
    the_stack.pop(name);
    the_stack.add_nl();
}

// case \end{document}
void Parser::T_enddocument(subtypes c) {
    if (c == 0) {
        E_input(end_all_input_code);
        kill_line();
        hash_table.eval_let("AtEndDocument", "@firstofone");
        back_input(hash_table.real_end_token);
        back_input(end_document_hook);
        if (tracing_commands()) spdlog::trace("atenddocumenthook: {}", fmt::streamed(TL));
    } else {
        flush_buffer();
        the_stack.end_module();
        the_parser.seen_enddocument = true;
    }
}

// case \begin{document}
auto Parser::T_begindocument() -> bool {
    if (the_parser.seen_document) {
        parse_error("Two environments named document");
        return true;
    }
    if (cur_level != 2) parse_error("\\begin{document} not at level 0");
    the_parser.seen_document = true;
    if (!Titlepage.is_valid()) add_language_att();
    cur_tok.kill();
    if (!pop_level(bt_env)) return false;
    cur_level = 1; // this is the outer level...
    if (the_main.dverbose) M_tracingall();
    if (tracing_commands()) { spdlog::trace("+stack: level set to 1"); }
    the_bibtex.bootagain();
    hash_table.eval_let("AtBeginDocument", "@firstofone");
    {
        TokenList L = tokenize_buffer("\\let\\do\\noexpand\\ignorespaces\n", "(AtBeginDocument hook)");
        back_input(L);
    }
    back_input(onlypreamble);
    back_input(document_hook);
    show_unused_options();
    if (tracing_commands()) spdlog::trace("atbegindocumenthook= {}", fmt::streamed(TL));
    return true;
}

// case \begin \end
[[nodiscard]] auto Parser::T_beginend(symcodes x) -> bool {
    flush_buffer();
    bool begin = x == begin_cmd;
    auto S     = fetch_name0();
    if (tracing_commands()) { spdlog::trace("{{\\{} {}}}", (begin ? "begin" : "end"), S); }
    return begin ? T_begin(S) : T_end(S);
}

// Case of \@setmode=12
void Parser::T_setmode() {
    scan_optional_equals();
    auto n = scan_int(cur_tok);
    if (!(n >= 0 && n <= 6)) n = 0;
    the_stack.set_mode(mode(n));
}

// Translates \aftergroup\token
void Parser::T_aftergroup() {
    get_token();
    SaveAuxBase *y = new SaveAuxAftergroup(cur_tok);
    push_save_stack(y);
}

// Translates \hskip1em, \vskip1em or \mskip3mu
void Parser::T_scan_glue(subtypes c) {
    Token T = cur_tok;
    if (c == mskip_code) {
        scan_glue(it_mu, T);
        // ignore value...
        return;
    }
    scan_glue(it_glue, T);
    // not perfect...
    append_glue(T, cur_val.get_glue_width(), c == vskip_code);
}

// Translates \bauthors{...} or \beditors{...}
auto Parser::T_bauteursediteurs(subtypes c) -> bool {
    mode m = the_stack.get_mode();
    need_bib_mode();
    flush_buffer();
    if (!T_arg1(the_names[c == 0 ? "bauteurs"s : "bediteur"s])) return false;
    the_stack.set_mode(m);
    the_stack.add_nl();
    return true;
}

// Translates \unhbox, \unhcopy \unvbox \unvcopy
void Parser::T_un_box(subtypes c) {
    auto i = scan_reg_num();
    if (c == unhbox_code) leave_v_mode();
    Xml *cur_box = box_table[i].val;
    the_stack.unbox(cur_box);
    if (c == unhbox_code || c == unvbox_code) box_table[i].val = nullptr;
}
