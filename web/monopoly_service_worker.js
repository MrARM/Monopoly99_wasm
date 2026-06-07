const CACHE_NAME = 'monopoly-shell-v11';
const SHELL_FILES = [
  './',
  './index.html',
  './monopoly.js',
  './monopoly.wasm',
  './resources/BackGround.bmp',
  './resources/BackGroundEuro.bmp'
];

self.addEventListener('install', event => {
  event.waitUntil(caches.open(CACHE_NAME).then(cache => cache.addAll(SHELL_FILES)));
  self.skipWaiting();
});

self.addEventListener('activate', event => {
  event.waitUntil(
    caches.keys().then(keys =>
      Promise.all(keys.filter(key => key.startsWith('monopoly-shell-') && key !== CACHE_NAME).map(key => caches.delete(key)))
    )
  );
  self.clients.claim();
});

self.addEventListener('fetch', event => {
  const url = new URL(event.request.url);
  if (url.origin !== location.origin) return;
  event.respondWith(
    fetch(event.request).catch(() => caches.match(event.request))
  );
});
