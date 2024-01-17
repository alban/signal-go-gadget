# signal-go-gadget

signal-go-gadget

## How to use

```bash
$ go run examples/close/main.go
pid 846299
```

```bash
$ IG_EXPERIMENTAL=true sudo -E ig run ghcr.io/alban/signal-go-gadget:latest --target_pid=846299
INFO[0000] Experimental features enabled                
RUNTIME.CONTAINERNAME                                              PID                                COMM            
                                                                   850158                             main            
                                                                   850158                             main            
```

You then see the stack of the close syscall called from the GC:
```
goroutine 5 [syscall]:
syscall.Syscall(0x0?, 0x0?, 0x0?, 0x0?)
	/home/alban/programs/golang/go/src/syscall/syscall_linux.go:69 +0x25
syscall.Close(0xc00004a568?)
	/home/alban/programs/golang/go/src/syscall/zsyscall_linux_amd64.go:320 +0x25
internal/poll.(*SysFile).destroy(...)
	/home/alban/programs/golang/go/src/internal/poll/fd_unixjs.go:24
internal/poll.(*FD).destroy(0xc000074180)
	/home/alban/programs/golang/go/src/internal/poll/fd_unix.go:81 +0x51
internal/poll.(*FD).decref(0x7f8572fc0030?)
	/home/alban/programs/golang/go/src/internal/poll/fd_mutex.go:213 +0x53
internal/poll.(*FD).Close(0xc000074180)
	/home/alban/programs/golang/go/src/internal/poll/fd_unix.go:104 +0x45
os.(*file).close(0xc000074180)
	/home/alban/programs/golang/go/src/os/file_unix.go:315 +0x98
```

And from ioutil.ReadFile:
```
goroutine 1 [syscall]:
syscall.Syscall(0x0?, 0xc00005dd28?, 0xc00005dd10?, 0x47ffff?)
	/home/alban/programs/golang/go/src/syscall/syscall_linux.go:69 +0x25
syscall.Close(0xc00005dd28?)
	/home/alban/programs/golang/go/src/syscall/zsyscall_linux_amd64.go:320 +0x25
internal/poll.(*SysFile).destroy(...)
	/home/alban/programs/golang/go/src/internal/poll/fd_unixjs.go:24
internal/poll.(*FD).destroy(0xc0000741e0)
	/home/alban/programs/golang/go/src/internal/poll/fd_unix.go:81 +0x51
internal/poll.(*FD).decref(0x0?)
	/home/alban/programs/golang/go/src/internal/poll/fd_mutex.go:213 +0x53
internal/poll.(*FD).Close(0xc0000741e0)
	/home/alban/programs/golang/go/src/internal/poll/fd_unix.go:104 +0x45
os.(*file).close(0xc0000741e0)
	/home/alban/programs/golang/go/src/os/file_unix.go:315 +0x98
os.(*File).Close(0x0?)
	/home/alban/programs/golang/go/src/os/file_posix.go:23 +0x1b
os.ReadFile({0x4a88cd?, 0xc00004c010?})
	/home/alban/programs/golang/go/src/os/file.go:750 +0x28f
io/ioutil.ReadFile(...)
	/home/alban/programs/golang/go/src/io/ioutil/ioutil.go:37
main.main()
	/home/alban/go/src/github.com/alban/signal-go-gadget/examples/close/main.go:47 +0xe9
```
