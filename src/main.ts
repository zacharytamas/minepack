import { IosDaemon } from './ios_daemon'

/**
 * Main application entry point
 */
async function main() {
  const daemon = new IosDaemon()
  daemon.start()

  await daemon.configure({
    addonName: "Ball's Life",
    localBehaviorPacksPath:
      "/Users/zachary/Library/Mobile Documents/com~apple~CloudDocs/Bridge Projects/com.mojang/development_behavior_packs/Ball's Life BP",
    localResourcePacksPath:
      "/Users/zachary/Library/Mobile Documents/com~apple~CloudDocs/Bridge Projects/com.mojang/development_resource_packs/Ball's Life RP",
    localSkinPacksPath:
      "/Users/zachary/Library/Mobile Documents/com~apple~CloudDocs/Bridge Projects/com.mojang/development_skin_packs/Ball's Life SP",
  })

  await daemon.pushResourcePack('pack_icon.png')
}

// Run the main function
main().catch(console.error)
