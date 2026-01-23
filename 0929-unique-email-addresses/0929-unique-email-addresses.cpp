#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_set<string> unique_emails;

        for (const string& email : emails) {
            string clean_email;
            
            // 1. Process Local Name
            // We iterate through the email string character by character
            for (char c : email) {
                // Stop local name processing if we hit the domain separator or the ignore flag
                if (c == '@' || c == '+') {
                    break;
                }
                // Skip periods
                if (c == '.') {
                    continue;
                }
                // Build the clean local name
                clean_email += c;
            }

            // 2. Append Domain Name
            // Find where the domain starts (the '@' symbol)
            int at_pos = email.find('@');
            
            // Append everything from '@' to the end of the string
            clean_email += email.substr(at_pos);

            // 3. Insert into Set (handles uniqueness automatically)
            unique_emails.insert(clean_email);
        }

        return unique_emails.size();
    }
};