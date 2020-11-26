#pragma once
#include "Parser.h"
#include "XkvToken.h"

class XkvSetkeys {
    Parser *                 P;
    Token                    comma_token;
    Token                    equals_token;
    Token                    na_token;
    Token                    rm_token;
    Token                    fams_token;
    Token                    savevalue_token;
    std::vector<std::string> Fams;           // the list of all families
    std::vector<std::string> Na;             // the list of keys not to set
    std::vector<std::string> Keys;           // the list of keys
    TokenList                fams;           // the list of families
    TokenList                na;             // the list of keys that should not be set
    TokenList                keyvals;        // the keylist to set
    TokenList                delayed;        // unknown ketyvalue pairs
    TokenList                action;         // expansion of \setkeys
    bool                     no_err{false};  // Error when undefined ?
    bool                     set_all{false}; // set key in all families ?
    bool                     in_pox{false};  // are in in \ProcessOptionsX ?

public:
    XkvSetkeys(Parser *P);
    void run(bool c);
    void run2(bool);
    void check_preset(String s);
    void extract_keys(TokenList &L, std::vector<std::string> &R);
    void fetch_fams();
    void special_fams();
    void fetch_na();
    void fetch_keys(bool c);
    void check_action(XkvToken &cur);
    void run_key(Token mac, XkvToken &cur, const std::string &fam);
    void save_key(const std::string &Key, TokenList &L);
    void run_default(const std::string &Key, Token mac, bool s);
    void replace_pointers(TokenList &L);
    void new_unknown(TokenList &L) {
        delayed.splice(delayed.end(), L);
        delayed.push_back(comma_token);
    }
    void more_action(TokenList &L) { action.splice(action.end(), L); }
    void finish();
    void set_aux(TokenList &W, long idx);
    void set_aux() { set_aux(keyvals, -1); }
    void set_inpox() { in_pox = true; }
    void dump_keys();
};
