package main

import (
	"fmt"
	"os"
	"strconv"
)

var array [1000000]int64

func doit(arr []int64, from int, to int, c chan int64) {

	sum := int64(0)

	fmt.Println("doing", from, " -- ", to-1)

	sum = (array[from] + array[to-1]) * int64(to-from) / 2

	c <- sum

}

func main() {

	var err error
	var N int

	for i := 0; i < len(array); i++ {
		array[i] = int64(i + 1)
	}

	if len(os.Args) != 2 {
		fmt.Print("number-of-goroutine")
		panic(err)
	}

	if N, err = strconv.Atoi(os.Args[1]); err != nil {
		fmt.Print("Wrong command line argument")
		panic(err)
	}

	fmt.Println("number of goroutines = ", N)

	var portions [1000000]int

	part := len(array) / N

	fmt.Println("slice = ", part)

	for i := 0; i < N; i++ {
		portions[i] = part

	}

	if part*N != len(array) {
		remains := len(array) - part*N
		for i := 0; i < remains; i++ {
			portions[i]++
		}
	}

	start := 0
	all_sum := int64(0)

	for i := 0; i < N; i++ {
		c := make(chan int64)
		go doit(array[0:], start, start+portions[i], c)
		start = start + portions[i]
		sum := <-c
		all_sum = all_sum + sum
	}

	fmt.Println(all_sum)

}
