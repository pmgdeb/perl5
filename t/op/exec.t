#!./perl

BEGIN {
    chdir 't' if -d 't';
    @INC = ('../lib');
    require './test.pl';
}

my $vms_exit_mode = 0;

if ($^O eq 'VMS') {
    if (eval 'require VMS::Feature') {
        $vms_exit_mode = !(VMS::Feature::current("posix_exit"));
    } else {
        my $env_unix_rpt = $ENV{'DECC$FILENAME_UNIX_REPORT'} || '';
        my $env_posix_ex = $ENV{'PERL_VMS_POSIX_EXIT'} || '';
        my $unix_rpt = $env_unix_rpt =~ /^[ET1]/i; 
        my $posix_ex = $env_posix_ex =~ /^[ET1]/i;
        if (($unix_rpt || $posix_ex) ) {
            $vms_exit_mode = 0;
        } else {
            $vms_exit_mode = 1;
        }
    }
}


# suppress VMS whinging about bad execs.
use vmsish qw(hushed);

$| = 1;				# flush stdout

$ENV{LC_ALL}   = 'C';		# Forge English error messages.
$ENV{LANGUAGE} = 'C';		# Ditto in GNU.

my $Is_VMS   = $^O eq 'VMS';
my $Is_Win32 = $^O eq 'MSWin32';

plan(tests => 24);

my $Perl = which_perl();

my $exit;
SKIP: {
    skip("bug/feature of pdksh", 2) if $^O eq 'os2';

    my $tnum = curr_test();
    $exit = system qq{$Perl -le "print q{ok $tnum - interp system(EXPR)"}};
    next_test();
    is( $exit, 0, '  exited 0' );
}

my $tnum = curr_test();
$exit = system qq{$Perl -le "print q{ok $tnum - split & direct system(EXPR)"}};
next_test();
is( $exit, 0, '  exited 0' );

# On VMS and Win32 you need the quotes around the program or it won't work.
# On Unix its the opposite.
my $quote = $Is_VMS || $Is_Win32 ? '"' : '';
$tnum = curr_test();
$exit = system $Perl, '-le', 
               "${quote}print q{ok $tnum - system(PROG, LIST)}${quote}";
next_test();
is( $exit, 0, '  exited 0' );


# Some basic piped commands.  Some OS's have trouble with "helpfully"
# putting newlines on the end of piped output.  So we split this into
# newline insensitive and newline sensitive tests.
my $echo_out = `$Perl -e "print 'ok'" | $Perl -le "print <STDIN>"`;
$echo_out =~ s/\n\n/\n/g;
is( $echo_out, "ok\n", 'piped echo emulation');

{
    # here we check if extra newlines are going to be slapped on
    # piped output.
    local $TODO = 'VMS sticks newlines on everything' if $Is_VMS;

    is( scalar `$Perl -e "print 'ok'"`,
        "ok", 'no extra newlines on ``' );

    is( scalar `$Perl -e "print 'ok'" | $Perl -e "print <STDIN>"`, 
        "ok", 'no extra newlines on pipes');

    is( scalar `$Perl -le "print 'ok'" | $Perl -le "print <STDIN>"`, 
        "ok\n\n", 'doubled up newlines');

    is( scalar `$Perl -e "print 'ok'" | $Perl -le "print <STDIN>"`, 
        "ok\n", 'extra newlines on inside pipes');

    is( scalar `$Perl -le "print 'ok'" | $Perl -e "print <STDIN>"`, 
        "ok\n", 'extra newlines on outgoing pipes');

    {
	local($/) = \2;       
	$out = runperl(prog => 'print q{1234}');
	is($out, "1234", 'ignore $/ when capturing output in scalar context');
    }
}


is( system(qq{$Perl -e "exit 0"}), 0,     'Explicit exit of 0' );

my $exit_one = $vms_exit_mode ? 4 << 8 : 1 << 8;
is( system(qq{$Perl "-I../lib" -e "use vmsish qw(hushed); exit 1"}), $exit_one,
    'Explicit exit of 1' );

$rc = system { "lskdfj" } "lskdfj";
unless( ok($rc == 255 << 8 or $rc == -1 or $rc == 256 or $rc == 512) ) {
    print "# \$rc == $rc\n";
}

unless ( ok( $! == 2  or  $! =~ /\bno\b.*\bfile/i or  
             $! == 13 or  $! =~ /permission denied/i or
             $! == 22 or  $! =~ /invalid argument/i  ) ) {
    printf "# \$! eq %d, '%s'\n", $!, $!;
}


is( `$Perl -le "print 'ok'"`,   "ok\n",     'basic ``' );
is( <<`END`,                    "ok\n",     '<<`HEREDOC`' );
$Perl -le "print 'ok'"
END

{
    my $_ = qq($Perl -le "print 'ok'");
    is( readpipe, "ok\n", 'readpipe default argument' );
}

TODO: {
    my $tnum = curr_test();
    if( $^O =~ /Win32/ ) {
        print "not ok $tnum - exec failure doesn't terminate process " .
              "# TODO Win32 exec failure waits for user input\n";
        next_test();
        last TODO;
    }

    ok( !exec("lskdjfalksdjfdjfkls"), 
        "exec failure doesn't terminate process");
}

# [perl #104084] system($tied) and exec($tied) should FETCH in the parent,
#                and only once
fresh_perl_is(
  q{
    use 5.10.0;
    package D {
     sub TIESCALAR { return bless {},  shift }
     sub FETCH { say ++$y; }
    }
    tie $x, "D";
    system { $x } $^X, '-e', $x;
    say $D::y;
  },
  "1\n2\n2\n",
  {},
  'system $tied should FETCH in the parent',
);
fresh_perl_is(
  q{
    delete
     @ENV{qw{ PATH TERM DCL$PATH IFS CDPATH ENV BASH_ENV PERL5SHELL }};
    use 5.10.0;
    package D {
     sub TIESCALAR { return bless {},  shift }
     sub FETCH { say ++$y; "lskdjfalksdjfdjfkls" }
    }
    tie $x, "D";
    exec {$x} $x;
    exec $x;
    say $D::y;
  },
  "1\n2\n3\n3\n",
  { switches => ['-T'] }, # fetch was being called twice under taint
  'exec $tied should FETCH in the parent',
);

my $test = curr_test();
exec $Perl, '-le', qq{${quote}print 'ok $test - exec PROG, LIST'${quote}};
fail("This should never be reached if the exec() worked");
