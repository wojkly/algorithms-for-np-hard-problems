//
// Created by wojkly on 16.04.22.
//
//

#include <iostream>
#include <queue>
#include <string>
#include <cmath>

using namespace std;

int VP_SIZE = 0;
int height = 0;
int width = 0;
int HW_mul = 0;
int D = 0;
int K = 0;

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

int get_index(vector<pair<int, int>> positions) {
    int index = 0;
    for (int i = 0; i < positions.size(); ++i) {
        index += ipow(HW_mul, i) * (width * positions[i].first + positions[i].second);
    }
    return index;
}

vector<pair<int, int>> get_positions(int index) {
    vector<pair<int, int>> positions;
    int p, x, y;

    while (index > 0) {
        p = index % HW_mul;
        y = p % width;
        x = (p - y) / height;

        positions.emplace_back(x, y);
        index /= HW_mul;
    }
    while (positions.size() < K) {
        positions.emplace_back(0, 0);
    }
    return positions;
}

bool is_distance_greater(vector<pair<int, int>> positions) {
    int x1, x2, y1, y2;

    for (int i = 0; i < positions.size() - 1; ++i) {
        x1 = positions[i].first;
        y1 = positions[i].second;
        for (int j = i + 1; j < positions.size(); ++j) {
            x2 = positions[j].first;
            y2 = positions[j].second;

            if ( sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ) <= D) {
                return false;
            }
        }
    }
    return true;
}

vector<int> get_move_combination(int k) {
    vector<int> subset;
    while (k > 0) {
        subset.push_back(k % 5);
        k /= 5;
    }
    while (subset.size() < K) {
        subset.push_back(0);
    }

    return subset;
}

int main() {
    string line, tmp;

    cin >> tmp;
    height = stoi(tmp);
    cin >> tmp;
    width = stoi(tmp);
    cin >> tmp;
    K = stoi(tmp);
    cin >> tmp;
    D = stoi(tmp);
    cin >> tmp;
//    N = stoi(tmp);

    HW_mul = height * width;
    VP_SIZE = ipow(HW_mul, K);

    bool board[height][width];
    bool visited[VP_SIZE];
    int parent[VP_SIZE];
    for (int i = 0; i < VP_SIZE; ++i) {
        visited[i] = false;
        parent[i] = -1;
    }

    int start = 0, finish = 0;

    for (int i = 0; i < height; ++i) {
        cin >> line;
        for (int j = 0; j < width; ++j) {
            switch (line[j]) {
                case '#':
                    board[i][j] = false;
                    break;
                case '.':
                    board[i][j] = true;
                    break;
                case 'a':
                    start += i * width + j;
                    board[i][j] = true;
                    break;
                case 'A':
                    finish += i * width + j;
                    board[i][j] = true;
                    break;
                case 'b':
                    start += HW_mul * (i * width + j);
                    board[i][j] = true;
                    break;
                case 'B':
                    finish += HW_mul * (i * width + j);
                    board[i][j] = true;
                    break;
                case 'c':
                    start += HW_mul * HW_mul * (i * width + j);
                    board[i][j] = true;
                    break;
                case 'C':
                    finish += HW_mul * HW_mul * (i * width + j);
                    board[i][j] = true;
                    break;
                default:
                    cout << "zly znak";
                    EXIT_FAILURE;
            }
        }
    }

    //BFS
    queue<int> queue;
    queue.push(start);
    int cur_pos;
    int new_pos, new_w, new_h;

    vector<vector<int>> possible_move_combinations;
    for (int i = 1; i < ipow(5, K); ++i) {
        possible_move_combinations.push_back(get_move_combination(i));
    }

    pair<int,int> possible_moves[5];
    possible_moves[0] = pair<int,int>(-1,0);
    possible_moves[1] = pair<int,int>(1,0);
    possible_moves[2] = pair<int,int>(0,-1);
    possible_moves[3] = pair<int,int>(0,1);
    possible_moves[4] = pair<int,int>(0,0);

//    cout << "starting bfs" << endl;
    visited[start] = true;

    int its = 0;
    while (!queue.empty()) {
        its ++;
        cur_pos = queue.front();
        queue.pop();

        if (cur_pos == finish) {
            break;
        }

        bool check_next_combination;
        for (auto possible_move_combination: possible_move_combinations) {
            check_next_combination = false;

            vector<pair<int, int>> coords = get_positions(cur_pos);

            // move each robot
            for (int i = 0; i < K; ++i) {
                pair<int, int> move = possible_moves[possible_move_combination[i]];
                coords[i].first += move.first;
                coords[i].second += move.second;
                new_h = coords[i].first;
                new_w = coords[i].second;

                if (0 <= new_h && new_h < height && 0 <= new_w && new_w < width && board[new_h][new_w]) {
                    check_next_combination = false;
                } else {
                    check_next_combination = true;
                    break;
                }
            }

            if (check_next_combination) {
                continue;
            }

            new_pos = get_index(coords);
            if (!visited[new_pos] && is_distance_greater(coords)) {
                parent[new_pos] = cur_pos;
                visited[new_pos] = true;
                queue.push(new_pos);
            }
        }
    }

//
//    //get path
//    cout << "getting path" << endl;
    string result[K];
    cur_pos = finish;
    int next_pos = parent[cur_pos];
    int h_dif, w_dif, total_diff;

    while(next_pos != -1) {
        vector<pair<int, int>> cur_coords = get_positions(cur_pos);
        vector<pair<int, int>> next_coords = get_positions(next_pos);

        for (int i = 0; i < K; ++i) {
            h_dif = cur_coords[i].first - next_coords[i].first;
            w_dif = (cur_coords[i].second - next_coords[i].second) * 2;
            total_diff = h_dif + w_dif;
            switch (total_diff) {
                case 0:
                    result[i].append("S");
                    break;
                case -1:
                    result[i].append("G");
                    break;
                case 1:
                    result[i].append("D");
                    break;
                case -2:
                    result[i].append("L");
                    break;
                case 2:
                    result[i].append("P");
                    break;
                default:
                    cout << "wrong move detected";
                    EXIT_FAILURE;
            }
        }

//        cout << "cur: " << cur_pos[0] << "," << cur_pos[1] << "next: " << next_pos[0] << "," << next_pos[1] << endl;
        cur_pos = next_pos;
        next_pos = parent[cur_pos];
    }

    for (int i = 0; i < K; ++i) {
        for (int j = result[i].length() - 1; j >= 0; --j) {
            cout << result[i][j];
        }
        cout << endl;
    }


    return EXIT_SUCCESS;
}