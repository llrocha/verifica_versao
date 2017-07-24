#!/usr/bin/perl
####################################################################
#Interface: Leitura de arquivos e o respectivo MD5SUM
#Descricao: Le arquivos da pasta passada e retorna MD5SUM
#Script: readtws.pl
#Empresa: HBSIS Informatica Ltda
#Autor: Leonardo Larsen Rocha
#Data: 23/03/2014
####################################################################

#Includes
####################################################################
#use strict;
use POSIX;
use File::Copy;
use File::Basename;
use File::stat;
#use Fcntl ':mode';
use CGI qw(:standard Vars);
use Digest::MD5 qw(md5 md5_hex md5_base64);

## Configuracao
####################################################################
my $DEBUG = 0;
my @DIRS = ();
my @FILES = ();

#Define rotinas
####################################################################
sub LogMsg($)
{
  my $log = sprintf("#[%s] - %s\n", strftime("%Y-%m-%d %H:%M:%S", localtime), shift);
  printf($log) if($DEBUG);
}

sub ObjectType($)
{
  my $o = shift;
  return 'l' if(-l $o);
  return 'f' if(-f $o);
  return 'd' if(-d $o);
  return 's' if(-S $o);
  return 'p' if(-p $o);
  return 'c' if(-c $o);
  return 'b' if(-b $o);
  return 'u';
}

sub VerificaLetra($)
{
  my $file = shift;
  my $letra = '';

  open(IN, $file);
  while(my $line = <IN>) {
    if($line =~ /{@@@.([a-z]).@@@}/) {
      $letra = $1;
      last;
    }
  }
  close(IN);
  if(length($letra) == 0) {
    $letra = 0;
  }

  return $letra;
}

#Inicio do script
####################################################################
printf("Content-type: text/plain\r\n\r\n");

#Verifica SO
####################################################################
my $is_windows = 0;
foreach $k (keys %{ENV}) {
    if("$k" == "WINDIR") {
        $is_windows = 1;
    }
}

#Trata parametros recebidos
####################################################################
my $type;
my %REQUEST = Vars();
my $page = CGI->new();
LogMsg(sprintf("Variaveis recebidas: %d", scalar %REQUEST));
foreach my $var (sort(keys(%REQUEST))) {
  my @values = $page->param($var);
  if($var eq 'd') {
    $type = 'd';
    foreach my $val (@values) {
      LogMsg(sprintf("Variavel recebida: %s=\"%s\"", $var, $val));
      push(@DIRS, $val);
    }
  } elsif($var eq 'f') {
    $type = 'f';
    foreach my $val (@values) {
      LogMsg(sprintf("Variavel recebida: %s=\"%s\"", $var, $val));
      push(@FILES, $val);
    }
  } else {
    LogMsg(sprintf("Parametro nao reconhecido: %s", $var));
  }
}

if($type ne 'f') {
  if(scalar @DIRS == 0) {
    push(@DIRS, '/amb/eventbin');
    if(length($ENV{'SCRIPT_FILENAME'}) > 0) {
      push(@DIRS, dirname($ENV{'SCRIPT_FILENAME'})); 
    }
  }

  my @D = @DIRS;
  foreach my $dir (@D) {
    if($dir =~ /\/$/) {
      $dir = $dir . '*';
    } else {
      $dir = $dir . '/*';
    }
    push(@DIRS, $dir);
  }
} else {
  foreach my $file (@FILES) {
    push(@DIRS, $file);
  }
}

foreach my $dir (@DIRS) {
  LogMsg(sprintf("DIR=%s", $dir));
  while(my $file = < $dir >) {
    my $ctx = Digest::MD5->new;
    my $sb = stat($file);
    my $digest = "";

    my $filetype = ObjectType($file);
    if($is_windows) {
      $fileinfo = sprintf("%s.%04d.%04d.%04o.%s.%s", 
        $filetype, 0, 0, 0,
        strftime("%Y-%m-%d.%H:%M:%S", localtime 0),
        strftime("%Y-%m-%d.%H:%M:%S", localtime 0)
      );
    } else {
      $fileinfo = sprintf("%s.%04d.%04d.%04o.%s.%s", 
        $filetype, $sb->uid, $sb->gid, $sb->mode & 07777,
        strftime("%Y-%m-%d.%H:%M:%S", localtime $sb->mtime),
        strftime("%Y-%m-%d.%H:%M:%S", localtime $sb->ctime)
      );
    }
    
    if(-f $file) {
      if(open(IN, $file)) {
        $ctx->addfile(IN);
        $digest = $ctx->hexdigest;
        close(IN);
      } else {
        $ctx->add($fileinfo);
        $digest = $ctx->hexdigest;
        #$digest = $fileinfo;
      }
    } else {
      $ctx->add($fileinfo);
      $digest = $ctx->hexdigest;
      #$digest = $fileinfo;
    }
    printf("%s\t%s\t%s\t%s\n", VerificaLetra($file), $digest, $fileinfo, $file);
  }
}
