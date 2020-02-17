#pragma once
#include <string>
#include <vector>

class SongDatabase
{
public:
	void add_song(const std::string& path);
	void del_song(const std::string& path);
private:
	bool file_existence(const std::string& name);
	std::vector<std::string> songs_;
};

