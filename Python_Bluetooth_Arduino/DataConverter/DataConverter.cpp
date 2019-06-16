#include <iostream>

using namespace std;

int main(){
    int expansion = 3;
    int num = 18;
    int data[num] = {0, 50, 0, 0, 10, 0, 0, 20, 0, 0, 48, 99, 0, -12, 0, 0, 32, 0};
    int data_final[num/expansion];
    for (int i = 0; i < num; i++){
      int sign = 1;
      int current_res = 0;
      for (int j = 0; j < expansion; j++){
        int current_val = data[i*expansion+j];
        if (current_val < 0)
        {
            sign = -1;
            current_val = -current_val;
        }
        current_res = current_res*100;
        current_res += current_val;
      }
      data_final[i] = sign*current_res;
    }
    for (int i = 0; i < num/expansion; i++)
    {
        cout << data_final[i] << " ";
    }
    cout << endl;
    
}