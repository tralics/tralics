// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains the code for accents.
// for instance, \'e expands to the character 0351 (internal encoding is
// iso-8859-1)

#include "tralics.h"
const char* txaccent_rcsid =
  "$Id: txaccent.C,v 2.16 2011/06/28 08:15:25 grimm Exp $";


namespace accent_ns {
  Token fetch_accent(int chr, int accent_code);
  Token fetch_double_accent(int chr, int);
  int combine_accents(int,int);
  int double_a_accent(int acc3);
  int double_e_accent(int acc3);
  int double_o_accent(int acc3);
  int double_u_accent(int acc3);
  int double_other_accent(int,int acc3);
  void boot_accents ();
  Token mk_acc(uint s) ;
  void special_acc_hack(TokenList&y);
  Token special_double_acc(int chr,int acc);

// Accent tables. Holds the result of the expansion
  Token accent_cir [nb_accents]; // \^
  Token accent_acute [nb_accents]; // \'
  Token accent_grave [nb_accents];  // \`
  Token accent_trema [nb_accents];  // \"
  Token accent_cedille[nb_accents]; // \c
  Token accent_breve[nb_accents]; // \u
  Token accent_check[nb_accents]; // \v
  Token accent_tilde[nb_accents]; // \~
  Token accent_uml[nb_accents]; // \H
  Token accent_ogon[nb_accents]; // \k
  Token accent_dotabove[nb_accents]; // \.
  Token accent_macron[nb_accents]; // \=
  Token accent_ring[nb_accents]; // \r
  Token accent_barunder[nb_accents]; // \b
  Token accent_dotunder[nb_accents]; // \d
  Token accent_ibreve[nb_accents]; // \f (inverted breve)
  Token accent_dgrave[nb_accents]; // \C like \`\`
  Token accent_tildeunder[nb_accents]; // \T
  Token accent_circunder[nb_accents]; // \V
  Token accent_rondunder[nb_accents]; // \D
  Token accent_hook[nb_accents]; // \h
  Token other_accent[special_table_length];
  Token special_double[20];
}


// Returns the token associated to acc for character chr.
// Return zero-token in case of failure
Token accent_ns::fetch_accent(int chr, int acc)
{
  switch(acc) {
  case '\'': return accent_acute[chr]; 
  case '`': return accent_grave[chr]; 
  case '^': return accent_cir[chr]; 
  case '"': return accent_trema[chr]; 
  case '~': return accent_tilde[chr]; 
  case 'k': return accent_ogon[chr]; 
  case 'H': return accent_uml[chr]; 
  case 'v': return accent_check[chr]; 
  case 'u': return accent_breve[chr]; 
  case 'c': return accent_cedille[chr]; 
  case '.': return accent_dotabove[chr]; 
  case '=': return accent_macron[chr]; 
  case 'r': return accent_ring[chr];
  case 'b': return accent_barunder[chr];
  case 'd': return accent_dotunder[chr];
  case 'C': return accent_dgrave[chr];
  case 'f': return accent_ibreve[chr];
  case 'T': return accent_tildeunder[chr];
  case 'V': return accent_circunder[chr];
  case 'D': return accent_rondunder[chr];
  case 'h': return accent_hook[chr];
  default:
    return Token(0);
  }
}

// For the case of \'\^e, construct an accent code.
// It is assumed that the order is irrelevant.

int accent_ns::combine_accents(int acc1, int acc2)
{
  // start with acute and something
  if(acc1=='\'' && acc2 =='~') return 9;
  else if(acc2=='\'' && acc1 =='~') return 9;
  else if(acc1=='\'' && acc2 =='=') return 10;
  else if(acc2=='\'' && acc1 =='=') return 10;
  else if(acc1=='\'' && acc2 =='.') return 11;
  else if(acc2=='\'' && acc1 =='.') return 11;
  else if(acc1=='\'' && acc2 =='^') return 12;
  else if(acc2=='\'' && acc1 =='^') return 12;
  else if(acc1=='\'' && acc2 =='H') return 16;
  else if(acc2=='\'' && acc1 =='H') return 16;
  else if(acc1=='\'' && acc2 =='c') return 17;
  else if(acc2=='\'' && acc1 =='c') return 17;
  else if(acc1=='\'' && acc2 =='"') return 2;
  else if(acc2=='\'' && acc1 =='"') return 2;
  else if(acc1=='\'' && acc2 =='^') return 21;
  else if(acc2=='\'' && acc1 =='^') return 21;
  else if(acc1=='\'' && acc2 =='u') return 34;
  else if(acc2=='\'' && acc1 =='u') return 34;
  // now grave and something
  else if(acc1=='`' && acc2 =='=') return 13;
  else if(acc2=='`' && acc1 =='=') return 13;
  else if(acc1=='`' && acc2 =='^') return 15;
  else if(acc2=='`' && acc1 =='^') return 15;
  else if(acc1=='`' && acc2 =='H') return 18;
  else if(acc2=='`' && acc1 =='H') return 18;
  else if(acc1=='`' && acc2 =='"') return 4;
  else if(acc2=='`' && acc1 =='"') return 4;
  else if(acc1=='`' && acc2 =='u') return 22;
  else if(acc2=='`' && acc1 =='u') return 22;
  // now dot and something
  else if(acc1=='.' && acc2 =='=') return 5;
  else if(acc2=='.' && acc1 =='=') return 5;
  else if(acc1=='.' && acc2 =='v') return 19;
  else if(acc2=='.' && acc1 =='v') return 19;
  else if(acc1=='.' && acc2 =='d') return 20;
  else if(acc2=='.' && acc1 =='d') return 20;
  // now hat and something
  else if(acc1=='^' && acc2 =='h')  return  23;
  else if(acc2=='^' && acc1 =='h')  return  23;
  else if(acc1=='^' && acc2 =='~')  return  24;
  else if(acc2=='^' && acc1 =='~')  return  24;
  else if(acc1=='^' && acc2 =='d')  return  25;
  else if(acc2=='^' && acc1 =='d')  return  25;
  // diaresis
  else if(acc1=='"' && acc2 =='=') return 1;
  else if(acc2=='"' && acc1 =='=') return 1;
  else if(acc1=='"' && acc2 =='v') return 3;
  else if(acc2=='"' && acc1 =='v') return 3;
  else if(acc1=='"' && acc2 =='~') return 27;
  else if(acc2=='"' && acc1 =='~') return 27;
  // other
  else if(acc1=='=' && acc2 =='~') return 6;
  else if(acc2=='=' && acc1 =='~') return 6;
  else if(acc1=='k' && acc2 =='=') return 7;
  else if(acc2=='k' && acc1 =='=') return 7;
  else if(acc1=='d' && acc2 =='=') return 14;
  else if(acc2=='d' && acc1 =='=') return 14;
  else if(acc1=='u' && acc2 =='c') return 26;
  else if(acc2=='u' && acc1 =='c') return 26;
  else if(acc1=='H' && acc2 =='h')  return  28;
  else if(acc2=='H' && acc1 =='h')  return  28;
  else if(acc1=='H' && acc2 =='~')  return  29;
  else if(acc2=='H' && acc1 =='~')  return  29;
  else if(acc1=='H' && acc2 =='d')  return  30;
  else if(acc2=='H' && acc1 =='d')  return  30;
  else if(acc1=='u' && acc2 =='h')  return  31;
  else if(acc2=='u' && acc1 =='h')  return  31;
  else if(acc1=='u' && acc2 =='~')  return  32;
  else if(acc2=='u' && acc1 =='~')  return  32;
  else if(acc1=='u' && acc2 =='d')  return  33;
  else if(acc2=='u' && acc1 =='d')  return  33;
  else return 0;
}

// Fetches the position of a double accent on capital O
int accent_ns::double_o_accent(int acc3)
{
  switch(acc3) {
  case 1: return Odiamacro_cc;
  case 5: return Odotmacro_cc;
  case 6: return Otilmacro_cc;
  case 7: return Oogonmacron_cc;
  case 9: return Otildeaigu_cc;
  case 10: return Obaraigu_cc;
  case 12: return Ociracute_cc;
  case 13: return Obargrave_cc;
  case 16: return  Oacuteyy_cc;
  case 18: return  Ograveyy_cc;
  case 21: return  Ociracute_cc;
  case 15: return  Ocirgrave_cc;
  case 23: return  Ocirxx_cc;
  case 24: return  Ocirtilde_cc;
  case 25: return  Ocirdot_cc;
  case 27: return  Otildedp_cc;
  case 28: return  Oxxyy_cc;
  case 29: return  Otildeyy_cc;
  case 30: return  Odotyy_cc;
  default: return unused_accent_even_cc;
  }
} 

// Fetches the position of a double accent on capital E
int accent_ns::double_e_accent(int acc3)
{
  if(acc3==10) return  Ebaracute_cc;
  if(acc3==12) return  Eciracute_cc;
  if(acc3==13) return  Ebargrave_cc;
  if(acc3==26)  return  Ecedrond_cc;
  if(acc3==15)  return  Ecirgrave_cc;
  if(acc3==23)  return  Ecirxx_cc;
  if(acc3==24)  return  Ecirtilde_cc;
  if(acc3==25)  return  Ecirdot_cc;
  return unused_accent_even_cc;
}


// Fetches the position of a double accent on capital A
int accent_ns::double_a_accent(int acc3)
{
  if(acc3==1) return Adiamacro_cc;
  if(acc3==5) return Adotmacro_cc;
  if(acc3==12) return Aciracute_cc;
  if(acc3==15) return Acirgrave_cc;
  if(acc3==25) return Acirdot_cc;
  if(acc3==23) return Acirxx_cc;
  if(acc3==24) return Acirtilde_cc;
  if(acc3==34) return Abreveacute_cc;
  if(acc3==22) return Abrevegrave_cc;
  if(acc3==31) return Abrevexx_cc;
  if(acc3==32) return Abrevetilde_cc;
  if(acc3==33) return Abrevedot_cc;  
  return unused_accent_even_cc;
}

// Fetches the position of a double accent on capital U
int accent_ns::double_u_accent(int acc3)
{
  if(acc3==1) return Udiamacro_cc;
  if(acc3==2) return Udiaacute_cc;
  if(acc3==3) return Udiacaron_cc;
  if(acc3==4) return Udiagrave_cc;
  if(acc3==9) return Utildeacute_cc;
  if(acc3==16)  return  Uacuteyy_cc;
  if(acc3==18)  return  Ugraveyy_cc;
  if(acc3==28)  return  Uxxyy_cc;
  if(acc3==29)  return  Utildeyy_cc;
  if(acc3==30)  return  Udotyy_cc;
  return unused_accent_even_cc;
}

// Fetches the position of a double accent on a, not in the list above
int accent_ns::double_other_accent(int a, int acc3)
{
  if(a=='I') {
    if(acc3==2) return  Itremaacute_cc;
  }
  if(a=='S') {
    if(acc3==11) return  Sdotacute_cc;
    if(acc3==19) return  Sdotcaron_cc;
    if(acc3==20) return  Sdotdot_cc;
  }
  if(a=='C') {
    if(acc3==17) return Ccedilacute_cc;
  }
  if(a=='L') {
    if(acc3==14) return  Ldotbar_cc;
  }
  if(a=='R') {
    if(acc3==14) return  Rdotbar_cc;
  }
  if('a'<=a && a <= 'z') 
    return double_other_accent(a-'a'+'A',acc3) + 1;
  return unused_accent_even_cc;
}

// This command puts a double accent on a letter.
// The result is a token (character or command) from
// the other_accent table defined above; or empty token in case of failure.
// Assumes that lower case letters follow upper case ones.
Token accent_ns::fetch_double_accent(int a, int acc3)
{
  if(a=='U') return other_accent[double_u_accent(acc3)];
  if(a=='u') return other_accent[double_u_accent(acc3)+1];
  if(a=='A') return other_accent[double_a_accent(acc3)];
  if(a=='a') return other_accent[double_a_accent(acc3)+1];
  if(a=='O') return other_accent[double_o_accent(acc3)];
  if(a=='o') return other_accent[double_o_accent(acc3)+1];
  if(a=='E') return other_accent[double_e_accent(acc3)];
  if(a=='e') return other_accent[double_e_accent(acc3)+1];
  return other_accent[double_other_accent(a,acc3)];
}

// This implements \a', or \a{'}, expansion is \', empty in case of an error
// cur_tok is \a, in case of error
void Parser::accent_a ()
{
  if(tracing_macros()) the_log << lg_startbrace << cur_tok << lg_endbrace;
  TokenList y = mac_arg();
  if(!token_ns::has_a_single_token(y)) {
    parse_error("wanted a single token as argument to \\a");
    return;
  }
  Token t = token_ns::get_unique(y);
  if(!t.is_a_char()) {  // latex says \csname\string #1 \endcsname
    parse_error("Bad syntax of \\a, argument not a character ", t,"",
		"bad accent");
    return;
  }
  Token Y = Token(t.chr_val() + single_base + cs_token_flag);
  token_from_list(Y);
  if(cur_cmd_chr.get_cmd() !=accent_cmd) { 
    parse_error("Bad syntax of \\a, argument is ", t,"","bad accent");
    return;
  }
  back_input();
}

// removes some useless braces.
void accent_ns::special_acc_hack(TokenList&y)
{
  if(y.empty()) return;
  if(y.front().is_space_token()) y.pop_front();
  if(y.empty()) return;
  Token T=y.front();
  if(!T.is_OB_token()) return;
  y.pop_front();
  if(y.empty()) return; // this should not happen
  Token T2=y.front();
  y.pop_front();
  if(y.empty()) return; // this should not happen
  if(y.front().is_CB_token()) {
    y.pop_front();
    y.push_front(T2);
  } else {
    y.push_front(T2);
    y.push_front(T);
  }
}

Token accent_ns::special_double_acc(int chr,int acc)
{
  if(acc=='`' && chr=='*') return special_double[0];
  if(acc=='`' && chr=='.') return special_double[1];
  if(acc=='\'' && chr=='*') return special_double[2];
  if(acc=='\'' && chr=='=') return special_double[3];
  if(acc=='^' && chr=='*') return special_double[4];
  if(acc=='^' && chr=='.') return special_double[5];
  if(acc=='~' && chr=='*') return special_double[6];
  if(acc=='~' && chr=='.') return special_double[7];
  if(acc=='"' && chr=='*') return special_double[8];
  if(acc=='H' && chr=='*') return special_double[9];
  if(acc=='r' && chr=='*') return special_double[10];
  if(acc=='r' && chr=='=') return special_double[11];
  if(acc=='v' && chr=='*') return special_double[12];
  if(acc=='v' && chr=='\'') return special_double[13];
  if(acc=='u' && chr=='*') return special_double[14];
  if(acc=='u' && chr=='=') return special_double[15];
  if(acc=='=' && chr=='*') return special_double[16];
  if(acc=='.' && chr=='*') return special_double[17];
  if(acc=='.' && chr=='\'') return special_double[18];
  if(acc=='t' && chr=='*') return special_double[19];
  return Token();
}



// This implements \'e, or \'{a} or \a{'}{e}, \href{\~grimm}
// or \'{\= u}, a double accent
// cur_tok, cur_cmd_chr explain what to do.
void Parser::accent ()
{
  if(tracing_macros()) 
    the_log << lg_startbrace << "accent " << cur_tok << lg_endbrace;
  int acc_code = cur_cmd_chr.get_chr();
  if(InUrlHandler::global_in_url && acc_code=='~') {
    if(tracing_macros()) {
       the_log << lg_startbrace << "\\~ gives ~ " << lg_endbrace;
    }
    back_input(Token(other_t_offset,'~'));
    return;
  }
  static String msg1= "Error in accent, command = " ;
  static String msg2= "bad accent";
  Token tfe= cur_tok;
  // Fetch the accent
  TokenList y = mac_arg();
  if(y.empty()) {
    String msg3 = acc_code=='~' ?
      "\n\\~{} is the wrong way to put a tilde in an URL" :
      "\nThings like {\\'{}} are a bit strange";
    parse_error(msg1,tfe,msg3,msg2);
    return;
  }
  int acc_code2 = 0; // will be set in the double acc case.
  Token tfe2; // Second token for error.
  if(y.size() >= 2) { 
    token_from_list(y.front()); // Get the meaning of the start of y
    // case \'{\a'e}; 
    if(cur_cmd_chr.get_cmd()==a_cmd) {
      y.pop_front(); // discard the \a
      accent_ns::special_acc_hack(y);
      if(y.empty()) {
	parse_error("Internal error, lost argument of \\a");
	return;
      }
      Token t = y.front(); // get the '
      if(!t.is_a_char()) {
	parse_error("Bad syntax of \\a, argument not a character ", t,"", msg2);
	return;
      }
      y.pop_front(); // discard the accent
      tfe2 = Token(t.chr_val() + single_base + cs_token_flag);
      token_from_list(tfe2);
      if(cur_cmd_chr.get_cmd() !=accent_cmd) { 
	parse_error("Bad syntax of \\a,  argument is ", t,"",msg2);
	return;
      }
    } else if(cur_cmd_chr.get_cmd() ==accent_cmd) {
      tfe2 = cur_tok;
      y.pop_front();
    } else {
      parse_error(msg1,tfe,"\nWanted a single token",msg2);
      return;
    }
    acc_code2 = cur_cmd_chr.get_chr();
    accent_ns::special_acc_hack(y);
    if(y.size() != 1) { 
      parse_error(msg1,tfe,"\nWanted a single token",msg2); 
      return; 
    }
  }
  Token Y = token_ns::get_unique(y);
  token_from_list(Y);
  unsigned int achar = cur_cmd_chr.get_chr();
  if(achar<=8) achar = 8;  // make these invalid
  else if(achar==0xc6) achar =1;
  else if(achar==0xe6) achar =2;
  else if(achar==0xc5) achar =3;
  else if(achar==0xe5) achar =4;
  else if(achar==0xd8) achar =5;
  else if(achar==0xf8) achar =6;
  if(cur_cmd_chr.get_cmd() == cst1_cmd && achar==i_code) achar = 'i';
  else if(cur_cmd_chr.get_cmd()==specchar_cmd) {}
  else if(cur_cmd_chr.is_letter_other()) {}
  else {
    err_ns::local_buf << bf_reset << msg1;
    err_ns::local_buf << tfe.tok_to_str();
    if(acc_code2) err_ns::local_buf << tfe2.tok_to_str();
    if(Y.is_invalid()) 
      err_ns::local_buf <<  "\nEnd of data reached while scanning argument";
    else if(!cur_tok.not_a_cmd())
      err_ns::local_buf << "\nLetter needed instead of " << cur_tok.tok_to_str();
    else 
      err_ns::local_buf << "\nLetter needed"; 
    err_ns::signal_error(msg2,0);
    return;
  }
  Token res;
  bool spec = false;
  if(achar <nb_accents) {
    if(acc_code2) {
      int acc3_code = accent_ns::combine_accents(acc_code,acc_code2);
      res = accent_ns::fetch_double_accent(achar, acc3_code);
    } else {
      res = accent_ns::fetch_accent(achar, acc_code);
      if(res.is_null()&& achar<128) {
	if(is_letter(achar)) {
	  spec = true; // T. Bouche veut une erreur
	  res = accent_ns::fetch_accent(0, acc_code);
	}
	else res = accent_ns::special_double_acc(achar, acc_code);
      }
    }
  }
  if(res.is_null()) { 
    String s = achar>=128 ? "a non 7-bit character" : 
      is_letter(achar) ? "letter" : is_digit(achar) ? "digit" : 
      "non-letter character";
    err_ns::local_buf << bf_reset << msg1;
    err_ns::local_buf << tfe.tok_to_str();
    if(acc_code2) err_ns::local_buf << tfe2.tok_to_str();
    err_ns::local_buf <<  "\nCannot put accent on " << s;
    if(achar<128) err_ns::local_buf << " " << char(achar);
    err_ns::signal_error(msg2,0);
    return;
  }
  TokenList expansion;
  expansion.push_back(res);
  if(spec) {
    expansion.push_front(Y);
    brace_me(expansion);
    expansion.push_front(hash_table.composite_token);
  }
    
  if(tracing_macros()) { 
    the_log << lg_startbrace << "accent on " << uchar(achar) <<  " -> "
	    << expansion << lg_endbrace;
  }
  back_input(expansion);
}


// Simple case of \^a that gives \342. 
inline Token accent_ns::mk_acc(uint s) 
{
  return Token(other_t_offset, Utf8Char(s));
}


// Creates the table for accents.
void accent_ns::boot_accents ()
{
  accent_acute[0] = mk_acc(0x301); 
  accent_grave[0] = mk_acc(0x300); 
  accent_cir[0] = mk_acc(0x302); 
  accent_trema[0] = mk_acc(0x30e); 
  accent_tilde[0] = mk_acc(0x303); 
  accent_ogon[0] = mk_acc(0x328); 
  accent_uml[0] = mk_acc(0x30b); 
  accent_check[0] = mk_acc(0x30c); 
  accent_breve[0] = mk_acc(0x306); 
  accent_cedille[0] = mk_acc(0x327); 
  accent_dotabove[0] = mk_acc(0x307); 
  accent_macron[0] = mk_acc(0x304); 
  accent_ring[0] = mk_acc(0x30a);
  accent_barunder[0] = mk_acc(0x320);
  accent_dotunder[0] = mk_acc(0x323);
  accent_dgrave[0] = mk_acc(0x30f);
  accent_ibreve[0] = mk_acc(0x311);
  accent_tildeunder[0] = mk_acc(0x330);
  accent_circunder[0] = mk_acc(0x32d);
  accent_rondunder[0] = mk_acc(0x325);
  accent_hook[0] = mk_acc(0x309);

  accent_grave[uchar('A') ] = mk_acc(uchar('\300'));
  accent_acute[uchar('A') ] = mk_acc(uchar('\301'));
  accent_cir[uchar('A') ] = mk_acc(uchar('\302'));
  accent_tilde[uchar('A') ] = mk_acc(uchar('\303'));
  accent_trema[uchar('A') ] = mk_acc(uchar('\304'));
  accent_ring[uchar('A') ] = mk_acc(0305);
  // 306=\AE
  accent_cedille[uchar('C') ] = mk_acc(uchar('\307'));
  accent_grave[uchar('E') ] = mk_acc(uchar('\310'));
  accent_acute[uchar('E') ] = mk_acc(uchar('\311'));
  accent_cir[uchar('E') ] = mk_acc(uchar('\312'));
  accent_trema[uchar('E') ] = mk_acc(uchar('\313'));
  accent_grave[uchar('I') ] = mk_acc(uchar('\314'));
  accent_acute[uchar('I') ] = mk_acc(uchar('\315'));
  accent_cir[uchar('I') ] = mk_acc(uchar('\316'));
  accent_trema[uchar('I') ] = mk_acc(uchar('\317'));
  // 320= \DH
  accent_tilde[uchar('N') ] = mk_acc(uchar('\321'));
  accent_grave[uchar('O') ] = mk_acc(uchar('\322'));
  accent_acute[uchar('O') ] = mk_acc(uchar('\323'));
  accent_cir[uchar('O') ] = mk_acc(uchar('\324'));
  accent_tilde[uchar('O') ] = mk_acc(uchar('\325'));
  accent_trema[uchar('O') ] = mk_acc(uchar('\326'));
  // 327=\times
  // 330=\O
  accent_grave[uchar('U') ] = mk_acc(uchar('\331'));
  accent_acute[uchar('U') ] = mk_acc(uchar('\332'));
  accent_cir[uchar('U') ] = mk_acc(uchar('\333'));
  accent_trema[uchar('U') ] = mk_acc(uchar('\334'));
  accent_acute[uchar('Y') ] = mk_acc(uchar('\335'));
  // 336=\TH, \337=\ss
  accent_grave[uchar('a') ] = mk_acc(uchar('\340'));
  accent_acute[uchar('a') ] = mk_acc(uchar('\341'));
  accent_cir[uchar('a') ] = mk_acc(uchar('\342'));
  accent_tilde[uchar('a') ] = mk_acc(uchar('\343'));
  accent_trema[uchar('a') ] = mk_acc(uchar('\344'));
  accent_ring[uchar('a') ] = mk_acc(uchar('\345'));
  // 346=\ae
  accent_cedille[uchar('c') ] = mk_acc(uchar('\347'));
  accent_grave[uchar('e') ] = mk_acc(uchar('\350'));
  accent_acute[uchar('e') ] = mk_acc(uchar('\351'));
  accent_cir[uchar('e') ] = mk_acc(uchar('\352'));
  accent_trema[uchar('e') ] = mk_acc(uchar('\353'));
  accent_grave[uchar('i') ] = mk_acc(uchar('\354'));
  accent_acute[uchar('i') ] = mk_acc(uchar('\355'));
  accent_cir[uchar('i') ] = mk_acc(uchar('\356'));
  accent_trema[uchar('i') ] = mk_acc(uchar('\357'));
  // 360=\dh
  accent_tilde[uchar('n') ] = mk_acc(uchar('\361'));
  accent_grave[uchar('o') ] = mk_acc(uchar('\362'));
  accent_acute[uchar('o') ] = mk_acc(uchar('\363'));
  accent_cir[uchar('o') ] = mk_acc(uchar('\364'));
  accent_tilde[uchar('o') ] = mk_acc(uchar('\365'));
  accent_trema[uchar('o') ] = mk_acc(uchar('\366'));
  // 367=\div 370=\o
  accent_grave[uchar('u') ] = mk_acc(uchar('\371'));
  accent_acute[uchar('u') ] = mk_acc(uchar('\372'));
  accent_cir[uchar('u') ] = mk_acc(uchar('\373'));
  accent_trema[uchar('u') ] = mk_acc(uchar('\374'));
  accent_acute[uchar('y') ] = mk_acc(uchar('\375'));
  // 376 =\th
  accent_trema[uchar('y') ] = mk_acc(uchar('\377'));





  accent_macron[uchar('A') ] = mk_acc(0x100);
  accent_macron[uchar('a') ] = mk_acc(0x101);
  accent_breve[uchar('A') ] = mk_acc(0x102);
  accent_breve[uchar('a') ] = mk_acc(0x103);
  accent_ogon[uchar('A') ] = mk_acc(0x104); 
  accent_ogon[uchar('a') ] = mk_acc(0x105); 
  accent_acute[uchar('C') ] = mk_acc(0x106);
  accent_acute[uchar('c') ] = mk_acc(0x107);
  accent_cir[uchar('C') ] = mk_acc(0x108);
  accent_cir[uchar('c') ] = mk_acc(0x109);
  accent_dotabove[uchar('C') ] = mk_acc(0x10a);
  accent_dotabove[uchar('c') ] = mk_acc(0x10b);
  accent_check[uchar('C') ] = mk_acc(0x10c);
  accent_check[uchar('c') ] = mk_acc(0x10d);
  accent_check[uchar('D') ] = mk_acc(0x10e);
  accent_check[uchar('d') ] = mk_acc(0x10f);

  // 110 = \Dj, 111=\dj
  accent_macron[uchar('E') ] = mk_acc(0x112);
  accent_macron[uchar('e') ] = mk_acc(0x113);
  accent_breve[uchar('E') ] = mk_acc(0x114);
  accent_breve[uchar('e') ] = mk_acc(0x115);
  accent_dotabove[uchar('E') ] = mk_acc(0x116);
  accent_dotabove[uchar('e') ] = mk_acc(0x117);
  accent_ogon[uchar('E') ] = mk_acc(0x118); 
  accent_ogon[uchar('e') ] = mk_acc(0x119); 
  accent_check[uchar('E') ] = mk_acc(0x11a);
  accent_check[uchar('e') ] = mk_acc(0x11b);
  accent_cir[uchar('G') ] = mk_acc(0x11c);
  accent_cir[uchar('g') ] = mk_acc(0x11d);
  accent_breve[uchar('G') ] = mk_acc(0x11e);
  accent_breve[uchar('g') ] = mk_acc(0x11f);

  accent_dotabove[uchar('G') ] = mk_acc(0x120);
  accent_dotabove[uchar('g') ] = mk_acc(0x121);
  accent_cedille[uchar('G') ] = mk_acc(0x122);
  accent_cedille[uchar('g') ] = mk_acc(0x123);
  accent_cir[uchar('H') ] = mk_acc(0x124);
  accent_cir[uchar('h') ] = mk_acc(0x125);
  accent_macron[uchar('H') ] = mk_acc(0x126);
  accent_macron[uchar('h') ] = mk_acc(0x127);
  accent_tilde[uchar('I') ] = mk_acc(0x128);
  accent_tilde[uchar('i') ] = mk_acc(0x129);
  accent_macron[uchar('I') ] = mk_acc(0x12a);
  accent_macron[uchar('i') ] = mk_acc(0x12b);
  accent_breve[uchar('I') ] = mk_acc(0x12c);
  accent_breve[uchar('i') ] = mk_acc(0x12d);
  accent_ogon[uchar('I') ] = mk_acc(0x12e); 
  accent_ogon[uchar('i') ] = mk_acc(0x12f); 

  accent_dotabove[uchar('I') ] = mk_acc(0x130);
  // i_code=131, IJ = 0x132, ij = 0x133
  accent_cir[uchar('J') ] = mk_acc(0x134);
  accent_cir[uchar('j') ] = mk_acc(0x135);
  accent_cedille[uchar('K') ] = mk_acc(0x136);
  accent_cedille[uchar('k') ] = mk_acc(0x137);
  // kgreen = 138
  accent_acute[uchar('L') ] = mk_acc(0x139);
  accent_acute[uchar('l') ] = mk_acc(0x13a);
  accent_cedille[uchar('L') ] = mk_acc(0x13b);
  accent_cedille[uchar('l') ] = mk_acc(0x13c);
  accent_check[uchar('L') ] = mk_acc(0x13d);
  accent_check[uchar('l') ] = mk_acc(0x13e);
  accent_dotabove[uchar('L') ] = mk_acc(0x13f);
  accent_dotabove[uchar('l') ] = mk_acc(0x140);
  // \L = 0x141, \l = 0x142
  accent_acute[uchar('N') ] = mk_acc(0x143);
  accent_acute[uchar('n') ] = mk_acc(0x144);
  accent_cedille[uchar('N') ] = mk_acc(0x145);
  accent_cedille[uchar('n') ] = mk_acc(0x146);
  accent_check[uchar('N') ] = mk_acc(0x147);
  accent_check[uchar('n') ] = mk_acc(0x148);
  // 0x149 = n apostrophe "ng" 0x14a, NG = 0x14b
  accent_macron[uchar('O') ] = mk_acc(0x14c);
  accent_macron[uchar('o') ] = mk_acc(0x14d);
  accent_breve[uchar('O') ] = mk_acc(0x14e); 
  accent_breve[uchar('o') ] = mk_acc(0x14f); 
  accent_uml[uchar('O') ] = mk_acc(0x150);
  accent_uml[uchar('o') ] = mk_acc(0x151);
  // mk_acc("OE",0x152);
  // mk_acc("oe",0x153);
  accent_acute[uchar('R') ] = mk_acc(0x154);
  accent_acute[uchar('r') ] = mk_acc(0x155);
  accent_cedille[uchar('R') ] = mk_acc(0x156);
  accent_cedille[uchar('r') ] = mk_acc(0x157);
  accent_check[uchar('R') ] = mk_acc(0x158);
  accent_check[uchar('r') ] = mk_acc(0x159);
  accent_acute[uchar('S') ] = mk_acc(0x15a);
  accent_acute[uchar('s') ] = mk_acc(0x15b);
  accent_cir[uchar('S') ] = mk_acc(0x15c);
  accent_cir[uchar('s') ] = mk_acc(0x15d);
  accent_cedille[uchar('S') ] = mk_acc(0x15e);
  accent_cedille[uchar('s') ] = mk_acc(0x15f);
  accent_check[uchar('S') ] = mk_acc(0x160);
  accent_check[uchar('s') ] = mk_acc(0x161);
  accent_cedille[uchar('T') ] = mk_acc(0x162);
  accent_cedille[uchar('t') ] = mk_acc(0x163);
  accent_check[uchar('T') ] = mk_acc(0x164);
  accent_check[uchar('t') ] = mk_acc(0x165);
  accent_macron[uchar('T') ] = mk_acc(0x166);
  accent_macron[uchar('t') ] = mk_acc(0x167);
  accent_tilde[uchar('U') ] = mk_acc(0x168);
  accent_tilde[uchar('u') ] = mk_acc(0x169);
  accent_macron[uchar('U') ] = mk_acc(0x16a);
  accent_macron[uchar('u') ] = mk_acc(0x16b);
  accent_breve[uchar('U') ] = mk_acc(0x16c);
  accent_breve[uchar('u') ] = mk_acc(0x16d);
  accent_ring[uchar('U') ] = mk_acc(0x16e);
  accent_ring[uchar('u') ] = mk_acc(0x16f);

  accent_uml[uchar('U') ] = mk_acc(0x170);
  accent_uml[uchar('u') ] = mk_acc(0x171);
  accent_ogon[uchar('U') ] = mk_acc(0x172); 
  accent_ogon[uchar('u') ] = mk_acc(0x173); 
  accent_cir[uchar('W') ] = mk_acc(0x174);
  accent_cir[uchar('w') ] = mk_acc(0x175); 
  accent_cir[uchar('Y') ] = mk_acc(0x176);
  accent_cir[uchar('y') ] = mk_acc(0x177);
  accent_trema[uchar('Y') ] = mk_acc(0x178);
  accent_acute[uchar('Z') ] = mk_acc(0x179);
  accent_acute[uchar('z') ] = mk_acc(0x17a);
  accent_dotabove[uchar('Z') ] = mk_acc(0x17b);
  accent_dotabove[uchar('z') ] = mk_acc(0x17c);
  accent_check[uchar('Z') ] = mk_acc(0x17d);
  accent_check[uchar('z') ] = mk_acc(0x17e);
  // long_s  0x17f

  // 01c?
  accent_check[uchar('A') ] = mk_acc(0x1cd);
  accent_check[uchar('a') ] = mk_acc(0x1ce);
  accent_check[uchar('I') ] = mk_acc(0x1cf);

  accent_check[uchar('i') ] = mk_acc(0x1d0);
  accent_check[uchar('O') ] = mk_acc(0x1d1);
  accent_check[uchar('o') ] = mk_acc(0x1d2);
  accent_check[uchar('U') ] = mk_acc(0x1d3);
  accent_check[uchar('u') ] = mk_acc(0x1d4);
  other_accent[Udiamacro_cc] = mk_acc(0x1d5);
  other_accent[udiamacro_cc] = mk_acc(0x1d6);
  other_accent[Udiaacute_cc] = mk_acc(0x1d7);
  other_accent[udiaacute_cc] = mk_acc(0x1d8);
  other_accent[Udiacaron_cc] =  mk_acc(0x1d9);
  other_accent[udiacaron_cc] =  mk_acc(0x1da);
  other_accent[Udiagrave_cc] =  mk_acc(0x1db);
  other_accent[udiagrave_cc] =  mk_acc(0x1dc);
  // inverted e = 1dd
  other_accent[Adiamacro_cc] = mk_acc(0x1de);
  other_accent[adiamacro_cc] = mk_acc(0x1df);

  other_accent[Adotmacro_cc] = mk_acc(0x1e0);
  other_accent[adotmacro_cc] = mk_acc(0x1e1);
  accent_macron[1] = mk_acc(0x1e2);
  accent_macron[2] = mk_acc(0x1e3);
  // 1e4 1e5: G with bar
  accent_check[uchar('G') ] = mk_acc(0x1e6);
  accent_check[uchar('g') ] = mk_acc(0x1e7);
  accent_check[uchar('K') ] = mk_acc(0x1e8);
  accent_check[uchar('k') ] = mk_acc(0x1e9);
  accent_ogon[uchar('O') ] = mk_acc(0x1ea); 
  accent_ogon[uchar('o') ] = mk_acc(0x1eb); 
  other_accent[Oogonmacron_cc] = mk_acc(0x1ec);
  other_accent[oogonmacron_cc] = mk_acc(0x1ed);
  // 1ee 1ef: ezh with caron.

  accent_check[uchar('j') ] = mk_acc(0x1f0);
  // 1f1 1f2 1f3: DZ, Dz and dz
  accent_acute[uchar('G') ] = mk_acc(0x1f4);
  accent_acute[uchar('g') ] = mk_acc(0x1f5);
  // 1f6 1f7 ?
  accent_grave[uchar('N') ] = mk_acc(0x1f8);
  accent_grave[uchar('n') ] = mk_acc(0x1f9);
  accent_acute[3] = mk_acc(0x1fa);
  accent_acute[4] = mk_acc(0x1fb);
  accent_acute[1] = mk_acc(0x1fc);
  accent_acute[2] = mk_acc(0x1fd);
  accent_acute[5] = mk_acc(0x1fe);
  accent_acute[6] = mk_acc(0x1ff);

  accent_dgrave[uchar('A')] = mk_acc(0x200);
  accent_dgrave[uchar('a')] = mk_acc(0x201);
  accent_ibreve[uchar('A')] = mk_acc(0x202);
  accent_ibreve[uchar('a')] = mk_acc(0x203);
  accent_dgrave[uchar('E')] = mk_acc(0x204);
  accent_dgrave[uchar('e')] = mk_acc(0x205);
  accent_ibreve[uchar('E')] = mk_acc(0x206);
  accent_ibreve[uchar('e')] = mk_acc(0x207);
  accent_dgrave[uchar('I')] = mk_acc(0x208);
  accent_dgrave[uchar('i')] = mk_acc(0x209);
  accent_ibreve[uchar('I')] = mk_acc(0x20a);
  accent_ibreve[uchar('i')] = mk_acc(0x20b);
  accent_dgrave[uchar('O')] = mk_acc(0x20c);
  accent_dgrave[uchar('o')] = mk_acc(0x20d);
  accent_ibreve[uchar('O')] = mk_acc(0x20e);
  accent_ibreve[uchar('o')] = mk_acc(0x20f);

  accent_dgrave[uchar('R')] = mk_acc(0x210);
  accent_dgrave[uchar('r')] = mk_acc(0x211);
  accent_ibreve[uchar('R')] = mk_acc(0x212);
  accent_ibreve[uchar('r')] = mk_acc(0x213);
  accent_dgrave[uchar('U')] = mk_acc(0x214);
  accent_dgrave[uchar('u')] = mk_acc(0x215);
  accent_ibreve[uchar('U')] = mk_acc(0x216);
  accent_ibreve[uchar('u')] = mk_acc(0x217);
  // 218 219 220 221: ST with comma below
  accent_check[uchar('H')] = mk_acc(0x21e);
  accent_check[uchar('h')] = mk_acc(0x21f);

  // 0220 to  225 missing
  accent_dotabove[uchar('A')] = mk_acc(0x226);
  accent_dotabove[uchar('a')] = mk_acc(0x227);
  accent_cedille[uchar('E')] = mk_acc(0x228);
  accent_cedille[uchar('e')] = mk_acc(0x229);
  other_accent[Odiamacro_cc] = mk_acc(0x22a);
  other_accent[odiamacro_cc] = mk_acc(0x22b);
  other_accent[Otilmacro_cc] = mk_acc(0x22c);
  other_accent[otilmacro_cc] = mk_acc(0x22d);
  accent_dotabove[uchar('O')] = mk_acc(0x22e);
  accent_dotabove[uchar('o')] = mk_acc(0x22f);
  other_accent[Odotmacro_cc] = mk_acc(0x230);
  other_accent[odotmacro_cc] = mk_acc(0x231);
  accent_macron[uchar('Y')] = mk_acc(0x232);
  accent_macron[uchar('y')] = mk_acc(0x233);

  // later
  accent_rondunder[uchar('A')] = mk_acc(0x1e00);
  accent_rondunder[uchar('a')] = mk_acc(0x1e01);
  accent_dotabove[uchar('B')] = mk_acc(0x1e02);
  accent_dotabove[uchar('b')] = mk_acc(0x1e03);
  accent_dotunder[uchar('B')] = mk_acc(0x1e04);
  accent_dotunder[uchar('b')] = mk_acc(0x1e05);
  accent_barunder[uchar('B')] = mk_acc(0x1e06);
  accent_barunder[uchar('b')] = mk_acc(0x1e07);
  other_accent[Ccedilacute_cc] = mk_acc(0x1e08);
  other_accent[ccedilacute_cc] = mk_acc(0x1e09);
  accent_dotabove[uchar('D')] = mk_acc(0x1e0a);
  accent_dotabove[uchar('d')] = mk_acc(0x1e0b);
  accent_dotunder[uchar('D')] = mk_acc(0x1e0c);
  accent_dotunder[uchar('d')] = mk_acc(0x1e0d);
  accent_barunder[uchar('D')] = mk_acc(0x1e0e);
  accent_barunder[uchar('d')] = mk_acc(0x1e0f);

  accent_cedille[uchar('D')] = mk_acc(0x1e10);
  accent_cedille[uchar('d')] = mk_acc(0x1e11);
  accent_circunder[uchar('D')] = mk_acc(0x1e12);
  accent_circunder[uchar('d')] = mk_acc(0x1e13);
  other_accent[Ebargrave_cc] = mk_acc(0x1e14);
  other_accent[ebargrave_cc] = mk_acc(0x1e15);
  other_accent[Ebaracute_cc] = mk_acc(0x1e16);
  other_accent[ebaracute_cc] = mk_acc(0x1e17);
  accent_circunder[uchar('E')] = mk_acc(0x1e18);
  accent_circunder[uchar('e')] = mk_acc(0x1e19);
  accent_tildeunder[uchar('E')] = mk_acc(0x1e1a);
  accent_tildeunder[uchar('e')] = mk_acc(0x1e1b);
  other_accent[Ecedrond_cc] = mk_acc(0x1e1c);
  other_accent[ecedrond_cc] = mk_acc(0x1e1d);
  accent_dotabove[uchar('F')] = mk_acc(0x1e1e);
  accent_dotabove[uchar('f')] = mk_acc(0x1e1f);

  accent_macron[uchar('G')] = mk_acc(0x1e20);
  accent_macron[uchar('g')] = mk_acc(0x1e21);
  accent_dotabove[uchar('H')] = mk_acc(0x1e22);
  accent_dotabove[uchar('h')] = mk_acc(0x1e23);
  accent_dotunder[uchar('H')] = mk_acc(0x1e24);
  accent_dotunder[uchar('h')] = mk_acc(0x1e25);
  accent_trema[uchar('H')] = mk_acc(0x1e26);
  accent_trema[uchar('h')] = mk_acc(0x1e27);
  accent_cedille[uchar('H')] = mk_acc(0x1e28);
  accent_cedille[uchar('h')] = mk_acc(0x1e29);
  // 1e2a 122b H breve below
  accent_tildeunder[uchar('I')] = mk_acc(0x1e2c);
  accent_tildeunder[uchar('i')] = mk_acc(0x1e2d);
  other_accent[Itremaacute_cc] = mk_acc(0x1e2e);
  other_accent[itremaacute_cc] = mk_acc(0x1e2f);

  accent_acute[uchar('K')] = mk_acc(0x1e30);
  accent_acute[uchar('k')] = mk_acc(0x1e31);
  accent_dotunder[uchar('K')] = mk_acc(0x1e32);
  accent_dotunder[uchar('k')] = mk_acc(0x1e33);
  accent_barunder[uchar('K')] = mk_acc(0x1e34);
  accent_barunder[uchar('k')] = mk_acc(0x1e35);
  accent_dotunder[uchar('L')] = mk_acc(0x1e36);
  accent_dotunder[uchar('l')] = mk_acc(0x1e37);
  other_accent[Ldotbar_cc] = mk_acc(0x1e38);
  other_accent[ldotbar_cc] = mk_acc(0x1e39);
  accent_barunder[uchar('L')] = mk_acc(0x1e3a);
  accent_barunder[uchar('l')] = mk_acc(0x1e3b);
  accent_circunder[uchar('L')] = mk_acc(0x1e3c);
  accent_circunder[uchar('l')] = mk_acc(0x1e3d);
  accent_acute[uchar('M')] = mk_acc(0x1e3e);
  accent_acute[uchar('m')] = mk_acc(0x1e3f);

  accent_dotabove[uchar('M')] = mk_acc(0x1e40);
  accent_dotabove[uchar('m')] = mk_acc(0x1e41);
  accent_dotunder[uchar('M')] = mk_acc(0x1e42);
  accent_dotunder[uchar('m')] = mk_acc(0x1e43);
  accent_dotabove[uchar('N')] = mk_acc(0x1e44);
  accent_dotabove[uchar('n')] = mk_acc(0x1e45);
  accent_dotunder[uchar('N')] = mk_acc(0x1e46);
  accent_dotunder[uchar('n')] = mk_acc(0x1e47);
  accent_barunder[uchar('N')] = mk_acc(0x1e48);
  accent_barunder[uchar('n')] = mk_acc(0x1e49);
  accent_circunder[uchar('N')] = mk_acc(0x1e4a);
  accent_circunder[uchar('n')] = mk_acc(0x1e4b);
  other_accent[Otildeaigu_cc] = mk_acc(0x1e4c);
  other_accent[otildeaigu_cc] = mk_acc(0x1e4d);
  other_accent[Otildedp_cc] = mk_acc(0x1e4e);
  other_accent[otildedp_cc] = mk_acc(0x1e4f);

  other_accent[Obargrave_cc] = mk_acc(0x1e50);
  other_accent[obargrave_cc] = mk_acc(0x1e51);
  other_accent[Obaraigu_cc] = mk_acc(0x1e52);
  other_accent[obaraigu_cc] = mk_acc(0x1e53);
  accent_acute[uchar('P')] = mk_acc(0x1e54);
  accent_acute[uchar('p')] = mk_acc(0x1e55);
  accent_dotabove[uchar('P')] = mk_acc(0x1e56);
  accent_dotabove[uchar('p')] = mk_acc(0x1e57);
  accent_dotabove[uchar('R')] = mk_acc(0x1e58);
  accent_dotabove[uchar('r')] = mk_acc(0x1e59);
  accent_dotunder[uchar('R')] = mk_acc(0x1e5a);
  accent_dotunder[uchar('r')] = mk_acc(0x1e5b);
  other_accent[Rdotbar_cc] = mk_acc(0x1e5c);
  other_accent[rdotbar_cc] = mk_acc(0x1e5d);
  accent_barunder[uchar('R')] = mk_acc(0x1e5e);
  accent_barunder[uchar('r')] = mk_acc(0x1e5f);

  accent_dotabove[uchar('S')] = mk_acc(0x1e60);
  accent_dotabove[uchar('s')] = mk_acc(0x1e61);
  accent_dotunder[uchar('S')] = mk_acc(0x1e62);
  accent_dotunder[uchar('s')] = mk_acc(0x1e63);
  other_accent[Sdotacute_cc] = mk_acc(0x1e64);
  other_accent[sdotacute_cc] = mk_acc(0x1e65);
  other_accent[Sdotcaron_cc] = mk_acc(0x1e66);
  other_accent[sdotcaron_cc] = mk_acc(0x1e67);
  other_accent[Sdotdot_cc] = mk_acc(0x1e68);
  other_accent[sdotdot_cc] = mk_acc(0x1e69);
  accent_dotabove[uchar('T')] = mk_acc(0x1e6a);
  accent_dotabove[uchar('t')] = mk_acc(0x1e6b);
  accent_dotunder[uchar('T')] = mk_acc(0x1e6c);
  accent_dotunder[uchar('t')] = mk_acc(0x1e6d);
  accent_barunder[uchar('T')] = mk_acc(0x1e6e);
  accent_barunder[uchar('t')] = mk_acc(0x1e6f);

  accent_circunder[uchar('T')] = mk_acc(0x1e70);
  accent_circunder[uchar('t')] = mk_acc(0x1e71);
  // 1e72 1e73 etrange
  accent_tildeunder[uchar('U')] = mk_acc(0x1e74);
  accent_tildeunder[uchar('u')] = mk_acc(0x1e75);
  accent_circunder[uchar('U')] = mk_acc(0x1e76);
  accent_circunder[uchar('u')] = mk_acc(0x1e77);
  other_accent[Utildeacute_cc] = mk_acc(0x1e78);
  other_accent[utildeacute_cc] = mk_acc(0x1e79);
  other_accent[Udiamacro_cc] = mk_acc(0x1e7a);
  other_accent[udiamacro_cc] = mk_acc(0x1e7b);
  accent_tilde[uchar('V')] = mk_acc(0x1e7c);
  accent_tilde[uchar('v')] = mk_acc(0x1e7d);
  accent_dotunder[uchar('V')] = mk_acc(0x1e7e);
  accent_dotunder[uchar('v')] = mk_acc(0x1e7f);

  accent_grave[uchar('W')] = mk_acc(0x1e80);
  accent_grave[uchar('w')] = mk_acc(0x1e81);
  accent_acute[uchar('W')] = mk_acc(0x1e82);
  accent_acute[uchar('w')] = mk_acc(0x1e83);
  accent_trema[uchar('W')] = mk_acc(0x1e84);
  accent_trema[uchar('w')] = mk_acc(0x1e85);
  accent_dotabove[uchar('W')] = mk_acc(0x1e86);
  accent_dotabove[uchar('w')] = mk_acc(0x1e87);
  accent_dotunder[uchar('W')] = mk_acc(0x1e88);
  accent_dotunder[uchar('w')] = mk_acc(0x1e89);
  accent_dotabove[uchar('X')] = mk_acc(0x1e8a);
  accent_dotabove[uchar('x')] = mk_acc(0x1e8b);
  accent_trema[uchar('X')] = mk_acc(0x1e8c);
  accent_trema[uchar('x')] = mk_acc(0x1e8d);
  accent_dotabove[uchar('Y')] = mk_acc(0x1e8e);
  accent_dotabove[uchar('y')] = mk_acc(0x1e8f);

  accent_cir[uchar('Z')] = mk_acc(0x1e90);
  accent_cir[uchar('z')] = mk_acc(0x1e91);
  accent_dotunder[uchar('Z')] = mk_acc(0x1e92);
  accent_dotunder[uchar('z')] = mk_acc(0x1e93);
  accent_barunder[uchar('Z')] = mk_acc(0x1e94);
  accent_barunder[uchar('z')] = mk_acc(0x1e95);
  accent_barunder[uchar('h')] = mk_acc(0x1e96);
  accent_trema[uchar('t')] = mk_acc(0x1e97);
  accent_ring[uchar('w')] = mk_acc(0x1e98);
  accent_ring[uchar('y')] = mk_acc(0x1e99);

  // hole
  accent_dotunder[uchar('A')] = mk_acc(0x1ea0);
  accent_dotunder[uchar('a')] = mk_acc(0x1ea1);
  accent_hook[uchar('A')] = mk_acc(0x1ea2);
  accent_hook[uchar('a')] = mk_acc(0x1ea3);
  other_accent[Aciracute_cc] = mk_acc(0x1ea4);
  other_accent[aciracute_cc] = mk_acc(0x1ea5);
  other_accent[Acirgrave_cc] = mk_acc(0x1ea6);
  other_accent[acirgrave_cc] = mk_acc(0x1ea7);
  other_accent[Acirxx_cc] = mk_acc(0x1ea8);
  other_accent[acirxx_cc] = mk_acc(0x1ea9);
  other_accent[Acirtilde_cc] = mk_acc(0x1eaa);
  other_accent[acirtilde_cc] = mk_acc(0x1eab);
  other_accent[Acirdot_cc] = mk_acc(0x1eac);
  other_accent[acirdot_cc] = mk_acc(0x1ead);
  other_accent[Abreveacute_cc] = mk_acc(0x1eae);
  other_accent[abreveacute_cc] = mk_acc(0x1eaf);

  other_accent[Abrevegrave_cc] = mk_acc(0x1eb0);
  other_accent[abrevegrave_cc] = mk_acc(0x1eb1);
  other_accent[Abrevexx_cc] = mk_acc(0x1eb2);
  other_accent[abrevexx_cc] = mk_acc(0x1eb3);
  other_accent[Abrevetilde_cc] = mk_acc(0x1eb4);
  other_accent[abrevetilde_cc] = mk_acc(0x1eb5);
  other_accent[Abrevedot_cc] = mk_acc(0x1eb6);
  other_accent[abrevedot_cc] = mk_acc(0x1eb7);
  accent_dotunder[uchar('E')] = mk_acc(0x1eb8);
  accent_dotunder[uchar('e')] = mk_acc(0x1eb9);
  accent_hook[uchar('E')] = mk_acc(0x1eba);
  accent_hook[uchar('e')] = mk_acc(0x1ebb);
  accent_tilde[uchar('E')] = mk_acc(0x1ebc);
  accent_tilde[uchar('e')] = mk_acc(0x1ebd);
  other_accent[Eciracute_cc] = mk_acc(0x1ebe);
  other_accent[eciracute_cc] = mk_acc(0x1ebf);

  other_accent[Ecirgrave_cc] = mk_acc(0x1ec0);
  other_accent[ecirgrave_cc] = mk_acc(0x1ec1);
  other_accent[Ecirxx_cc] = mk_acc(0x1ec2);
  other_accent[ecirxx_cc] = mk_acc(0x1ec3);
  other_accent[Ecirtilde_cc] = mk_acc(0x1ec4);
  other_accent[ecirtilde_cc] = mk_acc(0x1ec5);
  other_accent[Ecirdot_cc] = mk_acc(0x1ec6);
  other_accent[ecirdot_cc] = mk_acc(0x1ec7);
  accent_hook[uchar('I')] = mk_acc(0x1ec8);
  accent_hook[uchar('i')] = mk_acc(0x1ec9);
  accent_dotunder[uchar('I')] = mk_acc(0x1eca);
  accent_dotunder[uchar('i')] = mk_acc(0x1ecb);
  accent_dotunder[uchar('O')] = mk_acc(0x1ecc);
  accent_dotunder[uchar('o')] = mk_acc(0x1ecd);
  accent_hook[uchar('O')] = mk_acc(0x1ece);
  accent_hook[uchar('o')] = mk_acc(0x1ecf);

  other_accent[Ociracute_cc] = mk_acc(0x1ed0);
  other_accent[ociracute_cc] = mk_acc(0x1ed1);
  other_accent[Ocirgrave_cc] = mk_acc(0x1ed2);
  other_accent[ocirgrave_cc] = mk_acc(0x1ed3);
  other_accent[Ocirxx_cc] = mk_acc(0x1ed4);
  other_accent[ocirxx_cc] = mk_acc(0x1ed5);
  other_accent[Ocirtilde_cc] = mk_acc(0x1ed6);
  other_accent[ocirtilde_cc] = mk_acc(0x1ed7);
  other_accent[Ocirdot_cc] = mk_acc(0x1ed8);
  other_accent[ocirdot_cc] = mk_acc(0x1ed9);
  other_accent[Oacuteyy_cc] = mk_acc(0x1eda);
  other_accent[oacuteyy_cc] = mk_acc(0x1edb);
  other_accent[Ograveyy_cc] = mk_acc(0x1edc);
  other_accent[ograveyy_cc] = mk_acc(0x1edd);
  other_accent[Oxxyy_cc] = mk_acc(0x1ede);
  other_accent[oxxyy_cc] = mk_acc(0x1edf);

  other_accent[Otildeyy_cc] = mk_acc(0x1ee0);
  other_accent[otildeyy_cc] = mk_acc(0x1ee1);
  other_accent[Odotyy_cc] = mk_acc(0x1ee2);
  other_accent[odotyy_cc] = mk_acc(0x1ee3);
  accent_dotunder[uchar('U')] = mk_acc(0x1ee4);
  accent_dotunder[uchar('u')] = mk_acc(0x1ee5);
  accent_hook[uchar('U')] = mk_acc(0x1ee6);
  accent_hook[uchar('u')] = mk_acc(0x1ee7);
  other_accent[Uacuteyy_cc] = mk_acc(0x1ee8);
  other_accent[uacuteyy_cc] = mk_acc(0x1ee9);
  other_accent[Ugraveyy_cc] = mk_acc(0x1eea);
  other_accent[ugraveyy_cc] = mk_acc(0x1eeb);
  other_accent[Uxxyy_cc] = mk_acc(0x1eec);
  other_accent[uxxyy_cc] = mk_acc(0x1eed);
  other_accent[Utildeyy_cc] = mk_acc(0x1eee);
  other_accent[utildeyy_cc] = mk_acc(0x1eef);

  other_accent[Udotyy_cc] = mk_acc(0x1ef0);
  other_accent[udotyy_cc] = mk_acc(0x1ef1);
  accent_grave[uchar('Y')] = mk_acc(0x1ef2);
  accent_grave[uchar('y')] = mk_acc(0x1ef3);
  accent_dotunder[uchar('Y')] = mk_acc(0x1ef4);
  accent_dotunder[uchar('y')] = mk_acc(0x1ef5);
  accent_hook[uchar('Y')] = mk_acc(0x1ef6);
  accent_hook[uchar('y')] = mk_acc(0x1ef7);
  accent_tilde[uchar('Y')] = mk_acc(0x1ef8);
  accent_tilde[uchar('y')] = mk_acc(0x1ef9);

  other_accent[unused_accent_even_cc] = Token(0);
  other_accent[unused_accent_odd_cc] = Token(0);
  special_double[0] = the_parser.hash_table.locate("textsubgrave");
  special_double[1] = the_parser.hash_table.locate("textgravedot");
  special_double[2] = the_parser.hash_table.locate("textsubacute");
  special_double[3] = the_parser.hash_table.locate("textacutemacron");
  special_double[4] = the_parser.hash_table.locate("textsubcircum");
  special_double[5] = the_parser.hash_table.locate("textcircumdot");
  special_double[6] = the_parser.hash_table.locate("textsubtilde");
  special_double[7] = the_parser.hash_table.locate("texttildedot");
  special_double[8] = the_parser.hash_table.locate("textsubumlaut");
  special_double[9] = the_parser.hash_table.locate("textdoublegrave");
  special_double[10] = the_parser.hash_table.locate("textsubring");
  special_double[11] = the_parser.hash_table.locate("textringmacron");
  special_double[12] = the_parser.hash_table.locate("textsubwedge");
  special_double[13] = the_parser.hash_table.locate("textacutewedge");
  special_double[14] = the_parser.hash_table.locate("textsubarch");
  special_double[15] = the_parser.hash_table.locate("textbrevemacron");
  special_double[16] = the_parser.hash_table.locate("textsubbar");
  special_double[17] = the_parser.hash_table.locate("textsubdot");
  special_double[18] = the_parser.hash_table.locate("textdotacute");
  special_double[19] = the_parser.hash_table.locate("textbottomtiebar");
}
