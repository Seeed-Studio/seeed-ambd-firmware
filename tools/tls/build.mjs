#!/usr/bin/env node
// Rebuild the RPC-only TLS library without changing the SDK's Mbed TLS 2.4 ABI.
// SPDX-License-Identifier: MIT
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const scriptDir = path.dirname(fileURLToPath(import.meta.url));
const args = process.argv.slice(2);
function option(name, fallback) {
  const index = args.indexOf(name);
  return index < 0 ? fallback : args[index + 1];
}
const out = path.resolve(option('--out', path.join(scriptDir, '..', '..', '.build', 'tls')));
const toolchain = path.resolve(option('--toolchain', ''));
const sourceOverride = option('--source', undefined);
const archiveOverride = option('--archive', undefined);
const core = option('--core', undefined);
const gcc = path.join(toolchain, process.platform === 'win32' ? 'arm-none-eabi-gcc.exe' : 'arm-none-eabi-gcc');
const ar = path.join(toolchain, process.platform === 'win32' ? 'arm-none-eabi-ar.exe' : 'arm-none-eabi-ar');
const nm = path.join(toolchain, process.platform === 'win32' ? 'arm-none-eabi-nm.exe' : 'arm-none-eabi-nm');
const version = '3.6.7';
const sha256 = 'a7e8bcbec0e6f761b4af24f25677626b35f762f68eef79c08677a363212d11f6';
const url = `https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-${version}/mbedtls-${version}.tar.bz2`;
fs.mkdirSync(out, { recursive: true });
if (!fs.existsSync(gcc)) throw new Error('Pass --toolchain <directory containing arm-none-eabi-gcc>.');

function run(exe, parameters, options = {}) {
  const result = spawnSync(exe, parameters, { encoding: 'utf8', windowsHide: true, ...options });
  if (result.error) throw result.error;
  if (result.status !== 0) throw new Error(`${exe} failed (${result.status}):\n${result.stdout ?? ''}${result.stderr ?? ''}`);
  return result.stdout ?? '';
}
let upstream = sourceOverride && path.resolve(sourceOverride);
let sourceFiles;
if (!upstream) {
  const archive = archiveOverride ? path.resolve(archiveOverride) : path.join(out, `mbedtls-${version}.tar.bz2`);
  if (!fs.existsSync(archive)) run('curl.exe', ['--fail', '--location', '--retry', '3', '--output', archive, url], { stdio: 'inherit' });
  const actual = crypto.createHash('sha256').update(fs.readFileSync(archive)).digest('hex');
  if (actual !== sha256) throw new Error(`Mbed TLS source SHA256 mismatch: ${actual}`);
  upstream = path.join(out, `mbedtls-${version}`);
  // Overwrite the extracted cache from the checked archive on every build.
  // Only archive-listed translation units enter the build, so an obsolete
  // cached .c file cannot become an accidental extra object.
  const entries = run('tar', ['-tf', archive]).split(/\r?\n/);
  sourceFiles = entries.filter(name => new RegExp(`^mbedtls-${version.replaceAll('.', '\\.')}\\/library\\/[^/]+\\.c$`).test(name))
    .map(name => path.basename(name).replace(/^psa_/, 'wio_psa_').replace(/^mbedtls_/, 'wio_mbedtls_'));
  if (sourceFiles.length < 100) throw new Error('Unexpected Mbed TLS source archive layout.');
  run('tar', ['-xf', archive, '-C', out]);
}
const upstreamVersion = fs.readFileSync(path.join(upstream, 'include/mbedtls/build_info.h'), 'utf8');
if (!upstreamVersion.includes(`#define MBEDTLS_VERSION_STRING         "${version}"`)) throw new Error('Unexpected upstream version.');

// Rewrite source, all headers, macros, guards, types and external symbols. Both
// libraries can coexist even in one translation unit, without include-path
// shadowing or resolving a 2.4 SDK context against a 3.6 implementation.
function namespace(text) {
  return text.replaceAll('mbedtls/', 'wio_mbedtls/')
    .replaceAll('psa/', 'wio_psa/')
    .replace(/\bmbedtls_/g, 'wio_mbedtls_')
    .replace(/\bMBEDTLS_/g, 'WIO_MBEDTLS_')
    .replace(/\bpsa_/g, 'wio_psa_')
    .replace(/\bPSA_/g, 'WIO_PSA_');
}
function rewriteTree(from, to) {
  fs.mkdirSync(to, { recursive: true });
  for (const entry of fs.readdirSync(from, { withFileTypes: true })) {
    const input = path.join(from, entry.name);
    const output = path.join(to, namespace(entry.name));
    if (entry.isDirectory()) rewriteTree(input, output);
    else fs.writeFileSync(output, namespace(fs.readFileSync(input, 'utf8')));
  }
}
const include = path.join(out, 'include');
const library = path.join(out, 'library');
const objects = path.join(out, 'objects');
rewriteTree(path.join(upstream, 'include/mbedtls'), path.join(include, 'wio_mbedtls'));
rewriteTree(path.join(upstream, 'include/psa'), path.join(include, 'wio_psa'));
rewriteTree(path.join(upstream, 'library'), library);
fs.mkdirSync(objects, { recursive: true });
fs.writeFileSync(path.join(include, 'wio_mbedtls/wio_mbedtls_config.h'), namespace(fs.readFileSync(path.join(scriptDir, 'mbedtls_config.h'), 'utf8')));
const publicHeaders = ['platform.h', 'platform_util.h', 'net_sockets.h', 'ssl.h', 'entropy.h', 'ctr_drbg.h', 'error.h', 'sha256.h', 'oid.h', 'version.h'];
fs.writeFileSync(path.join(include, 'wio_tls.h'), '#ifndef WIO_TLS_PUBLIC_H\n#define WIO_TLS_PUBLIC_H\n' + publicHeaders.map(header => `#include "wio_mbedtls/${header}"`).join('\n') + '\n#ifdef __cplusplus\nextern "C" {\n#endif\nint wio_tls_provision(const unsigned char *seed, size_t length, unsigned int epoch);\nint wio_tls_ready(void);\n#ifdef __cplusplus\n}\n#endif\n#endif\n');

const flags = ['-mcpu=cortex-m33', '-mthumb', '-mcmse', '-mfloat-abi=hard', '-mfpu=fpv5-sp-d16', '-Os', '-std=gnu99', '-ffunction-sections', '-fdata-sections', '-fno-common', '-Wall', '-Wextra', '-I' + include, '-I' + library];
const objectFiles = [];
for (const name of (sourceFiles ?? fs.readdirSync(library)).filter(name => name.endsWith('.c') && name !== 'net_sockets.c').sort()) {
  const output = path.join(objects, name.replace(/\.c$/, '.o'));
  run(gcc, [...flags, '-c', path.join(library, name), '-o', output]);
  objectFiles.push(output);
}
if (core) {
  const sdk = path.join(path.resolve(core), 'system/libameba/sdk');
  const platform = fs.readFileSync(path.join(path.resolve(core), 'platform.txt'), 'utf8');
  const project = platform.match(/^ameba\.project=(.+)$/m)?.[1].trim();
  const includeLine = platform.match(/^compiler\.ameba\.c\.include=(.+)$/m)?.[1];
  if (!project || !includeLine) throw new Error('Unsupported Ameba platform include configuration.');
  const sdkIncludes = [...includeLine.matchAll(/"-I\{ameba\.sdkpath\}\/([^"]+)"/g)]
    .map(match => '-I' + path.join(sdk, match[1].replaceAll('{ameba.project}', project)));
  const port = path.join(objects, 'wio_tls_port.o');
  run(gcc, [...flags, ...sdkIncludes, '-DARDUINO_SDK', '-DCONFIG_PLATFORM_8721D', '-c', path.join(scriptDir, 'port.c'), '-o', port]);
  objectFiles.push(port);
}
const archive = path.join(out, 'lib_wio_mbedtls367.a');
// ar replaces members by name but doesn't remove obsolete members, so use a new
// archive each run; no recursive deletion of generated directories is needed.
if (fs.existsSync(archive)) fs.unlinkSync(archive);
run(ar, ['rcs', archive, ...objectFiles]);
const symbols = run(nm, ['-A', '-g', archive]);
if (/\b[UTDBR] (?:mbedtls_|psa_)/m.test(symbols)) throw new Error('Legacy unprefixed TLS symbol escaped the private namespace.');
const defined = new Set([...symbols.matchAll(/\b[TDBRWV] (\S+)$/gm)].map(match => match[1]));
const unresolved = [...new Set([...symbols.matchAll(/\bU (\S+)$/gm)].map(match => match[1]))].filter(name => !defined.has(name)).sort();
if (core && unresolved.some(name => name.startsWith('wio_mbedtls_') || name.startsWith('wio_psa_')))
  throw new Error('Unresolved private TLS symbol: ' + unresolved.join(', '));
fs.writeFileSync(path.join(out, 'symbols.txt'), symbols);
for (const name of ['LICENSE', 'LICENSES']) {
  const license = path.join(upstream, name);
  if (fs.existsSync(license) && fs.statSync(license).isFile()) fs.copyFileSync(license, path.join(out, `upstream-${name}`));
}
fs.writeFileSync(path.join(out, 'manifest.json'), JSON.stringify({
  version, url, sha256, sourceOverride: !!sourceOverride,
  compiler: run(gcc, ['--version']).split('\n')[0], flags,
  archive: path.basename(archive), archiveSha256: crypto.createHash('sha256').update(fs.readFileSync(archive)).digest('hex'),
  configSha256: crypto.createHash('sha256').update(fs.readFileSync(path.join(scriptDir, 'mbedtls_config.h'))).digest('hex'),
  portSha256: crypto.createHash('sha256').update(fs.readFileSync(path.join(scriptDir, 'port.c'))).digest('hex'),
  externalSymbols: unresolved,
}, null, 2) + '\n');
console.log(`Built ${archive} (${objectFiles.length} source files); headers: ${include}`);
