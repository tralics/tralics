#include "tralics/XmlIO.h"
#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txinline.h"

namespace {
    inline auto is_spacer(char32_t c) -> bool { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
} // namespace

// Rule 1 defines Document
// Rule 2 defines Char
// rule [3] says that S is a non-empty sequence

// Rule 4 says that Namechar can be a lot of things...
// Rule 5 says that a Name starts with Letter, underscore colon,
//    and is followed by some NameChar
// Rule 7 says that Nmtoken is a sequence of Namechar
// Rules 6,8 define Names Nmtokens

// Rule 9, 10 say that EntityValue and AttValue are quoted things
// like 'foo' , 'foo"bar', "foo", no no les than sign
// Ampersand can appear in the form &foo;, &#12; &#x12; and

// Rule 11 says that SystemLiteral is any quoted string
// Rule 12,13 says that Pubidliteral is a quoted string, containing
// ascii letters, digits, space newline -'()+,.?;=?;!*#@$_%
// Note : no tab, no doule quote, no ampersand

// Rule 14 defines CharData, s string without <, &, ]]>
// Rule 15 defines a comment <-- -- -->
// Rules 16, 17 defineds <?foo ?>
// Rules 18-21 define <!CDATA[ ..... ]]>
// Rules 22-27 define the prologue; XMLDecl? doctypedecl? and junk
// where junk is PI comment or space
// xmldecl is <?xml version='1.0' encoding SDDdecl ?>

// Rule 28 says <!DOCTYPE name ExternalID [...]>
// where brackets are optiona, contain elementdecl AttlistDecl Entitydecl
// NotationDecL PI Comment PEReference
// Rule 30-31 defines an external subset
// Rule 32 says SDdecl is standalone='yes'

// Rules 33-38 defines LanguageId
// Rules 39-44 describe element
// Rules 45-51 define <!ELEMENT>

// Note <!ELEMENT foo spec>
// where spec is EMPTY or ANY or (#PCDATA | foo | foo)*
// or ( X | Y | Z)?*+ or  ( X , Y , Z)?*+, mod is optional,
// where foo is element, X, Z is name4 choice or seq

// The external function. Needs to be completed
auto read_xml(const std::string &s) -> Xml * {
    auto of = tralics_ns::find_in_path(s);
    if (!of) {
        the_parser.parse_error(the_parser.err_tok, "Unable to read the XML input file", s, "noinput");
        return nullptr;
    }
    XmlIO res;
    if (res.init(*of)) return nullptr;
    return res.prun();
}

auto XmlIO::prun() -> Xml * {
    try {
        run();
    } catch (EndOfData cc) {};
    return cur_xml;
}

// This reads the XML file, without conversion
auto XmlIO::init(const std::string &name) -> bool {
    for (auto &i : Type) i = xt_invalid;
    for (size_t i = '0'; i <= '9'; i++) Type[i] = xt_digit;
    for (size_t i = 'a'; i <= 'z'; i++) Type[i] = xt_letter;
    for (size_t i = 'A'; i <= 'Z'; i++) Type[i] = xt_letter;
    Type[uchar(' ')]  = xt_space;
    Type[uchar('\r')] = xt_space;
    Type[uchar('\n')] = xt_space;
    Type[uchar('\t')] = xt_space;
    Type[uchar('_')]  = xt_ok_first;
    Type[uchar(':')]  = xt_ok_first;
    Type[uchar('$')]  = xt_other;
    Type[uchar('-')]  = xt_other;
    lines.read(name, 3);
    encoding       = 0;
    input_line_pos = cur_line_len = 0;
    nb_cond                       = 0;
    eof_ok                        = false;
    if (lines.empty()) {
        cur_line = 0;
        error("Unable to read the XML input file");
        return true;
    }
    return false;
}

// This parses the whole XML file
void XmlIO::run() {
    cur_xml = new Xml(std::string("root"), nullptr);
    cur_stack.push_back(cur_xml);
    B.clear();
    eof_ok = true;
    for (;;) {
        char32_t c = next_char();
        // We should handle &foo; as well, but we cannot change it
        if (c != '<')
            B.push_back(c);
        else {
            eof_ok = false;
            parse_lt();
            eof_ok = true;
        }
    }
}

// This fetches a line from the file, true at eof
void XmlIO::next_line() {
    line_buffer.clear();
    int n = lines.get_next_cv(line_buffer, encoding);
    if (n == -1) {
        if (eof_ok && cur_stack.size() == 1)
            ;
        else
            error("Unexpected EOF");
        throw EndOfData();
    }
    cur_line       = n;
    input_line     = codepoints(line_buffer);
    input_line_pos = 0;
    cur_line_len   = input_line.size();
}

// Characters can come from back of readlist, or head of input_line
// This leaves the character where it is
auto XmlIO::peek_char() -> char32_t {
    if (!reread_list.empty()) return reread_list.back();
    if (at_eol()) next_line();
    return input_line[input_line_pos];
}

// This removes the character, assuming peek-char has been called
void XmlIO::skip_char() {
    if (!reread_list.empty())
        reread_list.pop_back();
    else
        input_line_pos++;
}

// This returns the next character
auto XmlIO::next_char() -> char32_t {
    char32_t res = peek_char();
    skip_char();
    return res;
}

// Skip over spaces; unread char is in cur_char
void XmlIO::skip_space() {
    for (;;) {
        cur_char = peek_char();
        if (!is_spacer(cur_char)) return;
        skip_char();
    }
}

// Puts all characters from B into the current element
void XmlIO::flush_buffer() {
    if (B.empty()) return;
    cur_xml->add_last_string(B);
    B.clear();
}

// Scans a Name with a simplified syntax
void XmlIO::scan_name() {
    B.clear();
    char32_t x = peek_char();
    // x should be Letter underscore colon
    B.push_back(x);
    skip_char();
    for (;;) {
        char32_t xx = peek_char();
        if (is_ascii(xx)) {
            x_type w = Type[xx];
            if (w == xt_space || w == xt_invalid) return;
        }
        skip_char();
        B.push_back(xx);
    }
}

// Reads all characters, until finding either character c, or a space if c=0
// Maybe we should change this into: parse a Nntoken or something like that
void XmlIO::scan_name(uchar c) {
    B.clear();
    for (;;) {
        char32_t x = peek_char();
        if (x == c || is_spacer(x)) return;
        skip_char();
        B.push_back(x);
    }
}

// Reads all characters, until finding either character c, or a space if c=0
void XmlIO::scan_name(uchar c1, uchar c2) {
    B.clear();
    for (;;) {
        char32_t x = peek_char();
        if (x == c1 || x == c2 || is_spacer(x)) return;
        skip_char();
        B.push_back(x);
    }
}

// We may have <foo> </foo>, <?foo?> or <!foo>
// In any case, buffer B is flushed at the start, reset at the end
void XmlIO::parse_lt() {
    flush_buffer();
    cur_char = next_char();
    if (cur_char == '/')
        parse_end();
    else if (cur_char == '!')
        parse_dec();
    else if (cur_char == '?')
        parse_pi();
    else
        parse_tag();
    B.clear();
}

// Case of </foo>
// We finish reading the element, and add some checks
void XmlIO::parse_end() {
    scan_name();
    skip_space();
    auto ref = std::string(B);
    if (!cur_xml->has_name(ref)) {
        error("Bad end tag");
        std::cout << "Got " << ref << ", Expected " << cur_xml->name << "\n";
    }
    pop_this();
}

// Pop the XML stack
void XmlIO::pop_this() {
    if (cur_char != '>') {
        error("Expected > at end of element");
        std::cout << " got " << to_utf8(cur_char) << "\n";
    }
    skip_char();
    if (cur_stack.size() < 2)
        error("Spurious end tag");
    else {
        cur_stack.pop_back();
        cur_xml = cur_stack.back();
    }
}

// Reads the start of a tag
void XmlIO::parse_tag() {
    reread_list.push_back(cur_char);
    scan_name();
    Xml *res = new Xml(std::string(B), nullptr);
    cur_xml->push_back_unless_nullptr(res);
    cur_xml = res;
    cur_stack.push_back(cur_xml);
    parse_attributes();
}

// reads the equals sign and the value that follows
void XmlIO::parse_att_val() {
    skip_space();
    if (cur_char != '=') {
        error("Equals sign expected");
        std::cout << " After {}";
        std::cout << " got " << to_utf8(cur_char) << "\n";
    }
    skip_char();
    parse_quoted();
}

void XmlIO::parse_quoted() {
    skip_space();
    char32_t delim{' '};
    if (cur_char == '\'' || cur_char == '"')
        delim = cur_char;
    else {
        error("Quote expected");
        std::cout << " got " << to_utf8(cur_char) << "\n";
    }
    skip_char();
    B.clear();
    for (;;) {
        char32_t x = next_char();
        if (x == delim) break;
        B.push_back(x);
    }
}

// Parses the attributes of the current element
void XmlIO::parse_attributes() {
    for (;;) {
        skip_space();
        // handle the case <foo/>
        if (cur_char == '/') {
            skip_char();
            cur_char = peek_char();
            pop_this();
            return;
        }
        // case of <foo>
        if (cur_char == '>') {
            skip_char(); // no need to pop
            return;
        }
        // Now we have an attribute pair
        scan_name();
        auto att_name = std::string(B);
        parse_att_val();
        auto att_val = std::string(B);
        cur_xml->id.add_attribute(att_name, att_val);
    }
}

// Case of process instruction, say <?xml?>
// The PI is ignored
void XmlIO::parse_pi() {
    scan_name('?');
    bool is_tralics = B == "tralics";
    bool is_xml     = B == "xml";
    aux             = B;
    if (is_xml) {
        for (;;) {
            skip_space();
            if (cur_char == '?') {
                skip_char();
                cur_char = next_char();
                if (cur_char != '>') {
                    error("Greater than sign expected");
                    std::cout << " got " << to_utf8(cur_char) << "\n";
                }
                break;
            }
            scan_name('=');
            bool is_encoding = B == "encoding";
            aux.format(" {}='", B);
            parse_att_val();
            aux.format("{}'", B);
            if (is_encoding) {
                if (B == "iso-8859-1")
                    encoding = 1;
                else if (B == "UTF-8")
                    encoding = 0;
                else
                    error("Unrecognised encoding");
            }
        }
    } else
        for (;;) {
            char32_t c = next_char();
            if (c == '?') {
                cur_char = peek_char();
                if (cur_char == '>') {
                    skip_char();
                    break;
                }
            }
            B << c;
        }
    Xml *res = new Xml(aux);
    res->id  = size_t(-3); // mark this as a pi
    cur_xml->push_back_unless_nullptr(res);
    if (is_tralics) std::cout << "Unrecognised PI " << B << "\n";
}

// Scans a declaration. We test the first or second letter
void XmlIO::parse_dec() {
    cur_char   = next_char();
    char32_t c = peek_char();
    reread_list.push_back(cur_char);
    if (c == '-')
        parse_dec_comment();
    else if (c == 'N')
        parse_dec_entity();
    else if (c == 'L')
        parse_dec_element();
    else if (cur_char == 'A')
        parse_dec_attlist();
    else if (cur_char == 'D')
        parse_dec_doctype();
    else if (c == 'C')
        parse_dec_cdata();
    else if (cur_char == '[')
        parse_dec_conditional();
    else
        parse_dec_notation();
}

// Note that first character is in cur_char
void XmlIO::expect(String s) {
    auto n = strlen(s);
    for (size_t i = 0; i < n; i++) {
        cur_char = next_char();
        if (cur_char != uchar(s[i])) {
            Buffer b = fmt::format("Expected {} got ", s[i]);
            b << cur_char; // \todo make codepoint formattable
            error(b);
        }
    }
}

// Comments are of the form <!-- comment -->
// Three hyphens in a row are illegal
// Comments are ignored.
void XmlIO::parse_dec_comment() {
    expect("--");
    B.clear();
    for (;;) {
        char32_t c = next_char();
        B.push_back(c);
        if (c != '>') continue;
        auto k = B.size(); // B[k-1] is >
        if (k >= 3 && B[k - 2] == '-' && B[k - 3] == '-') break;
    }
    B.remove_last(3);
    Xml *res = new Xml(B);
    res->id  = size_t(-1); // mark this as a comment
    cur_xml->push_back_unless_nullptr(res);
}

// CDATA section; is inserted in the tree as is
void XmlIO::parse_dec_cdata() {
    expect("[CDATA");
    B.clear();
    B.append("<![CDATA ");
    skip_space();
    for (;;) {
        char32_t c = next_char();
        B.push_back(c);
        if (c == '>') {
            auto k = B.size(); // B[k-1] is >
            if (B[k - 2] == ']' && B[k - 3] == ']') break;
        }
    }
    flush_buffer();
}

// to be completed
void XmlIO::parse_dec_conditional() {
    B.clear();
    skip_char(); // skip
    B.append("<![");
    bool keep = false;
    skip_space();
    char32_t c = peek_char();
    if (c == '%') {
        skip_char();
        expand_PEReference();
    }
    c = next_char();
    if (c != 'I') error("expected INCLUDE or IGNORE");
    c = peek_char();
    if (c == 'N') {
        expect("NCLUDE");
        keep = true;
    } else if (c == 'G')
        expect("GNORE");
    else
        error("expected INCLUDE or IGNORE");
    skip_space();
    if (cur_char != '[') error("expected [");
    skip_char();
    B.clear();
    if (keep)
        B += "[INCLUDE[";
    else
        B += "[IGNORE[";
    Xml *res = new Xml(std::string(B));
    res->id  = size_t(-2); // mark this as a declaration
    cur_xml->push_back_unless_nullptr(res);
    cur_xml = res;
    cur_stack.push_back(cur_xml);
    ++nb_cond;
    B.clear();
}

// Parses SYSTEM 'foo' or PUBLIC 'foo' 'bar'
auto XmlIO::parse_sys_pub() -> bool {
    if (cur_char == 'P') {
        expect("PUBLIC");
        aux.append(" PUBLIC '");
        parse_quoted();
        aux += B + "' '";
        parse_quoted();
        aux += B + "'";
        return true;
    }
    if (cur_char == 'S') {
        expect("SYSTEM");
        parse_quoted();
        aux += " SYSTEM '" + B + "'";
        return true;
    }
    return false;
}

// We scan one of
// <!ENTITY foo PUBLIC 'pub-part' 'system-part'>
// <!ENTITY foo SYSTEM 'system-part'>
// We insert
void XmlIO::parse_dec_entity() {
    aux.clear();
    aux.append("ENTITY ");
    expect("ENTITY");
    skip_space();
    bool parameter = false;
    if (cur_char == '%') {
        skip_char();
        skip_space();
        aux.append("% ");
        parameter = true;
    }
    scan_name(0);
    std::string name = B;
    aux.append(name);
    skip_space();
    // entity value is defined by a name (system or public) or value
    if (!parse_sys_pub()) {
        parse_quoted();
        aux += " '" + B + "'";
        if (parameter) entities.push_back({name, B});
    }
    skip_space();
    // We might have NDATA here, if parameter==false
    if (cur_char != '>') {
        B.clear();
        for (;;) {
            char32_t c = next_char();
            if (c == '>') break;
            B.push_back(c);
        }
        aux += " " + B;
    } else
        skip_char();
    Xml *res = new Xml(aux);
    res->id  = size_t(-2); // mark this as a declaration
    cur_xml->push_back_unless_nullptr(res);
}

// We might have <!ELEMENT %foo; %bar;>
void XmlIO::parse_dec_element() {
    Buffer tmp;
    aux.clear();
    aux.clear();
    tmp.append("ELEMENT");
    expect("ELEMENT");
    bool        ok            = true;
    bool        is_first      = true;
    bool        prev_is_space = false;
    bool        first_space   = true;
    std::string elt_name;
    for (;;) {
        char32_t c = next_char();
        if (c == '>') break;
        if (c == '%') {
            if (expand_PEReference()) continue;
            ok = false;
        }
        if (is_spacer(c)) {
            if (is_first || prev_is_space) continue;
            if (first_space) {
                elt_name = aux;
                aux.clear();
                first_space = false;
                is_first    = true;
                continue;
            }
            aux.push_back(c);
            prev_is_space = true;
            continue;
        }
        aux.push_back(c);
        is_first      = false;
        prev_is_space = false;
    }
    if (!ok) the_log << "Possible problem in XML scan on line " << the_parser.get_cur_line() << "\n";
    if (prev_is_space) aux.pop_back();
    tmp += " " + elt_name;
    if (aux == "EMPTY") {
    } else if (aux == "ANY") {
    } else
        tmp += " ";
    tmp += " " + aux;
    Xml *res = new Xml(tmp);
    res->id  = size_t(-2); // mark this as a declaration
    cur_xml->push_back_unless_nullptr(res);
}

void XmlIO::parse_dec_attlist() {
    expect("ATTLIST");
    Buffer tmp;
    aux.clear();
    aux.clear();
    tmp.append("ATTLIST");
    bool        ok            = true;
    bool        is_first      = true;
    bool        prev_is_space = false;
    bool        first_space   = true;
    std::string elt_name;
    for (;;) {
        char32_t c = next_char();
        if (c == '>') break;
        if (c == '%') {
            if (expand_PEReference()) continue;
            ok = false;
        }
        if (is_spacer(c)) {
            if (is_first || prev_is_space) continue;
            if (first_space) {
                elt_name = aux;
                aux.clear();
                first_space = false;
                is_first    = true;
                continue;
            }
            aux.push_back(c);
            prev_is_space = true;
            continue;
        }
        aux.push_back(c);
        is_first      = false;
        prev_is_space = false;
    }
    if (!ok) the_log << "Possible problem in XML scan on line " << the_parser.get_cur_line() << "\n";
    if (prev_is_space) aux.pop_back();
    tmp += " " + elt_name;
    // Syntax Attdef*
    // AttDef ::= A Name S AttType S DefaultDecl
    // AttType ::= StringType | TokenizedTtype | EnumeratedType
    // StringType ::= 'CDATA'
    // TokenizedType ::= ID IDREF IDREFS ENTITY ENTITIES NMTOKEN NMTOKENS
    // EnumeratedType ::= NotationType | Enumeration
    // NotationType ::= NOTATION ( name-choice )
    // Enumeration ::= ( Nmtoken-choice )
    // DefaultDecl = #REQUIRED | #IMPLIED | (#FIXED ? AttValue)

    tmp += " " + aux;
    Xml *res = new Xml(tmp);
    res->id  = size_t(-2); // mark this as a declaration
    cur_xml->push_back_unless_nullptr(res);
}

void XmlIO::parse_dec_doctype() {
    expect("DOCTYPE");
    Xml *res = new Xml(std::string("DOCTYPE"), nullptr);
    res->id  = size_t(-2);                  // mark this as a declaration
    cur_xml->push_back_unless_nullptr(res); // Insert this in the tree
    skip_space();
    scan_name(0);
    aux = " " + B;
    skip_space();
    parse_sys_pub();
    res->add_last_string(aux);
    skip_space();
    skip_char();
    if (cur_char != '[') {
        if (cur_char != '>') error("Expected [ or > at end of doctype");
        return;
    }
    // push the element on the stack
    cur_xml = res;
    cur_stack.push_back(cur_xml);
    // Read the local DTD
    B = " [";
    for (;;) {
        char32_t c = next_char();
        if (c == '<')
            parse_lt();
        else {
            B.push_back(c);
            if (c == ']') {
                if (nb_cond != 0) {
                    c = next_char();
                    if (c != ']') error("Expected ]]>");
                    B.append("]");
                    flush_buffer();
                    cur_char = peek_char();
                    pop_this();
                    nb_cond--;
                    continue;
                }
                break;
            }
        }
    }
    flush_buffer();
    skip_space();
    pop_this();
}

void XmlIO::parse_dec_notation() {
    expect("NOTATION");
    B.clear();
    B.append("<!NOTATION ");
    skip_space();
    for (;;) {
        char32_t c = next_char();
        B.push_back(c);
        if (c == '>') break;
    }
    flush_buffer();
}

// Replaces %foo by its value
auto XmlIO::expand_PEReference() -> bool {
    Buffer buf;
    for (;;) {
        char32_t c = next_char();
        if (c == ';') break;
        buf.push_back(c);
    }
    std::string s  = buf;
    bool        ok = false;
    auto        n  = entities.size();
    for (size_t i = 0; i < n; i++) {
        if (entities[i].name == s) {
            buf.clear();
            buf.append(entities[i].value);
            ok = true;
            break;
        }
    }
    if (!ok) buf += ";";
    auto V = codepoints(buf);
    while (!V.empty()) {
        reread_list.push_back(V.back());
        V.pop_back();
    }
    return ok;
}

void XmlIO::error(const std::string &s) const {
    nb_errs++;
    log_and_tty << "Error while parsing XML (line " << cur_line << ")\n" << s << ".\n";
}
