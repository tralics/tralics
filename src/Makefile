OBJECTS=  txtoken1.o txtoken2.o tralics.o txparser.o  txparser2.o txparser3.o\
  txmath.o txmath1.o txmathboot.o  txbuffer.o txtoken.o txmlio.o\
  txstack.o txtranslate.o  txmain.o txfp.o txfonts.o txscan.o txio.o\
  txtrees.o  txtitlepage.o txcommands.o txarray.o  txatt.o txaccent.o\
  txpost.o txerr.o txaux.o txbib.o readline.o txclasses.o\
  txra.o txconfig.o

INCLUDES=tralics.h txinline.h  txparser.h  txvars.h txstring.h \
   txstack.h txfonts.h  txlogger.h  txtoken.h  txbuffer.h txparam.h \
   tximage.h txscaled.h txid.h txeqtb.h txfp.h txio.h txstats.h  \
   txcond.h txatt.h txhash.h txxml.h txtokenlist.h txconst.h \
   txchars.h txcmd.h 

RAOPT=
TRALICSDIR=/usr/lib/tralics
CONFDIR=../confdir
#RAOPT=-DTRALICSDIR=\"$(TRALICSDIR)\" -DCONFDIR=\"$(CONFDIR)\"


CC=g++
CCC=g++
CXX=g++

CPPFLAGS=-O2 -Wall
#CPPFLAGS= -g -Wall 


tralics: $(OBJECTS)
	$(CXX) -o tralics $(OBJECTS)

tralics_static: $(OBJECTS)
	$(CXX) -o tralics_static -static $(OBJECTS)

lxp: $(OBJECTS)
	purify  -chain_length=10 -cache-dir=/u/medee/0/miaou/grimm/raweb/lx/TMP g++ -o lxp $(OBJECTS)

readline.o: readline.C
tralics.o : tralics.C $(INCLUDES)
txarray.o: txarray.C $(INCLUDES) txarray.h
txatt.o: txatt.C $(INCLUDES)
txaux.o: txaux.C $(INCLUDES)
txbib.o: txbib.C txbib.h $(INCLUDES)
txbuffer.o : txbuffer.C $(INCLUDES)
txcheck.o: txcheck.C $(INCLUDES) txcheck.h
txcommands.o: txcommands.C $(INCLUDES)
txerr.o: txerr.C $(INCLUDES)
txfonts.o: txfonts.C $(INCLUDES)
txfp.o: txfp.C $(INCLUDES)
txio.o: txio.C $(INCLUDES)
txmath.o: txmath.C  $(INCLUDES) txmath.h
txmath1.o: txmath1.C  $(INCLUDES) txmath.h txmath1.h
txmathboot.o: txmathboot.C $(INCLUDES) txmath.h
txparser.o: txparser.C $(INCLUDES)
txparser3.o: txparser3.C $(INCLUDES)
txparser2.o: txparser2.C $(INCLUDES) txclasses.h txparser2.h
txpost.o: txpost.C $(INCLUDES) txtrees.h txpost.h
txscan.o: txscan.C $(INCLUDES)
txstack.o: txstack.C $(INCLUDES)
txaccent.o: txaccent.C $(INCLUDES)
txtitlepage.o: txtitlepage.C $(INCLUDES) txtitlepage.h
txtoken.o : txtoken.C $(INCLUDES)
txtoken1.o : txtoken1.C $(INCLUDES)
txtoken2.o : txtoken2.C $(INCLUDES)
txconfig.o : txconfig.C $(INCLUDES)
txra.o : txra.C $(INCLUDES)
txtranslate.o: txtranslate.C $(INCLUDES)
txtrees.o: txtrees.C $(INCLUDES) txtrees.h
txclasses.o: txclasses.C $(INCLUDES) txclasses.h
txmlio.o: txmlio.C $(INCLUDES) txmlio.h
txmain.o: txmain.C $(INCLUDES)  txpath.h
	 $(CC) $(CPPFLAGS) -c $*.C $(RAOPT)
