// -*- C++ -*-
//$Id: txconst.h,v 2.9 2008/03/17 07:41:00 grimm Exp $
// Copyright (c)INRIA/apics (Jose' Grimm) 2006, 2007

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains some constants that define table sizes


// This is the number of characters; see txtoken.h
// This is also the size of tables that remember character properties
static const uint nb_characters = 65536;

// This is the number of other registers
static const uint nb_registers = 1024;


// This is the number of different TeX fonts
static const uint nb_tex_fonts = 256;

// This is the size of the table ok_for_xspace
// Only ascii characters are considered, so that 128 is OK
static const uint nb_xspace_values = 128;

// This is the size of the table for shortverb
// You cannot use euro sign as a shortverb delimiter
static const uint nb_shortverb_values = 256;

// This is the size of the accent table.
// If you want to put an accent on a character with code >128, 
// you must remap it
static const uint nb_accents = 128;

// Max char for which $x$ is trivial math
static const uint nb_simplemath = 128;

// Max char for newcolumntype
static const uint nb_newcolumn = 128;
static const uint max_newcolumn_loops = 10000; // mx nb of iterations

// This is the number of slots for Mathml characters 
static const uint nb_mathchars = 128;

// There are two hash tables. In the current version, they have the same size
// Size of eqtb should be the same (??)
static const int hash_prime = 40009;
static const int hash_size = 50000; 
// Size of eqtb. This +100 is strange. 
// The big part of the table 
static const int eqtb_size = hash_size+2*nb_characters+100; 

static const int bib_hash_mod = 6397;

static const int max_dimension=07777777777;
static const int max_integer =017777777777;
