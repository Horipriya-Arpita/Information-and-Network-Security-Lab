#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

class Attack {
public:
    const string cipher;
    unordered_map<char, double> freq;
    unordered_map<char, vector<pair<char, double>>> mappings;
    unordered_map<char, char> key;
    unordered_map<char, double> freq_eng;
    string plain_chars_left;
    string cipher_chars_left;
    const string alphabet = "abcdefghijklmnopqrstuvwxyz";


    Attack(const string& cipher) : cipher(cipher) {
        freq_eng = {
            {'a', 0.0805}, {'b', 0.0167}, {'c', 0.0223}, {'d', 0.0510}, {'e', 0.1222}, {'f', 0.0214},
            {'g', 0.0230}, {'h', 0.0662}, {'i', 0.0628}, {'j', 0.0019}, {'k', 0.0095}, {'l', 0.0408},
            {'m', 0.0233}, {'n', 0.0695}, {'o', 0.0763}, {'p', 0.0166}, {'q', 0.0006}, {'r', 0.0529},
            {'s', 0.0602}, {'t', 0.0967}, {'u', 0.0292}, {'v', 0.0082}, {'w', 0.0260}, {'x', 0.0011},
            {'y', 0.0204}, {'z', 0.0006}
        };
        plain_chars_left = "abcdefghijklmnopqrstuvwxyz";
        cipher_chars_left = "abcdefghijklmnopqrstuvwxyz";
    }

    void calculateFreq(const string& cipher) {
        int letter_count = 0;
        for (char letter : alphabet)
            freq[letter] = 0;

        for (char c : cipher) {
            if (c >= 'a' && c <= 'z') {
                freq[c]++;
                letter_count++;
            }
        }

        for (char letter : alphabet)
            freq[letter] = round(freq[letter] / letter_count * 10000) / 10000; // Rounded to 4 decimal places
    }

    void printFreq() {
        for (const auto& [letter, frequency] : freq)
            cout << letter << ": " << frequency << endl;
    }

    void calculateMatches() {
        for (char cipher_char : alphabet) {
            unordered_map<char, double> map;
            for (char plain_char : alphabet) {
                map[plain_char] = round(abs(freq[cipher_char] - freq_eng[plain_char]) * 10000) / 10000; // Rounded to 4 decimal places
            }
            vector<pair<char, double>> sorted_map(map.begin(), map.end());
            sort(sorted_map.begin(), sorted_map.end(), [](const pair<char, double>& a, const pair<char, double>& b) {
                return a.second < b.second;
            });
            mappings[cipher_char] = sorted_map;
        }
    }

    unordered_map<char, char> guessKey() {
        for (char c : cipher_chars_left) {
            for (const auto& [plain_char, diff] : mappings[c]) {
                if (plain_chars_left.find(plain_char) != string::npos) {
                    key[c] = plain_char;
                    plain_chars_left.erase(plain_chars_left.find(plain_char), 1);
                    break;
                }
            }
        }
        return key;
    }

    void setKeyMapping(char cipher_char, char plain_char) {
        if (cipher_chars_left.find(cipher_char) == string::npos || plain_chars_left.find(plain_char) == string::npos) {
            cerr << "ERROR: No Mapping Exists!" << endl;
            exit(-1);
        }
        key[cipher_char] = plain_char;
        cipher_chars_left.erase(cipher_chars_left.find(cipher_char), 1);
        plain_chars_left.erase(plain_chars_left.find(plain_char), 1);
    }

    string decrypt(const unordered_map<char, char>& key, const string& cipher) {
        string message;
        for (char c : cipher) {
            if (key.find(c) != key.end())
                message += key.at(c);
            else
                message += c;
        }
        return message;
    }
};

int main() {

    string cipher = "aceah toz puvg vcdl omj puvg yudqecov, omj loj auum klu thmjuv hs klu zlcvu shv zcbkg guovz, upuv zcmdu lcz vuwovroaeu jczoyyuovomdu omj qmubyudkuj vukqvm. klu vcdluz lu loj avhqnlk aodr svhw lcz kvopuez loj mht audhwu o ehdoe eunumj, omj ck tozyhyqeoveg auecupuj, tlokupuv klu hej sher wcnlk zog, klok klu lcee ok aon umj toz sqee hskqmmuez zkqssuj tckl kvuozqvu. omj cs klok toz mhk umhqnl shv sowu, kluvu toz oezh lczyvhehmnuj pcnhqv kh wovpue ok. kcwu thvu hm, aqk ck zuuwuj kh lopu eckkeu ussudk hmwv. aonncmz. ok mcmukg lu toz wqdl klu zowu oz ok scskg. ok mcmukg-mcmu klug aunom khdoee lcw tuee-yvuzuvpuj; aqk qmdlomnuj thqej lopu auum muovuv klu wovr. kluvu tuvu zhwuklok zlhhr klucv luojz omj klhqnlk klcz toz khh wqdl hs o nhhj klcmn; ck zuuwuj qmsocv klok";
        Attack attack(cipher);

    //
    attack.calculateFreq(attack.cipher);
    attack.printFreq();
    attack.calculateMatches();

    // Mapping improvement
    //attack.setKeyMapping('r','e');
    //attack.setKeyMapping('d','d');
    //attack.setKeyMapping('v','c');
    //attack.setKeyMapping('p','h');
    //attack.setKeyMapping('m','a');
    //attack.setKeyMapping('w','i');
    //attack.setKeyMapping('t','y');
    //attack.setKeyMapping('u','r');
    //attack.setKeyMapping('x','f');
    //attack.setKeyMapping('a','x');
    //attack.setKeyMapping('y','m');
    //attack.setKeyMapping('s','p');
    //attack.setKeyMapping('c','w');
    //attack.setKeyMapping('o','g');
    //attack.setKeyMapping('g','z');
    //attack.setKeyMapping('e','v');
    //attack.setKeyMapping('f','q');

    cout << endl;
    auto key = attack.guessKey();
    cout << endl;
    for (const auto& [cipher_char, plain_char] : key)
        cout << cipher_char << " -> " << plain_char << endl;
    string message = attack.decrypt(key, attack.cipher);
    cout << endl;

    vector<string> cipher_lines;
    vector<string> plain_lines;
    string line;
    stringstream ss(attack.cipher);
    while (getline(ss, line, '\n')) {
        cipher_lines.push_back(line);
    }
    ss.clear();
    ss.str(message);
    while (getline(ss, line, '\n')) {
        plain_lines.push_back(line);
    }
    for (size_t i = 0; i < cipher_lines.size(); ++i) {
        cout << "M: " << plain_lines[i] << endl;
    }

    return 0;
}


