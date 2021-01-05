cc = g++
std = c++11
prom = target
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o) 

$(prom): $(obj)
	$(cc) -std=$(std) -o $(prom) $(obj) -O2 

%.o: %.cpp $(deps)
	$(cc) -std=$(std) -c $< -o $@ -O2

clean:
	rm -rf $(obj) $(prom)

