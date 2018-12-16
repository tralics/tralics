// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002,2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


#include "tralics.h"
const char* txaux_rcsid=
  "$Id: txaux.C,v 2.32 2011/06/28 08:15:25 grimm Exp $";


namespace arith_ns {
  void start_err(String);
  void end_err();
};

using arith_ns::start_err;
using arith_ns::end_err;


// This prepares for an arithmetic error.
void arith_ns::start_err(String s)
{
  err_ns::local_buf << bf_reset << "Arithmetic overflow";
  if(s) err_ns::local_buf << ", threshold=" << s;
}

// Signals the error in the buffer.
void arith_ns::end_err()
{
  err_ns::signal_error("Arithmetic overflow",0);
}

// This makes sure that n fits on 30 bits
void ScaledInt::ovf30()
{
  if(value > max_dimension || -value > max_dimension) {
    start_err(0);
    err_ns::local_buf << ", threshold=" << max_dimension<< ", cur val="<< value;
    end_err();
    value= max_dimension;
  }
}

// This makes sure that n fits on 31 bits
// On a 32 bit machine, only -(max_integer+1) can overflow
void ScaledInt::ovf31()
{
  if(value>max_integer || value< -max_integer) {
    start_err(0); 
    err_ns::local_buf << ", threshold=" << max_integer << ", cur val=" <<value;
    end_err();
    value = 0;
  }
}

// If this is a TeX token representing a valid digit in base radix, 
// returns the value,
// otherwise return -1.
int Token::tex_is_digit(int radix)
{
  int w = val_as_other();
  if('0'<=w && w <= radix+'0' && w<='9') // do not use is_digit...
    return w-'0';
  if(radix != 16) return -1;
  if(w<='F' && w>= 'A')
    return w-'A' + 10;
  w = val_as_letter();  // allow category code 11 digits
  if(w<='F' && w>= 'A')
    return w-'A' + 10;
  return -1;
}

// --------------------------------------------------
// Division

// Divide by zero.
void Glue::zdv()
{
  start_err(0);
  err_ns::local_buf <<", division by 0";
  end_err();
}


// This is called x_over_n in TeX
// Divides this by n.
// No overflow possible. Division by zero may happen.
void ScaledInt::divide(int n)
{
  int x = value;
  if(n==0) {
    // value = 0; 
    start_err(0);
    err_ns::local_buf << ", division by 0\nin " << x << "/0";
    end_err();
    return;
  }
  if(n<0) {
    x = -x ; n = -n;
  }
  if(x>=0) 
    value = x/n;
  else
    value = -((-x)/n);
}


// computes xn/d , returns quotient and remainder
// Assumes n>0 and d>0.  Quotient is < 2^30
// This is used for instance when converting 38cm into 38*n/d pt
int arith_ns::xn_over_d(int x, int n, int d, int& remainder)
{
  const int two_fifteen = 1<<15;
  bool positive = true;
  if(x<0) { positive = false; x = -x; }
  int t = (x %two_fifteen)*n;
  int u = (x/two_fifteen)*n + (t/two_fifteen);
  int v = (u%d) *two_fifteen + (t%two_fifteen);
  int w = u/d;
  if(w >= two_fifteen) { 
    start_err("2^{30}");
    err_ns::local_buf <<"\nin " << x << "*" <<n  <<"/" << d;
    end_err();
    u = 1; 
  }
  else u = two_fifteen*w+(v/d);
  remainder = positive ? (v%d) : -(v%d);
  return positive ? u : -u;
}

// Returns floor(xn/d +1/2)
// This is used when you say \numexpr X*Y/Z
void ScaledInt::scale(int n, int d, int max_answer)
{
  int x = value;
  bool negative = false;
  if(scale(x,n,d,max_answer,negative)) {
    start_err(0);
    err_ns::local_buf <<"\nin " << x << "*" <<n  <<"/" << d;
    end_err();
  }
  if(negative) value = -value;
}

// Return true if overflow.
bool ScaledInt::scale(int x, int n, int d, int max_answer,bool&negative)
{
  value = 0;
  if(x==0) return false;
  if(n==0) return false;
  if(d<0) { d = -d; negative = true; } 
  if(x<0) { x = -x; negative = !negative; }
  if(n<0) { n = -n; negative = !negative; }
  int t = n/d;
  if(t>max_answer/x) return true;
  int a = t*x;
  n = n-t*d; // answer is a + xn/d, with n<d 
  if(n==0) { value = a; return false; }
  t = x/d;
  if(t> (max_answer-a)/n) return true;
  a = a+t*n; 
  x = x-t*d;  // answer is a + xn/d, with n<d and x<d
  if(x==0)  { value = a; return false; }
  if(x<n) {
    t = x; x = n ; n = t;
  }
  // Invariants -d<=r<0<n<=x<d
  // f+ floor[(xn+(r+d))/d]  = floor(x0n0/d+1/2)
  int f = 0;
  int r = (d/2)-d;
  int h = -r;
  for(;;) {
    if(n&1) {
      r = r+x;
      if(r>=0) { r = r-d; ++f; }
    }
    n = n/2;
    if(n==0) break;
    if(x<h) x = x+x;
    else {
      t = x-d; x = t+x; f = f+n;
      if(x== 0) break;
      if(x<n) {t = x; x = n; n = t;}
    }
  }
  if(f>(max_answer-a)) return true;
  value = a+f;
  return false;
}


// Computes x/n, rounded
// No overflow possible. Division by zero must be checked by caller.
// This is used when you say \dimexpr 2pt/3
void ScaledInt::quotient(int d)
{
  bool negative = false;
  int n = value;
  if(n<0) { n = -n ; negative = !negative;  }
  if(d<0) { d = -d ; negative = !negative;  }
  int a = n/d;
  n = n-a*d;
  d = n -d;
  if(d+n>=0)  ++a;
  if(negative) a = -a;
  value = a;
}

// Three of the four functions defined above apply to glue.
void Glue::quotient(int f)
{
  width.quotient(f);
  stretch.quotient(f);
  shrink.quotient(f);
  normalise();
}

void Glue::scale(int n, int d)
{
  width.scale(n,d,max_dimension);
  stretch.scale(n,d,max_dimension);
  shrink.scale(n,d,max_dimension);
  normalise();
}

void Glue::divide(int n)
{
  if(n==0) { 
    zdv();
    return;
  }
  width.divide(n);
  stretch.divide(n);
  shrink.divide(n); 
  normalise();
}

// Two of the four functions defined above apply to everything
// (\numexpr, etc).
void SthInternal::quotient(int f)
{
  if(f==0) { the_parser.parse_error("Division by zero"); return; }
  if(type==it_glue||type==it_mu) glue_val.quotient(f);
  else int_val.quotient(f);
}

void SthInternal::scale(int n, int d)
{
  if(d==0) { the_parser.parse_error("Division by zero"); return; }
  if(type==it_glue||type==it_mu)
    glue_val.scale(n,d);
  else if(type==it_int) int_val.scale(n,d,max_integer);
  else int_val.scale(n,d,max_dimension);
}

// --------------------------------------------------

// Computes nx+y. Overflow is 2^30.  
// We assume |y|< 2^{30};  so that overflow_threshold \pm y is OK
int arith_ns::nx_plus_y(int n, int x, int y) 
{
  if(n<0) { x = -x; n = -n; }
  if(n==0) return y;
  if(x<=(max_dimension-y)/n && -x <= (max_dimension+y)/n)
    return n*x+y;
  else { 
    start_err("2^{30}");
    err_ns::local_buf <<"\nin " << n << "*" << x << "+" << y;
    end_err();
    return 0;
  }
}
// Was arith_ns::mult_integers
// Multiplies the scaled number considered as an int by the integer.
void ScaledInt::mult_integer(int x) 
{
  int n = value;
  if(n<0) { x = -x; n = -n; }
  if(n==0) return;
  if(x<= max_integer/n && -x <= max_integer/n)
    value =  n*x;
  else { 
    start_err("2^{31}");
    err_ns::local_buf <<"\nin " << n << "*" << x;
    end_err();
    value = 0;
  }
}

// Was arith_ns::nx_plus_y with 2 args
// Multiplies the scaled number considered as a dimension by the integer.
void ScaledInt::mult_scaled(int x) 
{
  int n= value;
  if(n<0) { x = -x; n = -n; }
  if(n==0) return;
  if(x<=max_dimension/n && -x <= max_dimension/n)
    value = n*x;
  else { 
    start_err("2^{30}=16384pt");
    err_ns::local_buf <<"\nin " << n << "*" << x;
    end_err();
    value = 0;
  }
}

void SthInternal::expr_mul(int n)
{
  if(type==it_glue || type==it_mu) glue_val.multiply(n);
  else if(type==it_dimen) int_val.mult_scaled(n);
  else int_val.mult_integer(n);
}


// Multiplies a glue by an integer n
// What is the difference with void Glue::expr_mul(int n) ??
void Glue::multiply(int n)
{
  if(n==0) { kill(); return; }  
  width.mult_scaled(n);
  stretch.mult_scaled(n);
  shrink.mult_scaled(n);
}

// This negates a Glue.
void Glue::negate()
{
  width = -width;
  shrink = -shrink;
  stretch = -stretch;
}

// Adds y to this, assuming we have dimensions.
void ScaledInt::add_dim(ScaledInt Y)
{
  int x=get_value(),y=Y.get_value(),mx=max_dimension; 
  if(x>=0 && y<=mx-x) value = x+y;
  else if(x<=0 && y>=-mx-x) value =  x+y; 
  else {
    value = max_dimension; 
    start_err("2^{30}");
    err_ns::local_buf <<"\nin " << x << "+" << y << "\n";
    end_err();
  }
}

void Glue::normalise()
{
  if(stretch.null()) stretch_order = glue_spec_pt;
  if(shrink.null()) shrink_order = glue_spec_pt;
}

void SthInternal::normalise()
{
  if(type==it_glue || type==it_mu)
    glue_val.normalise();
}


// Add a glue to glue. 
void Glue::add(const Glue &r)
{
  width.add_dim(r.width);
  if(stretch_order ==r.stretch_order) {
    stretch.add_dim(r.stretch); 
    if(stretch.null()) stretch_order = glue_spec_pt;
  } else if (stretch_order<r.stretch_order && !r.stretch.null()) {
    stretch = r.stretch; 
    stretch_order = r.stretch_order;
  }
  if (shrink_order == r.shrink_order) {
    shrink.add_dim(r.shrink);
    if(shrink.null()) shrink_order = glue_spec_pt;
  } else if (shrink_order<r.shrink_order && !r.shrink.null()) {
    shrink = r.shrink;
    shrink_order = r.shrink_order;
  }
}

// Converts a RealNumber into a a TeX scaled number.
// Overflow is 4096pt
// Inlined call to attach sign
void SthInternal::attach_fraction(RealNumber x)
{
  int_val = x.get_ipart();
  int f = x.get_fpart();
  if(int_val.get_value() >= (1<<14)) {
    start_err("2^{14}");
    err_ns::local_buf <<"\nfor " << int_val.get_value();
    end_err();
    int_val = max_dimension;
  }
  else int_val = (int_val.get_value()<<16) + f;
  if(x.get_negative()) int_val = - int_val;
}

// This attaches the sign, and checks for overflow.
// Overflow is 4096pt (i.e. 2^30sp) 
void SthInternal::attach_sign(bool negative)
{
  int_val.ovf30();
  if(negative) int_val = - int_val;
}

// Initialises to zero, with a given type.
void SthInternal::initialise(internal_type t)
{
  type = t;
  int_val = 0;
  glue_val.kill();
}

// copies X into THIS.
void SthInternal::copy(const SthInternal& x)
{
  if(type==it_int) {
    if(x.type==it_glue)
      int_val = x.get_glue_width();
    else int_val = x.int_val;
  } else if(type== it_dimen) {
    if(x.type==it_glue)
      int_val = x.get_glue_width();
    else if(x.type==it_dimen)
      int_val = x.int_val;
    else {
      int_val=x.int_val; 
      the_parser.parse_error("Internal error: Bad Coerce int to dim");
    }
  } else {
    if(x.type==it_glue)
      glue_val = x.glue_val;
    else if(x.type==it_dimen){
      glue_val.kill();
      glue_val.set_width(x.int_val);
    }
    else {
      glue_val.kill();
      glue_val.set_width(x.int_val);
      the_parser.parse_error("Internal error: Bad Coerce int to dim");
    }
  }
}

// Hope it's ok for the types...
// No overflow tests ?
void SthInternal::add(const SthInternal& r)
{
  if(type != r.type) {
    the_parser.parse_error("Internal error in addition");
    return;
  }
  if(type==it_mu || type==it_glue) {
    glue_val.add(r.glue_val);
    return;
  }    
  int x= int_val.get_value(),y=r.int_val.get_value();
  int mx = type==it_int ? max_integer : max_dimension; 
  if(x>=0 && y<=mx-x) { int_val= x+y; return; }
  if(x<=0 && y>=-mx-x) { int_val = x+y; return; }
  start_err(type==it_int ? "2^{31}": "2^{30}");
  err_ns::local_buf <<"\nin " << x << "+" << y;
  end_err();
  int_val = mx;
}


// Converson between mu and normal units, assuming one em =10pt
// The Tex algorithm is more complicated because we have to multiply by a 
// non integer. Whose sees the difference. 
// the inversion function times_18_10 is not used anymore
void ScaledInt::times_10_18()
{
  divide(18);
  mult_scaled(10);
}

void ScaledInt::times_18_10()
{
  divide(10);
  mult_scaled(18);
}


void SthInternal::get_info(subtypes m)
{
  if(type != it_glue) {
    set_int(0);
    return;
  }
  Glue q = glue_val;
  if(m==gluestretch_code) set_dim(q. get_stretch());
  else if(m==glueshrink_code) set_dim(q. get_shrink());
  else if(m==gluestretchorder_code) set_int(q. get_stretch_order());
  else if(m==glueshrinkorder_code) set_int(q. get_shrink_order());
  else set_int(0);
}

// Initially arith_error was set to true;
// Signals an error
void SthInternal::check_overflow(scan_expr_t t)
{
  if(type==it_int ||(t==se_mult || t== se_div || t==se_scale)) {
    int_val.ovf31();
  } else if(type==it_dimen) {
    int_val.ovf30();
  } else glue_val.check_overflow();
}

void Glue::check_overflow()
{
  width.ovf30();
  stretch.ovf30();
  shrink.ovf30();
}

// Reset to zero.
void Glue::kill()
{ 
  width =0; shrink=0; stretch=0;
  shrink_order=glue_spec_pt;
  stretch_order=glue_spec_pt;
}

// Return true if the string contains only integers.
bool tralics_ns::only_digits(const string& s)
{
  String S = s.c_str();
  for(int i=0;;i++) {
    char c = S[i];
    if(c==0) return true;
    if(!is_digit(c)) return false;
  }
}

// Returns: 0 if false, 1 if true, 2 if empty, 3 otherwise
int SpecialHash::find_true_false(String s)
{
  string res = find(s);
  if(res.empty()) return 2;
  if(res=="true") return 1;
  if(res=="false") return 0;
  return 3;
}

// returns true if this is a valid register index.
int SpecialHash::counter_val(int k)
{
  if(k>=0 && uint(k)<nb_registers) return k;
  return -1;
}

// Hack. Returns -1 in case no counter was given
// counter=12345 was given, this is out_of_range,  counter=foo is given
// but \c@foo is not a counter. 
// Otherwise, returns the register number of the counter
int SpecialHash::find_counter()
{
  string s = find("counter");
  if(s.empty()) return -1;
  if(tralics_ns::only_digits(s)) 
    return counter_val(atoi(s.c_str()));
  Buffer&B = the_parser.hash_table.my_buffer();
  B << bf_reset << "c@" << s;
  Token t = the_parser.hash_table.locate(B);
  int cs = t.to_cs();
  if(the_parser.hash_table.eqtb[cs-1].get_cmd() != assign_int_cmd)
    return -1;
  return counter_val(the_parser.hash_table.eqtb[cs-1].get_chr() - count_reg_offset);
}


// finds a slot for the macro
// Note: the reference count is 0. This is strange: the macro has to
// be killed or its reference count increased.

subtypes Mactab::new_macro (Macro* s)
{
  the_parser.my_stats.one_more_macro();
  if(ptr<0)
    rc_mac_realloc();
  int w = ptr;
  ptr = rc_table[w];
  rc_table[w] = 0;
  table[w] = s;
  return subtypes(w);
}


// Initially cur_rc_mac_len=0, tables are empty
// Note that the table always contains a valid pointer
void Mactab::rc_mac_realloc()
{
  int k = cur_rc_mac_len;
  int ns = k+400;
  static Macro* empty_mac=0;
  if(!empty_mac) empty_mac = new Macro;
  Macro** T1 = new Macro*[ns];
  int* T2 = new int[ns];
  for(int i=0;i<k;i++) {
    T1[i]  = table[i];
    T2[i]  = rc_table[i];
  }
  if(table) delete[] table;
  if(rc_table) delete[] rc_table;
  table = T1;
  rc_table = T2;
  for(int i=k;i<ns;i++) {
    table[i] = empty_mac;
    rc_table[i] = i+1;
  }
  ptr = k==0 ? 1 : k;
  rc_table[ns-1] = -1;
  cur_rc_mac_len = ns;
}

// Decrements the reference count. If it is zero, we kill the macro.
void Mactab::delete_macro_ref(int i)
{
  if(rc_table[i]<=0) { 
    main_ns::log_and_tty << "FATAL: macro reference count negative\n" << lg_fatal;
    abort();
  }
  rc_table[i] --;
  if(rc_table[i]==0) {
    the_parser.my_stats.one_less_macro();
    delete table[i];
    rc_table[i] = ptr;
    ptr = i;
  }
}
