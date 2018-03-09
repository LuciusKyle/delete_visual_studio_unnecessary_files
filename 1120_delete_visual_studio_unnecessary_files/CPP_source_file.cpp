
//
//									Hello World!
//------------------------------------------------------------------------------------------------
//
//						this is a cpp source file.
//
//						Lucius@LUCIUS-PC
//											--11/20/2017 11:40:32
//
//
//			Copyright (c) 2017 LuciusKyle@outlook.com. All rights reserved.
//
//------------------------------------------------------------------------------------------------
//									Goodbye World!
//


#include<string>
#include<boost/filesystem.hpp>


void validate_file(const boost::filesystem::path &file_path)
{
	if (file_path.has_extension()) {
		auto var = file_path.extension().wstring();
		if (var == L".log"
			|| var == L".obj"
			|| var == L".pdb"
			|| var == L".ilk"
			|| var == L".exp"
			|| var == L".lib"
			|| var == L".aps"
			|| var == L".lastbuildstate"
			|| var == L".tlog"
			|| var == L".idb"
			|| var == L".iobj"
			|| var == L".ipdb"
			|| var == L".ipch"
			|| var == L".VC") {
			boost::filesystem::remove(file_path);
			return;
		}
		//else if (var == L".exe") {
		//	try {
		//		boost::filesystem::remove(file_path);
		//	}
		//	catch (...) {
		//		//do nothing!
		//	}
		//	return;
		//}
	}
}

void traversal_directory(const boost::filesystem::path &cur_path)
{
	using namespace boost::filesystem;
	for (auto &directory : directory_iterator(cur_path)) {
		if (is_directory(directory.path())) {
			traversal_directory(directory.path());
		}
		else if (is_regular_file(directory.path())) {
			validate_file(directory.path());
		}
	}
	if (is_empty(cur_path)) {
		boost::filesystem::remove(cur_path);
	}
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			boost::filesystem::path my_path = argv[i];
			if (boost::filesystem::is_directory(my_path)) {
				traversal_directory(my_path);
			}
		}
	}
	else {
		traversal_directory(boost::filesystem::current_path());
	}
	return 0;
}
