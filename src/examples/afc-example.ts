import { AFCClient } from '../afc-client'

/**
 * Example demonstrating how to use the AFC client to interact with an iOS device
 */
async function main() {
  try {
    const afcClient = new AFCClient()

    const localPath = `${__filename}` as any
    console.log(`Local path: ${localPath}`)

    const putFileResult = afcClient.putFile(localPath, 'afc-example.ts')
    console.log(`Put file result: ${putFileResult}`)
  } catch (error) {
    console.error('Error:', error)
  }
}

// Run the example
main().catch(console.error)
