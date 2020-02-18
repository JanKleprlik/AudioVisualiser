#include "SongDatabase.h"
#include <fstream>
void SongDatabase::add_song(const std::string& name)
{
	if (file_existence(name))
	{
		songs_.push_back(name);
	}
}
void SongDatabase::del_song(const std::string& name)
{
	for(auto i = songs_.begin();i < songs_.end(); i++)
	{
		if (*i == name)
		{
			songs_.erase(i);
		}
	}
}
bool SongDatabase::file_existence(const std::string& name)
{
	std::ifstream fs(name.c_str());
	return fs.good();
}
