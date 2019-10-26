const fs = require('fs');
const {promisify} = require('util');
const WasiNode = require('../index');

promisify(fs.readFile)(process.argv[2])
.then(data => WasiNode(data, {
  argv: process.argv.slice(2),
  capable_path: [process.cwd()],
}))
.then(({instance}) => {
  try{
    instance.exports._start()
  }catch(e){
    Promise.reject(e)
  }
  return instance;
})
.then((instance) => {
  fs.writeFileSync("dumpmem.txt", new Uint8Array(instance.exports.memory.buffer));
})
.catch(err => {
  console.error(err)
})