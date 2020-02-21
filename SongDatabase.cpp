#include "SongDatabase.h"
#include <fstream>
/*
 * Adds song 'name' to the database.
 */
void SongDatabase::add_song(const std::string& name)
{
	if (file_existence(name))
	{
		database.push_back(name);
	}
}
/*
 * Deletes song 'name' from the database.
 */
void SongDatabase::del_song(const std::string& name)
{
	for(auto i = database.begin();i < database.end(); i++)
	{
		if (*i == name)
		{
			database.erase(i);
		}
	}
}
/*
 * Helper function
 * Check for the existance of file 'name'
 */
bool SongDatabase::file_existence(const std::string& name)
{
	std::ifstream fs(path + name);
	return fs.good();
}

std::string SongDatabase::get_song_at(int index)
{
	if (index > database.size())
	{
		return "";
	}
	else
	{
		return database[index];
	}

}