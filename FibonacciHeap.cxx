#include "FibonacciHeap.hxx"
 

void menu(){
	std::cout<<"\n\t||MENU||\n";
	std::cout<<"0. EXIT\n";
	std::cout<<"1. Generate random data to the heap\n";
	std::cout<<"2. Enter own data to the heap\n";
	std::cout<<"3. Push() operation\n";
	std::cout<<"4. Pop() operation\n";
	std::cout<<"5. Top() operation\n";
	std::cout<<"6. Size() operation\n";
	std::cout<<"7. Decrease-key() operation\n";
	std::cout<<"8. Remove() operation\n";
	std::cout<<"9. Display a graphical representation of the heap in the terminal\n";
	std::cout<<"10. Delete all tree elements\n";
	std::cout<<"[ x < 10 || x < 0 ] MENU\n\n";
}

int main() {

	FibonacciHeap<int> heap;

	int x, min, max, n, key, newKey, v;
	menu();

	while(x != 0){
		std::cout<<"\nOPERATION: ";
		std::cin>>x;
		switch(x){
			case 0:
				break;
			case 1:
				std::cout<<"heap_size >> min_range >> max_range :\n"; 
				std::cin>>n>>min>>max;
				generator(heap, n, min, max);
				break;
			case 2:
				std::cout<<"heap_size: \n";
				std::cin>>n;
				std::cout<<"enter the values: \n";
				for(int i = 0; i < n; i++) {
					std::cin>>v;
					heap.push(v);
				}
				break;
			case 3:
				std::cout<<"enter the key values: \n";
				std::cin>>v;
				heap.push(v);
				break;
			case 4:
				try {
					std::cout<<"smallest key removed: "<<heap.pop()<<'\n';
				}catch (const std::invalid_argument& ia) {
					std::cout<<"empty"<<"\n";
				}
				break;
			case 5:
				std::cout<<"smallest value removed: "<<heap.top()<<'\n';
				break;
			case 6:
				std::cout<<"heap size: "<<heap.size()<<'\n';
				break;
			case 7:
				std::cout<<"key << new_key_value:\n";
				std::cin>>key>>newKey;
				heap.decreaseKey(key, newKey);
				break;
			case 8:
				std::cout<<"enter the key value to delete:\n";
				std::cin>>key;
				heap.remove(key);
				break;
			case 9:
				std::cout<<'\n';
				heap.display();
				break;
			case 10:
				heap.deleteAll();
				std::cout<<"Is the heap empty?"<<((heap.empty())?" YES":" NO")<<'\n';
				break;
			default:
					menu();
					break;
		}
	}

	return 0;
}
 