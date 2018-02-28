
#define _CRT_SECURE_NO_WARNINGS



//
//									Hello World!
//------------------------------------------------------------------------------------------------
//
//						this is a cpp source file.
//
//						Lucius@LUCIUS-PC
//											--11/26/2017 01:06:32
//
//
//			Copyright (c) 2017 LuciusKyle@outlook.com. All rights reserved.
//
//------------------------------------------------------------------------------------------------
//									Goodbye World!
//


#include<map>
//#include<filesystem>
#include<boost/filesystem.hpp>
#include<boost/filesystem/path.hpp>
#include<string>
#include<openssl/sha.h>

using std::map;
using std::string;
using std::wstring;

std::multimap<string, wstring> hash_path_map;

bool file_hash(const string &file_path, string &str_sha256);
void traversal_directory(const boost::filesystem::path &cur_path);
void delete_duplicated_file(std::multimap<string, wstring> &);


int main(int argc, char *argv[])
{
	std::multimap<size_t, size_t>test;
	test.insert({ 0,0 });
	test.insert({ 0,1 });
	test.insert({ 0,2 });
	test.insert({ 1,3 });
	test.insert({ 1,4 });
	test.insert({ 1,5 });
	test.insert({ 1,6 });

	for (auto map_iter = test.begin(); map_iter != test.end(); map_iter = test.upper_bound(map_iter->first)) {
		size_t key = map_iter->first;
		size_t val = map_iter->second;
		size_t key_count = test.count(key);
	}

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

void traversal_directory(const boost::filesystem::path &cur_path)
{
	using namespace boost::filesystem;
	for (auto &directory : directory_iterator(cur_path)) {
		if (is_directory(directory.path())) {
			traversal_directory(path(directory.path()));
		}
		else if (is_regular_file(directory.path())) {
			string hash_val;
			if (file_hash(directory.path().string(), hash_val)) {
				hash_path_map.insert({ hash_val, directory.path().wstring() });
			}
		}
	}
	//if (is_empty(cur_path)) {
	//	std::remove(cur_path);
	//}
}


bool file_hash(const string &file_path, string &str_sha256)
{
	constexpr size_t my_buffer_lenth = 0x4000;
	FILE *pFile = fopen(file_path.c_str(), "rb");
	if (pFile == NULL) {
		return false;
	}
	SHA256_CTX *sha256_ctx = new SHA256_CTX;
	size_t len = 0;
	SHA256_Init(sha256_ctx);

	unsigned char *fread_buffer = new unsigned char[my_buffer_lenth] { 0 };
	while ((len = fread(fread_buffer, 1, my_buffer_lenth, pFile)) > 0) {
		SHA256_Update(sha256_ctx, fread_buffer, len);
	}

	unsigned char sha_val[SHA256_DIGEST_LENGTH + 1]{ 0 };
	SHA256_Final(sha_val, sha256_ctx);
	char buf256[65] = { '\0' };
	char tmp256[3] = { '\0' };
	for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(tmp256, "%02X", sha_val[i]);
		strcat(buf256, tmp256);
	}

	str_sha256 = buf256;

	delete sha256_ctx;
	delete[] fread_buffer;
	return true;
}

void delete_duplicated_file(std::multimap<string, wstring> &hash_path_map)
{
	for (auto map_iter = hash_path_map.begin(); map_iter != hash_path_map.end(); map_iter = hash_path_map.upper_bound(map_iter->first)) {
		if (hash_path_map.count(map_iter->first) > 1) {
			auto sub_iter = hash_path_map.equal_range(map_iter->first);
			for (auto i = sub_iter.first; i != sub_iter.second; ++i) {
				i->first; //file hash value
				i->second; //path
			}
		}
	}

}
