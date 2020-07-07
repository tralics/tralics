// tralics, a LaTeX to XML translator
// Copyright INRIA (Jose Grimm) 2002, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// InterDepositDigitalNumber = IDDN.FR.001.510030.000.S.P.2002.000.31235
// for version 1.6, dated 05/12/2002.
// InterDepositDigitalNumber = IDDN.FR.001.510030.001.S.P.2002.000.31235
// for version 2.9.4, dated 18/02/2007.

// The file contains the main data structures, and code to fill the tables.

#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <ctime>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

// This is the main function. It does the following: boot the parser and the
// math part, and execute the main prog.
auto main(int argc, char **argv) -> int {
    spdlog::info("This is tralics {}, a LaTeX to XML translator", tralics_version);
    MainClass M;
    the_main    = &M;
    M.the_stack = &the_parser.the_stack;
    spdlog::info("Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm");
    the_parser.boot(); // create the hash table and all that
    spdlog::info("Licensed under the CeCILL Free Software Licensing Agreement");
    M.run(argc, argv);
    return main_ns::nb_errs == 0 ? 0 : 2;
}
