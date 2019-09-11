# WASI node

Provide a Node.js C/C++ addon supporting WASI interface.

## Build

* `npm install` 
    * Install deps and using node-gyp to build our addon module. (`build/Release/wasi_node.node`)

## Run a WebAssembly binary

```shell
node --experimental-wasm-bigint scripts/start.js <wasm_file>
```

## Documents

* [Purposal](docs/Purposal.md)
