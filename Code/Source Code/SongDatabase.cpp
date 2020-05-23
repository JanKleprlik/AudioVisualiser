#include <fstream>
#include "../Headerss/SongDatabase.h"

/**
 * Adds song 'name' to the database.
 */
void SongDatabase::add_song(const std::string& name)
{
	if (file_existence(name))
	{
		std::ofstream ofs;
		ofs.open(path + "song_database.txt", std::ios_base::app);
		ofs << name << std::endl;
		ofs.close();
		database.push_back(name);

	}
}
/**
 * Loads songs form 'song_database.txt' if the song exists.
 */
void SongDatabase::load_song(const std::string& name)
{
	if (file_existence(name))
	{
		database.push_back(name);
	}
}
 

/**
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
/**
 * Helper function. 
 * Check for the existance of file 'name'.
 */
bool SongDatabase::file_existence(const std::string& name)
{
	std::ifstream fs(path + name);
	return fs.good();
}
/**
* Returns name of the song at index position.
*/
std::string SongDatabase::get_song_at(int index)
{
	if (index >= database.size())
	{
		return "";
	}
	else
	{
		return database[index];
	}

}
/**
* Returns number of songs in the database.
*/
int SongDatabase::get_size()
{
	return database.size();
}

/**
 * Clears the database.
 */
void SongDatabase::clear()
{
	database.clear();
}
