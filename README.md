
# Audio Visualizer

Application visualizing songs in real time.

## Installation



## Using the application
### Main menu
In main menu  you can choose one of the songs from the database and one of the visualizing modes. 
Currently selected song and mode are highlighted. If there is more than six songs in the database you can list using **triangles** under the column with song names.
After clicking **Play** chosen song will be visualized with chosen mode. 
The plus button serves the purpose to add new songs. See* Adding new songs* section.
![Main menu after startup](/Documentation/menu.png)

### Adding new songs
>Note that the only accepted format is .wav and .flac.

If you want to add new song, you have to manually add the song in .wav or .flac format to `Resources/Songs` file. Then you can either write its name into the `song_database.txt`. Or click on **plus button** at main menu screen and write its name into the Console.
>Note name that you write either to the `song_database.txt` file or `Console` must be exactly the same as name of the song you added to the `Resources/Songs` file. 

## Modes
### Amplitude mode
This mode shows raw frequency waves.
![Amplitude mode](/Documentation/amplitude.png)
### Radio mode
This mode visualizes the song as bars. Each bar represents specific frequency and its height represents its loudness.
![Radio mode](/Documentation/radio.png)
### Map mode
This mode creates a map that moves with time. Map is generated similarly as radio mode is.
![Map mode](/Documentation/map.png)
### Space mode
This is the most complex mode. Stars speed is linked to sub bass frequencies. Planet size is mapped to human voice frequencies. Rotation of the planet is mapped to high pitch frequencies. And lastly halo is radio mode-like representation of bass frequencies.
![Space mode](/Documentation/space.png)