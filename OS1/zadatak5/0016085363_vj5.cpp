#include <iostream>
#include <cstdlib>
using namespace std;

class c_lru{
	private:
		int *list;
		int *memory;
		int *time;
		int mem_size;
		int n;
	private:
		void init(){
			cout << "Zahtjevi: ";
			for(int i=0; i<n; i++){
				list[i] = rand()%8+1;
				cout << list[i] << " ";
			}
			cout << endl;
			for(int i=0; i<mem_size; i++) memory[i] = time[i] = -1;
		}
		void print(int hit=0, bool n=0){
			for(int i=0; i<mem_size; i++){
				if(memory[i]==-1) cout << "-";
				else if(n && hit==memory[i]) cout << "[" << memory[i] << "]";
				else if(!n && hit == memory[i]) cout << "(" << memory[i] << ")";
				else cout << memory[i];
				cout <<"\t";
			}
			if(hit>0 && !n) cout << "#pogodak";
			cout << endl;
		}
		int find(int index){
			int min = 0;
			for(int i=0; i<mem_size; i++){
				if(memory[i] == list[index]){
					time[i] = index;
					return -1;
				}
				if(time[i] < time[min]) min = i;
			}
			return min;
		}
		void lru(){
			cout << "#N\t";
			for(int i=1; i<=mem_size; i++) cout << i << "\t";
			cout << endl << "-------";
			for(int i=1; i<=mem_size; i++) cout << "-------";
			cout << endl;
			for(int i=0; i<n; i++){
				cout << list[i] << "\t";
				int index = find(i);
				if(index==-1){
					print(list[i],false);
				} else {
					memory[index] = list[i];
					time[index] = i;
					print(list[i],true);
				}
			}
		}
	public:
		void start(int a, int b){
			mem_size = a;
			n = b;
			list = new int[n];
			time = new int[mem_size];
			memory = new int[mem_size];
			init();
			lru();
		}
}*lru = new c_lru;

int main(int argv, char *argc[]){
	lru->start(atoi(argc[1]), atoi(argc[2]));
	return 0;
}
