#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003,  2007, 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Buffer.h"
#include "txid.h"
#include "txscaled.h"
#include <fstream>
#include <iostream>
#include <memory>

// This include file holds some declarations for printing objects
// and the classes that allow us to print either on the tty, the log file
// or both.

auto operator<<(std::ostream &fp, const Glue &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const Istring &L) -> std::ostream &;
auto operator<<(std::ostream &fp, const Macro &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
auto operator<<(std::ostream &fp, const SthInternal &x) -> std::ostream &;
auto operator<<(std::ostream &fp, Token x) -> std::ostream &;
auto operator<<(std::ostream &fp, Xid X) -> std::ostream &;
auto operator<<(std::ostream &fp, const codepoint &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const Xml *T) -> std::ostream &;
auto operator<<(std::ostream &fp, const ScaledInt &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream &;

inline auto operator<<(std::ostream &fp, const Buffer &L) -> std::ostream & { return fp << L.c_str(); }
