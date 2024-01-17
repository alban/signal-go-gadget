// Copyright 2021-2024 The Inspektor Gadget authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"runtime"
	"time"

	_ "github.com/alban/signal-go-gadget"
)

func main() {
	fmt.Printf("pid %d\n", os.Getpid())
	time.Sleep(5 * time.Second)
	go func() {
		fmt.Printf("Open /dev/null\n")
		file, err := os.Open("/dev/null")
		if err != nil {
			panic(err)
		}
		content, err := ioutil.ReadAll(file)
		if err != nil {
			panic(err)
		}
		fmt.Printf("%s", string(content))
		fmt.Printf("Calling runtime.GC\n")
		runtime.GC()
		fmt.Printf("Calling runtime.GC done\n")
	}()
	time.Sleep(50 * time.Millisecond)
	fmt.Printf("ReadFile /dev/null\n")
	_, _ = ioutil.ReadFile("/dev/null")
	fmt.Printf("ReadFile /dev/null done\n")
}
