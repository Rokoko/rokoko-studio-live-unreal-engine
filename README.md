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
- [Usage](#usage)
- [Contributing](#contributing)
- [Issues](#issues)
- [License](#license)

## Installation
Note that this repository is an Unreal Engine project with a custom plugin, as thus you are expected to have Unreal Engine installed.
Be advised that it recommended that you disable the official plugin on the marketplace before trying to open this specific project,
such that the marketplace version will not interfere with this one.

Once you have Unreal Engine installed, simply clone this project and open it in Unreal Engine.

If you wish to simply install the latest release into your _own_ Unreal Engine project, download the Smartsuit.zip fron the release and unzip it into the Plugins folder of your project.

> [!IMPORTANT]
> Packaging and distribution of the plugin is reserved for Rokoko and is therefore not discussed on this page.

## Usage

Inside the Plugins folder of the Unreal Engine project you will find the `Smartsuit` folder which is the root of the actual plugin.

## Contributing

To contribute please create a pull request with a meaningful title, description and ping us at support@rokoko.com and/or in the official Rokoko [Discord server](https://discordapp.com/channels/897473293500710912/897482352417202176).

## Issues

For any issues please write to support@rokoko.com and considered added a Github issue for others to see in [GitHub Issues](https://github.com/RokokoElectronics/rokoko-studio-unreal-sample-project/issues).

## License

Copyright (c) Rokoko Electronics ApS. All rights reserved.

Licensed under the [GNU GPLv3](https://github.com/RokokoElectronics/rokoko-studio-unreal-sample-project/blob/master/LICENSE.md) License.
