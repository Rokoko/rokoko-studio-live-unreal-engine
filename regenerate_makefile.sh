set -e
ENGINE="$HOME/UnrealEngine-5.8/UnrealEngine"
SCRIPT="$ENGINE/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh"
PROJECT="$HOME/rokoko-studio-live-unreal-engine/RokokoUEPlugin.uproject"
if [[ -f "$SCRIPT" ]]; then
  echo "Found GenerateProjectFiles.sh at $SCRIPT"
  bash "$SCRIPT" -project="$PROJECT" -game -engine -makefile
else
  echo "GenerateProjectFiles.sh not found at $SCRIPT" >&2
  echo "Trying UnrealVersionSelector..."
  if command -v UnrealVersionSelector >/dev/null 2>&1; then
    UnrealVersionSelector -projectfiles -project="$PROJECT"
  else
    echo "UnrealVersionSelector not found in PATH." >&2
    exit 1
  fi
fi
