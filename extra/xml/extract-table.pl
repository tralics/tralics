#! /usr/bin/perl
# -*- perl -*-

$empty =""; # hack
$Id="\$Id: extract-table.pl,v 2.7 2007/01/19 18:48:41 grimm Exp $empty";
package main;
use strict;  # teste les arguments

$::Id =~ /,v (\S*?) /;
print "extract-table.pl $1 Copyright INRIA/APICS 2006-2007, Jos\'e Grimm\n";
print "Licensed under the CeCILL Free Software Licensing Agreement\n";

$::SGML_CATALOG_FILES= "./catalog";
$::xml_dir="../../xml";

$::name = shift;      ## argument of the script
$::name_o = $::name . ".xml";  ## tralics file
$::name_a = $::name . "_e.xml"; ## the trees
$::name_t = $::name . "_g.xml";
$::name_x = $::name . "html.xsl";
$::img_ctr = 0;
$::pstoimg_cmd 	= "pstoimg"; ## pathname of pstoimg
$::use_convert = 0; ## Use convert instead of pstoimg if non-zero
$::tralics_options= "-noentnames -nostraightquotes -nozerowidthspace -trivialmath=7 -usequotes";

print "Working on $::name\n";
-f $::name . ".tex" or die ("No tex source"); 

sub slurp_input {
    my($file) = @_;
    my(@file_string);
    open(INPUT, "<$file") or die "File $file cannot be opened";
    while (<INPUT>) {
        push(@file_string, $_ );
    };
    close INPUT;
    join('',@file_string);
}


# This creates the XML file with only the trees
sub extract() {
  $ENV{SGML_CATALOG_FILES}= $::SGML_CATALOG_FILES;
  `xsltproc --catalogs -o  $::name_a extract.xsl $::name_o`;
}

## je veux les versions patchees
sub link_files() {
  unlink("images.cfg");
  `ln -s $::xml_dir/raweb-cfg.sty images.cfg`;
  unlink("fotex-add.sty");
  `ln -s $::xml_dir/fotex-add.sty`;
  unlink("fotex.sty");
  `ln -s $::xml_dir/fotex.sty`;
  unlink("fotex.xmt");
  `ln -s $::xml_dir/fotex.xmt`;
  unlink("raweb-uni.sty");
  `ln -s $::xml_dir/raweb-uni.sty`;
}

# cree le fichier images.tex et autres
sub prepare_for_latex {
  open OUT,"> images.tex" || die "cannot create images.tex\n";
  print OUT "\\def\\xmlfile{$::name_a}\n";
  print OUT "\\def\\LastDeclaredEncoding{T1}\n";
  print OUT "\\input{xmltex.tex}\n";
  print OUT "\\end{document}\n";
  close OUT;
  open OUT,"> fotex-supp.tex" || die "cannot create fotex-supp.tex\n";
  print OUT "\\textwidth=17cm\n";
  print OUT "\\RequirePackage{tree-dvips}\n";
  print OUT "\\hyperreffalse\n";
  close OUT;
}


sub convert_table {
  my $x = $_[0];
  my $y = $_[1];
  if($x eq "preview" || $y =~ /<\/node>/) {
    $::img_ctr ++;
    return "<img src='images/tree_image_$::img_ctr.png' alt='Tree $::img_ctr'/>";
  } else { return $y; }
}

sub read_log {
  my ($logfile) = @_;
  my ($name);
  my $TeXpt = 72/72.27;
  my $image_counter;
  open(LOG, "<$logfile") || die "\nCannot read logfile $logfile\n";
  while (<LOG>) {
    if (/latex2htmlSize|l2hSize/) {
      /:([^:]*):/;
      $name = $1;
      ++$image_counter;
      s/:([0-9.]*)pt/$::x_height{$name} = $1*$TeXpt;''/e;
      s/::([0-9.]*)pt/$::x_depth{$name} = $1*$TeXpt;''/e;
      s/::([0-9.]*)pt/$::x_width{$name} = $1*$TeXpt;''/e;
      $::x_align{$name} = "align = 'bottom'";
      if($::x_depth{$name}) { $::x_align{$name} = "align='middle'";}
    }
  }
  print STDOUT "Processing $image_counter images \n";
  $::ps_counter = $image_counter;
  close(LOG);
}

sub create_one_image {
  my $name = $_[0];
  my $im_name = "images";
  if($name<100) { $im_name .= "0";}
  if($name<10) { $im_name .= "0";}
  $im_name .= $name;
  my $w = $::x_width{$name};
  my $h = $::x_height{$name};
  my $d = $::x_depth{$name};
  $w = int($w + 0.6 + 20);
  $h = int($h + $d + 0.6 + 20);

  my $size = "-geometry ${w}x$h";
  my $cmd = "";
  print "$name ";
  if ($::use_convert) {
    $cmd = $::convert_cmd;
    $cmd .= " -crop ${w}x$h+64+44 ";
    $cmd .= " tmpdir/$im_name tree_image_$name.png ";
  } else {
    $cmd = $::pstoimg_cmd;
    $cmd .= " -type png -tmp tmpdir";
    $cmd .= " -discard -interlace -antialias";
    $cmd .= " -depth 1 -scale 1.4 $size";
    # marges 78,72 ou 72,72 ???
    $cmd .= " -margins 62,41 -crop a -transparent";
    $cmd .= " -out tree_image_$name.png tmpdir/$im_name";
  }
  print LOG "$cmd\n";
  print LOG `$cmd`;
  
  my $info = `file tree_image_$name.png`;
  if($info =~ /PNG image data, (\d+) x (\d+),/) {
    $::x_align{$name} .= " width ='$1' height ='$2'";
  }
  `mv tree_image_$name.png images`;
}


sub convert_to_png {
  `rm -rf tmpdir`;
  `mkdir tmpdir`;
  `rm -rf images`;
  `mkdir images`;
  my $DVIPSOPT="";
  my $dvips  = "dvips -S1 -i $DVIPSOPT -otmpdir/images ./images.dvi";
  open LOG, "> pstoimg.log";
  print LOG `$dvips 2>&1`;
  print "dvips done\n";
  my $i;
  foreach $i (1 .. $::ps_counter) { create_one_image ($i); }
  `rm -rf tmpdir`;
  print "\n";
  close LOG;
}

sub main()  {
  `tralics $::name $::tralics_options`;
  $::file = slurp_input($::name_o);
  link_files();
  prepare_for_latex();
  print "Creating  $::name_a\n";
  extract;
  print "Running tex\n";
  system("latex", "images");
  read_log("images.log");
  print "Converting images\n";
  convert_to_png;
  $::file =~ s!<(table|preview).*?</\1>!convert_table($1,$&)!egs;
  open OUT,"> $::name_t" || die "cannot open $::name_t\n";
  print OUT "$::file";
  close OUT;
  print "Seen $::img_ctr elements\n";
  ($::ps_counter == $::img_ctr) or die "Wrong number of images"; 
  print "xsltproc --catalogs --stringparam Main $::name -o $::name.html  $::name_x $::name_t\n";
  `xsltproc --catalogs --stringparam Main $::name -o $::name.html  $::name_x $::name_t`

}



main;
