CFLAGS = -std=c++17 -O2
LDFLAGS = `pkg-config --static --libs glfw3` -lvulkan -ldl -lpthread -lX11 -lXrandr

VulkanTest: *.cpp *.hpp
	g++ $(CFLAGS) -o VulkanTest *.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
