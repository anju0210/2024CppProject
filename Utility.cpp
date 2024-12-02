#include "Utility.h"

using namespace sf;
using namespace std;

//유사도 측정
int calculateLevenshteinDistance(const string& s1, const string& s2) {
    size_t len1 = s1.size();
    size_t len2 = s2.size();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= len2; ++j) dp[0][j] = j;

    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] }) + 1;
            }
        }
    }
    return dp[len1][len2];
}

// 호버 및 클릭 적용 조건
bool isMouseOver(const Text& text, const RenderWindow& window) {
    Vector2i mousePos = Mouse::getPosition(window);
    Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    return text.getGlobalBounds().contains(mousePosF);
}