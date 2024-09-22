#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace::std;

void fileRead(vector<string> &transmissionOne, vector<string> &transmissionTwo, string &codeOne, string &codeTwo, string &codeThree){
    ifstream transmissionOneFile("transmission1.txt");
    ifstream transmissionTwoFile("transmission2.txt");
    ifstream codeOneFile("mcode1.txt");
    ifstream codeTwoFile("mcode2.txt");
    ifstream codeThreeFile("mcode3.txt");

    string currentLine;

    while(getline(transmissionOneFile, currentLine)){
        transmissionOne.push_back(currentLine);
    }

    while(getline(transmissionTwoFile, currentLine)){
        transmissionTwo.push_back(currentLine);
    }

    while(getline(codeOneFile, currentLine)){
        codeOne = currentLine;
    }

    while(getline(codeTwoFile, currentLine)){
        codeTwo = currentLine;
    }

    while(getline(codeThreeFile, currentLine)){
        codeThree = currentLine;
    }

    transmissionOneFile.close();
    transmissionTwoFile.close();
    codeOneFile.close();
    codeTwoFile.close();
    codeThreeFile.close();
}

void printFiles(vector<string> transmissionOne, vector<string> transmissionTwo, string codeOne, string codeTwo, string codeThree){
    cout << "TRANSMISSION 1" << endl;

    for(int i = 0; i < transmissionOne.size(); i++){
        cout << transmissionOne[i] << endl;
    }

    cout << "TRANSMISSION 2" << endl;
    for(int i = 0; i < transmissionTwo.size(); i++){
        cout << transmissionTwo[i] << endl;
    }

    cout << "CODE 1: " << codeOne << endl;
    cout << "CODE 2: " << codeTwo << endl;
    cout << "CODE 3: " << codeTwo << endl;

}


vector<int> preKMP(string code){
    int m = code.length(); //Obtenemos el largo de nuestro string
    vector<int> lps(m,0); //Vector LPS inicializado en 0

    int j = 0;
    int i = 1;

    while(i < m){
        if(code[i] == code[j]){
            lps[i] = j+1;
            i++;
            j++;
        } 
        else{ 
            if (j > 0) {
                j = lps[j-1];
            } 
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

void KMP(vector<string> transmission, string code) {
    int n = transmission.size();
    int m = code.length();

    // Generar el vector LPS
    vector<int> lps = preKMP(code);

    // Para cada línea de la transmisión, buscamos el patrón
    for (int t = 0; t < n; t++) {
        string text = transmission[t];
        int i = 0; // índice para text
        int j = 0; // índice para code

        while (i < text.length()) {
            if (code[j] == text[i]) {
                i++;
                j++;
            }

            if (j == m) {
                // Se encontró el patrón en la posición (i-j)
                cout << "Pattern found at index " << i - j << " in line " << t + 1 << endl;
                j = lps[j - 1]; // Continuar buscando otras ocurrencias
            } else if (i < text.length() && code[j] != text[i]) {
                // Desajuste después de j coincidencias
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
    }
}


string preManacher(string code){
    string newString = "";

    for(int i = 0; i < code.size(); i++){
        newString.append("#");
        newString.append(string(1,code[i]));
    } 
    newString.append("#");
    return newString;
}

void manacher(vector<string> transmission) {

    for (int t = 0; t < transmission.size(); t++) {
        // Preprocess the input string
        string T = preManacher(transmission[t]);
        int n = T.length();
        vector<int> P(n, 0); // Array to store the palindrome lengths at each position
        int C = 0, R = 0; // Current center and right boundary of the palindrome

        for (int i = 1; i < n - 1; i++) {
            int mirror = 2 * C - i; // Mirror position of i with respect to center C

            // If i is within the right boundary of the current palindrome
            if (i < R) {
                P[i] = min(R - i, P[mirror]);
            }

            // Attempt to expand the palindrome centered at i
            while (T[i + P[i] + 1] == T[i - P[i] - 1]) {
                P[i]++;
            }

            // Update the center and right boundary if the expanded palindrome is larger
            if (i + P[i] > R) {
                C = i;
                R = i + P[i];
            }
        }

        // Find the maximum palindrome length
        int maxLen = 0;
        int centerIndex = 0;
        for (int i = 1; i < n - 1; i++) {
            if (P[i] > maxLen) {
                maxLen = P[i];
                centerIndex = i;
            }
        }

        cout << "Line: " << t << " Start: " << (centerIndex - maxLen)/2 << " End: " << ((centerIndex - maxLen)/2) + maxLen - 1 << endl;


    }
}

void LCS(vector<string> transmissionOne, vector<string> transmissionTwo) { //Longest Common Substring
    string transmissionOneString = "";
    string transmissionTwoString = "";

    // Concatenate all lines of transmissionOne into a string
    for(int i = 0; i < transmissionOne.size(); i++) {
        transmissionOneString.append(transmissionOne[i]);
    }

    // Same for transmissionTwo
    for(int i = 0; i < transmissionTwo.size(); i++) {
        transmissionTwoString.append(transmissionTwo[i]);
    }

    int n = transmissionOneString.length();
    int m = transmissionTwoString.length();
    int maxSize = 0; // Size of longest common substring
    int endIndex = 0; // End index of longest common substring

    // Create a table to store the lengths of common substrings
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // Fill the table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (transmissionOneString[i - 1] == transmissionTwoString[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxSize) {
                    maxSize = dp[i][j];
                    endIndex = i - 1;
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }


    // Calculate start and end positions
    int startIndex = endIndex - maxSize + 2; 
    int endIndexFinal = endIndex + 1; 

    cout << "Inicial Position: " << startIndex << " Final Position: " << endIndexFinal << endl;

    // Longest common substring
    string longestCommonSubstring = transmissionOneString.substr(endIndex - maxSize + 1, maxSize);

    cout << "Longest Common Substring: " << longestCommonSubstring << endl;
}


int main(){
    vector<string> transmissionOne;
    vector<string> transmissionTwo;
    string codeOne;
    string codeTwo;
    string codeThree;

    fileRead(transmissionOne,transmissionTwo,codeOne,codeTwo,codeThree);
    vector<int> codeOneLps = preKMP(codeOne);

    KMP(transmissionOne, codeOne);

    manacher(transmissionOne);

    manacher(transmissionTwo);

    LCS(transmissionOne, transmissionTwo);

    //cout << preManacher(codeOne);

    //Funcion para imprimir y aseguranos de la integridad de los inputs
    //printFiles(transmissionOne,transmissionTwo,codeOne,codeTwo,codeThree);

    /*
    for(int i = 0; i < codeOneLps.size(); i++){
        cout << codeOneLps[i] << endl;
    }
    */



    return 0;
}