// Use `go run foo.go` to run your program

package main

import (
	. "fmt"
	"runtime"
	"time"
)

var i = 0

func incrementing() {
	for j := 1; j <= 1000001; j++ {
		i += 1
	}
}

func decrementing() {
	for j := 1; j <= 1000001; j++ {
		i -= 1
	}
}

func main() {
	// What does GOMAXPROCS do? What happens if you set it to 1?:
	// GOMAXPROCS sets the maximum number of CPUs that can be executing
	// simultaneously and returns the previous setting. It defaults to
	// the value of runtime.NumCPU. If n < 1, it does not change the current setting.
	// This call will go away when the scheduler improves.
	runtime.GOMAXPROCS(2)

	// TODO: Spawn both functions as goroutines

	go incrementing()
	go decrementing()

	// We have no direct way to wait for the completion of a goroutine (without additional synchronization of some sort)
	// We will do it properly with channels soon. For now: Sleep.
	time.Sleep(500 * time.Millisecond)
	Println("The magic number is:", i)
}
