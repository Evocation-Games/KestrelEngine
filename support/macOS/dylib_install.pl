#!/usr/bin/perl
# Copyright (c) 2021 Tom Hancocks
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Trim function
sub trim
{
	my $str = $_[0];
	$str =~ s/^\s+|\n+//g;
	return $str;
}

# We need to extract certain information about the binary and the application
# bundle that we are installing.
my ($bin_path) = @ARGV;

if (!defined $bin_path) {
	die("You must provide a mach-o binary.");
}

my $macos_path = trim(`dirname ${bin_path}`);
my $contents_path = trim(`dirname ${macos_path}`);
my $frameworks_path = trim("${contents_path}/Frameworks");

# Make sure the frameworks directory actually exists in the application bundle.
`mkdir -p ${frameworks_path}`;

# Setup a selection of functions that are responsible for moving files and
# altering linking.
sub install_name_tool
{
	local ($path, $dylib_path) = ($_[0], $_[1]);
	local $dylib_name = trim(`basename ${dylib_path}`);
	local $dylib_link_path = "\@executable_path/../Frameworks/${dylib_name}";
	`install_name_tool -change "${dylib_path}" "${dylib_link_path}" "${path}"`;
}

sub get_dylib_install_path
{
	local ($dylib_name) = (trim(`basename $_[0]`));
	return "${frameworks_path}/${dylib_name}";
}

sub copy_dylib
{
	local ($dylib_path, $dylib_name) = ($_[0], get_dylib_install_path($_[0]));
	`cp -v ${dylib_path} ${dylib_install_path}`;
	`chmod 0755 ${dylib_install_path}`;
	return $dylib_install_path;
}

# Setup a subroutine to handle the actual DYLIB installation. This is 
# unfortunately a recursive operation, as actual DYLIBs can reference other
# DYLIBs.
sub install_dylib
{
	local $base = $_[0];
	local @result = split /\n/, `otool -L ${base}`;
	while (local $dylib = shift(@result)) {
		local $dylib_path = trim((split / /, $dylib)[0]);
		local $dylib_install_path = get_dylib_install_path($dylib_path);

		if ($dylib_install_path eq $base) {
			install_name_tool($dylib_install_path, $dylib_path);
		}

		# Check if the DYLIB is a user one (located in a brew install location)
		elsif ((rindex $dylib_path, "/usr/local") == 0) {
			local $dylib_install_path = copy_dylib($dylib_path);
			print("Installing DYLIB to ${dylib_install_path}\n");
			install_name_tool($base, $dylib_path);
			install_dylib($dylib_install_path);
		}
	}
}

install_dylib($bin_path);