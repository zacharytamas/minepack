#!/usr/bin/env node

import { execSync } from 'node:child_process'
import fs from 'node:fs'
import os from 'node:os'

console.log('Checking for libimobiledevice...')

try {
  // Try to get libimobiledevice version with pkg-config
  const version = execSync('pkg-config --modversion libimobiledevice', { encoding: 'utf8' }).trim()
  console.log(`Found libimobiledevice version ${version}`)

  // Try to get include paths
  const includePaths = execSync('pkg-config --cflags-only-I libimobiledevice', {
    encoding: 'utf8',
  }).trim()
  console.log(`Include paths: ${includePaths || 'None specified by pkg-config'}`)

  // Try to get library paths
  const libPaths = execSync('pkg-config --libs libimobiledevice', { encoding: 'utf8' }).trim()
  console.log(`Library paths: ${libPaths}`)
} catch (error) {
  console.warn('WARNING: Could not find libimobiledevice using pkg-config.')
  console.warn('Will try to use hardcoded paths from binding.gyp file.')
  console.warn(error.message)
}

// Check if we can actually find the header file
const platform = os.platform()
let headerFound = false
const possiblePaths = [
  '/usr/include/libimobiledevice/libimobiledevice.h',
  '/usr/local/include/libimobiledevice/libimobiledevice.h',
  '/opt/homebrew/include/libimobiledevice/libimobiledevice.h',
  '/opt/homebrew/Cellar/libimobiledevice/1.3.0_3/include/libimobiledevice/libimobiledevice.h',
]

for (const path of possiblePaths) {
  try {
    if (fs.existsSync(path)) {
      console.log(`Found header file at: ${path}`)
      headerFound = true
      break
    }
  } catch (e) {
    // Ignore errors checking paths
  }
}

if (!headerFound) {
  console.warn(
    '\nWARNING: Could not find libimobiledevice.h header file in any of these locations:',
  )
  for (const path of possiblePaths) {
    console.warn(`- ${path}`)
  }
  console.warn('\nThe build may fail. Please make sure libimobiledevice is properly installed.')

  if (platform === 'darwin') {
    console.warn(`
    On macOS, try:
    brew install libimobiledevice
    
    If using a custom Homebrew installation, you may need to set PKG_CONFIG_PATH:
    export PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig"
    `)
  } else if (platform === 'linux') {
    console.warn(`
    On Linux, try:
    sudo apt-get install libimobiledevice-dev libplist-dev libusbmuxd-dev
    `)
  }
} else {
  console.log('libimobiledevice header file found. Continuing with build...')
}
