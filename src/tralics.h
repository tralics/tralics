#pragma once
// -*- C++ -*-
// $Id: tralics.h,v 2.46 2008/11/12 09:43:56 grimm Exp $
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

using uint = unsigned int;

using namespace std;
extern auto is_letter(char c) -> bool;
class EndOfData {};

#include "txchars.h"
#include "txcond.h"
#include "txconst.h"
#include "txid.h"
#include "tximage.h"
#include "txstats.h"
#include "txvars.h"

#include "txtoken.h"
// these may depend on the token class

#include "txarray.h"
#include "txatt.h"
#include "txbuffer.h"
#include "txeqtb.h"
#include "txfonts.h"
#include "txfp.h"
#include "txhash.h"
#include "txinline.h"
#include "txio.h"
#include "txlogger.h"
#include "txparam.h"
#include "txparser.h"
#include "txscaled.h"
#include "txstack.h"
#include "txstring.h"
#include "txtokenlist.h"
#include "txxml.h"
