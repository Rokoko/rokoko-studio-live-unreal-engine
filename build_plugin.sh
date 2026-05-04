#!/bin/bash

PLUGINNAME="Smartsuit"
ENGINEDIR="$HOME/UnrealEngine-5.7/UnrealEngine" # Adjust this path to your Unreal Engine root
RUNUAT="$ENGINEDIR/Engine/Build/BatchFiles/RunUAT.sh"
PACKAGE_DIR="$(pwd)/Build/Plugins/$PLUGINNAME"
PLUGIN_DIR="$(pwd)/Plugins/$PLUGINNAME"

rm -rf "$PACKAGE_DIR"

"$RUNUAT" BuildPlugin -Plugin="$PLUGIN_DIR/$PLUGINNAME.uplugin" -Package="$PACKAGE_DIR" -Rocket -TargetPlatforms=Linux

# Uncomment below to clean plugin binaries/intermediate if needed
# rm -rf "$PLUGIN_DIR/Binaries"
# rm -rf "$PLUGIN_DIR/Intermediate"

# Uncomment below to copy binaries back if needed
# cp -r "$PACKAGE_DIR/Binaries" "$PLUGIN_DIR/Binaries"

echo "Build complete."
