This page describes the Perl Script used before 2008 to produce the
activity report of INRIA. It is Chapter 4 of `Tralics and the
Raweb <raweb.html>`__

.. _script0:

4. The auxiliary perl script
============================

The behavior of *Tralics* is not the same in Raweb mode or in normal
mode. Raweb mode if entered if the document type (deduced from the
options, the configuration file, and the document) is ``RA`` (or
``ra``). In the case, the file name should be apics2006.tex, and the
document class should be ``ra2006`` (the year can be replaced by 2004,
etc, provided that it is the same). If an XML file is created, it will
be apics.xml and the year will be an attribute of the document element.
If a TeX file is created, it will be named apics.tex, and it will use
the ``raweb`` documentclass.

-  If you say tralics apics2006 -check, the software will read your
   file, analyze it and, that's all (the transcript file is
   apics2006.log).
-  If you say tralics apics2006 -ps, the software will read your file,
   analyze it, create a new TeX file, and use an external program to
   compile it; communication is via a file named user_param.pl. The file
   apics.rh.log will hold the logs of all the external programs
   (apics2006.log is the transcript file of *Tralics*, apics.log the
   transcript file of LaTeX).
-  If you say tralics apics2006, the software will read your file,
   analyze it, and translate it. It will call the external program, but
   nothing will happen.
-  If you say tralics apics2006 -xmlall (or some other options) the
   software will read your file, analyze it, and translate it. It will
   call the external program, that will convert the XML file. As above,
   communication is via user_param.pl, transcript in apics.rh.log.

The auxiliary script starts like this

.. container:: ltx-source

   ::

      #!/usr/bin/perl
      # additional header info here
      print "(C) 2004 2006 INRIA, Jos\\'e Grimm, Team Apics\n";

All variables used by the program can be redefined by the \`require' at
the end of the file. If ``todo_ps`` is true (non-zero), then you want a
preview (conversion from latex to dvi and Postscript). Otherwise, if
``todo_xml`` is true, you want conversion from latex to XML; this is
done by *Tralics*. The post-processor allows you to check the XML (if
``todo_lint`` is true), convert it to HTML (if ``todo_html`` is true),
to XMLFO (if ``todo_fo`` is true), and to Pdf or Postscript (if
``todo_tex`` is true). The variables are initialized like this.

.. container:: ltx-source

   ::

      $::todo_fo = 1;
      $::todo_html = 1;
      $::todo_tex = 1;
      $::todo_lint = 1;
      $::todo_ps = 0;
      $::todo_xml = 1;

If you run *Tralics* on the file apics2006.tex, the preview will be in
the file apics.tex (from the variable ``tex_file``), while the XML file
will be apics.xml (from the variable ``no_year``). And the year is 2006.
Other variable contain a percent sign. Make sure that Perl does not
interpret them as a hash reference, since we will modify them later.
(`explanations here <#percent>`__).

.. container:: ltx-source

   ::

      # Predeclare all variables here.
      $::tralics_dir= "/user/grimm/home/cvs/tralics";
      $::ra_year = "2006";
      $::tex_file = "miaou";
      $::no_year = "miaou";
      $::generate_dvi = q/latex -interaction=nonstopmode %t/;
      $::generate_ps = q/dvips %t.dvi -o %t.ps/;
      $::makefo = 'xsltproc --catalogs -o %A.fo %B %C';
      $::makehtml ='xsltproc --catalogs  %B %C';
      $::makepdf = 'pdflatex -interaction=nonstopmode %w';
      $::makedvi = 'latex -interaction=nonstopmode %w';
      $::dvitops = 'dvips %w.dvi -o %w.ps';
      $::checkxml = 'xmllint --catalogs --valid  --noout %C';

      $::dtd_file = "raweb3.dtd"; # New in 2004.

The variables shown above are overwritten by the content of the
user_param.pl, an example is `given here <raweb4.html#user-param>`__. In
particuler, the user-param file must give the name of the TeX file, and
``tralics_dir``, the location where additional files are located.

4.1 Postscript mode
-------------------

The objective is easy to understand: we have to call latex three times,
then dvips; after the first call, we execute bibtex. Three auxiliary
file are needed for bibtex: one is created by the footbib package, and
one by the script. The last one is standard.

.. container:: ltx-source

   ::

      sub copy_and_exec {
        print "copy-and-exec for $::tex_file\n";
        link_some_files();
        open OUTFILE, ">$::no_year.refer.aux";
        print OUTFILE "\\bibstyle{rawebnum}\n";
        print OUTFILE "\\citation{*}\n";
        print OUTFILE "\\bibdata{$::no_year" . "_refer" . $::ra_year . "}\n";
        close OUTFILE;
        open LOGFILE, ">$::no_year.rh.log";
        call_latex();
        call_bibtex ("","");
        call_bibtex (".refer","_refer");
        call_bibtex (".fb","_foot");
        call_latex();
        call_latex();
        interpret_and_launch($::generate_ps,"junk",1,"dvips","");
        close LOGFILE;
        print "(Transcript written on $::no_year.rh.log)\n";
        print "done\n";
      }

This is an auxiliary routine that takes 5 arguments. The first one is a
string to be evaluated (as explained below), it comes from the
configuration file; the second argument is either \`junk', or the name
of a xslt file, it can be used by the evaluator of the argument. Then
comes a boolean, if false nothing is done. Argument number four is a
symbolic name of the command, and if the last argument is true, the
command is executed twice.

If you do not want to generate PS, just put the empty string in
``$::generate_ps``, because here, nothing is done if ``$template`` is
empty.

.. container:: ltx-source

   ::

      sub interpret_and_launch {
        my ($template,$aux,$tr,$name,$twice) = @_;
        $_ = $template;
        return unless $_;
        interpret_str($aux);
        launch_cmd($_,$tr,$name);
        launch_cmd($_,$tr,$name) if $twice;
      }

This launches latex.

.. container:: ltx-source

   ::

      sub call_latex {
        interpret_and_launch($::generate_dvi,"junk",1,"latex",0);
        copy_to_log("$::no_year.log");
      }

This launches bibtex. We do not use the ``interpret_str`` subroutine (if
your executable is called BibTex, just edit the Perl script). There are
three bibtex files, apics2006.bib, apics_foot2006.bib and
apics_refer2006.bib. The procedure gets \`apics' from ``$::no_year``,
the \`2006' from ``$::ra_year``, and what is between as second argument.
The bibliography files are optional, so nothing is done in case the
files do not exist. On the other hand, the auxiliary files have the same
name as in section 3.1, without the year. For instance, it would be
apics.fb.aux. The quantity between \`apics' and \`aux' is the first
argument.

.. container:: ltx-source

   ::

      sub call_bibtex {
        my ($exta, $extb) = @_;
        my ($bib);
        $bib = "$::no_year$extb$::ra_year.bib";
        return unless -f $bib;
        $_ = $::no_year . $exta;
        print "bibtex -min-crossrefs=1000 $_\n";
        print LOGFILE "result of bibtex $_\n";
        `bibtex -min-crossrefs=1000 $_`;
        print "bibtex exit status $?\n";
        print LOGFILE "bibtex exit status $?\n";
        copy_to_log("$_.blg");
      }

This allows us to merge the transcript files of the different processes.

.. container:: ltx-source

   ::

      sub copy_to_log  {
        my($file) = @_;
        open(INPUT,"<$file");
        while (<INPUT>) {
          print LOGFILE;
        };
        close INPUT;
      }

This executes the first argument if the second is true. Everything
printed on the screen is also copied in the transcript file. Note: *this
means that all messages of LaTeX are stored in a buffer and printed at
end of job*. Said otherwise, in case of trouble, you will see the error
messages only after you abort the job.

.. container:: ltx-source

   ::

      sub launch_cmd {
        my ($cmd,$test,$name) = @_;
        if(! $test) {
          print LOGFILE "(not done) $cmd\n";
          print  "(not done) $cmd\n";
          return;
        }
        print LOGFILE "$cmd\n";
        print "$cmd\n";
        print LOGFILE "-------------------------\nStart trace of $name\n";
        my $value;
        $value = `$cmd`;
        print LOGFILE $value;
        print $value;
        print LOGFILE "End trace of $name\n-------------------------\n";
        print LOGFILE "exit status of $name is $?\n";
        print "exit status of $name is $?\n";
      }

The raweb directory of *Tralics* contains a lot of useful files. What we
do here is to put a symbolic link for the following files: the three bst
files, the class file, and the two logos (in PostScript and Pdf form).

Todo. Some files depend on the year, and other do not. In particular,
raweb.cls and raweb.bst depend on the year. The class file contains the
current year for headings, name of logo, name of foot bibliography, the
bst file output warnings in case of a bad year. The name \`raweb.bst'
comes from the class file, this is easy to change, but the name of the
document class is generated by Tralics.

.. container:: ltx-source

   ::

      sub link_some_files {
        link_src ("rawebalpha.bst");
        link_src ("raweb.bst");
        link_src ("rawebnum.bst");
        link_src ("raweb.cls");
        link_src ("Logo-INRIA-couleur.pdf");
        link_src ("Logo-INRIA-couleur.ps");
        link_src ("LogoRA" . $::ra_year . ".eps");
        link_src ("LogoRA" . $::ra_year . ".pdf");
      }

This takes three arguments, A, B and C. The effect is to make a symbolic
link from file A in the source directory, in the directory B, to file C.
Nothing is done if the target exists, or if the source does not exists.

.. container:: ltx-source

   ::

      sub link_file {
         my($in,$out,$short_in);
         $in = @_[0]; $out = @_[1]; $short_in = @_[2];
         $in = "$::raweb_dir/$in";
         `ln -s $in $out` if (-f $in && !-f "$out/$short_in");
      }

Same as above, but B (the directory in which is put the link) is the
current directory.

.. container:: ltx-source

   ::

      sub link_src {
         my($in,$out,$short_in);
         $short_in =@_[0];
          $in = "$::tralics_dir/raweb/$short_in";
         `ln -s $in` if (-f $in && !-f "$short_in");
      }

Some other files come from the xml directory of *Tralics*, and neeed a
different function.

.. container:: ltx-source

   ::

      sub link_xml {
         my($in,$out,$short_in);
         $short_in =@_[0];
         $in = "$::tralics_dir/xml/$short_in";
         `ln -s $in` if (-f $in && !-f "$short_in");
      }

4.2 XML mode
------------

This is done in XML mode. Note that *Tralics* creates a file named
\_hyperref.cfg that contains useful information. On the other hand, the
hyperref.cfg file is used by the hyperref package. For this reason, we
are a bit careful, when copying.

The script constructs also a TeX file, that reads the XML file (value of
``\xmlfile``). This file loads the xmltex.tex package (by Carlisle)
explicitly, and the fotex.xmt (by Rahtz) implicitly, according to some
obscure rules. There are some scheduling problems; as a consequence, the
file wapics.cfg loads the MathML stuff. The fotex.cfg file is loaded
much too late (it is needed however, since it explains how to insert the
Logo on the front page).

After these files are created, all the shell scripts are run.

.. container:: ltx-source

   ::

      sub exec_xml {
        if(!($::todo_html || $::todo_fo || $::todo_tex || $::todo_lint) ) {
          print "Postprocessor did nothing\n";
          return;
        }
        print "exec-xml for $::tex_file\n";
        link_xml_files();# links files, for instance fotex.cfg 
        make_icons();# for HTML 
        if(-f "_hyperref.cfg") { # keywords and such 
          `rm hyperref.cfg`  if(-f "hyperref.cfg");
          `mv _hyperref.cfg hyperref.cfg`;
        }
        my $name1 = "w" . $::no_year . ".cfg";
        my $name2 = $::tralics_dir . "raweb-cfg.sty";
        `rm $name1` if -f $name1;
        `ln -s $name2 $name1` if -f $name2;
        make_catalog();
        open OUTFILE, ">w$::no_year.tex";
        print OUTFILE "\\def\\xmlfile{$::no_year.fo}\n";
        print OUTFILE "\\def\\LastDeclaredEncoding{T1}\n";
        print OUTFILE "\\input{xmltex.tex}\n";
        print OUTFILE "\\end{document}\n";
        close OUTFILE;

        open LOGFILE, ">$::no_year.rh.log";
        if($::todo_html) {
          `rm -rf $::no_year$::ra_year`;   # a bit aggressive? ;
          `mkdir $::no_year$::ra_year`;
        }
        interpret_and_launch($::makefo,"raweb3fo", $::todo_fo,"makefo",0);
        interpret_and_launch($::makehtml,"rawebhtml", $::todo_html,"makehtml",0);
        interpret_and_launch($::makepdf,"junk", $::todo_tex,"pdflatex",1);
        interpret_and_launch($::makedvi,"junk", $::todo_tex,"latex",1);
        interpret_and_launch($::dvitops,"junk", $::todo_tex,"dvips",0);
        interpret_and_launch($::checkxml,"junk", $::todo_lint,"lint",0);
        close LOGFILE;
        print "(Transcript written on $::no_year.rh.log)\n";
        print "done\n";
      }

Obtaining the Pdf from the XML requires the same logos. It also needs a
hook: this is fotex.cfg. We also make sure that the DTD is visible.
Maybe, we should have a raweb2004.dtd ?

.. container:: ltx-source

   ::

      sub link_xml_files {
        link_xml ($::dtd_file);
        link_xml ("fotex.cfg");
        link_xml ("fotex.sty");
        link_xml ("fotex.sty");
        link_xml ("raweb-uni.sty");
        link_xml ("fotex-add.sty");
        link_src ("Logo-INRIA-couleur.pdf");
        link_src ("Logo-INRIA-couleur.ps");
        link_src ("LogoRA" . $::ra_year . ".eps");
        link_src ("LogoRA" . $::ra_year . ".pdf");
      }

This routine creates a icons directory and links some files. The names
of the icons that are needed can be found in the different styles
sheets. Note that the HTML files are put in a directory below the
current directory, so that it accesses them via ../icons/xx.png. As a
result, if you call the script with different teams, the icons directory
will be shared. This is only useful if you generate HTML.

.. container:: ltx-source

   ::

      sub make_icons {
        `mkdir icons` unless -d "icons";
        link_file ("icons/contents_motif.png", "icons", "contents_motif.png");
        # more lines omitted
      }

The ``make_catalog`` routine copies the file catalog.src (found in the
source directory) into the file named catalog. However, it does not
overwrite it. The string ``source_directory`` is replaced by the source
directory (check for ``$ENV`` at the end of the script).

.. container:: ltx-source

   ::

      sub make_catalog {
        return if -f "catalog";
        my $src_name = $::tralics_dir . "catalog.src";
        open(INPUT,"<$src_name");
        my(@file_string);
        while (<INPUT>) {
          push(@file_string, $_ );
        };
        $_ = join('',@file_string);
        return unless $_;
        s!source_directory/dtd/!$::tralics_dir/xml/!g;
        s!source_directory/xml/!$::tralics_dir/xml/!g;
        open OUTFILE, ">catalog";
        print OUTFILE;
        close OUTFILE;
      }

This is a very complicated procedure, whose purpose is to produce a file
with the following two lines

.. container:: ltx-source

   ::

      PUBLIC "mathml" /user/grimm/home/cvs/tralics/xml/mathml2/mathml2.dtd
      PUBLIC "teixlite" "/user/grimm/home/cvs/tralics/xml/teixlite.dtd" 

4.3 Evaluation of parameters
----------------------------

We have seen that the configuration file of *Tralics* contains lines
like: ``makefo="xsltproc --catalogs -o %A.fo %B %C";``. These lines are
copied verbatim in the user_param.pl file (with single quotes instead of
double quotes). The lines are evaluated according to the following rules

-  %A and %a are replaced by \`apics' (the name of the team),
-  %B and %b are replaced by the value of the argument (in the source
   directory), with, or without the ``.xsl`` extension.
-  %C and %c are replaced by \`apics' (the name of the team), with and
   without the ``.xml`` suffix,
-  %W and %w are replaced by \`wapics' (team name, with an initial w),
   with and without the ``.tex`` suffix,
-  %T and %t are replaced by \`apics' (alternate team name) with and
   without the ``.tex`` suffix.

.. container:: ltx-source

   ::

      sub interpret_str {
        my $B = @_[0];
        my ($Y,$S,$P) = ($::no_year,$::raweb_dir_src,$::tex_file);
        # hack here in case replacement contains %
        s/%A/$Y/g;
        s/%a/$Y/g;
        s/%B/$S$B.xsl/g;
        s/%b/$S$B/g;
        s/%C/$Y.xml/g;
        s/%c/$Y/g;
        s/%W/w$Y.tex/g;
        s/%w/w$Y/g;
        s/%T/$P.tex/g;
        s/%t/$P/g;
      }

4.4 End of rahandler.pl
-----------------------

The rahandler.pl file contains all variables and procedures shown above,
maybe in a different order. The action of the script is given here. This
read the user parameters (created by Tralics, possibly modified by the
user). Since the file is read by \`require' it should terminate with
something that does not evaluate to zero.

.. container:: ltx-source

   ::

      require "user_param.pl";

It is important to export the catalog. It is also important to export
the ``TEXMFCNF`` variable. For security reasons, it is better not to put
this in the current directory. On the other hand, you need a huge
pdflatex when reading XML.

.. container:: ltx-source

   ::

        $ENV{SGML_CATALOG_FILES}= "./catalog";   
        $ENV{TEXMFCNF} = "$::tralics_dir" . "/xml:"
           . ( defined($ENV{TEXMFCNF}) ? "$ENV{TEXMFCNF}" : "");

Action.

.. container:: ltx-source

   ::

      if($::todo_ps) { copy_and_exec(); }
      elsif ($::todo_xml) { exec_xml(); }
