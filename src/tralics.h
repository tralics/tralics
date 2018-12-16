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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <string.h>
#include <stdio.h>

// use this for debug
#define the_tty cout
typedef unsigned int uint;


using namespace std;
extern bool is_letter (char c);
class EndOfData{};


#include "txconst.h"
#include "txvars.h"
#include "txchars.h"
#include "txstats.h"
#include "txid.h"
#include "tximage.h"
#include "txcond.h"

#include "txtoken.h"
// these may depend on the token class

#include "txtokenlist.h"
#include "txscaled.h"
#include "txbuffer.h"
#include "txstring.h"
#include "txfonts.h"
#include "txlogger.h"
#include "txeqtb.h"
#include "txhash.h"
#include "txfp.h"
#include "txxml.h"
#include "txatt.h"
#include "txstack.h"
#include "txio.h"
#include "txarray.h"
#include "txparser.h"
#include "txparam.h"
#include "txinline.h"
