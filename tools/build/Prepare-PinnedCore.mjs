#!/usr/bin/env node
// Reconcile the released AmebaD core with a pinned official Git tree. No Git clone is required.
import fs from 'node:fs/promises';
import { createReadStream } from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { spawn } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const commit = 'f81bca75e433e35b2d27cadd178596888940fb5a';
const repository = 'Seeed-Studio/ArduinoCore-ambd';
const workspace = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../../..');
const options = {
  tree: path.join(workspace, '.wio-tools/rtl-modern/core-tree.json'),
  core: path.join(workspace, '.wio-tools/data/packages/realtek/hardware/AmebaD/3.0.5'),
  cache: path.join(workspace, '.wio-tools/rtl-modern/core-blobs'),
  apply: false,
  verbose: false,
};
for (let i = 2; i < process.argv.length; i++) {
  const arg = process.argv[i];
  if (arg === '--apply') options.apply = true;
  else if (arg === '--verbose') options.verbose = true;
  else if (['--tree', '--core', '--cache'].includes(arg) && process.argv[i + 1]) options[arg.slice(2)] = path.resolve(process.argv[++i]);
  else if (arg === '--help') {
    console.log('node Prepare-PinnedCore.mjs [--tree FILE] [--core DIR] [--cache DIR] [--apply] [--verbose]\nDefault: read-only comparison. --apply downloads and verifies changed blobs, then updates only pinned tree files.');
    process.exit(0);
  } else throw new Error(`Unknown or incomplete argument: ${arg}`);
}

const log = message => console.log(`[${new Date().toISOString()}] ${message}`);
function targetPath(root, relative) {
  if (typeof relative !== 'string' || relative.includes('\\') || relative.includes(':') || relative.includes('\0') || path.posix.isAbsolute(relative) || relative.split('/').some(part => !part || part === '.' || part === '..')) {
    throw new Error(`Unsafe Git tree path: ${relative}`);
  }
  const resolved = path.resolve(root, ...relative.split('/'));
  if (!resolved.startsWith(path.resolve(root) + path.sep)) throw new Error(`Path escapes destination: ${relative}`);
  return resolved;
}
async function blobHash(file) {
  const stat = await fs.lstat(file);
  if (!stat.isFile() || stat.isSymbolicLink()) throw new Error(`Expected regular file: ${file}`);
  const hash = crypto.createHash('sha1').update(Buffer.from(`blob ${stat.size}\0`));
  for await (const chunk of createReadStream(file)) hash.update(chunk);
  return { sha: hash.digest('hex'), size: stat.size };
}
async function matches(file, entry) {
  try {
    const actual = await blobHash(file);
    return actual.sha === entry.sha && actual.size === entry.size;
  } catch (error) {
    if (error.code === 'ENOENT') return false;
    throw error;
  }
}
async function normalizedLocalBlob(file, entry) {
  try {
    const source = await fs.readFile(file);
    if (source.includes(0)) return null;
    const text = new TextDecoder('utf-8', { fatal: true }).decode(source);
    const candidates = [Buffer.from(text.replace(/\r\n/g, '\n')), Buffer.from(text.replace(/\r?\n/g, '\r\n'))];
    for (const data of candidates) {
      if (data.length !== entry.size) continue;
      const sha = crypto.createHash('sha1').update(Buffer.from(`blob ${data.length}\0`)).update(data).digest('hex');
      if (sha === entry.sha) return data;
    }
    return null;
  } catch (error) {
    if (error.code === 'ENOENT' || error.code === 'ERR_ENCODING_INVALID_ENCODED_DATA') return null;
    throw error;
  }
}
async function parallel(items, concurrency, work) {
  let cursor = 0;
  let failed = false;
  await Promise.all(Array.from({ length: Math.min(concurrency, items.length) }, async () => {
    while (!failed && cursor < items.length) {
      const index = cursor++;
      try { await work(items[index], index); }
      catch (error) { failed = true; throw error; }
    }
  }));
}
async function extraCoreSources(root, knownPaths) {
  const extras = [];
  async function visit(relative) {
    const directory = targetPath(root, relative);
    for (const entry of await fs.readdir(directory, { withFileTypes: true })) {
      const child = `${relative}/${entry.name}`;
      if (entry.isDirectory()) await visit(child);
      else if (entry.isFile() && /\.(?:c|cc|cpp|cxx|s)$/i.test(child) && !knownPaths.has(child.toLowerCase())) extras.push(child);
      else if (entry.isSymbolicLink()) throw new Error(`Unexpected symlink in installed core sources: ${child}`);
    }
  }
  await visit('cores/arduino');
  return extras;
}
async function runCurl(args) {
  return new Promise((resolve, reject) => {
    const child = spawn(process.platform === 'win32' ? 'curl.exe' : 'curl', args, { windowsHide: true, stdio: ['ignore', 'pipe', 'pipe'] });
    let diagnostic = '';
    child.stdout.on('data', chunk => { diagnostic = (diagnostic + chunk).slice(-5000); });
    child.stderr.on('data', chunk => { diagnostic = (diagnostic + chunk).slice(-5000); });
    child.on('error', reject);
    child.on('close', code => resolve({ code, diagnostic }));
  });
}
async function cachedBlob(entry) {
  const cached = path.join(options.cache, `${entry.sha}.blob`);
  if (await matches(cached, entry)) return cached;
  const partial = cached + '.part';
  const url = `https://raw.githubusercontent.com/${repository}/${commit}/${entry.path.split('/').map(encodeURIComponent).join('/')}`;
  for (let attempt = 1; attempt <= 6; attempt++) {
    try {
      const stat = await fs.stat(partial);
      if (stat.size > entry.size) await fs.unlink(partial);
    } catch (error) { if (error.code !== 'ENOENT') throw error; }
    log(`download ${entry.path} bytes=${entry.size} attempt=${attempt}`);
    const result = await runCurl(['--fail', '--location', '--silent', '--show-error', '--proto', '=https', '--proto-redir', '=https', '--connect-timeout', '20', '--max-time', '300', '--continue-at', '-', '--output', partial, url]);
    if (await matches(partial, entry)) {
      await fs.rename(partial, cached);
      return cached;
    }
    let partialSize = 0;
    try { partialSize = (await fs.stat(partial)).size; } catch (error) { if (error.code !== 'ENOENT') throw error; }
    // A complete but wrong blob, or a server rejecting Range, must be fetched afresh.
    if (partialSize >= entry.size || result.code === 33 || result.code === 36) await fs.unlink(partial).catch(error => { if (error.code !== 'ENOENT') throw error; });
    log(`retry ${entry.path}: curl_exit=${result.code} received=${partialSize}/${entry.size} ${result.diagnostic.trim()}`);
    await new Promise(resolve => setTimeout(resolve, attempt * 750));
  }
  throw new Error(`Could not download the verified Git blob for ${entry.path}`);
}

async function main() {
  const tree = JSON.parse((await fs.readFile(options.tree, 'utf8')).replace(/^\uFEFF/, ''));
  if (tree.sha !== commit || tree.truncated !== false || !Array.isArray(tree.tree)) throw new Error('Expected a complete official Git tree for the pinned commit.');
  const coreInfo = await fs.lstat(options.core);
  if (!coreInfo.isDirectory() || coreInfo.isSymbolicLink()) throw new Error('Core destination must be an existing regular directory.');
  const entries = tree.tree.filter(entry => entry.type === 'blob');
  const windowsPaths = new Map();
  for (const entry of entries) {
    targetPath(options.core, entry.path);
    if (!['100644', '100755'].includes(entry.mode) || !/^[0-9a-f]{40}$/.test(entry.sha) || !Number.isSafeInteger(entry.size) || entry.size < 0) throw new Error(`Unsupported Git blob metadata: ${entry.path}`);
    if (entry.path === 'installed.json') throw new Error('Pinned tree unexpectedly contains Arduino CLI installed.json metadata.');
    const folded = entry.path.toLowerCase();
    if (windowsPaths.has(folded)) throw new Error(`Git tree paths collide on Windows: ${entry.path} and ${windowsPaths.get(folded)}`);
    windowsPaths.set(folded, entry.path);
  }
  const differences = [];
  const normalizedBlobs = new Map();
  await parallel(entries, 12, async entry => {
    const file = targetPath(options.core, entry.path);
    if (!(await matches(file, entry))) {
      differences.push(entry);
      const normalized = await normalizedLocalBlob(file, entry);
      if (normalized !== null) normalizedBlobs.set(entry.path, normalized);
    }
  });
  differences.sort((a, b) => a.path.localeCompare(b.path));
  const downloads = differences.filter(entry => !normalizedBlobs.has(entry.path));
  const extraSources = await extraCoreSources(options.core, windowsPaths);
  log(`commit=${commit} files=${entries.length} changed_or_missing=${differences.length} locally_normalizable=${normalizedBlobs.size} downloads=${downloads.length} download_bytes=${downloads.reduce((sum, entry) => sum + entry.size, 0)}`);
  for (const extra of extraSources) log(`Extra compiled core source will be backed up and removed: ${extra}`);
  for (const entry of (options.verbose ? differences : downloads.slice(0, 25))) console.log(`${entry.size}\t${entry.path}\t${entry.sha}`);
  if (!options.verbose && downloads.length > 25) log(`Another ${downloads.length - 25} download entries omitted; use --verbose for the full list.`);
  if (!options.apply) { log('Comparison only. Add --apply to download and reconcile the core.'); return; }
  await fs.mkdir(options.cache, { recursive: true });
  // Fetch all files before modifying the installed core. A network failure leaves it untouched.
  const downloaded = new Map();
  const uniqueDownloads = [...new Map(downloads.map(entry => [entry.sha, entry])).values()];
  await parallel(uniqueDownloads, 6, async entry => downloaded.set(entry.sha, await cachedBlob(entry)));
  const markerPath = path.join(options.core, '.wio-pinned-core.json');
  const markerPartial = markerPath + '.pending';
  await fs.unlink(markerPath).catch(error => { if (error.code !== 'ENOENT') throw error; });
  const backupRoot = path.join(options.cache, 'backups', new Date().toISOString().replace(/[:.]/g, '-') + '-' + crypto.randomBytes(4).toString('hex'));
  for (const extra of extraSources) {
    const source = targetPath(options.core, extra);
    const backup = targetPath(backupRoot, extra);
    await fs.mkdir(path.dirname(backup), { recursive: true });
    await fs.rename(source, backup);
  }
  for (const entry of differences) {
    const destination = targetPath(options.core, entry.path);
    const backup = targetPath(backupRoot, entry.path);
    try {
      await fs.mkdir(path.dirname(backup), { recursive: true });
      await fs.copyFile(destination, backup, fs.constants.COPYFILE_EXCL);
    } catch (error) { if (error.code !== 'ENOENT') throw error; }
    await fs.mkdir(path.dirname(destination), { recursive: true });
    const temporary = destination + `.wio-pin-${crypto.randomBytes(6).toString('hex')}`;
    if (normalizedBlobs.has(entry.path)) await fs.writeFile(temporary, normalizedBlobs.get(entry.path));
    else await fs.copyFile(downloaded.get(entry.sha), temporary);
    if (!(await matches(temporary, entry))) throw new Error(`Copy verification failed: ${entry.path}`);
    await fs.rename(temporary, destination);
  }
  const invalid = [];
  await parallel(entries, 12, async entry => {
    if (!(await matches(targetPath(options.core, entry.path), entry))) invalid.push(entry.path);
  });
  if (invalid.length) throw new Error(`Pinned core verification failed: ${invalid.join(', ')}`);
  const marker = { status: 'verified', repository, commit, treeSha: tree.sha, verifiedAt: new Date().toISOString(), files: entries.length, replacedFiles: differences.length, excludedCoreSources: extraSources, backupDirectory: differences.length || extraSources.length ? backupRoot : null };
  await fs.writeFile(markerPartial, JSON.stringify(marker, null, 2) + '\n');
  await fs.rename(markerPartial, markerPath);
  log(`Verified all ${entries.length} Git blobs. Marker: ${markerPath}`);
  log('Arduino CLI metadata and other extra SDK files were preserved; obsolete compiled core sources were backed up.');
}

main().catch(error => { console.error(error.stack ?? error); process.exitCode = 1; });
