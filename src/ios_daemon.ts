import { type ChildProcess, spawn } from 'node:child_process'
import { EventEmitter } from 'node:events'
import { join } from 'node:path'

interface DaemonResponse {
  success: boolean
  message: string
  data?: unknown
  requestId?: string
}

interface DaemonConfig {
  addonName: string
  localBehaviorPacksPath?: string
  localResourcePacksPath?: string
  localSkinPacksPath?: string
}

type CommandType =
  | 'configure'
  | 'push_behavior_pack'
  | 'push_resource_pack'
  | 'push_skin_pack'
  | 'list_behavior_packs'
  | 'list_resource_packs'

interface Command {
  type: CommandType
  requestId?: string
  [key: string]: unknown
}

interface PendingRequest {
  resolve: (response: DaemonResponse) => void
  reject: (error: Error) => void
  timeout: NodeJS.Timeout
}

export class IosDaemon extends EventEmitter {
  private process: ChildProcess | null = null
  private isConfigured = false
  private responseBuffer = ''
  private pendingRequests = new Map<string, PendingRequest>()
  private requestCounter = 0

  constructor(private pythonPath = 'python3') {
    super()
  }

  /**
   * Start the daemon process
   */
  public start() {
    const scriptPath = join(__dirname, 'ios_daemon.py')
    this.process = spawn(this.pythonPath, [scriptPath])

    if (!this.process) {
      throw new Error('Failed to spawn daemon process')
    }

    this.process.stdout?.on('data', (data) => {
      this.handleStdout(data.toString())
    })

    this.process.stderr?.on('data', (data) => {
      console.error(`Daemon stderr: ${data}`)
    })

    this.process.on('error', (error) => {
      this.emit('error', error)
      this.process = null
      this.rejectAllPendingRequests(error)
    })

    this.process.on('close', (code) => {
      this.emit('close', code)
      this.process = null

      // Reject all pending requests
      this.rejectAllPendingRequests(new Error('Daemon process closed'))
    })
  }

  /**
   * Stop the daemon process
   */
  public stop(): void {
    if (this.process) {
      this.process.kill()
      this.process = null
      this.isConfigured = false

      // Reject all pending requests
      this.rejectAllPendingRequests(new Error('Daemon stopped'))
    }
  }

  /**
   * Configure the daemon with addon name and local paths
   */
  public async configure(config: DaemonConfig): Promise<DaemonResponse> {
    const response = await this.sendCommand({
      type: 'configure',
      addon_name: config.addonName,
      local_behavior_packs_path: config.localBehaviorPacksPath,
      local_resource_packs_path: config.localResourcePacksPath,
      local_skin_packs_path: config.localSkinPacksPath,
    })

    if (response.success) {
      this.isConfigured = true
    }

    return response
  }

  /**
   * Push a file to the behavior pack
   */
  public pushBehaviorPack(relativePath: string): Promise<DaemonResponse> {
    return this.sendCommand({
      type: 'push_behavior_pack',
      path: relativePath,
    })
  }

  /**
   * Push a file to the resource pack
   */
  public pushResourcePack(relativePath: string): Promise<DaemonResponse> {
    return this.sendCommand({
      type: 'push_resource_pack',
      path: relativePath,
    })
  }

  /**
   * Push a file to the skin pack
   */
  public pushSkinPack(relativePath: string): Promise<DaemonResponse> {
    return this.sendCommand({
      type: 'push_skin_pack',
      path: relativePath,
    })
  }

  /**
   * List behavior packs on the device
   */
  public listBehaviorPacks(): Promise<DaemonResponse> {
    return this.sendCommand({
      type: 'list_behavior_packs',
    })
  }

  /**
   * List resource packs on the device
   */
  public listResourcePacks(): Promise<DaemonResponse> {
    return this.sendCommand({
      type: 'list_resource_packs',
    })
  }

  /**
   * Send a command to the daemon and wait for a response
   */
  private sendCommand(command: Command): Promise<DaemonResponse> {
    if (!this.process) {
      return Promise.reject(new Error('Daemon is not running'))
    }

    if (command.type !== 'configure' && !this.isConfigured) {
      return Promise.reject(new Error('Daemon is not configured'))
    }

    return new Promise((resolve, reject) => {
      try {
        // Generate a unique request ID if not provided
        const requestId = command.requestId || `req_${this.requestCounter++}`
        command.requestId = requestId

        const commandJson = JSON.stringify(command)

        if (this.process?.stdin) {
          console.log(`Sending command: ${commandJson}`)
          this.process.stdin.write(`${commandJson}\n`)

          // Set a timeout for the request
          const timeout = setTimeout(() => {
            this.pendingRequests.delete(requestId)
            reject(new Error(`Request timed out: ${command.type}`))
          }, 30000) // 30 second timeout

          // Store the pending request
          this.pendingRequests.set(requestId, { resolve, reject, timeout })
        } else {
          reject(new Error('Daemon stdin is not available'))
        }
      } catch (error) {
        reject(error)
      }
    })
  }

  /**
   * Reject all pending requests with an error
   */
  private rejectAllPendingRequests(error: Error): void {
    for (const [requestId, request] of this.pendingRequests.entries()) {
      clearTimeout(request.timeout)
      request.reject(error)
    }
    this.pendingRequests.clear()
  }

  /**
   * Handle stdout data from the daemon
   */
  private handleStdout(data: string): void {
    this.responseBuffer += data

    // Check if we have a complete JSON response
    const lines = this.responseBuffer.split('\n')
    console.log(`Received lines: ${lines}`)
    // If we have more than one line, process the first complete line
    if (lines.length > 1) {
      const responseLine = lines[0]
      this.responseBuffer = lines.slice(1).join('\n')

      try {
        if (responseLine) {
          const response = JSON.parse(responseLine) as DaemonResponse

          // If the response has a requestId, find and resolve the corresponding request
          if (response.requestId && this.pendingRequests.has(response.requestId)) {
            const request = this.pendingRequests.get(response.requestId)
            if (request) {
              clearTimeout(request.timeout)
              this.pendingRequests.delete(response.requestId)
              request.resolve(response)
            }
          } else {
            // Otherwise emit the response as an event
            this.emit('response', response)
          }
        }
      } catch (error) {
        console.error('Error parsing daemon response:', error)
      }
    }
  }
}
