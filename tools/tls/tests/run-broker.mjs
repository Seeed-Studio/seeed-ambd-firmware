#!/usr/bin/env node
// Run after tools/tls/build.mjs has generated the private source/header tree.
// SPDX-License-Identifier: MIT
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { spawnSync } from 'node:child_process';
const here = path.dirname(fileURLToPath(import.meta.url));
const args = process.argv.slice(2);
const option = name => args[args.indexOf(name) + 1];
if (!args.includes('--zig') || !args.includes('--tls-build') || !args.includes('--out'))
  throw new Error('Usage: node run-broker.mjs --zig <zig.exe> --tls-build <generated TLS tree> --out <test output directory>');
const build = path.resolve(option('--tls-build'));
const out = path.resolve(option('--out'));
fs.mkdirSync(out, { recursive: true });
const executable = path.join(out, process.platform === 'win32' ? 'broker-state.exe' : 'broker-state');
const sources = ['aes.c', 'ctr_drbg.c', 'constant_time.c', 'platform.c', 'platform_util.c']
  .map(name => path.join(build, 'library', name));
const compile = spawnSync(path.resolve(option('--zig')), ['cc', '-std=c99', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
  '-I' + path.join(here, 'stubs'), '-I' + path.join(build, 'include'), '-I' + path.join(build, 'library'),
  path.join(here, 'broker-state.c'), ...sources, '-o', executable], {
  stdio: 'inherit', windowsHide: true,
  env: { ...process.env, ZIG_LOCAL_CACHE_DIR: path.join(out, 'zig-local'), ZIG_GLOBAL_CACHE_DIR: path.join(out, 'zig-global') },
});
if (compile.error) throw compile.error;
if (compile.status !== 0) process.exit(compile.status ?? 1);
const test = spawnSync(executable, [], { stdio: 'inherit', windowsHide: true });
if (test.error) throw test.error;
process.exit(test.status ?? 1);
