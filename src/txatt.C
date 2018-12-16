// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose Grimm) 2004,2007-2011.
// Functions on attributes for Tralics

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


#include "tralics.h"
const char* txatt_rcsid=
  "$Id: txatt.C,v 2.16 2015/08/06 09:28:44 grimm Exp $";


// This returns the attribute list of this id.
// Uses the global variable the_stack.
AttList& Xid::get_att() const
{
  return the_main->the_stack->get_att_list(value);
}

// Returns a pointer to the pair x=... if it exists, -1 otherwise
int AttList::has_value(Istring x) const
{
  int n = val.size();
  for(int i = 0; i<n;i++)
    if(val[i].name == x) return i;
  return -1;
}

// Return att value if this id has attribute value n.
// Returns null string otherwise
Istring Xid::has_attribute(Istring n)
{
  AttList& X = get_att();
  int i = X.has_value(n);
  if(i>=0) return X.get_val(i);
  else return Istring();
}

// Return true if this id has special attribute pair.
// (it is unprintable).
bool Xid::is_font_change() const
{
  Istring n = Istring(cst_flaghi);
  return get_att().has_value(n) != -1;
}

// adds a=b, unless there is already an a in the list.
// In this case, if force is true, removes the old value, 
// otherwise does nothing
// Does nothing if b is null (ok if b is empty).
void AttList::push_back(Istring a, Istring b, bool force)
{
  if(b.null()) return;
  int T = has_value(a);
  if(T>=0) {
    if(force) val[T].value = b;
    return;
  }
  val.push_back(AttPair(a,b)); 
}

// Same function with a name_positions instead of an istring
// Note: istring(a) is the same as the_names[a];
void AttList::push_back(name_positions a, name_positions b, bool force)
{
  push_back(the_names[a], the_names[b], force);
}

// Same functions, without a third argument (default is force).
void AttList::push_back(Istring n, Istring v)
{
  push_back(n,v,true);
}

void AttList::push_back(name_positions N, name_positions V)
{
  push_back(the_names[N], the_names[V], true);
}

void AttList::push_back(name_positions N, Istring v)
{
  push_back(the_names[N],v,true);
}

// Add attribute named A value B to this id.
void Xid::add_attribute(Istring A, Istring B)
{
  get_att().push_back(A,B);
}

// Add attribute named A value B to this id.
void Xid::add_attribute(Istring A, Istring B, bool f)
{
  get_att().push_back(A,B,f);
}

// Add attribute named A value B to this id.
void Xid::add_attribute(name_positions A, name_positions B)
{
  get_att().push_back(A,B);
}
void Xid::add_attribute(name_positions A, name_positions B,bool c)
{
  get_att().push_back(A,B,c);
}

void Xid::add_attribute(name_positions n, Istring v)
{
  get_att().push_back(n,v);
}

// Adds the list L to the attribute list of this id.


void Xid::add_attribute(const AttList& L, bool force)
{
  int n = L.val.size();
  AttList& l = get_att();
  for(int i=0; i<n;i++)
    l.push_back(L.val[i].name,L.val[i].value,force);
}

void Xid::add_attribute_but_rend(Xid b)
{
  AttList& L = b.get_att();
  int n = L.val.size();
  AttList& l = get_att();
  for(int i=0; i<n;i++)
    if(L.val[i].name != np_rend)
    l.push_back(L.val[i].name,L.val[i].value,true);
}

// Add attribute list of element B to this id.
void Xid::add_attribute(Xid b)
{
  AttList& L = b.get_att();
  add_attribute(L,true);
}

// We should remove the slot....instead of replacing
void AttList::delete_att(name_positions a)
{
  int i = has_value(Istring(a));
  if(i>0) val[i].name = Istring(0);
}

// This kills the whole list
void AttList::destroy() 
{
  val = vector<AttPair>();
}

// Puts in the buffer the value of the attribute M of element idx
// returns false if there is no such value.
bool Buffer::install_att (Xid idx, Istring m)
{
  AttList&L = idx.get_att();
  int k = L.has_value(m);
  if(k==-1) return false;
  reset();
  push_back(L.get_val(k).c_str());
  return true;
}

// Puts the attribute list in the buffer
// We can have Foo="bar'", printed as Foo='bar&apos;'
// The case Foo'bar="&gee" is not handled.
// nothing is printed in case the name starts with an apostrophe.
// Print in reverse order, because this was in the previous version

void Buffer::push_back(const AttList& Y)
{
  int n = Y.val.size();
  if (the_main -> use_double_quote_att ())
     for(int i = n-1; i>=0 ;i--) push_back_alt(Y.val[i]); 
  else  for(int i = n-1; i>=0 ;i--) push_back(Y.val[i]);
}

void Buffer::push_back(const AttPair& X)
{
  const Istring& b = X.name;
  const Istring& a = X.value;
  if(a.null())  return;
  if(b.null())  return;
  String B = b.p_str();
  String A = a.p_str();
  if(!A) A = "_bug_";
  if(!B) B = "_bug_";
  if(B[0]=='\'') return;
  push_back(' ');
  push_back(B);
  push_back('=');
  push_back('\'');
  while(*A) {
    if(*A=='\'') push_back("&apos;") ; 
    else push_back(*A);
    A++;
  }
  push_back('\'');
}

// Use double quotes instead of single quotes
void Buffer::push_back_alt(const AttPair& X)
{
  const Istring& b = X.name;
  const Istring& a = X.value;
  if(a.null())  return;
  if(b.null())  return;
  String B = b.p_str();
  String A = a.p_str();
  if(!A) A = "_bug_";
  if(!B) B = "_bug_";
  if(B[0]=='\'') return;
  push_back(' ');
  push_back(B);
  push_back('=');
  push_back('\"');
  while(*A) {
    if(*A=='\"') push_back("&quot;") ; 
    else push_back(*A);
    A++;
  }
  push_back('\"');
}


// Returns true if there is a space. Kills at the space. Advance
bool Buffer::look_at_space(string s)
{
  reset();
  push_back(s);
  bool has_space = false;
  reset_ptr();
  for(int i=0;;i++) {
    if(head() ==0) break;
    if(is_space(head())) {
      has_space = true;
      kill_at(ptr);
      advance();
      break;
    }
    advance();
  }
  return has_space;
}

// What follows is used for handling the configuration file.

// In the case of "foo" (no space), returns <foo>
// In the case of space, what follows the spaces is considered as
// attribute list.
Xmlp Buffer::xml_and_attrib(string s)
{
  bool has_spaces = look_at_space(s);
  if(!has_spaces) return new Xml(Istring(s),0);
  Xmlp res = new Xml(Istring(buf),0);
  push_back_special_att(res->get_id());
  return res;
}

// This converts the buffer in to an attribute list for id.
void Buffer::push_back_special_att(Xid id)
{
  for(;;) {
    if(!find_equals()) return;
    int J = get_ptr1();
    if(!backup_space()) return;
    advance();
    if(!string_delims()) return;
    Istring a = Istring(to_string(J));
    Istring b = Istring(to_string(ptr1));
    id.add_attribute(a,b);
    advance();
  }
}

// Returns true if we see space, then s then space then equals then space.
// sets ptr to the char after this space
bool Buffer::see_equals(String s)
{
  ptr = 0;
  skip_sp_tab();
  int k = strlen(s);
  if(strncmp(buf+ptr,s,k) !=0) return false;
  ptr += k;
  skip_sp_tab();
  if(next_char() != '=') return false;
  skip_sp_tab(); 
  return true;
}
