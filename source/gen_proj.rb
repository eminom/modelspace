

## So right
def outs(*a)
	puts *a
end

# Find all the directories
#   without ./, the root directory will not be searched.
Dir.glob("./**/") do |path|
	#puts path
	# puts path
	src_list_name = "SRC_LIST"
	#outs "set(#{src_list_name}"
	def detect(p, &cb)
		count = 0
		for suf in ['.h', '.c', '.cc', '.cpp', '.cxx'] do
			Dir.glob("#{p}*#{suf}") do |one|
				yield one
				count += 1
			end
		end
		count
	end		
	ls = detect(path){|x|}
	if ls > 0 then
		outs "set(#{src_list_name}"
	 	detect(path){|x| puts x}
	 	outs ")"
		outs "set(FULL_SRC_LIST ${FULL_SRC_LIST} ${#{src_list_name}})"  # Merge to full list
		group_name = path[2..-1]
		if group_name.length > 0 then
			group_name = group_name.split('/').join('\\\\')
			outs "source_group(#{group_name} FILES ${#{src_list_name}})"
		else
			outs "source_group(Source FILES ${#{src_list_name}})"
		end
	end
	puts ""
end



