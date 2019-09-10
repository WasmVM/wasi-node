const fs = require('fs');
const {promisify} = require('util');
const wasi = require('../index');

promisify(fs.readFile)(process.argv[2])
.then(data => WebAssembly.instantiate(data, wasi))
.then(({instance}) => {
  wasi.init(instance);
  instance.exports._start()
})
.catch(err => {
  console.error(err)
})