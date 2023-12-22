#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
// Structure to represent a part
struct Part {
    int width, height, price;
    // Custom hash function for symmetric hash
    struct SymmetricHash {
        size_t operator()(const Part& p) const {
            int x = max(p.width, p.height);
            int y = min(p.width, p.height);
                    return x * x + x + y;
        }
    };
    // Custom equality operator for symmetric comparison
    bool operator==(const Part& other) const {
        return (width == other.width && height == other.height) ||
               (width == other.height && height == other.width);
    }
};

int CalculateCuts(int dim) {
    if ((dim - 1) % 2 == 0) {
        return (dim - 1) / 2;
    } else {
        return dim / 2;
    }
}
// Function to calculate the maximum value
int calculateMaxValue(int X, int Y, unordered_map<Part, Part, Part::SymmetricHash>& partMap) {
    // dp[i][j] represents the maximum value obtainable with a sheet of dimensions i x j
    vector<vector<int>> dp(X + 1, vector<int>(Y + 1, 0));

    for (int i = 1; i <= X; i++) {
        for (int l = 1; l <= Y; l++) {
            // Access the part directly from the unordered_map
            auto it = partMap.find({ i, l, 0 });  // The third parameter can be any value since it's not used in comparison
            if (it != partMap.end()) {
                dp[i][l] = it->second.price;
            }

            int VerticalCuts = CalculateCuts(l);
            for (int a = 1; a <= VerticalCuts; a++) {
                dp[i][l] = max(dp[i][l], dp[i][a] + dp[i][l - a]);
            }

            int HorizontalCuts = CalculateCuts(i);
            for (int h = 1; h <= HorizontalCuts; h++) {
                dp[i][l] = max(dp[i][l], dp[h][l] + dp[i - h][l]);
            }
        }
    }
    return dp[X][Y];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    // Read input values
    int X, Y;
    cin >> X >> Y;
    int n;
    cin >> n;
    // Create the unordered_map directly
    unordered_map<Part, Part, Part::SymmetricHash> partMap;
    partMap.reserve(n);
    // Populate the partMap
    for (int i = 0; i < n; ++i) {
        Part originalPart;
        cin >> originalPart.width >> originalPart.height >> originalPart.price;
        partMap[originalPart] = originalPart;
    }
    // Calculate and print the result
    int result = calculateMaxValue(X, Y, partMap);
    printf("%d\n", result);
    return 0;
}