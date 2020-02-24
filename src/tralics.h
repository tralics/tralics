#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002,2004,2006,2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This is the main include  file for the tralics software

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "tralics/types.h"

using namespace std;

// \todo All these belong in their respective header files

auto get_math_char(uchar c, int f) -> string;
auto is_letter(uchar c) -> bool;
auto txgethostname(char *name, size_t len) -> int;
void readline_newprompt(string s);
void readline(char *buffer, int screen_size);
void set_everyjob(const string &s);
void set_math_char(uchar c, int f, string s);
void show_unused_options();
void txsleep(int);

#include "tralics/Token.h"
#include "tralics/codepoint.h"

#include "txcond.h"
#include "txid.h"
#include "tximage.h"
#include "txstats.h"
#include "txvars.h"

#include "txarray.h"
#include "txeqtb.h"
#include "txfonts.h"
#include "txhash.h"
#include "txinline.h"
