// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Post processing for tralics
#include "tralics.h"
#include "txtrees.h"
#include "txpost.h"


const char* txpost_rcsid= 
	    "$Id: txpost.C,v 2.80 2015/11/18 17:58:11 grimm Exp $";

namespace {
  Buffer scbuf; // scratch buffer for printing XML, and other things
  vector<pair<int,Istring> > ref_list;   // list of all \ref
  vector<pair<int,Istring> > refindex_list;   // list of all \ref to \index
  vector<pair<Istring,LabelInfo*> > defined_labels; // list of all \label
  vector<pair<String,Istring> > removed_labels; // list of all \label removed
  ostream* cur_fp; // the XML file

  int last_label_id = 0;
  int last_top_label_id = 0;
}

namespace post_ns {
  void remove_label(String s, Istring n);
  void remove_me_from_heads(Xmlp X);
  void print_no_title(int i, String s);
  auto is_entity(String s) -> int;
  void raw_subfigure(Xmlp from,Xmlp to,Xmlp junk);
  void table_subfigure(Xmlp from,Xmlp to,Xmlp junk);
  auto figline(Xmlp from, int &ctr, Xmlp junk) -> Xmlp;
  void postprocess_table(Xmlp to,Xmlp from);
  void postprocess_figure(Xmlp to,Xmlp from);
}


// For finding words.
namespace all_words_ns {
  int nb_words = 0;
  WordList* WL0[100];
  fstream* fp = 0;
  void add_a_word(String s, int h);
  void dump_and_list(WordList*,int i);
  void dump_words(string name);
  auto is_entity(String s) -> int;
}

//  --- Manipulating the XML tree

// return the true size, excluding null elements
auto Xml::real_size() const -> int {
  if(is_xmlc()) return -2;
  int n = 0;
  int k = tree.size();
  for (int i=0; i < k; i++) if(tree[i]) n ++;
  return n;
}

// returns the value at n, excluding null elements
auto Xml::value_at(int n) -> Xmlp {
  if(is_xmlc() || n < 0) return 0;
  int k = tree.size();
  for (int i=0; i < k; i++) {
    if(tree[i]) {
      if(n==0) return tree[i];
      n --;
    }
  }
  return 0;
}

auto Xml::put_at(int n, Xmlp x) -> bool {
  if(is_xmlc()) return false;
  int k = tree.size();
  for (int i=0; i < k; i++) {
    if(tree[i]) {
      if(n==0) {tree[i] = x; return true; }
      n --;
    }
  }
  return false;
}

// returns the value at n, excluding null elements, returns true if OK
auto Xml::remove_at(int n) -> bool {
  if(is_xmlc()) return false;
  int k = tree.size();
  for (int i=0; i < k; i++) {
    if(tree[i]) {
      if(n==0) { tree[i] = 0; return true; }
      n --;
    }
  }
  return false;
}

// inserts a position n, excluding spaces
// returns true if OK
auto Xml::insert_at_ck(int n, Xmlp v) -> bool {
  if(is_xmlc()) return false;
  int k = tree.size();
  for (int i=0; i < k; i++) { 
    if(n==0) { insert_at(i,v); return true; }
    if(tree[i]) n --;
    }
  if(n==1) { tree.push_back(v); return true; }
  return false;
}

auto Xml::is_child(Xmlp x) const -> bool {
  if(is_xmlc()) return false;
  int k = tree.size();
  for (int i=0; i < k; i++) 
    if(tree[i] ==x) return true;
  return false;
}

void Parser::user_XML_swap (subtypes c)
{
  if(c==zero_code) the_xmlB = the_xmlA;
  else if(c==one_code) the_xmlA = the_xmlB;
  else {
    Xmlp c = the_xmlA;
    the_xmlA = the_xmlB;
    the_xmlB = c;
  }
}

auto Xml::deep_copy() -> Xmlp {
  if(is_xmlc()) return this;
  Xmlp res = new Xml (name,0);
  res-> id.add_attribute(id);
  int n = size();
  for(int i=0; i< n; i++) {
    if(tree[i]) res -> push_back(tree[i]->deep_copy());
  }
  return res;
}

// puts in the_xmlA the element whose id is the argument of the command
void Parser::user_XML_fetch()
{
  Token T = cur_tok;
  int n = scan_int(T);
  the_xmlA = the_stack.fetch_by_id(n);
}

void Parser::user_XML_modify(subtypes c)
{
  if(c == xml_setA_code) { scan_box(xmlA_location); return; }
  if(c == xml_setB_code) { scan_box(xmlB_location); return; }
  if(c == xml_parent_code) {
    the_xmlB = the_stack.find_parent(the_xmlA);
    return;
  }
  int n = scan_int (cur_tok);
  switch(c) {
  case xml_get_code:
    the_xmlB = the_xmlA ? the_xmlA -> value_at(n) : 0;
    return;
  case xml_ins_code: 
    if (!the_xmlA || n<0) return;
    the_xmlA-> insert_at(n,the_xmlB? the_xmlB->deep_copy() : 0);
    return;
  case xml_del_code:
    if (!the_xmlA || n<0) return;
    the_xmlA-> remove_at(n);
    return;
  case xml_set_code: // how to tell the user that it failed ?
    if (!the_xmlA || n<0) return;
    the_xmlA-> put_at(n,the_xmlB? the_xmlB->deep_copy() : 0);
    return;
  default: return;
  }
}


// ----

// Implementation of \label, \ref
// This is assumed to create a unique ID.
auto StrHash::next_label_id() -> Istring {
  last_label_id++;
  mybuf << bf_reset << "uid" << last_label_id;
  return Istring(hash_find());
}

// Implementation of \label, \ref
// This is assumed to create a unique ID.
auto StrHash::next_top_label_id() -> Istring {
  last_top_label_id++;
  mybuf << bf_reset << "cid" << last_top_label_id;
  return Istring(hash_find());
}

auto StrHash::lab_val_check(Istring k) -> LabelInfo * {
  int K = k.get_value();
  if(!Labinfo[K]) Labinfo[K] = new LabelInfo(k);
  return Labinfo[K];
}

// This implements \label{foo}; second argument is the anchor id
// We enter foo in the hashtab, and look at the LabelInfo value.
// If the label is undefined, we define it, 

void Parser::create_label(const string& X, Istring S)
{
  Istring m = Istring(X);
  LabelInfo* V = the_main->SH.lab_val_check(m);
  if(V->set_defined()) {
    multiple_label(m.c_str(), V->get_lineno(),V->get_filename());
  } else {
    my_stats.one_more_label();
    V->set_id(S);
    V->set_lineno(get_cur_line());
    V->set_filename(get_cur_filename());
    defined_labels.push_back(make_pair(S,V));
  }
}

// Implementation of \ref{foo}. We enter foo in the hashtab.
// and create/update the LabelInfo. We remember the ref in the ref_list.
void Xid::add_ref(string s)
{
  tralics_ns::add_ref(value,s,false);
}

void tralics_ns::add_ref(int v, const string& s, bool idx)
{
  the_parser.my_stats.one_more_ref();
  Istring B = Istring(s);
  if(idx) 
    refindex_list.push_back(make_pair(v,B));
  else 
    ref_list.push_back(make_pair(v,B));
  LabelInfo* V = the_main->SH.lab_val_check(B);
  if(!V->set_used()) the_parser.my_stats.one_more_used_ref();
  if(!V->get_lineno()) V->set_lineno(the_parser.get_cur_line());
  if(V->get_filename().empty()) V->set_filename(the_parser.get_cur_filename());
}


// In ref_list, we have  (e,v), (e,v), (e,v) etc
// where E is the xid of a <ref> element, and V is an entry in the
// hash table of the label. After the translation is complete,
// we know the value of the label, and can add the attribute target=xxx.
void Parser::check_all_ids()
{
  for(unsigned int i=0;i<ref_list.size();i++) {
    int E = ref_list[i].first;
    Istring V = ref_list[i].second;
    LabelInfo*L = the_main->SH.lab_val(V);
    if(!L->is_defined()) { 
      main_ns::log_and_tty << lg_start
		  << "Error signaled in postprocessor\n"
		  << "undefined label `" << V 
		  << "' (first use at line " << L->get_lineno() 
		  << " in file " << L->get_filename() << ")";
      Istring B = L->get_id();
      for(unsigned int j=0;j<removed_labels.size(); j++) {
	if(removed_labels[j].second==B)
	  main_ns::log_and_tty << "\n(Label was removed with `" 
		      << removed_labels[j].first << "')";
	break;
      }
      main_ns::log_and_tty << "\n";
      main_ns::nb_errs++;
    }
    Istring B = L->get_id();
    Xid(E).add_attribute(the_names[np_target],B);
  }
}

// 
void tralics_ns::find_index_labels(vector<string>& W)
{
  for(unsigned int i=0;i<refindex_list.size();i++) {
    int E = refindex_list[i].first;
    Istring V = refindex_list[i].second;
    LabelInfo*L = the_main->SH.lab_val(V);
    if(!L->is_defined()) continue; // should not happen
    Istring B = L->get_id();
    scbuf.reset();
    scbuf.push_back(W[E]);
    if(!scbuf.empty()) scbuf.push_back(' ');
    scbuf.push_back(B);
    W[E] = scbuf.to_string();
  }
}

// This removes the object S, together with the label n
void post_ns::remove_label(String s, Istring n)
{
  for(unsigned int i=0;i<ref_list.size();i++) {
    Istring V = ref_list[i].second;
    LabelInfo* li = the_main->SH.lab_val(V);
    if(li->get_id() != n) continue;
    if(!li->is_used()) continue;
    main_ns::log_and_tty << "Error signaled by postprocessor\n"
		<< "Removing `"<< s << "' made the following label disappear: " 
		<< V << "\n";
    main_ns::nb_errs++;
  }
  for(unsigned int i=0;i<defined_labels.size();i++) {
    Istring j = defined_labels[i].first;
    LabelInfo* V = defined_labels[i].second;
    if(j==n && V->is_defined()  && !V->is_used()) {
      removed_labels.push_back(make_pair(s,n));
      V->set_undefined();
    }
  }
}

// This removes X from the list of all heads.
void post_ns::remove_me_from_heads(Xmlp X)
{
  int j = the_parser.all_heads.size();
  for(int i=0;i<j;i++)
    if(the_parser.all_heads[i] == X) 
      the_parser.all_heads[i] = 0;
}


// True if the string holds only white space
auto Istring::only_space() const -> bool {
  const string& s = the_main->SH[value];
  int i = 0;
  int l = s.length();
  while(i<l) {
    if(is_space(s[i])) i++;
    else if(uchar(s[i])==194 && i+1<l &&uchar(s[i+1])==160) i += 2;
    else return false;
  } 
  return true;
}

// True if the string holds only white space or &nbsp;
auto Istring::only_space_spec() const -> bool {
  int i = 0;
  String s = c_str();
  while(s[i]) {
    if(is_space(s[i])) i++;
    else if(strncmp(s+i,"&nbsp;",6) == 0) i+=6;
    else if(strncmp(s+i,"&#xA0;",6) == 0) i+=6; 
    else if(strncmp(s+i,"\302\240",2) == 0) i+=6; 
    else return false;
  }
  return true;
}

// This finds a sub element named match, and does some action X
// Recursion stops when found.
void Xml::recurse0(XmlAction& X)
{
  int tl = size();
  for(int k=0;k<tl;k++) {
    Xmlp y = tree[k];
    if(!y || y->is_xmlc()) continue;
    if(y->has_name(X.get_match()))
      switch(X.get_what()) {
      case rc_contains: 
	X.mark_found();
	return;
      case rc_delete_first: 
	X.set_int_val(y->get_id().value);
	tree[k] = 0;
	return;
      case rc_return_first: 
	X.set_xml_val(y);
	X.mark_found();
	return;
      case rc_return_first_and_kill: 
	X.set_xml_val(y);
	tree[k] = 0;
	X.mark_found();
	return;
      default:
	main_ns::log_and_tty <<"illegal value in recurse0\n" << lg_fatal;
	abort();
      }
    y->recurse0(X);
    if(X.is_ok()) 
      return;
  }
}

// This does some action for every element named X.
void Xml::recurse(XmlAction& X)
{
  int len = size();
  for(int k=0;k<len;k++) {
    Xmlp T = tree[k];
    if(!T || T->is_xmlc()) continue;
    if(T->has_name(X.get_match())) {
      switch(X.get_what()) {
      case rc_delete:  
	tree[k] = 0;
	continue;
      case rc_delete_empty: 
	if(T->is_empty()) { 
	  tree[k] = 0;
	  continue;
	}
	break;
      case rc_empty_par: 
	if(T->is_empty_spec()) { 
	  tree[k] = 0;
	  continue;
	}
	break;
      case rc_how_many:
	X.incr_int_val();
	break;
      case rc_subst: 
	tree[k] = X.get_xml_val();
	continue;
      case rc_move: 
	X.get_xml_val()->push_back(T);
	tree[k] =0;
	continue;
      case rc_composition: // a module in the comp section
	{
	  Istring an = T->get_id().has_attribute(the_names[np_id]);
	  if(!an.null()) post_ns::remove_label("module in composition",an);
	  tree[k] = 0;
	  k--;
	  int Len = T->size();
	  for(int j = 0; j<Len;j++) {
	    Xmlp W = T->tree[j];
	    if(!W) continue;
	    if(!W->is_xmlc() && W->has_name(np_head)) {
	      post_ns::remove_me_from_heads(W);
	      continue;
	    }
	    insert_at(k+1,W);
	    k++;
	  }
	  recurse(X); // this->tree has changed
	  return; // nothing more to do.
	}
      case rc_rename:
	T->change_name(X.get_string_val()); 
	break;
      default:
	main_ns::log_and_tty <<"illegal value in recurse\n" << lg_fatal; 
	abort();
      }
    }
    T->recurse(X);
  }
}

// Should we use insert here ?
void Xml::make_hole(int pos)
{
  int l =size();
  tree.push_back(0);
  for(int i=l;i>pos;i--)
    tree[i] = tree[i-1];
}

// Returns a pointer to the last son.
auto Xml::last_addr() const -> Xmlp {
  int len = size();
  return len>0 ? tree[len-1] : 0;
}

// Returns a pointer to the last element 
// and removes it if it is a Box
auto Xml::last_box() -> Xmlp {
  int len = size();
  if(len==0) return 0;
  Xmlp res = tree[len-1];
  if(res && !res->is_xmlc()) {
    tree.pop_back();
    return res;
  }
  else return 0;
}

// Remove last item if its an empty <hi>
void Xml::remove_last_empty_hi()
{
  int len = size();
  if(len==0) return;
  Xmlp T = tree[len-1];
  if(!T || T->is_xmlc()) return;
  if(!T->only_recur_hi()) return;
  tree.pop_back();
}

// Return true if this is an empty <hi> (recursion allowed)
auto Xml::only_recur_hi() const -> bool {
  if(!id.is_font_change()) return false;
  if(size()==0) return true;
  Xmlp x = single_son();
  if(!x) return false;
  if(x->is_xmlc()) return false;
  return x->only_recur_hi();
}

// Return true if this contains only <hi>
// Exemple \mbox{\tt x} gives {\tt x}
auto Xml::only_hi() const -> bool {
  Xmlp x = single_non_empty();
  if(!x) return false;
  if(x->is_xmlc()) return false;
  if(x->id.is_font_change()) return true;
  return false;
}

// Adds the content of x to this. Attributes are lost
// Assumes that x is not a string.
void Xml::push_back_list(Xmlp x)
{ 
  int n = x->size();
  for(int i=0;i<n;i++) {
    Xmlp y = x->tree[i];
    if(y) tree.push_back(y);
  }
}

// Insert X at the end; but the value if it is a temporary.
void Xml::add_tmp(Xmlp x)
{
  if(!x) return; // trivial case
  if(!x->is_xmlc() && x->has_name(cst_temporary))
    push_back_list(x);
  else 
    tree.push_back(x);
}

// Inserts x at position pos.
void Xml::insert_at(int pos, Xmlp x)
{
  make_hole(pos);
  tree[pos] = x;
}

// Inserts x at position 0.
void Xml::add_first(Xmlp x)
{
  make_hole(0);
  tree[0] = x;
}

// This find an element with a single son, the son should be res.
auto Xml::find_on_tree(Xmlp check, Xmlp *res) const -> bool {
  int n = size();
  for(int i=0; i<n;i++) {
    Xmlp T = tree[i];
    if(T && !T->is_xmlc() && T->size() == 1 && T->tree[0]==check)
      { *res = T ; return true; }
  }
  for(int i=0; i<n; i++)
    if(tree[i] && !tree[i]->is_xmlc() && tree[i]-> find_on_tree(check,res))
      return true;
  return false;
}

void Xml::insert_bib(Xmlp bib, Xmlp match)
{
  Xmlp* ptr = new Xml*(this);
  if(match) find_on_tree(match, ptr);
  ptr[0]->add_tmp(bib);
}

// Puts element T with its attribute list in the buffer.
// If w=0, we print <foo att-list/>
// If w=1, we print <foo att-list>
// if w=2, we print </foo>
void Buffer::push_back_elt(Istring name, Xid id,int w)
{
  push_back('<');
  if(w==2) push_back('/');
  push_back(name.p_str());
  if(w!=2) push_back(id.get_att());
  if(w==0) push_back ('/');
  push_back('>');
}

// int www = 0; debug: finding a cycle in the XML tree

// This puts element T, with its value in the buffer.
// If non trivial, the buffer is flushed (printed on the file)

void Xml::to_buffer (Buffer&b) const
{
  if(is_xmlc()) {
    if(id.value==0) b << name;
    else if(id.value==-1) b << "<!--" << name << "-->";
    else if(id.value==-2) {
      int len = size();
      b << "<!" << name;
      for(int i=0;i<len;i++) {
	if(tree[i]) 
	  tree[i]->to_buffer(b);
      }     
      b << ">";
      b.finish_xml_print();
    } else if(id.value==-3) b << "<?" << name << "?>";
    return;
  }
  int len = size();
  int rlen = 0;
  for(int i=0;i<len;i++) if(tree[i]) rlen++;
  if(name.get_value()>1) {
    if(rlen) b.push_back_elt(name,id,1);
    else {
      b.push_back_elt(name,id,0); // case of <foo/>
      return;
    }
  }
  //  www ++; if (www <0) b << "...loop ... "; else
  for(int i=0;i<len;i++) {
    if(tree[i]) 
      tree[i]->to_buffer(b);
  }
  //  www --;
  if(name.get_value()>1) 
    b.push_back_elt(name,id,2);
  b.finish_xml_print();
}

// This prints T on the file fp, using scbuf.
auto operator<<(ostream &fp, const Xml *T) -> ostream & {
  scbuf.reset();
  cur_fp = &fp;
  if(T)  T->to_buffer(scbuf); else scbuf << "</>";
  scbuf.finish_xml_print();
  return fp;
}

// This flushed the buffer, increments cur_fp_len.
void Buffer::finish_xml_print()
{
  *cur_fp << buf;
  the_main->incr_cur_fp_len(wptr);
#if defined(WINNT) || defined(__CYGWIN__) || defined(_WIN32)
  int k =0;
  for (int i=0;i<wptr;i++)
    if(buf[i]=='\n') k++;
  the_main->incr_cur_fp_len(k);
#endif
  reset();
}

// Replace <name/> by vl.
void Xml::subst_env0(Istring match, Xmlp vl)
{
  XmlAction X(match,rc_subst,vl);
  recurse(X);
}


// Returns number of sons named <match>.
auto Xml::how_many_env(Istring match) -> int {
  XmlAction X(match,rc_how_many);
  recurse(X);
  return X.get_int_val();
}

// Removes and returns first element named N.
auto Xml::get_first_env(name_positions N) -> Xmlp {
  XmlAction X(the_names[N],rc_return_first_and_kill);
  recurse0(X);
  return X.get_xml_val();
}

// Returns the element that is just before x.
auto Xml::prev_sibling(Xmlp x) -> Xmlp {
  int len = size();
  for(int i=1;i<len;i++)
    if(tree[i]==x) return tree[i-1];
  return 0;
}

// This returns true if it is possible to remove the p
auto Xml::is_empty_p() const -> bool {
  if(!has_name(cst_p)) return false;
  int len = size();
  for(int k=0;k<len;k++) {
    Xmlp T = tree[k];
    if(!T) continue;
    return false;
  }
  AttList& X = id.get_att();
  if(X.empty()) return true;
  if(X.val.size() !=1) return false;
  if(X.val.front().name==np_noindent) return true;
  return false;
}

// Returns true if empty
auto Xml::empty() const -> bool {
  if(!name.empty()) return false;
  int len = size();
  for(int k=0;k<len;k++) {
    if(tree[k]) return false;
  }
  return true;
}

// Returns true if empty (white space only)
auto Xml::is_empty() const -> bool {
  int len = size();
  for(int k=0;k<len;k++) {
    Xmlp T = tree[k];
    if(!T) continue;
    if(!T->is_xmlc()) return false;
    if(!T->get_name().only_space()) return false;
  }
  return true;
}

// Returns true if empty (white space or &nbsp; only)
auto Xml::is_empty_spec() const -> bool {
  int len = size();
  for(int k=0;k<len;k++) {
    Xmlp T = tree[k];
    if(!T) continue;
    if(!T->is_xmlc()) return false;
    if(!T->get_name().only_space_spec()) return false;
  }
  return true;
}

// If there is one non-empty son returns it.
auto Xml::single_non_empty() const -> Xmlp {
  int len = size();
  Xmlp res = 0;
  for(int k=0;k<len;k++) {
    Xmlp y = tree[k];
    if(!y) continue;
    if(!y->is_xmlc()) 
      { if (!res) res = y; else return 0; }
    else if(!y->get_name().only_space()) return 0;
  }
  return res;
}

// Returns true if text only.
auto Xml::only_text() const -> bool {
  int len = size();
  for(int k=0;k<len;k++) {
    Xmlp y = tree[k];
    if(!y) continue;
    if(!y->is_xmlc()) return false;
  }
  return true;
}

// Returns the only son, or 0 if more than one sons.
auto Xml::single_son() const -> Xmlp {
  int len = size();
  Xmlp res = 0;
  for(int k=0;k<len;k++) {
    Xmlp y = tree[k];
    if(!y) continue;
    if(!res) res = y;
    else return 0;
  }
  return res;
}

// Removes empty <p> elements
void Xml::remove_empty_par()
{
  XmlAction X (cst_p,rc_delete_empty);
  recurse(X);
}

// This swaps the trees of this and x
void Xml::swap_x(Xmlp x)
{
  vector<Xmlp> T = tree;
  tree = x->tree;
  x->tree = T;
}

// Moves to res every son named match.
void Xml::move(Istring match, Xmlp res)
{
  XmlAction X(match,rc_move,res);
  recurse(X);
}

// Renames all elements called old_name to new_name
void Xml::rename(Istring old_name, Istring new_name)
{
  XmlAction X(old_name,rc_rename,new_name);
  recurse(X);
}

// This is used to implement \unhbox
void Xml::unbox(Xmlp x)
{
  if(!x) return;
  if(x->is_xmlc()) {
    Buffer& b = scbuf;
    b.reset();
    b.push_back(x->get_name());
    add_last_string(b);
  } else push_back_list(x);
}

// Replaces <foo><p>a b c</p></foo> by <foo> a b c </foo>
void Xml::remove_par_bal_if_ok()
{
  Xmlp res = single_non_empty(); 
  if(res && !res->is_xmlc()&& res->has_name(cst_p)) {
    tree = res -> tree; res -> tree.clear ();
    }
}

// Post processor of figure or table.
void Xml::postprocess_fig_table(bool is_fig)
{
  // First copy this into a temporarry
  Xmlp T = new Xml(cst_temporary,0);
  swap_x(T);
  // move the caption from T to this
  Xmlp C = T->get_first_env(np_caption);
  if(C) {
    tree.push_back(C); // copy caption
    C->change_name(np_captions);
    tree.push_back(the_main->the_stack->newline_xml);
  }
  C = T->get_first_env(np_alt_caption);
  if(C) {
    tree.push_back(C);
    tree.push_back(the_main->the_stack->newline_xml);
  }

  // Move all data from T to this
  if(is_fig) post_ns::postprocess_figure(this,T);
  else  post_ns::postprocess_table(this,T);
  // test for junk in T
  T->remove_empty_par();
  T->remove_par_bal_if_ok();
  if(T->is_empty()) return;
  main_ns::log_and_tty << lg_start << "Warning: junk in " 
	      << (is_fig ? "figure" : "table") << "\n";
  {
    int n =  the_parser.get_cur_line();
    string f =  the_parser.get_cur_filename();
    main_ns::log_and_tty << "detected on line " << n;
    if(!f.empty()) main_ns::log_and_tty << " of file " << f ;
    main_ns::log_and_tty << ".\n"; 
  } 
  Xmlp U = new Xml(Istring("unexpected"),0);
  push_back(U);
  T->add_non_empty_to(U);
}

// Post processor of figure.
void post_ns::postprocess_figure(Xmlp to,Xmlp from)
{
  Xmlp X;
  XmlAction X1(np_table,rc_contains);
  XmlAction X2(np_subfigure,rc_contains);
  XmlAction X3(np_figure,rc_how_many);
  XmlAction X4(np_pre,rc_contains);
  from->recurse0(X1);
  from->recurse0(X2);
  from->recurse(X3);
  from->recurse0(X4);
  int w=4;
  if(X1.is_ok()) w=1;
  else if(X2.is_ok()) w = 2;
  else if(X4.is_ok()) w = 3;
  else if(X3.get_int_val()==1) w = 0;
  switch(w) {
  case 0:  // a single figure 
    X = from->get_first_env(np_figure);
    if(X) { // copy all atributes of X but rend in this
      to->get_id().add_attribute_but_rend(X->get_id());
    }
    return;
  case 1: // a table in the figure, move all tables
    X = new Xml(cst_p,0);
    to->push_back(X);
    from->move(the_names[np_table],X);
    to->get_id().add_attribute(np_rend,np_array);
    return;
  case 3:  // verbatim material in the figure; move all lines
    X = new Xml(cst_empty,0);
    to->push_back(X);
    from->move(the_names[np_pre],X);
    to->get_id().add_attribute(np_rend,np_pre);
    return;
  case 2:  // a subfigure
    //    T->remove_empty_par();
    X = new Xml(cst_p,0); // will contain junk
    if(the_parser.eqtb_int_table[use_subfigure_code].get_val())
      post_ns::raw_subfigure(from,to,X);
    else post_ns::table_subfigure(from,to,X);
    from->add_non_empty_to(X);
    from->swap_x(X);
    return;
  default: {  // other cases
    from->remove_empty_par();    
    Xmlp nbsp = new Xml(Istring(" &#xA0;"));
    from->subst_env0(the_names[np_hfill],nbsp);
    from->subst_env0(the_names[np_hfil],nbsp);
    from->move(the_names[cst_p],to);
    XmlAction X5(np_figure,rc_return_first);
    from->recurse0(X5);
    if(X5.get_xml_val())  // dommage
      from->add_non_empty_to(to);
  }
  }
}

// Post processor table.

void post_ns::postprocess_table(Xmlp to, Xmlp from)
{ 
  XmlAction X1(np_table,rc_how_many);
  from->recurse(X1);
  // Special case: more than one tabular in the table
  // We move in to all tabular
  if(X1.get_int_val()>1) {
    Xmlp X = new Xml(cst_p,0);
    to->push_back(X);
    from->move(the_names[np_table],X);
    to->get_id().add_attribute(np_rend,np_array);
    return;
  }
  // Normal case
  from->remove_empty_par();
  from->remove_par_bal_if_ok();
  to->get_id().add_attribute(np_rend,np_display);
  Xmlp C = from->single_non_empty();
  if(C && !C->is_xmlc()) {
    if(C->has_name(np_figure)) {
      to->push_back(C);
      from->reset();
    } else if(C->has_name(np_formula)) {
      to->push_back(C);
      from->reset();
    } else if(C->has_name(np_table)) {
      if(C->size()>0)
	to->push_back_list(C);
      else to->push_back(C); // This is strange...
      to ->get_id().add_attribute_but_rend(C ->get_id());
      from->reset();
      to->change_name(np_table);
    }
  }
}

void post_ns::table_subfigure(Xmlp from,Xmlp to,Xmlp junk)
{
  to->get_id().add_attribute(np_rend,np_array);
  int ctr = 'a';
  for(;;) {
    Xmlp sf = from->get_first_env(cst_p);
    if(!sf)  break;
    if(sf->is_xmlc()) junk->push_back(sf);
    else {
      Xmlp res1 = figline(sf,ctr,junk);
      if(res1) to->push_back(res1);
    }
  }
}

// Figure with subfigure. We construct a table with two rows
// for a par. ctr holds the value of the counter for the caption.
auto post_ns::figline(Xmlp from, int &ctr, Xmlp junk) -> Xmlp {
  Xmlp row1 = new Xml(np_row,0);
  Xmlp row2 = new Xml(np_row,0);
  int nrows = 0;
  for(;;) {
    Xmlp sf = from->get_first_env(np_subfigure);
    if(!sf)  break;
    nrows ++;
    if(sf->is_xmlc()) {
      junk->push_back(sf);
      continue;
    }
    Xmlp leg = sf->get_first_env(np_leg);
    Xmlp texte = sf->get_first_env(np_texte);
    
    sf->add_non_empty_to(junk);
    if(texte) {
      texte->change_name(np_cell);
      row1->push_back(texte);
      texte->get_id().add_attribute(sf->get_id());
    }
    if(leg) {
      leg->change_name(np_cell);
      Buffer& B = the_main->SH.shbuf();
      B << bf_reset << '(' << uchar(ctr) << ')' << ' ';
      leg->add_first(new Xml(the_main->SH));
      row2->push_back(leg);
    }
    ctr ++;
  }
  from->add_non_empty_to(junk);
  if(!nrows) return 0;
  Xmlp res = new Xml (np_table,0);
  res->get_id().add_attribute(np_rend,np_inline);
  res->push_back(row1);
  res->push_back(row2);
  return new Xml(cst_p,res);
}

void post_ns::raw_subfigure(Xmlp from,Xmlp to,Xmlp junk)
{
  to->get_id().add_attribute(np_rend,np_subfigure);
  int n = 0;
  static Istring parid_name = Istring("parid");
  for(;;) {
    Xmlp P = from->get_first_env(cst_p);
    if(!P)  break;
    if(P->is_xmlc()) { junk->push_back(P); continue; }
    scbuf << bf_reset << n;
    Istring par_id = Istring(scbuf);
    ++n;
    for(;;) {
      Xmlp sf = P->get_first_env(np_subfigure);
      if(!sf) { P->add_non_empty_to(junk); break; }
      if(sf->is_xmlc()) {
	junk->push_back(sf);
	continue;
      }
      sf->get_id().add_attribute(parid_name,par_id);
      Xmlp leg = sf->get_first_env(np_leg);
      Xmlp texte = sf->get_first_env(np_texte);
      sf->add_non_empty_to(junk); 
      if(leg) {
	leg->change_name(np_head);
	sf->push_back(leg);
      }
      if(texte) {
	Xmlp xx = texte->get_first_env(np_figure);
	if(xx) { 
	  sf->get_id().add_attribute_but_rend(xx->get_id());
	}
	texte->add_non_empty_to(junk); 
      }
      to->push_back(sf);
      to->push_back(the_main->the_stack->newline_xml);
    }
  }
}


// Adds all non-empty elements to res
void Xml::add_non_empty_to(Xmlp res)
{
  int len = size();
  for(int k=0;k<len;k++) {
    Xmlp T = tree[k];
    if(!T) continue;
    if(T->is_xmlc() && T->get_name().only_space()) continue;
    res->push_back(T);
  }
}

// Kills the tree.
void Xml::reset()
{
  tree.erase(tree.begin(), tree.end());
}


// Postprocessor for <composition>
void Xml::compo_special()
{
  XmlAction X (np_module,rc_composition);
  recurse(X);
}


// This is used by sT_translate. It converts an XML element
// to a string, using scbuf as temporary. clears the object
auto Xml::convert_to_string() -> string {
  scbuf.reset();
  convert_to_string(scbuf);
  tree.clear();
  return scbuf.to_string();
}

// This converts the content to a string. May be recursive
void Xml::convert_to_string(Buffer& buf)
{
  if(is_xmlc()) {
    buf << name.c_str();
    return;
  }
  if(empty_name() ||name==the_names[cst_temporary]) {
    int len = size();
    for(int k = 0; k<len;k++) 
      tree[k]->convert_to_string(buf);
    return;
  }
  err_ns::local_buf.reset();
  if(id.is_font_change()) {
    Istring w = get_id().has_attribute(the_names[np_rend]);
    if(!w.null()) {
      err_ns::local_buf <<  "unexpected font change " << w;
      the_parser.unexpected_font(); 
      the_parser.signal_error();
      return;
    }
  }
  err_ns::local_buf << "unexpected element " << name;
  the_parser.signal_error();
}

// Puts *this in the buffer B. Uses Internal Encoding
// Used to print the title of a section.
void Xml::put_in_buffer(Buffer& b)
{
  int len = size();
  for(int k=0;k<len;k++) {
    if(!tree[k]) continue;
    if(tree[k]->is_xmlc()) b << tree[k]->get_name().c_str();
    else if(tree[k]->has_name(cst_hi))
      tree[k]->put_in_buffer(b);
    else
      b << '<' << tree[k]->get_name() << "/>";
  }
}

// Removes and returns the last element
auto Xml::remove_last() -> Xmlp {
  int len = size();
  if(len==0) return 0;
  Xmlp res = tree.back();
  tree.pop_back();
  return res;
}

// True if this is empty, or contains only a <hi> element which is empty
auto Xml::par_is_empty() -> bool {
  int len = size();
  if(len>1) return false;
  if(len==0) return true;
  if(!tree[0]) return true;
  if(tree[0]->is_xmlc()) return false;
  if(tree[0]->is_xmlc() || !tree[0]->id.is_font_change())
    return false;
  return tree[0]->par_is_empty();
}
//--------------------------- Word stats

// This is called when a new word is seen.
void all_words_ns::add_a_word(String s, int h)
{
  int H = h%100;
  WordList* L = WL0[H];
  while(L) {
    if(L->is_here(s,h)) { L->incr_freq(); return; }
    else L = L->get_next();
  }
  s = tralics_ns::make_string(s);
  WL0[H] = new WordList(s,h,WL0[H]);
}

// Prints all words with frequency i. Removes them from the list
void all_words_ns::dump_and_list(WordList*WL,int i)
{
  WordList*L = WL->get_next();
  WordList*first = WL;
  int printed = 0;
  while(L) {
    WordList* N = L->get_next();
    if(L->dump(fp,i)) {
      printed++;
      delete L; 
    } else { 
      first->set_next(L); 
      first= L;
    } 
    L = N;
  }
  first->set_next(0);
  if(printed) {
    scbuf << i << '=' << printed << ", ";
  }
}

// Finish dumping the words
void all_words_ns::dump_words(string name)
{
  WordList* WL =new WordList(0,0,0);
  WordList*W = WL;
  for(int i=0;i<100;i++){
    WordList* L = WL0[i];
    if(!L) continue;
    while(W->get_next()) W = W->get_next();
    W->set_next(L);
  }
  if(!WL->get_next()) return;
  String wf = tralics_ns::get_out_dir("words");
  fp = new fstream(wf,std::ios::out);
  if(!fp) return;
  if(!*fp) return;
  if(!name.empty())   *fp << "Team " << name << "\n";
  scbuf.reset();
  int i = 0;
  while(WL->get_next()) {
    i++;
    dump_and_list(WL,i);
  }
  *fp << "Total "  << nb_words << "  ";
  scbuf.remove_last(); // space
  scbuf.remove_last(); // comma
  scbuf.push_back(".\n");
  *fp << scbuf;
  fp->close();
}

inline auto dig_char(char c) -> bool { return c == '-' || is_digit(c); }

void Buffer::new_word()
{
  if(!wptr) return;
  if(wptr==1) buf[0] = 'x';
  bool ok = true;
  for(int i=0;i<wptr;i++) {
    if(!dig_char(buf[i])) { ok = false; break; }
  }
  if(ok) { reset(); return; } 
  all_words_ns::nb_words++;
  ok = true;
  if(is_upper_case(buf[0])) {
    for(int i=1;i<wptr;i++) {
      if(!is_lower_case(buf[i])) ok = false;
    }
    if(ok) buf[0] += 'a'-'A';
  }
  all_words_ns::add_a_word(buf, hashcode(6397));
  reset();
}

const String entities[] = {"&nbsp;","&ndash;","&mdash;","&ieme;",
			   "&gt;", "&lt;",0};

// This is static. If s is &foo;bar, returns the length
// of the &foo; part. Rturns 0 if this is not an entity.
auto post_ns::is_entity(String s) -> int {
  for(int k=0;;k++) {
    String w = entities[k];
    if(!w) return 0;
    int n = strlen(w);
    if(strncmp(s,w,n)==0)
      return n;
  }
}

// The scanner for all_the_words
void Xml::word_stats_i()
{
  if(is_xmlc()) {
    String s = name.c_str();
    if(!s) return;
    for(int i=0;;i++) {
      char c = s[i];
      if(c==0)  return;
      if(c=='&') {
	if(strncmp(s+i,"&oelig;",7)==0) {
	  i += 6; 
	  scbuf << "oe";
	  continue;
	}
	if(strncmp(s+i,"&amp;",5)==0) {
	  i += 4; 
	  scbuf << "&";
	  continue;
	}
	int w = post_ns::is_entity(s+i);
	if(w) {
	  i += w-1; 
	  scbuf.new_word();
	  continue;
	}
      }
      if(c==' ' || c=='`' || c=='\n' ||c==',' || c=='.' || c=='(' || c==')' ||c==':'
        ||c==';' || c=='\253' || c== '\273' ||c=='\'' || c=='\"')
	scbuf.new_word();
      else scbuf << c;
    }
  } else {
    if(name == the_names[np_formula]) return;
    int len = size();
    for(int i=0;i<len;i++)
      if(tree[i])
	tree[i]->word_stats_i();
  }
}

void Xml::word_stats(string match)
{
  scbuf.reset();
  word_stats_i();
  scbuf.new_word();
  all_words_ns::dump_words(match);
}

