#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    string intToRoman(int num) {
        // Define values and their corresponding Roman numeral symbols in descending order.
        // This includes the special subtractive cases like 900 (CM) and 4 (IV).
        vector<pair<int, string>> roman_map = {
            {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
            {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
            {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
        };
        
        string result = "";
        
        // Iterate through the map
        for (const auto& pair : roman_map) {
            // While the number is greater than or equal to the current value,
            // subtract the value and append the symbol.
            while (num >= pair.first) {
                result += pair.second;
                num -= pair.first;
            }
        }
        
        return result;
    }
};