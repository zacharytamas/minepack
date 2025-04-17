# minepack

A Minecraft Bedrock add-on compiler and bundler.

## Prerequisites

This project has the following system dependencies:

- Node.js 16+
- libimobiledevice and its dependencies (libplist, libusbmuxd, libssl, libcrypto)

### Installing libimobiledevice

#### macOS

On macOS, you can install libimobiledevice using Homebrew:

```bash
brew install libimobiledevice
```

#### Ubuntu/Debian

On Ubuntu or Debian-based systems:

```bash
sudo apt-get update
sudo apt-get install -y libimobiledevice-dev libplist-dev libusbmuxd-dev libssl-dev
```

#### Windows

On Windows, installation is more complex. We recommend using WSL (Windows Subsystem for Linux) and following the Ubuntu/Debian instructions above.

## Native Modules

This project includes native modules that interact with iOS devices using the libimobiledevice library.

### AFC Client

The AFC (Apple File Conduit) client module allows interaction with iOS devices through the AFC protocol, enabling file system access to the device.

#### Usage

```typescript
import afcClient from "./afc-client";

// Initialize the libimobiledevice library and connect to a device
const initResult = afcClient.initialize();
console.log(`Initialization result: ${initResult}`);

// Start the AFC service
const afcResult = afcClient.startAFC();
console.log(`AFC service result: ${afcResult}`);

// List the contents of a directory
const contents = afcClient.listDirectory("/");
console.log("Directory contents:", contents);

// Read a file
const fileContent = afcClient.readFile("/path/to/file.txt");
console.log("File content:", fileContent);
```

See `src/examples/afc-example.ts` for a complete example.

## Development

```bash
# Install dependencies
npm install

# Build the project
npm run build

# Run the project
npm start
```

## Troubleshooting

If you encounter issues with the native module not finding libimobiledevice:

1. Ensure libimobiledevice is installed properly for your platform
2. Check that pkg-config can find libimobiledevice: `pkg-config --libs libimobiledevice`
3. If using macOS with a custom Homebrew installation, you may need to set PKG_CONFIG_PATH: `export PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig"`
