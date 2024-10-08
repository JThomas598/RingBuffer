CC := g++
CXX_FLAGS := -Wall -Werror --std=c++17

app: main.o
	$(CC) $^ -o $@

main.o: main.cpp
	$(CC) $(CXX_FLAGS) -c $< -o $@

%.o: %.cpp %.h
	$(CC) $(CXX_FLAGS) -c $< -o $@


clean:
	rm *.o app

