# Rokoko Studio Live Plugin for Unreal Engine
[Rokoko Studio](https://www.rokoko.com/en/products/studio) is a powerful and intuitive software for recording, visualizing and exporting motion capture.

This plugin let's you stream animation data from Rokoko Studio into Unreal Engine to preview and work with all your motion capture data easily and intuitively.

## Requirements
- Unreal Engine 5.x
- Rokoko Studio or Rokoko Studio Legacy (starting from 1.19.0b)
- The plugin supports both Windows, Mac and Linux

## Features
- Live stream data:
  * Up to 3 actors that can all include both body, face (52 blendshapes) and finger data at the same time.
  * Custom character data
- Control Rokoko Studio from within Unreal Engine

## Latest Release
Latest release requires Unreal Engine 5.4 and Rokoko Studio or Rokoko Studio Legacy (starting from 1.19.0b).
To get access to the latest streaming features you need Rokoko Studio 2.4.5 or higher.

https://github.com/Rokoko/rokoko-studio-live-unreal-engine/releases/tag/v1.12.3-UE5.4

## Table of Contents
- [Installation](#installation)
- [Building Manually](#building-manually)
- [Usage](#usage)
- [Contributing](#contributing)
- [Issues](#issues)
- [License](#license)

## Installation
> [!WARNING]
> Be advised that it recommended that you disable the official plugin on the marketplace before trying to open this specific project, such that the marketplace version will not interfere with this one.

Ensure that the special Plugins folder is created in your Unreal Engine project. Download the `Smartsuit.zip` from the respected [release](https://github.com/Rokoko/rokoko-studio-live-unreal-engine/releases)
and unzip it into the Plugins folder.
After copying the folder you may need to restart Unreal Engine, and ensure that `Show Pluging Content` is toggled ON in the settings of your Content Browser.

The latest release offers Windows and MacOS support.

To take full advantage of the plugin, ensure that the official LiveLink plugin is installed as well.

## Building Manually
Note that this repository is an Unreal Engine project with a custom plugin, as thus you are expected to have Unreal Engine installed.
Once you have Unreal Engine installed, simply clone this project and open it in Unreal Engine.

To build the plugin run the Package command from inside the Plugin window:
![image](https://github.com/user-attachments/assets/ff8060e8-bcd4-4eb9-94d3-9c35b501258c)

Should any compile errors arise, or any SDKs be missing, you must deal with them before retrying the Package command. If Packaging, without any local changes to the plugin, results in any compile errors related to the plugin please consider reporting it as an [issue](#issues).

The latest version of the project supports Windows, Linux and MacOS support.

> [!IMPORTANT]
> Packaging and distribution of the plugin is reserved for Rokoko and is therefore not discussed on this page.

### LZ4
For compression we use [LZ4](https://github.com/lz4/lz4) and the latest version includes binaries build from the [official 1.9.4 version](https://github.com/lz4/lz4/releases/tag/v1.9.4).

## Usage

Inside the Plugins folder of the Unreal Engine project you will find the `Smartsuit` folder which is the root of the actual plugin.

The plugin provides you with a `Rokoko Studio Source` livelink source, and once such a source has been instantiated you can stream from Rokoko Studio.

## Contributing

To contribute please create a pull request with a meaningful title, description and ping us at support@rokoko.com and/or in the official Rokoko [Discord server](https://discordapp.com/channels/897473293500710912/897482352417202176).

## Issues

For any issues please write to support@rokoko.com and considered added a Github issue for others to see in [GitHub Issues](https://github.com/RokokoElectronics/rokoko-studio-unreal-sample-project/issues).

## License

Copyright (c) Rokoko Electronics ApS. All rights reserved.

Licensed under the [GNU GPLv3](https://github.com/RokokoElectronics/rokoko-studio-unreal-sample-project/blob/master/LICENSE.md) License.
