#include <iostream>
#include <thread> 
#include <string>
#include "RingBuffer.h"

unsigned long sum = 0;

void consumer_task(long int n, RingBuffer<int>& ring){
	for(unsigned int i = 0; i < n; i++){
		sum += ring.pop();
	}
}

void producer_task(long int n, RingBuffer<int>& ring){
	for(unsigned int i = 0; i < n; i++){
		ring.push(i);
	}
}

void print_usage(){
	std::cout << "usage: app <num_productions> <buffer_size>" << std::endl;
}

int main(int argc, char* argv[]){
	if(argc < 3 || argc > 3){ 
		std::cout << "error: invaid number of arguments provided" << std::endl;
		print_usage();
		return 1;
	}

	long int size;
	long int n;

	try{
		n = std::stol(std::string(argv[1]));
		size = std::stol(std::string(argv[2]));
	}
	catch(std::invalid_argument& e){
		std::cout << "error: non-numerical argument cannot be parsed." << std::endl;
		print_usage();
		return 1;
	}

	if(size <= 0){
		std::cout << "error: invalid buffer length" << std::endl;
		print_usage();
		return 1;
	}

	 if(n < 0){
		 std::cout << "error: invalid number of iterations" << std::endl;
		 print_usage();
		 return 1;
	 }

	RingBuffer<int> ring(size);
	std::thread producer(&producer_task, n, std::ref(ring));
	std::thread consumer(&consumer_task, n, std::ref(ring));

	producer.join();
	consumer.join();

	std::cout << sum << std::endl; 
	return 0;
}
