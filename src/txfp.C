// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


#include "tralics.h"
const char* txfp_rcsif =
  "$Id: txfp.C,v 2.28 2012/05/15 17:14:30 grimm Exp $";

typedef unsigned int Digit;

namespace {
  Token fp_tmp_token,fp_test_token;
  Token fps[fp_last_code];
  FpStack upn_stack;
  Buffer tkbuf;
  Buffer fp_in_buf;
  const Digit fp_max = 1000000000; // 10^9
  const Digit ten_8  = 100000000; // 10^8
  Digit fp_seed = 123456789;
  FpNum fp_rand1, fp_rand2;
  FpNum e_powers[42];
  FpNum neg_e_powers[43];
  FpNum log_table[4];
  FpNum log10_table[19];
  Token fp_res,fp_res2,fp_res3,fp_res4,fp_name;
  FpNum pi_table[9];
  FpNum* pascal_table[64];
  bool pascal_table_created = false;

  const int power_table[]  = {1,10,100,1000,10000,100000,1000000,
				 10000000,100000000,1000000000};
}

namespace fp {
  void mul_split_aux(Digit A, Digit*T);
  Digit unsplit_mul(const Digit* z);
  int compare_abs(const FpNum& X, const FpNum& Y);
  int compare(const FpNum& X, const FpNum& Y);
  void create_pascal_table();
  Digit truncate_n (Digit x, int n);
  Digit round_n (Digit x, int n);
  void sincos(FpNum& res1,FpNum& res2, FpNum x, subtypes i);
  void arcsincos(FpNum& res1,FpNum& res2, FpNum x, subtypes i);
  bool quad_aux(FpNum r, FpNum w, FpNum&y, FpNum&Ay);
  void set_nb_sols(int n);
  void x_solve(FpNum& r1, FpNum A, FpNum B);
  int qsolve(FpNum& r1, FpNum& r2, FpNum B, FpNum C);
  int qsolve(FpNum& r1, FpNum& r2, FpNum A, FpNum B, FpNum C);
  void x_solve(FpNum& r1, FpNum& r2, FpNum A, FpNum B, FpNum C);
  void csolve_aux(FpNum A, FpNum B, FpNum C, FpNum D, 
		  FpNum& p, FpNum&q, FpNum& T);
  int x_solve(FpNum& r1, FpNum& r2, FpNum&r3,
	      FpNum A, FpNum B, FpNum C,FpNum D);
  void x_solve(FpNum& r1, FpNum& r2, FpNum&r3, FpNum&r4,
	       FpNum A, FpNum B, FpNum C,FpNum D,FpNum E);
    
}

// This prints a FpNum on a ostream
ostream& operator<<(ostream& fp, const FpNum&X)
{
  fp << X.to_string();
  return fp;
}

// This prints a FpNum on the logger.
Logger& operator<<(Logger& fp,const FpNum&X)
{
  fp << X.to_string();
  return fp;
}

// Returns the opposite of X
FpNum operator-(FpNum X)
{
  FpNum res;
  res = X;
  res.sign = !res.sign;
  res.correct_sign();
  return res;
}

// Returns the product of X and Y
FpNum operator*(FpNum X, FpNum Y)
{
  FpNum res;
  res.mul(X,Y);
  return res;
}

// Returns the product of X and Y. Assumes abs(Y)<1000
FpNum operator*(FpNum X, int Y)
{
  FpNum res;
  res.mul(X,Y);
  return res;
}

// Idem, in reverse order
FpNum operator*(int Y, FpNum X)
{
  FpNum res;
  res.mul(X,Y);
  return res;
}

// Returns the sum of X and Y
FpNum operator+(FpNum X, FpNum Y)
{
  FpNum res = X;
  res.add(Y);
  return res;
}

// Returns the quotient of X and n. Assumes abs(n)<1000,  n!=0
FpNum operator/(FpNum X, int n)
{
  FpNum res= X;
  res.div(n);
  return res;
}

// Returns the quotient of X and Y
FpNum operator/(FpNum X, FpNum Y)
{
  FpNum res;
  res.div(X,Y);
  return res;
}

// Returns the difference between X and Y
FpNum operator-(FpNum X, FpNum Y)
{
  Y.neg();
  X.add(Y);
  return X;
}

// Increments X by Y, returns X
FpNum& operator+=(FpNum& X, FpNum Y)
{
  X.add(Y);
  return X;
}

// divides X by Y, returns X.
FpNum& operator/=(FpNum& X, FpNum Y)
{
  FpNum res;
  res.div(X,Y);
  X=res;
  return X;
}

// divides X by Y, returns X. Assumes abs(Y)<1000
FpNum& operator/=(FpNum& X, int Y)
{
  X.div(Y);
  return X;
}

void FpNum::mul(FpNum X)
{
  mul(*this,X);
}

void FpNum::div(FpNum X)
{
  div(*this,X);
}

// Returns true if zero
bool FpNum::is_zero() const 
{
  return data[0]==0 && data[1]==0 && data[2]==0 && data[3]==0;
}

// Make sure that zero is positive
void FpNum::correct_sign()
{
  if(is_zero()) sign = true;
}

// Change the sign
void FpNum::neg()
{
  sign = !sign;
  correct_sign();
}

// Replace by its sign
void FpNum::sgn()
{
  if(is_zero()) return;
  data[0] = 0; data[1] = 1;
  data[2] = 0; data[3] = 0;
}

// Multiply by 10. We assume that there is no overflow.
void FpNum::mul_by_10()
{
  static const Digit B = ten_8;
  data[0] = data[0]*10 +(data[1]/B);
  data[1] = (data[1]%B)*10 +(data[2]/B);
  data[2] = (data[2]%B)*10 +(data[3]/B);
  data[3] = (data[3]%B)*10;
}

// Multiplies by 2, assumes no overflow
void FpNum::mul_by_2()
{
  data[0] *=2;
  data[1] *=2;
  data[2] *=2;
  data[3] *=2;
  if(data[3]>=fp_max) { data[2]++; data[3] -= fp_max; }
  if(data[2]>=fp_max) { data[1]++; data[2] -= fp_max; }
  if(data[1]>=fp_max) { data[0]++; data[1] -= fp_max; }
}

// Divides by 10. This could give a negative zero
void FpNum::div_by_10()
{
  static const Digit B = ten_8; // 10^8
  data[3] = data[3]/10 + B*(data[2] %10);
  data[2] = data[2]/10 + B*(data[1] %10);
  data[1] = data[1]/10 +B*(data[0] %10);
  data[0] = data[0]/10;
}

// Puts in T the three 1000-base digits of A. Assumes T==0.
inline void fp::mul_split_aux(Digit A, Digit*T)
{
  if(A) {
    T[2] = A%1000;
    A = A/1000;
    T[1] = A%1000;
    T[0] = A/1000;
  }
}

// Returns a number from three 1000-base digits
inline Digit fp::unsplit_mul(const Digit* z)
{
  return z[0]*1000000+z[1]*1000+z[2];
}

// Splits into 12 times 1000-base digits
void FpNum::mul_split(Digit* T) const
{
  for(int i=0;i<12;i++) T[i] = 0;
  fp::mul_split_aux(data[0],T);
  fp::mul_split_aux(data[1],T+3);
  fp::mul_split_aux(data[2],T+6);
  fp::mul_split_aux(data[3],T+9);
}

// Converse of mul_split.
void FpNum::unsplit_mul4(const Digit*z)
{
  data[0] = fp::unsplit_mul(z);
  data[1] = fp::unsplit_mul(z+3);
  data[2] = fp::unsplit_mul(z+6);
  data[3] = fp::unsplit_mul(z+9);
}

// Adds a 3digit number in the buffer
void Buffer::push_back3(Digit x)
{
  char a = (x%10) +'0';
  x = x/10;
  char b = (x%10) + '0';
  char c = (x/10) + '0';
  push_back(c); push_back(b); push_back(a);
}

// Adds a 9digit number in the buffer
void Buffer::push_back9(Digit x)
{
  Digit A = x%1000; x = x/1000;
  Digit B = x%1000; 
  Digit C = x/1000;
  push_back3(C); push_back3(B); push_back3(A);
}

// Insert a number in the buffer. Returns the string value.
String Buffer::insert_fp(const FpNum& X)
{
  reset0();
  push_back(' '); //reserve for the sign
  push_back9(X.data[0]);
  push_back9(X.data[1]);
  push_back('.');
  push_back9(X.data[2]);
  push_back9(X.data[3]);
  while(buf[wptr-1]=='0') wptr--; // remove trailing zeroes
  kill_at(wptr);
  int i = 1;
  while(buf[i]=='0') i++; // remove initial zeroes
  if(buf[i]=='.') i--; // Keep the zero in 0.1
  i--;
  buf[i] = X.sign ? '+' : '-'; // insert sign
  return buf+i;
}

// Set xmin and xmax, so that x[i] is zero, unless xmin <= i < xmax
// In case x=0, we have xmin=xmax
void FpNum::set_xmax(Digit* x, int & xmin, int& xmax)
{
  int M=12;
  while(M>0) {
    if(x[M-1]==0) M--;
    else break;
  }
  xmax = M;
  int m=0;
  while(m<M) {
    if(x[m]==0) m++;
    else break;
  }
  xmin = m;
}

// Propagates the carry for a table of size 24 of 1000-based numbers
void FpNum::prop_carry(Digit* z)
{
  Digit carry = 0;
  for(int k= 23;k>0;k--) {
    Digit u = carry+z[k];
    z[k] = u%1000;
    carry = u/1000;
  }
  z[0] = carry;
}

// Computes the product XY. Both numbers are considered as a 12B number
// (where B=1000).. The product gives a 24B number.
void FpNum::mul(FpNum X, FpNum Y)
{
  bool xs = X.sign == Y.sign;
  Digit x[12], y[12], z[24];
  for(int i=0;i<24;i++)
    z[i] = 0;
  X.mul_split(x);
  Y.mul_split(y);
  int xmax,xmin,ymax,ymin;
  set_xmax(x,xmin,xmax);
  set_xmax(y,ymin,ymax);
  for(int i=xmin; i<xmax;i++)
    for(int j=ymin; j<ymax;j++)
      z[i+j+1] += x[i]*y[j];
  prop_carry(z);
  finish_mul(xs,z);
}

// Multiplies by an integer, assumed to be less than 1000 and positive
void FpNum::mul(FpNum X, int y)
{
  Digit x[12],z[24];
  bool xs = X.sign;
  if(y<0) { y = -y; xs = ! xs; }
  for(int i=0;i<24;i++)
    z[i] = 0;
  X.mul_split(x);
  for(int i=0;i<12;i++) z[i+6] = x[i]*y;
  prop_carry(z);
  finish_mul(xs,z);
}

// Converts the 24 1000-base number into a 12 1000-base number
// by complaining for the first 6, and ignoring the last 6.
void FpNum::finish_mul(bool xs, Digit*z)
{
  if(z[0] || z[1] || z[2] || z[3] ||z[4] || z[5])
    the_parser.parse_error("Overflow in FPmul");
  unsplit_mul4(z+6);
  sign = xs;
  correct_sign();
}

// Returns -1, 0, 1 according to x<y x=y x>y (x=abs X, y = abs Y)
int fp::compare_abs(const FpNum& X, const FpNum& Y)
{
  if(X.data[0]<Y.data[0]) return -1;
  if(X.data[0]>Y.data[0]) return 1;
  if(X.data[1]<Y.data[1]) return -1;
  if(X.data[1]>Y.data[1]) return 1;
  if(X.data[2]<Y.data[2]) return -1;
  if(X.data[2]>Y.data[2]) return 1;
  if(X.data[3]<Y.data[3]) return -1;
  if(X.data[3]>Y.data[3]) return 1;
  return 0;
}

// Returns -1, 0, 1 according to X<Y X=Y X>Y
int fp::compare(const FpNum& X, const FpNum& Y)
{
  if(X.sign && !Y.sign) return 1;
  if(!X.sign && Y.sign) return -1;
  int res = compare_abs(X,Y);
  if(X.sign) return res;
  else return -res;
}

// This reads at most p, at most 9 digits from the buffer
// Computes \sum a_k b_k, where a_k is the next digit, b_k = 10^k
// via Horner (k is decreasing until 0).
// Ok for the integer part of 1234.5678
Digit Buffer::horner(int p)
{
  if(p>9) p = 9;
  Digit res = 0;
  while(p>0) {
    res = res*10 + (next_char() -'0');
    p--;
  }
  return res;
}

// Computes \sum a_k b_k, where a_k is the next digit, b_k = 10^k
// via explicit power. k is decreasing, starts with 0. Result is 
// multiplied by 10^9. Reading ends after 9 digits or EOL.
// Ok for the fractional part of 1234.5678
Digit Buffer::reverse_horner()
{
  int i=8;
  Digit res =0;
  for(;;) {
    if(at_eol()) return res;
    res += power_table[i]*(next_char() -'0');
    i--;
    if(i<0) return res;
  }
}

// Assume that the buffer holds : sign digits dot digits
// Creates a number with it. Returns true if overflow
bool FpNum::create(Buffer& B)
{
  B.reset_ptr();
  char c = B.next_char();
  bool retval = false;
  sign = c != '-';
  for(;;) { // skip initial zeroes
    if(B.head()=='0') B.advance();
    else break;
  }
  int k = B.get_ptr(); // index of first non-zero digit.
  for(;;) {
    if(B.head()!='.') B.advance();
    else break;
  }
  int n = B.get_ptr() - k; // number of chars before dot
  B.set_ptr(k);
  if(n>18) retval = true; 
  data[0] = B.horner(n-9);
  data[1] = B.horner(n);
  for(;;) {
    if(B.next_char()=='.') break; // ignore possible unused chars
  }
  data[2] = B.reverse_horner();
  data[3] = B.reverse_horner();
  correct_sign();
  return retval;
}

// This puts the digits in a buffer, removes leading and trailing zeroes
// This returns the buffer.
String FpNum::to_string() const
{
  String buf = fp_in_buf.insert_fp(*this); 
  return buf;
}

// This puts the digits in a buffer, then constructs a token list
// Leading and trailing zeroes are removed.
TokenList FpNum::to_list() const
{
  String buf = to_string();
  TokenList res;
  int i=0;
  if(buf[0]=='+') i++;
  while(buf[i]) {
    res.push_back(Token(other_t_offset+buf[i]));
    i++;
  }
  return res;
}

// Computes abs(A)+abs(B)
void FpNum::add_abs(FpNum Y)
{
  data[3] += Y.data[3];
  if(data[3]>=fp_max) { data[2] ++; data[3] -= fp_max; }
  data[2] += Y.data[2];
  if(data[2]>=fp_max) { data[1] ++; data[2] -= fp_max; }
  data[1] += Y.data[1];
  if(data[1]>=fp_max) { data[0] ++; data[1] -= fp_max; }
  data[0] += Y.data[0];
  if(data[0]>=fp_max) {
    the_parser.parse_error("Overflow in FPadd"); 
    data[0] -= fp_max;
  }
}


// This adds a digit (>=0 would be better)
void FpNum::add_digit(int y)
{
  data[3] += y;
  if(data[3]>=fp_max) { data[2] ++; data[3] -= fp_max; }
  if(data[2]>=fp_max) { data[1] ++; data[2] -= fp_max; }
  if(data[1]>=fp_max) { data[0] ++; data[1] -= fp_max; }
  if(data[0]>=fp_max) {
    the_parser.parse_error("Overflow in FPadd"); 
    data[0] -= fp_max;
  }
}

// Computes abs(*this)-abs(Y). Assumes result is positive. Sign is unchanged
void FpNum::sub_abs(FpNum Y)
{
  bool test = data[3]<Y.data[3];
  data[3] -= Y.data[3];
  if(test) { Y.data[2] ++; data[3] += fp_max; }
  test = data[2]<Y.data[2];
  data[2] -= Y.data[2];
  if(test) { Y.data[1] ++; data[2] += fp_max; }
  test = data[1]<Y.data[1];
  data[1] -= Y.data[1];
  if(test) { Y.data[0] ++; data[1] += fp_max; }
  data[0] -= Y.data[0];
}

// Computes abs(Y)-abs(*this). Assumes result is positive. Sign is unchanged
void FpNum::neg_sub_abs(FpNum Y)
{
  bool test = data[3]>Y.data[3];
  data[3] = Y.data[3] - data[3];
  if(test) { data[2] ++; data[3] += fp_max; }
  test = data[2]>Y.data[2];
  data[2] = Y.data[2] - data[2];
  if(test) { data[1] ++; data[2] += fp_max; }
  test = data[1]>Y.data[1];
  data[1] = Y.data[1] - data[1];
  if(test) { data[0] ++; data[1] += fp_max; }
  data[0] = Y.data[0] - data[0];
}

// Computes A+B. Assuming A and B are positive integers.
void FpNum::add_int(FpNum Y)
{
  data[1] += Y.data[1];
  if(data[1]>=fp_max) { data[0] ++; data[1] -= fp_max; }
  data[0] += Y.data[0];
  if(data[0]>=fp_max) {
    the_parser.parse_error("Overflow in FPadd"); 
    data[0] -= fp_max;
  }
}

// Creates the Pascal Table
void fp::create_pascal_table()
{
  for(int i=0;i<64;i++) {
    pascal_table[i] = new FpNum[i+1];
    pascal_table[i][0].init(0,1,0,0);
    pascal_table[i][i].init(0,1,0,0);
    for(int j=1;j<i;j++) {
      pascal_table[i][j] = pascal_table[i-1][j-1];
      pascal_table[i][j].add_int(pascal_table[i-1][j]);
    }
  }
  pascal_table_created = true;
}

// This puts in a token list, that is the body of the parameterless macro
// (value of fp_res), one row of th Pascal Table. 
// The list is comma-separated, with brackets around.
void Parser::fp_e_pascal()
{
  fp_prepare();
  if(!pascal_table_created) fp::create_pascal_table();
  int j = scan_braced_int(fp_name);
  if(j<0 || j>=64) {  
    tkbuf.reset(); tkbuf << "out of bound "; tkbuf << j;
    tkbuf << " max value is 63";
    parse_error(tkbuf.c_str()); 
    return; 
  }
  TokenList res;
  res.push_back(Token(other_t_offset,'['));
  for(int i=0;i<=j;i++) {
    if(i!=0) res.push_back(hash_table.comma_token);
    TokenList w = pascal_table[j][i].to_list();
    res.splice(res.end(), w);
  }
  res.push_back(Token(other_t_offset,']'));
  new_macro(res,fp_res);
}


// Truncate x to n digits.
Digit fp::truncate_n (Digit x, int n)
{
  if(n==0) return 0;
  if(n==9) return x;
  int k = power_table[9-n];
  return (x/k)*k;
}

// Round to n digits.
Digit fp::round_n (Digit x, int n)
{
  if(n==9) return x;
  if(n==0) return 0;
  Digit k = power_table[9-n];
  Digit A = x/k, B = x%k;
  if(B>=k/2) A++;
  return A*k;
}

// Truncate a FpNum to n digits
void FpNum::truncate(int n)
{
  if(n>=18) return;
  if(n<0){ 
    tkbuf.reset(); tkbuf << "Negative number "; tkbuf << n << " in truncate";
    the_parser.parse_error(tkbuf.c_str()); 
    return; 
  }
  if(n==0) { data[2] = data[3] = 0; }
  if(n<=9) { data[2] = fp::truncate_n(data[2],n); data[3] = 0; }
  else data[3] = fp::truncate_n(data[3],n-9);
  correct_sign(); // result could be negative zero
}

// Handles the carry for the round procedure.
void FpNum::round0()
{
  if(data[3]>=fp_max) {
    data[3] = 0;
    data[2] ++;
  }
  if(data[2]>=fp_max) { 
    data[2] = 0; 
    data[1]++;
    if(data[1]>=fp_max) { data[0]++; data[1] = 0; }
  }
  correct_sign();
}

// Round to n digits
void FpNum::round(int n)
{
  if(n>=18) return;
  if(n<0){ 
    tkbuf.reset(); tkbuf << "Negative number "; tkbuf << n << " in round";
    the_parser.parse_error(tkbuf.c_str()); 
    return; 
  }
  if(n==0) {
    if(data[2] >=500000000) { data[2] = fp_max; round0(); }
    data[2] = data[3] = 0;
    return;
  }
  if(n==9) {
    if(data[3] >=500000000) data[2] ++;
    data[3] = 0;
    return;
  } 
  if(n>9) 
    data[3] = fp::round_n(data[3],n-9);
  else { data[2] = fp::round_n(data[2],n); data[3] = 0; } 
}

// Increments *this by Y
void FpNum::add(FpNum Y)
{
  if(sign==Y.sign) add_abs(Y);
  else if(fp::compare_abs(*this,Y)>=0)
    sub_abs(Y);
  else {
    neg_sub_abs(Y);
    sign = Y.sign;
  }
  correct_sign();
}

// Replaces *this by X-Y
void FpNum::sub(FpNum X, FpNum Y)
{
  *this = X;
  Y.neg();
  add(Y);
}

// Replaces *this by X+Y
void FpNum::add(FpNum X, FpNum Y)
{
  *this = X;
  add(Y);
}

// Divide by an integer n. Assumea that abs(n) <1000
void FpNum::div(int n)
{
  Digit x[12];
  if(n<0) { sign = !sign; n = -n; }
  if(n==0) {
    the_parser.parse_error("Division by 0");
    return;
  }
  mul_split(x);
  Digit carry = 0;
  for(int i=0;i<12;i++) {
    Digit a = 1000* carry + x[i];
    carry = a %n;
    x[i] = a/n;
  }
  unsplit_mul4(x);
}

// It divides by 2.
void FpNum::divide2()
{
  if(data[0]%2) data[1] += fp_max;
  if(data[1]%2) data[2] += fp_max;
  if(data[2]%2) data[3] += fp_max;
  data[0] /= 2;
  data[1] /= 2;
  data[2] /= 2;
  data[3] /= 2;
}

// Let y=(x-1)/(x+1). Let f(y) =2\sum y^k/k, k odd. Then f'(y) is 2\sum y^{2k}
// hence 2/(1-y^2), and df/dx =1/x, so that f is log
// We assume 1\le x \l2 2, so that y is between 0 and 1/3. 
// The sequence converges
void FpNum::exec_ln_a()
{
  FpNum X= *this;
  FpNum Y= *this;
  X.data[1] --;
  Y.data[1] ++;
  FpNum Z;
  Z.div(X,Y); // (x-1)/(x+1);
  FpNum R,L;
  R = Z;
  L = Z;
  Z.mul(Z,Z);
  for(int n=3;;n+=2) {
    L.mul(L,Z);
    if(L.is_zero()) break;
    Y = L;
    Y.div(n);
    R.add(Y);
  }
  R.mul_by_2();
  *this = R;
}

// Computes log via  division, and exec_lna
void FpNum::exec_ln()
{
  if(!sign) { the_parser.parse_error("Log of negative number"); return ; }
  if(is_zero()) { the_parser.parse_error("Log of 0"); return ; }
  int n = 0;
  while(data[0]==0 && data[1]==0) { mul_by_10(); n++; }
  while(data[0] ||  data[1]>=10) { div_by_10(); n--; }
  FpNum X;
  X = log10_table[n<0? -n: n];
  if(n>0) X.sign = false;
  n =0;
  while(data[1]>=2) { divide2(); n++; }
  X.add(log_table[n]);
  exec_ln_a();
  add(X);
}

// Replaces *this by the exponential of its integer part.
// Returns true if overflow
bool FpNum::large_exp()
{
  int k = data[1];
  if(!sign && (data[0] || k>42)) {
    reset();
    sign = true;
    return true;
  }
  if(sign && (data[0] || k>41)) {
    init(0,1,0,0);
    the_parser.parse_error("Overflow in FPexp"); 
    return true;
  }
  *this = sign ? e_powers[k] : neg_e_powers[k];
  return false;
}

// Computes the exponential. exp(a+b)=exp(a) exp(b)
// Here a is integer part, exp(a) in found in a table; exp(b) =\sum b^k/k!
void FpNum::exec_exp()
{
  FpNum X = *this;
  if(large_exp()) return;
  X.data[0] = X.data[1] = 0;
  FpNum Y = X;
  FpNum res;
  res.init(0,1,0,0); 
  res.add(X);
  int n = 1;
  for(;;) {
    if(Y.is_zero()) break;
    n++;
    Y.mul(X,Y);
    Y.div(n);
    res.add(Y);    
  }
  mul(*this,res);
}

// Computes pow(X,Y) using exponential and log
// computes also pow(X,1/Y)
void FpNum::pow(FpNum X, FpNum Y, subtypes i)
{
  *this = X;
  if(!sign) {
    the_parser.parse_error("First argument negative");
    return;
  }
  if(X.is_zero()) return;
  exec_ln();
  if(i==fp_pow_code) mul(Y); else div(Y);
  exec_exp();
}

// Replaces *this by *this-Y, as longh as the result is positive
// Returns  how many times this was done.
int FpNum::count_times(FpNum Y)
{
  int i = 0;
  for(;;) {
    if(fp::compare_abs(*this,Y)>=0) {
      i++;
      sub_abs(Y);
    }
    else return i;	  
  }
}

// Computes the quotient of X and Y, result in *this
// The method is the following. We first multiply X and Y by a power of ten
// as large as possible, so that multiplying would overflow). We write X=QY,
// then multiply Q by the adequate power of ten. Assume Q=\sum ak.10^-k
// We compute each ak, and in fact, a0, 10a0+a1, 100a0+10a1+a2, etc.
// In fact, we compute Q.10^17. If n-1 terms have been computed,  we have
// X-\sum akY10^-k = \sum ak.Y.10^-k where k<N on LHS, k>=n on RHS. The RHS
// is also \sum a(k+N)(Y10^-N)10^-k. Thus aN is the integer part of the 
// quotient of the LHS by Y10^-N
void FpNum::div(FpNum X, FpNum Y)
{
  reset();
  if(Y.is_zero()) {
    the_parser.parse_error("division by zero");
    return;
  }
  if(X.is_zero()) {
    sign = true;
    return;
  }
  sign = X.sign == Y.sign;
  int shift = 0;
  while(Y.data[0]< ten_8) { Y.mul_by_10(); shift ++; }
  while(X.data[0]< ten_8) { X.mul_by_10(); shift --; }
  for(;;) {
    if(Y.is_zero()) break;
    int i = X.count_times(Y);
    Y.div_by_10();
    mul_by_10();
    add_digit(i);
  }
  while(shift>17) {
    if(data[0]>=ten_8) {
      the_parser.parse_error("Overflow in FPdiv");
      return;
    }
    mul_by_10();
    shift --;
  }
  while(shift<17) { div_by_10(); shift++;  }
  correct_sign();
}

// Reduces *this modulo 2\pi. Quotient is obtained as above. Note ig 1<=x<=10
// we have shift =-17; if x<1  we have shift<=-18 and loops<=0: quotient is
// zero, remainder is x. In case 100<=x<1000, we have shift=-15, count =3, and
// quotient can be as high as 999 (of course, the quotient is less than 200).
void FpNum::mod2pi()
{
  if(is_zero()) return;
  int shift = 0;
  while(data[0]<ten_8) { mul_by_10(); shift--;}
  FpNum bigpi=pi_table[0];
  int loops = 18 + shift;
  while(loops>0) {
    count_times(bigpi);
    bigpi.div_by_10();
    loops--;
  }  
  while(shift <0) {
    div_by_10();
    shift++;
  }
  correct_sign();
  if(!sign) add(pi_table[8]);
}

// Returns k if x is between k\pi/4 and (k+1)\pi/4
// computes x-y or y-x, where y is an integer multiple of \pi/2
// result is between 0 and \pi/4
int FpNum::octand()
{
  if(fp::compare_abs(*this, pi_table[4])>0) {
    if(fp::compare_abs(*this, pi_table[6])>0) {
      if(fp::compare_abs(*this, pi_table[7])>0) {
	neg_sub_abs(pi_table[8]);
	return 7;
      } else {
	sub_abs(pi_table[6]);
	return 6;
      }
    }
    if(fp::compare_abs(*this, pi_table[5])>0) {
      neg_sub_abs(pi_table[6]);
      return 5;
    } else {
      sub_abs(pi_table[4]);
      return 4;
    }
  }
  if(fp::compare_abs(*this, pi_table[2])>0) {
    if(fp::compare_abs(*this, pi_table[3])>0) {
      neg_sub_abs(pi_table[4]);
      return 3;
    }
    else {
      sub_abs(pi_table[2]);
      return 2;
    }
  }
  if(fp::compare_abs(*this, pi_table[1])>0) {
    neg_sub_abs(pi_table[2]);
    return 1;
  } else return 0;
}

// Computes \sum x^n/n! , with a sign, and only half terms
// If n=2, the first term added is -px^2/3
void FpNum::sincos_loop(int n, FpNum x, FpNum p)
{
  for(;;) {
    p.mul(p,x);
    p.mul(p,x);
    p.div(n);
    n++;
    p.div(n);
    n++;
    p.sign = !p.sign;
    if(p.is_zero()) return;
    add(p);
  }
}

// sin for numbers between 0 and pi/4
// Note that sin(x)=x-x^3/3 + ...
void FpNum::sin0()
{
  if(is_zero()) return;
  sincos_loop(2,*this,*this);
}

// cos for numbers between 0 and pi/4
void FpNum::cos0()
{
  FpNum x = *this, p;
  init(0,1,0,0);
  if(is_zero()) return; // cos(0)=1
  p.mul(x,x);
  p.div(2);
  sub_abs(p);  // start with 1-x^2/2
  p.neg();
  sincos_loop(3,x,p);
}

// sin(x-pi/2) = -sin(pi/2-x) etc...
void FpNum::sin1(int k)
{
  switch(k) {
  case 0: sin0(); return;
  case 1: cos0(); return;
  case 2: cos0(); return;
  case 3: sin0(); return;
  case 4: sin0(); neg(); return;
  case 5: cos0(); neg(); return;
  case 6: cos0(); neg(); return;
  case 7: sin0(); neg(); return;
  }
}

void FpNum::cos1(int k)
{
  switch(k) {
  case 0: cos0(); return;
  case 1: sin0(); return;
  case 2: sin0(); neg(); return;
  case 3: cos0(); neg(); return;
  case 4: cos0(); neg(); return;
  case 5: sin0(); neg(); return;
  case 6: sin0(); return;
  case 7: cos0(); return;
  }
}

// sin cos, sincos, tan, cot tancot
void fp::sincos(FpNum& res1,FpNum& res2, FpNum x, subtypes i)
{
  x.mod2pi();
  int oct = x.octand();
  res1 = x; res2 = x;
  if(i != fp_cos_code) { res1.sin1(oct); }
  if(i != fp_sin_code) { res2.cos1(oct); }
  if(i==fp_sin_code || i==fp_sincos_code) return;
  if(i==fp_cos_code) { res1 = res2; return;}
  if(i==fp_tan_code) { res1.div(res1,res2); return; }
  if(i==fp_cot_code) { res1.div(res2,res1); return; }
  if(i==fp_tancot_code) { 
    FpNum tmp; tmp.div(res1,res2); res2.div(res2,res1); 
    res1 = tmp;
    return; 
  }
}

// This computes only the cosine
void FpNum::cosine()
{
  FpNum res1,res2;
  fp::sincos(res1,res2,*this,fp_cos_code);
  *this = res2;
}

// Compute x/x+1, 0\le x \le 10^6
void FpNum::trigo_xy(int x)
{
  reset();
  sign  = true;
  Digit a = x;
  Digit b = x+1;
  Digit c = 0;
  a *= 1000;
  c = a/b;
  data[2] = c;
  a = 1000*(a- c*b); 
  c = a/b;
  data[2] = 1000*data[2]+c;
  a = 1000*(a-b*c);
  c = a/b;
  data[2] = 1000* data[2] + c;
  a = 1000*(a-b*c);
  c = a/b;
  data[3] = c;
  a = 1000*(a-b*c);
  c = a/b;
  data[3] = data[3]*1000+c;
  a = 1000*(a-b*c);
  c = a/b;
  data[3] = data[3]*1000+c;
}

// Assume a<1 and b<1, computes (a+b)/2
void FpNum::mean(FpNum a, FpNum b)
{
  data[2] = a.data[2]+ b.data[2];
  data[3] = a.data[3]+ b.data[3];
  data[3] /=2;
  if(data[2]%2) data[3] += 500000000;
  data[2] /= 2;
  if(data[3] >=fp_max) { data[3] -= fp_max; data[2] ++; }
}

// For arcsin, arccos. If x=\sin t, y=\cos t,  we have x^2+y^2=1. We assume 
// that x is known, t is desired. If x<y, this returns false. otherwise, it
// returns true, replaces *this by y. The square root is computed
// one bit after the other (dichotomy).
bool FpNum::sincos_transform()
{
  if(data[2]<=707106781) return false;
  FpNum y;
  // put 1-x^2 in y
  y.mul(*this,*this);
  y.data[2] = fp_max - y.data[2];
  if(y.data[3]) {
    y.data[3] = fp_max - y.data[3];
    y.data[2]--;
  }
  // compute sqrt y
  FpNum xf,oldval;
  FpNum r,rs;
  sign = true;
  init(0,0, 710000000,0); // xx near sqrt2 is a good starting point
  xf.reset();
  oldval.reset();
  for(;;) {
    if(xf.data[2]==data[2] && xf.data[3]==data[3])
      return true;
    r.mean(*this, xf);
    if(r.data[2] == oldval.data[2] && r.data[3] == oldval.data[3]) {
      *this = r; 
      return true;
    }
    oldval.data[2] = r.data[2]; oldval.data[3] = r.data[3];
    rs.mul(r,r);
    bool cmp;
    if(rs.data[2]<y.data[2])  cmp=true;
    else if(rs.data[2]>y.data[2])  cmp = false;
    else if(rs.data[3]<y.data[3])  cmp = true;
    else if(rs.data[3]>y.data[3])  cmp = false;
    else { *this = r; return true;}
    if(cmp) xf = r; else *this = r;
  }
}

// Return true for arcsin or false for arccos of x
// This computes  f(x)=\sum x^(2k+1) ak/(2k+1)
// where ak = 1/2*3/4....*(2k-1)/(2k).
// we have f'(x)=\sum x^(2k)ak=\sum x^(2k)(2k-1)!/k!2^k = (1-x^2)^(-1/2)
// hence f(x) = arcsin(x)
bool FpNum::arcsincos_loop()
{
  bool ovf=false;
  if(data[0]) ovf=true;
  if(data[1]>1)  ovf=true;
  if(data[1]==1) {
    if(data[2] || data[3]) ovf=true;
  }
  if(ovf) {
    the_parser.parse_error("Number greater than one for ",fp_name, "","OVF");
    return false;
  }
  if(data[1]==1 && data[2]==0 && data[3]==0) {
    *this = pi_table[2];
    return false;
  }
  bool ret = sincos_transform();
  sign = true;
  FpNum res = *this;
  FpNum L = *this;
  FpNum tmp;
  int n = 1;
  for(;;) {
    L.mul(L,*this);
    L.mul(L,*this);
    tmp.trigo_xy(n);
    L.mul(L,tmp);
    n += 2;
    tmp = L;
    tmp.div(n);
    res.add(tmp);
    if(tmp.is_zero()) {
      *this = res;
      return ret;
    }
  }
}

// Computes 1/y, y>=1
void FpNum::trigo_inv()
{
  int shift = 0;
  while(data[0]<ten_8) { mul_by_10(); shift ++; }
  FpNum x,r;
  x.init(ten_8*10,0,0,0);
  r.reset();
  for(;;) {
    if(is_zero()) break;
    int i = x.count_times(*this);
    div_by_10();
    r.mul_by_10();
    r.add_digit(i);
  }
  *this = r;
  while(shift<35) { shift++; div_by_10(); }
}

// Computes 1/\sqrt(1+x^2)
// if y=(5-4x^2)/9 then 1/\sqrt(1+x^2) = (2/3) /sqrt(1-y)
void FpNum::trigo_sqrt()
{
  FpNum y, one, L,t;
  y.mul(*this,*this);
  one.init(0,1,0,0);
  y.neg_sub_abs(one); //1-x^2
  y.data[2] += 250000000;  // 1-x^2 +1/4
  Digit w=444444444;
  L.init(0,0,w,w); // l = 4/9
  y.mul(y,L); 
  w=666666666; 
  L.init(0,0,w,w); // l = 2/3
  *this = L;
  int i =0;
  for(;;) {
    i += 2;
    t = one;
    t.div(i);
    t.neg_sub_abs(one); // 1-1/i
    L.mul(L,t);
    L.mul(L,y);
    if(L.is_zero()) return;
    add(L);
  }
}

// Assumes 0\le x<1/2
// computes f(x)=x-x^3/3+x^5/5-x^7/7. 
// f'(x) = 1-x^2+x^4-x^6 ... = 1/(1+x^2). Hence f = arctan(x)
void FpNum::arctan()
{
  if(is_zero()) return;
  FpNum x = *this;
  FpNum L = *this;
  // *this is this result. contains x initially
  x.mul(x,x);
  int n = 1;
  bool s = true;
  for(;;) {
    s = !s;
    n +=2;
    L.mul(L,x);
    FpNum y =L;
    y.div(n);
    if(y.is_zero()) return;
    y.sign = s;
    add(y);
  }
}

void FpNum::arctan1()
{
  bool xs = sign;
  sign = true;
  if(data[0]==0 && data[1]==1 && data[2]==0 && data[3]==0) {
    *this = pi_table[1];
    sign = xs;
    return;
  }
  bool subfpih=false;
  if(data[0]==0 && data[1]==0) {}
  else {
    subfpih = true;
    trigo_inv();
  }
  if(data[2]>888000000) {
    trigo_sqrt();
    bool r = arcsincos_loop();
    if(!r) neg_sub_abs(pi_table[2]);
  } else arctan();
  if(subfpih) neg_sub_abs(pi_table[2]);
  sign = xs;
}

// This computes the inverse sin cos or both.
void fp::arcsincos(FpNum& res1,FpNum& res2, FpNum x, subtypes i)
{
  if(i==fp_arcsin_code ||i==fp_arcsincos_code ||i==fp_arccos_code) {
    bool xs = x.sign;
    bool ret = x.arcsincos_loop();
    if(i==fp_arcsin_code ||i==fp_arcsincos_code) {
      res1 = x; res1.sign = xs;
      if(ret) 
	res1.neg_sub_abs(pi_table[2]);
    }
    if(i==fp_arccos_code ||i==fp_arcsincos_code) {
      res2 = x;
      if(!ret) res2.neg_sub_abs(pi_table[2]);
      if(!xs) res2.neg_sub_abs(pi_table[4]);
      res2.sign = true;
    }
    if(i==fp_arccos_code) res1 = res2;
    return;
  }
  x.arctan1();
  res1 = x;
  if(i==fp_arccot_code ||i==fp_arctancot_code) {
    res2 = x;
    if(!res2.sign) {
      res2.sign= true; 
      res2.neg_sub_abs(pi_table[2]);
      res2.neg_sub_abs(pi_table[4]);
    } else {
      res2.neg_sub_abs(pi_table[2]);
    }
  }
  if(i==fp_arccot_code) res1 = res2;
}

// This remebers the number of solutions of an equation.
void fp::set_nb_sols(int n)
{
  the_parser.word_define(count_reg_offset+0,n,false);
}

// Computes sqrt via log and exp.
void FpNum::sqrt()
{
  exec_ln();
  FpNum two(0,2,0,0);
  div(*this,two);
  exec_exp();
}

// Computes cubic root via log and exp.
void FpNum::croot()
{
  bool xs = sign;
  sign = true;
  exec_ln();
  FpNum three(0,3,0,0);
  div(*this,three);
  exec_exp();
  sign = xs;
}

// solve ax+b =0; result in r1
void fp::x_solve(FpNum& r1, FpNum A, FpNum B)
{
  set_nb_sols(0);
  if(A.is_zero()) {
    the_parser.parse_error("equation is degenerated");
    r1.reset();
    return;
  }
  set_nb_sols(1);
  r1.div(B,A);
  r1.neg();
}

// Solves x^2+Bx+C=0; result in r1 and r2, returns number of solutions
int fp::qsolve(FpNum& r1, FpNum& r2, FpNum B, FpNum C)
{
  B /= 2;
  FpNum delta = B*B - C;
  if(!delta.sign) return 0; // no solutions
  delta.sqrt();
  if(B.sign) delta.sign = true; else delta.sign = false;
  r2 = -(delta + B);
  r1 = C/r2;
  return 2;
}

// solves Ax^2+Bx+C=0; result in r1 and r2, returns number of solutions
int fp::qsolve(FpNum& r1, FpNum& r2, FpNum A, FpNum B, FpNum C)
{
  return qsolve(r1,r2,B/A,C/A);
}

// Solve ax^2+bx+c =0;
void fp::x_solve(FpNum& r1, FpNum& r2, FpNum A, FpNum B, FpNum C)
{
  r1.reset();
  r2.reset();
  if(A.is_zero()) {
    the_parser.parse_error("equation is degenerated");
    x_solve(r1,B,C);
    return;
  }
  set_nb_sols(qsolve(r1,r2,A,B,C));
}

// Computes p, q and T for solving a cubic
void fp::csolve_aux(FpNum A, FpNum B, FpNum C, FpNum D, 
		    FpNum& p, FpNum&q, FpNum& T)
{
  D = D/A/2;
  T = -B/(3*A);
  p = C/(3*A) - (T*T);
  q = -(B*C)/(6*A*A) + D - (T*T*T);
}

// solve ax^3+bx^2+cx+d =0;
int fp::x_solve(FpNum& r1, FpNum& r2, FpNum&r3,
		FpNum A, FpNum B, FpNum C,FpNum D)
{
  r1.reset();
  r2.reset();
  r3.reset();
  if(A.is_zero()) {
    x_solve(r1,r2,B,C,D);
    return 0;
  }
  FpNum p, q, T;
  csolve_aux(A,B,C,D,p,q,T);
  FpNum a = p*p*p + q*q;
  if(a.sign) { // one real root
    a.sqrt();
    FpNum u=a-q,v=-(a+q);
    u.croot(); v.croot();
    r1 = u+v+T;
    set_nb_sols(1);
    return 1;
  }
  set_nb_sols(3);
  FpNum R=p,pi3;
  R.sign = true;
  R.sqrt();
  R.sign = q.sign;
  q.div(R); q.div(R); q.div(R);
  fp::arcsincos(r1,r2,q,fp_arccos_code);
  q = r1/3;
  R=R*2;
  pi3.init(0,1,47197551,196597746);
  r1 = q; r1.cosine(); r1.neg();
  r2 = pi3-q; r2.cosine();
  r3 = pi3+q; r3.cosine();
  r1 = r1*R+T;
  r2 = r2*R+T;
  r3 = r3*R+T;
  return 3;
}

bool fp::quad_aux(FpNum r, FpNum w, FpNum& y, FpNum& Ay)
{
  y = r;
  Ay = 8*y + w;
  return Ay.sign && !Ay.is_zero();
}

// solve ax^4+bx^3+cx^2+dx+e =0;
void fp::x_solve(FpNum& r1, FpNum& r2, FpNum&r3, FpNum&r4,
		FpNum A, FpNum B, FpNum C,FpNum D,FpNum E)
{
  r1.reset();
  r2.reset();
  r3.reset();
  r4.reset();
  if(A.is_zero()) {
    x_solve(r1,r2,r3,B,C,D,E);
    return;
  }
  bool special_case = compare(A,E)==0 && compare(B,D)==0;
  B /=A;
  C /=A;
  D /=A;
  E /=A;
  A.init(0,1,0,0);
  if(special_case) {
    FpNum one(0,1,0,0);
    FpNum two(0,2,0,0);
    FpNum u1,u2;
    C.sub(C,two);
    B.neg();
    int n = qsolve(u1,u2,B,C);
    if(n==0) { set_nb_sols(0); return; }
    int n1 = qsolve(r1,r2,u1,one);
    int n2 = qsolve(r3,r4,u2,one);
    if(n1==0 && n2>0) { r1=r3; r2=r4; }
    set_nb_sols(n1+n2);
    return;
  }
  FpNum b2,b3,b4,T;
  B.div(4); T = -B;
  b2 = B*B; b3= b2*B; b4 = b3*B;
  E = E-B*D+C*b2 +(-3)*b4;
  D = D +(-2)*B*C + 8*b3;
  C = C-6*b2;
  int n;
  {
    if(D.is_zero()) {
      FpNum u,v;
      n = qsolve(u,v,C,E);
      if(n==0) { set_nb_sols(0); return; }
      if(!u.sign && !v.sign)  { set_nb_sols(0); return; }
      if(!u.sign) { u = v; n = 2; }
      else if(!v.sign) { v = u; n = 2; }
      else n=4;
      u.sqrt(); v.sqrt();
      r1=v; r2=-v; r3 = u; r4 = -u;
    } else {
      FpNum w = (-4)*C, a(0,8,0,0), y,Ay,b,d;
      n = fp::x_solve(r1,r2,r3,a,w,(-8)*E,4*C*E-D*D);
      if(!quad_aux(r1,w,y,Ay)) {
	if(n==1) { set_nb_sols(0); return; }
	if(!quad_aux(r2,w,y,Ay)) {
	  if(!quad_aux(r3,w,y,Ay)) {
	    set_nb_sols(0); 
	    return;
	  }
	}
      }
      Ay.sqrt();
      b = Ay/2; d = D/Ay;
      int n1 = qsolve(r1,r2,b,y-d);
      int n2 = qsolve(r3,r4,-b,y+d);
      if(n1==0) { r1=r3; r2 = r4; }
      n = n1+n2;
      if(n==0) { set_nb_sols(0); return; }
    }
  }
  if(n) { r1 += T; r2+= T; }
  if(n>2) { r3 += T; r4 += T; }
  set_nb_sols(n);
}

// This adds the list A (expanded) followed by ..\relax
// If A=17, we will get 17..\relax
// If A is 1.23, we will get 1.23..\relax
void Parser::fp_send_one_arg (TokenList& res)
{
  scan_toks_edef(res);
  res.push_back(hash_table.dot_token);
  res.push_back(hash_table.dot_token);
  res.push_back(hash_table.relax_token);
}

// In the case of 17..\relax or 1.23..\relax
// we read in A the 17 or 1, in B nothing or 23, and parse until the \relax.
// After that, we put the digits into the buffer, skipping unwanted
// things, and read the number.
FpNum Parser::fp_read_value()
{
  Token name = fp_name;
  name_for_error = name;
  TokenList A = read_until_long(hash_table.dot_token);
  TokenList B = read_until_long(hash_table.dot_token);
  read_until_long(hash_table.relax_token);
  fp_in_buf.reset();
  scan_toks_edef(A);
  bool negative = false;
  for(;;) {
    if(A.empty()) break;
    Token x = A.front();
    if(x.is_plus_token()) A.pop_front();
    else if(x.is_minus_token()) {
      A.pop_front();
      negative = !negative;
    } else break;
  }
  if(negative) fp_in_buf.push_back('-'); else  fp_in_buf.push_back('+');
  for(;;) {
    if(A.empty()) break;
    Token x = A.front();
    A.pop_front();
    if(x.is_space_token()) break;
    if(x.is_digit_token())
      fp_in_buf.push_back(x.get_val()-other_t_offset);
    else fp_parse_error(x,name);
  }
  fp_in_buf.push_back('.');
  scan_toks_edef(B);
  for(;;) {
    if(B.empty()) break;
    Token x = B.front();
    B.pop_front();
    if(x.is_space_token()) break;
    if(x.is_digit_token())
      fp_in_buf.push_back(x.get_val()-other_t_offset);
    else fp_parse_error(x,name);
  }
  FpNum res;
  if(res.create(fp_in_buf))
    parse_error("overflow in FPread");
  if(tracing_commands()) 
    the_log << lg_startbrace << "FPread for " << name << "="<< res<< lg_endbrace;
  return res;
}

// ---------------------------------------------------------
// If the token list contains a string of N characters, is followed by X
// returns X and sets n to N. Return null token in case all tokens in the
// list are characters.
Token FpGenList::find_str(int& n) const
{
  tkbuf.reset();
  const_token_iterator C = value.begin();
  const_token_iterator E = value.end();
  n = 0;
  while(C != E) {
    Token x = *C;
    ++C;
    if(x.is_lowercase_token()) {
      tkbuf.push_back(x.val_as_letter());
      n++;
    } else return x;
  }
  return Token(0);
}

// Adds at the end of *this a space and S (of type letter)
// Assumes that S is ascii
void FpGenList::add_last_space(String S)
{
  push_back(the_parser.hash_table.space_token);
  for(int i=0;;i++) {
    uchar c = S[i];
    if(!c) return;
    if(c>128) err_ns::fatal_error("add last space: internal error");
    push_back(Token(letter_t_offset,c));
  }
}

// Adds at the end of *this a space, W, a second space and S (of type letter)
// Assumes that S is ascii
void FpGenList::add_last_space(TokenList& W, String S)
{
  value.push_back(the_parser.hash_table.space_token);
  push_back(W);
  add_last_space(S);
}

// splits at X. What is before is put in z. Cur element disappears.
 
void FpGenList::split_after(token_iterator X, TokenList& z)
{
  z.clear();
  z.splice(z.begin(), value, X, value.end());
  z.pop_front();
  z.swap(value);
}

// Finds the first x1 or x2. Splits there. before is put in z.
// Returns x1 or x2 (the token that is found) or 0 (if nothing is found).
Token FpGenList::split_at(Token x1, Token x2, TokenList& z)
{
  token_iterator X = value.begin();
  token_iterator E = value.end();
  for(;;) {
    if(X==E) return Token(0);
    Token x = *X;
    if(x==x1 || x== x2) {
      split_after(X,z);
      return x;
    }
    ++X;
  }
}

// The first N tokens are put in z.
// the Token that follows is recycled
void FpGenList::split_after(int n, TokenList& z)
{
  token_iterator X = value.begin();
  while(n>0) {
    ++X;
    n--;
  }
  split_after(X,z);
}

// In the case of word space something, converts to something space word
// provided that word is in the list.
// For instance sin 25 gives 25 sin
void FpGenList::fp_gen_app()
{
  while(!value.empty() && value.front().is_space_token())
    value.pop_front();
  int n;
  Token x = find_str(n);
  if(!x.is_space_token()) return;
  String S = tkbuf.c_str();
  if(strcmp(S,"add")==0 || strcmp(S,"sub")==0 || strcmp(S,"mul")==0 ||
     strcmp(S,"div")==0 || strcmp(S,"abs")==0 || strcmp(S,"neg") == 0 ||
     strcmp(S,"sgn")==0 || strcmp(S,"min")==0 || strcmp(S,"max") == 0 ||
     strcmp(S,"round")==0 || strcmp(S,"trunc")==0 || strcmp(S,"clip") == 0 ||
     strcmp(S,"exp")==0 || strcmp(S,"ln")==0 || strcmp(S,"pow") == 0 ||
     strcmp(S,"root")==0 || strcmp(S,"seed")==0 || strcmp(S,"random") == 0 ||
     strcmp(S,"sin")==0 || strcmp(S,"cos")==0 || strcmp(S,"sincos") == 0 ||
     strcmp(S,"tan")==0 || strcmp(S,"cot")==0 || strcmp(S,"tancot") == 0 ||
     strcmp(S,"arcsin")==0 || strcmp(S,"arccos")==0 || strcmp(S,"arcsincos") == 0 ||
     strcmp(S,"arctan")==0 || strcmp(S,"arccot")==0 || strcmp(S,"arctancot") == 0 ||
     strcmp(S,"pop")==0 || strcmp(S,"swap")==0 || strcmp(S,"copy") == 0) {
    push_back(the_parser.hash_table.space_token);
    TokenList z;
    split_after(n,z);
    push_back(z);
  }
}

// Converts A^B into B A pow. If there is no ^, then fp_gen_app is called
// Note that `sin 1^cos 2' gives `2 cos 1 sin pow'. Note order of args.
void FpGenList::fp_gen_exp()
{
  Token x (hat_t_offset, '^');
  TokenList aux;
  if(!split_at(x,x,aux).is_null()) {
    fp_gen_exp(); // handle B
    FpGenList Aux(aux);
    Aux.fp_gen_exp(); // handle A
    add_last_space(Aux.value,"pow");  // insert operator and after
  } else fp_gen_app();
}

// Converts A via fp_gen_exp, A*B into A B mul, where A and B are handled
// and A*B*C gives A B mul C mul. Handles division also.
// Order is preserved.
void FpGenList::fp_gen_mul()
{
  static const Token x1 = the_parser.hash_table.star_token;
  static const Token x2 (other_t_offset, '/');
  for(;;) {
    TokenList aux;
    Token y = split_at(x1,x2,aux);
    if(y.is_null()) { fp_gen_exp(); return; } // return if no star
    TokenList aux2;
    Token y2 = split_at(x1,x2,aux2);
    if(y2.is_null()) { // cas a*B
      fp_gen_exp();   // handle  B
      FpGenList Aux(aux);
      Aux.fp_gen_exp();  // handle A
      Aux.add_last_space(value, y==x1 ? "mul" : "div");
      value.swap(Aux.value);
      return;
    }
    FpGenList Aux(aux);
    Aux.fp_gen_exp(); //case A*B*C, handle A
    FpGenList Aux2(aux2); 
    Aux2.fp_gen_exp(); // handle B 
    Aux.add_last_space(Aux2.value,y==x1 ? "mul" : "div"); // we have A B mul
    Aux.push_back(the_parser.hash_table.space_token);
    Aux.push_back(y2); // insert the second op
    Aux.push_back(value);
    value.swap(Aux.value);
  }
}

// This handles addition subtraction. in the same fashion as fp_gen_mul
void FpGenList::fp_gen_add()
{
  Token x1 = the_parser.hash_table.plus_token;
  Token x2 = the_parser.hash_table.minus_token;
  for(;;) {
    TokenList aux;
    Token y = split_at(x1,x2,aux);
    if(y.is_null()) {
      fp_gen_mul();
      return;
    }
    TokenList aux2;
    Token y2 = split_at(x1,x2,aux2);
    if(y2.is_null()) {
      fp_gen_mul();
      FpGenList Aux(aux);
      Aux.fp_gen_mul();
      Aux.add_last_space(value,y==x1 ? "add" : "sub");
      value.swap(Aux.value);
      return;
    }
    FpGenList Aux(aux);
    Aux.fp_gen_mul();
    FpGenList Aux2(aux2);
    Aux2.fp_gen_mul();

    Aux.add_last_space(Aux2.value,y==x1 ? "add" : "sub");
    Aux.push_back(the_parser.hash_table.space_token);
    Aux.push_back(y2);
    Aux.push_back(value);
    value.swap(Aux.value);
  }
}

// This replaces foo,bar or foo:bar by  foo bar. 
// If a character has been replaced, a space is added at the end.
void FpGenList::fp_gen_komma()
{
  bool need_space = false;
  token_iterator X = value.begin();
  token_iterator E = value.end();
  Token x1 = the_parser.hash_table.comma_token;
  Token x2 = Token(other_t_offset,':');
  while( X != E) {
    if(*X==x1 || *X== x2) {
      need_space = true;
      *X =the_parser.hash_table.space_token;
    }
    ++X;
  }
  if(need_space) push_back(the_parser.hash_table.space_token);
  fp_gen_add();
}

// In case of X(Y)Z, puts X in A, Y in B, leaves Z in *this
// First closing parenthesis is considered. Then last opening one is taken.
bool FpGenList::split_at_p(TokenList& A, TokenList& B)
{
  Token x1 = Token(other_t_offset, '(');
  Token x2 = Token(other_t_offset, ')');
  token_iterator X = value.begin();
  token_iterator E = value.end();
  token_iterator p1 = E; // at open paren
  token_iterator p2 = E; // at close paren
  for(;;) {
    if(X==E) return false;
    Token x = *X;
    if(x==x2) {
      if(p2 != E)  return false; // failed
      p2 = X;
      break;
    }
    if(x==x1) p1 = X;
    ++X;
  }
  // close paren seen
  if(p1==E) return false;
  A.splice(A.begin(), value, value.begin(), p1);
  value.pop_front(); // remove open parenn
  B.splice(B.begin(), value, value.begin(), p2);
  value.pop_front(); // remove close paren
  return true;
}

// In case of sin(a+b)+cos(c+d), B is a+b then c+d
// converted to: a b plus and c d plus.
// The argument of fp_gen_comma is: sin a b plus and cos c d plus.
void FpGenList::to_postfix()
{
  fp_check_paren();
  for(;;) {
    TokenList A,B;
    if(split_at_p(A,B)) {
      FpGenList BB(B); 
      BB.fp_gen_komma(); // 
      value.swap(A); // A holds: +cos(c+d), and value holds sin
      push_back(the_parser.hash_table.space_token);
      push_back(BB.value); 
      push_back(A);
    } else break;
  }
  fp_gen_komma();
}

// In the case of (+3+4)*(-5-6),
// this  gives    (3+4)*(0-5-6),
void FpGenList::fp_check_paren()
{
  Token x0 (other_t_offset, '(');
  Token x1 = the_parser.hash_table.minus_token;
  Token x2 = the_parser.hash_table.plus_token;
  token_iterator X = value.begin();
  token_iterator E = value.end();
  while(X != E) {
    Token x = *X;
    if(x==x0) {
      token_iterator Y = X;
      ++Y;
      if(Y==E) return;
      Token y = *Y;
      if(y.is_space_token() || y==x2) value.erase(Y);
      else if(y==x1)  value.insert(Y,the_parser.hash_table.zero_token);
    }
    ++X;
  }
}

// Most functions have the form \FPfct\foo{arg}. This reads the \foo
// and puts it in fp_res
void Parser::fp_prepare()
{
  get_r_token();
  fp_res = cur_tok;
}

// This puts X, converted in a token list, into fp_res.
void Parser::fp_finish(FpNum X)
{
  TokenList res = X.to_list();
  new_macro(res,fp_res);
}

// \FPprint{foo} is the same as \foo.
// This is very, very strange
void Parser::fp_special_expand(TokenList& B)
{
  if(B.empty()) B.push_back(hash_table.zero_token);
  Token B1 = B.front();
  token_from_list(B1);
  if(cur_cmd_chr.get_cmd()==other_catcode) {
    back_input(B);
    return;
  }
  if(B1.not_a_cmd())
    my_csname("","",B,"FPprint", false); 
  else 
    back_input(B);  
  expand_when_ok(false);
}

void Parser::fp_print()
{
  TokenList A = mac_arg(); 
  fp_special_expand(A);
}

// \FPset{foo}{bar} => \def\foo{...} where .. is expansion of \bar
// error recovery is low.
void Parser::fp_set()
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  back_input(hash_table.CB_token);
  fp_special_expand(B);
  back_input(hash_table.OB_token);
  if(!A.empty()) {
    Token A1 = A.front();
    token_from_list(A1);
    if(A1.not_a_cmd()) {
      my_csname("","",A,"FPset", false);
      A.clear(); // needed ?
    }
  }
  back_input(A);
  see_new_def(false,false,user_cmd);
}

void Parser::fp_e_arcsin(subtypes i)
{
  fp_prepare();
  if(i==fp_arcsincos_code ||i==fp_arctancot_code) {
    get_r_token();
    fp_res2 = cur_tok;
  }
  FpNum X = fp_read_value();
  FpNum a,b;
  fp::arcsincos(a,b,X,i);
  fp_finish(a);
  if(i==fp_arcsincos_code ||i==fp_arctancot_code) {
    fp_res = fp_res2;
    fp_finish(b);
  }
}

// Finish a boolean. If res is true, this is equivalent to
// \let\foo\iftroo\foo, where \foo is fp_test_token.
void Parser::fp_boolean(bool res)
{
  back_input(res ? hash_table.iftrue_token : hash_table.iffalse_token);
  back_input(fp_test_token);
  see_let(false);
  back_input(fp_test_token);
}

// FP binary comparison: lt, gt or eq
void Parser::fp_eval_lt(subtypes w)
{
  FpNum X = fp_read_value();
  FpNum Y = fp_read_value();
  int cp = fp::compare(X,Y);
  bool res = false;
  if(w==fp_iflt_code) res = cp==-1;
  else if(w==fp_ifgt_code) res = cp==1;
  else res = cp==0;
  fp_boolean(res);
}

// FP unary comparison: pos neg or zero.
void Parser::fp_eval_unarytest(subtypes w)
{
  FpNum X = fp_read_value();
  bool res = false;
  if(w==fp_ifneg_code) res = !X.sign;
  else if(w==fp_ifpos_code) res = X.sign;
  else if(w==fp_ifzero_code) res = X.is_zero();
  else res = (X.data[2]==0 && X.data[3]==0);
  fp_boolean(res);
}

// This reads the seed of the random number generator
void Parser::fp_setseed()
{
  TokenList res = mac_arg();
  fp_send_one_arg(res);
  back_input(res);
  fp_name = fps[fp_seed_code];
  FpNum X = fp_read_value();
  fp_seed = X.data[1];
}

// algo of Lewis, Goodman Miller 1969
void FpNum::random()
{
  Digit cst_q = 127773;
  Digit cst_m = 2147483647;
  Digit S = the_parser.eqtb_int_table[fpseed_code].get_val();
  Digit xia = S / cst_q;
  Digit xib = S % cst_q;
  int w = xib*16807 - xia *2836;
  if(w<=0) w += cst_m;
  the_parser.eqtb_int_table[fpseed_code].set_val(w);
  fp_rand2.data[1] = w;
  div(fp_rand2,fp_rand1);
}

// Code of \FPeval.
void Parser::fp_e_eval()
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  FpGenList evaluator(B);
  evaluator.to_postfix();
  if(tracing_commands())  the_log << lg_startbrace << "FPpostfix "
				  << evaluator.value << lg_endbrace;
  back_input(hash_table.CB_token);
  back_input(evaluator.value);
  back_input(hash_table.OB_token);
  back_input(A);
  back_input(fps[fp_upn_code]);
}

// Fpsolve: linear equations.
void Parser::fp_e_lsolve()
{
  get_r_token();
  fp_res = cur_tok;
  FpNum X = fp_read_value();
  FpNum Y = fp_read_value();
  FpNum Z;
  fp::x_solve(Z,X,Y);
  fp_finish(Z);
}

// Fpsolve: quadratic equations.
void Parser::fp_e_qsolve()
{
  get_r_token();
  fp_res = cur_tok;
  get_r_token();
  fp_res2 = cur_tok;
  FpNum X = fp_read_value();
  FpNum Y = fp_read_value();
  FpNum Z = fp_read_value();
  FpNum r1,r2;
  fp::x_solve(r1,r2,X,Y,Z);
  fp_finish(r1);
  fp_res = fp_res2;
  fp_finish(r2);
}

// Fpsolve: cubic equations.
void Parser::fp_e_csolve()
{
  get_r_token();
  fp_res = cur_tok;
  get_r_token();
  fp_res2 = cur_tok;
  get_r_token();
  fp_res3 = cur_tok;
  FpNum X = fp_read_value();
  FpNum Y = fp_read_value();
  FpNum Z = fp_read_value();
  FpNum T = fp_read_value();
  FpNum r1,r2,r3;
  fp::x_solve(r1,r2,r3,X,Y,Z,T);
  fp_finish(r1);
  fp_res = fp_res2;
  fp_finish(r2);
  fp_res = fp_res3;
  fp_finish(r3);
}

// Fpsolve: degree 4 equations.
void Parser::fp_e_qqsolve()
{
  get_r_token();
  fp_res = cur_tok;
  get_r_token();
  fp_res2 = cur_tok;
  get_r_token();
  fp_res3 = cur_tok;
  get_r_token();
  fp_res4 = cur_tok;
  FpNum X = fp_read_value();
  FpNum Y = fp_read_value();
  FpNum Z = fp_read_value();
  FpNum T = fp_read_value();
  FpNum U = fp_read_value();
  FpNum r1,r2,r3,r4;
  fp::x_solve(r1,r2,r3,r4,X,Y,Z,T,U);
  fp_finish(r1);
  fp_res = fp_res2;
  fp_finish(r2);
  fp_res = fp_res3;
  fp_finish(r3);
  fp_res = fp_res4;
  fp_finish(r4);
}

// upn. Thes stack is a comm-separated list of tokens.
// This adds L in front.
void FpStack::push_upn(TokenList& L)
{
  if(!empty()) push_front(the_parser.hash_table.comma_token);
  push_front(L);
}

// This adds a fp number in front of the stack.
void FpStack::push_upn(FpNum x)
{
  if(!empty()) push_front(the_parser.hash_table.comma_token);
  TokenList xv = x.to_list();
  push_front(xv);
}

// This removes the first N tokens from the list
void FpGenList::remove_first_n (int n)
{
  while(n>0) { value.pop_front(); n--;}
}

// This takes all tokens from this, until comma or end of list
// and puts them in L
void FpStack::pop_upn(TokenList& L)
{
  if(empty()) {
    the_parser.parse_error("UPN stack empty");
    return;
  }
  for(;;) {
    if(empty()) return;
    Token x = value.front();
    value.pop_front();
    if(x.is_comma_token()) return;
    L.push_back(x);
  }
}

// This takes all tokens from this, until comma or end of list
// converts them into a FpNumber and puts the result in x.
void FpStack::pop_upn(FpNum& x)
{
  x.reset();
  x.sign=true; // Now x is zero
  TokenList L;
  pop_upn(L);
  if(L.empty()) return;
  the_parser.fp_send_one_arg(L);
  the_parser.back_input(L);
  x = the_parser.fp_read_value();
}

void FpGenList::remove_spaces()
{
  TokenList&L = value;
  while(!L.empty() && L.front().is_space_token()) L.pop_front();
  while(!L.empty() && L.back().is_space_token()) L.pop_back();
}


// upn evaluator. Is a rather big function.
void Parser::upn_eval(TokenList& l)
{
  FpStack& S = upn_stack;
  FpGenList L(l);
  L.remove_spaces();
  Token t1 = the_parser.hash_table.space_token;
  Token t2 = the_parser.hash_table.newline_token;
  TokenList z;
  if(L.split_at(t1,t2,z).get_val() != 0) {
    upn_eval(z);
    upn_eval(L.value);
    return;
  }
  int n;
  Token x = L.find_str(n);
  String str = tkbuf.c_str();
  TokenList a1,a2;
  FpNum x1,x2,x3,x4;
  if(tracing_commands()) 
    the_log << lg_startbrace << "FPupcmd " << (n==0 ? "??" : str) << lg_endbrace;

  if(n==4 && strcmp(str,"copy")==0) {
    L.remove_first_n(n);
    S.pop_upn(a1);
    TokenList a2 = a1;
    S.push_upn(a2);
    S.push_upn(a1);
    return;
  }
  if(n==4 && strcmp(str,"swap")==0) {
    L.remove_first_n(n);
    S.pop_upn(a1);
    S.pop_upn(a2);
    S.push_upn(a1);
    S.push_upn(a2);
    return;
  }
  if(n==3 && strcmp(str,"pop")==0) {
    L.remove_first_n(n);
    S.pop_upn(a1);
    return;
  }
  if(n==9 &&strcmp(str,"arctancot")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::arcsincos(x3,x4,x1,fp_arctancot_code);
    S.push_upn(x4);
    S.push_upn(x3);
    return;
  }  
  if(n==6 &&strcmp(str,"arccot")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::arcsincos(x3,x4,x1,fp_arccot_code);
    S.push_upn(x3);
    return;
  }  
  if(n==6 &&strcmp(str,"arctan")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::arcsincos(x3,x4,x1,fp_arctan_code);
    S.push_upn(x3);
    return;
  }  
  if(n==9 &&strcmp(str,"arcsincos")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::arcsincos(x3,x4,x1,fp_arcsincos_code);
    S.push_upn(x4);
    S.push_upn(x3);
    return;
  }  
  if(n==6 &&strcmp(str,"arcsin")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::arcsincos(x3,x4,x1,fp_arcsin_code);
    S.push_upn(x3);
    return;
  }  
  if(n==6 &&strcmp(str,"arccos")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::arcsincos(x3,x4,x1,fp_arccos_code);
    S.push_upn(x3);
    return;
  }  
  if(n==6 &&strcmp(str,"tancot")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::sincos(x3,x4,x1,fp_tancot_code);
    S.push_upn(x4);
    S.push_upn(x3);
    return;
  }  
  if(n==3 &&strcmp(str,"cot")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::sincos(x3,x4,x1,fp_cot_code);
    S.push_upn(x3);
    return;
  }  
  if(n==3 &&strcmp(str,"tan")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::sincos(x3,x4,x1,fp_tan_code);
    S.push_upn(x3);
    return;
  }  
  if(n==6 &&strcmp(str,"sincos")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::sincos(x3,x4,x1,fp_sincos_code);
    S.push_upn(x4);
    S.push_upn(x3);
    return;
  }  
  if(n==3 &&strcmp(str,"sin")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::sincos(x3,x4,x1,fp_sin_code);
    S.push_upn(x3);
    return;
  }  
  if(n==3 &&strcmp(str,"cos")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    fp::sincos(x3,x4,x1,fp_cos_code);
    S.push_upn(x3);
    return;
  }  
  if(n==2 &&strcmp(str,"pi")==0) {
    L.remove_first_n(n);
    S.push_upn(pi_table[4]);
    return;
  }
  if(n==6 &&strcmp(str,"random")==0) {
    L.remove_first_n(n);
    x1.random();
    S.push_upn(x1);
    return;
  }
  if(n==4 &&strcmp(str,"seed")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    eqtb_int_table[fpseed_code].set_val(x1.data[1]);
    return;
  }
  if(n==4 &&strcmp(str,"root")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    S.pop_upn(x2);
    x3.pow(x1,x2,fp_root_code);
    S.push_upn(x3);
    return;
  }
  if(n==3 &&strcmp(str,"pow")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    S.pop_upn(x2);
    x3.pow(x1,x2,fp_pow_code);
    S.push_upn(x3);
    return;
  }
  if(n==3 &&strcmp(str,"exp")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    x1.exec_exp();
    S.push_upn(x1);
    return;
  }
  if(n==2 &&strcmp(str,"ln")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    x1.exec_ln();
    S.push_upn(x1);
    return;
  }
  if(n==1 &&strcmp(str,"e")==0) {
    L.remove_first_n(n);
    x1 = e_powers[1];
    S.push_upn(x1);
    return;
  }
  if(n==4 &&strcmp(str,"clip")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);
    S.push_upn(x1);
    return;
  }
  if(n==5 &&strcmp(str,"round")==0) {
    L.remove_first_n(n);
    S.pop_upn(x2);
    S.pop_upn(x1);
    x1.round(x2.data[1]);
    x1.round0();
    S.push_upn(x1);
    return;
  }
  if(n==5 &&strcmp(str,"trunc")==0) {
    L.remove_first_n(n);
    S.pop_upn(x2);
    S.pop_upn(x1);
    x1.truncate(x2.data[1]);
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"min")==0) {
    L.remove_first_n(n);
    S.pop_upn(x2);  
    S.pop_upn(x1);
    int res = fp::compare(x2,x1);
    S.push_upn(res>0?x1:x2);
    return;
  }
  if(n==3 &&strcmp(str,"max")==0) {
    L.remove_first_n(n);
    S.pop_upn(x2);
    S.pop_upn(x1);  
    int res = fp::compare(x1,x2);
    S.push_upn(res>0? x1:x2);
    return;
  }
  if(n==3 &&strcmp(str,"sgn")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);  
    x1.sgn();
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"neg")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);  
    x1.neg();
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"abs")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);  
    x1.sign=true;
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"add")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);  
    S.pop_upn(x2);  
    x1.add(x2);
    S.push_upn(x1);
    return;
  }
  if(n==0 &&x.is_plus_token()) {
    L.remove_first_n(1);
    S.pop_upn(x1);  
    S.pop_upn(x2);  
    x1.add(x2);
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"sub")==0) {
    L.remove_first_n(n);
    S.pop_upn(x2);  
    S.pop_upn(x1);  
    x2.neg();
    x1.add(x2);
    S.push_upn(x1);
    return;
  }
  if(n==0 &&x.is_minus_token()) {
    L.remove_first_n(1);
    S.pop_upn(x1);  
    S.pop_upn(x2);  
    x2.neg();
    x1.add(x2);
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"mul")==0) {
    L.remove_first_n(n);
    S.pop_upn(x1);  
    S.pop_upn(x2);  
    x1.mul(x1,x2);
    S.push_upn(x1);
    return;
  }
  if(n==0 &&x.is_star_token()) {
    L.remove_first_n(1);
    S.pop_upn(x1);  
    S.pop_upn(x2);  
    x1.mul(x1,x2);
    S.push_upn(x1);
    return;
  }
  if(n==3 &&strcmp(str,"div")==0) {
    L.remove_first_n(n);
    S.pop_upn(x2);  
    S.pop_upn(x1);  
    x1.div(x1,x2);
    S.push_upn(x1);
    return;
  }
  if(n==0 &&x.is_slash_token()) {
    L.remove_first_n(1);
    S.pop_upn(x1);  
    S.pop_upn(x2);  
    x1.div(x1,x2);
    S.push_upn(x1);
    return;
  }
  back_input(hash_table.CB_token);
  fp_special_expand(L.value);
  back_input(hash_table.OB_token);
  TokenList w = mac_arg();
  S.push_upn(w);
}

// Top-level upn function. 
void Parser::fp_e_upn()
{
  fp_prepare();
  upn_stack.clear();
  TokenList L = mac_arg();
  if(L.empty()) L.push_back(hash_table.zero_token);
  upn_eval(L);  
  if(upn_stack.empty()) {
    parse_error("UPN stack empty");
    upn_stack.push_front(hash_table.zero_token);
  }
  FpNum x;
  upn_stack.pop_upn(x);
  if(!upn_stack.empty())
    parse_error("Evaluation results in multiple values");
  fp_finish(x);
}

// This procedure creates all tokens etc, needed for the fp package.
void Parser::boot_fp()
{
  fp_rand1.data[0] = 2;
  fp_rand1.data[1] = 147483647;

  fp_tmp_token = hash_table.primitive("FP@tmp", relax_cmd,relax_code);
  fp_test_token = hash_table.primitive("ifFPtest", relax_cmd,relax_code);

  hash_table.primitive("FPident", fp_cmd,fp_ident_code);
  hash_table.primitive("FPprint", fp_cmd,fp_print_code);
  hash_table.primitive("FPset", fp_cmd,fp_set_code);
  hash_table.primitive("FPadd", fp_cmd,fp_add_code);
  hash_table.primitive("FPsub", fp_cmd,fp_sub_code);
  hash_table.primitive("FPmul", fp_cmd,fp_mul_code);
  hash_table.primitive("FPmin", fp_cmd,fp_min_code);
  hash_table.primitive("FPmax", fp_cmd,fp_max_code);
  hash_table.primitive("FPdiv", fp_cmd,fp_div_code);
  hash_table.primitive("FPabs", fp_cmd,fp_abs_code);
  hash_table.primitive("FPneg", fp_cmd,fp_neg_code);
  hash_table.primitive("FPsgn", fp_cmd,fp_sgn_code);
  hash_table.primitive("FPiflt", fpif_cmd,fp_iflt_code);
  hash_table.primitive("FPifeq", fpif_cmd,fp_ifeq_code);
  hash_table.primitive("FPifgt", fpif_cmd,fp_ifgt_code);
  hash_table.primitive("FPifneg", fpif_cmd,fp_ifneg_code);
  hash_table.primitive("FPifpos", fpif_cmd,fp_ifpos_code);
  hash_table.primitive("FPifzero", fpif_cmd,fp_ifzero_code);
  hash_table.primitive("FPifint", fpif_cmd,fp_ifint_code);
  hash_table.primitive("FPround", fp_cmd,fp_round_code);
  hash_table.primitive("FPclip", fp_cmd,fp_clip_code);
  hash_table.primitive("FPtrunc", fp_cmd,fp_trunc_code);
  fps[fp_eval_code]=hash_table.primitive("FPeval", fp_cmd,fp_eval_code);
  hash_table.primitive("FPupn", fp_cmd,fp_upn_code);
  fps[fp_random_code]=hash_table.primitive("FPrandom", fp_cmd,fp_random_code);
  //  fps[fp_seed_code]=hash_table.primitive("FPseed", fp_cmd,fp_seed_code);
  fps[fp_seed_code]=hash_table.primitive("FPseed", assign_int_cmd,fpseed_code);
  eqtb_int_table[fpseed_code].set_val(123456789);
  fps[fp_e_code]=hash_table.primitive("FPe", fp_cmd,fp_e_code);
  fps[fp_pascal_code]=hash_table.primitive("FPpascal", fp_cmd,fp_pascal_code);
  hash_table.primitive("FPexp", fp_cmd,fp_exp_code);
  hash_table.primitive("FPln", fp_cmd,fp_ln_code);
  hash_table.primitive("FPpow", fp_cmd,fp_pow_code);
  hash_table.primitive("FProot", fp_cmd,fp_root_code);
  hash_table.primitive("FPsin", fp_cmd,fp_sin_code);
  hash_table.primitive("FPcos", fp_cmd,fp_cos_code);
  hash_table.primitive("FPsincos", fp_cmd,fp_sincos_code);
  hash_table.primitive("FPtan", fp_cmd,fp_tan_code);
  hash_table.primitive("FPcot", fp_cmd,fp_cot_code);
  hash_table.primitive("FPtancot", fp_cmd,fp_tancot_code);
  hash_table.primitive("FParcsin", fp_cmd,fp_arcsin_code);
  hash_table.primitive("FParccos", fp_cmd,fp_arccos_code);
  hash_table.primitive("FParcsincos", fp_cmd,fp_arcsincos_code);
  hash_table.primitive("FParctan", fp_cmd,fp_arctan_code);
  hash_table.primitive("FParccot", fp_cmd,fp_arccot_code);
  hash_table.primitive("FParctancot", fp_cmd,fp_arctancot_code);
  hash_table.primitive("FPlsolve", fp_cmd,fp_lsolve_code);
  hash_table.primitive("FPqsolve", fp_cmd,fp_qsolve_code);
  hash_table.primitive("FPcsolve", fp_cmd,fp_csolve_code);
  hash_table.primitive("FPqqsolve", fp_cmd,fp_qqsolve_code);

  fps[fp_ident_code] = hash_table.primitive("FP@ident", fpi_cmd,fp_ident_code);
  fps[fp_add_code] = hash_table.primitive("FP@add", fpi_cmd,fp_add_code);
  fps[fp_sub_code] = hash_table.primitive("FP@sub", fpi_cmd,fp_sub_code);
  fps[fp_mul_code] = hash_table.primitive("FP@mul", fpi_cmd,fp_mul_code);
  fps[fp_min_code] = hash_table.primitive("FP@min", fpi_cmd,fp_min_code);
  fps[fp_max_code] = hash_table.primitive("FP@max", fpi_cmd,fp_max_code);
  fps[fp_div_code] = hash_table.primitive("FP@div", fpi_cmd,fp_div_code);
  fps[fp_abs_code] = hash_table.primitive("FP@abs", fpi_cmd,fp_abs_code);
  fps[fp_neg_code] = hash_table.primitive("FP@neg", fpi_cmd,fp_neg_code);
  fps[fp_sgn_code] = hash_table.primitive("FP@sgn", fpi_cmd,fp_sgn_code);
  fps[fp_round_code] = hash_table.primitive("FP@round", fpi_cmd,fp_round_code);
  fps[fp_trunc_code] = hash_table.primitive("FP@trunc", fpi_cmd,fp_trunc_code);
  fps[fp_clip_code] = hash_table.primitive("FP@clip", fpi_cmd,fp_clip_code);
  fps[fp_iflt_code] = hash_table.primitive("FP@iflt", fpi_cmd,fp_iflt_code);
  fps[fp_ifeq_code] = hash_table.primitive("FP@ifeq", fpi_cmd,fp_ifeq_code);
  fps[fp_ifgt_code] = hash_table.primitive("FP@ifgt", fpi_cmd,fp_ifgt_code);
  fps[fp_ifneg_code] = hash_table.primitive("FP@ifneg", fpi_cmd,fp_ifneg_code);
  fps[fp_ifpos_code] = hash_table.primitive("FP@ifpos", fpi_cmd,fp_ifpos_code);
  fps[fp_ifzero_code] = hash_table.primitive("FP@ifzero", fpi_cmd,fp_ifzero_code);
  fps[fp_ifint_code] = hash_table.primitive("FP@ifint", fpi_cmd,fp_ifint_code);
  fps[fp_exp_code] = hash_table.primitive("FP@exp", fpi_cmd,fp_exp_code);
  fps[fp_ln_code] = hash_table.primitive("FP@ln", fpi_cmd,fp_ln_code);
  fps[fp_pow_code] = hash_table.primitive("FP@pow", fpi_cmd,fp_pow_code);
  fps[fp_root_code] = hash_table.primitive("FP@root", fpi_cmd,fp_root_code);
  fps[fp_sin_code] = hash_table.primitive("FP@sin", fpi_cmd,fp_sin_code);
  fps[fp_cos_code] = hash_table.primitive("FP@cos", fpi_cmd,fp_cos_code);
  fps[fp_upn_code]= hash_table.primitive("FP@upn", fpi_cmd,fp_upn_code);
fps[fp_sincos_code] = hash_table.primitive("FP@sincos", fpi_cmd,fp_sincos_code);
  fps[fp_tan_code] = hash_table.primitive("FP@tan", fpi_cmd,fp_tan_code);
  fps[fp_cot_code] = hash_table.primitive("FP@cot", fpi_cmd,fp_cot_code);
  fps[fp_tancot_code] = hash_table.primitive("FP@tancot", fpi_cmd,fp_tancot_code);
  fps[fp_arcsin_code] = hash_table.primitive("FP@arcsin", fpi_cmd,fp_arcsin_code);
  fps[fp_arccos_code] = hash_table.primitive("FP@arccos", fpi_cmd,fp_arccos_code);
  fps[fp_arcsincos_code] = hash_table.primitive("FP@arcsincos", fpi_cmd,fp_arcsincos_code);
  fps[fp_arctan_code] = hash_table.primitive("FP@arctan", fpi_cmd,fp_arctan_code);
  fps[fp_arccot_code] = hash_table.primitive("FP@arccot", fpi_cmd,fp_arccot_code);
  fps[fp_arctancot_code] = hash_table.primitive("FP@arctancot", fpi_cmd,fp_arctancot_code);
  fps[fp_lsolve_code] = hash_table.primitive("FP@lsolve", fpi_cmd,fp_lsolve_code);
  fps[fp_qsolve_code] = hash_table.primitive("FP@qsolve", fpi_cmd,fp_qsolve_code);
  fps[fp_csolve_code] = hash_table.primitive("FP@csolve", fpi_cmd,fp_csolve_code);
  fps[fp_qqsolve_code] = hash_table.primitive("FP@qqsolve", fpi_cmd,fp_qqsolve_code);
  LinePtr L;
  L.insert("%% Begin bootstrap commands for FP"); 
  L.insert("\\def\\FPe{2.718281828459045235}");
  L.insert("\\let\\ifFPtest\\iftrue");
  lines.splice_first(L);
  e_powers[0].init(0,1,0,0);
  e_powers[1].init(0,2,718281828, 459045235);
  e_powers[2].init(0,7,389056098, 930650227);
  e_powers[3].init(0,20,85536923, 187667741);
  e_powers[4].init(0,54,598150033, 144239078);
  e_powers[5].init(0,148,413159102,576603421);
  e_powers[6].init(0,403,428793492,735122608);
  e_powers[7].init(0,1096,633158428,458599264);
  e_powers[8].init(0,2980,957987041,728274744);
  e_powers[9].init(0,8103,83927575,384007710);
  e_powers[10].init(0,22026,465794806,716516958);
  e_powers[11].init(0,59874,141715197,818455326);
  e_powers[12].init(0,162754,791419003,920808005);
  e_powers[13].init(0,442413,392008920,503326103);
  e_powers[14].init(0,1202604,284164776,777749237);
  e_powers[15].init(0,3269017,372472110,639301855);
  e_powers[16].init(0,8886110,520507872,636763024);
  e_powers[17].init(0,24154952,753575298,214775435);
  e_powers[18].init(0,65659969,137330511,138786503);
  e_powers[19].init(0,178482300,963187260,844910034);
  e_powers[20].init(0,485165195,409790277,969106831);
  e_powers[21].init(1,318815734,483214697,209998884);
  e_powers[22].init(3,584912846,131591561,681159946);
  e_powers[23].init(9,744803446,248902600,34632685);
  e_powers[24].init(26,489122129,843472294,139162153);
  e_powers[25].init(72,4899337,385872524,161351466);
  e_powers[26].init(195,729609428,838764269,776397876);
  e_powers[27].init(532,48240601,798616683,747304341);
  e_powers[28].init(1446,257064291,475173677,47422997);
  e_powers[29].init(3931,334297144,42074388,620580844);
  e_powers[30].init(10686,474581524,462146990,468650741);
  e_powers[31].init(29048,849665247,425231085,682111680);
  e_powers[32].init(78962,960182680,695160978,22635108);
  e_powers[33].init(214643,579785916,64624297,761531261);
  e_powers[34].init(583461,742527454,881402902,734610391);
  e_powers[35].init(1586013,452313430,728129644,625774660);
  e_powers[36].init(4311231,547115195,227113422,292856925);
  e_powers[37].init(11719142,372802611,308772939,791190195);
  e_powers[38].init(31855931,757113756,220328671,701298646);
  e_powers[39].init(86593400,423993746,953606932,719264934);
  e_powers[40].init(235385266,837019985,407899910,749034805);
  e_powers[41].init(639843493,530054949,222663403,515570819);

  neg_e_powers[0].init(0,1,0,0);
  neg_e_powers[1].init(0,0,367879441,171442322);
  neg_e_powers[2].init(0,0,135335283,236612692);
  neg_e_powers[3].init(0,0,49787068,367863943);
  neg_e_powers[4].init(0,0,18315638,888734180);
  neg_e_powers[5].init(0,0,6737946,999085467);
  neg_e_powers[6].init(0,0,2478752,176666358);
  neg_e_powers[7].init(0,0,911881,965554516);
  neg_e_powers[8].init(0,0,335462,627902512);
  neg_e_powers[9].init(0,0,123409,804086680);
  neg_e_powers[10].init(0,0,45399,929762485);
  neg_e_powers[11].init(0,0,16701,700790246);
  neg_e_powers[12].init(0,0,6144,212353328);
  neg_e_powers[13].init(0,0,2260,329406981);
  neg_e_powers[14].init(0,0,831,528719104);
  neg_e_powers[15].init(0,0,305,902320502);
  neg_e_powers[16].init(0,0,112,535174719);
  neg_e_powers[17].init(0,0,41,399377188);
  neg_e_powers[18].init(0,0,15,229979745);
  neg_e_powers[19].init(0,0,5,602796438);
  neg_e_powers[20].init(0,0,2,61153622);
  neg_e_powers[21].init(0,0,0,758256043);
  neg_e_powers[22].init(0,0,0,278946809);
  neg_e_powers[23].init(0,0,0,102618796);
  neg_e_powers[24].init(0,0,0,37751345);
  neg_e_powers[25].init(0,0,0,13887944);
  neg_e_powers[26].init(0,0,0,5109089);
  neg_e_powers[27].init(0,0,0,1879529);
  neg_e_powers[28].init(0,0,0,691440);
  neg_e_powers[29].init(0,0,0,254367);
  neg_e_powers[30].init(0,0,0,93576);
  neg_e_powers[31].init(0,0,0,34425);
  neg_e_powers[32].init(0,0,0,12664);
  neg_e_powers[33].init(0,0,0,4659);
  neg_e_powers[34].init(0,0,0,1714);
  neg_e_powers[35].init(0,0,0,631);
  neg_e_powers[36].init(0,0,0,232);
  neg_e_powers[37].init(0,0,0,85);
  neg_e_powers[38].init(0,0,0,31);
  neg_e_powers[39].init(0,0,0,12);
  neg_e_powers[40].init(0,0,0,4);
  neg_e_powers[41].init(0,0,0,2);
  neg_e_powers[42].init(0,0,0,1);
 
  log_table[0].init(0,0,0,0);  
  log_table[1].init(0,0,693147180,559945309);  
  log_table[2].init(0,1,386294361,119890618);  
  log_table[3].init(0,2,79441541,679835928);  

  log10_table[0].init(0,0,0,0);
  log10_table[1].init(0,2,302585092,994045684);
  log10_table[2].init(0,4,605170185,988091368);
  log10_table[3].init(0,6,907755278,982137052);
  log10_table[4].init(0,9,210340371,976182736);
  log10_table[5].init(0,11,512925464,970228420);
  log10_table[6].init(0,13,815510557,964274104);
  log10_table[7].init(0,16,118095650,958319788);
  log10_table[8].init(0,18,420680743,952365472);
  log10_table[9].init(0,20,723265836,946411156);
  log10_table[10].init(0,23,25850929,940456840);
  log10_table[11].init(0,25,328436022,934502524);
  log10_table[12].init(0,27,631021115,928548208);
  log10_table[13].init(0,29,933606208,922593892);
  log10_table[14].init(0,32,236191301,916639576);
  log10_table[15].init(0,34,538776394,910685260);
  log10_table[16].init(0,36,841361487,904730944);
  log10_table[17].init(0,39,143946580,898776628);
  log10_table[18].init(0,41,446531673,892822312);
  
  pi_table[0].init(628318530,717958647,692528676,655900577); // 2\pi *10^17
  pi_table[1].init(0,0,785398163,397448310); //  1\pi/4
  pi_table[2].init(0,1,570796326,794896619); //  2\pi/4
  pi_table[3].init(0,2,356194490,192344929); //  3\pi/4
  pi_table[4].init(0,3,141592653,589793238); //  4\pi/4
  pi_table[5].init(0,3,926990816,987241548); //  5\pi/4
  pi_table[6].init(0,4,712388980,384689858); //  6\pi/4
  pi_table[7].init(0,5,497787143,782138167); //  7\pi/4
  pi_table[8].init(0,6,283185307,179586477); //  8\pi/4
}

String CmdChr::token_fp_names() const
{
  switch(chr) {
  case fp_ident_code: return "FPident";
  case fp_print_code: return "FPprint";
  case fp_set_code: return "FPset";
  case fp_add_code: return "FPadd";
  case fp_sub_code: return "FPsub";
  case fp_mul_code: return "FPmul";
  case fp_min_code: return "FPmin";
  case fp_max_code: return "FPmax";
  case fp_div_code: return "FPdiv";
  case fp_abs_code: return "FPabs";
  case fp_neg_code: return "FPneg";
  case fp_sgn_code: return "FPsgn";
  case fp_iflt_code: return "FPiflt";
  case fp_ifeq_code: return "FPifeq";
  case fp_ifgt_code: return "FPifgt";
  case fp_ifneg_code: return "FPifneg";
  case fp_ifpos_code: return "FPifpos";
  case fp_ifzero_code: return "FPifzero";
  case fp_ifint_code: return "FPifint";
  case fp_round_code: return "FPround";
  case fp_clip_code: return "FPclip";
  case fp_trunc_code: return "FPtrunc";
  case fp_eval_code: return "FPeval";
  case fp_upn_code: return "FPupn";
  case fp_random_code: return "FPrandom";
  case fp_seed_code: return "FPseed";
  case fp_e_code: return "FPe";
  case fp_pascal_code: return "FPpascal";
  case fp_exp_code: return "FPexp";
  case fp_ln_code: return "FPln";
  case fp_pow_code: return "FPpow";
  case fp_root_code: return "FProot";
  case fp_sin_code: return "FPsin";
  case fp_cos_code: return "FPcos";
  case fp_sincos_code: return "FPsincos";
  case fp_tan_code: return "FPtan";
  case fp_cot_code: return "FPcot";
  case fp_tancot_code: return "FPtancot";
  case fp_arcsin_code: return "FParcsin";
  case fp_arccos_code: return "FParccos";
  case fp_arcsincos_code: return "FParcsincos";
  case fp_arctan_code: return "FParctan";
  case fp_arccot_code: return "FParccot";
  case fp_arctancot_code: return "FParctancot";
  case fp_lsolve_code: return "FPlsolve";
  case fp_qsolve_code: return "FPqsolve";
  case fp_csolve_code: return "FPcsolve";
  case fp_qqsolve_code: return "FPqqsolve";
  default: return 0;
  }
}


String CmdChr::token_fpi_names() const
{
  switch(chr) {
  case fp_upn_code: return "FP@upn";
  case fp_ident_code: return "FP@ident";
  case fp_add_code: return "FP@add";
  case fp_sub_code: return "FP@sub";
  case fp_mul_code: return "FP@mul";
  case fp_min_code: return "FP@min";
  case fp_max_code: return "FP@max";
  case fp_div_code: return "FP@div";
  case fp_abs_code: return "FP@abs";
  case fp_neg_code: return "FP@neg";
  case fp_sgn_code: return "FP@sgn";
  case fp_round_code: return "FP@round";
  case fp_trunc_code: return "FP@trunc";
  case fp_clip_code: return  "FP@clip";
  case fp_iflt_code: return  "FP@iflt";
  case fp_ifeq_code: return  "FP@ifeq";
  case fp_ifgt_code: return  "FP@ifgt";
  case fp_ifneg_code: return "FP@ifneg";
  case fp_ifpos_code: return "FP@ifpos";
  case fp_ifzero_code: return "FP@ifzero";
  case fp_ifint_code: return "FP@ifint";
  case fp_exp_code: return "FP@exp";
  case fp_ln_code: return "FP@ln";
  case fp_pow_code: return "FP@pow";
  case fp_root_code: return "FP@root";
  case fp_sin_code: return "FP@sin";
  case fp_cos_code: return "FP@cos";
  case fp_sincos_code: return "FP@sincos";
  case fp_tan_code: return "FP@tan";
  case fp_cot_code: return "FP@cot";
  case fp_tancot_code: return "FP@tancot";
  case fp_arcsin_code: return "FP@arcsin";
  case fp_arccos_code: return "FP@arccos";
  case fp_arcsincos_code: return "FP@arcsincos";
  case fp_arctan_code: return "FP@arctan";
  case fp_arccot_code: return "FP@arccot";
  case fp_arctancot_code: return "FP@arctancot";
  case fp_lsolve_code: return "FP@lsolve";
  case fp_qsolve_code: return "FP@qsolve";
  case fp_csolve_code: return "FP@csolve";
  case fp_qqsolve_code: return "FP@qqsolve";
  default: return 0;
  }
}    

void Parser::exec_fp_cmd(subtypes i)
{
  fp_name = cur_tok;
  Token aux = fps[i];
  TokenList res;
  switch(i) {
  case fp_print_code:
    fp_print();    
    return;
  case fp_set_code:
    fp_set();
    return;
  case fp_add_code:
  case fp_sub_code:
  case fp_mul_code:
  case fp_div_code:
  case fp_min_code:
  case fp_max_code:
  case fp_pow_code:
  case fp_root_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      TokenList L3 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      fp_send_one_arg(L2);
      res.splice(res.end(),L2);
      fp_send_one_arg(L3);
      res.splice(res.end(),L3);
    }
    break;
  case fp_abs_code:
  case fp_neg_code:
  case fp_sgn_code:
  case fp_round_code:
  case fp_clip_code:
  case fp_trunc_code:
  case fp_exp_code:
  case fp_ln_code:
  case fp_ident_code:
  case fp_sin_code:
  case fp_cos_code:
  case fp_tan_code:
  case fp_cot_code:
  case fp_arcsin_code:
  case fp_arccos_code:
  case fp_arctan_code:
  case fp_arccot_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      fp_send_one_arg(L2);
      res.splice(res.end(),L2);
    }
    break;
  case fp_upn_code:
    fp_e_upn();
    break;
  case fp_iflt_code:
  case fp_ifgt_code:
  case fp_ifeq_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      res.push_back(aux);
      fp_send_one_arg(L1);
      res.splice(res.end(),L1);
      fp_send_one_arg(L2);
      res.splice(res.end(),L2);
    }
    break;
  case fp_ifneg_code:
  case fp_ifpos_code:
  case fp_ifzero_code:
  case fp_ifint_code:
    {
      TokenList L1 = mac_arg();
      res.push_back(aux);
      fp_send_one_arg(L1);
      res.splice(res.end(),L1);
    }
    break;  
  case fp_sincos_code:
  case fp_tancot_code:
  case fp_arcsincos_code:
  case fp_arctancot_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      TokenList L3 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      res.splice(res.end(),L2);
      fp_send_one_arg(L3);
      res.splice(res.end(),L3);
    }
    break;
  case fp_random_code:
    {
      fp_prepare();
      FpNum X;
      X.random();
      fp_finish(X);
      return;
    }
  case fp_seed_code:
    fp_setseed();
    return;
  case fp_eval_code:
    fp_e_eval();
    return;
  case fp_pascal_code:
    fp_e_pascal();
    return;
  case fp_lsolve_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      TokenList L3 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      fp_send_one_arg(L2);
      res.splice(res.end(),L2);
      fp_send_one_arg(L3);
      res.splice(res.end(),L3);
      break;
    }
  case fp_qsolve_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      TokenList L3 = mac_arg();
      TokenList L4 = mac_arg();
      TokenList L5 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      res.splice(res.end(),L2);
      fp_send_one_arg(L3);
      res.splice(res.end(),L3);
      fp_send_one_arg(L4);
      res.splice(res.end(),L4);
      fp_send_one_arg(L5);
      res.splice(res.end(),L5);
      break;
    }
  case fp_csolve_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      TokenList L3 = mac_arg();
      TokenList L4 = mac_arg();
      TokenList L5 = mac_arg();
      TokenList L6 = mac_arg();
      TokenList L7 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      res.splice(res.end(),L2);
      res.splice(res.end(),L3);
      fp_send_one_arg(L4);
      res.splice(res.end(),L4);
      fp_send_one_arg(L5);
      res.splice(res.end(),L5);
      fp_send_one_arg(L6);
      res.splice(res.end(),L6);
      fp_send_one_arg(L7);
      res.splice(res.end(),L7);
      break;
    }
  case fp_qqsolve_code:
    {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      TokenList L3 = mac_arg();
      TokenList L4 = mac_arg();
      TokenList L5 = mac_arg();
      TokenList L6 = mac_arg();
      TokenList L7 = mac_arg();
      TokenList L8 = mac_arg();
      TokenList L9 = mac_arg();
      res.push_back(aux);
      res.splice(res.end(),L1);
      res.splice(res.end(),L2);
      res.splice(res.end(),L3);
      res.splice(res.end(),L4);
      fp_send_one_arg(L5);
      res.splice(res.end(),L5);
      fp_send_one_arg(L6);
      res.splice(res.end(),L6);
      fp_send_one_arg(L7);
      res.splice(res.end(),L7);
      fp_send_one_arg(L8);
      res.splice(res.end(),L8);
      fp_send_one_arg(L9);
      res.splice(res.end(),L9);
      break;
    }
  default: return;
  }
  back_input(res);
}

void Parser::exec_fpi_cmd(subtypes i)
{
  fp_name = cur_tok;
  FpNum X;
  switch(i) {
  case fp_ident_code: // \fpident\cmd{123.45} this puts 123.45 into \cmd
    fp_prepare();
    X = fp_read_value();
    break;
  case fp_add_code:
    { 
      fp_prepare();
      X = fp_read_value();
      FpNum Y = fp_read_value();
      X.add(Y);
      break;
    }
  case fp_sub_code:
    {
      fp_prepare();
      X = fp_read_value();
      FpNum Y = fp_read_value();
      Y.neg();
      X.add(Y);
      break;
    }
  case fp_mul_code:
    {  
      fp_prepare();
      FpNum a = fp_read_value();
      FpNum b = fp_read_value();
      X.mul(a,b);
      break;
    }
  case fp_min_code:
  case fp_max_code:
    {
      fp_prepare();
      FpNum X = fp_read_value();
      FpNum Y = fp_read_value();
      int res = fp::compare(X,Y);
      if(i==fp_min_code) res = -res;
      fp_finish(res>0?X:Y);
      return;
    }
  case fp_div_code:
    {
      fp_prepare();
      FpNum a = fp_read_value();
      FpNum b = fp_read_value();
      X.div(a,b);
      break;
    }
  case fp_abs_code:
    fp_prepare();
    X = fp_read_value();
    X.sign = true;
    break;
  case fp_neg_code:
    fp_prepare();
    X = fp_read_value();
    X.neg();
    break;
  case fp_clip_code:// clip is no-op
  case fp_round_code:
  case fp_trunc_code:
    fp_prepare();
    X = fp_read_value();
    if(i!=fp_clip_code) {
      int j = scan_braced_int(fp_name);
      if(i==fp_round_code) { X.round(j); X.round0(); }
      else X.truncate(j);
    }
    break;
  case fp_sgn_code: 
    fp_prepare();
    X = fp_read_value();
    X.sgn();
    break;
  case fp_exp_code:
    fp_prepare();
    X = fp_read_value();
    X.exec_exp();
    break;
  case fp_ln_code:
    fp_prepare();
    X = fp_read_value();
    X.exec_ln();
    break;
  case fp_pow_code:
  case fp_root_code: 
    {
      fp_prepare();
      FpNum a = fp_read_value();
      FpNum b = fp_read_value();
      X.pow(a,b,i);
      break;
    }
  case fp_iflt_code:
  case fp_ifgt_code:
  case fp_ifeq_code:
    fp_eval_lt(i);
    return;
  case fp_ifneg_code:
  case fp_ifpos_code:
  case fp_ifzero_code:
  case fp_ifint_code:
    fp_eval_unarytest(i);
    return;
  case fp_sin_code:
  case fp_cos_code:
  case fp_tan_code:
  case fp_cot_code:
  case fp_sincos_code:
  case fp_tancot_code:
    {
      fp_prepare();
      if(i==fp_sincos_code ||i==fp_tancot_code) {
	get_r_token();
	fp_res2 = cur_tok;
      }
      FpNum X = fp_read_value();
      FpNum a,b;
      fp::sincos(a,b,X,i);
      fp_finish(a);
      if(i==fp_sincos_code ||i==fp_tancot_code) {
	fp_res = fp_res2;
	fp_finish(b);
      }
    }
    return;
  case fp_arcsin_code:
  case fp_arccos_code:
  case fp_arctan_code:
  case fp_arccot_code:
  case fp_arcsincos_code:
  case fp_arctancot_code:
    fp_e_arcsin(i);
    return;
  case fp_lsolve_code:
    fp_e_lsolve();
    return;
  case fp_csolve_code:
    fp_e_csolve();
    return;
  case fp_qsolve_code:
    fp_e_qsolve();
    return;
  case fp_qqsolve_code:
    fp_e_qqsolve();
    return;
  case fp_upn_code:
    fp_e_upn();
    return;
  default: return;
  }
  fp_finish(X);
}
