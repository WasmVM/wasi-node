{
  "targets": [
    {
      "target_name": "wasi_node",
      "sources": [
        "./src/main.cc",
        "./src/fd_prestat_get.cc",
        "./src/fd_prestat_dir_name.cc",
        "./src/environ_sizes_get.cc",
        "./src/environ_get.cc",
        "./src/args_sizes_get.cc",
        "./src/args_get.cc",
        "./src/proc_exit.cc",
        "./src/fd_fdstat_get.cc",
        "./src/sys_wrap.cc",
        "./src/util.cc"
      ]
    },
  ]
}