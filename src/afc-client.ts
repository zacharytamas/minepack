import { createRequire } from 'node:module'

interface IAFCClient {
  initialize(): string
  putFile(clientData: any, srcPath: `/${string}`, destPath: string): string
  // startAFC(): string
  // listDirectory(path: string): string[]
  // readFile(path: string): string
}

// Load the native module
const require = createRequire(import.meta.url)
const nativeModule = require('../build/Release/afc_client.node') as IAFCClient

export class AFCClient {
  private clientData: any

  constructor() {
    this.clientData = nativeModule.initialize()
  }

  putFile(srcPath: `/${string}`, destPath: string): string {
    return nativeModule.putFile(this.clientData, srcPath, destPath)
  }
}
