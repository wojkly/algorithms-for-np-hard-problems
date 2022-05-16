//
// Created by wojkly on 16.04.22.
//
//

#include <iostream>
#include <queue>
#include <string>
#include <cmath>
#include <chrono>
#include <iterator>
#include <map>

using namespace std;

int height = 0;
int width = 0;
int HW_mul = 0;
int HW_mul_sqr = 0;
int D = 0;
int K = 0;
int N = 0;

chrono::time_point<chrono::system_clock, chrono::duration<double>> start_time;

void start_timer() {
    start_time = chrono::system_clock::now();
}

void end_timer() {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start_time;

    cout << "elapsed time: " << elapsed_seconds.count() << endl;
}

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
    int HW_pow = 1;
    for (auto & position : positions) {
        index += HW_pow * (width * position.first + position.second);
        HW_pow *= HW_mul;
    }
    return index;
}

vector<pair<int, int>> get_possible_moves() {
    vector<pair<int,int>> possible_moves;
    possible_moves.emplace_back(0, 0);
    possible_moves.emplace_back(-1, 0);
    possible_moves.emplace_back(1, 0);
    possible_moves.emplace_back(0, -1);
    possible_moves.emplace_back(0, 1);

    return possible_moves;
}

vector<pair<int, int>> get_positions(int index) {
    vector<pair<int, int>> positions;
    int p, x, y;

    for (int i = 0; i < K; ++i) {
        p = index % HW_mul;
        y = p % width;
        x = p / width;

        positions.emplace_back(x, y);
        index /= HW_mul;
    }
    return positions;
}
bool is_passing(vector<pair<int, int>> coords, vector<pair<int, int>> new_coords) {
    if (D != 0) {
        return false;
    } else {
        for (int i = 0; i < K - 1; ++i) {
            auto a1 = coords[i];
            auto a2 = new_coords[i];
            for (int j = i + 1; j < K; ++j) {
                auto b1 = coords[j];
                auto b2 = new_coords[j];

                if (a1 == b2 && b1 == a2) {
                    return true;
                }
            }
        }
        return false;
    }
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
    for (int i = 0; i < K; ++i) {
        subset.push_back(k % 5);
        k /= 5;
    }
    return subset;
}

int main() {
    string line, tmp;

    scanf("%d", &height);
    scanf("%d", &width);
    scanf("%d", &K);
    scanf("%d", &D);
    scanf("%d", &N);
//    N = stoi(tmp);

//    cout << "prepare arrays" << endl;
//    start_timer();
    HW_mul = height * width;
    HW_mul_sqr = HW_mul * HW_mul;

    bool board[height][width];

//    end_timer();
//    cout << "read input" << endl;
//    start_timer();
    int start = 0, finish = 0;
    char field;

    scanf("%c", &field);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            scanf("%c", &field);
            switch (field) {
                case '#':
                    board[i][j] = false;
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
                    start += HW_mul_sqr * (i * width + j);
                    board[i][j] = true;
                    break;
                case 'C':
                    finish += HW_mul_sqr * (i * width + j);
                    board[i][j] = true;
                    break;
                default:
                    board[i][j] = true;
                    break;
            }
        }
        scanf("%c", &field);
    }

    //BFS
    queue<int> queue;
    queue.push(start);
    int cur_pos;
    int new_pos, new_w, new_h;


//    end_timer();
//    cout << "COMBINATIONS" << endl;
//    start_timer();
    //
    vector<vector<int>> possible_move_combinations;
    possible_move_combinations.reserve(ipow(5, K));
    for (int i = 1; i< ipow(5, K); ++i) {
        possible_move_combinations.push_back(get_move_combination(i));
    }
    vector<pair<int,int>> possible_moves = get_possible_moves();

//    end_timer();
//    cout << "BFS" << endl;
//    start_timer();
//    cout << "starting bfs" << endl;
    map<int, int> parents;
    parents.emplace(start, -1);

    int its = 0;
    bool finish_visited = false;
    while (!queue.empty()) {
        its++;
        cur_pos = queue.front();
        queue.pop();

        if (cur_pos == finish) {
            break;
        }

        bool check_next_combination;
        vector<pair<int, int>> coords = get_positions(cur_pos);

        for (auto possible_move_combination: possible_move_combinations) {
            check_next_combination = false;

            vector<pair<int, int>> new_coords;
            // move each robot
            for (int i = 0; i < K; ++i) {
                pair<int, int> move = possible_moves[possible_move_combination[i]];
                new_coords.emplace_back(coords[i].first + move.first, coords[i].second + move.second);
                new_h = new_coords[i].first;
                new_w = new_coords[i].second;

                if (0 <= new_h && new_h < height && 0 <= new_w && new_w < width && board[new_h][new_w]) {
                } else {
                    check_next_combination = true;
                    break;
                }
            }

            if (check_next_combination) {
                continue;
            }

            new_pos = get_index(new_coords);
            if (parents.find(new_pos) == parents.end() && !is_passing(coords, new_coords) && is_distance_greater(new_coords)) {
                parents.emplace(new_pos, cur_pos);
                queue.push(new_pos);

                if (new_pos == finish) {
//                    cout << "done" << endl;
                    finish_visited = true;
                    break;
                }
            }
        }

        if (finish_visited) {
            break;
        }
    }

//    end_timer();
//    cout << "get path" << endl;
//    start_timer();
//
//    //get path
//    cout << "getting path" << endl;
    string result[K];
    cur_pos = finish;
    int next_pos = parents[cur_pos];
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
            }
        }

//        cout << "cur: " << cur_pos[0] << "," << cur_pos[1] << "next: " << next_pos[0] << "," << next_pos[1] << endl;
        cur_pos = next_pos;
        next_pos = parents[cur_pos];
    }

//    end_timer();
//    cout << "get result" << endl;
//    start_timer();
    for (int i = 0; i < K; ++i) {
        for (int j = result[i].length() - 1; j >= 0; --j) {
            cout << result[i][j];
        }
        cout << endl;
    }
//    end_timer();


    return EXIT_SUCCESS;
}