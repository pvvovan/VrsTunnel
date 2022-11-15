#include <iostream>
#include <chrono>
#include <thread>

int main()
{
	for ( ; ; ) {
		for (char i = 0; i < 10; i++) {
			for (int n = 0; n < 5; n++) {
				std::cout << static_cast<char>('0' + i) << std::flush;
			}
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1s);
		}
		std::cout << std::endl;
	}
}
