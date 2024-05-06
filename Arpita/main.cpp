#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void Decrypt(string inputText, int shift) {

    // Iterate through each character
    for(char &s: inputText) {

        // Check for alphabet
        if(isalpha(s)) {

            // Check for uppercase or lowercase
            if(s<'a') {
                s-=shift;           // Shift ascii value left
                if(s<'A') s+=26;    // If shift preceeds a, cycle back from z
            } else {
                s-=shift;           // Same procedure as above
                if(s<'a') s+=26;
            }
        }
    }

    cout << "The decoded message is " << inputText << "\n";
}

int main() {
    // Open the file
    ifstream file("test.txt");

    // Check if the file is opened successfully
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    // Read and output the contents of the file
    // Read the entire content of the file into a string
    string content;
    char ch;
    while (file.get(ch)) {
        content += ch;
    }

	cout<<"Aru -> "<<content<<endl;
    // Close the file
    file.close();
    
    for(int i = 1; i<26; i++)
    {
    	Decrypt(content,i);
    }
    
    return 0;
    
}
