
##################################
# No copyright
# All SlimGameBrew
##################################

## Do it in ruby ways
# I use to prefer Perl. But it is getting old
# So I catch up.

require "fileutils"

build_dir = "build"
source_dir = "source"
target_ide = "Visual Studio 11"

#puts __FILE__
curr_path = File.absolute_path(File.dirname(__FILE__.encode('UTF-8')))
build_dir = curr_path + "/" + build_dir
source_dir= curr_path + "/" + source_dir

if not Dir.exist? source_dir then
	puts "Error: #{source_dir} do not exist "
	exit
end

FileUtils.mkdir_p build_dir if not Dir.exist? build_dir
if not Dir.exist? build_dir then
	puts "#{build_dir} cannot be accessed"
	exit
end

exit if not Dir.chdir(build_dir)
puts Dir.getwd

cmd = "cmake -G \"#{target_ide}\" #{source_dir}"

system cmd
puts "done"
#mkdir_p build_dir if not Dir.exist? build_dir

