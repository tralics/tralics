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
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <cstdio>
#include <cstring>

using namespace std;

auto is_letter(char c) -> bool;

#include "tralics/Token.h"
#include "tralics/Utf8Char.h"

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
