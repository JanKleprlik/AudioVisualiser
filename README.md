# Audio Visualizer

![Linux build](https://github.com/JanKleprlik/AudioVisualiser/actions/workflows/linux.yml/badge.svg)

Application visualizing songs in real time.

## Installation

> Note that you can only compile this application on Windows in Visual Studio 2019 or newer.

Because this application is using library SFML, in order to be able to compile the code some libraries are needed. Those should be added automatically. If not, you have to add ***openal32.dll*, *sfml-audio-2.dll*, *sfml-graphics-2.dll*, *sfml-system-2.dll*, *sfml-window-2.dll*** to the Release folder. Respectively, ***sfml-xxx-d-2.dll*** to the Debug folder. You can find those files in `SFML-2.5.1` folder. You do not have to setup includes, header and libraries paths in visual studio. That is already done in configuration file for Visual Studio.

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

At each Mode screen there is triangle button at the top left corner that quits the play mode and enters the menu.

### Amplitude mode
This mode shows raw frequency waves.
![Amplitude mode](/Documentation/amplitude.PNG)
### Radio mode
This mode visualizes the song as bars. Each bar represents specific frequency and its height represents its loudness.
![Radio mode](/Documentation/radio.PNG)
### Map mode
This mode creates a map that moves with time. Map is generated similarly as radio mode is.
![Map mode](/Documentation/map.PNG)
### Space mode
This is the most complex mode. Stars speed is linked to sub bass frequencies. Planet size is mapped to human voice frequencies. Rotation of the planet is mapped to high pitch frequencies. And lastly halo is radio mode-like representation of bass frequencies.
![Space mode](/Documentation/space.PNG)
