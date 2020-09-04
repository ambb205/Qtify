# Qtify

## Contents

- [Contents](#contents)
- [Description](#description)
    - [Dependencies](#dependencies)
- [Download/Installation](#downloadinstallation)
    - [Downloading Qtify](#downloading-qtify)
    - [Installing Qtify](#installing-qtify)
- [Using Qtify](#using-qtify)

## Description
Qt/C++ based Spotify API..thing

## Dependencies
| Library                     |                                         Version          |
| :---------------------------|                                         :--------------: |
| [Qt](https://www.qt.io/)                                            |           v5.12.3 (any qt5 version should be fine)                                           |
| [nlohmann JSON](https://github.com/nlohmann/json)                                            |                                                  |

## Download/Installation

### Downloading Qtify
```bash
git clone git@github.com:ambb205/Qtify.git
cd Qtify
```

### Installing Qtify
TODO
#### Manual Installation

##### Dependencies:
Run the following commands to install requisite dependencies (assuming on Ubuntu operating system):
###### Qt
Download and follow instructions on Qt's website: [Qt](https://www.qt.io/) or
```bash
 sudo apt-get install qt5-default 
```
to avoid having to make a qt account

###### nlohmann JSONN
```bash
git clone https://github.com/nlohmann/json.git
cd json
mkdir build
cd build
cmake ../
sudo make install
```

##### Qtify:
TODO

## Using Qtify

```
./Qtify
```
TODO
