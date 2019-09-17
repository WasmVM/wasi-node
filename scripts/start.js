const fs = require('fs');
const {promisify} = require('util');
const wasi = require('../index');

promisify(fs.readFile)(process.argv[2])
.then(data => WebAssembly.instantiate(data, wasi))
.then(({instance}) => {
  wasi.init(instance, {
    capable_path: [process.cwd()]
  });
  instance.exports._start()
  let buffer = new Uint8Array(instance.exports.memory.buffer);
  fs.writeFileSync("dumpmem.txt", buffer);
})
.catch(err => {
  console.error(err)
})