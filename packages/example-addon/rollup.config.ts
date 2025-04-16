import json from '@rollup/plugin-json'
import typescript from '@rollup/plugin-typescript'
import type { RollupOptions, Plugin } from 'rollup'

const behaviorPack = () => {
  return {
    name: 'behavior-pack',
    renderChunk(code, chunkInfo) {},
  } as Plugin
}

export default {
  plugins: [json(), typescript(), behaviorPack()],
  input: 'manifest.json',
  output: {
    dir: './dist',
  },
} satisfies RollupOptions
