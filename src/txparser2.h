#pragma once
#include "tralics/Token.h"
#include "txclasses.h"
#include "txinline.h"
#include "txparser.h"
#include <utility>

// -*- C++ -*-
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file holds code for Xkeyval

class XkvToken {
    TokenList   initial; // of the form \gsavevalue{foo}=bar
    TokenList   value;
    std::string keyname;
    bool        is_global;
    bool        has_save;
    bool        has_val;
    TokenList   action;

public:
    [[nodiscard]] auto key_empty() const -> bool { return keyname.empty(); }
    [[nodiscard]] auto val_empty() const -> bool { return value.empty(); }
    void               set_initial(TokenList L) { initial = std::move(L); }
    auto               get_all() -> TokenList { return initial; }
    auto               get_code() -> TokenList { return value; }
    auto               get_action() -> TokenList & { return action; }
    void               extract();
    auto               ignore_this(std::vector<std::string> &igna) -> bool;
    auto               get_name() -> std::string & { return keyname; }
    void               prepare(const std::string &fam);
    auto               is_defined(const std::string &fam) -> bool;
    auto               get_save() -> bool { return has_save; }
    auto               check_save() -> bool;
};

// Used by \setkeys in xkv
class XkvSetkeys {
    Parser *                 P;
    Token                    comma_token;
    Token                    equals_token;
    Token                    na_token;
    Token                    rm_token;
    Token                    fams_token;
    Token                    savevalue_token;
    Token                    usevalue_token;
    std::vector<std::string> Fams;    // the list of all families
    std::vector<std::string> Na;      // the list of keys not to set
    std::vector<std::string> Keys;    // the list of keys
    TokenList                fams;    // the list of families
    TokenList                na;      // the list of keys that should not be set
    TokenList                keyvals; // the keylist to set
    TokenList                delayed; // unknown ketyvalue pairs
    TokenList                action;  // expansion of \setkeys
    bool                     no_err;  // Error when undefined ?
    bool                     set_all; // set key in all families ?
    bool                     in_pox;  // are in in \ProcessOptionsX ?
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
    void        more_action(TokenList &L) { action.splice(action.end(), L); }
    void        finish();
    void        set_aux(TokenList &W, long idx);
    void        set_aux() { set_aux(keyvals, -1); }
    static void find_pointer(const std::string &Key);
    void        set_inpox() { in_pox = true; }
    void        dump_keys();
};
