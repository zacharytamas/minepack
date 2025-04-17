import afcClient from './afc-client'

/**
 * Main application entry point
 */
async function main() {
  try {
    // Initialize the libimobiledevice library and connect to a device
    console.log('Initializing libimobiledevice...')
    const initResult = afcClient.initialize()
    console.log(`Initialization result: ${initResult}`)

    // Start the AFC service
    console.log('Starting AFC service...')
    const afcResult = afcClient.startAFC()
    console.log(`AFC service result: ${afcResult}`)

    // Example: List the contents of the root directory
    console.log('Listing root directory...')
    const rootContents = afcClient.listDirectory('/')
    console.log('Root directory contents:')
    for (const item of rootContents) {
      console.log(`- ${item}`)
    }

    // Example: Read a file (if it exists)
    if (rootContents.includes('Documents')) {
      console.log('Listing Documents directory...')
      const documentsContents = afcClient.listDirectory('/Documents')
      console.log('Documents directory contents:')
      for (const item of documentsContents) {
        console.log(`- ${item}`)
      }
    }
  } catch (error) {
    console.error('Error:', error)
  }
}

// Run the main function
main().catch(console.error)
