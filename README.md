# WASI node

## Goal

Provide a Node.js C/C++ addon supporting WASI interface.

We can use [wasi-sdk](https://github.com/CraneStation/wasi-sdk) to compile C/C++ source code into WebAssembly binary, and then execute it by Node with our addon.

## Milestones

* Infrastructure
    * Create repository
    * Create Development container
        * wasi-sdk
        * Node.js 10

* Build a native addon and include `wasi_core.h`
    * [wasi-libc/core.h](https://github.com/CraneStation/wasi-libc/blob/wasi/libc-bottom-half/headers/public/wasi/core.h)

* Compile c source with wasi-sdk to a wasm module
    * helloworld.c

* Implement WASI functions and test with c source
    * __wasi_args_get()
    * __wasi_args_sizes_get()
    * __wasi_clock_res_get()
    * __wasi_clock_time_get()
    * __wasi_environ_get()
    * __wasi_environ_sizes_get()
    * __wasi_fd_advise()
    * __wasi_fd_allocate()
    * __wasi_fd_close()
    * __wasi_fd_datasync()
    * __wasi_fd_fdstat_get()
    * __wasi_fd_fdstat_set_flags()
    * __wasi_fd_fdstat_set_rights()
    * __wasi_fd_filestat_get()
    * __wasi_fd_filestat_set_size()
    * __wasi_fd_filestat_set_times()
    * __wasi_fd_pread()
    * __wasi_fd_prestat_dir_name()
    * __wasi_fd_prestat_get()
    * __wasi_fd_pwrite()
    * __wasi_fd_read()
    * __wasi_fd_readdir()
    * __wasi_fd_renumber()
    * __wasi_fd_seek()
    * __wasi_fd_sync()
    * __wasi_fd_tell()
    * __wasi_fd_write()
    * __wasi_path_create_directory()
    * __wasi_path_filestat_get()
    * __wasi_path_filestat_set_times()
    * __wasi_path_link()
    * __wasi_path_open()
    * __wasi_path_readlink()
    * __wasi_path_remove_directory()
    * __wasi_path_rename()
    * __wasi_path_symlink()
    * __wasi_path_unlink_file()
    * __wasi_poll_oneoff()
    * __wasi_proc_exit()
    * __wasi_proc_raise()
    * __wasi_random_get()
    * __wasi_sched_yield()
    * __wasi_sock_recv()
    * __wasi_sock_send()
    * __wasi_sock_shutdown()

## Notes

* Preferred N-API
    * [ref-1](https://nodejs.org/api/n-api.html)
    * [ref-2](https://nodejs.org/api/addons.html#addons_n_api)

## Reference

* [wasi-sdk](https://github.com/CraneStation/wasi-sdk)
* [WASI API doc](https://github.com/CraneStation/wasmtime/blob/master/docs/WASI-api.md)
* [WASI C definition](https://github.com/CraneStation/wasi-libc/blob/wasi/libc-bottom-half/headers/public/wasi/core.h)