// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2006-2011.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

#include<iostream>
#include<cstdlib>
#include "tralics.h"

#include<cctype>
#include<sys/types.h>
#include<signal.h>


const char* read_line_rcsid =
 "$Id: readline.C,v 2.14 2011/06/28 08:15:25 grimm Exp $";

// The line editor
class Slined;

static Slined* the_editor=0;
static Slined* the_editor_c=0;
static const int buf_size = 2048;
static const int in_size = 4096;
static bool term_set = false;
static int word_beg[in_size];
static int word_end[in_size];
string cur_prompt="> ";

// Conditional code for windows

#if defined(_WIN32)
// no termio
inline void RESET_TERMIO() {}
inline void SET_TERMIO() {}
inline void INIT_TERMIO() {}
inline unsigned char check_for_control(unsigned char c) {return c;}
inline bool is_interrupt(char cc)
{ return cc== 3 || cc=='y' || cc=='Y'; }

#else

#include<termios.h>
static struct termios orig_termio,rl_termio;
inline void RESET_TERMIO()  { tcsetattr(0, TCSANOW, &orig_termio); }
inline void SET_TERMIO()  { tcsetattr(0,TCSANOW,&rl_termio); }

unsigned char check_for_control(unsigned char c) 
{
  if(c==orig_termio.c_cc[VERASE])
    c = 8; //^H
  else if(c==orig_termio.c_cc[VEOF])
    c = 4; // ^D
  else if(c==orig_termio.c_cc[VKILL])
    c = 11; // ^K
  else if(c==orig_termio.c_cc[VWERASE])
    c = 23; //^W
  else if(c==orig_termio.c_cc[VSUSP])
    c = 26; // ^Z
  else if(c==orig_termio.c_cc[VINTR])
    c = 3; // ^C
  else if(c==orig_termio.c_cc[VQUIT])
    c = 28;
  return c;
}
inline bool is_interrupt(char cc)
{
  return cc==orig_termio.c_cc[VINTR] || cc==3 || cc==orig_termio.c_cc[VQUIT]
    || cc=='y' || cc=='Y';
}
inline void INIT_TERMIO() 
{
  tcgetattr(0,&orig_termio);
  rl_termio= orig_termio;
  rl_termio.c_iflag&= ~(BRKINT|PARMRK|INPCK|IXOFF|IMAXBEL);
  rl_termio.c_iflag|= (IGNBRK|IGNPAR);
  rl_termio.c_lflag&= ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|NOFLSH);
  rl_termio.c_lflag|= (ISIG);
  rl_termio.c_cc[VMIN]= 1;
  rl_termio.c_cc[VTIME]= 0;
  rl_termio.c_cc[VSUSP]= 0;
  rl_termio.c_cc[VINTR]= 0;
  rl_termio.c_cc[VQUIT]= 0;
  rl_termio.c_cc[VSTOP]= 0;
  rl_termio.c_cc[VSTART]= 0;
#if defined(__ppc__)
  rl_termio.c_cc[VDSUSP]= 0; // needed on the mac
#endif 
}
#endif


void readline_newprompt(string s)
{
  cur_prompt = s;
}

namespace readline_ns {
  void make_edit();
  void set_termio();
  void reset_termio();
  void my_exit();
  void tybeep();
  int skip_over_letters(String buf, int j);
  void tycleol();
  void right_fill();
  void shift_string(char*S,int s,int p,int n);
  void rl_delete0(char*s,int pos,int size,int w);
  int find_word_beg(char*buf,int size);
}

using namespace readline_ns;


inline void color_red() {} // dummy function
inline void color_black() {}

class Slined {

  char* m_inbuf;
  char* m_buffer;
  string m_killbuf;
  int m_inpos;
  int m_inmax;
  string m_prompt;
  std::vector<string> m_history;
  int m_hpos;
  int m_history_size;
  int m_mark;
  string m_search;
  int m_left;
  int m_right;
  int m_pos;
  int m_max;
  int m_size;
  bool m_hack;
  bool cur_line_modified;
  char g_buffer[buf_size];
  bool done;
 public:
  int newpos(int x,int n);
  Slined(int, String);
  int copystring(String string,int s,int inpos,bool);
  void redisplay();
  void redisplay0();
  void redisplay1(int x);
  void redisplay_eol();
  void finish_redisplay(int pleft,int pright);
  void I_move(int pos,int x);
  void insert_in_image(int n,char c1,char c2);
  void delete_in_image(int w);
  void insert(int n,char c);
  void insert1(int n,unsigned char c);
  void rl_move(int y,int x);
  void rl_delete(bool sw);
  void toggle_char();
  void delete_string(int sw,int deb,int fn);
  void fast_ins(int n,String s,int l);
  void insert_substring(int n,String s,int l);
  void Hshow();
  bool Hfind(int n);
  void Hfirst();
  void Hlast();
  void Hprevious(int n);
  void Hnext(int n);
  void search_string(int);
  void forword(bool sw,int n);
  void backword(bool sw,int n);
  void replace_string();
  void store_line();
  void maybe_store_line();
  void do_esc_command(int n);
  void do_esc_command(int n,char c);
  void do_n_command(int n,unsigned char c);
  void do_command(int n,int c);
  void initialise(char*buf,const string& prompt,int size);
  void run();
};


// constructor. 
Slined::Slined(int sz, String P) : m_prompt("> "),m_hpos(0)
{
  m_inpos = 0;
  m_inmax = 0;
  m_left = m_right = m_size = m_max = 0;
  m_search = "";
  m_killbuf = "";
  m_mark = 0;
  m_buffer = new char [buf_size];
  for(int i= 0;i<buf_size;i++)
    m_buffer[i] = ' ';
  if(sz) m_inbuf = new char [sz];
  if(P) {
    m_history.push_back(P);
    m_history_size=1; // first line is a comment
  }
}

// Constructs two copies, one for showing the history.
// This function is called only once.
void readline_ns::make_edit()
{
  the_editor_c = new Slined(in_size,0);
  the_editor = new Slined(0,"%% start of history (say \\stop to exit)");
}

// Copy len characters from source to target. Strings may overlap.
void readline_ns::shift_string(char*S, int len, int source, int target)
{
  if(len==1)
    S[target] = S[source];
  else if(len==2) {
    char x = S[source+1];
    S[target] = S[source];
    S[target+1] = x;
  } else if(target<=source) {
    for(int i= 0; i<len; i++,S++) S[target] = S[source];
  } else {
    S += len-1;
    for(int i= 0;i<len; i++,S--) S[target] = S[source];
  }
}

// This assumes that characters with code >=128 are printable.
inline bool not_printable (unsigned char c)
{
  return c== 127 || c<32; 
}

// Returns the cursor position y of position x. Returns -1-y if the cursor
// is invisible. It is assumed that position n corresponds to m_pos.
int Slined::newpos(int x, int n)
{
  int i = 0, y = 0;
  if(x>=n){ // We can hack a bit.
    i = n;
    y = m_pos;
  }
  while(i<x){
    if(not_printable(m_inbuf[i])) y += 2; else y++;
    i++;
  }
  if(y>=m_left && y<m_right)
    return y;
  else
    return -1-y;
}

// Beeps
void readline_ns::tybeep ()
{
  std::cerr.put(7);
}

// Copy a string of size s into m_buffer or g_buffer, depending on sw.
// Control-A is copied as a two character sequence ^A. Returns
// the number of chars put in the buffer. If character at position
// inpos is copied, its location will be in m_pos.

int Slined::copystring(String string, int s, int inpos, bool sw)
{
  int j = 0;
  unsigned char cn;
  char*buf= sw ? m_buffer : g_buffer;
  for(int i=0; i<s; i++){
    if(i==inpos) m_pos = j;
    cn = string[i];
    if(not_printable(cn)) {
      buf[j] = '^';
      j++;
      cn = cn|64;
    }
    buf[j] = cn;
    j++;
  }
  return j;
}

// Full redisplay.
void Slined::redisplay()
{
  int j = copystring(m_inbuf,m_inmax,m_inpos,true);
  if(m_inpos>=m_inmax) m_pos = j; // set cursor to EOL
  m_max = j;
  color_black();
  std::cerr << "\r" << m_prompt;
  color_red();
  tycleol();
  redisplay0();
}

// Redisplay after prompt. 
void Slined::redisplay0()
{
  int left,right; // position of left and right chars
  int s = m_size, p = m_pos, m = m_max;
  if(m<=s) { // we have a short line, ok
    left = 0;
    right = m;
  } else if(p<s-2) {  // cursor visible if we show first char
    left = 0;
    right = s-2;
  } else if(m-p<s-2) { // cursor visible if we show last char
    right = m;
    left = m-(s-2);
  } else {   // put the cursor in the middle
    right = p+(s-4)/2;
    left = right-(s-4);
  }
  m_left = left;
  if(left!=0)
    std::cerr << "> "; // show that there are unseen characters there
  finish_redisplay(left,right);
  return;
}

// clear to end of line. 
void readline_ns::tycleol()
{
  std::cerr.put(27);
  std::cerr.put('[');
  std::cerr.put('K');
}

// redisplay, set current pos to x.
void Slined::redisplay1(int x)
{
  I_move(m_pos,m_left);
  m_pos = x;
  if(m_left!=0){
    std::cerr.put(8); // backspace twice, because of the marker
    std::cerr.put(8);
  }
  tycleol();
  redisplay0();
}

// redisplay what's at right of cursor.
void Slined::redisplay_eol()
{
  tycleol();
  int f = m_left+m_size;
  if(m_left!=0) f -= 2;
  if(f>=m_max) f = m_max;
  else f -= 2;
  finish_redisplay(m_pos,f);
}

// Send out right marker plus two backspaces.
void readline_ns::right_fill()
{
  std::cerr.put(' ');
  std::cerr.put('<');
  std::cerr.put(8);
  std::cerr.put(8);
}

// move the cursor to the right end, then back to target.
void Slined::finish_redisplay(int left, int right)
{
  m_right = right;
  I_move(left,right);
  if(m_max!=right) right_fill();
  I_move(right,m_pos);
}

// insert char c1 (or c1+c2) n times at current position
void Slined::insert_in_image(int n, char c1, char c2)
{
  int m = m_max, p = m_pos;
  int size = m-p; // number of chars after cur pos
  int k = n;
  if(c1!=0) k += n; // k is the number of chars to insert
  char*buf = m_buffer;
  buf = buf+p;
  if(size!=0)
    shift_string(buf,size,0,k); // make room
  for(int i= 0;i<n;i++) {        // copy
    if(c1!=0) *(buf++) = c1;
    *(buf++) = c2;
  }
  m += k;
  m_max = m; // new size
  p += k;
  m_pos = p;  // new position, cursor is after inserted chars
  int d = m_right+k;
  m_right = d; // new position of last char
  int s = m_size;
  if(m_left!=0) s -= 2;
  if(m!=d) s -= 2; // s is number of free positions on line
  if(d-m_left>s)  // cursor invisible, full redisplay needed
    redisplay();
  else {
    for(int i=0; i<n; i++) {
      if(c1) std::cerr.put(c1);
      std::cerr.put(c2);
    }
    if(p==m) {}   // we are at the right end
    else if(p==d)   // we are at the boundary
      right_fill();
    else
      redisplay_eol();  // insert, and redisplay after position
  }
}

// delete a substring of size w
// in the string, at position pos, size is the string length
void readline_ns::rl_delete0(char*s, int pos, int size, int w)
{
  int t = size-pos-w;
  if(t>0) shift_string(s,t,w+pos,pos);
}

// delete and redisplay
void Slined::delete_in_image(int w)
{
  int p = m_pos;
  rl_delete0(m_buffer,p,m_max,w);
  m_right -= w;
  m_max -= w;
  if(p==m_right) { // deleted all chars after cursor
    tycleol();
    if(m_right!=m_max) right_fill();
  }
  else redisplay_eol();  // non trivial case
}

// Insert a character n times
void Slined::insert(int n,char c)
{
  if(n+m_inmax>=buf_size-1) // overfull
    tybeep();
  else insert1(n,c);
}

// The real function that inserts a character c n times
void Slined::insert1(int n, unsigned char c)
{
  int pos = m_inpos;
  int size = m_inmax;
  int aux = size-pos;
  char*buffer = m_inbuf;
  buffer = buffer+pos;
  if(aux!=0)  // make room
    shift_string(buffer,size,0,n);
  for(int i= 0;i<n;i++) buffer[i] = c; // insert
  size += n;
  pos += n;
  m_inpos = pos;
  m_inmax = size;
  unsigned char c1 = 0, c2 = c;
  if(not_printable(c)) { c1 = '^'; c2 |= 64; }
  insert_in_image(n,c1,c2);
  cur_line_modified = true;
}

// delete a character. Redisplay only if sw true.
void Slined::rl_delete(bool sw)
{
  unsigned char cn = m_inbuf[m_inpos];
  rl_delete0(m_inbuf,m_inpos,m_inmax,1);
  m_inmax--;
  cur_line_modified = true;
  if(sw)
    delete_in_image(not_printable(cn) ? 2 : 1);
}

// Move the cursor from Y to X
void Slined::rl_move(int y, int x)
{
  if(x==y) return;
  m_inpos = x;
  if(x==0) {            // move to left
    if(m_left==0){      // first char is visible
      I_move(m_pos,0);
      m_pos = 0;
    }
    else redisplay1(0);  // else general case
  } else if(x==m_inmax){ // move to last char
    if(m_right==m_max){  // last char is visible
      I_move(m_pos,m_right);
      m_pos = m_right;
    }
    else redisplay1(m_max); // else general case
  } else {
    y = newpos(x,y);
    if(y >=0){              // target is visible
      I_move(m_pos,y);
      m_pos = y;
    }
    else redisplay1(-1-y);   // target not visible
  }
}

// Move from a visible point to another visible point
void Slined::I_move(int pos, int x)
{
  if(x==pos) return;
  else if(x>pos)
    while(x>pos) {
      std::cerr.put(m_buffer[pos]); // advance -> output current char
      pos++;
    }
  else while(x<pos) {
    pos--;
    std::cerr.put(8);   // otherwise, emit some backspaces
  }
}

// Toggle two chars: the character on the left and the character under
// the cursor and advance by one char.
void Slined::toggle_char()
{
  int max = m_inmax;
  int pos = m_inpos;
  if(max<2 || pos<1) return;
  if(pos==max){ // go back by 1 character
    rl_move(pos,pos-1);
    pos--;
  }
  unsigned char c1 = m_inbuf[pos-1], c2 = m_inbuf[pos];
  if(c1==c2)   // we can  hack
    rl_move(pos,pos+1);
  else{
    rl_delete(1); // must delete 2 chars and insert 2 chars
    rl_move(pos,pos-1);
    rl_delete(1);
    insert(1,c2);
    insert(1,c1);
  }
}

// this deletes a string between deb and fn. 
void Slined::delete_string(int sw,int deb,int fn)
{
  int debut,fin;
  if(deb<fn) { debut = deb; fin = fn; }
  else { debut = fn; fin = deb; }
  if(debut<0) debut = 0;
  if(fin>=m_inmax) fin = m_inmax-1;

  int taille = fin-debut+1; // Size of what must be killed.
  m_killbuf = string(m_inbuf+debut,taille);
  if(sw) {
    rl_move(m_inpos,debut);
    for(int i= 0;i<taille;i++)
      if(m_inpos<m_inmax) rl_delete(0);
    redisplay();
  }
}

// inserts  a  string n times
void Slined::insert_substring(int n,String s,int l)
{
  if(m_inmax+n*l>=buf_size-1)
    tybeep();
  else if(n*l>10)
    fast_ins(n,s,l);
  else{
    for(int j= 0;j<n;j++)
      for(int i= 0;i<l;i++) insert(1,s[i]);
  }
}

// insert in the buffer then redisplay all
void Slined::fast_ins(int n, String s, int l)
{
  int pos = m_inpos;
  int size = m_inmax;
  char*buffer = m_inbuf;
  int aux = size-pos;
  if(aux!=0) shift_string(buffer,aux,pos,pos+n*l); //make room
  for(int i= 0;i<n;i++){
    strncpy(buffer+pos,s,l);
    pos += l;
    size += l;
  }
  m_inpos = pos;
  m_inmax = size;
  int j = copystring(s,l,-1,false);
  pos = m_pos;
  buffer = m_buffer;
  size = m_max;
  aux = size-pos;
  if(aux!=0) shift_string(buffer,aux,pos,pos+j*n); // make room here also
  for(int i= 0;i<n;i++){
    strncpy(buffer+pos,g_buffer,j);
    pos += j;
    size += j;
  }
  m_max = size;
  redisplay_eol();
  rl_move(m_pos,pos); //put cursor where it should be
}

// show history
void Slined::Hshow()
{
  int n = 0;
  int sz = m_history.size();
  Slined*new_ed = the_editor_c;
  char p[4];
  p[2] = ' ';
  p[3] = 0;
  new_ed->m_size = m_size+m_prompt.size()-3;
  for(int i=0;i<sz;i++){
    n++;
    if(n>=100) n = n-100;
    if(n<10) p[0] = ' ';
    else p[0] = '0'+(n/10);
    p[1] = '0'+(n%10);
    new_ed->m_prompt = p;
    new_ed->m_inpos = 0;
    String S = m_history[i].c_str();
    int k = strlen(S);
    if(k>buf_size-2) k = buf_size-2;
    new_ed->m_inmax = k;
    strncpy(new_ed->m_inbuf,S,k);
    new_ed->m_inbuf[k] = 0;
    new_ed->redisplay();
    std::cerr.put('\n');
  }
}

// search a string, going back n steps. 
void Slined::search_string(int n)
{
  if(m_search.empty()) {// searches current line in history
    m_inbuf[m_inmax] = 0;
    m_search = m_inbuf;
  }
  if(!Hfind(n))
    return;
  replace_string();
}

// Find a string in the history
bool Slined::Hfind(int n)
{
  String s = m_search.c_str();
  bool seen_once = false; // if seen at least once, do not kill
  int pos = m_hpos;
  while(pos>0) {
    pos--;
    if(strstr(m_history[pos].c_str(),s)) {
      seen_once = true;
      n--;
      if(n==0) { 
	m_hpos = pos;
	return true;
      }
    }
  }
  tybeep();
  if(!seen_once) m_search="";
  return false;
}

// set position to first line.
void Slined::Hfirst()
{
  if(m_history_size==1) m_hpos = 0; // first is a dummy, skip it.
  else m_hpos = 1;
}

// set position to last line
void Slined::Hlast()
{
  m_hpos = m_history_size -1;
}

// go back n lines
void Slined::Hprevious(int n)
{
  if(m_hpos == m_history_size-1 && !m_hack) {
    n--;
    m_hack = true;
  }
  int k = m_hpos -n;
  if(k<0)  k = 0;
  m_hpos = k;
}

// advance by n lines
void Slined::Hnext(int n)
{
  int k = m_history_size -1;
  n += m_hpos;
  if(n>k)  n = k;
  m_hpos = n;
}

int readline_ns::skip_over_letters(String buf, int j)
{
  for(;;) {
    if(!is_letter(buf[j])) return j;
    j++;
  }
}

// Find word boundaries
int readline_ns::find_word_beg(char*buf, int size)
{
  int i = -1;
  unsigned char c;
  int j=0;
  for(;;) {
    i++;
    buf[size] = '*';
    while(buf[j]==' ') j++;
    if(j>=size)
      return i;
    word_beg[i] = j;
    buf[size] = ' ';
    c = buf[j];
    j++;
    if(c=='\\') {
      c = buf[j]; j++;
      if(is_letter(c)) j = skip_over_letters(buf,j);
    } else if(is_letter(c)) j = skip_over_letters(buf,j);
    word_end[i] = j;
  }
}

// advance by n words. If sw false deletes the string
void Slined::forword(bool sw, int n)
{
  int i = find_word_beg(m_inbuf,m_inmax);
  int y = m_inpos;
  int k = 0;
  while(k<i && y>=word_end[k]) k++;  // k = index of cur word
  if(k>=i) y = m_inmax; else y = word_beg[k];  // y is here boundary
  k = k+n-1;
  if(k>=i) k = m_inmax; else k = word_end[k]; // k is there boundary
  if(sw)
    rl_move(m_inpos,k);
  else if(k>y)  // do nothing if k=0
    delete_string(1,y,k-1);
}

// go backwards by n words. If sw false, delete the string.
// same code, with begin and end exchanged.
void Slined::backword(bool sw,int n)
{
  int i = find_word_beg(m_inbuf,m_inmax);
  int y = m_inpos;
  int k = i-1;
  while(k>=0 && y<=word_beg[k]) k--;
  if(k<0) y = 0; else y = word_end[k];
  k = k-n+1;
  if(k<0) k = 0; else k = word_beg[k];
  if(sw) rl_move(m_inpos,k);
  else if(k<y) delete_string(1,k,y-1);
}

// Make s be the current string.
void Slined::replace_string()
{
  maybe_store_line();
  cur_line_modified = false;  
  string s = m_hpos<m_history_size ? m_history[m_hpos].c_str() :  "";
  int l = s.size();
  strncpy(m_inbuf,s.c_str(),l);
  m_inmax = l;
  m_inpos = l;
  redisplay();
}

// If the line is not empty, it is stored in the history. 
void Slined::store_line()
{
  if(m_inmax>0){
    m_history.push_back(string(m_inbuf,m_inmax));
    m_history_size ++;
  }
}

void Slined::maybe_store_line()
{
  if(!cur_line_modified) return;
  store_line();
}

void Slined::do_esc_command(int n)
{
  char c;
  std::cin.get(c);
  if('1'<=c && c<='9'){ // non zero digit; ignore n
    char cc;
    std::cin.get(cc);
    do_command(c-'0',cc);
    return;
  }
  if('A'<=c && c<='Z') // ignore case
    c = c-'A'+'a';
  do_esc_command(n,c);
}


// Action associated to escape something
void Slined::do_esc_command(int n, char c)
{
  switch(c){
  case '0':   // <esc>022 = repeat 22 times
    {
      int count= 0;
      for(;;){
	std::cin.get(c);
	if('0'<=c && c<='9') count = count*10+c-'0';
	else break;
      }
      count = count%1000;   // <ESC>45679 = repeat 679 
      do_command(count,c);
      return;
    }
  case 'b':
    backword(true,n);
    return;
  case 'f':
    forword(true,n);
    return;
  case 8:  // backspace
  case 127: // delete
    backword(false,n);
    return;
  case 'h':
    Hshow(); redisplay(); return;
  case 'd':
  case 4: // control-D
    forword(false,n);
    return;
  case 7:  // control G
    tybeep();
    return;
  case '<':
    Hfirst(); replace_string(); return;
  case '>':
    Hlast(); replace_string(); return;
  case 27:
    search_string(n);
    return;
  case 'o':
    do_n_command(1,15);
    return;
  case 's':
    m_search = ""; search_string(n); return;
  case 'w':
    delete_string(false,m_mark,m_inpos);
    return;
  case '@':
    m_mark = m_inpos;
    return;
  case '?': 
    color_black();
    std::cerr << "key bindings are more or less the same as in Emacs\n";
    std::cerr << "^R searches a string, <ESH>h shows history\n";
    std::cerr << "<ESC>n CMD : repeat the command CMD n times\n";
    std::cerr << "<ESC>0n CMD : repeat the command n times (case n>10)\n";
    redisplay();
    return ;
  case 28: // escape control backslash
    std::cerr << "\nQUIT\n";
    my_exit();
    return;
  case '[': // ESC[A is up-arrow
    {
      char cc;
      std::cin.get(cc);
      switch(cc){
      case 'A': do_n_command(n,16); return;
      case 'B': do_n_command(n,14); return;
      case 'C': do_n_command(n,6); return;
      case 'D': do_n_command(n,2); return;
      case 'F': do_n_command(n,'>'); return;
      case 'H': do_n_command(n,'<'); return;
      default: tybeep();
      }
      return;
    }
  default:
    tybeep();
  }
}


void Slined::do_n_command(int n, unsigned char c)
{
  int pos = m_inpos;
  int max = m_inmax;
  int aux = 0;
  c = check_for_control(c);
  switch(c) {
  case 1: // ^A
    rl_move(pos,0);
    return;
  case 2: // ^B
    aux = pos-n;
    if(aux<0) aux = 0;
    rl_move(pos,aux);
    return;
  case 5: // ^E
    rl_move(pos,max);
    return;
  case 6: // ^F
    aux = pos+n;
    if(aux>max) aux = max;
    rl_move(pos,aux);
    return;
  case 10:
  case 13:
    store_line();
    Hlast();
    done = true;
    return;
  case 15:
    store_line();
    Hnext(1);
    done = true;
    return;
  case 127:
  case 8:
    if(n>pos) n = pos;
    for(aux= 0;aux<n;aux++){
      rl_move(pos,pos-1);
      pos--;
      rl_delete(1);
    }
    return;
  case 4:
    for(aux= 0;aux<n;aux++){
      if(pos<m_inmax)
	rl_delete(1);
    }
    return;
  case 11:
    delete_string(1,pos,max);
    return;
  case 21:
  case 24:
    delete_string(1,0,pos-1);
    return;
  case 25:
    if(m_killbuf.size())
      insert_substring(n,m_killbuf.c_str(),m_killbuf.size());
    else
      tybeep();
    return;
  case 16:
    Hprevious(n);
    replace_string();
    return;
  case 14:
    Hnext(n);
    replace_string();
    return;
  case 18:
    search_string(n);
    return;
  case 20:
    toggle_char();
    return;
  case 12:
    redisplay();
    return;
  case 17:
    {
      char cc;
      std::cin.get(cc);
      insert(n,cc);
      return;
    }
  case 27:
    do_esc_command(n);
    return;
  case 0:
    m_mark= pos;
    return;
  case 23:
    delete_string(1,m_mark,pos);
    return;
  case 26:
    reset_termio();
    kill(0,SIGTSTP);
    set_termio();
    redisplay();
    return;
  case 28:
    std::cerr << "\nQUIT\n";
    my_exit();
    return;
  case 3:
    {
      std::cerr << "\nreally quit ?\n";
      char cc;
      std::cin.get(cc);
      if(is_interrupt(cc)) {	
	  reset_termio();
	  exit(1);
	}
      redisplay();
    }
    return;
  default:
    insert(n,c);
  }
}

void Slined::do_command(int n, int c)
{
  if(c==EOF) {
    std::cerr << "EOF on tty\n";
    my_exit();
  }
  if(c>=256||c<=-256){
    std::cerr << "???" << c << "???\n";
    return;
  }
  do_n_command(n,c);
}


void Slined::initialise(char*buffer, const string& prompt,int size)
{
  m_inbuf = buffer; 
  m_prompt = prompt;
  m_size = size-prompt.size();
  if(m_hpos<m_history_size-1)
    replace_string();
  else {
    m_inmax = 0;
    m_inpos = 0;
    redisplay();
  }
  m_hack = false;
}


void Slined::run()
{
  cur_line_modified = false;
  done = false;
  char c;
  for(;;){
    std::cin.get(c);
    do_command(1,c);
    if(done) break;
  }
  std::cerr.put('\n');
  m_inbuf[m_inmax] = 0;
}


void readline(char*buffer, int screen_size)
{
  if(!the_editor) readline_ns::make_edit();
  the_editor->initialise(buffer, cur_prompt, screen_size);
  set_termio();
  the_editor->run();
  reset_termio();
}

void readline_ns::set_termio()
{
  static bool term_copied = false;
  if(!term_copied){
    INIT_TERMIO();
    term_copied = true;
  }
  if(!term_set){
    SET_TERMIO();
    term_set = true;
    color_red();
  }
}


void readline_ns::reset_termio()
{
  if(term_set) {
    RESET_TERMIO();
    term_set = false;
    color_black();
  }
}

// reset and quit (in case of error)
void readline_ns::my_exit()
{
  reset_termio();
  exit(1);
}
